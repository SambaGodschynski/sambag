/*
 * TimeInfoVst3xHelper.hpp
 *
 *  Created on: Fri Feb 13 21:11:50 2015
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TIMEINFOVST3XHELPER_H
#define SAMBAG_TIMEINFOVST3XHELPER_H

#include "HostTimeInfo.hpp"
#include <ivstprocesscontext.h>
#include <string>

namespace sambag { namespace dsp {
//=============================================================================
namespace timeInfoVst3xHelper {
    typedef Steinberg::Vst::ProcessContext::StatesAndFlags StateAndFlags;
    StateAndFlags toVst3xFilter(int filter);
    HostTimeInfo::Filter toHostTimeFilter(int filter);
    void convert(HostTimeInfo &dst, const Steinberg::Vst::ProcessContext &src);
    void convert(Steinberg::Vst::ProcessContext &dst, const HostTimeInfo &src);
    std::string toString(StateAndFlags filter);
    std::string toString(const Steinberg::Vst::ProcessContext &inf);
}; // TimeEventVst3xHelper

}} // namespace(s)

#endif /* SAMBAG_TIMEINFOVST3XHELPER_H */
