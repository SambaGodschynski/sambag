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
	//#########################################################################
	// TODO: create a svg element database class:
	//       -- getElementById
	//       -- getElementByClass
	//       -- getElementByTag
	//#########################################################################
	//-------------------------------------------------------------------------
	// all svg objects with a id are stored here
	typedef std::map<IdType, SvgObject::Ptr> IdMap;
	//-------------------------------------------------------------------------
	// all svg class with a id are stored here
	typedef std::multimap<IdType, SvgObject::Ptr> ClassMap;
	//-------------------------------------------------------------------------
	/**
	 * TODO: workaround; use future SvgElementDatabase instead
	 */
	typedef std::list<SvgObject::Ptr> Svgs;
	Svgs svgs;
private:
	//-------------------------------------------------------------------------
	/**
	 * Initiates svg objects. eg
	 */
	virtual void init();
	//-------------------------------------------------------------------------
	IdMap idMap;
	//-------------------------------------------------------------------------
	ClassMap classMap;
protected:
	//-------------------------------------------------------------------------
	SvgRoot(){}
public:
	//-------------------------------------------------------------------------
	/**
	 * @return num of all registered objects with an id attribute
	 */
	size_t getNumIdObjects() const {
		return idMap.size();
	}
	//-------------------------------------------------------------------------
	/**
	 * @return num of all registered objects with an class attribute
	 */
	size_t getNumClassObjects() const {
		return classMap.size();
	}
	/**
	 * @param id
	 * @return SvgObject with specific id.
	 */
	//-------------------------------------------------------------------------
	SvgObject::Ptr getObjectById( const IdType &id ) const {
		IdMap::const_iterator it = idMap.find(id);
		if (it==idMap.end()) return SvgObject::Ptr();
		return it->second;
	}
	//-------------------------------------------------------------------------
	/**
	 * Container needs a push_back(SvgObject::Ptr) method.
	 * @param _class class name
	 * @param c container object
	 */
	template <typename Container>
	void getObjectsByClass( const ClassType &_class, Container &c ) const {
		typedef ClassMap::const_iterator It;
		std::pair<It, It> range = classMap.equal_range(_class);
		for ( It it =range.first; it!=range.second; ++it ) {
			c.push_back( it->second );
		}
 	}
	//-------------------------------------------------------------------------
	virtual ~SvgRoot(){}
	//-------------------------------------------------------------------------
	static Ptr create(graphicElements::SceneGraph *g = NULL) {
		Ptr neu(new SvgRoot());
		neu->__setSelf(neu);
		if (!g)
			return neu;
		neu->setRelatedSceneGraph(g->getPtr());
		return neu;
	}
	//-------------------------------------------------------------------------
	/**
	 * Funtion for @see XML2Object::CreatedSignal slot.
	 * Will be called whenever a sub object of this root element was created.
	 * @param newObject
	 */
	void subObjectCreated( SvgObject::Ptr newObject );
};


}}} // namespace
#endif /* SVGROOT_HPP_HPP_ */
