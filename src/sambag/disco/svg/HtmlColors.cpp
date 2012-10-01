/*
 * StyleParser.cpp
 *
 *  Created on: 21.09.2011
 *      Author: samba
 */

#include "HtmlColors.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/assign/list_inserter.hpp>
#include "sambag/com/Common.hpp"
#include "graphicElements/Style.hpp"

namespace sambag { namespace disco { namespace svg {
//=============================================================================
// Parser
//=============================================================================
//-----------------------------------------------------------------------------
HtmlColors::ColorMap HtmlColors::colorMap;
//-----------------------------------------------------------------------------
/**
 * created with "extract_html_colors.py"
 */
void HtmlColors::initHtmlColors() {
	using namespace boost::assign;
	insert(colorMap)
	( "aliceblue", ColorRGBA(0.941176470588,0.972549019608,1.0, 1.0) )
	( "antiquewhite", ColorRGBA(0.980392156863,0.921568627451,0.843137254902, 1.0) )
	( "aqua", ColorRGBA(0.0,1.0,1.0, 1.0) )
	( "aquamarine", ColorRGBA(0.498039215686,1.0,0.83137254902, 1.0) )
	( "azure", ColorRGBA(0.941176470588,1.0,1.0, 1.0) )
	( "beige", ColorRGBA(0.960784313725,0.960784313725,0.862745098039, 1.0) )
	( "bisque", ColorRGBA(1.0,0.894117647059,0.76862745098, 1.0) )
	( "black", ColorRGBA(0.0,0.0,0.0, 1.0) )
	( "blanchedalmond", ColorRGBA(1.0,0.921568627451,0.803921568627, 1.0) )
	( "blue", ColorRGBA(0.0,0.0,1.0, 1.0) )
	( "blueviolet", ColorRGBA(0.541176470588,0.16862745098,0.886274509804, 1.0) )
	( "brown", ColorRGBA(0.647058823529,0.164705882353,0.164705882353, 1.0) )
	( "burlywood", ColorRGBA(0.870588235294,0.721568627451,0.529411764706, 1.0) )
	( "cadetblue", ColorRGBA(0.372549019608,0.619607843137,0.627450980392, 1.0) )
	( "chartreuse", ColorRGBA(0.498039215686,1.0,0.0, 1.0) )
	( "chocolate", ColorRGBA(0.823529411765,0.411764705882,0.117647058824, 1.0) )
	( "coral", ColorRGBA(1.0,0.498039215686,0.313725490196, 1.0) )
	( "cornflowerblue", ColorRGBA(0.392156862745,0.58431372549,0.929411764706, 1.0) )
	( "cornsilk", ColorRGBA(1.0,0.972549019608,0.862745098039, 1.0) )
	( "crimson", ColorRGBA(0.862745098039,0.078431372549,0.235294117647, 1.0) )
	( "cyan", ColorRGBA(0.0,1.0,1.0, 1.0) )
	( "darkblue", ColorRGBA(0.0,0.0,0.545098039216, 1.0) )
	( "darkcyan", ColorRGBA(0.0,0.545098039216,0.545098039216, 1.0) )
	( "darkgoldenrod", ColorRGBA(0.721568627451,0.525490196078,0.043137254902, 1.0) )
	( "darkgray", ColorRGBA(0.662745098039,0.662745098039,0.662745098039, 1.0) )
	( "darkgrey", ColorRGBA(0.662745098039,0.662745098039,0.662745098039, 1.0) )
	( "darkgreen", ColorRGBA(0.0,0.392156862745,0.0, 1.0) )
	( "darkkhaki", ColorRGBA(0.741176470588,0.717647058824,0.419607843137, 1.0) )
	( "darkmagenta", ColorRGBA(0.545098039216,0.0,0.545098039216, 1.0) )
	( "darkolivegreen", ColorRGBA(0.333333333333,0.419607843137,0.18431372549, 1.0) )
	( "darkorange", ColorRGBA(1.0,0.549019607843,0.0, 1.0) )
	( "darkorchid", ColorRGBA(0.6,0.196078431373,0.8, 1.0) )
	( "darkred", ColorRGBA(0.545098039216,0.0,0.0, 1.0) )
	( "darksalmon", ColorRGBA(0.913725490196,0.588235294118,0.478431372549, 1.0) )
	( "darkseagreen", ColorRGBA(0.560784313725,0.737254901961,0.560784313725, 1.0) )
	( "darkslateblue", ColorRGBA(0.282352941176,0.239215686275,0.545098039216, 1.0) )
	( "darkslategray", ColorRGBA(0.18431372549,0.309803921569,0.309803921569, 1.0) )
	( "darkslategrey", ColorRGBA(0.18431372549,0.309803921569,0.309803921569, 1.0) )
	( "darkturquoise", ColorRGBA(0.0,0.807843137255,0.819607843137, 1.0) )
	( "darkviolet", ColorRGBA(0.580392156863,0.0,0.827450980392, 1.0) )
	( "deeppink", ColorRGBA(1.0,0.078431372549,0.576470588235, 1.0) )
	( "deepskyblue", ColorRGBA(0.0,0.749019607843,1.0, 1.0) )
	( "dimgray", ColorRGBA(0.411764705882,0.411764705882,0.411764705882, 1.0) )
	( "dimgrey", ColorRGBA(0.411764705882,0.411764705882,0.411764705882, 1.0) )
	( "dodgerblue", ColorRGBA(0.117647058824,0.564705882353,1.0, 1.0) )
	( "firebrick", ColorRGBA(0.698039215686,0.133333333333,0.133333333333, 1.0) )
	( "floralwhite", ColorRGBA(1.0,0.980392156863,0.941176470588, 1.0) )
	( "forestgreen", ColorRGBA(0.133333333333,0.545098039216,0.133333333333, 1.0) )
	( "fuchsia", ColorRGBA(1.0,0.0,1.0, 1.0) )
	( "gainsboro", ColorRGBA(0.862745098039,0.862745098039,0.862745098039, 1.0) )
	( "ghostwhite", ColorRGBA(0.972549019608,0.972549019608,1.0, 1.0) )
	( "gold", ColorRGBA(1.0,0.843137254902,0.0, 1.0) )
	( "goldenrod", ColorRGBA(0.854901960784,0.647058823529,0.125490196078, 1.0) )
	( "gray", ColorRGBA(0.501960784314,0.501960784314,0.501960784314, 1.0) )
	( "grey", ColorRGBA(0.501960784314,0.501960784314,0.501960784314, 1.0) )
	( "green", ColorRGBA(0.0,0.501960784314,0.0, 1.0) )
	( "greenyellow", ColorRGBA(0.678431372549,1.0,0.18431372549, 1.0) )
	( "honeydew", ColorRGBA(0.941176470588,1.0,0.941176470588, 1.0) )
	( "hotpink", ColorRGBA(1.0,0.411764705882,0.705882352941, 1.0) )
	( "indianred", ColorRGBA(0.803921568627,0.360784313725,0.360784313725, 1.0) )
	( "indigo", ColorRGBA(0.294117647059,0.0,0.509803921569, 1.0) )
	( "ivory", ColorRGBA(1.0,1.0,0.941176470588, 1.0) )
	( "khaki", ColorRGBA(0.941176470588,0.901960784314,0.549019607843, 1.0) )
	( "lavender", ColorRGBA(0.901960784314,0.901960784314,0.980392156863, 1.0) )
	( "lavenderblush", ColorRGBA(1.0,0.941176470588,0.960784313725, 1.0) )
	( "lawngreen", ColorRGBA(0.486274509804,0.988235294118,0.0, 1.0) )
	( "lemonchiffon", ColorRGBA(1.0,0.980392156863,0.803921568627, 1.0) )
	( "lightblue", ColorRGBA(0.678431372549,0.847058823529,0.901960784314, 1.0) )
	( "lightcoral", ColorRGBA(0.941176470588,0.501960784314,0.501960784314, 1.0) )
	( "lightcyan", ColorRGBA(0.878431372549,1.0,1.0, 1.0) )
	( "lightgoldenrodyellow", ColorRGBA(0.980392156863,0.980392156863,0.823529411765, 1.0) )
	( "lightgray", ColorRGBA(0.827450980392,0.827450980392,0.827450980392, 1.0) )
	( "lightgrey", ColorRGBA(0.827450980392,0.827450980392,0.827450980392, 1.0) )
	( "lightgreen", ColorRGBA(0.564705882353,0.933333333333,0.564705882353, 1.0) )
	( "lightpink", ColorRGBA(1.0,0.713725490196,0.756862745098, 1.0) )
	( "lightsalmon", ColorRGBA(1.0,0.627450980392,0.478431372549, 1.0) )
	( "lightseagreen", ColorRGBA(0.125490196078,0.698039215686,0.666666666667, 1.0) )
	( "lightskyblue", ColorRGBA(0.529411764706,0.807843137255,0.980392156863, 1.0) )
	( "lightslategray", ColorRGBA(0.466666666667,0.533333333333,0.6, 1.0) )
	( "lightslategrey", ColorRGBA(0.466666666667,0.533333333333,0.6, 1.0) )
	( "lightsteelblue", ColorRGBA(0.690196078431,0.76862745098,0.870588235294, 1.0) )
	( "lightyellow", ColorRGBA(1.0,1.0,0.878431372549, 1.0) )
	( "lime", ColorRGBA(0.0,1.0,0.0, 1.0) )
	( "limegreen", ColorRGBA(0.196078431373,0.803921568627,0.196078431373, 1.0) )
	( "linen", ColorRGBA(0.980392156863,0.941176470588,0.901960784314, 1.0) )
	( "magenta", ColorRGBA(1.0,0.0,1.0, 1.0) )
	( "maroon", ColorRGBA(0.501960784314,0.0,0.0, 1.0) )
	( "mediumaquamarine", ColorRGBA(0.4,0.803921568627,0.666666666667, 1.0) )
	( "mediumblue", ColorRGBA(0.0,0.0,0.803921568627, 1.0) )
	( "mediumorchid", ColorRGBA(0.729411764706,0.333333333333,0.827450980392, 1.0) )
	( "mediumpurple", ColorRGBA(0.576470588235,0.439215686275,0.847058823529, 1.0) )
	( "mediumseagreen", ColorRGBA(0.235294117647,0.701960784314,0.443137254902, 1.0) )
	( "mediumslateblue", ColorRGBA(0.482352941176,0.407843137255,0.933333333333, 1.0) )
	( "mediumspringgreen", ColorRGBA(0.0,0.980392156863,0.603921568627, 1.0) )
	( "mediumturquoise", ColorRGBA(0.282352941176,0.819607843137,0.8, 1.0) )
	( "mediumvioletred", ColorRGBA(0.780392156863,0.0823529411765,0.521568627451, 1.0) )
	( "midnightblue", ColorRGBA(0.0980392156863,0.0980392156863,0.439215686275, 1.0) )
	( "mintcream", ColorRGBA(0.960784313725,1.0,0.980392156863, 1.0) )
	( "mistyrose", ColorRGBA(1.0,0.894117647059,0.882352941176, 1.0) )
	( "moccasin", ColorRGBA(1.0,0.894117647059,0.709803921569, 1.0) )
	( "navajowhite", ColorRGBA(1.0,0.870588235294,0.678431372549, 1.0) )
	( "navy", ColorRGBA(0.0,0.0,0.501960784314, 1.0) )
	( "oldlace", ColorRGBA(0.992156862745,0.960784313725,0.901960784314, 1.0) )
	( "olive", ColorRGBA(0.501960784314,0.501960784314,0.0, 1.0) )
	( "olivedrab", ColorRGBA(0.419607843137,0.556862745098,0.137254901961, 1.0) )
	( "orange", ColorRGBA(1.0,0.647058823529,0.0, 1.0) )
	( "orangered", ColorRGBA(1.0,0.270588235294,0.0, 1.0) )
	( "orchid", ColorRGBA(0.854901960784,0.439215686275,0.839215686275, 1.0) )
	( "palegoldenrod", ColorRGBA(0.933333333333,0.909803921569,0.666666666667, 1.0) )
	( "palegreen", ColorRGBA(0.596078431373,0.98431372549,0.596078431373, 1.0) )
	( "paleturquoise", ColorRGBA(0.686274509804,0.933333333333,0.933333333333, 1.0) )
	( "palevioletred", ColorRGBA(0.847058823529,0.439215686275,0.576470588235, 1.0) )
	( "papayawhip", ColorRGBA(1.0,0.937254901961,0.835294117647, 1.0) )
	( "peachpuff", ColorRGBA(1.0,0.854901960784,0.725490196078, 1.0) )
	( "peru", ColorRGBA(0.803921568627,0.521568627451,0.247058823529, 1.0) )
	( "pink", ColorRGBA(1.0,0.752941176471,0.796078431373, 1.0) )
	( "plum", ColorRGBA(0.866666666667,0.627450980392,0.866666666667, 1.0) )
	( "powderblue", ColorRGBA(0.690196078431,0.878431372549,0.901960784314, 1.0) )
	( "purple", ColorRGBA(0.501960784314,0.0,0.501960784314, 1.0) )
	( "red", ColorRGBA(1.0,0.0,0.0, 1.0) )
	( "rosybrown", ColorRGBA(0.737254901961,0.560784313725,0.560784313725, 1.0) )
	( "royalblue", ColorRGBA(0.254901960784,0.411764705882,0.882352941176, 1.0) )
	( "saddlebrown", ColorRGBA(0.545098039216,0.270588235294,0.0745098039216, 1.0) )
	( "salmon", ColorRGBA(0.980392156863,0.501960784314,0.447058823529, 1.0) )
	( "sandybrown", ColorRGBA(0.956862745098,0.643137254902,0.376470588235, 1.0) )
	( "seagreen", ColorRGBA(0.180392156863,0.545098039216,0.341176470588, 1.0) )
	( "seashell", ColorRGBA(1.0,0.960784313725,0.933333333333, 1.0) )
	( "sienna", ColorRGBA(0.627450980392,0.321568627451,0.176470588235, 1.0) )
	( "silver", ColorRGBA(0.752941176471,0.752941176471,0.752941176471, 1.0) )
	( "skyblue", ColorRGBA(0.529411764706,0.807843137255,0.921568627451, 1.0) )
	( "slateblue", ColorRGBA(0.41568627451,0.352941176471,0.803921568627, 1.0) )
	( "slategray", ColorRGBA(0.439215686275,0.501960784314,0.564705882353, 1.0) )
	( "slategrey", ColorRGBA(0.439215686275,0.501960784314,0.564705882353, 1.0) )
	( "snow", ColorRGBA(1.0,0.980392156863,0.980392156863, 1.0) )
	( "springgreen", ColorRGBA(0.0,1.0,0.498039215686, 1.0) )
	( "steelblue", ColorRGBA(0.274509803922,0.509803921569,0.705882352941, 1.0) )
	( "tan", ColorRGBA(0.823529411765,0.705882352941,0.549019607843, 1.0) )
	( "teal", ColorRGBA(0.0,0.501960784314,0.501960784314, 1.0) )
	( "thistle", ColorRGBA(0.847058823529,0.749019607843,0.847058823529, 1.0) )
	( "tomato", ColorRGBA(1.0,0.388235294118,0.278431372549, 1.0) )
	( "turquoise", ColorRGBA(0.250980392157,0.878431372549,0.81568627451, 1.0) )
	( "violet", ColorRGBA(0.933333333333,0.509803921569,0.933333333333, 1.0) )
	( "wheat", ColorRGBA(0.960784313725,0.870588235294,0.701960784314, 1.0) )
	( "white", ColorRGBA(1.0,1.0,1.0, 1.0) )
	( "whitesmoke", ColorRGBA(0.960784313725,0.960784313725,0.960784313725, 1.0) )
	( "yellow", ColorRGBA(1.0,1.0,0.0, 1.0) )
	( "yellowgreen", ColorRGBA(0.603921568627,0.803921568627,0.196078431373, 1.0) )
	( "none", graphicElements::Style::NONE_COLOR );
}
//-----------------------------------------------------------------------------
ColorRGBA HtmlColors::getColor( const std::string &name )
{
	using boost::algorithm::to_lower_copy;
	if (colorMap.empty())
		initHtmlColors();
	std::string key = to_lower_copy(name);
	ColorMap::const_iterator it = colorMap.find(key);
	if (it==colorMap.end())
		return ColorRGBA::NULL_COLOR;
	return it->second;
}
}}} // namespaces
