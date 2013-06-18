/*
 * ADiscoFactory.cpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#include "IDiscoFactory.hpp"
#include "CairoDiscoFactory.hpp"
#include <loki/Singleton.h>
namespace sambag { namespace disco {
namespace {
    struct DummyFactory : public IDiscoFactory {
        virtual std::string implDetails() const {
            return "";
        }
        virtual IDrawContext::Ptr createContext( ISurface::Ptr surface ) const {
            return IDrawContext::Ptr();
        }
        virtual IDrawContext::Ptr createContext() const {
            return IDrawContext::Ptr();
        }
        virtual IImageSurface::Ptr
        createImageSurface(const Integer &width, const Integer &height) const {
            return IImageSurface::Ptr();
        }
        virtual IImageSurface::Ptr
        createImageSurface(IDataHandler::Ptr) const {
            return IImageSurface::Ptr();
        }
        virtual IRecordingSurface::Ptr
        createRecordingSurface() const {
            return IRecordingSurface::Ptr();
        }
        virtual IRecordingSurface::Ptr
        createRecordingSurface(const Integer &width, const Integer &height) const {
            return IRecordingSurface::Ptr();
        }
        virtual ISolidPattern::Ptr createSolidPattern(const ColorRGBA &col) const {
            return ISolidPattern::Ptr();
        }
        virtual ILinearPattern::Ptr
        createLinearPattern(Point2D p0, Point2D p1) const {
            return ILinearPattern::Ptr();
        }
        virtual IRadialPattern::Ptr
        createRadialPattern(Point2D c0, Number r0,  Point2D c1, Number r1) const {
            return IRadialPattern::Ptr();
        }
        virtual ISurfacePattern::Ptr
        createSurfacePattern(ISurface::Ptr surf) const {
            return ISurfacePattern::Ptr();
        }
    };
    //typedef Loki::SingletonHolder<CairoDiscoFactory> DiscoFactoryHolder;
    typedef Loki::SingletonHolder<DummyFactory> DiscoFactoryHolder;
}
//-----------------------------------------------------------------------------
IDiscoFactory * getDiscoFactory() {
	return &DiscoFactoryHolder::Instance();
}
}} // namespace
