/*
 * ICommand.hpp
 *
 *  Created on: Mon Jun  4 10:49:53 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ICOMMAND_H
#define SAMBAG_ICOMMAND_H

#include <boost/function.hpp>

namespace sambag { namespace com {

//=============================================================================
/** 
  * @class ICommand.
  * Interface for a Command Object.
  */
struct ICommand {
//=============================================================================
	//-------------------------------------------------------------------------
	typedef boost::function<void()> Function;

}; // ICommand
}} // namespace(s)

#endif /* SAMBAG_ICOMMAND_H */
