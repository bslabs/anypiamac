// Declarations for the <see cref="BendPia"/> class to manage array of PIA formula bend points.

// $Id: bendpia.h 1.18 2017/11/14 12:56:12EST 277133 Development  $

#pragma once

#include <cstddef>
#include "avgwg.h"

/// <summary>Manages the array of bend points used in the PIA formula.
/// </summary>
///
/// <remarks>An instance of this class is used in 
/// <see cref="WageIndGeneral"/>. There is an array with 5 slots for bend
/// points.</remarks>
class BendPia
{
private:
	/// <summary>Dimension limits.</summary>
	enum DimLimits {
		/// Maximum possible number of percentages.
		MAXBP = 5
	};
	/// <summary>Array of bend points.</summary>
	double theData[MAXBP];
	/// <summary>Number of bend points in formula.</summary>
	int numBend;
public:
	/// <summary>Three bend points from 1979.</summary>
	static const double BP1979[3];
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
	/// <summary>The type of size in this array.</summary>
	typedef std::size_t size_type;
	/// <summary>The type of difference in this array.</summary>
	typedef ptrdiff_t difference_type;
public:
	BendPia(int newNumBend);
	~BendPia();
	/// <summary>Returns a pia formula bend point, non-const version.</summary>
	///
	/// <returns>A pia formula bend point.</returns>
	///
	/// <param name="index">Index of bend point desired (0-2).</param>
	reference operator[](std::size_t index) { return theData[index]; }
	/// <summary>Returns a pia formula bend point, const version.</summary>
	///
	/// <returns>A pia formula bend point.</returns>
	///
	/// <param name="index">Index of bend point desired (0-2).</param>
	const_reference operator[](std::size_t index) const { return theData[index]; }
	/// <summary>Returns bend points as a simple array.</summary>
	///
	/// <returns>Percentages as a simple array.</returns>
	double* as_array() { return theData; }
	/// <summary>Returns beginning of array, non-const version.</summary>
	///
	/// <returns>Beginning of array.</returns>
	iterator begin() { return theData; }
	/// <summary>Returns beginning of array, non-const version.</summary>
	///
	/// <returns>Beginning of array.</returns>
	const_iterator begin() const { return theData; }
	void deleteContents();
	/// <summary>Returns one past end of array, non-const version.</summary>
	///
	/// <returns>One past end of array.</returns>
	iterator end() { return theData + numBend; }
	/// <summary>Returns one past end of array, const version.</summary>
	///
	/// <returns>One past end of array.</returns>
	const_iterator end() const { return theData + numBend; }
	/// <summary>Returns maximum number of bend points.</summary>
	///
	/// <returns>Number of bend points.</returns>
	size_type max_size() const { return numBend; }
	void project(int eligYear, const AverageWage& fq);
	/// <summary>Returns number of bend points.</summary>
	///
	/// <returns>Number of bend points.</returns>
	size_type size() const { return numBend; }
    /// <summary>Returns the number of bendpoints.</summary>
    ///
    /// <returns>Number of bend points.</returns>
    size_type getNumBend() const { return numBend; }
    /// <summary>Sets the number of bendpoints.</summary>
    ///
    /// <returns>Number of bend points.</returns>
    void setNumBend(int newNumBend );

};

