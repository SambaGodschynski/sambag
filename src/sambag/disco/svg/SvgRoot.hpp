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
#include <sambag/disco/svg/units/Units.hpp>
#include "AttributeParser.hpp"
#include <map>
#include <string>

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
	//-------------------------------------------------------------------------
	struct Width_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct Height_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct Viewbox_tag { typedef std::string Type; };
	//-------------------------------------------------------------------------
	typedef std::map<std::string, SvgObject::WPtr> IdMap;
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
	std::string svgPath; // location of svg file
	//-------------------------------------------------------------------------
	SvgRoot() : creationCompleted(false) {}
	//-------------------------------------------------------------------------
	virtual void init();
private:
	//-------------------------------------------------------------------------
	Rectangle size, viewBox;
public:
	//-------------------------------------------------------------------------
	void setSvgPath(const std::string &loc) {
		svgPath = loc;	
	}
	//-------------------------------------------------------------------------
	const std::string & getSvgPath() const {
		return svgPath;	
	}
	//-------------------------------------------------------------------------
	const Rectangle & getSize() const {
		return size;
	}
	//-------------------------------------------------------------------------
	const Rectangle & getViewBox() const {
		return viewBox;
	}
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
			bool firstElement = false);
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param g
	 * @param firstElement true if first element in tree
	 * @return
	 */
	static Ptr create(
			graphicElements::SceneGraph *g = NULL,
			bool firstElement = false);
	//-------------------------------------------------------------------------
	/**
	 * Funtion for @see XML2Object::CreatedSignal slot.
	 * Will be called whenever a sub object of this root element was created.
	 * @param newObject
	 */
	void subObjectCreated( SvgObject::Ptr newObject, const std::string& );
	//-------------------------------------------------------------------------
	virtual void set(const Width_tag::Type &v, const Width_tag &) {
		size.setWidth(v.solve(0.));
	}
	//-------------------------------------------------------------------------
	virtual void set(const Height_tag::Type &v, const Height_tag &) {
		size.setHeight(v.solve(0.));
	}
	//-------------------------------------------------------------------------
	virtual void set(const Viewbox_tag::Type &v, const Viewbox_tag &) {
		AttributeParser::parseViewBox(v, viewBox);
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.
		registerAttribute<Width_tag::Type, Width_tag, SvgRoot>("width");
		binder.
		registerAttribute<Height_tag::Type, Height_tag, SvgRoot>("height");
		binder.
		registerAttribute<Viewbox_tag::Type, Viewbox_tag, SvgRoot>("viewBox");
	}
};


}}} // namespace
#endif /* SVGROOT_HPP_HPP_ */
