/*
 * HostTimeInfo.hpp
 *
 *  Created on: Thu Oct 18 10:35:04 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_HOSTTIMEINFO_H
#define SAMBAG_HOSTTIMEINFO_H

#include <boost/shared_ptr.hpp>

namespace sambag { namespace dsp {
//=============================================================================
/** 
  * @class HostTimeInfo.
  */
struct HostTimeInfo {
//=============================================================================
	enum Filter {
		FrxTempo = 1,
		FrxPpqPos = 1<<1,
	};
	double tempo;
	double sampleRate; // always valid
	double ppqPos;
	bool transportIsPlaying;
};
}} // namespace(s)

#endif /* SAMBAG_HOSTTIMEINFO_H */
