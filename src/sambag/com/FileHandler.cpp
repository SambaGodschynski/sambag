/*
 * PngHandler.cpp
 *
 *  Created on: 11.10.2011
 *      Author: samba
 */

#include "FileHandler.hpp"
#include "FileSystem.hpp"

namespace {
//----------------------------------------------------------------------------
void destroyFStream( std::fstream *fs ) {
	fs->close();
	delete fs;
}
} // namespace


namespace sambag { namespace com {
//=============================================================================
// PngHandler
//=============================================================================
//-----------------------------------------------------------------------------
FileHandler::FileHandler(StreamPtr stream) : is(stream) {
}
//-----------------------------------------------------------------------------
FileHandler::~FileHandler() {
}
//-----------------------------------------------------------------------------
FileHandler::Ptr FileHandler::create(const std::string &location) {
	if (!boost::filesystem::exists(location)) {
		return FileHandler::Ptr();
	}
	FStreamPtr fs(new std::fstream(location.c_str(), std::ios::in), &destroyFStream);
	return FileHandler::create(fs);
}


}} // namespace
