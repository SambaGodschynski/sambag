/*
 * SizeRequirements.hpp
 *
 *  Created on: Mon Jun 11 14:42:09 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SIZEREQUIREMENTS_H
#define SAMBAG_SIZEREQUIREMENTS_H

#include <boost/shared_ptr.hpp>
#include <vector>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class SizeRequirements.
  */
class SizeRequirements {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SizeRequirements> Ptr;
	//-------------------------------------------------------------------------
	typedef std::vector<int> Ints;
	//-------------------------------------------------------------------------
	typedef std::vector<SizeRequirements> Container;
protected:
private:
public:
	//-------------------------------------------------------------------------
	/**
	 * Creates a SizeRequirements object with the specified minimum, preferred,
	 * and maximum sizes and the specified alignment.
	 *
	 * @param min the minimum size >= 0
	 * @param pref the preferred size >= 0
	 * @param max the maximum size >= 0
	 * @param a the alignment >= 0.0f && <= 1.0f
	 */
	SizeRequirements(int min = 0, int pref = 0, int max = 0, float a = 0.5) {
		minimum = min;
		preferred = pref;
		maximum = max;
		alignment = a > 1.0f ? 1.0f : a < 0.0f ? 0.0f : a;
	}
	//-------------------------------------------------------------------------
	/**
	 * The minimum size required.
	 * For a component <code>comp</code>, this should be equal to either
	 * <code>comp.getMinimumSize().width</code> or
	 * <code>comp.getMinimumSize().height</code>.
	 */
	int minimum;
	//-------------------------------------------------------------------------
	/**
	 * The preferred (natural) size.
	 * For a component <code>comp</code>, this should be equal to either
	 * <code>comp.getPreferredSize().width</code> or
	 * <code>comp.getPreferredSize().height</code>.
	 */
	int preferred;
	//-------------------------------------------------------------------------
	/**
	 * The maximum size allowed.
	 * For a component <code>comp</code>, this should be equal to either
	 * <code>comp.getMaximumSize().width</code> or
	 * <code>comp.getMaximumSize().height</code>.
	 */
	int maximum;
	//-------------------------------------------------------------------------
	/**
	 * The alignment, specified as a value between 0.0 and 1.0,
	 * inclusive.
	 * To specify centering, the alignment should be 0.5.
	 */
	float alignment;
	//-------------------------------------------------------------------------
	/**
	 * Adjust a specified array of sizes by a given amount.
	 */
	static Ints adjustSizes(int delta,
			const Container &children);
	//-------------------------------------------------------------------------
	/**
	 * Creates a set of offset/span pairs specifying how to lay out a set
	 * of components with the specified alignments.
	 */
	static void calculateAlignedPositions(int allocated,
			SizeRequirements total, const Container & children,
			Ints & offsets, Ints & spans, bool normal = true);
	//-------------------------------------------------------------------------
	/**
	 * Creates a set of offset/span pairs representing how
	 *  to lay out a set of components end-to-end.
	 */
	static void calculateTiledPositions(int allocated,
			const SizeRequirements &total,
			const Container & children, Ints & offsets,
			Ints & spans, bool forward = true);
	//-------------------------------------------------------------------------
	/**
	 * Determines the total space necessary to align a set of components.
	 */
	static SizeRequirements getAlignedSizeRequirements(
			const Container & children);
	//-------------------------------------------------------------------------
	/**
	 * Determines the total space necessary to place a set of components end-to-end.
	 */
	static SizeRequirements getTiledSizeRequirements(
			const Container & children);
private:
	//-------------------------------------------------------------------------
	static void expandedTile(int allocated, long min, long pref, long max,
			const Container & request, Ints & offsets,
			Ints & spans, bool forward);
	//-------------------------------------------------------------------------
	static void compressedTile(int allocated, long min, long pref, long max,
			const Container & request, Ints & offsets,
			Ints & spans, bool forward);

}; // SizeRequirements
}}} // namespace(s)

#endif /* SAMBAG_SIZEREQUIREMENTS_H */
