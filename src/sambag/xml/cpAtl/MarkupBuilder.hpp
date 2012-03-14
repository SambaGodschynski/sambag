/*
 * MarkupBuilder.hpp
 *  Created on: Dec 10, 2011
 *      Author: samba
 */

/**
 * This file is part of the CpATL package which is a port of
 * the JATL library by Adam Gent.
 *
 * http://code.google.com/p/jatl/
 *
 * Copyright (C) 2010 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MARKUPBUILDER_HPP_
#define MARKUPBUILDER_HPP_

#include <ostream>
#include <stack>
#include <map>
#include <list>
#include "sambag/com/Common.hpp"
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_tree_to_xml.hpp>
#include <boost/algorithm/string.hpp>

namespace sambag { namespace xml { namespace cpAtl {
using namespace std;
using namespace sambag::com;
namespace {
	inline string operator*(const string &str, size_t c) {
		if (c==0)
			return "";
		stringstream ss(str);
		while (c-- > 0) {
			ss<<str;
		}
		return ss.str();
	}
}
//=============================================================================
/**
 * @class MarkupBuilder
 */
template<typename T>
class MarkupBuilder {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	enum TagIndentSpot {
		BEFORE_START_TAG,
		AFTER_START_TAG,
		BEFORE_END_TAG,
		AFTER_END_TAG
	};
	//-------------------------------------------------------------------------
	/**
	* Policy for how tags should be closed.
	* @author adamgent
	*
	*/
	struct TagClosingPolicy {
		//---------------------------------------------------------------------
		enum Value{
			NORMAL,
			SELF,
			PAIR
		} _this;
		//---------------------------------------------------------------------
		operator Value() const {
			return _this;
		}
		TagClosingPolicy( Value v ) : _this(v) {}
		//---------------------------------------------------------------------
		bool isAlwaysSelfClosing() const {
			return _this == SELF;
		}
		//---------------------------------------------------------------------
		/**
		 * @return <code>true</code> if the tag is allowed to self close.
		 */
		bool isSelfClosing() const {
			return _this == SELF || _this == NORMAL;
		}
		//---------------------------------------------------------------------
		/**
		* @return <code>true</code> if the tag is allowed to close with a matching end tag (<code>&lt;/tag&gt;</code>).
		*/
		bool isPairClosing() const {
			return _this == PAIR || _this == NORMAL;
		}
	};
private:
	//---------------------------------------------------------------------
	static bool isBlank(const string &str) {
		using namespace boost::algorithm;
		return trim_copy(str).size() == 0;
	}
	//---------------------------------------------------------------------
	struct Tag {
		int depth;
		string name;
		string prefix;
		bool empty;
		bool start;
		bool end;
		TagClosingPolicy closePolicy;
		//---------------------------------------------------------------------
		Tag(const string & name) :
			depth(0),
			name(name),
			empty(true),
			start(false),
			end(false),
			closePolicy(TagClosingPolicy::NORMAL)
		{
		}
	public:
		//---------------------------------------------------------------------
		string getQualifiedName() const {
			return isBlank(prefix) || name.find(":")!=string::npos ? name :
			prefix + ":"  + name;
		}
		//---------------------------------------------------------------------
		virtual bool isSelfClosing() const {
			return empty && closePolicy.isSelfClosing();
		}
	};
private:
	class Tag;
	stack<Tag> tagStack;
	ostream &writer;
	typedef pair<string, string> Attribute;
	typedef list<Attribute> Attributes;
	Attributes attributes;
	//map<string, Object> bindings = new HashMap<String, Object>();
	//MarkupBuilder<T> previousBuilder = null;
	int depth;
	string namespacePrefix;
	//Indenter previousIndenter = indentOn;
	//Indenter indenter = indentOn;
	static const string _q;

public:
	/**
	* Create a builder using the given writer.
	* @param writer never <code>null</code>.
	*/
	//-------------------------------------------------------------------------
	MarkupBuilder(ostream &writer) : writer(writer), depth(0) {}
	//-------------------------------------------------------------------------
//
//        /**
//         * Create a nested builder from given builder.
//         * Make sure {@link #done()} is called when finished with the
//         * nested builder so that the parent builder can resume using the writer.
//         *
//         * @param builder parent builder, never <code>null</code>.
//         * @see #done()
//         */
//        public MarkupBuilder(MarkupBuilder<?> builder) {
//                this(builder, true);
//        }
//
	/**
	* Use for deferred writer.
	* The writer should be {@link #setWriter(Writer) set} before
	* the builder is used.
	*
	* @see #setWriter(Writer)
	* @see MarkupWriter
	*/
protected:
public:
//        /**
//         * Create a nested builder from a builder or resume from a builder.
//         * @param builder never <code>null</code>.
//         * @param nested <code>true</code> means nested, <code>false</code> means resuming.
//         */
//        public MarkupBuilder(MarkupBuilder<?> builder, boolean nested) {
//                builder.checkWriter();
//                if (nested)
//                        builder.writeCurrentTag();
//                if (nested && ! builder.tagStack.isEmpty()) {
//                        Tag t = builder.tagStack.peek();
//                        this.depth = 1 + t.depth + builder.depth;
//                }
//                else {
//                        this.depth = builder.depth;
//                }
//                this.writer = builder.writer;
//                builder.writer = null;
//
//                if (nested) {
//                        //Clone the previous builders binding
//                        this.previousBuilder = builder;
//                        this.bindings = new HashMap<String, Object>(builder.bindings);
//                }
//                else {
//                        this.tagStack = builder.tagStack;
//                        this.bindings = builder.bindings;
//                        this.attributes = builder.attributes;
//                }
//        }
//
protected:
	//-------------------------------------------------------------------------
	/**
	* Needed for fluent style and Java parameterization limitations.
	* Almost all public methods should return whatever this method returns.
	* <p>
	* Most implementations only have to do:
	* <pre>
	* return this;
	* </pre>
	* @return the current builder which is usually <code>this</code> object.
	*/
	virtual T & getSelf() = 0;
public:
	//-------------------------------------------------------------------------
	/**
	* Restores the current namespace prefix
	* to whatever the surrounding tags prefix is.
	* To set the namespace to the default namespace call:
	* <p>
	* {@code ns(null);}
	* @return maybe <code>null</code>.
	* @see #ns(String)
	*/
	virtual T & ns() {
		if (tagStack.empty()) {
			return ns("");
		}
		return ns(tagStack.top().prefix);
	}
	//-------------------------------------------------------------------------
	/**
	* Sets the current namespace prefix. If a tag is started when
	* the prefix is set to a non-null non-empty string, the prefix will
	* be added in front of the tag.
	* <p>
	* <strong>Example:</strong><p>
	* <pre>
	* ns("html").div().end();
	* </pre>
	* <strong>Result:</strong>
	* <p>
	* <pre>
	* &lt;html:div&gt;&lt;/html:div&gt;
	* </pre>
	* @param prefix maybe <code>null</code>
	* @return this, not <code>null</code>.
	*/
	virtual T & ns(const string & prefix) {
		namespacePrefix = prefix;
		return getSelf();
	}
	//-------------------------------------------------------------------------
	/**
	* Writes variable expanded escaped text inside a tag.
	*
	* @param text the text will be escaped and variables will be expanded.
	* @return never <code>null</code>.
	* @see #raw(String)
	* @see #bind(String, Object)
	*/
	virtual T & text(const string & text) {
		if (text.length()>0) {
				writeCurrentTag();
				write(escapeMarkup(expand(text)));
		}
		return getSelf();
	}
	//-------------------------------------------------------------------------
	/**
	* Write text with out escaping or variable expansion.
	* @param text
	* @return never <code>null</code>.
	* @see #raw(String, boolean)
	*/
	virtual T & raw(const string & text) {
		return raw(text, false);
	}
	//-------------------------------------------------------------------------
	/**
	* Writes text with out escaping.
	* @param text
	* @param expand <code>true</code> does variable expansion.
	* @return never <code>null</code>.
	*/
	virtual T & raw(const string & text, bool _expand) {
		if (text.length()>0) {
			writeCurrentTag();
			write(text);
		}
		return getSelf();
	}
	//-------------------------------------------------------------------------
//	/**
//	* Binds a named variables to be used for expansion in {@link #attr(String...) attributes}
//	* and {@link #text(String) text}. Variables are represented with by <code>${...}</code>.
//	* <p>
//	* <strong>Example</strong>
//	* <pre>
//	* bind("name", "Ferris");
//	* text("${name}");
//	* </pre>
//	* <p>
//	* <em>Variables are expanded in order and can be referred in a later binding.</em>
//	* <p>
//	* <pre>
//	* bind("firstName", "Adam");
//	* bind("lastName", "Gent");
//	* bind("fullName", "${firstName} ${lastName}");
//	* </pre>
//	* @param name never <code>null</code> or empty.
//	* @param value maybe <code>null</code>.
//	* @return never <code>null</code>.
//	*/
//        public final T bind(String name, Object value) {
//                notEmpty(name);
//                Object v = value != null && value instanceof String ? expand(value.toString()) : value;
//            bindings.put(name, v);
//            return getSelf();
//        }
//
//        /**
//         * Removes a binding.
//         * There is no failure if the binding does not exist.
//         * @param name maybe <code>null</code>.
//         * @return never <code>null</code>.
//         * @see #bind(String, Object)
//         */
//        public final T unbind(String name) {
//                if (bindings.containsKey(name))
//                        bindings.remove(name);
//            return getSelf();
//        }
	//-------------------------------------------------------------------------
//        /**
//         * Convenience for {@link #bind(String, Object)}
//         * @param nvps never <code>null</code>.
//         * @return never <code>null</code>.
//         */
//        public final T bind(Collection<Entry<String, Object>> nvps) {
//            for (Entry<String,Object> nvp : nvps) {
//                bind(nvp.getKey(), nvp.getValue());
//            }
//            return getSelf();
//        }
	//-------------------------------------------------------------------------
	/**
	* Starts a tag using the default closing policy {@link TagClosingPolicy#NORMAL}.
	* <p>
	* Equivalent to: {@code start("tag", TagClosingPolicy.NORMAL)}.
	*
	* @param tag
	* @return this, never <code>null</code>.
	* @see #start(String, TagClosingPolicy)
	*/
	virtual T & start(const string & tag) {
		return start(tag, TagClosingPolicy::NORMAL);
	}
	//-------------------------------------------------------------------------
	/**
	* Starts a tag but does not immediately write it till the next
	* tag is started.
	* The {@link TagClosingPolicy} dictates whether or not the tag
	* needs to be  {@link #end() closed}. Thus {@link #end()} does not need to be called
	* for all tags.
	*
	* @param tag never <code>null</code> or empty.
	* @param policy never <code>null</code>.
	* @return never <code>null</code>.
	* @see #end()
	*/
	virtual T & start(const string & tag, TagClosingPolicy policy) {
		writeCurrentTag();
		Tag t(tag);
		t.closePolicy = policy;
		t.depth = tagStack.size();
		t.prefix = namespacePrefix;
		tagStack.push(t);
		return getSelf();
	}
	//-------------------------------------------------------------------------
	/**
	* Sets the default namespace on the last started tag.
	* @param uri if <code>null</code> nothing will happen.
	* @return this, never <code>null</code>.
	* @see #xmlns(String, String)
	*/
	virtual T & xmlns(const string & uri) {
		return xmlns(uri, "");
	}
	//-------------------------------------------------------------------------
	/**
	* Adds attributes to the last {@link #start(String) start tag}.
	* Attributes do not need an {@link #end()} call.
	* @param attrs name value pairs. Its invalid for an odd number of arguments.
	* @return never <code>null</code>.
	* @throws IllegalArgumentException odd number of arguments.
	*/
	virtual T & attr(const string &n, const string &v) {
		getAttributes().push_back( std::make_pair(n, v));
		return getSelf();
	}
	//-------------------------------------------------------------------------
	/**
	* Sets an XML namespace.
	* @param uri if <code>null</code> nothing will happen.
	* @param prefix if <code>null</code> or blank will act
	*      like the default namespace and no prefix.
	* @return this, never <code>null</code>.
	*/
	virtual T & xmlns(const string & uri, const string & prefix) {
		if (isBlank(uri)) return getSelf();
		string n = isBlank(prefix) ? "xmlns" : prefix + ":xmlns";
		return attr(n, uri);
	}
	//-------------------------------------------------------------------------
	/**
	* Closes the inputed number of open tags.
	* @param i less than zero will do nothing.
	* @return never <code>null</code>.
	* @see #end()
	*/
	virtual T & end(int i) {
		while ( i-- > 0 && ! tagStack.empty() ) {
			end();
		}
		return getSelf();
	}
	//-------------------------------------------------------------------------
	/**
	* Closes the last {@link #start(String) start tag}.
	* This is equivalent to <code>&lt;/tag&gt; or &lt;tag/&gt; depending
	*  on the {@link TagClosingPolicy}.
	* @return never <code>null</code>.
	* @see #start(String, TagClosingPolicy)
	* @see #endAll()
	* @see #done()
	*/
	virtual T & end() {
		Tag t = tagStack.top();
		writeStartTag(t);
		writeEndTag(t);
		tagStack.pop();
		attributes.clear();
		return getSelf();
	}
	//-------------------------------------------------------------------------
	/**
	* Closes all open tags.
	* @return never <code>null</code>.
	* @see #end()
	*/
	virtual T & endAll() {
		while( ! tagStack.empty() ) {
			end();
		}
		return getSelf();
	}
//	//-------------------------------------------------------------------------
//	/**
//	* Call when completely done with the builder.
//	* This is required for nested builders.
//	* <p>
//	* If a builder is used again after {@code done()} is called
//	* an exception (usually {@link IllegalArgumentException}) will be thrown.
//	*/
//	virtual void done() {
//		endAll();
//		if (previousBuilder != null) {
//			isTrue(previousBuilder.writer == null,
//					"The previous builder has its writer set. It should not.");
//			previousBuilder.writer = writer;
//		}
//		this.writer = null;
//	}
private:
	//-------------------------------------------------------------------------
	virtual void writeCurrentTag() {
		if ( ! tagStack.empty() ) {
			Tag & current = tagStack.top();
			if ( current.empty && ! current.end ) {
				current.empty = current.closePolicy.isAlwaysSelfClosing();
				writeStartTag(current);
				if (current.closePolicy.isAlwaysSelfClosing()) {
					end();
				}
			}
		}
		attributes.clear();
	}
	//-------------------------------------------------------------------------
	Attributes & getAttributes() {
		return attributes;
	}
	//-------------------------------------------------------------------------
	void writeStartTag(Tag &t) {
		if ( ! t.end  && ! t.start) {
			writeIndent(t, BEFORE_START_TAG);
			writeTag(t.getQualifiedName(), t.isSelfClosing());
			writeIndent(t, AFTER_START_TAG);
			if (t.isSelfClosing()) {
				writeIndent(t, AFTER_END_TAG);
			}
			t.end = t.isSelfClosing();
			t.start = true;
		}
	}
	//-------------------------------------------------------------------------
	virtual void writeTag(const string & tag, bool close) {
		Attributes &attrs = getAttributes();
		write("<" + tag);
		if (!attrs.empty()) {
			write(" ");
			writeAttributes(attrs);
		}
		if (close)
			write("/>");
		else
			write(">");
	}
	//-------------------------------------------------------------------------
	virtual void writeEndTag(Tag & t) {
		if ( ! t.end ) {
			writeIndent(t, BEFORE_END_TAG);
			write("</" + t.getQualifiedName() + ">");
			t.end = true;
			writeIndent(t, AFTER_END_TAG);
		}
	}
	//-------------------------------------------------------------------------
	virtual void writeAttributes(const Attributes &attrs) {
		bool space = false;
		boost_for_each (const Attribute &attr, attrs) {
			if (space) {
				write(" ");
			}
			else {
				space = true;
			}
			writeAttr(attr.first, attr.second);
		}
	}
	//-------------------------------------------------------------------------
	virtual void writeAttr(const string & name, const string & value) {
		if (value.length() > 0 && name.length() > 0) {
			write(expand(name + "=" + q(value)));
		}
	}
	//-------------------------------------------------------------------------
	virtual string q(const string &raw) {
		return _q + escapeMarkup(expand(raw)) + _q;
	}
	//-------------------------------------------------------------------------
	virtual string expand(const string & text) const {
		//StrSubstitutor s = new StrSubstitutor(bindings);
		//return s.replace(text);
		return text;
	}
	//-------------------------------------------------------------------------
	virtual void write(const string & raw) {
		writer << raw;
	}
protected:
	//-------------------------------------------------------------------------
	/**
	* The strategy for escaping markup.
	* The default is escape for XML.
	* @param raw maybe <code>null</code>.
	* @return maybe <code>null</code> if null for input.
	* @see #text(String)
	*/
	virtual string escapeMarkup(const string & raw) const {
		return boost::spirit::classic::xml::encode(raw);
	}
private:
	//-------------------------------------------------------------------------
	virtual void writeIndent(
			const Tag & t,
			TagIndentSpot spot)
	{
		string v;
		switch (spot) {
			case BEFORE_START_TAG :
				writer<<(std::string("\t") * (size_t)t.depth);
				break;
			case AFTER_START_TAG :
				writer<<"\n";
				break;
			case BEFORE_END_TAG :
				writer<<"\n"<<(std::string("\t") * (size_t)t.depth);
				break;
			case AFTER_END_TAG :
				break;
		}
	}
};
//-----------------------------------------------------------------------------
template <typename T>
const string MarkupBuilder<T>::_q = "'";
}}}

#endif /* MARKUPBUILDER_HPP_ */
