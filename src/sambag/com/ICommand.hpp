/*
 * ICommand.hpp
 *
 *  Created on: Mon Jun  4 10:49:53 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ICOMMAND_H
#define SAMBAG_ICOMMAND_H

#include <boost/shared_ptr.hpp>

namespace sambag { namespace com {

//=============================================================================
/** 
  * @class ICommand.
  * Interface for a Command Object.
  */
class ICommand {
//=============================================================================
protected:
	//-------------------------------------------------------------------------
	/**
	 * Constructor is protected. Always use a static create method.
	 */
	ICommand() {}
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ICommand> Ptr;
	//-------------------------------------------------------------------------
	virtual void execute() = 0;
}; // ICommand
}} // namespace(s)

#endif /* SAMBAG_ICOMMAND_H */
