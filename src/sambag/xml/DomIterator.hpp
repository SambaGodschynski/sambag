/*
 * DomIterator.hpp
 *
 *  Created on: 17.10.2011
 *      Author: samba
 */

#ifndef DOMITERATOR_HPP_
#define DOMITERATOR_HPP_

#include "DomObject.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>

namespace sambag { namespace xml {
//=============================================================================
/**
*  @class DomIterator.
*  A Iterator with filter suppot to simplyfiy finding objects.
*  example:
*    it.allTagsWith("[a-z]*");
*    obj = *(++it);
*/
typedef DomObject::DomObjects::iterator DomIteratorBase;
class DomIterator : public DomIteratorBase {
//=============================================================================
private:
	//-------------------------------------------------------------------------
	enum SearchFor {
		NOTHING,
		CLASSES,
		TAGS
	};
	//-------------------------------------------------------------------------
	SearchFor searchFor;
	//-------------------------------------------------------------------------
	boost::regex re;
	//-------------------------------------------------------------------------
	const DomIteratorBase &end;
public:
	//-------------------------------------------------------------------------
	DomIterator(const DomIteratorBase &baseIt, const DomIteratorBase &end) :
		DomIteratorBase(baseIt), searchFor(NOTHING), end(end)
	{
	}
	//-------------------------------------------------------------------------
	void allTagsWith(const std::string &regexStr) {
		searchFor = TAGS;
		re = boost::regex(regexStr);
	}
	//-------------------------------------------------------------------------
	void allTagsWith(const boost::regex &_re) {
		searchFor = TAGS;
		re = _re;
	}
	//-------------------------------------------------------------------------
	void allClassesWith(const std::string &regexStr) {
		searchFor = CLASSES;
		re = boost::regex(regexStr);
	}
	//-------------------------------------------------------------------------
	void allClassesWith(const boost::regex &_re) {
		searchFor = CLASSES;
		re = _re;
	}
	//-------------------------------------------------------------------------
	void next();
};
}} // namespace



#endif /* DOMITERATOR_HPP_ */
