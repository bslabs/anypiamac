// Declarations needed for class to manage array of MFB formula percentages.
//
// $Id: percmfb.h 1.17 2017/11/14 12:56:12EST 277133 Development  $

#pragma once

#include <cstddef>

/// <summary>Manages the array of percentages used in the MFB formula.</summary>
///
/// <remarks>An instance of this class is a member of most
/// <see cref="PiaMethod"/> classes.</remarks>
class PercMfb
{
private:
	/// <summary>Dimension limits.</summary>
	enum DimLimits {
		/// <summary>Number of percentages.</summary>
		MAXPERC = 4
	};
	/// <summary>Array of percentages.</summary>
	double theData[MAXPERC];
public:
	/// <summary>Four percentages.</summary>
	static const double PERC[MAXPERC];
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
	typedef std::ptrdiff_t difference_type;
	PercMfb();
	~PercMfb();
	/// <summary>Returns an mfb formula percentage, non-const version.</summary>
	///
	/// <returns>An mfb formula percentage.</returns>
	///   
	/// <param name="index">Index of percentage desired (0-3).</param>
	reference operator[](std::size_t index) { return theData[index]; }
	/// <summary>Returns an mfb formula percentage, const version.</summary>
	///
	/// <returns>An mfb formula percentage.</returns>
	///   
	/// <param name="index">Index of percentage desired (0-3).</param>
	const_reference operator[](std::size_t index) const { return theData[index]; }
	/// <summary>Returns percentages as a simple array.</summary>
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
	/// <summary>Returns one past end of array, non-const version.</summary>
	///
	/// <returns>One past end of array.</returns>
	iterator end() { return theData + MAXPERC; }
	/// <summary>Returns one past end of array, const version.</summary>
	///
	/// <returns>One past end of array.</returns>
	const_iterator end() const { return theData + MAXPERC; }
	/// <summary>Returns maximum number of percentages.</summary>
	///
	/// <returns>Number of percentages.</returns>
	size_type max_size() const { return MAXPERC; }
	void reset();
	/// <summary>Returns number of percentages.</summary>
	///
	/// <returns>Number of percentages.</returns>
	size_type size() const { return MAXPERC; }
};
