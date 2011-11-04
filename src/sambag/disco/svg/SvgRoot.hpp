/*
 * SvgRoot.hpp.hpp
 *
 *  Created on: 24.09.2011
 *      Author: samba
 */

#ifndef SVGROOT_HPP_HPP_
#define SVGROOT_HPP_HPP_

#include "Svg.hpp"
#include "SvgCompound.hpp"
#include "sambag/com/Common.hpp"
#include <boost/function.hpp>
#include <map>

namespace sambag { namespace disco { namespace svg {

//=============================================================================
/**
 * binding to the svg <root> element.
 */
class SvgRoot : public SvgCompound {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgRoot> Ptr;
	//-------------------------------------------------------------------------
	typedef std::list<SvgObject::Ptr> Svgs;
	// created svg objects
	Svgs svgs;
	//-------------------------------------------------------------------------
	typedef boost::function<void(SvgObject::Ptr)> ObjectRequestFunction;
private:
	//-------------------------------------------------------------------------
	typedef std::map<std::string, SvgObject::Ptr> IdMap;
	//-------------------------------------------------------------------------
	IdMap idMap;
	//-------------------------------------------------------------------------
	/**
	 * is true when object structure is completely created.
	 */
	bool creationCompleted;
	//-------------------------------------------------------------------------
	typedef std::pair<std::string, ObjectRequestFunction> ObjectRequest;
	//-------------------------------------------------------------------------
	typedef std::list<ObjectRequest> ObjectRequests;
	//-------------------------------------------------------------------------
	ObjectRequests requests;
	//-------------------------------------------------------------------------
	void handleRequests();
protected:
	//-------------------------------------------------------------------------
	SvgRoot() : creationCompleted(false) {}
public:
	//-------------------------------------------------------------------------
	/**
	 * calls callBk when id's object is available.
	 * @param url
	 * @param callBk
	 */
	void requestForObject(const std::string &id, ObjectRequestFunction callBk) {
		requests.push_back(std::make_pair(id, callBk));
		if (creationCompleted)
			handleRequests();
	}
	//-------------------------------------------------------------------------
	void initCreatedObjects();
	//-------------------------------------------------------------------------
	virtual ~SvgRoot(){}
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param root
	 * @param firstElement true if first element in tree
	 * @return
	 */
	static Ptr create(
			SvgRoot *root = NULL,
			bool firstElement = false)
	{
		Ptr neu(new SvgRoot());
		neu->__setSelf(neu);
		if (!root || root == neu.get())
			return neu;
		neu->createBase(root);
		// set default style if first element
		if (firstElement) {
			root->getRelatedSceneGraph()->setStyleTo(neu->getGraphicElement(),
			graphicElements::Style::DEFAULT_STYLE);
		}
		return neu;
	}
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param g
	 * @param firstElement true if first element in tree
	 * @return
	 */
	static Ptr create(
			graphicElements::SceneGraph *g = NULL,
			bool firstElement = false)
	{
		Ptr neu(new SvgRoot());
		neu->__setSelf(neu);
		if (!g)
			return neu;
		neu->setRelatedSceneGraph(g->getPtr());
		// set default style if first element
		if (firstElement) {
			g->setStyleTo(neu->getGraphicElement(),
					graphicElements::Style::DEFAULT_STYLE);
		}
		return neu;
	}
	//-------------------------------------------------------------------------
	/**
	 * Funtion for @see XML2Object::CreatedSignal slot.
	 * Will be called whenever a sub object of this root element was created.
	 * @param newObject
	 */
	void subObjectCreated( SvgObject::Ptr newObject, const std::string& );
};


}}} // namespace
#endif /* SVGROOT_HPP_HPP_ */
