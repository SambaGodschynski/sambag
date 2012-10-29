/*
 * One2One.hpp
 *
 *  Created on: Thu Oct 18 14:08:27 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ONE2ONE_H
#define SAMBAG_ONE2ONE_H

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <sambag/com/Exception.hpp>
#include <boost/tuple/tuple.hpp>

namespace sambag { namespace com {
//=============================================================================
/** 
  * @class One2One.
  * Like singleton but with key value.
  * Creates one object (using creation policy) per key.
  * @note: keyvalue has to be hashable by boost::hash_value()
  *
  * CreatorPolicy concept:
  *
  * struct CreatorPolicy {
  *		ResultType create(key);
  * };
  */
template <
	class _ResultType,
	template <class> class _CreatorPolicy,
	class _KeyType
>
class One2One : public _CreatorPolicy<_ResultType> {
//=============================================================================
public:
	typedef _ResultType ResultType;
	typedef _CreatorPolicy<ResultType> CreatorPolicy;
	typedef _KeyType KeyType;
	typedef One2One<ResultType, _CreatorPolicy, KeyType> Class;
private:
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<KeyType, ResultType> Map;
	//-------------------------------------------------------------------------
	Map map;
public:
	//-------------------------------------------------------------------------
	ResultType & get(const KeyType &k) {
		Map::iterator it = map.find(k);
		if (it==map.end()) { // create result type object 
			ResultType res = typename CreatorPolicy::create(k);
			bool inserted = false;
			boost::tie(it, inserted) = map.insert(Map::value_type(k, res));
			SAMBAG_ASSERT(inserted);
		}
		return it->second;
	}
	//-------------------------------------------------------------------------
	void remove(const KeyType &k) {
		Map::iterator it = map.find(k);
		if (it==map.end()) {
			return;
		}
		map.erase(k);
	}
	//-------------------------------------------------------------------------
	size_t getSize() const {
		return map.size();
	}


}; // One2One
}} // namespace(s)

#endif /* SAMBAG_ONE2ONE_H */
