/*
 * Graphics.hpp
 *
 *  Created on: Wed May 23 11:06:47 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_GRAPHICS_H
#define SAMBAG_GRAPHICS_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/IDrawContext.hpp>

namespace sambag { namespace disco { namespace components {

//=============================================================================
/** 
  * @class Graphics.
  * Extends IDrawContext with useful functions
  */
class Graphics : public IDrawContext {
//=============================================================================
public:
protected:
	//-------------------------------------------------------------------------
	IDrawContext::Ptr cn;
private:
public: /*Interface impl.*/
	virtual void moveTo(const Point2D &p0) { cn->moveTo(p0); }
	virtual void lineTo(const Point2D &p0) { cn->lineTo(p0); }
	virtual void relMoveTo(const Point2D &p0) { cn->relMoveTo(p0); }
	virtual void relLineTo(const Point2D &p0) { cn->relLineTo(p0); }
	virtual void closePath() { cn->closePath(); }
	virtual void curveTo(const Point2D &x1, const Point2D &x2,
			const Point2D &x3)
	{
		cn->curveTo(x1, x2, x3);
	}
	virtual void relCurveTo(const Point2D &x1, const Point2D &x2,
			const Point2D &x3)
	{
		cn->relCurveTo(x1, x2, x3);
	}
	virtual void quadraticCurveTo(const Point2D &x1, const Point2D &x2) {
		cn->quadraticCurveTo(x1, x2);
	}
	virtual void relQuadraticCurveTo(const Point2D &x1, const Point2D &x2) {
		cn->relQuadraticCurveTo(x1, x2);
	}
	virtual void arc(const Point2D &c, const Coordinate &r,
					const Coordinate &angle1 = 0,
					const Coordinate &angle2 = M_PI * 2.0)
	{
		cn->arc(c, r, angle1, angle2);
	}
	virtual void arcNegative(const Point2D &c, const Coordinate &r,
					const Coordinate &angle1 = 0,
					const Coordinate &angle2 = M_PI * 2.0)
	{
		cn->arcNegative(c, r, angle1, angle2);
	}
	virtual void rect(const Rectangle &rect) { cn->rect(rect); }
	virtual void rect(const Rectangle &rect, const Number &cornerRadius) {
		cn->rect(rect, cornerRadius); }
	virtual void fill() { cn->fill(); }
	virtual void fillPreserve() { cn->fillPreserve(); }
	virtual void stroke() { cn->stroke(); }
	virtual void drawSurface(ISurface::Ptr s, Number opacity = 1.0) {
		cn->drawSurface(s, opacity);
	}
	virtual void setFont(const Font &font) {
		cn->setFont(font);
	}
	virtual void setFontFace(const Font::FontFace &fontFace) {
		cn->setFontFace(fontFace);
	}
	virtual void setFontWeight(const Font::Weight &weight) {
		cn->setFontWeight(weight);
	}
	virtual void setFontSlant(const Font::Slant &slant) {
		cn->setFontSlant(slant);
	}
	virtual void setFontSize(const Font::Size &size) {
		cn->setFontSize(size);
	}
	virtual Font getCurrentFont() const { return cn->getCurrentFont(); }
	virtual void textPath(const std::string &text) {
		cn->textPath(text);
	}
	virtual void setStrokeWidth(const Coordinate &val) {
		cn->setStrokeWidth(val);
	}
	virtual Coordinate getStrokeWidth() const { return cn->getStrokeWidth(); }
	virtual void setFillColor(const ColorRGBA &val) {
		cn->setFillColor(val);
	}
	virtual ColorRGBA getFillColor() const { return cn->getFillColor(); }
	virtual bool isStroked() const { return cn->isStroked(); }
	virtual bool isFilled() const { return cn->isFilled(); }
	virtual void setStrokeColor(const ColorRGBA &val) {
		cn->setStrokeColor(val);
	}
	virtual ColorRGBA getStrokeColor() const { return cn->getStrokeColor(); }
	virtual void setDash(Dash::Ptr dash) {
		cn->setDash(dash);
	}
	virtual Dash::Ptr getDash() const { return cn->getDash(); }
	virtual void disableDash() { cn->disableDash(); }
	virtual void setLineCap(LineCapStyle style) { cn->setLineCap(style); }
	virtual LineCapStyle getLineCap() const { return cn->getLineCap(); }
	virtual void setFillRule(FillRule rule) { cn->setFillRule(rule); }
	virtual FillRule getFillRule() const { return cn->getFillRule(); }
	virtual void setFillPattern(IPattern::Ptr pattern) {
		cn->setFillPattern(pattern);
	}
	virtual void setStrokePattern(IPattern::Ptr pattern) {
		cn->setStrokePattern(pattern);
	}
	virtual void setLineJoin(LineJoin join) { cn->setLineJoin(join); }
	virtual LineJoin getLineJoin() const { return cn->getLineJoin(); }
	virtual void setMiterLimit(const Number &v) { cn->setMiterLimit(v); }
	virtual Number getMiterLimit() const { return cn->getMiterLimit(); }
	virtual void save() { cn->save(); }
	virtual void restore() { cn->restore(); }
	virtual Point2D getCurrentPoint() const { return cn->getCurrentPoint(); }
	virtual bool hasCurrentPoint() const { return cn->hasCurrentPoint(); }
	virtual void clip() { cn->clip(); }
	virtual Rectangle clipExtends() const { return cn->clipExtends(); }
	virtual void setClip(const Rectangle &r) { cn->setClip(r); }
	virtual void newSubPath() { cn->newSubPath(); }
	virtual Rectangle textExtends(const std::string &str) const {
		return cn->textExtends(str);
	}
	virtual Path::Ptr copyPath() const { return cn->copyPath(); }
	virtual Path::Ptr copyPathFlat() const { return cn->copyPathFlat(); }
	virtual void appendPath(Path::Ptr path) { cn->appendPath(path); }
	virtual ISurface::Ptr getSurface() const { return cn->getSurface(); }
	virtual Rectangle pathExtends() const { return cn->pathExtends(); }
	virtual void translate(const Point2D &p0) { cn->translate(p0); }
	virtual void scale(const Point2D &p0) { cn->scale(p0); }
	virtual void rotate(const Number &angle) { cn->rotate(angle); }
	virtual void identityMatrix() { cn->identityMatrix(); }
	virtual void transform(const Matrix &m) { cn->transform(m); }
	virtual void getMatrix(Matrix &m) { cn->getMatrix(m); }
public:
	//-------------------------------------------------------------------------
	IDrawContext::Ptr getPtr() const {
		return cn;
	}
	//-------------------------------------------------------------------------
	Graphics(IDrawContext::Ptr context) : cn(context) {}
	//-------------------------------------------------------------------------
	void clearRect(const Rectangle &r);
	//-------------------------------------------------------------------------
	bool hitClip(const Rectangle &r) const;
}; // Graphics
//=============================================================================
/**
  * @class ScratchGraphics.
  * automatic scope-related(RII) context save/restore.
  */
class ScratchGraphics : public Graphics {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	ScratchGraphics(IDrawContext::Ptr context) : Graphics(context) {
		cn->save();
	}
	//-------------------------------------------------------------------------
	~ScratchGraphics() {
		cn->restore();
	}
}; // Graphics
}}} // namespace(s)

#endif /* SAMBAG_GRAPHICS_H */