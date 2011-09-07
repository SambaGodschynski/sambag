#include "FileSystem.hpp"
#include <string>

namespace sambag {
namespace com {

//============================================================================================================
// dirWalker: 
// Walks through directory tree. Calls visitor events while walking:
//     - changeDirectory
//     - file
//============================================================================================================
void dirWalker ( const Location &loc, IWalkerVisitor &vis, bool *abort ) {
	using namespace com;
	using namespace boost;	
	using namespace filesystem; // boost::filesystem	
	int files = 0;
	if ( !exists( loc ) ) return;
	directory_iterator end_it;
	directory_iterator it(loc);
	for ( ; it!=end_it; ++it ) {
		
		if ( abort ) 
			if ( *abort ) return;

		if ( is_directory ( *it ) ) {
			if ( !vis.changeDirectory ( *it ) ) continue; // visitor returns false=>skip directory						
			dirWalker ( *it, vis, abort );
		}		
		else vis.file(*it);
	} //for
}
//============================================================================================================
// isSubDirectory: 
// returns true if sub subdirectory of parent
//============================================================================================================
bool isSubDirectory ( const Location &parent,  const Location &sub ) {
	std::string p = parent.string();
	std::string s = sub.string();
	if ( p==s ) return false;
	return s.find (p) != std::string::npos;
}

} // com
} // namespace sambag

