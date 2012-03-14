/*
 * SimpleTagCreator.hpp
 *
 *  Created on: Dec 10, 2011
 *      Author: samba
 */

#ifndef SIMPLETAGCREATOR_HPP_
#define SIMPLETAGCREATOR_HPP_

#include "MarkupBuilder.hpp"

namespace sambag { namespace xml { namespace cpAtl {
//=============================================================================
class SimpleTagCreator : public MarkupBuilder<SimpleTagCreator> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	SimpleTagCreator(ostream &os) : MarkupBuilder<SimpleTagCreator>(os) {}
	//-------------------------------------------------------------------------
	virtual ~SimpleTagCreator() {}
	//-------------------------------------------------------------------------
	SimpleTagCreator & addTag(const string &tagName) {
		return start(tagName);
	}
	//-------------------------------------------------------------------------
	virtual SimpleTagCreator & getSelf() {
		return *this;
	}
};
}}}
#endif /* SIMPLETAGCREATOR_HPP_ */
