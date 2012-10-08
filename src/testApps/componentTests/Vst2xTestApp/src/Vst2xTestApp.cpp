#include "Vst2xTestApp.hpp"
#include <stdlib.h>
#include <sambag/dsp/VST2xPluginWrapper.hpp>
#include <sstream>
#include "TestAppEditor.hpp"

namespace {
//=============================================================================
// class CreateDiscoEditor
//=============================================================================
struct CreateDiscoEditor {
	template <class Editor, class Plugin>
	Editor * createEditor(Plugin* aeff) { return new TestAppEditor(aeff); }
};
} // namespace

//=============================================================================
// class DiscoVst2xTestApp 
//=============================================================================
//-----------------------------------------------------------------------------
DiscoVst2xTestApp::DiscoVst2xTestApp()
{
}
//-----------------------------------------------------------------------------
DiscoVst2xTestApp::~DiscoVst2xTestApp() {
}
//-----------------------------------------------------------------------------
void DiscoVst2xTestApp::process(float **in, float **out, int numSamples) {
}
//-----------------------------------------------------------------------------
AudioEffect * createEffectInstance ( audioMasterCallback audioMaster ) {
#if _WIN32
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); //VS memory tracking
#endif

	using namespace sambag::dsp::vst;
	// settingup plugin
	typedef VST2xPluginWrapper<
		DiscoVst2xTestApp, // Processor
		'frxr', // uid
		sambag::dsp::StdPluginTraits<2,2,false,0>, // plugin constructor
		CreateDiscoEditor
	> Plugin;
	// create plugin
	try{
		return new Plugin(audioMaster);
	} catch(...) {
		return NULL;
	}
}
