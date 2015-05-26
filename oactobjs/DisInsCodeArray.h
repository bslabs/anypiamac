// Declarations for the <see cref="DisInsCodeArray"/> class to manage an array
// of disability insured status codes.
//
// $Id: DisInsCodeArray.h 1.14 2011/07/29 15:26:17EDT 044579 Development  $

#pragma once

#include <string>
#include <vector>
#include "dinscode.h"
#include "Resource.h"
#include "PiaException.h"

/// <summary>Manages an array of annual <see cref="DisInsCode"/>'s.</summary>
///
/// <remarks>Manages an array of annual <see cref="DisInsCode"/>'s.</remarks>
class DisInsCodeArray
{
private:
  /// <summary>Disability insured status code data.</summary>
  std::vector< DisInsCode > theCodes;
  /// <summary>First year of data.</summary>
  int firstYear;
  /// <summary>Last year of data.</summary>
  int lastYear;
public:
  DisInsCodeArray( int newFirstYear, int newLastYear );
  /// <summary>Returns the disability insured status code at end of year,
  /// non-const version.</summary>
  ///
  /// <returns>The disability insured status code at end of year.</returns>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> if year is out
  /// of range (only in debug mode).</exception>
  ///
  /// <param name="year">Year for which disability insured status code is
  /// desired.</param>
  DisInsCode& operator[]( int year )
  {
  #if !defined(NDEBUG)
    if (year < firstYear || year > lastYear)
      throw PiaException("Year out of range in DisInsCodeArray::operator[]");
  #endif
    return(theCodes[year - firstYear]);
  }
  /// <summary>Returns the disability insured status code at end of year,
  /// const version.</summary>
  ///
  /// <returns>The disability insured status code at end of year.</returns>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> if year is out
  /// of range (only in debug mode).</exception>
  ///
  /// <param name="year">Year for which disability insured status code is
  /// desired.</param>
  const DisInsCode operator[]( int year ) const
  {
  #if !defined(NDEBUG)
    if (year < firstYear || year > lastYear)
      throw PiaException("Year out of range in DisInsCodeArray::operator[]");
  #endif
    return(theCodes[year - firstYear]);
  }
  void assign( const DisInsCodeArray& newDataArray, int firstDataYear,
    int lastDataYear);
  void assign( const DisInsCode& newData, int firstDataYear,
    int lastDataYear);
  void deleteContents();
  /// <summary>Returns the first year of data.</summary>
  ///
  /// <returns>The first year of data.</returns>
  int getFirstYear() const { return(firstYear); }
  /// <summary>Returns the last year of data.</summary>
  ///
  /// <returns>The last year of data.</returns>
  int getLastYear() const { return(lastYear); }
  bool isNonzero() const;
  /// <summary>Sets the first year of data.</summary>
  ///
  /// <param name="newFirstYear">New first year of data.</param>
  void setFirstYear( int newFirstYear ) { firstYear = newFirstYear; }
  /// <summary>Sets the last year of data.</summary>
  ///
  /// <param name="newLastYear">New last year of data.</param>
  void setLastYear( int newLastYear ) { lastYear = newLastYear; }
  std::string toString() const;
};
