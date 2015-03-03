/*
 * SizeRequirements.cpp
 *
 *  Created on: Mon Jun 11 14:42:09 2012
 *      Author: Johannes Unger
 */

#include "SizeRequirements.hpp"
#include <algorithm>
#include <limits.h>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class SizeRequirements
//=============================================================================
//-----------------------------------------------------------------------------
SizeRequirements::Ints SizeRequirements::adjustSizes(int delta,
		const Container &children)
{
	return Ints();
}
//-----------------------------------------------------------------------------
void SizeRequirements::calculateAlignedPositions(int allocated,
		SizeRequirements total, const Container & children,
		SizeRequirements::Ints & offsets,
		SizeRequirements::Ints & spans, bool normal)
{
	float totalAlignment = normal ? total.alignment : 1.0f - total.alignment;
	int totalAscent = (int) (allocated * totalAlignment);
	int totalDescent = allocated - totalAscent;
	for (size_t i = 0; i < children.size(); ++i) {
		const SizeRequirements &req = children[i];
		float alignment = normal ? req.alignment : 1.0f - req.alignment;
		int maxAscent = (int) (req.maximum * alignment);
		int maxDescent = req.maximum - maxAscent;
		int ascent = std::min(totalAscent, maxAscent);
		int descent = std::min(totalDescent, maxDescent);

		offsets[i] = totalAscent - ascent;
		spans[i] = (int) std::min((long) ascent + (long) descent,
				LONG_MAX);
	}
}
//-----------------------------------------------------------------------------
void SizeRequirements::calculateTiledPositions(int allocated,
		const SizeRequirements &total,
		const Container & children,
		SizeRequirements::Ints & offsets,
		SizeRequirements::Ints & spans, bool forward)
{
	// The total argument turns out to be a bad idea since the
	// total of all the children can overflow the integer used to
	// hold the total.  The total must therefore be calculated and
	// stored in long variables.
	long min = 0;
	long pref = 0;
	long max = 0;
	for (size_t i = 0; i < children.size(); ++i) {
		min += children[i].minimum;
		pref += children[i].preferred;
		max += children[i].maximum;
	}
	if (allocated >= pref) {
		expandedTile(allocated, min, pref, max, children, offsets, spans,
				forward);
	} else {
		compressedTile(allocated, min, pref, max, children, offsets, spans,
				forward);
	}
}
//-----------------------------------------------------------------------------
SizeRequirements SizeRequirements::getAlignedSizeRequirements(
		const Container & children)
{
	SizeRequirements totalAscent;
	SizeRequirements totalDescent;
	for (size_t i = 0; i < children.size(); i++) {
		const SizeRequirements &req = children[i];

		int ascent = (int) (req.alignment * req.minimum);
		int descent = req.minimum - ascent;
		totalAscent.minimum = std::max(ascent, totalAscent.minimum);
		totalDescent.minimum = std::max(descent, totalDescent.minimum);

		ascent = (int) (req.alignment * req.preferred);
		descent = req.preferred - ascent;
		totalAscent.preferred = std::max(ascent, totalAscent.preferred);
		totalDescent.preferred = std::max(descent, totalDescent.preferred);

		ascent = (int) (req.alignment * req.maximum);
		descent = req.maximum - ascent;
		totalAscent.maximum = std::max(ascent, totalAscent.maximum);
		totalDescent.maximum = std::max(descent, totalDescent.maximum);
	}
	int min = (int) std::min(
			(long) totalAscent.minimum + (long) totalDescent.minimum,
			LONG_MAX);
	int pref = (int) std::min(
			(long) totalAscent.preferred + (long) totalDescent.preferred,
			LONG_MAX);
	int max = (int) std::min(
			(long) totalAscent.maximum + (long) totalDescent.maximum,
			LONG_MAX);
	float alignment = 0.0f;
	if (min > 0) {
		alignment = (float) totalAscent.minimum / min;
		alignment = alignment > 1.0f ? 1.0f : alignment < 0.0f ? 0.0f
				: alignment;
	}
	return SizeRequirements(min, pref, max, alignment);
}
//-----------------------------------------------------------------------------
SizeRequirements SizeRequirements::getTiledSizeRequirements(
		const Container & children)
{
	SizeRequirements total;
	for (size_t i = 0; i < children.size(); i++) {
		const SizeRequirements &req = children[i];
		total.minimum = (int) std::min(
				(long) total.minimum + (long) req.minimum, LONG_MAX);
		total.preferred = (int) std::min(
				(long) total.preferred + (long) req.preferred,
				LONG_MAX);
		total.maximum = (int) std::min(
				(long) total.maximum + (long) req.maximum, LONG_MAX);
	}
	return total;
}
//-----------------------------------------------------------------------------
void SizeRequirements::expandedTile(int allocated, long min,
		long pref, long max, const Container & request,
		SizeRequirements::Ints & offsets,
		SizeRequirements::Ints & spans, bool forward)
{
	// ---- determine what we have to work with ----
	float totalPlay = std::min(allocated - pref, max - pref);
	float factor = (max - pref == 0) ? 0.0f : totalPlay / (max - pref);

	// ---- make the adjustments ----
	int totalOffset;
	if (forward) {
		// lay out with offsets increasing from 0
		totalOffset = 0;
		for (size_t i = 0; i < spans.size(); ++i) {
			offsets[i] = totalOffset;
			const SizeRequirements &req = request[i];
			int play = (int) (factor * (req.maximum - req.preferred));
			spans[i] = (int) std::min((long) req.preferred + (long) play,
					LONG_MAX);
			totalOffset = (int) std::min((long) totalOffset + (long) spans[i],
					LONG_MAX);
		}
	} else {
		// lay out with offsets decreasing from the end of the allocation
		totalOffset = allocated;
		for (size_t i = 0; i < spans.size(); ++i) {
			const SizeRequirements &req = request[i];
			int play = (int) (factor * (req.maximum - req.preferred));
			spans[i] = (int) std::min((long) req.preferred + (long) play,
					LONG_MAX);
			offsets[i] = totalOffset - spans[i];
			totalOffset = (int) std::max((long) totalOffset - (long) spans[i],
					(long) 0);
		}
	}
}
//-----------------------------------------------------------------------------
void SizeRequirements::compressedTile(int allocated, long min,
		long pref, long max, const Container & request,
		SizeRequirements::Ints & offsets,
		SizeRequirements::Ints & spans, bool forward)
{
	// ---- determine what we have to work with ----
	float totalPlay = std::min(pref - allocated, pref - min);
	float factor = (pref - min == 0) ? 0.0f : totalPlay / (pref - min);

	// ---- make the adjustments ----
	int totalOffset;
	if (forward) {
		// lay out with offsets increasing from 0
		totalOffset = 0;
		for (size_t i = 0; i < spans.size(); ++i) {
			offsets[i] = totalOffset;
			const SizeRequirements &req = request[i];
			float play = factor * (req.preferred - req.minimum);
			spans[i] = (int) (req.preferred - play);
			totalOffset = (int) std::min((long) totalOffset + (long) spans[i],
					LONG_MAX);
		}
	} else {
		// lay out with offsets decreasing from the end of the allocation
		totalOffset = allocated;
		for (size_t i = 0; i < spans.size(); ++i) {
			const SizeRequirements &req = request[i];
			float play = factor * (req.preferred - req.minimum);
			spans[i] = (int) (req.preferred - play);
			offsets[i] = totalOffset - spans[i];
			totalOffset = (int) std::max((long) totalOffset - (long) spans[i],
					(long) 0);
		}
	}
}

}}} // SizeRequirements::namespace(s)
