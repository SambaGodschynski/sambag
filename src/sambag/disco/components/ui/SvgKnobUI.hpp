/*
 * SvgKnobUI.hpp
 *
 *  Created on: Thu Jul  3 09:59:43 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SVGKNOBUI_H
#define SAMBAG_SVGKNOBUI_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/ui/AComponentUI.hpp>
#include <sambag/disco/components/events/MouseEvent.hpp>
#include <sambag/disco/components/DefaultBoundedRangeModel.hpp>
#include <sambag/disco/components/SvgComponent.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "SvgComponentUI.hpp"

namespace sambag { namespace disco {
namespace components { namespace ui {
//=============================================================================
/** 
  * @class SvgKnobUI.
  */
class SvgKnobUI : public SvgComponentUI<DefaultBoundedRangeModel>
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgKnobUI> Ptr;
    //-------------------------------------------------------------------------
    typedef DefaultBoundedRangeModel::StateChangedEvent StateChanged;
    //-------------------------------------------------------------------------
    typedef SvgComponentUI<DefaultBoundedRangeModel> Super;
protected:
	//-------------------------------------------------------------------------
	SvgKnobUI(){}
    //-------------------------------------------------------------------------
    virtual void installListeners(AComponent::Ptr c);
    //-------------------------------------------------------------------------
    virtual void installHandleListeners();
    //-------------------------------------------------------------------------
    virtual void installModelListeners();
    //-------------------------------------------------------------------------
    void rotateKnob(double value);
private:
	//-------------------------------------------------------------------------
	Point2D firstPoint;
	Point2D lastPoint;
	Coordinate startValue;
	Coordinate oldValue;
	Coordinate entryState;
	Coordinate range;
	Coordinate coef;
	Coordinate radius;
	Coordinate startAngle;
	Coordinate rangeAngle;
	Coordinate halfAngle;
	Coordinate aCoef;
	Coordinate bCoef;
    //-------------------------------------------------------------------------
    SvgComponent::Dummy::WPtr main, handle;
public:
    //-------------------------------------------------------------------------
    SvgComponent::Dummy::Ptr getMain(bool _throw=true) const { return getPtr(main, _throw); }
    //-------------------------------------------------------------------------
	SvgComponent::Dummy::Ptr getHandle(bool _throw = true) const { return getPtr(handle, _throw); }
	//-------------------------------------------------------------------------
	/**
	 * Configures the specified component appropriately for the look and feel.
	 * @param c
	 */
	virtual void installUI(AComponentPtr c);
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new SvgKnobUI());
	}
    //-------------------------------------------------------------------------
    void onPropertyChanged(const com::events::PropertyChanged &ev);
	//-------------------------------------------------------------------------
	void onMouse(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void mousePressed(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void mouseDragged(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void mouseWheelRotated(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void mouseEntered(const events::MouseEvent &ev);
    //-------------------------------------------------------------------------
	void mouseExited(const events::MouseEvent &ev);
    //-------------------------------------------------------------------------
    void onKnobStateChanged(const StateChanged &ev);
}; // SvgKnobUI
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
}}}} // namespace(s)

#endif /* SAMBAG_SvgKnobUI_H */

