

#ifndef GENERATED_HTML_BUILDER_HPP_
#define GENERATED_HTML_BUILDER_HPP_

#include "MarkupBuilder.hpp"

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
namespace sambag { namespace xml { namespace cpAtl {
/**
 * Auto-Generated builder from schema: xhtml1-transitional.xsd
 */
template <typename T>
class GeneratedHtmlBuilder : public MarkupBuilder<T> {
public:

	GeneratedHtmlBuilder(ostream & writer) : MarkupBuilder<T>(writer) {}
	typedef typename MarkupBuilder<T>::TagClosingPolicy TagClosingPolicy;
	/**
	 * Starts the &lt;a&gt; tag.
	 * [content is "Inline" except that anchors shouldn't be nested]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  a() {
		return MarkupBuilder<T>::start("a", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;abbr&gt; tag.
	 * [abbreviation]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  abbr() {
		return MarkupBuilder<T>::start("abbr", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;acronym&gt; tag.
	 * [acronym]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  acronym() {
		return MarkupBuilder<T>::start("acronym", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;address&gt; tag.
	 * [information on author]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  address() {
		return MarkupBuilder<T>::start("address", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;applet&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  applet() {
		return MarkupBuilder<T>::start("applet", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;area&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#SELF}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  area() {
		return MarkupBuilder<T>::start("area", TagClosingPolicy::SELF);
	}

	/**
	 * Starts the &lt;b&gt; tag.
	 * [bold font]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  b() {
		return MarkupBuilder<T>::start("b", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;base&gt; tag.
	 * [document base URI]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#SELF}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  base() {
		return MarkupBuilder<T>::start("base", TagClosingPolicy::SELF);
	}

	/**
	 * Starts the &lt;basefont&gt; tag.
	 * [base font size]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#SELF}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  basefont() {
		return MarkupBuilder<T>::start("basefont", TagClosingPolicy::SELF);
	}

	/**
	 * Starts the &lt;bdo&gt; tag.
	 * [I18N BiDi over-ride]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  bdo() {
		return MarkupBuilder<T>::start("bdo", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;big&gt; tag.
	 * [bigger font]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  big() {
		return MarkupBuilder<T>::start("big", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;blockquote&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  blockquote() {
		return MarkupBuilder<T>::start("blockquote", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;body&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  body() {
		return MarkupBuilder<T>::start("body", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;br&gt; tag.
	 * [forced line break]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#SELF}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  br() {
		return MarkupBuilder<T>::start("br", TagClosingPolicy::SELF);
	}

	/**
	 * Starts the &lt;button&gt; tag.
	 * [Content is "Flow" excluding a, form and form controls]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  button() {
		return MarkupBuilder<T>::start("button", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;caption&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  caption() {
		return MarkupBuilder<T>::start("caption", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;center&gt; tag.
	 * [center content]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  center() {
		return MarkupBuilder<T>::start("center", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;cite&gt; tag.
	 * [citation]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  cite() {
		return MarkupBuilder<T>::start("cite", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;code&gt; tag.
	 * [program code]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  code() {
		return MarkupBuilder<T>::start("code", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;col&gt; tag.
	 * [col elements define the alignment properties for cells in,       one or more columns., ,       The width attribute specifies the width of the columns, e.g., ,           width=64        width in screen pixels,           width=0.5*      relative width of 0.5, ,       The span attribute causes the attributes of one,       col element to apply to more than one column.]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#SELF}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  col() {
		return MarkupBuilder<T>::start("col", TagClosingPolicy::SELF);
	}

	/**
	 * Starts the &lt;colgroup&gt; tag.
	 * [colgroup groups a set of col elements. It allows you to group,       several semantically related columns together.]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  colgroup() {
		return MarkupBuilder<T>::start("colgroup", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;dd&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  dd() {
		return MarkupBuilder<T>::start("dd", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;del&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  del() {
		return MarkupBuilder<T>::start("del", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;dfn&gt; tag.
	 * [definitional]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  dfn() {
		return MarkupBuilder<T>::start("dfn", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;dir&gt; tag.
	 * [multiple column list (DEPRECATED)]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  dir() {
		return MarkupBuilder<T>::start("dir", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;div&gt; tag.
	 * [generic language/style container]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  div() {
		return MarkupBuilder<T>::start("div", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;dl&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  dl() {
		return MarkupBuilder<T>::start("dl", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;dt&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  dt() {
		return MarkupBuilder<T>::start("dt", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;em&gt; tag.
	 * [emphasis]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  em() {
		return MarkupBuilder<T>::start("em", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;fieldset&gt; tag.
	 * [The fieldset element is used to group form fields.,       Only one legend element should occur in the content,       and if present should only be preceded by whitespace., ,       NOTE: this content model is different from the XHTML 1.0 DTD,,       closer to the intended content model in HTML4 DTD]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  fieldset() {
		return MarkupBuilder<T>::start("fieldset", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;font&gt; tag.
	 * [local change to font]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  font() {
		return MarkupBuilder<T>::start("font", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;form&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  form() {
		return MarkupBuilder<T>::start("form", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;h1&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  h1() {
		return MarkupBuilder<T>::start("h1", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;h2&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  h2() {
		return MarkupBuilder<T>::start("h2", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;h3&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  h3() {
		return MarkupBuilder<T>::start("h3", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;h4&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  h4() {
		return MarkupBuilder<T>::start("h4", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;h5&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  h5() {
		return MarkupBuilder<T>::start("h5", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;h6&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  h6() {
		return MarkupBuilder<T>::start("h6", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;head&gt; tag.
	 * [content model is "head.misc" combined with a single,       title and an optional base element in any order]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  head() {
		return MarkupBuilder<T>::start("head", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;hr&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#SELF}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  hr() {
		return MarkupBuilder<T>::start("hr", TagClosingPolicy::SELF);
	}

	/**
	 * Starts the &lt;html&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  html() {
		return MarkupBuilder<T>::start("html", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;i&gt; tag.
	 * [italic font]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  i() {
		return MarkupBuilder<T>::start("i", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;iframe&gt; tag.
	 * [inline subwindow]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  iframe() {
		return MarkupBuilder<T>::start("iframe", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;img&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#SELF}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  img() {
		return MarkupBuilder<T>::start("img", TagClosingPolicy::SELF);
	}

	/**
	 * Starts the &lt;input&gt; tag.
	 * [form control]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#SELF}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  input() {
		return MarkupBuilder<T>::start("input", TagClosingPolicy::SELF);
	}

	/**
	 * Starts the &lt;ins&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  ins() {
		return MarkupBuilder<T>::start("ins", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;isindex&gt; tag.
	 * [single-line text input control (DEPRECATED)]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#SELF}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  isindex() {
		return MarkupBuilder<T>::start("isindex", TagClosingPolicy::SELF);
	}

	/**
	 * Starts the &lt;kbd&gt; tag.
	 * [something user would type]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  kbd() {
		return MarkupBuilder<T>::start("kbd", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;label&gt; tag.
	 * [Each label must not contain more than ONE field,       Label elements shouldn't be nested.]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  label() {
		return MarkupBuilder<T>::start("label", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;legend&gt; tag.
	 * [fieldset label]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  legend() {
		return MarkupBuilder<T>::start("legend", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;li&gt; tag.
	 * [list item]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  li() {
		return MarkupBuilder<T>::start("li", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;link&gt; tag.
	 * [Relationship values can be used in principle:, ,       a) for document specific toolbars/menus when used,          with the link element in document head e.g.,            start, contents, previous, next, index, end, help,       b) to link to a separate style sheet (rel="stylesheet"),       c) to make a link to a script (rel="script"),       d) by stylesheets to control how collections of,          html nodes are rendered into printed documents,       e) to make a link to a printable version of this document,          e.g. a PostScript or PDF version (rel="alternate" media="print")]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#SELF}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  link() {
		return MarkupBuilder<T>::start("link", TagClosingPolicy::SELF);
	}

	/**
	 * Starts the &lt;map&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  map() {
		return MarkupBuilder<T>::start("map", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;menu&gt; tag.
	 * [single column list (DEPRECATED)]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  menu() {
		return MarkupBuilder<T>::start("menu", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;meta&gt; tag.
	 * [generic metainformation]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#SELF}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  meta() {
		return MarkupBuilder<T>::start("meta", TagClosingPolicy::SELF);
	}

	/**
	 * Starts the &lt;noframes&gt; tag.
	 * [alternate content container for non frame-based rendering]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  noframes() {
		return MarkupBuilder<T>::start("noframes", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;noscript&gt; tag.
	 * [alternate content container for non script-based rendering]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  noscript() {
		return MarkupBuilder<T>::start("noscript", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;object&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  object() {
		return MarkupBuilder<T>::start("object", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;ol&gt; tag.
	 * [Ordered (numbered) list]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  ol() {
		return MarkupBuilder<T>::start("ol", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;optgroup&gt; tag.
	 * [option group]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  optgroup() {
		return MarkupBuilder<T>::start("optgroup", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;option&gt; tag.
	 * [selectable choice]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  option() {
		return MarkupBuilder<T>::start("option", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;p&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  p() {
		return MarkupBuilder<T>::start("p", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;param&gt; tag.
	 * [param is used to supply a named property value.,       In XML it would seem natural to follow RDF and support an,       abbreviated syntax where the param elements are replaced,       by attribute value pairs on the object start tag.]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#SELF}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  param() {
		return MarkupBuilder<T>::start("param", TagClosingPolicy::SELF);
	}

	/**
	 * Starts the &lt;pre&gt; tag.
	 * [content is "Inline" excluding ,          "img|object|applet|big|small|sub|sup|font|basefont"]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  pre() {
		return MarkupBuilder<T>::start("pre", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;q&gt; tag.
	 * [inlined quote]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  q() {
		return MarkupBuilder<T>::start("q", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;s&gt; tag.
	 * [strike-through]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  s() {
		return MarkupBuilder<T>::start("s", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;samp&gt; tag.
	 * [sample]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  samp() {
		return MarkupBuilder<T>::start("samp", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;script&gt; tag.
	 * [script statements, which may include CDATA sections]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  script() {
		return MarkupBuilder<T>::start("script", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;select&gt; tag.
	 * [option selector]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  select() {
		return MarkupBuilder<T>::start("select", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;small&gt; tag.
	 * [smaller font]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  small() {
		return MarkupBuilder<T>::start("small", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;span&gt; tag.
	 * [generic language/style container]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  span() {
		return MarkupBuilder<T>::start("span", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;strike&gt; tag.
	 * [strike-through]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  strike() {
		return MarkupBuilder<T>::start("strike", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;strong&gt; tag.
	 * [strong emphasis]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  strong() {
		return MarkupBuilder<T>::start("strong", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;style&gt; tag.
	 * [style info, which may include CDATA sections]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  style() {
		return MarkupBuilder<T>::start("style", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;sub&gt; tag.
	 * [subscript]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  sub() {
		return MarkupBuilder<T>::start("sub", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;sup&gt; tag.
	 * [superscript]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  sup() {
		return MarkupBuilder<T>::start("sup", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;table&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  table() {
		return MarkupBuilder<T>::start("table", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;tbody&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  tbody() {
		return MarkupBuilder<T>::start("tbody", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;td&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  td() {
		return MarkupBuilder<T>::start("td", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;textarea&gt; tag.
	 * [multi-line text field]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  textarea() {
		return MarkupBuilder<T>::start("textarea", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;tfoot&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  tfoot() {
		return MarkupBuilder<T>::start("tfoot", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;th&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  th() {
		return MarkupBuilder<T>::start("th", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;thead&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  thead() {
		return MarkupBuilder<T>::start("thead", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;title&gt; tag.
	 * [The title element is not considered part of the flow of text.,       It should be displayed, for example as the page header or,       window title. Exactly one title is required per document.]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  title() {
		return MarkupBuilder<T>::start("title", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;tr&gt; tag.
	 * []
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  tr() {
		return MarkupBuilder<T>::start("tr", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;tt&gt; tag.
	 * [fixed pitch font]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  tt() {
		return MarkupBuilder<T>::start("tt", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;u&gt; tag.
	 * [underline]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  u() {
		return MarkupBuilder<T>::start("u", TagClosingPolicy::NORMAL);
	}

	/**
	 * Starts the &lt;ul&gt; tag.
	 * [Unordered list]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#PAIR}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  ul() {
		return MarkupBuilder<T>::start("ul", TagClosingPolicy::PAIR);
	}

	/**
	 * Starts the &lt;var&gt; tag.
	 * [variable]
	 * <p>
	 * Tag Closing Policy: {@link MarkupBuilder.TagClosingPolicy#NORMAL}
	 * @return this, never <code>null</code> 
	 */
	virtual T &  var() {
		return MarkupBuilder<T>::start("var", TagClosingPolicy::NORMAL);
	}

	/**
	 * Sets the <code>abbr</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  abbr(const string & value) {
		return MarkupBuilder<T>::attr("abbr", value);
	}

	/**
	 * Sets the <code>accept</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  accept(const string & value) {
		return MarkupBuilder<T>::attr("accept", value);
	}

	/**
	 * Sets the <code>accept-charset</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  acceptCharset(const string & value) {
		return MarkupBuilder<T>::attr("accept-charset", value);
	}

	/**
	 * Sets the <code>accesskey</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  accesskey(const string & value) {
		return MarkupBuilder<T>::attr("accesskey", value);
	}

	/**
	 * Sets the <code>action</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  action(const string & value) {
		return MarkupBuilder<T>::attr("action", value);
	}

	/**
	 * Sets the <code>align</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  align(const string & value) {
		return MarkupBuilder<T>::attr("align", value);
	}

	/**
	 * Sets the <code>alink</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  alink(const string & value) {
		return MarkupBuilder<T>::attr("alink", value);
	}

	/**
	 * Sets the <code>alt</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  alt(const string & value) {
		return MarkupBuilder<T>::attr("alt", value);
	}

	/**
	 * Sets the <code>archive</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  archive(const string & value) {
		return MarkupBuilder<T>::attr("archive", value);
	}

	/**
	 * Sets the <code>axis</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  axis(const string & value) {
		return MarkupBuilder<T>::attr("axis", value);
	}

	/**
	 * Sets the <code>background</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  background(const string & value) {
		return MarkupBuilder<T>::attr("background", value);
	}

	/**
	 * Sets the <code>bgcolor</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  bgcolor(const string & value) {
		return MarkupBuilder<T>::attr("bgcolor", value);
	}

	/**
	 * Sets the <code>border</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  border(const string & value) {
		return MarkupBuilder<T>::attr("border", value);
	}

	/**
	 * Sets the <code>cellpadding</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  cellpadding(const string & value) {
		return MarkupBuilder<T>::attr("cellpadding", value);
	}

	/**
	 * Sets the <code>cellspacing</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  cellspacing(const string & value) {
		return MarkupBuilder<T>::attr("cellspacing", value);
	}

	/**
	 * Sets the <code>char</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  charAttr(const string & value) {
		return MarkupBuilder<T>::attr("char", value);
	}

	/**
	 * Sets the <code>charoff</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  charoff(const string & value) {
		return MarkupBuilder<T>::attr("charoff", value);
	}

	/**
	 * Sets the <code>charset</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  charset(const string & value) {
		return MarkupBuilder<T>::attr("charset", value);
	}

	/**
	 * Sets the <code>checked</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  checked(const string & value) {
		return MarkupBuilder<T>::attr("checked", value);
	}

	/**
	 * Sets the <code>cite</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  cite(const string & value) {
		return MarkupBuilder<T>::attr("cite", value);
	}

	/**
	 * Sets the <code>class</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  classAttr(const string & value) {
		return MarkupBuilder<T>::attr("class", value);
	}

	/**
	 * Sets the <code>classid</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  classid(const string & value) {
		return MarkupBuilder<T>::attr("classid", value);
	}

	/**
	 * Sets the <code>clear</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  clear(const string & value) {
		return MarkupBuilder<T>::attr("clear", value);
	}

	/**
	 * Sets the <code>code</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  code(const string & value) {
		return MarkupBuilder<T>::attr("code", value);
	}

	/**
	 * Sets the <code>codebase</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  codebase(const string & value) {
		return MarkupBuilder<T>::attr("codebase", value);
	}

	/**
	 * Sets the <code>codetype</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  codetype(const string & value) {
		return MarkupBuilder<T>::attr("codetype", value);
	}

	/**
	 * Sets the <code>color</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  color(const string & value) {
		return MarkupBuilder<T>::attr("color", value);
	}

	/**
	 * Sets the <code>cols</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  cols(const string & value) {
		return MarkupBuilder<T>::attr("cols", value);
	}

	/**
	 * Sets the <code>colspan</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  colspan(const string & value) {
		return MarkupBuilder<T>::attr("colspan", value);
	}

	/**
	 * Sets the <code>compact</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  compact(const string & value) {
		return MarkupBuilder<T>::attr("compact", value);
	}

	/**
	 * Sets the <code>content</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  content(const string & value) {
		return MarkupBuilder<T>::attr("content", value);
	}

	/**
	 * Sets the <code>coords</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  coords(const string & value) {
		return MarkupBuilder<T>::attr("coords", value);
	}

	/**
	 * Sets the <code>data</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  data(const string & value) {
		return MarkupBuilder<T>::attr("data", value);
	}

	/**
	 * Sets the <code>datetime</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  datetime(const string & value) {
		return MarkupBuilder<T>::attr("datetime", value);
	}

	/**
	 * Sets the <code>declare</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  declare(const string & value) {
		return MarkupBuilder<T>::attr("declare", value);
	}

	/**
	 * Sets the <code>defer</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  defer(const string & value) {
		return MarkupBuilder<T>::attr("defer", value);
	}

	/**
	 * Sets the <code>dir</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  dir(const string & value) {
		return MarkupBuilder<T>::attr("dir", value);
	}

	/**
	 * Sets the <code>disabled</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  disabled(const string & value) {
		return MarkupBuilder<T>::attr("disabled", value);
	}

	/**
	 * Sets the <code>enctype</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  enctype(const string & value) {
		return MarkupBuilder<T>::attr("enctype", value);
	}

	/**
	 * Sets the <code>face</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  face(const string & value) {
		return MarkupBuilder<T>::attr("face", value);
	}

	/**
	 * Sets the <code>for</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  forAttr(const string & value) {
		return MarkupBuilder<T>::attr("for", value);
	}

	/**
	 * Sets the <code>frame</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  frame(const string & value) {
		return MarkupBuilder<T>::attr("frame", value);
	}

	/**
	 * Sets the <code>frameborder</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  frameborder(const string & value) {
		return MarkupBuilder<T>::attr("frameborder", value);
	}

	/**
	 * Sets the <code>headers</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  headers(const string & value) {
		return MarkupBuilder<T>::attr("headers", value);
	}

	/**
	 * Sets the <code>height</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  height(const string & value) {
		return MarkupBuilder<T>::attr("height", value);
	}

	/**
	 * Sets the <code>href</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  href(const string & value) {
		return MarkupBuilder<T>::attr("href", value);
	}

	/**
	 * Sets the <code>hreflang</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  hreflang(const string & value) {
		return MarkupBuilder<T>::attr("hreflang", value);
	}

	/**
	 * Sets the <code>hspace</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  hspace(const string & value) {
		return MarkupBuilder<T>::attr("hspace", value);
	}

	/**
	 * Sets the <code>http-equiv</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  httpEquiv(const string & value) {
		return MarkupBuilder<T>::attr("http-equiv", value);
	}

	/**
	 * Sets the <code>id</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  id(const string & value) {
		return MarkupBuilder<T>::attr("id", value);
	}

	/**
	 * Sets the <code>ismap</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  ismap(const string & value) {
		return MarkupBuilder<T>::attr("ismap", value);
	}

	/**
	 * Sets the <code>label</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  label(const string & value) {
		return MarkupBuilder<T>::attr("label", value);
	}

	/**
	 * Sets the <code>lang</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  lang(const string & value) {
		return MarkupBuilder<T>::attr("lang", value);
	}

	/**
	 * Sets the <code>language</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  language(const string & value) {
		return MarkupBuilder<T>::attr("language", value);
	}

	/**
	 * Sets the <code>link</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  link(const string & value) {
		return MarkupBuilder<T>::attr("link", value);
	}

	/**
	 * Sets the <code>longdesc</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  longdesc(const string & value) {
		return MarkupBuilder<T>::attr("longdesc", value);
	}

	/**
	 * Sets the <code>marginheight</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  marginheight(const string & value) {
		return MarkupBuilder<T>::attr("marginheight", value);
	}

	/**
	 * Sets the <code>marginwidth</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  marginwidth(const string & value) {
		return MarkupBuilder<T>::attr("marginwidth", value);
	}

	/**
	 * Sets the <code>maxlength</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  maxlength(const string & value) {
		return MarkupBuilder<T>::attr("maxlength", value);
	}

	/**
	 * Sets the <code>media</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  media(const string & value) {
		return MarkupBuilder<T>::attr("media", value);
	}

	/**
	 * Sets the <code>method</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  method(const string & value) {
		return MarkupBuilder<T>::attr("method", value);
	}

	/**
	 * Sets the <code>multiple</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  multiple(const string & value) {
		return MarkupBuilder<T>::attr("multiple", value);
	}

	/**
	 * Sets the <code>name</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  name(const string & value) {
		return MarkupBuilder<T>::attr("name", value);
	}

	/**
	 * Sets the <code>nohref</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  nohref(const string & value) {
		return MarkupBuilder<T>::attr("nohref", value);
	}

	/**
	 * Sets the <code>noshade</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  noshade(const string & value) {
		return MarkupBuilder<T>::attr("noshade", value);
	}

	/**
	 * Sets the <code>nowrap</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  nowrap(const string & value) {
		return MarkupBuilder<T>::attr("nowrap", value);
	}

	/**
	 * Sets the <code>object</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  object(const string & value) {
		return MarkupBuilder<T>::attr("object", value);
	}

	/**
	 * Sets the <code>onblur</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onblur(const string & value) {
		return MarkupBuilder<T>::attr("onblur", value);
	}

	/**
	 * Sets the <code>onchange</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onchange(const string & value) {
		return MarkupBuilder<T>::attr("onchange", value);
	}

	/**
	 * Sets the <code>onclick</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onclick(const string & value) {
		return MarkupBuilder<T>::attr("onclick", value);
	}

	/**
	 * Sets the <code>ondblclick</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  ondblclick(const string & value) {
		return MarkupBuilder<T>::attr("ondblclick", value);
	}

	/**
	 * Sets the <code>onfocus</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onfocus(const string & value) {
		return MarkupBuilder<T>::attr("onfocus", value);
	}

	/**
	 * Sets the <code>onkeydown</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onkeydown(const string & value) {
		return MarkupBuilder<T>::attr("onkeydown", value);
	}

	/**
	 * Sets the <code>onkeypress</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onkeypress(const string & value) {
		return MarkupBuilder<T>::attr("onkeypress", value);
	}

	/**
	 * Sets the <code>onkeyup</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onkeyup(const string & value) {
		return MarkupBuilder<T>::attr("onkeyup", value);
	}

	/**
	 * Sets the <code>onload</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onload(const string & value) {
		return MarkupBuilder<T>::attr("onload", value);
	}

	/**
	 * Sets the <code>onmousedown</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onmousedown(const string & value) {
		return MarkupBuilder<T>::attr("onmousedown", value);
	}

	/**
	 * Sets the <code>onmousemove</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onmousemove(const string & value) {
		return MarkupBuilder<T>::attr("onmousemove", value);
	}

	/**
	 * Sets the <code>onmouseout</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onmouseout(const string & value) {
		return MarkupBuilder<T>::attr("onmouseout", value);
	}

	/**
	 * Sets the <code>onmouseover</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onmouseover(const string & value) {
		return MarkupBuilder<T>::attr("onmouseover", value);
	}

	/**
	 * Sets the <code>onmouseup</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onmouseup(const string & value) {
		return MarkupBuilder<T>::attr("onmouseup", value);
	}

	/**
	 * Sets the <code>onreset</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onreset(const string & value) {
		return MarkupBuilder<T>::attr("onreset", value);
	}

	/**
	 * Sets the <code>onselect</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onselect(const string & value) {
		return MarkupBuilder<T>::attr("onselect", value);
	}

	/**
	 * Sets the <code>onsubmit</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onsubmit(const string & value) {
		return MarkupBuilder<T>::attr("onsubmit", value);
	}

	/**
	 * Sets the <code>onunload</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  onunload(const string & value) {
		return MarkupBuilder<T>::attr("onunload", value);
	}

	/**
	 * Sets the <code>profile</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  profile(const string & value) {
		return MarkupBuilder<T>::attr("profile", value);
	}

	/**
	 * Sets the <code>prompt</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  prompt(const string & value) {
		return MarkupBuilder<T>::attr("prompt", value);
	}

	/**
	 * Sets the <code>readonly</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  readonly(const string & value) {
		return MarkupBuilder<T>::attr("readonly", value);
	}

	/**
	 * Sets the <code>rel</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  rel(const string & value) {
		return MarkupBuilder<T>::attr("rel", value);
	}

	/**
	 * Sets the <code>rev</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  rev(const string & value) {
		return MarkupBuilder<T>::attr("rev", value);
	}

	/**
	 * Sets the <code>rows</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  rows(const string & value) {
		return MarkupBuilder<T>::attr("rows", value);
	}

	/**
	 * Sets the <code>rowspan</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  rowspan(const string & value) {
		return MarkupBuilder<T>::attr("rowspan", value);
	}

	/**
	 * Sets the <code>rules</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  rules(const string & value) {
		return MarkupBuilder<T>::attr("rules", value);
	}

	/**
	 * Sets the <code>scheme</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  scheme(const string & value) {
		return MarkupBuilder<T>::attr("scheme", value);
	}

	/**
	 * Sets the <code>scope</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  scope(const string & value) {
		return MarkupBuilder<T>::attr("scope", value);
	}

	/**
	 * Sets the <code>scrolling</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  scrolling(const string & value) {
		return MarkupBuilder<T>::attr("scrolling", value);
	}

	/**
	 * Sets the <code>selected</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  selected(const string & value) {
		return MarkupBuilder<T>::attr("selected", value);
	}

	/**
	 * Sets the <code>shape</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  shape(const string & value) {
		return MarkupBuilder<T>::attr("shape", value);
	}

	/**
	 * Sets the <code>size</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  size(const string & value) {
		return MarkupBuilder<T>::attr("size", value);
	}

	/**
	 * Sets the <code>span</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  span(const string & value) {
		return MarkupBuilder<T>::attr("span", value);
	}

	/**
	 * Sets the <code>src</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  src(const string & value) {
		return MarkupBuilder<T>::attr("src", value);
	}

	/**
	 * Sets the <code>standby</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  standby(const string & value) {
		return MarkupBuilder<T>::attr("standby", value);
	}

	/**
	 * Sets the <code>start</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  startAttr(const string & value) {
		return MarkupBuilder<T>::attr("start", value);
	}

	/**
	 * Sets the <code>style</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  style(const string & value) {
		return MarkupBuilder<T>::attr("style", value);
	}

	/**
	 * Sets the <code>summary</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  summary(const string & value) {
		return MarkupBuilder<T>::attr("summary", value);
	}

	/**
	 * Sets the <code>tabindex</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  tabindex(const string & value) {
		return MarkupBuilder<T>::attr("tabindex", value);
	}

	/**
	 * Sets the <code>target</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  target(const string & value) {
		return MarkupBuilder<T>::attr("target", value);
	}

	/**
	 * Sets the <code>text</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  textAttr(const string & value) {
		return MarkupBuilder<T>::attr("text", value);
	}

	/**
	 * Sets the <code>title</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  title(const string & value) {
		return MarkupBuilder<T>::attr("title", value);
	}

	/**
	 * Sets the <code>type</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  type(const string & value) {
		return MarkupBuilder<T>::attr("type", value);
	}

	/**
	 * Sets the <code>usemap</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  usemap(const string & value) {
		return MarkupBuilder<T>::attr("usemap", value);
	}

	/**
	 * Sets the <code>valign</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  valign(const string & value) {
		return MarkupBuilder<T>::attr("valign", value);
	}

	/**
	 * Sets the <code>value</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  value(const string & value) {
		return MarkupBuilder<T>::attr("value", value);
	}

	/**
	 * Sets the <code>valuetype</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  valuetype(const string & value) {
		return MarkupBuilder<T>::attr("valuetype", value);
	}

	/**
	 * Sets the <code>vlink</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  vlink(const string & value) {
		return MarkupBuilder<T>::attr("vlink", value);
	}

	/**
	 * Sets the <code>vspace</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  vspace(const string & value) {
		return MarkupBuilder<T>::attr("vspace", value);
	}

	/**
	 * Sets the <code>width</code> attribute on the last started tag that 
	 * has not been closed.
	 *
	 * @param value the value to set, maybe <code>null</code>
	 * @return this, never <code>null</code>
	 */
	virtual T &  width(const string & value) {
		return MarkupBuilder<T>::attr("width", value);
	}
};

}}}

#endif
