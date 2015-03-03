/*
 * ===========================================================================================================
 * Events.h
 *      Author: Johannes Unger
 * ===========================================================================================================
 */

#ifndef SAMBAG_EVENTS_H
#define SAMBAG_EVENTS_H

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/signals2.hpp>
#include <boost/smart_ptr.hpp>

namespace sambag { namespace com { namespace events{
//=============================================================================
/** 
 * @class TrackingDummy.
 * useful for signal::track.
 */
struct TrackingDummy {
//=============================================================================
	typedef boost::shared_ptr<TrackingDummy> Ptr;
	static Ptr create() {
		return Ptr( new TrackingDummy() );
	}
	virtual ~TrackingDummy(){}
};
//=============================================================================
typedef boost::weak_ptr<void> AnyWPtr;
//=============================================================================
/**
 * @class ValueChangedEventSender.
 */
template < class T >
class EventSender {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::function< void ( void*, const T& ) > EventFunction;
	//-------------------------------------------------------------------------
	typedef boost::signals2::signal< void ( void*, const T& ) > Signal;
	//-------------------------------------------------------------------------
	typedef boost::signals2::connection Connection;
private:
	//-------------------------------------------------------------------------
	Signal signal;
protected:
public:
	//-------------------------------------------------------------------------
	/**
	 * @return true if sender has no listeners
     */	
	bool unconnected() const {
		return signal.empty();	
	}
	//-------------------------------------------------------------------------
	Connection addEventListener ( const EventFunction &vCl ) {
		return signal.connect(vCl);
	}
	//-------------------------------------------------------------------------
	// cannot impl.:
	// void removealueChangedListener ( const ValueChangedFunction &vCl ) {}
	// because:
	// http://www.boost.org/doc/libs/1_49_0/doc/html/function/faq.html 
	// Why can't I compare boost::function objects with operator== or operator!=?
	// use this instead:
	// addTrackedValueChangedListener UND ggf. TrackingDummy
	//-------------------------------------------------------------------------
	/**
	 * @see http://www.boost.org/doc/libs/1_40_0/doc/html/signals2/tutorial.html#id1664686 
	 * Section: Automatic Connection Management (Intermediate)
	 * @param 
	 * @param weak pointer
	 */
	Connection addTrackedEventListener ( const EventFunction &vCl, AnyWPtr toTrack )
	{ 
		return signal.connect(
			typename Signal::slot_type(vCl).track(toTrack)
		);
	}
	//-------------------------------------------------------------------------
	void notifyListeners ( void *src, const T &value ) const {
		signal( src, value );
	}
};
}}} // namespace
#endif


