/*
 * Timer.cpp
 *
 *  Created on: Mon Jun 25 10:35:04 2012
 *      Author: Johannes Unger
 */

#include "Timer.hpp"
#include "WindowToolkit.hpp"

namespace sambag { namespace disco { namespace components {
template <>
void __startTimer<Timer>(const Timer &tm) {
     getWindowToolkit()->startTimer(tm.getPtr());
}
template <>
void __stopTimer<Timer>(const Timer &tm) {
     getWindowToolkit()->stopTimer(tm.getPtr());
}
}}} // namespace(s)
