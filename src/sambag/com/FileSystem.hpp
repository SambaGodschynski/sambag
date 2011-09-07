#ifndef _DIR_WALKER_H_
#define _DIR_WALKER_H_

#include "boost/filesystem.hpp"

namespace sambag {
namespace com {
typedef boost::filesystem::path Location;
//============================================================================================================
// class IWalkerVisitor
//============================================================================================================
struct IWalkerVisitor {
	// called while starting iterate path. If returned with false scanning path will be skipped.
	virtual bool changeDirectory ( const Location & path ) = 0;
	// called when visiting new file. 
	virtual void file ( const Location & file ) = 0;
};

//============================================================================================================
// dirWalker: 
// Walks through directory tree. Calls visitor events while walking:
//     - changeDirectory
//     - file
// Abborts scan when *abort sets to true. 
//============================================================================================================
void dirWalker ( const Location &loc, IWalkerVisitor &vis, bool *abort = NULL );
//============================================================================================================
// isSubDirectory: 
// returns true if sub subdirectory of parent
//============================================================================================================
bool isSubDirectory ( const Location &parent,  const Location &sub );
} // com
} // namespace sambag

#endif
