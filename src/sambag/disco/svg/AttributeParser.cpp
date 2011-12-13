/*
 * StyleParser.cpp
 *
 *  Created on: 21.09.2011
 *      Author: samba
 */

#include "AttributeParser.hpp"
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/assign/list_inserter.hpp>
#include <vector>
#include <sstream>
#include <math.h>



namespace {
using namespace sambag::disco;
using namespace sambag::disco::svg;
using namespace sambag::math;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>transfomation
//-----------------------------------------------------------------------------
void transformTransl(const std::vector<Number> &values, Matrix& m) {
	if (values.size()<1) return;
	m(0,2) = values[0];
	if (values.size()<2) {
		m(1,2) = 0;
		return;
	}
	m(1,2) = values[1];
}
//-----------------------------------------------------------------------------
void transformRot(const std::vector<Number> &values, Matrix& m) {
	if (values.size()<1) return;
	Number _cos = cos(values[0] * M_PI / 180.0);
	Number _sin = sin(values[0] * M_PI / 180.0);
	m(0,0) = _cos;
	m(1,0) = _sin;
	m(0,1) = -_sin;
	m(1,1) = _cos;
	if (values.size()<2) return;
	// rotation around a specific point
	m(0,2) = values[0];
	if (values.size()<3) {
		m(1,2) = 0;
		return;
	}
	m(1,2) = values[1];
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
//-----------------------------------------------------------------------------
/**
 * processes cmd string with given values.
 * @param cmd: could be rotate, translate, scale, skewX, skewY, matrix
 * @param values: a list of numbers: x,y,...
 * @param m: out matrix
 */
void processTransformStrings( const std::string &cmd, const std::string &values, Matrix& matrix ) {
	// split components of value string
	std::vector<Number> v;
	AttributeParser::getValuesFromString< std::vector<Number> >(values, v);
	if (v.empty()) return;
	// select fitting transformation
	if (cmd=="translate")
		transformTransl(v, matrix);
	if (cmd=="rotate")
		transformRot(v, matrix);
	if (cmd=="scale")
		transformScal(v, matrix);
	if (cmd=="skewx")
		transformSkewX(v, matrix);
	if (cmd=="skewy")
		transformSkewY(v, matrix);
	if (cmd=="matrix")
		transformMatr(v, matrix);
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>path
void processPathStrings(
		const std::string &cmd,
		const std::string &values,
		sambag::disco::graphicElements::pathInstruction::PathInstructions &pI)
{
	using namespace graphicElements::pathInstruction;
	if (cmd=="") return;
	PointContainer pC;
	AttributeParser::getValuesFromString<PointContainer>(values, pC);
	pI.push_back( std::make_pair(
		pC,
		svg::AttributeParser::getPathInstruction(cmd)
	));
}

} // namespace

namespace sambag { namespace disco { namespace svg {
//=============================================================================
// Parser
//=============================================================================
//-----------------------------------------------------------------------------
AttributeParser::HtmlColors AttributeParser::htmlColors;
//-----------------------------------------------------------------------------
AttributeParser::PathInstrMap AttributeParser::pathInstrMap;
//-----------------------------------------------------------------------------
void AttributeParser::parseTransform(const std::string &str, Matrix &matrix) {
	if (str.length()==0) return;
	std::string inStr = str;
	prepareString(inStr);
	// extract all matches
	std::string::const_iterator begin = inStr.begin();
	std::string::const_iterator end = inStr.end();
	boost::match_results<std::string::const_iterator> what;
	boost::regex re("([a-zA-Z]+?)\\s*\\(([ 0-9.,-]+)\\)");
	for ( ;
		regex_search(begin, end, what, re);
		begin = what[0].second
	) {
	    std::string cmd = what[1];
	    std::string values = what[2];
	    Matrix m = IDENTITY_MATRIX;
	    processTransformStrings(cmd, values, m);
	    matrix = prod(matrix, m);
	}
}
//-----------------------------------------------------------------------------
void AttributeParser::parsePathInstructions(const std::string &str, PathInstructions& pi) {
	if (str.length()==0) return;
	std::string inStr = str;
	prepareString(inStr, false);
	// extract all matches
	std::string::const_iterator begin = inStr.begin();
	std::string::const_iterator end = inStr.end();
	boost::match_results<std::string::const_iterator> what;
	boost::regex re("([a-zA-Z]+)\\s*([ 0-9.,-]*)");
	for ( ;
		regex_search(begin, end, what, re);
		begin = what[0].second
	) {
		std::string cmd = what[1];
	    std::string values = what[2];
		processPathStrings(cmd, values, pi);
	}
}
//-----------------------------------------------------------------------------
void AttributeParser::parsePointContainer(const std::string &str, PointContainer& pC) {
	if (str.length()==0) return;
	std::string inStr = str;
	prepareString(inStr, false);
	// extract all matches
	std::string::const_iterator begin = inStr.begin();
	std::string::const_iterator end = inStr.end();
	boost::match_results<std::string::const_iterator> what;
	boost::regex re("([0-9.-]+)");
	for ( ;
		regex_search(begin, end, what, re);
		begin = what[0].second
	) {
		std::string value = what[1];
		pC.push_back(string2Number(value));
	}
}
//-----------------------------------------------------------------------------
Dash::Ptr AttributeParser::parseDashArray(const std::string &str) {
	if (str.length()==0) return Dash::Ptr();
	std::string inStr = str;
	prepareString(inStr, false);
	// extract all matches
	std::string::const_iterator begin = inStr.begin();
	std::string::const_iterator end = inStr.end();
	boost::match_results<std::string::const_iterator> what;
	boost::regex re("([a-z0-9.-]+)");
	std::list<Coordinate> l;
	for ( ;
		regex_search(begin, end, what, re);
		begin = what[0].second
	) {
		Coordinate c;
		parseCoordinate(what[1], c);
		l.push_back(c);
	}
	return Dash::createWithValues(l);
}
//-----------------------------------------------------------------------------
void AttributeParser::parseViewBox(const std::string &str, Rectangle& pC) {
	if (str.length()==0) return;
	std::string inStr = str;
	prepareString(inStr, false);
	// extract all matches
	std::string::const_iterator begin = inStr.begin();
	std::string::const_iterator end = inStr.end();
	boost::match_results<std::string::const_iterator> what;
	std::string d = "([a-z0-9.-]+)";
	boost::regex re(d + "\\s+" + d + "\\s+" + d +"\\s+" + d);
	if (!regex_search(begin, end, what, re))
		return;
	Coordinate x, y, x1, y1;
	parseCoordinate(what[1], x);
	parseCoordinate(what[2], y);
	parseCoordinate(what[3], x1);
	parseCoordinate(what[4], y1);
	if (x < 0 || y < 0 || x1 < 0 || y1 < 0)
		return;
	if (x1 < x || y1 < y)
		return;
	pC = Rectangle(Point2D(x,y), Point2D(x1, y1));
}
//-----------------------------------------------------------------------------
void AttributeParser::parseCoordinate(const std::string &str, Coordinate&c) {
	if (str.length()==0) return;
	std::string::const_iterator begin = str.begin();
	std::string::const_iterator end = str.end();
	boost::match_results<std::string::const_iterator> what;
	boost::regex re("([0-9.-]+)([a-z%])*");
	regex_search(begin, end, what, re);
	std::stringstream ss(what[1]); //value
	ss>>c.value;
	/*
	 * enum Type {
		NONE,
		PX,
		IN,
		CM,
		MM,
		PT,
		EM,
		EX,
		PERCENT
	};
	 *
	 */
	if (what[2].length() > 0) {
		std::string m = boost::algorithm::to_lower_copy(std::string(what[2]));
		if (m=="px") c.type = Coordinate::PX;
		if (m=="in") c.type = Coordinate::IN;
		if (m=="cm") c.type = Coordinate::CM;
		if (m=="mm") c.type = Coordinate::MM;
		if (m=="pt") c.type = Coordinate::PT;
		if (m=="pc") c.type = Coordinate::PC;
		if (m=="em") c.type = Coordinate::EM;
		if (m=="ex") c.type = Coordinate::EX;
		if (m=="%") c.type = Coordinate::PERCENT;
	}
}
//-----------------------------------------------------------------------------
void AttributeParser::parseColor(const std::string &in, ColorRGBA &color) {
	if (in.length()==0) return;

	std::string str=in;
	prepareString(str);
	static const boost::regex colorname("[a-zA-Z]+");
	static const boost::regex colorval("#[0-9a-fA-F]+");
	if ( boost::regex_match(str, colorname) ) { // is colorname
		const ColorRGBA &c = getColorByHtmlName(str);
		color.setValues(c.getR(), c.getG(), c.getB(), c.getA());
		return;
	}
	if ( !boost::regex_match(str, colorval) ) { // doesn't match as value
		color = ColorRGBA::NULL_COLOR;
		return;
	}
	// is color html value
	std::string expr = boost::algorithm::trim_copy(str);
	if (expr.length() == 7) {// #rrggbb
		Number r = hex2Int(expr.substr(1,2)) / 255.0;
		Number g = hex2Int(expr.substr(3,2)) / 255.0;
		Number b = hex2Int(expr.substr(5,2)) / 255.0;
		Number a = 1.0;
		color.setValues(r,g,b,a);
		return;
	}
	if (expr.length() == 4) {// #rgb => #rrggbb
		Number r = hex2Int(expr.substr(1,1)) / 255.0;
		r+= 16 * r;
		Number g = hex2Int(expr.substr(2,1)) / 255.0;
		g+= 16 * g;
		Number b = hex2Int(expr.substr(3,1)) / 255.0;
		b+= 16 * b;
		Number a = 1.0;
		color.setValues(r,g,b,a);
		return;
	}
	color = ColorRGBA::NULL_COLOR;
}
//-----------------------------------------------------------------------------
void AttributeParser::parseOpacity(const std::string &in, Number &v) {
	if (in.length()==0) return;
	std::string str=in;
	prepareString(str);
	static const boost::regex single("[0-9.]+");
	static const boost::regex percent("[0-9.]+%");
	if ( boost::regex_match(str, single) ) { // is a single number
		v = string2Number(str);
	}
	if ( !boost::regex_match(str, percent) ) return;

	v = string2Number(str);
	v /= 100; // percent to 0.0 .. 1.0
}
//-----------------------------------------------------------------------------
const ColorRGBA & AttributeParser::getColorByHtmlName( const std::string &name ) {
	using boost::algorithm::to_lower_copy;
	if (htmlColors.empty())
		initHtmlColors();
	std::string key = to_lower_copy(name);
	HtmlColors::const_iterator it = htmlColors.find(key);
	if (it==htmlColors.end())
		return ColorRGBA::NULL_COLOR;
	return it->second;
}
//-----------------------------------------------------------------------------
pathInstruction::InstructionOp
AttributeParser::getPathInstruction( const std::string &op )
{
	if (pathInstrMap.empty())
		initPathInstr();
	PathInstrMap::const_iterator it = pathInstrMap.find(op);
	if (it==pathInstrMap.end())
		return pathInstruction::NONE;
	return it->second;
}
//-----------------------------------------------------------------------------
/**
 * Prepares string for further processing. such as trim, to_lower ...
 * @param inout
 * @param toLower
 * @return prepared string
 */
std::string &
AttributeParser::prepareString(std::string &inout, bool toLower) {
	boost::algorithm::trim(inout);
	if (toLower)
		boost::algorithm::to_lower(inout);
	return inout;
}
//-----------------------------------------------------------------------------
int AttributeParser::hex2Int ( const std::string &hex) {
	int x = 0;
	std::stringstream ss;
	ss << std::hex << hex;
	ss >> x;
	return x;
}
//-----------------------------------------------------------------------------
/**
 * copies whole istream content to out string.
 * @param istr
 * @param out
 */
void AttributeParser::getWholeString( std::istream& istr, std::string &out ) {
	istr>>out;
	while ( !istr.eof() ) {
		std::string n;
		istr>>n;
		out+= " " + n;
	}
}
//-----------------------------------------------------------------------------
Number AttributeParser::string2Number(const std::string &str) {
	std::stringstream ss;
	ss<<str;
	Number n;
	ss>>n;
	return n;
}
//-----------------------------------------------------------------------------
/**
 * Converts Container<string>:in to Container<Number>:out.
 * out will be empty if one convertion failed.
 * @param in
 * @param out
 */
template <typename StrContainer, typename NumberContainer>
void AttributeParser::strings2Numbers(const StrContainer &in, NumberContainer &out) {
	boost_for_each( const std::string &str, in ) {
		std::stringstream ss;
		ss<<str;
		Number n;
		ss>>n;
		if (ss.fail() || !ss.eof()) {
			out.clear();
			return;
		}
		out.push_back(n);
	}
}
/**
 * gets values string such as "3, 4.5, 3" into Container of Numbers
 * @param values
 * @param out
 */
template< typename Container >
void AttributeParser::getValuesFromString( const std::string &_values, Container &out ) {
	std::string values = boost::algorithm::trim_copy(_values);
	std::vector<std::string> strs;
	boost::regex re( "\\s*,\\s*|\\s+" );
	boost::algorithm::split_regex( strs, values, re) ;
	if (strs.empty()) return;
	strings2Numbers<std::vector<std::string>, Container >(strs, out);
}
//-----------------------------------------------------------------------------
std::string AttributeParser::getUrl(const std::string &url) {
	std::string inStr = url;
	prepareString(inStr, false);
	// extract all matches
	std::string::const_iterator begin = inStr.begin();
	std::string::const_iterator end = inStr.end();
	boost::match_results<std::string::const_iterator> what;
	boost::regex re("url\\(([a-zA-Z0-9_#-]*)\\)");
	if(regex_search(begin, end, what, re)) {
		return what[1];
	}
	return "";
}
}}} // namespaces

//=============================================================================
// stream operators
//=============================================================================
//-----------------------------------------------------------------------------
extern std::istream & operator>>(std::istream& istr, sambag::disco::Coordinate &c) {
	std::string in;
	AttributeParser::getWholeString(istr, in);
	AttributeParser::prepareString(in);
	AttributeParser::parseCoordinate(in, c);
	return istr;
}
//-----------------------------------------------------------------------------
std::istream & operator>>(std::istream& istr, sambag::disco::ColorRGBA& color) {
	using namespace sambag::disco::svg;
	std::string str;
	AttributeParser::getWholeString(istr, str);
	AttributeParser::parseColor(str, color);
	return istr;
}
//-----------------------------------------------------------------------------
std::istream & operator>>(std::istream& istr, sambag::disco::Font::Weight &weight) {
	using namespace sambag::disco;
	std::string in;
	AttributeParser::getWholeString(istr, in);
	AttributeParser::prepareString(in);
	if (in=="bold") weight = Font::WEIGHT_BOLD;
	else weight = Font::WEIGHT_NORMAL;
	return istr;

}
//-----------------------------------------------------------------------------
std::istream & operator>>(std::istream& istr, sambag::disco::Font::Slant &slant) {
	using namespace sambag::disco;
	std::string in;
	AttributeParser::getWholeString(istr, in);
	AttributeParser::prepareString(in);
	if (in=="italic") slant = Font::SLANT_ITALIC; return istr;
	if (in=="oblique") slant = Font::SLANT_OBLIQUE; return istr;
	slant = Font::SLANT_NORMAL;
	return istr;
}
//-----------------------------------------------------------------------------
std::istream & operator>>(std::istream& istr, sambag::math::Matrix &m) {
	using namespace sambag::disco::svg;
	std::string str;
	AttributeParser::getWholeString(istr, str);
	m = IDENTITY_MATRIX;
	AttributeParser::parseTransform(str, m);
	return istr;
}
//-----------------------------------------------------------------------------
std::istream & operator>>(
	std::istream& istr,
    sambag::disco::graphicElements::pathInstruction::PathInstructions &pI)
{
	using namespace sambag::disco::svg;
	std::string str;
	AttributeParser::getWholeString(istr, str);
	AttributeParser::parsePathInstructions(str, pI);
	return istr;
}
//-----------------------------------------------------------------------------
std::istream & operator>>(
	std::istream& istr,
    sambag::disco::graphicElements::pathInstruction::PointContainer &pC)
{
	using namespace sambag::disco::svg;
	std::string str;
	AttributeParser::getWholeString(istr, str);
	AttributeParser::parsePointContainer(str, pC);
	return istr;
}
//-----------------------------------------------------------------------------
extern std::istream & operator>>(
	std::istream &istr,
	sambag::disco::IDrawContext::LineCapStyle &v
)
{
	std::string in;
	AttributeParser::getWholeString(istr, in);
	AttributeParser::prepareString(in, true);
	if (in=="butt")
		v = sambag::disco::IDrawContext::LINE_CAP_BUTT;
	if (in=="square")
		v = sambag::disco::IDrawContext::LINE_CAP_SQUARE;
	if (in=="round")
		v = sambag::disco::IDrawContext::LINE_CAP_ROUND;
	return istr;
}
//-----------------------------------------------------------------------------
extern std::istream & operator>>(
	std::istream &istr,
	sambag::disco::IDrawContext::LineJoin &v
)
{
	std::string in;
	AttributeParser::getWholeString(istr, in);
	AttributeParser::prepareString(in, true);
	if (in=="miter")
		v = sambag::disco::IDrawContext::LINE_JOIN_MITER;
	if (in=="round")
		v = sambag::disco::IDrawContext::LINE_JOIN_ROUND;
	if (in=="bevel")
		v = sambag::disco::IDrawContext::LINE_JOIN_BEVEL;
	return istr;
}
