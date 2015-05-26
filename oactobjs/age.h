// Declarations for <see cref="Age"/> class to manage age in years and months.

// $Id: age.h 1.30 2010/12/06 09:09:36EST 044579 Development  $

#pragma once

#include <string>
#include "boost/serialization/access.hpp"

/// <summary>Manages a year and month age stored as integers.</summary>
///
/// <remarks>The year and month are stored as 16-bit unsigned shorts to save
/// space.</remarks>
class Age
{
  /// <summary>Friend class to allow serialization.</summary>
  friend class boost::serialization::access;
  /// <summary>Archives the data.</summary>
  ///
  /// <remarks>When the class Archive corresponds to an output archive, the
  /// & operator is defined similar to <<. Likewise, when the class Archive is
  /// a type of input archive, the & operator is defined similar to >>.
  /// </remarks>
  ///
  /// <param name="ar">The archive to use.</param>
  /// <param name="version">The version of the archive to use.</param>
  template< class Archive >
  void serialize( Archive& ar, const unsigned int version )
    { ar & yearsOfAge & monthsOfAge; }
private:
  /// <summary>Years of age.</summary>
  unsigned short yearsOfAge;
  /// <summary>Months of age (0-11).</summary>
  unsigned short monthsOfAge;
public:
  /// <summary>Initializes age years and months to zero.</summary>
  ///
  /// <remarks>All versions of the constructor initialize the years and
  /// months of age.</remarks>
  Age() : yearsOfAge(0u), monthsOfAge(0u) { }
  /// <summary>Copies age from another Age.</summary>
  ///
  /// <param name="newAge">The age to copy from.</param>
  Age( const Age& newAge ) :
    yearsOfAge(newAge.yearsOfAge), monthsOfAge(newAge.monthsOfAge) { }
  /// <summary>Initializes age years and months from data in an array.
  /// </summary>
  ///
  /// <param name="newYrMo">The years and months in an array.</param>
  Age( unsigned newYrMo[2] ) :
    yearsOfAge((unsigned short)newYrMo[0]),
    monthsOfAge((unsigned short)newYrMo[1]) { }
  /// <summary>Initializes age years and months from two integers.</summary>
  ///
  /// <param name="newYears">The years of age.</param>
  /// <param name="newMonths">The months of age.</param>
  Age( unsigned newYears, unsigned newMonths ) :
    yearsOfAge((unsigned short)newYears),
    monthsOfAge((unsigned short)newMonths) { }
  /// <summary>Destructor.</summary>
  ///
  /// <remarks>Does nothing.</remarks>
  ~Age() { }
  /// <summary>Copies from another <see cref="Age"/>.</summary>
  ///
  /// <returns>A reference to this <see cref="Age"/>.</returns>
  ///
  /// <param name="newAge">The <see cref="Age"/> to copy from.</param>
  Age& operator=( const Age& newAge ) { yearsOfAge = newAge.yearsOfAge;
    monthsOfAge = newAge.monthsOfAge; return *this; }
  bool operator<( const Age& newAge ) const;
  /// <summary>Compares (years,months) ages for equality.</summary>
  ///
  /// <returns>True if years and months agree, false otherwise.</returns>
  ///
  /// <param name="newAge">The age to compare this one to.</param>
  bool operator==( const Age& newAge ) const
    { return (yearsOfAge == newAge.yearsOfAge &&
      monthsOfAge == newAge.monthsOfAge); }
  int operator-( const Age& newAge ) const;
  /// <summary>Zeroes years and months.</summary>
  void deleteContents() { yearsOfAge = monthsOfAge = 0u; }
  /// <summary>Returns months of age.</summary>
  ///
  /// <returns>Number of months of age (0-11).</returns>
  unsigned getMonths() const { return(monthsOfAge); }
  /// <summary>Returns years of age.</summary>
  ///
  /// <returns>Number of years of age.</returns>
  unsigned getYears() const { return(yearsOfAge); }
  void monthsCheck() const;
  /// <summary>Returns the age converted to months.</summary>
  ///
  /// <returns>The age converted to months.</returns>
  int toMonths() const { return monthsOfAge + 12 * yearsOfAge; }
  std::string toString() const;
  std::string toString( const std::string& format ) const;
};
