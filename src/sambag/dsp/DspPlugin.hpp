/*
 * DspPlugin.hpp
 *
 *  Created on: 02 April 2012
 *      Author: samba
 */
#ifndef DSP_PLUGIN_HPP_
#define DSP_PLUGIN_HPP_

#include <string>
#include <sstream>
#include "HostTimeInfo.hpp"
#include "IMidiEvents.hpp"
#include "IEditor.hpp"
namespace sambag { namespace dsp { 
//=============================================================================
/** 
 * @classe IHost
 * PluginProcessor -> Wrapper communication.
 */
struct IHost {
//=============================================================================
	virtual void delayChanged(int delaySamples) = 0;
	virtual void ioConfigurationChanged(int numInputs, int numOutputs) = 0;
	virtual void parameterChanged(int index) = 0;
	virtual HostTimeInfo * getHostTimeInfo (int filter) = 0;
	/**
	 * @return true if succeed.
	 */
	virtual bool requestEditorResize(int witdh, int height) = 0;
	/**
	 * @return editor. Is NULL if no editor which extends IEditor is used.
	 */
	virtual IEditor * getEditor() const = 0;
};	
//=============================================================================
class PluginProcessorBase {
public:
	template <typename ParameterType>
	void setParameterValue(int index, const ParameterType &value){}
	template <typename ParameterType> 
	void getParameterValue(int index, ParameterType &value) const {}
	template <class String> 
	void getParameterName (int index, String &outStr) const {}
	template <class String> 
	void getParameterLabel(int index, String &outStr) const {}
	template <class String> 
	void getParameterDisplay(int index, String &outStr) const {}
	void processEvents(IMidiEvents *ev) {}
	void setBlockSize(int blockSize){}
	void process(float **in, float **out, int numSamples){}
	void setSampleRate(float blockSize){}
	int getLatency() const {return 0;}
	void open(){}
	void close(){}
	void suspend(){}
	void resume(){}
	int getChunk(void **data) { 
		static char dummy[] = "\0";
		data[0] = (void*)&dummy[0];
		return 1; 
	}
	int setChunk(void *data, int byteSize) { 
		return 1; 
	}
	// will be setted by client wrapper.
	void setHost(IHost *_host) { host = _host; }
	IHost * getHost() const { return host; }
	template <class String> 
	void getVendor(String &outStr) const {}
	template <class String> 
	void getProductName(String &outStr) const {} 
	int getProductVersion() const { return 0; }
protected:
	IHost *host;
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
	template <class Editor, class Plugin>
	Editor * createEditor(Plugin*) { return NULL; }
};
}} // namespace

#endif // DSP_PLUGIN_HPP_