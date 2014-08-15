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
	enum Flag {			// !there is no bitshifting, thats ok! see impl. 
		WND_FRAMED,
		WND_RESIZEABLE,
		WND_NESTED,		    // is true when windowImpl. is nested in a host window. 
		WND_RAW,			// create raw window without disco rendering
		WND_NO_SYSTEM_MENU,
		WND_ALWAYS_ON_TOP,
		EXIT_ON_CLOSE
	};
	WindowFlags() : flags(0) {}
	void setFlag(Flag aFlag, bool b) {
		if (b) {
			flags |= (1 << aFlag);
		} else {
			flags &= ~(1 << aFlag);
		}
	}
	bool getFlag(Flag aFlag) const {
		unsigned int mask = (1 << (FlagType)aFlag);
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