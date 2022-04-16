/*
 * GeometrySerialization.hpp
 *
 *  Created on: Fri Oct 12 14:50:13 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_GEOMETRYSERIALIZATION_H
#define SAMBAG_GEOMETRYSERIALIZATION_H

#include <boost/shared_ptr.hpp>
#include "Geometry.hpp"
#include <boost/serialization/split_free.hpp>

namespace boost { namespace serialization {
namespace sd = sambag::disco;
//-----------------------------------------------------------------------------
template<class Archive>
void save(Archive & ar, const sd::Point2D & obj, const unsigned int version) 
{
	ar << obj.x();
	ar << obj.y();
}
//-----------------------------------------------------------------------------
template<class Archive>
void load(Archive & ar, sd::Point2D & obj, const unsigned int version) 
{
	sd::Coordinate x, y;
	ar >> x;
	ar >> y;
	obj.x(x);
	obj.y(y);
	ar.reset_object_address(& (obj.x()), &x);
	ar.reset_object_address(& (obj.y()), &y);
}
//-----------------------------------------------------------------------------
template<class Archive>
void save(Archive & ar, const sd::Dimension & obj, const unsigned int version) 
{
	ar<<obj.width();
	ar<<obj.height();
}
//-----------------------------------------------------------------------------
template<class Archive>
void load(Archive & ar, sd::Dimension &obj, const unsigned int version) 
{
	sd::Coordinate w, h;
	ar>>w;
	ar>>h;
	obj.width(w);
	obj.height(h);
	ar.reset_object_address(& (obj.width()), &w);
	ar.reset_object_address(& (obj.height()), &h);
}
//-----------------------------------------------------------------------------
template<class Archive>
void save(Archive & ar, const sd::Rectangle & obj, const unsigned int version) 
{
	ar<<obj.x();
	ar<<obj.y();
	ar<<obj.width();
	ar<<obj.height();
}
//-----------------------------------------------------------------------------
template<class Archive>
void load(Archive & ar, sd::Rectangle &obj, const unsigned int version) 
{
	sd::Coordinate x, y, w, h;
	ar>>x;
	ar>>y;
	ar>>w;
	ar>>h;
	obj.x(x);
	obj.y(y);
	obj.width(w);
	obj.height(h);
	ar.reset_object_address(& (obj.x()), &x);
	ar.reset_object_address(& (obj.y()), &y);
	ar.reset_object_address(& (obj.width()), &w);
	ar.reset_object_address(& (obj.height()), &h);
}
//-----------------------------------------------------------------------------
template<class Archive>
void serialize(Archive & ar, sd::Insets & obj, const unsigned int version) {
	ar & boost::serialization::base_object<sd::Rectangle>(obj); 
}
}} // namespace(s)

BOOST_SERIALIZATION_SPLIT_FREE(sd::Point2D)
BOOST_SERIALIZATION_SPLIT_FREE(sd::Dimension)
BOOST_SERIALIZATION_SPLIT_FREE(sd::Rectangle)

#endif /* SAMBAG_GEOMETRYSERIALIZATION_H */
