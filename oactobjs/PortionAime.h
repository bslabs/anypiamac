// Declarations needed for class to manage array of portions of AIME in the PIA formula.
//
// $Id: PortionAime.h 1.2 2017/11/14 12:56:11EST 277133 Development  $

#pragma once

#include <cstddef>
#include "avgwg.h"

/// <summary>Manages the array of portions of AIME in the PIA formula.
/// </summary>
///
/// <remarks>An instance of this class is used in <see cref="WageIndGeneral"/>.
/// There is an array with up to 5 slots for portions of AIME.</remarks>
class PortionAime
{
private:
	/// <summary>Dimension limits.</summary>
	enum DimLimits {
		/// Maximum possible number of percentages.
		MAXPART = 5
	};
	/// <summary>Array of bend points.</summary>
	double theData[MAXPART];
	/// <summary>Number of bend points in formula.</summary>
	int numPart;
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
	typedef ptrdiff_t difference_type;
public:
	PortionAime(int newNumPart);
	~PortionAime();
	/// <summary>Returns a pia formula AIME portion, non-const version.</summary>
	///
	/// <returns>A pia formula AIME portion.</returns>
	///
	/// <param name="index">Index of AIME portion desired.</param>
	reference operator[](std::size_t index) { return theData[index]; }
	/// <summary>Returns a pia formula AIME portion, const version.</summary>
	///
	/// <returns>A pia formula AIME portion.</returns>
	///
	/// <param name="index">Index of AIME portion desired.</param>
	const_reference operator[](std::size_t index) const { return theData[index]; }
	/// <summary>Returns AIME portions as a simple array.</summary>
	///
	/// <returns>AIME portions as a simple array.</returns>
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
	iterator end() { return theData + numPart; }
	/// <summary>Returns one past end of array, const version.</summary>
	///
	/// <returns>One past end of array.</returns>
	const_iterator end() const { return theData + numPart; }
	/// <summary>Returns maximum number of AIME portions.</summary>
	///
	/// <returns>Number of AIME portions.</returns>
	size_type max_size() const { return numPart; }
	/// <summary>Returns number of AIME portions.</summary>
	///
	/// <returns>Number of AIME portions.</returns>
	size_type size() const { return numPart; }
    /// <summary>Returns the number of AIME portions.</summary>
    ///
    /// <returns>Number of AIME portions.</returns>
    size_type getNumPart() const { return numPart; }
    void setNumPart(int newNumPart );

};

