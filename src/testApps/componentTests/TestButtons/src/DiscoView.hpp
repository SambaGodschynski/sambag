/*
 * DiscoView.hpp
 *
 *  Created on: Mar 21, 2012
 *      Author: samba
 */

#ifndef DISCOVIEW_HPP_
#define DISCOVIEW_HPP_

#include <sambag/disco/IDrawContext.hpp>
#include <string>

void processDraw();
void init(int &outWidth, int &outHeight);
void handleMouseButtonPressEvent(int x, int y, int buttons);
void handleMouseButtonReleaseEvent(int x, int y, int buttons);
void handleMouseMotionEvent(int x, int y);
void createWindow(sambag::disco::ISurface::Ptr surf);
typedef void (*FpsCallback)(int fps);
void registerFPSCallbackHandler(FpsCallback cllbk);
#endif /* DISCOVIEW_HPP_ */
