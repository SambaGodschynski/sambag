/*
 * ===========================================================================================================
 * Events.h
 *      Author: Johannes Unger
 * ===========================================================================================================
 */

#ifndef SAMBAG_EVENTS_H
#define SAMBAG_EVENTS_H

#include <functional>
#include <memory>
#include <list>

namespace sambag { namespace com { namespace events{
//=============================================================================
/**
 * @class TrackingDummy.
 * useful for signal::track.
 */
struct TrackingDummy {
//=============================================================================
	typedef std::shared_ptr<TrackingDummy> Ptr;
	static Ptr create() {
		return Ptr( new TrackingDummy() );
	}
	virtual ~TrackingDummy(){}
};
//=============================================================================
typedef std::weak_ptr<void> AnyWPtr;
//=============================================================================
/**
 * @class Connection.
 * Lightweight replacement for boost::signals2::connection.
 */
class Connection {
	struct State {
		bool disconnected;
		State() : disconnected(false) {}
	};
	std::shared_ptr<State> state_;
	explicit Connection(std::shared_ptr<State> s) : state_(s) {}
public:
	Connection() {}
	void disconnect()  { if (state_) state_->disconnected = true; }
	bool connected()   const { return state_ && !state_->disconnected; }
	template<class T> friend class EventSender;
};
//=============================================================================
/**
 * @class EventSender.
 * Lightweight replacement for boost::signals2-based EventSender.
 */
template < class T >
class EventSender {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef std::function< void ( void*, const T& ) > EventFunction;
	//-------------------------------------------------------------------------
	typedef ::sambag::com::events::Connection Connection;
private:
	//-------------------------------------------------------------------------
	struct Slot {
		EventFunction fn;
		std::weak_ptr<void> tracker;
		bool hasTracker;
		std::shared_ptr<Connection::State> state;
	};
	mutable std::list<Slot> slots;
protected:
public:
	//-------------------------------------------------------------------------
	/**
	 * @return true if sender has no listeners
	 */
	bool unconnected() const {
		for (typename std::list<Slot>::const_iterator it = slots.begin(); it != slots.end(); ++it) {
			if (!it->state->disconnected && (!it->hasTracker || !it->tracker.expired()))
				return false;
		}
		return true;
	}
	//-------------------------------------------------------------------------
	Connection addEventListener ( const EventFunction &vCl ) {
		std::shared_ptr<Connection::State> s = std::make_shared<Connection::State>();
		Slot slot;
		slot.fn = vCl;
		slot.hasTracker = false;
		slot.state = s;
		slots.push_back(slot);
		return Connection(s);
	}
	//-------------------------------------------------------------------------
	/**
	 * @see http://www.boost.org/doc/libs/1_40_0/doc/html/signals2/tutorial.html#id1664686
	 * Section: Automatic Connection Management (Intermediate)
	 */
	Connection addTrackedEventListener ( const EventFunction &vCl, AnyWPtr toTrack )
	{
		std::shared_ptr<Connection::State> s = std::make_shared<Connection::State>();
		Slot slot;
		slot.fn = vCl;
		slot.tracker = toTrack;
		slot.hasTracker = true;
		slot.state = s;
		slots.push_back(slot);
		return Connection(s);
	}
	//-------------------------------------------------------------------------
	void notifyListeners ( void *src, const T &value ) const {
		for (typename std::list<Slot>::iterator it = slots.begin(); it != slots.end(); ) {
			if (it->state->disconnected) {
				it = slots.erase(it);
				continue;
			}
			if (it->hasTracker && it->tracker.expired()) {
				it = slots.erase(it);
				continue;
			}
			it->fn(src, value);
			++it;
		}
	}
};
}}} // namespace
#endif
