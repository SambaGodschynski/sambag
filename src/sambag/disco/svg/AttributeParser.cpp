/*
 * StyleParser.cpp
 *
 *  Created on: 21.09.2011
 *      Author: samba
 */

#include "AttributeParser.hpp"
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign/list_inserter.hpp>
#include <vector>
#include "sambag/com/Common.hpp"
#include <sstream>
#include <math.h>

namespace {
using namespace sambag::disco;
//-----------------------------------------------------------------------------
int hex2Int ( const std::string &hex) {
	int x = 0;
	std::stringstream ss;
	ss << std::hex << hex;
	ss >> x;
	return x;
}
//-----------------------------------------------------------------------------
/**
 * Converts Container<string>:in to Container<Number>:out.
 * out will be empty if one convertion failed.
 * @param in
 * @param out
 */
template <typename StrContainer, typename NumberContainer>
void string2Number(const StrContainer &in, NumberContainer &out) {
	for_each( const std::string &str, in ) {
		std::stringstream ss;
		ss<<str;
		Number n;
		ss>>n;
		if (ss.fail()) {
			out.clear();
			return;
		}
		out.push_back(n);
	}
}
//-----------------------------------------------------------------------------
void transformTrans(const std::vector<Number> &values, Matrix& m) {
	if (values.size()!=2) return;
	m(0,2) = values[0];
	m(1,2) = values[1];
}
//-----------------------------------------------------------------------------
void transformRot(const std::vector<Number> &values, Matrix& m) {
	// TODO: impl. svg rot values:
	/*
	 * rotate(<rotate-angle> [<cx> <cy>]), which specifies a rotation by <rotate-angle> degrees about a given point.
       If optional parameters <cx> and <cy> are not supplied, the rotate is about the origin of the current user coordinate system. The operation corresponds to the matrix [cos(a) sin(a) -sin(a) cos(a) 0 0].
       If optional parameters <cx> and <cy> are supplied, the rotate is about the point (cx, cy). The operation represents the equivalent of the following specification: translate(<cx>, <cy>) rotate(<rotate-angle>) translate(-<cx>, -<cy>).
	 */
	if (values.size()!=1) return;
	Number _cos = cos(values[0] * M_PI / 180.0);
	Number _sin = sin(values[0] * M_PI / 180.0);
	m(0,0) = _cos;
	m(1,0) = _sin;
	m(0,1) = -_sin;
	m(1,1) = _cos;
}
//-----------------------------------------------------------------------------
void transformScal(const std::vector<Number> &values, Matrix& m) {
	if (values.size()<1) return;
	m(0,0) = values[0];
	if (values.size()<2) {
		m(1,1) = values[0];
		return;
	}
	m(1,1) = values[1];
}
//-----------------------------------------------------------------------------
void transformSkewX(const std::vector<Number> &values, Matrix& m) {
	if (values.size()!=1) return;
	Number _tan = tan(values[0] * M_PI / 180.0);
	m(0,1) = _tan;
}
//-----------------------------------------------------------------------------
void transformSkewY(const std::vector<Number> &values, Matrix& m) {
	if (values.size()!=1) return;
	Number _tan = tan(values[0] * M_PI / 180.0);
	m(1,0) = _tan;

}
//-----------------------------------------------------------------------------
void transformMatr(const std::vector<Number> &values, Matrix& m) {
	if (values.size()!=6) return;
	m(0,0) = values[0];
	m(1,0) = values[1];
	m(0,1) = values[2];
	m(1,1) = values[3];
	m(0,2) = values[4];
	m(1,2) = values[5];
}
} // namespace

namespace sambag { namespace disco { namespace svg {
//=============================================================================
// Parser
//=============================================================================
//-----------------------------------------------------------------------------
AttributeParser::HtmlColors AttributeParser::htmlColors;
//-----------------------------------------------------------------------------
void AttributeParser::parseTransform(const std::string &str, Matrix &matrix) {
	if (str.length()==0) return;
	std::string inStr = str;
	prepareString(inStr);
	// is in-string valid
	static const boost::regex validExpr("([a-zA-Z]+?)\\(([0-9,-]+)\\)*");
	boost::smatch what;
	if (!boost::regex_search(inStr, what, validExpr) ) return;
	if (what.size()!=3) return;
	// split components of value
	std::string cmd = what[1];
	std::string values = what[2];
	std::vector<std::string> v;
	boost::algorithm::split(v, values, boost::is_any_of(","));
	if (v.empty()) return;
	// covert vector<string> 2 vector<Number>
	std::vector<Number> v2;
	v2.reserve(v.size());
	string2Number<std::vector<std::string>, std::vector<Number> >(v, v2);
	if (v2.empty()) return;
	// select fitting transformation
	if (cmd=="translate")
		transformTrans(v2, matrix);
	if (cmd=="rotate")
		transformRot(v2, matrix);
	if (cmd=="scale")
		transformScal(v2, matrix);
	if (cmd=="skewx")
		transformSkewX(v2, matrix);
	if (cmd=="skewy")
			transformSkewY(v2, matrix);
	if (cmd=="matrix")
		transformMatr(v2, matrix);
}
//-----------------------------------------------------------------------------
void AttributeParser::parseColor(const std::string &str, ColorRGBA &color) {
	if (str.length()==0) return;
	static const boost::regex colorname("[a-zA-Z]+");
	static const boost::regex colorval("#[0-9a-fA-F]+");
	if ( boost::regex_match(str, colorname) ) { // is colorname
		const ColorRGBA &c = getColorByHtmlName(str);
		color.setValues(c.getR(), c.getG(), c.getB(), c.getA());
		return;
	}
	if ( !boost::regex_match(str, colorval) ) return;
	// is color html value
	std::string expr = boost::algorithm::trim_copy(str);
	size_t num = expr.length();
	while (num++<7) { // append missing zeros
		expr.append("0");
	}
	Number r = hex2Int(expr.substr(1,2)) / 255.0;
	Number g = hex2Int(expr.substr(3,2)) / 255.0;
	Number b = hex2Int(expr.substr(5,2)) / 255.0;
	Number a = 255;

	color.setValues(r,g,b,a);
}
//-----------------------------------------------------------------------------
const ColorRGBA & AttributeParser::getColorByHtmlName( const std::string &name ) {
	using boost::algorithm::to_lower_copy;
	if (htmlColors.empty())
		initHtmlColors();
	std::string key = to_lower_copy(name);
	HtmlColors::const_iterator it = htmlColors.find(key);
	if (it==htmlColors.end())
		return NULL_COLOR;
	return it->second;
}
//-----------------------------------------------------------------------------
std::string & AttributeParser::prepareString(std::string &inout) {
	boost::algorithm::trim(inout);
	boost::algorithm::to_lower(inout);
	return inout;
}
}}} // namespaces

//=============================================================================
// stream operators
//=============================================================================
//-----------------------------------------------------------------------------
std::istream & operator>>(std::istream& istr, sambag::com::ColorRGBA& color) {
	using namespace sambag::disco::svg;
	std::string str;
	istr>>str;
	AttributeParser::parseColor(str, color);
	return istr;
}
//-----------------------------------------------------------------------------
std::istream & operator>>(std::istream& istr, sambag::disco::Font::Weight &weight) {
	using namespace sambag::disco;
	std::string in;
	istr>>in;
	svg::AttributeParser::prepareString(in);
	if (in=="bold") weight = Font::WEIGHT_BOLD;
	else weight = Font::WEIGHT_NORMAL;
	return istr;

}
//-----------------------------------------------------------------------------
std::istream & operator>>(std::istream& istr, sambag::disco::Font::Slant &slant) {
	using namespace sambag::disco;
	std::string in;
	istr>>in;
	svg::AttributeParser::prepareString(in);
	if (in=="italic") slant = Font::SLANT_ITALIC; return istr;
	if (in=="oblique") slant = Font::SLANT_OBLIQUE; return istr;
	slant = Font::SLANT_NORMAL;
	return istr;
}
//-----------------------------------------------------------------------------
std::istream & operator>>(std::istream& istr, sambag::com::Matrix &m) {
	using namespace sambag::disco::svg;
	std::string str;
	istr>>str;
	m = IDENTITY_MATRIX;
	AttributeParser::parseTransform(str, m);
	return istr;
}
