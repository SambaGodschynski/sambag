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
namespace sambag { namespace dsp { 
//=============================================================================
/** 
 * @classe IHost
 * PluginProcessor -> Wrapper calls.
 */
struct IHost {
//=============================================================================
	virtual void configurationChanged() = 0;
	virtual void parameterChanged(int index) = 0;
	virtual HostTimeInfo * getHostTimeInfo (int filter) = 0;
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
	void setSampleRate(float blockSize){}
	int getLatency() const {return 0;}
	void open(){}
	void close(){}
	void suspend(){}
	void resume(){}
	// will be setted by client wrapper.
	void setHost(IHost *_host) { host = _host; }
	IHost * getHost() const { return host; }
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