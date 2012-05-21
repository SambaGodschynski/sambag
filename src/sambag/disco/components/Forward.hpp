/*
 * Forward.hpp
 *
 *  Created on: May 21, 2012
 *      Author: samba
 */

#ifndef FORWARD_HPP_
#define FORWARD_HPP_

#include <boost/shared_ptr.hpp>

//=============================================================================
// Forward declarations of some important classes
//=============================================================================
namespace sambag { namespace disco { namespace components {
	class AComponent; typedef boost::shared_ptr<AComponent> AComponentPtr;
	class AContainer; typedef boost::shared_ptr<AContainer> AContainerPtr;
}}}

#endif /* FORWARD_HPP_ */
