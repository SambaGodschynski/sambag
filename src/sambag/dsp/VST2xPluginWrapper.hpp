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

namespace sambag { namespace dsp { namespace vst {
//=============================================================================
struct StdParameterAcessor {
	template <class Plugin, typename T>
	void setParameterValue(const Plugin &plugin, int index, const T &value){}
	template <class Plugin, typename T>
	void getParameterValue(const Plugin &plugin, int index, T &value){}
	template <class Plugin, class String>
	void getParameterName (const Plugin &plugin, int index, String &outStr){}
	template <class Plugin, class String>
	void getParameterLabel(const Plugin &plugin, int index, String &outStr){}
	template <class Plugin, class String>
	void getParameterDisplay(const Plugin &plugin, int index, String &outStr){}
};
//=============================================================================
template <int _NumInputs,
	int _NumOutputs,
	bool _IsSynth,
	int _NumParameter = 0>
struct StdPluginTraits {
	enum { 
		NumInputs = _NumInputs,
		NumOutputs = _NumOutputs,
		NumProgram = 0,
		NumParameter = _NumParameter,
		IsSynth = _IsSynth
	};
	typedef float ParameterType;
	typedef std::string StringType;
};

//=============================================================================
struct CreateNoEditor {
	template <class Editor, class T>
	Editor * createEditor(T*) { return NULL; }
};

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
	class _ParameterAcessor = StdParameterAcessor,
	class _CreateEditorPolicy = CreateNoEditor > 
class VST2xPluginWrapper :
	public AudioEffectX, 
	public _PluginProcessor,
	public _PluginTraits,
	public _CreateEditorPolicy,
	public _ParameterAcessor
{
//=============================================================================
friend typename _ParameterAcessor;
public:
	//-------------------------------------------------------------------------
	typedef _PluginProcessor PluginProcessor;
	//-------------------------------------------------------------------------
	typedef _PluginTraits PluginTraits;
	//-------------------------------------------------------------------------
	typedef _CreateEditorPolicy CreateEditorPolicy;
	//-------------------------------------------------------------------------
	typedef _ParameterAcessor ParameterAcessor;
	//-------------------------------------------------------------------------
	enum {UniqueId = _UniqueId};
protected:
private:	
public:
	//-------------------------------------------------------------------------
	void setParameter (VstInt32 index, float value) {
		typename ParameterAcessor::setParameterValue(*this, index, value);
	}
	//-------------------------------------------------------------------------
	float getParameter (VstInt32 index) {
		PluginTraits::ParameterType value = 0;
		typename ParameterAcessor::getParameterValue(*this, index, value);
		return value;
	}
	//-------------------------------------------------------------------------
	void getParameterLabel(VstInt32 index, char * outLabel) {
		PluginTraits::StringType label;
		typename ParameterAcessor::getParameterLabel(*this, index, label);
		vst_strncpy ( outLabel , label.c_str(), label.length() );
	}
	//-------------------------------------------------------------------------
	void getParameterDisplay(VstInt32 index, char * outDisplay) {
		PluginTraits::StringType display;
		typename ParameterAcessor::getParameterDisplay(*this, index, display);
		vst_strncpy ( outDisplay , display.c_str(), display.length() );
	}
	//-------------------------------------------------------------------------
	void getParameterName(VstInt32 index, char * outName) {
		PluginTraits::StringType name;
		typename ParameterAcessor::getParameterName(*this, index, name);
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
};
//#############################################################################
//	Impl.:
//#############################################################################
//-----------------------------------------------------------------------------
}}} // namespace

#endif // VST2XPLUGIN_WRAPPER_HPP_