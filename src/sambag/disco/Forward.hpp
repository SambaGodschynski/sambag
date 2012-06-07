/*
 * Forward.hpp
 *
 *  Created on: Thu Jun  7 11:02:27 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_FORWARD_H
#define SAMBAG_FORWARD_H

#include <boost/shared_ptr.hpp>

namespace sambag { namespace disco {
class AWindow; typedef boost::shared_ptr<AWindow> AWindowPtr;
class AFramedWindow; typedef boost::shared_ptr<AFramedWindow> AFramedWindowPtr;
}} // namespace(s)

#endif /* SAMBAG_FORWARD_H */
