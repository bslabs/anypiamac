// Declarations needed for class to manage array of MFB formula bend points.
//
// $Id: bendmfb.h 1.16 2017/09/18 10:37:58EDT 277133 Development 277133(2017/11/14 07:34:35EST) $

#pragma once

#include <cstddef>
#include "avgwg.h"

/// <summary>Manages the array of bend points used in the MFB formula.
/// </summary>
///
/// <remarks>An instance of this class is used in most of the
/// <see cref="PiaMethod"/> classes. There is an array with 4 slots for bend
/// points.</remarks>
class BendMfb
{
private:
	/// <summary>Dimension limits.</summary>
	enum DimLimits {
		/// Number of percentages.
		MAXBP = 4
	};
	/// <summary>Array of bend points.</summary>
	double theData[MAXBP];
public:
	/// <summary>Four bend points from 1979.</summary>
	static const double BP1979[MAXBP];
	/// <summary>The type of data in this array.</summary>
	typedef double value_type;
	/// <summary>An iterator for this array.</summary>
	typedef double* iterator;
	/// <summary>A const iterator for this array.</summary>
	typedef const double* const_iterator;
	/// <summary>A reference for this array.</summary>
	typedef double& reference;
	/// <summary>A const reference for this array.</summary>
	typedef const double& const_reference;
	/// <summary>The type of number of bend points in this array.</summary>
	typedef std::size_t size_type;
	/// <summary>The type of difference in this array.</summary>
	typedef ptrdiff_t difference_type;
public:
	BendMfb();
	~BendMfb();
	/// <summary>Returns an mfb formula bend point, non-const version.</summary>
	///
	/// <returns>An mfb formula bend point.</returns>
	///
	/// <param name="index">Index of bend point desired (0-3).</param>
	reference operator[](std::size_t index) { return theData[index]; }
	/// <summary>Returns an mfb formula bend point, const version.</summary>
	///
	/// <returns>An mfb formula bend point.</returns>
	///
	/// <param name="index">Index of bend point desired (0-3).</param>
	const_reference operator[](std::size_t index) const
	{
		return theData[index];
	}
	/// <summary>Returns bend points as a simple array.</summary>
	///
	/// <returns>Percentages as a simple array.</returns>
	double* as_array() { return theData; }
	/// <summary>Returns beginning of array, non-const version.</summary>
	///
	/// <returns>Beginning of array.</returns>
	iterator begin() { return theData; }
	/// <summary>Returns beginning of array, const version.</summary>
	///
	/// <returns>Beginning of array.</returns>
	const_iterator begin() const { return theData; }
	void deleteContents();
	/// <summary>Returns one past end of array, non-const version.</summary>
	///
	/// <returns>One past end of array.</returns>
	iterator end() { return theData + MAXBP; }
	/// <summary>Returns one past end of array, const version.</summary>
	///
	/// <returns>One past end of array.</returns>
	const_iterator end() const { return theData + MAXBP; }
	/// <summary>Returns maximum number of bend points.</summary>
	///
	/// <returns>Number of bend points.</returns>
	size_type max_size() const { return MAXBP; }
	void project(int eligYear, const AverageWage& fq);
	/// <summary>Returns number of bend points.</summary>
	///
	/// <returns>Number of bend points.</returns>
	size_type size() const { return MAXBP; }
};
