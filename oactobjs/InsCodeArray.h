// Declarations for the <see cref="InsCodeArray"/> class to manage an array
// of insured status codes.
//
// $Id: InsCodeArray.h 1.14 2011/07/29 15:26:15EDT 044579 Development  $

#pragma once

#include <string>
#include <vector>
#include "inscode.h"
#include "Resource.h"
#include "PiaException.h"

/// <summary>Manages an array of annual insured status codes.</summary>
///
/// <remarks>Manages an array of annual insured status codes.</remarks>
class InsCodeArray
{
private:
  /// <summary>Array of insured status codes.</summary>
  std::vector< InsCode > theCodes;
  /// <summary>First year of data.</summary>
  int firstYear;
  /// <summary>Last year of data.</summary>
  int lastYear;
public:
  InsCodeArray( int newFirstYear, int newLastYear );
  /// <summary>Returns the insured status code at end of year, non-const
  /// version.</summary>
  ///
  /// <returns>The insured status code at end of year.</returns>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> if year is out
  /// of range (only in debug mode).</exception>
  ///
  /// <param name="year">Year for which insured status code is desired.
  /// </param>
  InsCode& operator[]( int year )
  {
  #if !defined(NDEBUG)
    if (year < firstYear || year > lastYear)
      throw PiaException("Year is out of range in InsCodeArray::operator[]");
  #endif
    return(theCodes[year - firstYear]);
  }
  /// <summary>Returns the insured status code at end of year, const version.
  /// </summary>
  ///
  /// <returns>The insured status code at end of year.</returns>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> if year is out
  /// of range (only in debug mode).</exception>
  ///
  /// <param name="year">Year for which insured status code is desired.
  /// </param>
  const InsCode operator[]( int year ) const
  {
  #if !defined(NDEBUG)
    if (year < firstYear || year > lastYear)
      throw PiaException("Year is out of range in InsCodeArray::operator[]");
  #endif
    return(theCodes[year - firstYear]);
  }
  void assign( const InsCodeArray& newDataArray, int firstDataYear,
    int lastDataYear );
  void assign( const InsCode& newData, int firstDataYear, int lastDataYear );
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
