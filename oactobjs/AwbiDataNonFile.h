// Declaration of the <see cref="AwbiDataNonFile"/> class to manage historical
// Social Security parameters stored with the code.
//
// $Id: AwbiDataNonFile.h 1.12 2011/07/28 10:16:27EDT 044579 Development  $

#pragma once

#include "awbidat.h"
#include "oactcnst.h"

/// <summary>Manages the historical Social Security parameters stored in
/// arrays in the code.</summary>
///
/// <remarks>An instance of this class is used in the Anypia program. See
/// <see cref="AwbiDataFile"/> for a version of this class that stores the
/// parameters in a file.</remarks>
///
/// <seealso cref="AwbiDataFile"/>
class AwbiDataNonFile : public AwbiData
{
private:
  static const double cpiincData[];
  static const double fqData[];
  static const double baseOasdiData[];
  static const double base77Data[];
  static const double baseHiData[];
public:
  AwbiDataNonFile( int newLastYear, int newMaxyear );
  explicit AwbiDataNonFile( int newMaxyear );
  void setData();
  /// <summary>Returns a historical OASDI wage base.</summary>
  ///
  /// <returns>The OASDI wage base for the specified year.</returns>
  //
  /// <param name="year">Year for which OASDI wage base is desired.</param>
  static double getBaseOasdiData( int year )
  { return baseOasdiData[year - YEAR37]; }
  /// <summary>Returns a historical old-law wage base.</summary>
  ///
  /// <returns>The old-law wage base for the specified year.</returns>
  ///
  /// <param name="year">Year for which old-law wage base is desired.</param>
  static double getBase77Data( int year ) { return base77Data[year - 1937]; }
  /// <summary>Returns a historical HI wage base.</summary>
  ///
  /// <returns>The HI wage base for the specified year.</returns>
  ///
  /// <param name="year">Year for which HI wage base is desired.</param>
  static double getBaseHiData( int year )
  { return (year < HI_BASE_YEAR) ?
    baseOasdiData[year - YEAR37] : baseHiData[year - HI_BASE_YEAR]; }
  /// <summary>Returns a historical benefit increase.</summary>
  ///
  /// <returns>The benefit increase for the specified year.</returns>
  ///
  /// <param name="year">Year for which benefit increase is desired.</param>
  static double getCpiincData( int year )
  { return cpiincData[year - YEAR51]; }
  /// <summary>Returns a historical average indexing wage.</summary>
  ///
  /// <returns>The average indexing wage for the specified year.</returns>
  ///
  /// <param name="year">Year for which average indexing wage is desired.
  /// </param>
  static double getFqData( int year ) { return fqData[year - YEAR37]; }
};
