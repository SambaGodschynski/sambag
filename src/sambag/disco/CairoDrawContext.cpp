/*
 * CairoDrawContext.cpp
 *
 *  Created on: 11.09.2011
 *      Author: samba
 */

#include "CairoDrawContext.hpp"
#include "CairoSurface.hpp"
#include <sambag/com/Common.hpp>
#include "IDiscoFactory.hpp"


bool isFatalCairoStatus(cairo_status_t status) {
	if (status==CAIRO_STATUS_INVALID_STRING)
		return false;
	return true;
}

namespace sambag { namespace disco {
//=============================================================================
//  Class CairoDrawContext:
//    implements IDrawContext with Cairos draw operations.
//=============================================================================
//-----------------------------------------------------------------------------
CairoDrawContext::CairoDrawContext(CairoSurface::Ptr _surface)
: patternInUse(INVALID), surfaceRef(_surface), context(NULL)
{
	surface = surfaceRef->getCairoSurface();
	context = cairo_create(surface);
	setFont(currentFont);
}
//-----------------------------------------------------------------------------
CairoDrawContext::~CairoDrawContext() {
	cairo_destroy(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::_save() {
	stack.push_back(
			StateInfo(strokePattern, fillPattern, currentFont, currentDash)
	);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::_restore() {
	if (stack.empty())
		return;
	boost::tie(strokePattern, fillPattern, currentFont, currentDash) = stack.back();
	stack.pop_back();
	patternInUse = INVALID;
}
//-----------------------------------------------------------------------------
void CairoDrawContext::rect(const Rectangle &rect, const Number &cornerRadius) {
	Number radius = cornerRadius;
	Number degrees = M_PI / 180.0;

	Number width = rect.getWidth();
	Number height = rect.getHeight();
	Number x = rect.x0().x();
	Number y = rect.x0().y();

	cairo_new_sub_path (context);
	cairo_arc (context, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
	cairo_arc (context, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
	cairo_arc (context, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
	cairo_arc (context, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
	cairo_close_path (context);

}
//-----------------------------------------------------------------------------
void CairoDrawContext::setFillPattern(IPattern::Ptr pattern) {
	fillPattern = boost::dynamic_pointer_cast<CairoPatternBase>(pattern);
	patternInUse = INVALID;
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setStrokePattern(IPattern::Ptr pattern) {
	strokePattern = boost::dynamic_pointer_cast<CairoPatternBase>(pattern);
	patternInUse = INVALID;
}
//-----------------------------------------------------------------------------
void CairoDrawContext::drawSurface(ISurface::Ptr _surface, Number opacity) {
	CairoSurface::Ptr surface = boost::dynamic_pointer_cast<CairoSurface>(_surface);
	if (!_surface)
		return;
	cairo_surface_t *png = surface->getCairoSurface();
	if (!png)
		return;
	cairo_set_source_surface (context, png, 0, 0);
	if (opacity == 1.0) {
		cairo_paint (context);
	} else {
		cairo_paint_with_alpha(context, opacity);
	}
	// bug/curiosity:
	// color settings has to be reseted.
	setFillColor(getFillColor());
	setStrokeColor(getStrokeColor());
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setFillPattern() {
	if (!fillPattern)
		return;
	cairo_set_source(context, fillPattern->getCairoPatternRef().get());
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setStrokePattern() {
	if (!strokePattern)
		return;
	cairo_set_source(context, strokePattern->getCairoPatternRef().get());
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setClip(const Rectangle &r) {
	rect(r);
	clip();
}
//-----------------------------------------------------------------------------
void CairoDrawContext::copyTo(IDrawContext::Ptr cn) const
{
	CairoDrawContext::Ptr dst =
		boost::dynamic_pointer_cast<CairoDrawContext>(cn);
	SAMBAG_ASSERT(dst);
	cairo_set_source_surface (dst->getCairoContext(),
			surfaceRef->getCairoSurface(),
			0, 0);
	cairo_paint (dst->getCairoContext());
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::copyAreaTo(IDrawContext::Ptr cn,
		const Rectangle &src, const Point2D &dest) const
{
	CairoDrawContext::Ptr dstCn =
		boost::dynamic_pointer_cast<CairoDrawContext>(cn);
	SAMBAG_ASSERT(dstCn);
	cairo_t *cr = dstCn->getCairoContext();
	cairo_set_source_surface (cr,
			surfaceRef->getCairoSurface(),
			dest.x() - src.x0().x(), dest.y() - src.x0().y());

	cairo_rectangle (cr, dest.x(), dest.y(), src.width(), src.height());

	cairo_fill (cr);
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
Number CairoDrawContext::getCurrentFontHeight() const {
	cairo_font_extents_t res = {0};
	cairo_font_extents(context, &res);
	SAMBAG_CHECK_CONTEXT_STATE(context);
	return res.ascent;
}
//-----------------------------------------------------------------------------
void CairoDrawContext::moveTo( const Point2D &p0 ) {
	cairo_move_to ( context, p0.x(), p0.y() );
}
//-----------------------------------------------------------------------------
void CairoDrawContext::lineTo( const Point2D &p0 ) {
	cairo_line_to ( context, p0.x(), p0.y() );
}
//-----------------------------------------------------------------------------
void CairoDrawContext::relMoveTo( const Point2D &p0 ) {
	if (!cairo_has_current_point(context)) {
		cairo_move_to ( context, p0.x(), p0.y() );
		return;
	}
	cairo_rel_move_to ( context, p0.x(), p0.y() );
}
//-----------------------------------------------------------------------------
void CairoDrawContext::relLineTo( const Point2D &p0 ) {
	if (!cairo_has_current_point(context)) {
		cairo_line_to ( context, p0.x(), p0.y() );
		return;
	}
	cairo_rel_line_to ( context, p0.x(), p0.y() );
}
//-----------------------------------------------------------------------------
void CairoDrawContext::closePath() {
	cairo_close_path(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::curveTo(
	const Point2D &x1,
	const Point2D &x2,
	const Point2D &x3 )
{
	cairo_curve_to(
		context, x1.x(), x1.y(), x2.x(), x2.y(), x3.x(), x3.y()
	);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::relCurveTo(
	const Point2D &x1,
	const Point2D &x2,
	const Point2D &x3 )
{
	if (!cairo_has_current_point(context)) {
		moveTo(Point2D(0,0));
	}
	cairo_rel_curve_to(
		context, x1.x(), x1.y(), x2.x(), x2.y(), x3.x(), x3.y()
	);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::quadraticCurveTo( const Point2D &x1, const Point2D &x2 ) {
	Point2D x0 = getCurrentPoint();
	cairo_curve_to (context,
	                2.0 / 3.0 * x1.x() + 1.0 / 3.0 * x0.x(),
	                2.0 / 3.0 * x1.y() + 1.0 / 3.0 * x0.y(),
	                2.0 / 3.0 * x1.x() + 1.0 / 3.0 * x2.x(),
	                2.0 / 3.0 * x1.y() + 1.0 / 3.0 * x2.y(),
	                x2.x(), x2.y());
}
//-----------------------------------------------------------------------------
void CairoDrawContext::relQuadraticCurveTo( const Point2D &x1, const Point2D &x2 ) {
	if (!cairo_has_current_point(context)) {
		moveTo(Point2D(0,0));
	}
	Point2D x0 = Point2D(0,0);
	cairo_rel_curve_to (context,
	                    2.0 / 3.0 * x1.x() + 1.0 / 3.0 * x0.x(),
	                    2.0 / 3.0 * x1.y() + 1.0 / 3.0 * x0.y(),
	                    2.0 / 3.0 * x1.x() + 1.0 / 3.0 * x2.x(),
	                    2.0 / 3.0 * x1.y() + 1.0 / 3.0 * x2.y(),
	                    x2.x(), x2.y());
}
//-----------------------------------------------------------------------------
void CairoDrawContext::arc(
	const Point2D &c,
	const Coordinate &r,
	const Coordinate &angle1,
	const Coordinate &angle2)
{
	cairo_arc( context, c.x(), c.y(), r, angle1, angle2 );
}
//-----------------------------------------------------------------------------
void CairoDrawContext::arcNegative(
	const Point2D &c,
	const Coordinate &r,
	const Coordinate &angle1,
	const Coordinate &angle2 )
{
	cairo_arc_negative( context, c.x(), c.y(), r, angle1, angle2 );
}
//-----------------------------------------------------------------------------
void CairoDrawContext::rect( const Rectangle &rect ) {
	cairo_rectangle(
		context,
		rect.x0().x(),
		rect.x0().y(),
		rect.getWidth(),
		rect.getHeight()
	);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::stroke() {
	if (!isStroked())
		return;
	if (patternInUse != STROKE) {
		setStrokePattern();
		patternInUse = STROKE;
	}
	cairo_stroke(context);
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::fill() {
	if (!isFilled())
		return;
	if (patternInUse != FILL) {
		setFillPattern();
		patternInUse = FILL;
	}
	cairo_fill(context);
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::fillPreserve() {
	if (!isFilled())
		return;
	if (!isFilled())
		return;
	if (patternInUse != FILL) {
		setFillPattern();
		patternInUse = FILL;
	}
	cairo_fill_preserve(context);
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setFont( const Font &font ) {
	cairo_select_font_face (
		context,
		font.fontFace.c_str(),
		(cairo_font_slant_t)font.slant,
		(cairo_font_weight_t)font.weight
	);
	cairo_set_font_size (context, font.size);
	currentFont = font;
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setFontFace(const Font::FontFace &fontFace) {
	currentFont.fontFace = fontFace;
	cairo_select_font_face (
		context,
		currentFont.fontFace.c_str(),
		(cairo_font_slant_t)currentFont.slant,
		(cairo_font_weight_t)currentFont.weight
	);
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setFontWeight(const Font::Weight &weight) {
	currentFont.weight = weight;
	cairo_select_font_face (
		context,
		currentFont.fontFace.c_str(),
		(cairo_font_slant_t)currentFont.slant,
		(cairo_font_weight_t)currentFont.weight
	);
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setFontSlant(const Font::Slant &slant) {
	currentFont.slant = slant;
	cairo_select_font_face (
		context,
		currentFont.fontFace.c_str(),
		(cairo_font_slant_t)currentFont.slant,
		(cairo_font_weight_t)currentFont.weight
	);
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setFontSize(const Font::Size &size) {
	currentFont.size = size;
	cairo_set_font_size (context, currentFont.size);
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
Font CairoDrawContext::getCurrentFont() const {
	return currentFont;
}
//-----------------------------------------------------------------------------
void CairoDrawContext::textPath( const std::string &text ) {
	cairo_text_path( context, text.c_str() );
}
//-----------------------------------------------------------------------------
Rectangle CairoDrawContext::textExtends(const std::string &str) const {
	cairo_text_extents_t ex;
	cairo_text_extents(context, str.c_str(), &ex);
	Point2D p = getCurrentPoint();
	boost::geometry::add_point(p, Point2D(ex.x_bearing, ex.y_bearing));
	return Rectangle(p, ex.width, ex.height);
}
//-----------------------------------------------------------------------------
CairoDrawContext::TextExtends
CairoDrawContext::textExtendsX(const std::string &str) const 
{
	cairo_text_extents_t ex;
	TextExtends res = {0};

	cairo_text_extents(context, str.c_str(), &ex);
    res.x_bearing = ex.x_bearing;
    res.y_bearing = ex.y_bearing;
    res.width = ex.width;
    res.height = ex.height;
    res.x_advance = ex.x_advance;
    res.y_advance = ex.y_advance;
	return res;
}
//-----------------------------------------------------------------------------
bool CairoDrawContext::isStroked() const {
	if (!strokePattern) {
		return false;
	}
	return strokePattern->getOpacity() > 0.0;
}
//-----------------------------------------------------------------------------
bool CairoDrawContext::isFilled() const {
	if (!fillPattern) {
		return false;
	}
	return fillPattern->getOpacity() > 0.0;
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setStrokeWidth( const Coordinate &val ) {
	cairo_set_line_width(context, val);
}
//-----------------------------------------------------------------------------
Coordinate CairoDrawContext::getStrokeWidth() const {
	return Coordinate(cairo_get_line_width(context));
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setFillColor( const ColorRGBA &val ) {
	patternInUse = INVALID;
	fillPattern = CairoSolidPattern::create(val);
}
//-----------------------------------------------------------------------------
ColorRGBA CairoDrawContext::getFillColor() const {
	CairoSolidPattern::Ptr sPt =
			boost::dynamic_pointer_cast<CairoSolidPattern>(fillPattern);
	if (!sPt)
		return false;
	return sPt->getSolidColor();
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setStrokeColor( const ColorRGBA &val ) {
	patternInUse = INVALID;
	strokePattern = CairoSolidPattern::create(val);
}
//-----------------------------------------------------------------------------
ColorRGBA CairoDrawContext::getStrokeColor() const {
	CairoSolidPattern::Ptr sPt =
			boost::dynamic_pointer_cast<CairoSolidPattern>(strokePattern);
	if (!sPt)
		return false;
	return sPt->getSolidColor();
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setDash( Dash::Ptr dash ) {
	currentDash = dash;
	if (!dash) {
		disableDash();
		return;
	}
	// copy dashes to plain array
	boost::scoped_array<Number> bff(new Number[dash->size()]);
	for(size_t i=0; i<dash->size(); ++i) {
		bff[i] = (*dash)[i];
	}
	cairo_set_dash(
		context,
		bff.get(),
		dash->size(),
		dash->offset()
	);
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::resetClip() {
	cairo_reset_clip( context );
}
//-----------------------------------------------------------------------------
void CairoDrawContext::disableDash() {
	cairo_set_dash( context, NULL, 0, 0 );
}
//-----------------------------------------------------------------------------
Dash::Ptr CairoDrawContext::getDash() const {
	return currentDash;
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setLineCap ( LineCapStyle style ) {
	cairo_set_line_cap( context, (cairo_line_cap_t)style );
}
//-----------------------------------------------------------------------------
CairoDrawContext::LineCapStyle CairoDrawContext::getLineCap () const {
	return (LineCapStyle)cairo_get_line_cap(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setFillRule( FillRule rule ) {
	cairo_set_fill_rule(context, (cairo_fill_rule_t)rule);
}
//-----------------------------------------------------------------------------
CairoDrawContext::FillRule CairoDrawContext::getFillRule() const {
	return (FillRule)cairo_get_fill_rule(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setLineJoin(LineJoin join) {
	cairo_set_line_join(context, (cairo_line_join_t)join);
}
//-----------------------------------------------------------------------------
CairoDrawContext::LineJoin CairoDrawContext::getLineJoin() const {
	return (LineJoin)cairo_get_line_join(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setMiterLimit(const Number &v) {
	cairo_set_miter_limit(context, v);
}
//-----------------------------------------------------------------------------
Number CairoDrawContext::getMiterLimit() const {
	return cairo_get_miter_limit(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::save() {
	_save();
	cairo_save(context);
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::restore() {
	_restore();
	cairo_restore(context);
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
Point2D CairoDrawContext::getCurrentPoint() const {
	if (!cairo_has_current_point(context)) {
		return Point2D(0,0);
	}
	Number x=0, y=0;
	cairo_get_current_point( context, &x, &y );
	return Point2D(x,y);
}
//-----------------------------------------------------------------------------
bool CairoDrawContext::hasCurrentPoint() const {
	if(cairo_has_current_point(context))
		return true;
	return false;
}
//-----------------------------------------------------------------------------
void CairoDrawContext::clip() {
	cairo_clip(context);
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
Rectangle CairoDrawContext::clipExtends() const {
	double x0, y0, x1, y1;
	x0 = y0 = x1 = y1 = NULL_NUMBER;
	cairo_clip_extents(context, &x0, &y0, &x1, &y1);
	return Rectangle(Point2D(x0,y0), Point2D(x1, y1), false);
}
//-----------------------------------------------------------------------------
Path::Ptr CairoDrawContext::copyPath() const {
	cairo_path_t * path = cairo_copy_path(context);
	SAMBAG_CHECK_CONTEXT_STATE(context);
	return CairoPath::create(path);
}
//-----------------------------------------------------------------------------
Path::Ptr CairoDrawContext::copyPathFlat() const {
	cairo_path_t * path = cairo_copy_path_flat(context);
	SAMBAG_CHECK_CONTEXT_STATE(context);
	return CairoPath::create(path);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::appendPath( Path::Ptr path ) {
	CairoPath::Ptr cp = boost::dynamic_pointer_cast<CairoPath, Path>(path);
	if (!cp) return;
	cairo_append_path(context, cp->getPath());
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::newSubPath() {
	cairo_new_sub_path(context);
}
//-----------------------------------------------------------------------------
ISurface::Ptr CairoDrawContext::getSurface() const {
	return surfaceRef;
}
//-----------------------------------------------------------------------------
Rectangle CairoDrawContext::pathExtends() const {
	Number x,y,x1,y1;
	cairo_path_extents(context, &x, &y, &x1, &y1);
	return Rectangle(Point2D(x,y), Point2D(x1, y1), false);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::translate( const Point2D &p0 ) {
	cairo_translate( context, p0.x(), p0.y() );
}
//-----------------------------------------------------------------------------
void CairoDrawContext::scale( const Point2D &p0 ) {
	cairo_scale( context, p0.x(), p0.y() );
}
//-----------------------------------------------------------------------------
void CairoDrawContext::rotate( const Number &angle ) {
	cairo_rotate( context, angle );
}
//-----------------------------------------------------------------------------
void CairoDrawContext::identityMatrix() {
	cairo_identity_matrix( context );
}
//-----------------------------------------------------------------------------
void CairoDrawContext::transform ( const Matrix &m ) {
	if ( m.size1() != 3 || m.size2() != 3 ) return;
	cairo_matrix_t cm;
	discoMatrixToCairoMatrix( m, cm );
	cairo_transform(context, &cm);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::getMatrix( Matrix &m ) {
	if ( m.size1() != 3 || m.size2() != 3 ) return;
	cairo_matrix_t cm;
	cairo_get_matrix(context, &cm);
	cairoMatrixToDiscoMatrix(cm, m);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::deviceToUser(Point2D &inOut) {
    double x=inOut.x();
    double y=inOut.y();
    cairo_device_to_user(context, &x, &y);
    inOut.x(x);
    inOut.y(y);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::userToDevice(Point2D &inOut) {
    double x=inOut.x();
    double y=inOut.y();
    cairo_user_to_device(context, &x, &y);
    inOut.x(x);
    inOut.y(y);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::deviceToUserDistance(Point2D &inOut) {
    double x=inOut.x();
    double y=inOut.y();
    cairo_device_to_user_distance(context, &x, &y);
    inOut.x(x);
    inOut.y(y);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::userToDeviceDistance(Point2D &inOut) {
    double x=inOut.x();
    double y=inOut.y();
    cairo_user_to_device_distance(context, &x, &y);
    inOut.x(x);
    inOut.y(y);
}
}} // namespaces
