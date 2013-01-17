/*
 * IResourceManager.cpp
 *
 *  Created on: Thu Jan  17 11:29:51 2013
 *      Author: Johannes Unger
 */

#include "IResourceManager.hpp"
#include "FileResourceManager.hpp"

namespace sambag { namespace disco {
namespace {
	IResourceManager * currentManager = NULL; 
}
//-----------------------------------------------------------------------------
IResourceManager & getResourceManager() {
	if (!currentManager) { // install default
		currentManager = &FileResourceManager::instance();
	}
	return *currentManager;
}
//-----------------------------------------------------------------------------
IResourceManager * installResourceManager(IResourceManager &m) {
	IResourceManager * old = currentManager;
	currentManager = &m;
	return old;
}

}}
