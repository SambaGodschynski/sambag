/*
 * SvgComponentUIBase.hpp
 *
 *  Created on: Thu Jul  3 09:59:43 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SvgComponentUI_H
#define SAMBAG_SvgComponentUI_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/ui/AComponentUI.hpp>
#include <sambag/disco/components/events/MouseEvent.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <sambag/disco/components/SvgComponent.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui {
//=============================================================================
/** 
  * @class SvgComponentUIBase.
  */
class SvgComponentUIBase : public AComponentUI,
                           public boost::enable_shared_from_this<SvgComponentUIBase>
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgComponentUIBase> Ptr;
protected:
	//-------------------------------------------------------------------------
	SvgComponentUIBase(){}
    //-------------------------------------------------------------------------
    template <class T>
    boost::shared_ptr<T> getPtr(boost::weak_ptr<T> _x, bool _throw=true) const {
        boost::shared_ptr<T> res = _x.lock();
        if (!res && _throw) {
            throw std::runtime_error(std::string("SvgComponentUIBase:") +
            typeid(T).name() + " = null");
        }
        return res;
    }
    //-------------------------------------------------------------------------
    SvgComponent::Ptr getSvgComponent(AComponent::Ptr c);
    //-------------------------------------------------------------------------
    svg::graphicElements::ISceneGraph::Ptr getSceneGraph(SvgComponent::Ptr svg);
    //-------------------------------------------------------------------------
    SvgComponent::Dummy::Ptr getFirstChildOfClass(const std::string &className,
        SvgComponent::Dummy::Ptr c,
        SvgComponent::Ptr svg = SvgComponent::Ptr(),
        svg::graphicElements::ISceneGraph::Ptr g = svg::graphicElements::ISceneGraph::Ptr()
    );
public:
}; // SvgComponentUIBase
///////////////////////////////////////////////////////////////////////////////
//=============================================================================
/** 
  * @class SvgComponentUI.
  */
template <class _Model>
class SvgComponentUI : public SvgComponentUIBase
{
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef _Model Model;
    typedef SvgComponentUI<Model> ThisClass;
	typedef boost::shared_ptr<ThisClass> Ptr;
	typedef boost::weak_ptr<ThisClass> WPtr;
    typedef boost::shared_ptr<Model> ModelPtr;
	typedef boost::weak_ptr<Model> ModelWPtr;
protected:
    //-------------------------------------------------------------------------
    ModelWPtr model;
	//-------------------------------------------------------------------------
	SvgComponentUI(){}
    //-------------------------------------------------------------------------
    void onPropertyChanged(const com::events::PropertyChanged &ev);
    //-------------------------------------------------------------------------
    virtual void installModelListeners() {}
    //-------------------------------------------------------------------------
    virtual void installModel(SvgComponent::Dummy::Ptr c);
private:
public:
    //-------------------------------------------------------------------------
    ModelPtr getModel() const {
        return getPtr(model);
    }
}; // SvgComponentUIBase
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class M>
void SvgComponentUI<M>::installModel(SvgComponent::Dummy::Ptr c)
{
    ModelPtr model =
        c->getModel<Model>();
    if (!model) {
        throw std::runtime_error("SvgComponentUI: wrong model");
    }
    this->model = model;
    installModelListeners();
    
    using namespace com::events;
    c->EventSender<PropertyChanged>::addTrackedEventListener(
        boost::bind(&ThisClass::onPropertyChanged, this, _2),
        shared_from_this()
    );
}
//-----------------------------------------------------------------------------
template <class M>
void SvgComponentUI<M>::onPropertyChanged(const com::events::PropertyChanged &ev)
{
    // swap model
    if (ev.getPropertyName() == SvgComponent::Dummy::PROPERTY_MODEL) {
        ArbitraryType::Ptr _new;
        ev.getNewValue(_new);
        if (!_new) {
            throw std::runtime_error("SvgComponentUI model property null");
        }
        ModelPtr newModel;
        com::get(_new, newModel);
        if (!newModel) {
            return;
        }
        this->model = newModel;
        installModelListeners();
    }

}
}}}} // namespace(s)

#endif /* SAMBAG_SvgComponentUI_H */

