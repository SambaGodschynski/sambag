/*
 * Path.hpp
 *
 *  Created on: 27.09.2011
 *      Author: samba
 */

#ifndef DISCO_PATH_HPP_
#define DISCO_PATH_HPP_

#include "GraphicElement.hpp"
#include "sambag/disco/graphicElements/PointContainer.hpp"
#include <list>
#include "sambag/com/Helper.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
class Path : public GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Path> Ptr;
private:
	//-------------------------------------------------------------------------
	sambag::disco::Path::Ptr storedPath;
	//-------------------------------------------------------------------------
	virtual void drawPath( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	virtual void drawInstructions( IDrawContext::Ptr context ) const;
protected:
	//-------------------------------------------------------------------------
	pathInstruction::PathInstructions pathInstructions;
	//-------------------------------------------------------------------------
	Path();
public:
	//-------------------------------------------------------------------------
	std::string toString() const {
			return "Path";
	}
	//-------------------------------------------------------------------------
	/**
	 * @param cn
	 */
	virtual void appendPathToContext(IDrawContext::Ptr cn) const {
		drawInstructions(cn);
	}
	//-------------------------------------------------------------------------
	virtual GraphicElement::Ptr clone() const {
		Ptr neu = create();
		*neu = *this;
		return neu;
	}
	//-------------------------------------------------------------------------
	void setPathInstructions( const pathInstruction::PathInstructions &pI ) {
		pathInstructions = pI;
		storedPath.reset();
	}
	//-------------------------------------------------------------------------
	const pathInstruction::PathInstructions & getPathInstructions() const {
		return pathInstructions;
	}
	//-------------------------------------------------------------------------
	static Ptr create()
	{
		Ptr neu(new Path());
		neu->__setSelf(neu);
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual ~Path();
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const {
		return Rectangle(); // TODO: implement boundingbox
	}
};

}}} // namespace


#endif /* PATH_HPP_ */
