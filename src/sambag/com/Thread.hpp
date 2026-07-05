/*
 * Thread.hpp
 *
 *  Created on: May 18, 2012
 *      Author: samba
 */

#ifndef SAMBAG_THREAD_HPP_
#define SAMBAG_THREAD_HPP_

#include <mutex>
#include <thread>
#include <chrono>
#include <sambag/com/Exception.hpp>

namespace sambag { namespace com {
	typedef std::recursive_timed_mutex RecursiveMutex;
	typedef std::timed_mutex Mutex;
    typedef std::thread::id ThreadId;
	inline ThreadId getThreadId() {
        return std::this_thread::get_id();
    }
    SAMBAG_EXCEPTION_CLASS(DeadLockException);
	inline void wait(long sec) {
		std::this_thread::sleep_for(std::chrono::seconds(sec));
	}
}} // namespaces

#define SAMBAG_DEADLOCK_EXCEPTION sambag::com::DeadLockException
#define SAMBAG_LOCK_TIMEOUT 30

#define SAMBAG_TRY_TO_LOCK_TIMED(mutex) std::unique_lock<sambag::com::Mutex> __lock( (mutex), std::try_to_lock);\
	if (!__lock.owns_lock()) { __lock.try_lock_for(std::chrono::seconds(SAMBAG_LOCK_TIMEOUT)); }\
	if ( !__lock.owns_lock() ) SAMBAG_THROW(SAMBAG_DEADLOCK_EXCEPTION, "deadlock exception");

#define SAMBAG_TRY_TO_LOCK_RECURSIVE(mutex) std::unique_lock<sambag::com::RecursiveMutex> __lock( (mutex), std::try_to_lock);\
	if (!__lock.owns_lock()) { __lock.try_lock_for(std::chrono::seconds(SAMBAG_LOCK_TIMEOUT)); }\
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
	std::unique_lock<sambag::com::Mutex> __lock( (mutex), std::try_to_lock);\
		if (__lock.owns_lock()) {

#define SAMBAG_END_WHEN_UNLOCKED }}

#endif /* SAMBAG_THREAD_HPP_ */
