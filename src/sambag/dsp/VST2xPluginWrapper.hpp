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

#include <AudioEffectX.h>
#include <string>
#include <sstream>
#include "DspPlugin.hpp"
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
/**
 * @class VST2xPluginWrapper
 * is a wrapper for AudioEffectX classes. 

 * Main purpose of this pattern: use a independent Pluginclass - PluginProcessor - 
 * which can be used by different wrapper classes 
 * such as VST2xPluginWrapper or AUPluginWrapper.
 */
template <
	class _PluginProcessor,
	int _UniqueId,
	class _PluginTraits = StdPluginTraits<2,2,false>,
	class _CreateEditorPolicy = CreateNoEditor,
	class _ConstructorPolicy = StdConstructPlolicy> 
class VST2xPluginWrapper :
	public AudioEffectX, 
	public _PluginProcessor,
	public _PluginTraits,
	public _CreateEditorPolicy,
	public _ConstructorPolicy,
	public IHost
{
//=============================================================================
friend struct FriendBuilder<_ConstructorPolicy>::Type;
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
	enum {UniqueId = _UniqueId};
protected:
private:	
public:
	//-------------------------------------------------------------------------
	VstInt32 canDo (char *text ) {
		if (!strcmp (text, "receiveVstEvents") )
			return 1;
		if (!strcmp (text, "receiveVstMidiEvent") )
			return 1;
		return -1;	// explicitly can't do; 0 => don't know
	}
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
		vst_strncpy ( outLabel , label.c_str(), label.length() );
	}
	//-------------------------------------------------------------------------
	void getParameterDisplay(VstInt32 index, char * outDisplay) {
		typename PluginTraits::StringType display;
		PluginProcessor::getParameterDisplay(index, display);
		vst_strncpy ( outDisplay , display.c_str(), display.length() );
	}
	//-------------------------------------------------------------------------
	void getParameterName(VstInt32 index, char * outName) {
		typename PluginTraits::StringType name;
		PluginProcessor::getParameterName(index, name);
		vst_strncpy ( outName , name.c_str(), name.length() );
	}
	//-------------------------------------------------------------------------
	VST2xPluginWrapper(audioMasterCallback audioMaster) : 
	  AudioEffectX(audioMaster, 
		PluginTraits::NumProgram,
		PluginTraits::NumParameter
	){
		PluginProcessor::setHost(this);
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
		PluginProcessor::processEvents(events);
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
	// host impl.
	//-------------------------------------------------------------------------
	virtual void configurationChanged() {
		AudioEffectX::setInitialDelay( PluginProcessor::getLatency() );
		AudioEffectX::ioChanged();
	}
	//-------------------------------------------------------------------------
	virtual void parameterChanged(int index) {
		typename PluginTraits::ParameterType value;
		getParameterValue(index, value);
		setParameterAutomated(index, value);
	}
};
//#############################################################################
//	Impl.:
//#############################################################################
//-----------------------------------------------------------------------------
}}} // namespace

#endif // VST2XPLUGIN_WRAPPER_HPP_