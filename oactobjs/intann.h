// Declarations for the <see cref="IntAnnual"/> class to manage an array of
// annual integers.
//
// $Id: intann.h 1.34 2011/08/11 16:23:33EDT 044579 Development  $

#pragma once

#include <vector>
#include "PiaException.h"
#include "Resource.h"

/// <summary>Manages an array of annual integers.</summary>
///
/// <remarks>See <see cref="DoubleAnnual"/> and <see cref="FloatAnnual"/> for
/// similar classes that manage arrays of annual doubles and floats.</remarks>
///
/// <seealso cref="DoubleAnnual"/>
/// <seealso cref="FloatAnnual"/>
class IntAnnual
{
public:
  /// <summary>Integer array.</summary>
  std::vector< int > theData;
  /// <summary>Base year of data.</summary>
  const int baseYear;
private:
  IntAnnual& operator=( const IntAnnual& newData );
public:
  IntAnnual( int newBaseYear, int newLastYear );
  /// <summary>Destructor.</summary>
  ~IntAnnual() { }
  /// <summary>Returns one value of data, non const version.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_INTANN1"/> if year is out of range (only in debug
  /// mode).</exception>
  ///
  /// <returns>One value of data for specified year.</returns>
  ///
  /// <param name="year">Year for which data is desired.</param>
  int& operator[]( int year )
  {
  #if !defined(NDEBUG)
    if (year < baseYear || year > getLastYear()) {
      throw PiaException(PIA_IDS_INTANN1);
    }
  #endif
    return theData[year - baseYear];
  }
  /// <summary>Returns one value of data, const version.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_INTANN1"/> if year is out of range (only in debug
  /// mode).</exception>
  ///
  /// <returns>One value of data for specified year.</returns>
  ///
  /// <param name="year">Year for which data is desired.</param>
  const int operator[]( int year ) const
  {
  #if !defined(NDEBUG)
    if (year < baseYear || year > getLastYear()) {
      throw PiaException(PIA_IDS_INTANN1);
    }
  #endif
     return theData[year - baseYear];
  }
  void assign( const IntAnnual& intAnnual, int firstYear, int lastYear );
  void assign( int newData, int firstYear, int lastYear );
  /// <summary>Fills all data slots with the given value.</summary>
  ///
  /// <param name="value">The value to fill the data slots with.</param>
  void fill( int value )
  {std::fill_n(theData.begin(), theData. size(), value); }
  /// <summary>Returns base year of data.</summary>
  ///
  /// <returns>Base year of data.</returns>
  int getBaseYear() const { return(baseYear); }
  /// <summary>Returns last year of data.</summary>
  ///
  /// <remarks>This is the base year plus the size of the data array, minus 1.
  /// Note that this is less than the base year if the size is zero.</remarks>
  ///
  /// <returns>Last year of data.</returns>
  int getLastYear() const
    { return(baseYear + static_cast< int >(theData.size()) - 1); }
};
