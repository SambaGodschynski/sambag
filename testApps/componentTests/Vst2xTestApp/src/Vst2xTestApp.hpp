/*
 * RemoteChannelMain.hpp
 *
 *  Created on: 02 April 2012
 *      Author: samba
 */
#ifndef DISCO_VST2XTESTAPP_HPP_
#define DISCO_VST2XTESTAPP_HPP_

#include <sambag/dsp/DspPlugin.hpp>
#include <string>

//=============================================================================
class DiscoVst2xTestApp : public sambag::dsp::PluginProcessorBase {
//=============================================================================
private:
public:
	//-------------------------------------------------------------------------
	~DiscoVst2xTestApp();
	//-------------------------------------------------------------------------
	DiscoVst2xTestApp();
	//-------------------------------------------------------------------------
	void process(float **in, float**out, int numSamples);
	//-------------------------------------------------------------------------
	void setParameterValue(int index, float value) {
	}
	//-------------------------------------------------------------------------
	void getParameterValue(int index, float &outValue) {
	}
};

#endif //VSTFORX_MAIN_HPP_