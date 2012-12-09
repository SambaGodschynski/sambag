/*
 * GenericFormatter.hpp
 *
 *  Created on: Sat Dec  8 09:41:12 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_GENERICFORMATTER_H
#define SAMBAG_GENERICFORMATTER_H

namespace sambag { namespace disco { namespace genFormatter {
//=============================================================================
/** 
  * @class GenericFormatter.
  */
template <
	class Component,
	 template <class> class ComponentAccessPolicy,
	 template <class> class FormatterPolicy
>
struct GenericFormatter : 
	public FormatterPolicy<ComponentAccessPolicy<Component> >
{
//=============================================================================
	typedef Component ComponentType;
	typedef ComponentAccessPolicy<Component> ComponentAcess;
	typedef FormatterPolicy<ComponentAccessPolicy<Component> > Formatter;
};
}}} // namespace(s)

#endif /* SAMBAG_GENERICFORMATTER_H */
