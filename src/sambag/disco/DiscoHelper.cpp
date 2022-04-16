/*
 * DiscoHelper.cpp
 *
 *  Created on: Sat Apr  6 15:16:56 2013
 *      Author: Johannes Unger
 */

#include "DiscoHelper.hpp"
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <vector>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <sambag/disco/IDiscoFactory.hpp>

namespace sambag { namespace disco {
//=============================================================================
// DiscoHelper
//=============================================================================
//-----------------------------------------------------------------------------
Rectangle getPatternRect(IPattern::Ptr p) {
	ILinearPattern *lin = dynamic_cast<ILinearPattern*>(p.get());
	if (lin) {
		const ILinearPattern::LinearPoints &p = lin->getLinearPoints();
		return Rectangle( boost::get<0>(p), boost::get<1>(p) );
	}
	IRadialPattern *rad = dynamic_cast<IRadialPattern*>(p.get());
	if (rad) {
		const IRadialPattern::RadialCircles &c = rad->getRadialCircles();
		const Point2D &p1 = boost::get<0>(c);
		Number rad1 = boost::get<1>(c);
		Rectangle r1(p1.x() - rad1/2., p1.y() - rad1/2., p1.x() + rad1, p1.y() + rad1);
		const Point2D &p2 = boost::get<2>(c);
		Number rad2 = boost::get<3>(c);
		Rectangle r2(p2.x() - rad2/2., p2.y() - rad2/2., p2.x() + rad2, p2.y() + rad2);
		Rectangle res = union_(r1, r2);
		return res;
	}
	ISurfacePattern *sfp = dynamic_cast<ISurfacePattern*>(p.get());
	if (sfp) {
		ISurface::Ptr sf = sfp->getSurface();
		if (sf) {
			return sf->getSize();
		}
	}
	return Rectangle();
}
//-----------------------------------------------------------------------------
void alignPattern(IDrawContext::Ptr cn, IPattern::Ptr p, const Rectangle &dst) {
	Rectangle pRect = getPatternRect(p);
	Number sx = pRect.width() == 0. ?  1. : (Number)pRect.width();
	Number sy = pRect.height() == 0. ? 1. : (Number)pRect.height();
	sx /= dst.width();
	sy /= dst.height();
	Matrix m = sambag::math::translate2D(pRect.x(), pRect.y());
	m = boost::numeric::ublas::prod(m, sambag::math::scale2D(sx, sy));
	m = boost::numeric::ublas::prod(m, translate2D(-dst.x(), -dst.y()));
	p->setMatrix(m);
}
//-----------------------------------------------------------------------------
namespace {
using boost::spirit::qi::hex;
using boost::spirit::qi::parse;
namespace lambda = boost::lambda;

struct GradOpcodes : 
  boost::spirit::qi::symbols<char, unsigned> 
{
	enum { GRAD_LIN=1, GRAD_RAD };
	GradOpcodes() {
	add
		("lin"    , GRAD_LIN)
		("rad"   , GRAD_RAD)
	;
	}
};

template <typename Iterator>
int __parseGrad(Iterator first, 
		   Iterator last,
		   std::vector<unsigned int> &colors,
		   std::vector<double> &offsets,
		   std::vector<double> &coords) {
	using namespace boost::spirit;
	using qi::double_;
	using qi::hex;
	using qi::phrase_parse;
	using qi::_1;
	using qi::repeat;
	using ascii::space;
	using ascii::char_;
	using boost::phoenix::push_back;
	GradOpcodes opc;
	int _opc = 0;
	bool r = phrase_parse(first, last,
		//  Begin grammar
		(
		opc[boost::phoenix::ref(_opc) = _1] >> // opc
		'(' >> repeat(2,6)[ double_[push_back(boost::phoenix::ref(coords), _1)] >>',' ] >> // coords
		'[' >> *( (hex[push_back(boost::phoenix::ref(colors), _1)] >> ':' >> // colors
		//double_[push_back(ref(alphas), _1)] >> ':' >> 
		double_[push_back(boost::phoenix::ref(offsets), _1)] ) % ',') >> ']' >> ')'
		),
		//  End grammar
		space);
	if (first != last) // fail if we did not get a full match
		return 0;
	return _opc;
}
//-----------------------------------------------------------------------------
ColorRGBA __toColor(unsigned int val) {
	//rrggbb
	ColorRGBA res;
	res.setA(  (0x000000ff & val) / 255. );
	res.setB( ((0x0000ff00 & val) >> 8 )/ 255. );
	res.setG( ((0x00ff0000 & val) >> 16 )/ 255. );
	res.setR( ((0xff000000 & val) >> 24 )/ 255. );
	return res;
}
} // namespace(s)
//-----------------------------------------------------------------------------
IPattern::Ptr createPattern(const std::string &str) {
	std::vector<unsigned int> colors;
	std::vector<double> coords;
	std::vector<double> offsets;
	IPattern::Ptr res;
	int opc = __parseGrad(str.begin(), str.end(), colors, offsets, coords);
	if (opc==0) {
		return IPattern::Ptr();
	}
	SAMBAG_ASSERT( colors.size() == offsets.size() );
	
	size_t numCoords = coords.size();
	if (opc==GradOpcodes::GRAD_LIN && (numCoords == 2 || numCoords == 4)) {
		double x1=0, y1=0, x2=0, y2=0;
		if (numCoords==2) {
			x2 = coords[0]; y2 = coords[1];
		} else {
			x1 = coords[0]; y1 = coords[1];
			x2 = coords[2]; y2 = coords[3];
		}
		res = 
			getDiscoFactory()->
				createLinearPattern(Point2D(x1, y1), Point2D(x2, y2));
	}
	if (opc==GradOpcodes::GRAD_RAD && (numCoords == 3 || numCoords == 6)) {
		double x1=0, y1=0, x2=0, y2=0, r1=0, r2=0;
		if (numCoords==3) {
			x2 = coords[0]; y2 = coords[1]; r2=coords[2];
		} else {
			x1 = coords[0]; y1 = coords[1]; r1=coords[2];
			x2 = coords[3]; y2 = coords[4]; r2=coords[5];
		}
		res = 
			getDiscoFactory()->
				createRadialPattern(Point2D(x1, y1), r1, Point2D(x2, y2), r2);
	}
	if (!res) {
		return IPattern::Ptr();
	}
	IGradient *grad = dynamic_cast<IGradient*>(res.get());
	for(size_t i=0; i<colors.size(); ++i) {
		grad->addColorStop(
			IGradient::ColorStop( __toColor(colors[i]),
			offsets[i]
		));
	}
	return res;
}
//-----------------------------------------------------------------------------
bool isGradient(IPattern::Ptr pat) {
	return dynamic_cast<IGradient*>(pat.get()) != NULL;
}
}} // namespace(s)
