/*
 * TestDefaultSelectionModel.cpp
 *
 *  Created on: Wed Feb 20 11:09:04 2013
 *      Author: Johannes Unger
 */

#include "TestDefaultSelectionModel.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/components/DefaultListSelectionModel.hpp>
#include <exception>
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestDefaultSelectionModel );

namespace tests {
namespace {
using namespace sambag::disco::components;
void setSelection(int index, DefaultListSelectionModel &model) {
	model.setSelectionInterval(index, index);
	if (!model.isSelectedIndex(index)) {
		throw std::runtime_error("setSelection failed.");
	}
}
void selectDeselectLin(int start, int end, DefaultListSelectionModel &model) {
	for (int i=0; i<end; ++i) {
		setSelection(i, model);
	}
}
void selectDeselectRnd(int max, DefaultListSelectionModel &model) {
	int i = rand() % max;
	setSelection(i, model);
}
} // namespace(s)
//=============================================================================
//  Class TestDefaultSelectionModel
//=============================================================================
//-----------------------------------------------------------------------------
void TestDefaultSelectionModel::issue309() {
	using namespace sambag::disco::components;
	{
		DefaultListSelectionModel model;
		selectDeselectLin(0, 1000, model);
	}
	for (int i=0; i<100; ++i) {
		for (int j=0; j<100; ++j) {
			DefaultListSelectionModel model;
			selectDeselectRnd(100000, model);
		}
	}
}
} //namespace
