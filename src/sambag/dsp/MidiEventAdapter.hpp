/*
 * VstMidiEventAdapter.hpp
 *
 *  Created on: Fri Nov 30 11:12:28 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_VSTMIDIEVENTADAPTER_H
#define SAMBAG_VSTMIDIEVENTADAPTER_H

#include <boost/shared_ptr.hpp>
#include  "IMidiEvents.hpp"
namespace sambag { namespace dsp {

//=============================================================================
/** 
  * @class VstMidiEventAdapter.
  */
class VstMidiEventAdapter : public IMidiEvents {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<VstMidiEventAdapter> Ptr;
protected:
private:
public:
}; // VstMidiEventAdapter
}} // namespace(s)

#endif /* SAMBAG_VSTMIDIEVENTADAPTER_H */
