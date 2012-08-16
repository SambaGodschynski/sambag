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
		WND_FRAMED,
		WND_RESIZEABLE,
		WND_NESTED		// is true when windowImpl. is nested in a host window. 
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
};

}}} // namespaces

#endif // SAMBAG_WINDOWFLAGS