/*
 * Thread.hpp
 *
 *  Created on: May 18, 2012
 *      Author: samba
 */

#ifndef SAMBAG_THREAD_HPP_
#define SAMBAG_THREAD_HPP_

#include <boost/thread.hpp>

namespace sambag { namespace com {
	typedef boost::recursive_timed_mutex Mutex;
	struct DeadLockException {};
	inline void wait(long sec) {
		boost::this_thread::sleep(boost::posix_time::seconds(sec));
	}
}} // namespaces

#define SAMBAG_DEADLOCK_EXCEPTION sambag::com::DeadLockException()
#define SAMBAG_LOCK_TIMEOUT 1

#define SAMBAG_TRY_TO_LOCK_TIMED(mutex) boost::unique_lock<sambag::com::Mutex> __lock( (mutex), boost::try_to_lock);\
	if (!__lock.owns_lock()) { __lock.timed_lock(boost::get_system_time() + boost::posix_time::seconds(SAMBAG_LOCK_TIMEOUT)); }\
	if ( !__lock.owns_lock() ) throw SAMBAG_DEADLOCK_EXCEPTION;
#define SAMBAG_TRY_TO_LOCK_TIMED2(mutex,timeout) boost::unique_lock<sambag::com::Mutex> __lock( (mutex), boost::try_to_lock);\
	if (!__lock.owns_lock()) { __lock.timed_lock(boost::get_system_time() + boost::posix_time::seconds(timeout)); }\
	if ( !__lock.owns_lock() ) throw SAMBAG_DEADLOCK_EXCEPTION;

/**
 * usage:
 * SAMBAG_BEGIN_SYNCHRONIZED(mutexObject)
 * 		// do something
 * SAMBAG_END_SYNCHRONIZED
 */
#define SAMBAG_BEGIN_SYNCHRONIZED(mutex) { SAMBAG_TRY_TO_LOCK_TIMED((mutex));
#define SAMBAG_END_SYNCHRONIZED }

#endif /* SAMBAG_THREAD_HPP_ */
