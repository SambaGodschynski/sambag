/*
 * WindowFlags.hpp
 *
 *  Created on: Mon Aug  6 13:14:06 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_WINDOWFLAGS
#define SAMBAG_WINDOWFLAGS

namespace sambag { namespace disco { namespace components {

class WindowFlags {
public:
	typedef unsigned int FlagType;
private:
	FlagType flags;
public:
	enum Flag {
		WND_FRAMED     = 1 << 0,
		WND_RESIZEABLE = 1 << 1,
		WND_NESTED     = 1 << 2,		    // is true when windowImpl. is nested in a host window.
		WND_RAW        = 1 << 3,			// create raw window without disco rendering
		WND_NO_SYSTEM_MENU = 1 << 4,
		WND_ALWAYS_ON_TOP  = 1 << 5,
        WND_VST2X_CARBON_COCOA_HACK = 1 << 6, // on OsX Vst2x we use either Carbon or
                                              // Cocoa (as Raw) dependend on the used architecture
                                              // with this flag we handle this.
		EXIT_ON_CLOSE = 1 << 7
	};
	WindowFlags() : flags(0) {}
	void setFlag(Flag aFlag, bool b) {
		if (b) {
			flags |= (aFlag);
		} else {
			flags &= ~(aFlag);
		}
	}
	bool getFlag(Flag aFlag) const {
		unsigned int mask = ((FlagType)aFlag);
		return ((flags & mask) == mask);
	}
	void clearAllFlags() {
		flags = 0;
	}
	void setFlags(unsigned int flags) {
		this->flags = flags;
	}
};

}}} // namespaces

#endif // SAMBAG_WINDOWFLAGS