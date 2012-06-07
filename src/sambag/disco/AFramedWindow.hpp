/*
 * AFramedWindow.hpp
 *
 *  Created on: Thu Jun  7 10:44:33 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_AFRAMEDWINDOW_H
#define SAMBAG_AFRAMEDWINDOW_H

#include <boost/shared_ptr.hpp>
#include "AWindow.hpp"
#include <string>
namespace sambag { namespace disco {

//=============================================================================
/** 
  * @class AFramedWindow.
  */
class AFramedWindow : public AWindow {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<AFramedWindow> Ptr;
protected:
	//-------------------------------------------------------------------------
	AFramedWindow(AWindow::Ptr parent);
private:
public:
	//-------------------------------------------------------------------------
	virtual void setTitle(const std::string &title) = 0;
}; // AFramedWindow
}} // namespace(s)

#endif /* SAMBAG_AFRAMEDWINDOW_H */
