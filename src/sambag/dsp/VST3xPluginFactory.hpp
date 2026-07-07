/*
 * VST3xPluginFactory.hpp
 *
 * Minimal IPluginFactory implementation (no public.sdk required).
 *
 * Usage:
 *
 *   // In exactly ONE .cpp file per plug-in binary:
 *   #include <sambag/dsp/VST3xPluginWrapper.hpp>
 *   #include <sambag/dsp/VST3xPluginFactory.hpp>
 *
 *   SAMBAG_VST3_EXPORT_FACTORY(
 *       MyPlugin,          // VST3xPluginWrapper<...> typedef
 *       "\x01\x23...",     // 16-byte TUID string literal
 *       "Acme Corp",       // vendor
 *       "https://acme.com",
 *       "info@acme.com",
 *       "My Plugin",       // class name visible in host
 *       "Fx"               // sub-category (kVstAudioEffectClass sub-type)
 *   )
 */

#ifndef SAMBAG_VST3XPLUGINFACTORY_HPP_
#define SAMBAG_VST3XPLUGINFACTORY_HPP_

#include "VST3xPluginWrapper.hpp"   // brings in tuidEqual/tuidCopy and all VST3 interfaces

#include <atomic>
#include <cstring>

namespace sambag { namespace dsp { namespace vst {

//=============================================================================
/**
 * @class VST3xPluginFactory
 *
 * Implements IPluginFactory for a single VST3 plug-in class.
 * Template parameters:
 *   Plugin  – the VST3xPluginWrapper<...> specialisation
 */
template <class Plugin>
class VST3xPluginFactory : public Steinberg::IPluginFactory
{
//=============================================================================
    std::atomic<Steinberg::int32> _refCount{1};

    // ── factory meta data set by SAMBAG_VST3_EXPORT_FACTORY ───────────────
    const char* _vendor;
    const char* _url;
    const char* _email;
    const char* _className;
    const char* _subCategory;
    Steinberg::TUID _classId;

    // ── FUnknown ─────────────────────────────────────────────────────────
    Steinberg::uint32 PLUGIN_API addRef() override {
        return static_cast<Steinberg::uint32>(++_refCount);
    }
    Steinberg::uint32 PLUGIN_API release() override {
        auto rc = --_refCount;
        if (rc == 0) { delete this; return 0; }
        return static_cast<Steinberg::uint32>(rc < 0 ? 0 : rc);
    }
    Steinberg::tresult PLUGIN_API queryInterface(const Steinberg::TUID iid, void** obj) override {
        if (!obj) return Steinberg::kInvalidArgument;
        if (tuidEqual(iid, IPluginFactory::iid) || tuidEqual(iid, Steinberg::FUnknown::iid)) {
            *obj = static_cast<IPluginFactory*>(this);
            addRef();
            return Steinberg::kResultOk;
        }
        *obj = nullptr;
        return Steinberg::kNoInterface;
    }

    // ── IPluginFactory ───────────────────────────────────────────────────
    Steinberg::tresult PLUGIN_API getFactoryInfo(Steinberg::PFactoryInfo* info) override {
        if (!info) return Steinberg::kInvalidArgument;
        Steinberg::strncpy8(info->vendor, _vendor, Steinberg::PFactoryInfo::kNameSize);
        Steinberg::strncpy8(info->url,    _url,    Steinberg::PFactoryInfo::kURLSize);
        Steinberg::strncpy8(info->email,  _email,  Steinberg::PFactoryInfo::kEmailSize);
        info->flags = Steinberg::PFactoryInfo::kUnicode;
        return Steinberg::kResultOk;
    }

    Steinberg::int32 PLUGIN_API countClasses() override { return 1; }

    Steinberg::tresult PLUGIN_API getClassInfo(Steinberg::int32 index,
                                               Steinberg::PClassInfo* info) override {
        if (index != 0 || !info) return Steinberg::kResultFalse;
        tuidCopy(info->cid, _classId);
        info->cardinality = Steinberg::PClassInfo::kManyInstances;
        Steinberg::strncpy8(info->category, kVstAudioEffectClass,
                            Steinberg::PClassInfo::kCategorySize);
        Steinberg::strncpy8(info->name, _className, Steinberg::PClassInfo::kNameSize);
        return Steinberg::kResultOk;
    }

    Steinberg::tresult PLUGIN_API createInstance(Steinberg::FIDString cid,
                                                  Steinberg::FIDString riid,
                                                  void** obj) override {
        if (!obj) return Steinberg::kInvalidArgument;
        if (std::memcmp(cid, _classId, sizeof(Steinberg::TUID)) != 0) {
            *obj = nullptr;
            return Steinberg::kResultFalse;
        }
        Plugin* instance = new Plugin();
        Steinberg::tresult res = instance->queryInterface(riid, obj);
        instance->release(); // factory ref; queryInterface added its own ref
        return res;
    }

public:
    VST3xPluginFactory(const char* vendor, const char* url, const char* email,
                       const Steinberg::TUID classId,
                       const char* className, const char* subCategory)
        : _vendor(vendor), _url(url), _email(email)
        , _className(className), _subCategory(subCategory)
    {
        tuidCopy(_classId, classId);
    }

    virtual ~VST3xPluginFactory() = default;
};

}}} // namespace sambag::dsp::vst

// ─────────────────────────────────────────────────────────────────────────────
// SAMBAG_VST3_EXPORT_FACTORY
//
// Place this macro in exactly ONE .cpp file to export GetPluginFactory().
//
//   SAMBAG_VST3_EXPORT_FACTORY(
//       MyPluginType,
//       "\x01\x23\x45\x67\x89\xab\xcd\xef\x01\x23\x45\x67\x89\xab\xcd\xef",
//       "Vendor Name", "https://vendor.com", "info@vendor.com",
//       "My Plugin Name", "Fx"
//   )
// ─────────────────────────────────────────────────────────────────────────────
#define SAMBAG_VST3_EXPORT_FACTORY(PluginType, classIdStr,          \
    vendor, url, email, className, subCategory)                      \
extern "C" {                                                         \
SMTG_EXPORT_SYMBOL Steinberg::IPluginFactory* PLUGIN_API            \
GetPluginFactory()                                                   \
{                                                                    \
    static const Steinberg::TUID _cid =                             \
        { classIdStr[0],  classIdStr[1],  classIdStr[2],  classIdStr[3],  \
          classIdStr[4],  classIdStr[5],  classIdStr[6],  classIdStr[7],  \
          classIdStr[8],  classIdStr[9],  classIdStr[10], classIdStr[11], \
          classIdStr[12], classIdStr[13], classIdStr[14], classIdStr[15] };\
    return new sambag::dsp::vst::VST3xPluginFactory<PluginType>(    \
        vendor, url, email, _cid, className, subCategory);          \
}                                                                    \
}

#endif // SAMBAG_VST3XPLUGINFACTORY_HPP_
