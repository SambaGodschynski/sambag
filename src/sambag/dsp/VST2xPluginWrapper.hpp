/*
 * VST2xPluginWrapper.hpp
 *
 *  Created on: 02 April 2012
 *      Author: samba
 */
#ifndef VST2XPLUGIN_WRAPPER_HPP_
#define VST2XPLUGIN_WRAPPER_HPP_

namespace {
	/**
	 * gcc template as friend workaround.
	 */
	template <typename T>
	struct FriendBuilder { typedef T Type; }; 
}

// TODO: find a better way (maybe discart friend approach)
#ifdef _MSC_VER
#define SAMBAG_FRIEND_OF_T(_class) friend typename _class
#elif __APPLE_CC__ >= 6000
#define SAMBAG_FRIEND_OF_T(_class) friend _class
#else
#define SAMBAG_FRIEND_OF_T(_class) friend class FriendBuilder<_class>::Type
#endif

#include <audioeffectx.h>
#include <string>
#include <sstream>
#include "VstMidiEventAdapter.hpp"
#include "DspPlugin.hpp"
#include <loki/Typelist.h>
#include <algorithm>
#include "TimeInfoVst2xHelper.hpp"

namespace sambag { namespace dsp { namespace vst {
//=============================================================================
struct StdConstructPlolicy {
	template <class VSTPlugin, class PluginTraits>
	void construct(VSTPlugin &plug) {
		plug.setNumInputs (PluginTraits::NumInputs); 
		plug.setNumOutputs (PluginTraits::NumOutputs); 
		plug.canProcessReplacing ();
		plug.programsAreChunks(true);
		plug.noTail (false);
		plug.isSynth(PluginTraits::IsSynth);
	}
};
//=============================================================================
// CanDo Tags
struct CanReceiveVstEventTag {
	static const char * name() { return "receiveVstEvents"; }
};
struct CanReceiveVstMidiEventTag {
	static const char * name() { return "receiveVstMidiEvent"; }
};
//=============================================================================
/**
 * @class VST2xPluginWrapper
 * is a wrapper for AudioEffectX classes. 

 * Main purpose of this pattern: use a independent Pluginclass - PluginProcessor - 
 * which can be used by different wrapper classes 
 * such as VST2xPluginWrapper or AUPluginWrapper.
 * Example:
 * typedef VST2xPluginWrapper<
 *		frx::processing::VstForxPlug, // Processor
 *		'frxr', // uid
 *		sambag::dsp::StdPluginTraits< // plug traits (ins, outs, isInstrument, numParameter)
 *			numInputs, numOutputs, isIsntrument, numProgramParameter
 *		>, 
 *		frx::gui::components::CreateEditor // editor creator
 *	> Plugin;
 */
template <
	class _PluginProcessor,
	int _UniqueId,
	class _PluginTraits = StdPluginTraits<2,2,false>,
	class _CreateEditorPolicy = CreateNoEditor,
	class _ConstructorPolicy = StdConstructPlolicy,
	class _CanDoTagList = LOKI_TYPELIST_2(
		CanReceiveVstEventTag, 
		CanReceiveVstMidiEventTag)
> 
class VST2xPluginWrapper :
	public AudioEffectX, 
	public _PluginProcessor,
	public _PluginTraits,
	public _CreateEditorPolicy,
	public _ConstructorPolicy,
	public IHost
{
//=============================================================================
SAMBAG_FRIEND_OF_T(_ConstructorPolicy);
public:
	//-------------------------------------------------------------------------
	typedef _PluginProcessor PluginProcessor;
	//-------------------------------------------------------------------------
	typedef _PluginTraits PluginTraits;
	//-------------------------------------------------------------------------
	typedef _CreateEditorPolicy CreateEditorPolicy;
	//-------------------------------------------------------------------------
	typedef _ConstructorPolicy ConstructorPolicy;
	//-------------------------------------------------------------------------
	typedef _CanDoTagList CanDoTagList;
	//-------------------------------------------------------------------------
	enum {UniqueId = _UniqueId};
protected:
private:	
	//-------------------------------------------------------------------------
	HostTimeInfo timeInfo;
public:
	//-------------------------------------------------------------------------
	VstInt32 canDo (char *text );
	//-------------------------------------------------------------------------
	void setParameter (VstInt32 index, float value) {
		PluginProcessor::setParameterValue(index, value);
	}
	//-------------------------------------------------------------------------
	float getParameter (VstInt32 index) {
		typename PluginTraits::ParameterType value = 0;
		PluginProcessor::getParameterValue(index, value);
		return value;
	}
	//-------------------------------------------------------------------------
	void getParameterLabel(VstInt32 index, char * outLabel) {
		typename PluginTraits::StringType label;
		PluginProcessor::getParameterLabel(index, label);
		vst_strncpy ( outLabel , label.c_str(), std::min((size_t)8, label.length()) );
	}
	//-------------------------------------------------------------------------
	void getParameterDisplay(VstInt32 index, char * outDisplay) {
		typename PluginTraits::StringType display;
		PluginProcessor::getParameterDisplay(index, display);
		vst_strncpy ( outDisplay , display.c_str(), std::min((size_t)24, display.length()) );
	}
	//-------------------------------------------------------------------------
	void getParameterName(VstInt32 index, char * outName) {
		typename PluginTraits::StringType name;
		PluginProcessor::getParameterName(index, name);
		vst_strncpy ( outName , name.c_str(), std::min((size_t)16, name.length()) );
	}
	//-------------------------------------------------------------------------
	VST2xPluginWrapper(audioMasterCallback audioMaster) : 
	  AudioEffectX(audioMaster, 
		PluginTraits::NumProgram,
		PluginTraits::NumParameter
	){
		PluginProcessor::setHost(this);
		PluginProcessor::setPluginTraits( PluginTraits() );
		ConstructorPolicy::template construct<AudioEffectX, PluginTraits>(*this);
		setEditor (
			CreateEditorPolicy::template createEditor<AEffEditor>(this)
		);
		setUniqueID(UniqueId);
		resume();
	}
	//-------------------------------------------------------------------------
	~VST2xPluginWrapper() {}
	//-------------------------------------------------------------------------
	void processReplacing(float **in, float **out, VstInt32 numSamples) {
		PluginProcessor::process(in, out, numSamples);
	}
	//-------------------------------------------------------------------------
	void setSampleRate(float sampleRate) {
		PluginProcessor::setSampleRate(sampleRate);
	}
	//-------------------------------------------------------------------------
	void setBlockSize (VstInt32 blockSize) {
		PluginProcessor::setBlockSize(blockSize);
	}
	//-------------------------------------------------------------------------
	void open() {
		PluginProcessor::open();
	}
	//-------------------------------------------------------------------------
	void close() {
		AudioEffectX::close();
		PluginProcessor::close();
	}
	//-------------------------------------------------------------------------
	VstInt32 processEvents(VstEvents * events) {
		if (!events) {
			return 0;
		}
		if (events->numEvents == 0) {
			return 1;
		}
        SAMBAG_LOG_TRACE << " BEFORE " << *events;
		VstMidiEventAdapter::Ptr midiev = VstMidiEventAdapter::create(events);
		PluginProcessor::processEvents(midiev);
		return 1;
	}
	//-------------------------------------------------------------------------
	void suspend(){
		AudioEffectX::suspend();
		PluginProcessor::suspend();
	}
	//-------------------------------------------------------------------------
	void resume(){
		AudioEffectX::resume();
		PluginProcessor::resume();
	}
	//-------------------------------------------------------------------------
	VstInt32 getChunk(void **data, bool isPreset) {
		return PluginProcessor::getChunk(data);
	}
	//-------------------------------------------------------------------------
	VstInt32 setChunk(void *data, VstInt32 byteSize, bool isPreset) {
		return PluginProcessor::setChunk(data, byteSize);
	}
	//-------------------------------------------------------------------------
	// host impl.
	virtual void delayChanged(int delaySamples) {
		AudioEffectX::setInitialDelay( delaySamples );
		AudioEffectX::ioChanged();
	}
	//-------------------------------------------------------------------------
	virtual void ioConfigurationChanged(int numInputs, int numOutputs) {
		AudioEffectX::setNumInputs(numInputs);
		AudioEffectX::setNumOutputs(numOutputs);
		AudioEffectX::ioChanged();
	}
	//-------------------------------------------------------------------------
	virtual void parameterChanged(int index) {
		typename PluginTraits::ParameterType value;
		this->getParameterValue(index, value);
        beginEdit(index);
		setParameterAutomated(index, value);
        endEdit(index);
	}
	//-------------------------------------------------------------------------
	HostTimeInfo * getHostTimeInfoImpl (int filter);
	//-------------------------------------------------------------------------
	virtual bool requestEditorResize(int width, int height) {
		if (!AudioEffectX::canHostDo ("sizeWindow")) {
			return false;
		}
		return AudioEffectX::sizeWindow(width, height);
	}
	//-------------------------------------------------------------------------
	virtual void * getEditor() const {
            return editor;
    }
	//-------------------------------------------------------------------------
	virtual bool getVendorString (char* text) { 
		typename PluginTraits::StringType name;
		PluginProcessor::getVendor(name);
		vst_strncpy ( text , name.c_str(), std::min((size_t)64, name.length()) );
		return true; 
	}
	//-------------------------------------------------------------------------
	virtual bool getProductString (char* text) { 
		typename PluginTraits::StringType name;
		PluginProcessor::getProductName(name);
		vst_strncpy ( text , name.c_str(), std::min((size_t)64, name.length()) );
		return true; 
	}
	//-------------------------------------------------------------------------
	virtual VstInt32 getVendorVersion () { 
		return PluginProcessor::getProductVersion(); 
	}
    //-------------------------------------------------------------------------
	virtual size_t getNumInputs() const {
        return (size_t)AudioEffectX::cEffect.numInputs;
    }
    //-------------------------------------------------------------------------
	virtual size_t getNumOutputs() const {
        return (size_t)AudioEffectX::cEffect.numOutputs;
    }
    //-------------------------------------------------------------------------
	virtual size_t getNumParameter() const {
        return (size_t)AudioEffectX::cEffect.numParams;
    }
};
//#############################################################################
//	Impl.:
//#############################################################################
//-----------------------------------------------------------------------------
template <class P, int U, class T , class E , class C, class CD> 
HostTimeInfo * 
VST2xPluginWrapper<P,U,T,E,C,CD>::getHostTimeInfoImpl (int filter)
{
	// convert filter
	VstTimeInfoFlags f =
        timeInfoVst2xHelper::toVst2xFilter((HostTimeInfo::Filter)filter);
    timeInfo = HostTimeInfo();
	VstTimeInfo * vstTime = AudioEffectX::getTimeInfo(f);
	if (!vstTime) 
		return &timeInfo;
    
    timeInfoVst2xHelper::convert(timeInfo, *vstTime);

	return &timeInfo;
}
//-----------------------------------------------------------------------------
namespace {
	// return true if one of the CanDo list compares to sbj
	template <class CanDo>
	bool _can(char *sbj) {
		return strcmp(sbj, CanDo::Head::name())==0 ||
			_can<typename CanDo::Tail>(sbj);
	}
	template<>
	bool _can< ::Loki::NullType>(char*) {
		return false;
	}
}
template <class P, int U, class T , class E , class C, class CD> 
int VST2xPluginWrapper<P,U,T,E,C,CD>::canDo(char *sbj) {
	// explicitly can't do; 0 => don't know
	return _can<CanDoTagList>(sbj) ? 1 : -1;
}
}}} // namespace

#endif // VST2XPLUGIN_WRAPPER_HPP_
