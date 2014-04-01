/*
 * Path.hpp
 *
 *  Created on: 27.09.2011
 *      Author: samba
 */

#ifndef DISCO_PATH_HPP_
#define DISCO_PATH_HPP_

#include "GraphicElement.hpp"
#include "PointContainer.hpp"
#include <list>
#include "sambag/com/Helper.hpp"
#include <sambag/disco/Shape.hpp>

namespace sambag { namespace disco { namespace svg { namespace graphicElements {
//=============================================================================
class Path : public virtual GraphicElement, public virtual Shape {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Path> Ptr;
private:
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
	virtual void shape( IDrawContext::Ptr context );
};

}}}} // namespace


#endif /* PATH_HPP_ */
