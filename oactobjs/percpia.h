// Declarations needed for class to manage array of PIA formula percentages.
//
// $Id: percpia.h 1.20 2017/11/14 12:56:08EST 277133 Development  $

#pragma once

#include <cstddef>

/// <summary>Manages the array of percentages used in the PIA formula.</summary>
///
/// <remarks>An instance of this class is a member of the
/// <see cref="WageIndGeneral"/> class.</remarks>
class PercPia
{
private:
	/// <summary>Dimension limits.</summary>
	enum DimLimits {
		/// <summary>Number of percentages in 1977 amendments formula.</summary>
    BASEPERC = 3,
		/// <summary>Maximum number of percentages in any formula.</summary>
		MAXPERC = 5
	};
	/// <summary>Array of percentages.</summary>
	double theData[MAXPERC];
	/// <summary>Number of percentages in formula.</summary>
	int numPerc;
public:
	/// <summary>Number of years of coverage that negates the windfall
	/// elimination provision.</summary>
	static const int WINDFALL_YEARS = 30;
	/// <summary>The three percentages for the regular benefit formula.</summary>
	static const double PERC[BASEPERC];
	/// <summary>The first percentage under the windfall elimination
	/// provision.</summary>
	static const double WINDFALL_PERC;
public:
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
	PercPia();
	~PercPia();
	/// <summary>Returns a pia formula percentage, non-const version.</summary>
	///
	/// <returns>A pia formula percentage.</returns>
	///   
	/// <param name="index">Index of percentage desired (0-2).</param>
	reference operator[](std::size_t index) { return theData[index]; }
	/// <summary>Returns a pia formula percentage, const version.</summary>
	///
	/// <returns>A pia formula percentage.</returns>
	///   
	/// <param name="index">Index of percentage desired (0-2).</param>
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
	iterator end() { return theData + numPerc; }
	/// <summary>Returns one past end of array, const version.</summary>
	///
	/// <returns>One past end of array.</returns>
	const_iterator end() const { return theData + numPerc; }
	/// <summary>Returns maximum number of percentages.</summary>
	///
	/// <returns>Number of percentages.</returns>
	size_type max_size() const { return numPerc; }
	void reset();
	/// <summary>Returns number of percentages.</summary>
	///
	/// <returns>Number of percentages.</returns>
	size_type size() const { return numPerc; }
	void setWindfallPerc(int eligYear, int benefitDate, int specMinTot);
  /// <summary>Returns the number of percentages.</summary>
  ///
  /// <returns>Number of percentages.</returns>
  size_type getNumPerc() const { return numPerc; }
  void setNumPerc(int newNumPerc );
};
