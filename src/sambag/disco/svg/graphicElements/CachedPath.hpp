/*
 * Rect.hpp
 *
 *  Created on: 22.09.2011
 *      Author: samba
 */

#ifndef SAMBAG_CACHED_PATH_HPP
#define SAMBAG_CACHED_PATH_HPP

#include <sambag/disco/IDrawContext.hpp>

namespace sambag { namespace disco { namespace svg { namespace graphicElements {
//=============================================================================
class CachedPath {
//=============================================================================
private:
	Path::Ptr path;
public:
	void appendPath( IDrawContext::Ptr context ) {
        if (!path) {
            return;
        }
        context->appendPath(path);
    }
    bool hasPath() const {
        return path.get() != NULL;
    }
    void copyPath(IDrawContext::Ptr context) {
        this->path = context->copyPath();
    }
    void resetPath() {
        path.reset();
    }
};

}}}} // namespace


#endif /* RECT_HPP_ */
