/*
 * Thread.hpp
 *
 *  Created on: May 18, 2012
 *      Author: samba
 */

#ifndef SAMBAG_THREAD_HPP_
#define SAMBAG_THREAD_HPP_

#include <boost/thread.hpp>
#include <sambag/com/Exception.hpp>

namespace sambag { namespace com {
	typedef boost::recursive_timed_mutex RecursiveMutex;
	typedef boost::timed_mutex Mutex;
	SAMBAG_EXCEPTION_CLASS(DeadLockException);
	inline void wait(long sec) {
		boost::this_thread::sleep(boost::posix_time::seconds(sec));
	}
}} // namespaces

#define SAMBAG_DEADLOCK_EXCEPTION sambag::com::DeadLockException
#define SAMBAG_LOCK_TIMEOUT 10

#define SAMBAG_TRY_TO_LOCK_TIMED(mutex) boost::unique_lock<sambag::com::Mutex> __lock( (mutex), boost::try_to_lock);\
	if (!__lock.owns_lock()) { __lock.timed_lock(boost::get_system_time() + boost::posix_time::seconds(SAMBAG_LOCK_TIMEOUT)); }\
	if ( !__lock.owns_lock() ) SAMBAG_THROW(SAMBAG_DEADLOCK_EXCEPTION, "deadlock exception");

#define SAMBAG_TRY_TO_LOCK_RECURSIVE(mutex) boost::unique_lock<sambag::com::RecursiveMutex> __lock( (mutex), boost::try_to_lock);\
	if (!__lock.owns_lock()) { __lock.timed_lock(boost::get_system_time() + boost::posix_time::seconds(SAMBAG_LOCK_TIMEOUT)); }\
	if ( !__lock.owns_lock() ) SAMBAG_THROW(SAMBAG_DEADLOCK_EXCEPTION, "deadlock exception");


/**
 * usage:
 * SAMBAG_BEGIN_SYNCHRONIZED(mutexObject)
 * 		// do something
 * SAMBAG_END_SYNCHRONIZED
 */
#define SAMBAG_BEGIN_SYNCHRONIZED(mutex) { SAMBAG_TRY_TO_LOCK_RECURSIVE((mutex));
#define SAMBAG_END_SYNCHRONIZED }

#define SAMBAG_WHEN_UNLOCKED(mutex) {											\
	boost::unique_lock<sambag::com::Mutex> __lock( (mutex), boost::try_to_lock);\
		if (__lock.owns_lock()) {

#define SAMBAG_END_WHEN_UNLOCKED }}

#endif /* SAMBAG_THREAD_HPP_ */
