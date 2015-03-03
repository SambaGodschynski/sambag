#ifndef XML2OBJ_H
#define XML2OBJ_H

#include <boost/unordered_map.hpp>
#include <map>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include <boost/function.hpp>
#include <sstream>
#include "ticpp/ticpp.h"

namespace sambag {
namespace io {}
namespace xml {
//=============================================================================
// Default Creator policies:
template <class T>
struct SharedCreateCreator {
	typedef boost::shared_ptr<T> PointeeType;
	PointeeType create() {
		return T::create();
	}
	SharedCreateCreator(){}
	template <class U>
	SharedCreateCreator(const SharedCreateCreator<U> &b) {
	}
};
template <class Closure>
struct SharedWithClosure {
	template <class T>
	struct Creator {
		Closure val;
		Closure *pClosure;
		typedef boost::shared_ptr<T> PointeeType;
		PointeeType create() {
			if (!pClosure)
				return T::create(val);
			return T::create(*pClosure);
		}
		Creator() : pClosure(NULL) {
		}
		template <class U>
		Creator(Creator<U> &b) {
			if (!(b.pClosure)) {
				pClosure = &(b.val);
				return;
			}
			pClosure = b.pClosure;
		}
		void setClosure(const Closure &val) {
			if (!pClosure) {
				this->val = val;
				return;			
			}
			*pClosure = val;		
		}
		const Closure & getClosure() const {
			if (!pClosure)
				return val;
			return *pClosure;		
		}
	};
};
template <class T>
struct NewCreator {
	typedef T* PointeeType;
	PointeeType create() {
		return new T();
	}
	NewCreator(){}
	template <class U>
	NewCreator(const NewCreator<U> &b) {
	}
};
//=============================================================================
// Default AddObject policy:
template <class BaseType>
struct DefaultAddObject {
	template <class U, class V>
	void performAdd(U &u, const V &v) {
		u.add(v);
	}
};
//=============================================================================
// Default AttributeSetter policy:
template <class BaseType>
class TaggedAttributeSetterPolicy {
	//-------------------------------------------------------------------------
	struct IAttributeSetter {
		virtual void set(BaseType &obj,
				const std::string &attrValue) = 0;
	};
	//-------------------------------------------------------------------------
	template<typename AttrType, typename TagClassType, typename ObjectType>
	struct AttributeSetter: public IAttributeSetter 
	{
		virtual void set(BaseType &obj,
				const std::string &strValue) 
		{
            using namespace io; // operators defined here
			ObjectType *tObj = dynamic_cast<ObjectType*> (&obj);
			if (!tObj)
				return;
			std::stringstream ss;
			ss << strValue;
			AttrType value;
			ss >> value;
			tObj->set(value, TagClassType());
		}
	};
	//-------------------------------------------------------------------------
	template<typename TagClassType, typename ObjectType>
	struct AttributeSetter<std::string, TagClassType, ObjectType> : 
		public IAttributeSetter 
	{
		virtual void set(BaseType &obj,
				const std::string &strValue) 
		{
			ObjectType *tObj = dynamic_cast<ObjectType*> (&obj);
			if (!tObj)
				return;
			tObj->set(strValue, TagClassType());
		}
	};
	//-------------------------------------------------------------------------
	typedef std::string AttributeName;
	typedef std::multimap<AttributeName, IAttributeSetter*> AttributeMap;
	AttributeMap attrMap;
	//-------------------------------------------------------------------------
public:
	//-------------------------------------------------------------------------
	virtual ~TaggedAttributeSetterPolicy() {
		typename AttributeMap::iterator atIt = attrMap.begin();
		for (; atIt != attrMap.end(); ++atIt) {
			delete atIt->second;
			atIt->second = NULL;
		}	
	}
	//-------------------------------------------------------------------------
	/**
	 * template: AttrType,
	 *           TagClassType,
	 *           ObjectType
	 * @param attrName
	 */
	template<typename AttrType, typename TagClassType, typename ObjectType>
	void registerAttribute(const std::string &attrName) {
		IAttributeSetter *setter = new AttributeSetter<AttrType, TagClassType,
				ObjectType> ();
		attrMap.insert(std::make_pair(attrName, setter));
	}
	//-------------------------------------------------------------------------
	template <class T>	
	void performSetAttribute(T &obj, const std::string &attrName, 
		const std::string &val) 
	{
		typedef typename AttributeMap::iterator It;
		std::pair<It, It> range = 
			attrMap.equal_range(attrName);
		// try every possible entries (stupid)
		for ( It it=range.first; it!=range.second; ++it) {
			it->second->set(obj, val);
		}
	}
};
//=============================================================================
// Default XmlTextSetter policy:
template <class BaseType>
struct DefaultXmlTextSetter {
	template <class T>
	void performSetXmlText(T &u, const std::string &txt) {
		u.setXmlText(txt);
	}
};
//=============================================================================
/** @class XML2Object Parser:
 *	Policy concepts:
 *	- CreatorPolicy:
 *		typedef boost::shared_ptr<T> PointeeType;
 *		PointeeType create();
 *		template <class U>
 *		SharedCreateCreator(const SharedCreateCreator<U> &b);
 *	- AddObjectPolicy:
 *		template <class U, class V>
 *		void performAdd(U &u, const V &v);
 *	- AtrributeSetterPolicy:
 *		template <class T>	
 *		void performSetAttribute(T &obj, const std::string &attrName, 
 *			const std::string &val);
 *	- XmlTextPolicy:
 *		template <class T>
 *		void performSetXmlText(T &u, const std::string &txt);
 */
template<
	class BaseType, 
	template <class> class CreatorPolicy = SharedCreateCreator,
	template <class> class _AddObjectPolicy = DefaultAddObject,
	template <class> class _XmlTextSetterPolicy = DefaultXmlTextSetter,
	template <class> class _AttributeSetterPolicy = TaggedAttributeSetterPolicy
>
class XML2Object : public CreatorPolicy<BaseType>,
	public _AddObjectPolicy<BaseType>,
	public _XmlTextSetterPolicy<BaseType>,
	public _AttributeSetterPolicy<BaseType>
{
//=============================================================================
public:
	typedef _XmlTextSetterPolicy<BaseType> XmlTextSetterPolicy;
	//-------------------------------------------------------------------------
	typedef _AttributeSetterPolicy<BaseType> AttributeSetterPolicy;
	//-------------------------------------------------------------------------
	typedef _AddObjectPolicy<BaseType> AddObjectPolicy;
	//-------------------------------------------------------------------------
	typedef typename CreatorPolicy<BaseType>::PointeeType BaseTypePtr;
	//-------------------------------------------------------------------------
	typedef std::string TagName;
	//-------------------------------------------------------------------------
	typedef boost::signals2::signal<void(BaseTypePtr, const TagName&)> CreatedSignal;
	//-------------------------------------------------------------------------
	typedef boost::function<void(BaseTypePtr, const TagName&)>
		CreatedSignalFunction;
	//-------------------------------------------------------------------------
	CreatedSignal sObjCreated;
private:
	//#########################################################################
	// Object Creation:
	//-------------------------------------------------------------------------
	struct IObjectFactory {
		virtual BaseTypePtr create() = 0;
	};
	//-------------------------------------------------------------------------
	template<typename ObjectType>
	struct ObjectFactory: public IObjectFactory {
		CreatorPolicy<ObjectType> creator;
		virtual BaseTypePtr create() {
			return creator.create();
		}
	};
	//-------------------------------------------------------------------------
	// relation from tagname to object factory
	typedef boost::unordered_map<TagName, IObjectFactory*> TagMap;
	TagMap tagMap;
	//-------------------------------------------------------------------------
	BaseTypePtr buildObject(const ticpp::Element &el) {
		using namespace boost::algorithm;
		typename TagMap::iterator it = tagMap.find(to_lower_copy(el.Value()));
		if (it == tagMap.end())
			return BaseTypePtr();
		BaseTypePtr obj = it->second->create();
		return obj;
	}
	//-------------------------------------------------------------------------
	void setAttribute(BaseTypePtr obj, const ticpp::Attribute &attr) {
		using namespace boost::algorithm;
		AttributeSetterPolicy::performSetAttribute(
			*obj, attr.Name(), attr.Value()
		);
	}
	//-------------------------------------------------------------------------
	void setAttributes(BaseTypePtr obj, const ticpp::Element &el) {
		ticpp::Iterator < ticpp::Attribute > attribute;
		attribute = attribute.begin(&el);
		for (; attribute != attribute.end(); attribute++) {
			setAttribute(obj, *attribute);
		}
	}
	//#########################################################################
	// building
	//-------------------------------------------------------------------------
	/**
	 *  walks rekursively through xml element structure and creates objects if
	 *  tagnames are registered.
	 * @param element: current xml emelment
	 * @param givenRoot: setted if the root object is already created (opt.)
	 * @return created object ptr
	 */
	BaseTypePtr walk(
			const ticpp::Element &element,
			BaseTypePtr givenRoot = BaseTypePtr() )
	{
		// create new object
		BaseTypePtr object = (givenRoot) ? givenRoot : buildObject(element);
		if (!object)
			return BaseTypePtr();
		// iterate through child elements
		ticpp::Iterator < ticpp::Element > child;
		for (child = child.begin(&element); child != child.end(); child++) {
			BaseTypePtr app = walk(*child); // rekusive call
			if (!app)
				continue;
			AddObjectPolicy::performAdd(*object, app);
		}
		setAttributes(object, element);
		XmlTextSetterPolicy::performSetXmlText(*object, 
			element.GetTextOrDefault("")
		);
		sObjCreated(object, element.Value()); // fire objCreated signal
		return object;
	}
	//-------------------------------------------------------------------------
	/**
	 * builds object structure. Container has to append ObjectType objects
	 * with a push_back method.
	 * @param doc
	 * @param givenRoot setted if the root object is already created (opt.)
	 * @return root object ptr
	 */
	BaseTypePtr build(
			ticpp::Document &doc,
			BaseTypePtr givenRoot = BaseTypePtr())
	{
		ticpp::Element* pElem = doc.FirstChildElement();
		return walk(*pElem, givenRoot);
	}
public:
	//--------------------------------------------------------------------------
	/**
	 * XML2Object<BaseObject, ClosureType=NO_TYPE>
	 * Closure is a pointer which comes as parameter for the
	 * ObjectType::create() method. If no ClosureType setted this create
	 * will be called without this closure parameter.
	 * @param closure
	 */
	XML2Object() {
	}
	//--------------------------------------------------------------------------
	// frees IOBjectFactory objects
	~XML2Object() {
		//release IFactory Objects
		typename TagMap::iterator tagIt = tagMap.begin();
		for (; tagIt != tagMap.end(); ++tagIt) {
			delete tagIt->second;
			tagIt->second = NULL;
		}
	}
	//-------------------------------------------------------------------------
	const CreatedSignal & getCreatedSignal() const {
		return sObjCreated;
	}
	//-------------------------------------------------------------------------
	boost::signals2::connection
	addObjectCreatedSlot(const CreatedSignalFunction &f)
	{
		return sObjCreated.connect(f);
	}
	//-------------------------------------------------------------------------
	// register object type with related tag string
	template<typename ObjectType>
	void registerObject(const std::string &tagName) {
		using namespace boost::algorithm;
		ObjectFactory<ObjectType> *fact = new ObjectFactory<ObjectType> ();
		fact->creator = *this;
		tagMap.insert(std::make_pair(to_lower_copy(tagName), fact));
	}
	//-------------------------------------------------------------------------
	/**
	 * TODO: replace buildWithXmlString and buildWithXmlFile with buildWithXmlStream(istream)
	 *
	 * @param xml:  a valid xml document as string
	 * @return the root object containing a object representation of the xml structure.
	 */
	BaseTypePtr buildWithXmlString(
			const std::string &xml /*default value dosent work with MVSC
									 (complaining about typename) so we use overload*/)
	{
		ticpp::Document doc;
		doc.Parse(xml);
		return build(doc, BaseTypePtr());
	}
	//-------------------------------------------------------------------------
	/**
	 * TODO: replace buildWithXmlString and buildWithXmlFile with buildWithXmlStream(istream)
	 *
	 * @param xml:  a valid xml document as string
	 * @param givenRoot: you are able to use a already created root element.
	 * @return the root object containing a object representation of the xml structure.
	 */
	BaseTypePtr buildWithXmlString(
			const std::string &xml,
			BaseTypePtr givenRoot)
	{
		ticpp::Document doc;
		doc.Parse(xml);
		return build(doc, givenRoot);
	}
	//-------------------------------------------------------------------------
	/**
	 * TODO: replace buildWithXmlString and buildWithXmlFile with buildWithXmlStream(istream)
	 *
	 * @param xml: location of a valid xml document.
	 * @param givenRoot: you are able to use a already created root element.
	 * @return the root object containing a object representation of the xml structure.
	 */
	BaseTypePtr buildWithXmlFile(
			const std::string &filename,
			BaseTypePtr givenRoot = BaseTypePtr())
	{
		ticpp::Document doc(filename);
		doc.LoadFile();
		return build(doc, givenRoot);
	}
};
}
} // namespace
#endif
