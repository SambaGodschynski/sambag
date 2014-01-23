/*
 * TimeEventVst2xHelper.hpp
 *
 *  Created on: Thu Jan 23 12:33:01 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TIMEEVENTVST2XHELPER_H
#define SAMBAG_TIMEEVENTVST2XHELPER_H

#include "HostTimeInfo.hpp"
#include <aeffectx.h>
#include <string>

namespace sambag { namespace dsp {
//=============================================================================
namespace timeInfoVst2xHelper {
    VstTimeInfoFlags toVst2xFilter(int filter);
    HostTimeInfo::Filter toHostTimeFilter(int filter);
    void convert(HostTimeInfo &dst, const VstTimeInfo &src);
    void convert(VstTimeInfo &dst, const HostTimeInfo &src);
    std::string toString(VstTimeInfoFlags filter);
    std::string toString(HostTimeInfo::Filter filter);
    std::string toString(const HostTimeInfo &inf);
    std::string toString(const VstTimeInfo &inf);
}; // TimeEventVst2xHelper
}} // namespace(s)

#endif /* SAMBAG_TIMEEVENTVST2XHELPER_H */
