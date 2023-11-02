//------------------------------------------------------------------------
// Project     : SDK Base
// Version     : 1.0
//
// Category    : Helpers
// Filename    : base/source/fobject.h
// Created by  : Steinberg, 2008
// Description : Basic Object implementing FUnknown
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2010, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// This Software Development Kit may not be distributed in parts or its entirety  
// without prior written agreement by Steinberg Media Technologies GmbH. 
// This SDK must not be used to re-engineer or manipulate any technology used  
// in any Steinberg or Third-party application or software module, 
// unless permitted by law.
// Neither the name of the Steinberg Media Technologies nor the names of its
// contributors may be used to endorse or promote products derived from this 
// software without specific prior written permission.
// 
// THIS SDK IS PROVIDED BY STEINBERG MEDIA TECHNOLOGIES GMBH "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL STEINBERG MEDIA TECHNOLOGIES GMBH BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------
/** @file base/source/fobject.h
	Basic Object implementing FUnknown. */
//------------------------------------------------------------------------

#ifndef __fobject__
#define __fobject__

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/base/iupdatehandler.h"
#include "base/source/fdebug.h" // NEW

namespace Steinberg {

typedef FIDString FClassID;

//------------------------------------------------------------------------
// Basic FObject - implements FUnknown
//------------------------------------------------------------------------
/** Implements FUnknown and IDependent.

FObject is a polymorphic class that implements IDependent (of SKI module)
and therefore derived from FUnknown, which is the most abstract base class of all. 

All COM-like virtual methods of FUnknown such as queryInterface(), addRef(), release()
are implemented here. On top of that, dependency-related methods are implemented too.

Pointer casting is done via the template methods FCast, either FObject to FObject or
FUnknown to FObject.

FObject supports a new singleton concept, therefore these objects are deleted automatically upon program termination.

@see 
	- FUnknown
	- IDependent
	- IUpdateHandler
*/
//------------------------------------------------------------------------
class FObject : public IDependent
{
public:
	//------------------------------------------------------------------------
	FObject () : refCount (1) {}											///< default constructor...
	FObject (const FObject&) : refCount (1) {}								///< overloaded constructor...
	virtual ~FObject () {}													///< destructor...
	FObject& operator = (const FObject&) { return *this; }					///< overloads operator "=" as the reference assignment

	// OBJECT_METHODS
	static inline FClassID getFClassID () {return "FObject";}				///< return Class ID as an ASCII string (statically)
	virtual FClassID isA () const {return FObject::getFClassID ();}			///< a local alternative to getFClassID ()
	virtual bool isA (FClassID s) const {return isTypeOf (s, false);}		///< evaluates if the passed ID is of the FObject type
	virtual bool isTypeOf (FClassID s, bool /*askBaseClass*/ = true) const {return classIDsEqual (s, FObject::getFClassID ());}
																			///< evaluates if the passed ID is of the FObject type
	int32 getRefCount () {return refCount;}									///< returns the current interface reference count
	FUnknown* unknownCast () {return this;}									///< get FUnknown interface from object

	// FUnknown
	virtual tresult PLUGIN_API queryInterface (const TUID iid, void** obj); ///< please refer to FUnknown::queryInterface ()
	virtual uint32 PLUGIN_API addRef ();									///< please refer to FUnknown::addref ()
	virtual uint32 PLUGIN_API release ();									///< please refer to FUnknown::release ()

	// IDependent
	virtual void PLUGIN_API update (FUnknown* /*changedUnknown*/, int32 /*message*/) {}
																			///< empty virtual method that should be overridden by derived classes for data updates upon changes	
	void addDependent (IDependent* dep);									///< adds dependency to the object
	void removeDependent (IDependent* dep);									///< removes dependency from the object
	virtual void changed (int32 msg = kChanged);							///< Inform all dependents, that the object has changed.
	virtual void deferUpdate (int32 msg = kChanged);						///< Similar to triggerUpdates, except only delivered in idle (usefull in collecting updates).
	virtual void updateDone () {}											///< empty virtual method that should be overridden by derived classes
	static void setUpdateHandler (IUpdateHandler* handler) {gUpdateHandler = handler;}	///< set method for the local attribute
	static IUpdateHandler* getUpdateHandler () {return gUpdateHandler;}					///< get method for the local attribute 

	// static helper functions
	static inline bool classIDsEqual (FClassID ci1, FClassID ci2);			///< compares (evaluates) 2 class IDs
	static inline FObject* unknownToObject (FUnknown* unknown);				///< pointer conversion from FUnknown to FObject

	/** Special UID that is used to cast an FUnknown pointer to a FObject */
	static const FUID iid;

//------------------------------------------------------------------------
protected:
	int32 refCount;															///< COM-model local reference count

	static IUpdateHandler* gUpdateHandler;
};


//------------------------------------------------------------------------
// conversion from FUnknown to FObject
//------------------------------------------------------------------------
inline FObject* FObject::unknownToObject (FUnknown* unknown)
{
	FObject* object = 0;
	if (unknown) 
	{
		unknown->queryInterface (FObject::iid, (void**)&object);
		if (object)
			object->release (); // queryInterface has added ref		
	}
	return object;
}

//------------------------------------------------------------------------
inline bool FObject::classIDsEqual (FClassID ci1, FClassID ci2)
{
	return (ci1 && ci2) ? (strcmp (ci1, ci2) == 0) : false;
}

//-----------------------------------------------------------------------
/** FCast overload 1 - FObject to FObject */
//-----------------------------------------------------------------------
template <class C>
inline C* FCast (const FObject* object)
{
	if (object && object->isTypeOf (C::getFClassID (), true))
		return (C*) object;
	return 0;
}

//-----------------------------------------------------------------------
/** FCast overload 2 - FUnknown to FObject */
//-----------------------------------------------------------------------
template <class C>
inline C* FCast (FUnknown* unknown)
{
	FObject* object = FObject::unknownToObject (unknown);
	return FCast<C> (object);
}

//------------------------------------------------------------------------
//* automatic creation and destruction of singleton instances. */
namespace Singleton {
	void registerInstance (FObject** o);
	bool isTerminated ();
}

//------------------------------------------------------------------------
/** OPtr - owning smart pointer used for newly created FObjects.
	Because of the FObject refCount being intialized with 1 in constructor, writing 
					\code
	IPtr<FObject> tmp (NEW FObject);
					\endcode 	
	will lead to a leak without further release call. This can be done by writing
					\code
	IPtr<FObject> tmp (NEW FObject, false);
					\endcode 	
	or by the usage of OPtr:
					\code
	OPtr<FObject> tmp (NEW FObject);
	OPtr<FObject> tmp2;
	tmp2 = NEW FObject;
					\endcode 	
*/
//------------------------------------------------------------------------
template <class I>
class OPtr : public IPtr<I>
{
public:
//------------------------------------------------------------------------
	inline OPtr (I* p) : IPtr<I> (p, false) {}
	inline OPtr (const IPtr<I>& p) : IPtr<I> (p) {}
	inline OPtr (const OPtr<I>& p) : IPtr<I> (p) {}
	inline OPtr () {}
	inline I* operator=(I* _ptr)
	{
		if (_ptr != this->ptr)
		{
			if (this->ptr)
				this->ptr->release ();
			this->ptr = _ptr;
		}
		return this->ptr;
	}
};

} // namespace Steinberg

//-----------------------------------------------------------------------
#define SINGLETON(ClassName)	\
	static ClassName* instance (bool create = true) \
	{ \
		static FObject* inst = 0; \
		if (inst == 0 && create && Singleton::isTerminated () == false) \
		{	\
			inst = NEW ClassName; \
			Singleton::registerInstance (&inst); \
		}	\
		return (ClassName*)inst; \
	}

//-----------------------------------------------------------------------
#define OBJ_METHODS(className, baseClass)								\
	static inline FClassID getFClassID () {return (#className);}		\
	virtual FClassID isA () const {return className::getFClassID ();}	\
	virtual bool isA (FClassID s) const {return isTypeOf (s, false);}	\
	virtual bool isTypeOf (FClassID s, bool askBaseClass = true) const	\
    {  return (classIDsEqual (s, #className) ? true : (askBaseClass ? baseClass::isTypeOf (s, true) : false)); } 

//------------------------------------------------------------------------
/** Delegate refcount functions to BaseClass.
	BaseClase must implement ref counting. 
*/
//------------------------------------------------------------------------
#define REFCOUNT_METHODS(BaseClass) \
virtual Steinberg::uint32 PLUGIN_API addRef (){ return BaseClass::addRef (); } \
virtual Steinberg::uint32 PLUGIN_API release (){ return BaseClass::release (); }

//------------------------------------------------------------------------
/** @name Macros to implement FUnknown::queryInterface ().

	<b>Examples:</b>
	@code
	class Foo : public FObject, public IFoo2, public IFoo3 
	{
	    ...
		DEFINE_INTERFACES
	        DEF_INTERFACE (IFoo2)
	        DEF_INTERFACE (IFoo3)
	    END_DEFINE_INTERFACES (FObject)
	    REFCOUNT_METHODS(FObject)
	    // Implement IFoo2 interface ...
	    // Implement IFoo3 interface ...
	    ...
	};
	@endcode	
*/
///@{
//------------------------------------------------------------------------
/** Start defining interfaces. */
//------------------------------------------------------------------------
#define DEFINE_INTERFACES \
tresult PLUGIN_API queryInterface (const TUID iid, void** obj) \
{

//------------------------------------------------------------------------
/** Add a interfaces. */
//------------------------------------------------------------------------
#define DEF_INTERFACE(InterfaceName) \
	QUERY_INTERFACE (iid, obj, InterfaceName::iid, InterfaceName)

//------------------------------------------------------------------------
/** End defining interfaces. */
//------------------------------------------------------------------------
#define END_DEFINE_INTERFACES(BaseClass) \
	return BaseClass::queryInterface (iid, obj); \
}
///@}

//------------------------------------------------------------------------
/** @name Convenient macros to implement Steinberg::FUnknown::queryInterface ().
	<b>Examples:</b>
	@code
	class Foo : public FObject, public IFoo2, public IFoo3 
	{
	    ...
	    DEF_INTERFACES_2(IFoo2,IFoo3,FObject)
	    REFCOUNT_METHODS(FObject)
	    ...
	};
	@endcode
*/
///@{
//------------------------------------------------------------------------
#define DEF_INTERFACES_1(InterfaceName,BaseClass) \
DEFINE_INTERFACES \
DEF_INTERFACE (InterfaceName) \
END_DEFINE_INTERFACES (BaseClass)

//------------------------------------------------------------------------
#define DEF_INTERFACES_2(InterfaceName1,InterfaceName2,BaseClass) \
DEFINE_INTERFACES \
DEF_INTERFACE (InterfaceName1) \
DEF_INTERFACE (InterfaceName2) \
END_DEFINE_INTERFACES (BaseClass)

//------------------------------------------------------------------------
#define DEF_INTERFACES_3(InterfaceName1,InterfaceName2,InterfaceName3,BaseClass) \
DEFINE_INTERFACES \
DEF_INTERFACE (InterfaceName1) \
DEF_INTERFACE (InterfaceName2) \
DEF_INTERFACE (InterfaceName3) \
END_DEFINE_INTERFACES (BaseClass)
///@}

//------------------------------------------------------------------------
/** @name Convenient macros to implement Steinberg::FUnknown methods.
	<b>Examples:</b>
	@code
	class Foo : public FObject, public IFoo2, public IFoo3 
	{
	    ...
	    FUNKNOWN_METHODS2(IFoo2,IFoo3,FObject)
	    ...
	};
	@endcode
*/
///@{
#define FUNKNOWN_METHODS(InterfaceName,BaseClass) \
DEF_INTERFACES_1(InterfaceName,BaseClass) \
REFCOUNT_METHODS(BaseClass)

#define FUNKNOWN_METHODS2(InterfaceName1,InterfaceName2,BaseClass) \
DEF_INTERFACES_2(InterfaceName1,InterfaceName2,BaseClass) \
REFCOUNT_METHODS(BaseClass)

#define FUNKNOWN_METHODS3(InterfaceName1,InterfaceName2,InterfaceName3,BaseClass) \
DEF_INTERFACES_3(InterfaceName1,InterfaceName2,InterfaceName3,BaseClass) \
REFCOUNT_METHODS(BaseClass)
///@}

//------------------------------------------------------------------------
/** @name Convenient macros that call release and delete respectively
	on a pointer if it is non-zero, and then set the pointer to zero.
	<b>Examples:</b>
	@code
	~Foo ()
	{
		// instead of ...
		if (somePointer)
		{
			somePointer->release ();
			somePointer = 0;
		}
		// ... just being lazy I write
		FReleaseAndZero (somePointer)
	}
	@endcode
*/
///@{
#define FReleaseAndZero(ptr)	{ if (ptr) {ptr->release (); (ptr) = 0;} }

#define FDeleteAndZero(ptr)	{ if (ptr) {delete (ptr); (ptr) = 0;} }
///@}

#endif	// __fobject__
