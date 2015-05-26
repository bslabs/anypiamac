// Declarations for the <see cref="BenefitAmountArray"/> class of an
// array of Social Security benefit amounts.
//
// $Id: BenefitAmountArray.h 1.13 2011/07/28 15:19:55EDT 044579 Development  $

#pragma once

#include <vector>
#include "BenefitAmount.h"
#include "oactcnst.h"
#include "PiaException.h"
#include "Resource.h"

/// <summary>Manages an array of <see cref="BenefitAmount"/>'s.</summary>
///
/// <remarks>Manages an array of <see cref="BenefitAmount"/>'s.</remarks>
class BenefitAmountArray
{
private:
  /// <summary>Benefit amount data.</summary>
  std::vector< BenefitAmount > theData;
  /// <summary>Last year of data.</summary>
  int lastYear;
  /// <summary>Indicator for values having been set.</summary>
  bool dirty;
public:
  BenefitAmountArray();
  explicit BenefitAmountArray( int year );
  ~BenefitAmountArray();
  /// <summary>Returns the benefit amount (non-const).</summary>
  ///
  /// <returns>The benefit amount for the specified year.</returns>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_BENARRAY2"/> if year is out of range (in debug
  /// version only).</exception>
  ///
  /// <param name="year">Year for which benefit amount is desired.</param>
  BenefitAmount& operator[]( int year )
  {
  #ifndef NDEBUG
    if (year < YEAR37 || year > lastYear)
      throw PiaException(PIA_IDS_BENARRAY2);
  #endif
    return(theData[year - YEAR37]);
  }
  /// <summary>Returns the benefit amount (const).</summary>
  ///
  /// <returns>The benefit amount for the specified year.</returns>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_BENARRAY2"/> if year is out of range (in debug
  /// version only).</exception>
  ///
  /// <param name="year">Year for which benefit amount is desired.</param>
  const BenefitAmount operator[]( int year ) const
  {
  #ifndef NDEBUG
    if (year < YEAR37 || year > lastYear)
      throw PiaException(PIA_IDS_BENARRAY2);
  #endif
    return(theData[year - YEAR37]);
  }
  void deleteContents();
  /// <summary>Returns the last year of data.</summary>
  ///
  /// <returns>The last year of data.</returns>
  int getLastYear() const { return(lastYear); }
  void initialize( int newLastYear );
  /// <summary>Sets indicator for non-zero values.</summary>
  void setDirty() { dirty = true; }
};
