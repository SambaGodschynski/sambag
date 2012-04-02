/*
 * VST2xPluginWrapper.hpp
 *
 *  Created on: 02 April 2012
 *      Author: samba
 */
#ifndef VST2XPLUGIN_WRAPPER_HPP_
#define VST2XPLUGIN_WRAPPER_HPP_

#include <AudioEffectX.h>
#include <string>
#include <sstream>
#include "DspPlugin.hpp"
namespace sambag { namespace dsp { namespace vst {
//=============================================================================
/**
 * @class VST2xPluginWrapper
 * is a wrapper for AudioEffectX classes. 

 * Main purpose of this pattern: use a universal Pluginclass 
  * - PluginProcessor - which can be used
 * by different wrapper classes such as VST2xPluginWrapper or AUPluginWrapper.
 */
template <
	class _PluginProcessor,
	int _UniqueId,
	class _PluginTraits = StdPluginTraits<2,2,false>,
	class _CreateEditorPolicy = CreateNoEditor > 
class VST2xPluginWrapper :
	public AudioEffectX, 
	public _PluginProcessor,
	public _PluginTraits,
	public _CreateEditorPolicy
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef _PluginProcessor PluginProcessor;
	//-------------------------------------------------------------------------
	typedef _PluginTraits PluginTraits;
	//-------------------------------------------------------------------------
	typedef _CreateEditorPolicy CreateEditorPolicy;
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
		typename PluginProcessor::setParameterValue(index, value);
	}
	//-------------------------------------------------------------------------
	float getParameter (VstInt32 index) {
		PluginTraits::ParameterType value = 0;
		typename PluginProcessor::getParameterValue(index, value);
		return value;
	}
	//-------------------------------------------------------------------------
	void getParameterLabel(VstInt32 index, char * outLabel) {
		PluginTraits::StringType label;
		typename PluginProcessor::getParameterLabel(index, label);
		vst_strncpy ( outLabel , label.c_str(), label.length() );
	}
	//-------------------------------------------------------------------------
	void getParameterDisplay(VstInt32 index, char * outDisplay) {
		PluginTraits::StringType display;
		typename PluginProcessor::getParameterDisplay(index, display);
		vst_strncpy ( outDisplay , display.c_str(), display.length() );
	}
	//-------------------------------------------------------------------------
	void getParameterName(VstInt32 index, char * outName) {
		PluginTraits::StringType name;
		typename PluginProcessor::getParameterName(index, name);
		vst_strncpy ( outName , name.c_str(), name.length() );
	}
	//-------------------------------------------------------------------------
	VST2xPluginWrapper(audioMasterCallback audioMaster) : 
	  AudioEffectX(audioMaster, 
		typename PluginTraits::NumProgram,
		typename PluginTraits::NumParameter
	) {

		setNumInputs (typename PluginTraits::NumInputs); 
		setNumOutputs (typename PluginTraits::NumOutputs); 
		canProcessReplacing ();
		programsAreChunks(true);
		noTail (false);
		isSynth(typename PluginTraits::IsSynth);

		setEditor (
			typename CreateEditorPolicy::createEditor<AEffEditor>(this)
		);
		setUniqueID(UniqueId);
		resume();
	}
	//-------------------------------------------------------------------------
	~VST2xPluginWrapper() {}
	//-------------------------------------------------------------------------
	void processReplacing(float **in, float **out, VstInt32 numSamples) {
		typename PluginProcessor::process(in, out, numSamples);
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
};
//#############################################################################
//	Impl.:
//#############################################################################
//-----------------------------------------------------------------------------
}}} // namespace

#endif // VST2XPLUGIN_WRAPPER_HPP_