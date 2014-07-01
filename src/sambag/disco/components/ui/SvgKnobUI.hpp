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

namespace sambag { namespace disco {
namespace components { namespace ui {
//=============================================================================
/** 
  * @class SvgKnobUI.
  */
class SvgKnobUI : public AComponentUI,
                  public boost::enable_shared_from_this<SvgKnobUI>
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgKnobUI> Ptr;
    //-------------------------------------------------------------------------
    typedef DefaultBoundedRangeModel::StateChangedEvent StateChanged;
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
    virtual void installPropertyListeners();
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
    boost::weak_ptr<DefaultBoundedRangeModel> model;
    //-------------------------------------------------------------------------
    template <class T>
    boost::shared_ptr<T> getPtr(boost::weak_ptr<T> _x) const {
        boost::shared_ptr<T> res = _x.lock();
        if (!res) {
            throw std::runtime_error("SvgKnobUI null_pointer");
        }
        return res;
    }
public:
    //-------------------------------------------------------------------------
    SvgComponent::Dummy::Ptr getMain() const { return getPtr(main); }
    //-------------------------------------------------------------------------
    SvgComponent::Dummy::Ptr getHandle() const { return getPtr(handle); }
    //-------------------------------------------------------------------------
    boost::shared_ptr<DefaultBoundedRangeModel> getModel() const { return getPtr(model); }
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
    virtual void draw(IDrawContext::Ptr cn, AComponentPtr c) {
        std::cout<<"draw"<<std::endl;
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

