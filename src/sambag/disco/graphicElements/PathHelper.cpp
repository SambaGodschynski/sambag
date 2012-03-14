/*
 * patArcHelper.cpp
 *
 *  Created on: 02.10.2011
 *      Author: samba
 */

#include "sambag/com/Common.hpp"
#include <boost/tuple/tuple.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <math.h>

namespace sambag { namespace disco { namespace graphicElements { namespace pathHelper {
/*
 *  Conversion from endpoint to center parameterization
 *  see:
 *  http://www.w3.org/TR/2003/REC-SVG11-20030114/implnote.html//ArcImplementationNotes
 *
 *  An elliptical arc is a particular path command.  As such, it is described by
 *  the following parameters in order:
 *  (x0, y0) are the absolute coordinates of the current point on the path, obtained
 *  from the last two parameters of the previous path command.
 *
 *  rX and rY are the radii of the ellipse (also known as its semi-major and semi-minor axes).
 *  phi is the angle from the x-axis of the current coordinate system to the x-axis of the ellipse.
 *
 *  fA is the large arc flag, and is 0 if an arc spanning less than or equal to 180 degrees is chosen,
 *  or 1 if an arc spanning greater than 180 degrees is chosen.
 *
 *  fS is the sweep flag, and is 0 if the line joining center to arc sweeps through decreasing angles,
 *  or 1 if it sweeps through increasing angles.
 *
 *  (x, y) are the absolute coordinates of the final point of the arc.
 *  This parameterization of elliptical arcs will be referred to as endpoint parameterization.
 *  One of the advantages of endpoint parameterization is that it permits a consistent
 *  path syntax in which all path commands end in the coordinates of the new "current point".
 *  The following notes give rules and formulae to help implementers deal with endpoint parameterization.
 */

using namespace boost;
using namespace sambag::com;
using namespace boost::numeric::ublas;
namespace {

/*
 * Modulo for floating numbers
 */
Number floatMod(Number x, Number m = 360.0) {
	Number f = long(x/m);
	Number r = 0.0;
	Number res = 0.0;
	  if (f==0)
	    r = x;
	  else
	    r = (x/f - m) * f;
	  if ( r < 0.0 )
	    res = m - abs(r);
	  else
	    res = 0.0 + r;
	  return res;
}
} // namespace

/**
 * cx, cy, rx, ry,  theta1, delta
 */
typedef boost::tuple<Number, Number, Number, Number, Number, Number> Tuple6;

/**
 * From Endpoint To Center Parameterization
 * ----------------------------------------
 * Implementation based on :
 * 	http://commons.oreilly.com/wiki/index.php/SVG_Essentials/Paths
 *
 * @param x0  startPoint x
 * @param y0  startPoint y
 * @param x   endpoint x
 * @param y   endpoint y
 * @param rx and @param ry are the radii of the ellipse
 *        (also known as its semi-major and semi-minor axes)
 * @param phi rotation angle
 * @param fA is the large arc flag
 * @param fS is the sweep flag
 * @return cx, cy, rx, ry,  theta1, delta
 */
Tuple6 fromEndpointToCenter(
		Number x0, Number y0, Number x, Number y,
		Number rx, Number ry, Number phi,
		Number fA, Number fS)
{

	// Compute 1/2 distance between current and final point
     Number  dx2 = ( x0 -  x) / 2.0;
     Number  dy2 = ( y0 -  y) / 2.0;

    // Convert from degrees to radians
     phi = floatMod(phi);
     Number  phi_r =  M_PI/180.0 * phi;

    // Compute (x1, y1)
     Number  x1 = cos( phi_r) *  dx2 + sin( phi_r) *  dy2;
     Number  y1 = -sin( phi_r) *  dx2 + cos( phi_r) *  dy2;

    // Make sure radii are large enough
     rx = abs(rx);  ry = abs(ry);
     Number  rx_sq =  rx *  rx;
     Number  ry_sq =  ry *  ry;
     Number  x1_sq =  x1 *  x1;
     Number  y1_sq =  y1 *  y1;

    Number radius_check = ( x1_sq /  rx_sq) + ( y1_sq /  ry_sq);
    if ( radius_check > 1) {
         rx *= sqrt( radius_check);
         ry *= sqrt( radius_check);
         rx_sq =  rx *  rx;
         ry_sq =  ry *  ry;
    }

    // Step 2: Compute (cx1, cy1)

     Number  sign = ( fA ==  fS ) ? -1 : 1;
     Number  sq = (( rx_sq *  ry_sq) - ( rx_sq *  y1_sq) - ( ry_sq *  x1_sq)) /
        (( rx_sq *  y1_sq) + ( ry_sq *  x1_sq));
     sq = ( sq < 0.0) ? 0.0 :  sq;
     Number  coef =  sqrt(sq) * sign;
     Number  cx1 =  coef * (( rx *  y1) /  ry);
     Number  cy1 =  coef * -(( ry *  x1) /  rx);

    //   Step 3: Compute (cx, cy) from (cx1, cy1)

     Number  sx2 = ( x0 + x) / 2.0;
     Number  sy2 = ( y0 + y) / 2.0;

     Number  cx =  sx2 + (cos( phi_r) *  cx1 - sin( phi_r) *  cy1);
     Number  cy =  sy2 + (sin( phi_r) *  cx1 + cos( phi_r) *  cy1);

    //   Step 4: Compute angle start and angle extent

     Number  ux = ( x1 -  cx1) /  rx;
     Number  uy = ( y1 -  cy1) /  ry;
     Number  vx = (- x1 -  cx1) /  rx;
     Number  vy = (- y1 -  cy1) /  ry;
     Number  n = sqrt( ( ux *  ux) + ( uy *  uy) );
     Number  p =  ux; // 1 * ux + 0 * uy
     sign = ( uy < 0.0) ? -1.0 : 1.0;

     Number  theta =  sign * acos(  p /  n );
     theta = 180.0 / M_PI * theta;

     n = sqrt(( ux *  ux +  uy *  uy) * ( vx *  vx +  vy *  vy));
     p =  ux *  vx +  uy *  vy;

     // there was a precision problem  in this calculation:
     // when ry was 100. p was -1.000....002. but acos only works with
     // [-1.0 .. 1.0]
     // so:
     p = std::min( std::max(-1.0, p), 1.0 );
     // ------------------------------------
     sign = (( ux *  vy -  uy *  vx) < 0.0) ? -1.0 : 1.0;
     Number  delta =  sign * acos( p /  n );
     delta = 180.0 / M_PI *delta;

    if ( fS == 0.0 &&  delta > 0.0) {
         delta -= 360.0;
    }
    else if ( fS == 1 &&  delta < 0)
    {
         delta += 360.0;
    }

    delta = floatMod(delta);
    theta = floatMod(theta);

	return Tuple6(cx, cy, rx, ry, theta, delta);
}

}}}}

































