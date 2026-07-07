/*
 * VST3xPluginWrapper.hpp
 *
 * Single-component VST3 wrapper template.
 * Implements IComponent + IAudioProcessor + IEditController in one object,
 * using only pluginterfaces (no public.sdk required).
 *
 * Usage (in your plug-in's GetPluginFactory.cpp):
 *
 *   #include <sambag/dsp/VST3xPluginWrapper.hpp>
 *   #include <sambag/dsp/VST3xPluginFactory.hpp>
 *
 *   typedef sambag::dsp::vst::VST3xPluginWrapper<
 *       frx::processing::VstForxPlug,
 *       sambag::dsp::StdPluginTraits<2, 4, false, 0>,
 *       frx::gui::components::CreateVstForxEditor
 *   > Plugin;
 *
 *   SAMBAG_VST3_EXPORT_FACTORY(
 *       Plugin,
 *       "\x01\x23\x45\x67\x89\x01\x23\x45\x67\x89\x01\x23\x45\x67\x89\x01", // component TUID
 *       "VSTForx", "https://vstforx.de", "info@vstforx.de",
 *       "VSTForx", "Fx"
 *   )
 */

#ifndef SAMBAG_VST3XPLUGINWRAPPER_HPP_
#define SAMBAG_VST3XPLUGINWRAPPER_HPP_

#include <atomic>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/base/ipluginbase.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/fstrdefs.h"
#include "pluginterfaces/vst/ivstcomponent.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstprocesscontext.h"
#include "pluginterfaces/vst/vstspeaker.h"
#include "pluginterfaces/vst/vsttypes.h"
#include "pluginterfaces/vst/ivstunits.h"

#include "DspPlugin.hpp"
// TimeInfoVst3xHelper.hpp uses legacy CLIBS include paths; include the
// VST3 process context directly for the new pluginterfaces submodule layout.
#include "pluginterfaces/vst/ivstprocesscontext.h"

namespace sambag { namespace dsp { namespace vst {

//=============================================================================
// TUID helpers
//=============================================================================
inline bool tuidEqual(const Steinberg::TUID a, const Steinberg::TUID b) {
    return std::memcmp(a, b, sizeof(Steinberg::TUID)) == 0;
}

inline void tuidCopy(Steinberg::TUID dst, const Steinberg::TUID src) {
    std::memcpy(dst, src, sizeof(Steinberg::TUID));
}

//=============================================================================
// IBStream helpers (read/write chunk data)
//=============================================================================
namespace detail {

inline bool ibstreamReadAll(Steinberg::IBStream* s, std::vector<char>& out) {
    out.clear();
    if (!s) return false;
    Steinberg::int64 startPos = 0;
    s->tell(&startPos);
    Steinberg::int64 endPos = 0;
    s->seek(0, Steinberg::IBStream::kIBSeekEnd, &endPos);
    s->seek(startPos, Steinberg::IBStream::kIBSeekSet, nullptr);
    Steinberg::int32 len = static_cast<Steinberg::int32>(endPos - startPos);
    if (len <= 0) return false;
    out.resize(static_cast<size_t>(len));
    Steinberg::int32 bytesRead = 0;
    s->read(out.data(), len, &bytesRead);
    out.resize(static_cast<size_t>(bytesRead));
    return bytesRead > 0;
}

inline bool ibstreamWriteAll(Steinberg::IBStream* s, const void* data, int byteSize) {
    if (!s || !data || byteSize <= 0) return false;
    Steinberg::int32 written = 0;
    return s->write(const_cast<void*>(data), byteSize, &written) == Steinberg::kResultOk
        && written == byteSize;
}

} // namespace detail

//=============================================================================
/**
 * @class VST3xPluginWrapper
 *
 * Single-component VST3 plug-in wrapper.
 *
 * Template parameters mirror VST2xPluginWrapper:
 *   _PluginProcessor    – your processor class (derives from PluginProcessorBase)
 *   _PluginTraits       – StdPluginTraits<numIns, numOuts, isSynth, numParams>
 *   _CreateEditorPolicy – policy with createEditor<IPlugView>(Plugin*) or CreateNoEditor
 */
template <
    class _PluginProcessor,
    class _PluginTraits        = StdPluginTraits<2, 2, false>,
    class _CreateEditorPolicy  = CreateNoEditor
>
class VST3xPluginWrapper :
    public Steinberg::Vst::IComponent,       // IPluginBase -> FUnknown
    public Steinberg::Vst::IAudioProcessor,  // FUnknown
    public Steinberg::Vst::IEditController,  // IPluginBase -> FUnknown
    public _PluginProcessor,
    public IHost
{
//=============================================================================
public:
    typedef _PluginProcessor    PluginProcessor;
    typedef _PluginTraits       PluginTraits;
    typedef _CreateEditorPolicy CreateEditorPolicy;

private:
    std::atomic<Steinberg::int32> _refCount{1};
    Steinberg::Vst::IComponentHandler* _componentHandler{nullptr};
    HostTimeInfo _hostTimeInfo;

    // ─────────────────────────────────────────────────────────────────────
    // FUnknown — overrides every vtable slot (all three base-class copies)
    // ─────────────────────────────────────────────────────────────────────
    Steinberg::uint32 PLUGIN_API addRef() override {
        return static_cast<Steinberg::uint32>(++_refCount);
    }

    Steinberg::uint32 PLUGIN_API release() override {
        Steinberg::int32 rc = --_refCount;
        if (rc == 0) {
            delete this;
            return 0;
        }
        return static_cast<Steinberg::uint32>(rc < 0 ? 0 : rc);
    }

    Steinberg::tresult PLUGIN_API queryInterface(const Steinberg::TUID iid, void** obj) override {
        using namespace Steinberg;
        using namespace Steinberg::Vst;
        if (!obj) return kInvalidArgument;

        if (tuidEqual(iid, IComponent::iid)) {
            *obj = static_cast<IComponent*>(this);
        } else if (tuidEqual(iid, IAudioProcessor::iid)) {
            *obj = static_cast<IAudioProcessor*>(this);
        } else if (tuidEqual(iid, IEditController::iid)) {
            *obj = static_cast<IEditController*>(this);
        } else if (tuidEqual(iid, IPluginBase::iid)) {
            *obj = static_cast<IComponent*>(this);
        } else if (tuidEqual(iid, FUnknown::iid)) {
            *obj = static_cast<IComponent*>(this);
        } else {
            *obj = nullptr;
            return kNoInterface;
        }
        addRef();
        return kResultOk;
    }

    // ─────────────────────────────────────────────────────────────────────
    // IPluginBase (shared by IComponent and IEditController)
    // Both declare the same virtual signature; one override satisfies both.
    // ─────────────────────────────────────────────────────────────────────
    Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* context) override {
        PluginProcessor::setHost(this);
        PluginProcessor::setPluginTraits(PluginTraits{});
        PluginProcessor::open();
        PluginProcessor::resume();
        return Steinberg::kResultOk;
    }

    Steinberg::tresult PLUGIN_API terminate() override {
        PluginProcessor::suspend();
        PluginProcessor::close();
        if (_componentHandler) {
            _componentHandler->release();
            _componentHandler = nullptr;
        }
        return Steinberg::kResultOk;
    }

    // ─────────────────────────────────────────────────────────────────────
    // IComponent
    // ─────────────────────────────────────────────────────────────────────
    Steinberg::tresult PLUGIN_API getControllerClassId(Steinberg::TUID classId) override {
        // Single-component: the host discovers IEditController via queryInterface.
        // Returning kResultFalse signals "I am my own controller".
        std::memset(classId, 0, sizeof(Steinberg::TUID));
        return Steinberg::kResultFalse;
    }

    Steinberg::tresult PLUGIN_API setIoMode(Steinberg::Vst::IoMode /*mode*/) override {
        return Steinberg::kResultOk;
    }

    Steinberg::int32 PLUGIN_API getBusCount(Steinberg::Vst::MediaType type,
                                             Steinberg::Vst::BusDirection dir) override {
        using namespace Steinberg::Vst;
        if (type == kAudio) return 1;
        if (type == kEvent && dir == kInput && PluginTraits::IsSynth) return 1;
        return 0;
    }

    Steinberg::tresult PLUGIN_API getBusInfo(Steinberg::Vst::MediaType type,
                                              Steinberg::Vst::BusDirection dir,
                                              Steinberg::int32 index,
                                              Steinberg::Vst::BusInfo& bus) override {
        using namespace Steinberg;
        using namespace Steinberg::Vst;
        if (index != 0) return kResultFalse;
        bus.mediaType  = type;
        bus.direction  = dir;
        bus.busType    = kMain;
        bus.flags      = BusInfo::kDefaultActive;
        if (type == kAudio) {
            bus.channelCount = (dir == kInput)
                               ? PluginTraits::NumInputs
                               : PluginTraits::NumOutputs;
            str8ToStr16(bus.name, (dir == kInput) ? "Audio Input" : "Audio Output", 128);
        } else {
            bus.channelCount = 16;
            str8ToStr16(bus.name, "Event Input", 128);
        }
        return kResultOk;
    }

    Steinberg::tresult PLUGIN_API getRoutingInfo(Steinberg::Vst::RoutingInfo& /*inInfo*/,
                                                  Steinberg::Vst::RoutingInfo& /*outInfo*/) override {
        return Steinberg::kNotImplemented;
    }

    Steinberg::tresult PLUGIN_API activateBus(Steinberg::Vst::MediaType /*type*/,
                                               Steinberg::Vst::BusDirection /*dir*/,
                                               Steinberg::int32 /*index*/,
                                               Steinberg::TBool /*state*/) override {
        return Steinberg::kResultOk;
    }

    Steinberg::tresult PLUGIN_API setActive(Steinberg::TBool state) override {
        if (state) PluginProcessor::resume();
        else       PluginProcessor::suspend();
        return Steinberg::kResultOk;
    }

    // IComponent::setState / IComponent::getState
    // IEditController declares the same signatures — one override covers both.
    Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state) override {
        std::vector<char> buf;
        if (!detail::ibstreamReadAll(state, buf)) return Steinberg::kResultFalse;
        PluginProcessor::setChunk(buf.data(), static_cast<int>(buf.size()));
        return Steinberg::kResultOk;
    }

    Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state) override {
        void* data = nullptr;
        int size = PluginProcessor::getChunk(&data);
        if (size <= 0 || !data) return Steinberg::kResultFalse;
        return detail::ibstreamWriteAll(state, data, size)
               ? Steinberg::kResultOk : Steinberg::kResultFalse;
    }

    // ─────────────────────────────────────────────────────────────────────
    // IAudioProcessor
    // ─────────────────────────────────────────────────────────────────────
    Steinberg::tresult PLUGIN_API setBusArrangements(
        Steinberg::Vst::SpeakerArrangement* /*inputs*/,  Steinberg::int32 /*numIns*/,
        Steinberg::Vst::SpeakerArrangement* /*outputs*/, Steinberg::int32 /*numOuts*/) override {
        return Steinberg::kResultOk;
    }

    Steinberg::tresult PLUGIN_API getBusArrangement(Steinberg::Vst::BusDirection dir,
                                                     Steinberg::int32 index,
                                                     Steinberg::Vst::SpeakerArrangement& arr) override {
        using namespace Steinberg::Vst;
        if (index != 0) return Steinberg::kResultFalse;
        if (dir == kInput)
            arr = (PluginTraits::NumInputs == 1)
                  ? SpeakerArr::kMono : SpeakerArr::kStereo;
        else
            arr = (PluginTraits::NumOutputs == 1)
                  ? SpeakerArr::kMono : SpeakerArr::kStereo;
        return Steinberg::kResultOk;
    }

    Steinberg::tresult PLUGIN_API canProcessSampleSize(Steinberg::int32 symbolicSampleSize) override {
        return symbolicSampleSize == Steinberg::Vst::kSample32
               ? Steinberg::kResultOk : Steinberg::kResultFalse;
    }

    Steinberg::uint32 PLUGIN_API getLatencySamples() override {
        return static_cast<Steinberg::uint32>(PluginProcessor::getLatency());
    }

    Steinberg::tresult PLUGIN_API setupProcessing(Steinberg::Vst::ProcessSetup& setup) override {
        PluginProcessor::setSampleRate(static_cast<float>(setup.sampleRate));
        PluginProcessor::setBlockSize(setup.maxSamplesPerBlock);
        return Steinberg::kResultOk;
    }

    Steinberg::tresult PLUGIN_API setProcessing(Steinberg::TBool /*state*/) override {
        return Steinberg::kResultOk;
    }

    Steinberg::tresult PLUGIN_API process(Steinberg::Vst::ProcessData& data) override {
        // Parameter changes from host automation
        if (data.inputParameterChanges) {
            Steinberg::int32 n = data.inputParameterChanges->getParameterCount();
            for (Steinberg::int32 i = 0; i < n; ++i) {
                Steinberg::Vst::IParamValueQueue* q =
                    data.inputParameterChanges->getParameterData(i);
                if (!q) continue;
                Steinberg::int32 numPts = q->getPointCount();
                if (numPts == 0) continue;
                Steinberg::int32 offset = 0;
                Steinberg::Vst::ParamValue value = 0.0;
                q->getPoint(numPts - 1, offset, value);
                PluginProcessor::setParameterValue(
                    static_cast<int>(q->getParameterId()),
                    static_cast<typename PluginTraits::ParameterType>(value));
            }
        }

        // MIDI / event input
        if (data.inputEvents) {
            // Use the existing IMidiAdapter stub; concrete MIDI conversion can be
            // added here once the VSTforx event bridge is wired up.
            // PluginProcessor::processEvents(...);
        }

        // Time info — copy relevant fields from VST3 ProcessContext
        if (data.processContext) {
            const Steinberg::Vst::ProcessContext& pc = *data.processContext;
            _hostTimeInfo.tempo      = pc.tempo;
            _hostTimeInfo.sampleRate = pc.sampleRate;
        }

        // Audio processing
        if (data.numSamples > 0 && data.numInputs > 0 && data.numOutputs > 0) {
            float** in  = data.inputs[0].channelBuffers32;
            float** out = data.outputs[0].channelBuffers32;
            PluginProcessor::process(in, out, data.numSamples);
        }

        return Steinberg::kResultOk;
    }

    Steinberg::uint32 PLUGIN_API getTailSamples() override {
        return Steinberg::Vst::kNoTail;
    }

    // ─────────────────────────────────────────────────────────────────────
    // IEditController
    // ─────────────────────────────────────────────────────────────────────
    Steinberg::tresult PLUGIN_API setComponentState(Steinberg::IBStream* state) override {
        // Sync controller with the component's persisted state
        return setState(state);
    }

    // setState / getState already overridden above (shared with IComponent).

    Steinberg::int32 PLUGIN_API getParameterCount() override {
        return static_cast<Steinberg::int32>(PluginTraits::NumParameter);
    }

    Steinberg::tresult PLUGIN_API getParameterInfo(Steinberg::int32 paramIndex,
                                                    Steinberg::Vst::ParameterInfo& info) override {
        if (paramIndex < 0 || paramIndex >= PluginTraits::NumParameter)
            return Steinberg::kResultFalse;

        typename PluginTraits::StringType name;
        PluginProcessor::getParameterName(paramIndex, name);

        info.id                   = static_cast<Steinberg::Vst::ParamID>(paramIndex);
        info.stepCount            = 0;
        info.defaultNormalizedValue = 0.0;
        info.unitId               = Steinberg::Vst::kRootUnitId;
        info.flags                = Steinberg::Vst::ParameterInfo::kCanAutomate;

        Steinberg::str8ToStr16(info.title,      name.c_str(), 128);
        Steinberg::str8ToStr16(info.shortTitle, name.c_str(), 128);
        info.units[0] = 0;

        return Steinberg::kResultOk;
    }

    Steinberg::tresult PLUGIN_API getParamStringByValue(Steinberg::Vst::ParamID id,
                                                         Steinberg::Vst::ParamValue /*valueNormalized*/,
                                                         Steinberg::Vst::String128 string) override {
        typename PluginTraits::StringType display;
        PluginProcessor::getParameterDisplay(static_cast<int>(id), display);
        Steinberg::str8ToStr16(string, display.c_str(), 128);
        return Steinberg::kResultOk;
    }

    Steinberg::tresult PLUGIN_API getParamValueByString(Steinberg::Vst::ParamID /*id*/,
                                                         Steinberg::Vst::TChar* /*string*/,
                                                         Steinberg::Vst::ParamValue& /*valueNormalized*/) override {
        return Steinberg::kNotImplemented;
    }

    Steinberg::Vst::ParamValue PLUGIN_API normalizedParamToPlain(Steinberg::Vst::ParamID /*id*/,
                                                                   Steinberg::Vst::ParamValue v) override {
        return v;
    }

    Steinberg::Vst::ParamValue PLUGIN_API plainParamToNormalized(Steinberg::Vst::ParamID /*id*/,
                                                                   Steinberg::Vst::ParamValue v) override {
        return v;
    }

    Steinberg::Vst::ParamValue PLUGIN_API getParamNormalized(Steinberg::Vst::ParamID id) override {
        typename PluginTraits::ParameterType value = 0;
        PluginProcessor::getParameterValue(static_cast<int>(id), value);
        return static_cast<Steinberg::Vst::ParamValue>(value);
    }

    Steinberg::tresult PLUGIN_API setParamNormalized(Steinberg::Vst::ParamID id,
                                                      Steinberg::Vst::ParamValue value) override {
        PluginProcessor::setParameterValue(
            static_cast<int>(id),
            static_cast<typename PluginTraits::ParameterType>(value));
        return Steinberg::kResultOk;
    }

    Steinberg::tresult PLUGIN_API setComponentHandler(Steinberg::Vst::IComponentHandler* handler) override {
        if (_componentHandler) _componentHandler->release();
        _componentHandler = handler;
        if (_componentHandler) _componentHandler->addRef();
        return Steinberg::kResultOk;
    }

    Steinberg::IPlugView* PLUGIN_API createView(Steinberg::FIDString name) override {
        if (name && std::strcmp(name, Steinberg::Vst::ViewType::kEditor) == 0) {
            return CreateEditorPolicy::template createEditor<Steinberg::IPlugView>(this);
        }
        return nullptr;
    }

    // ─────────────────────────────────────────────────────────────────────
    // IHost implementation
    // ─────────────────────────────────────────────────────────────────────
    void delayChanged(int /*delaySamples*/) override {}

    void ioConfigurationChanged(int /*numInputs*/, int /*numOutputs*/) override {}

    void parameterChanged(int index) override {
        if (!_componentHandler) return;
        typename PluginTraits::ParameterType value = 0;
        PluginProcessor::getParameterValue(index, value);
        auto id = static_cast<Steinberg::Vst::ParamID>(index);
        _componentHandler->beginEdit(id);
        _componentHandler->performEdit(id,
            static_cast<Steinberg::Vst::ParamValue>(value));
        _componentHandler->endEdit(id);
    }

    HostTimeInfo* getHostTimeInfoImpl(int filter) override {
        return &_hostTimeInfo;
    }

    bool requestEditorResize(int /*width*/, int /*height*/) override { return false; }
    void* getEditor() const override { return nullptr; }
    size_t getNumInputs()    const override { return PluginTraits::NumInputs;    }
    size_t getNumOutputs()   const override { return PluginTraits::NumOutputs;   }
    size_t getNumParameter() const override { return PluginTraits::NumParameter; }

public:
    VST3xPluginWrapper() = default;
    virtual ~VST3xPluginWrapper() = default;
};

}}} // namespace sambag::dsp::vst

#endif // SAMBAG_VST3XPLUGINWRAPPER_HPP_
