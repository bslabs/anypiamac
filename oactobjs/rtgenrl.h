// Declarations for the <see cref="RtGeneral"/> class - parent of classes to
// manage one retirement test projection.

// $Id: rtgenrl.h 1.21 2011/08/09 14:58:42EDT 044579 Development  $

#pragma once

#include "dbleann.h"
#include "age.h"
#include "oactcnst.h"

/// <summary>Parent of classes to manage the retirement test exempt amount
/// projections.</summary>
///
/// <remarks>Child classes are <see cref="Rtea62"/>, <see cref="Rtea62m"/>,
/// <see cref="Rtea65"/>, <see cref="Rtea65m"/>, and <see cref="RteaHigh"/>.
/// </remarks>
///
/// <seealso cref="Rtea62"/>
/// <seealso cref="Rtea62m"/>
/// <seealso cref="Rtea65"/>
/// <seealso cref="Rtea65m"/>
/// <seealso cref="RteaHigh"/>
class RtGeneral : public DoubleAnnual
{
public:
  RtGeneral( int newLastYear );
  RtGeneral( int newBaseYear, int newLastYear );
  virtual ~RtGeneral();
  void convertMonthToAnnual( const RtGeneral& rteam );
  static Age getMaxAgePL( int year );
  static double project1( double baseAmount, double numerator,
    double denominator, double previous );
};

/// <summary>Manages the annual retirement test exempt amount projections
/// under full retirement age.</summary>
///
/// <remarks>The monthly retirement test exempt amount projections under
/// full retirement age are managed by <see cref="Rtea62m"/>.</remarks>
///
/// <seealso cref="Rtea62m"/>
class Rtea62 : public RtGeneral
{
public:
  Rtea62( int newLastYear );
  Rtea62( int newBaseYear, int newLastYear );
};

/// <summary>Manages the monthly retirement test exempt amount
/// projections under full retirement age.</summary>
///
/// <remarks>The annual retirement test exempt amount projections under
/// full retirement age are managed by <see cref="Rtea62"/>.</remarks>
///
/// <seealso cref="Rtea62"/>
class Rtea62m : public RtGeneral
{
private:
  /// <summary>Historical retirement test exempt amounts.</summary>
  static const double theAmts[];
public:
  Rtea62m( int newLastYear );
  Rtea62m( int newBaseYear, int newLastYear );
  /// <summary>Returns historical monthly exempt amount for specified year.
  /// </summary>
  ///
  /// <returns>Exempt amount for specified year.</returns>
  ///
  /// <param name="year"> Year for which exempt amount is desired.</param>
  static double getHistAmount( int year ) { return theAmts[year - YEAR51]; }
  static int getFraction( int year );
  void initialize();
  void project( int firstYear, const DoubleAnnual& averageWages,
    const DoubleAnnual& cpi );
};

/// <summary>Manages the annual retirement test exempt amount
/// projections over full retirement age.</summary>
///
/// <remarks>The monthly retirement test exempt amount projections over
/// full retirement age are managed by <see cref="Rtea65m"/>.</remarks>
///
/// <seealso cref="Rtea65m"/>
class Rtea65 : public RtGeneral
{
public:
  Rtea65( int newLastYear );
  Rtea65( int newBaseYear, int newLastYear );
};

/// <summary>Manages the monthly retirement test exempt amount
/// projections over full retirement age.</summary>
///
/// <remarks>The annual retirement test exempt amount projections over
/// full retirement age are managed by <see cref="Rtea65"/>.</remarks>
///
/// <seealso cref="Rtea65"/>
class Rtea65m : public RtGeneral
{
private:
  /// <summary>Historical retirement test exempt amounts.</summary>
  static const double theAmts[];
public:
  Rtea65m( int newLastYear );
  Rtea65m( int newBaseYear, int newLastYear );
  /// <summary>Returns historical monthly exempt amount for specified year.
  /// </summary>
  ///
  /// <returns>Exempt amount for specified year.</returns>
  ///
  /// <param name="year"> Year for which exempt amount is desired.</param>
  static double getHistAmount( int year ) { return theAmts[year - YEAR51]; }
  static int getFraction( int year );
  void initialize();
  void project( int firstYear, const DoubleAnnual& averageWages,
    const DoubleAnnual& cpi );
};

/// <summary>Manages the upper annual retirement test exempt
/// amounts when there were two tiers.</summary>
///
/// <remarks>There were two tiers from 1961 to 1972.</remarks>
class RteaHigh : public RtGeneral
{
private:
  /// <summary>First year of two-tier retirement test.</summary>
  static const int YEAR61 = 1961;
  /// <summary>Last year of two-tier retirement test.</summary>
  static const int YEAR72 = 1972;
  /// <summary>Exempt amounts, 1961-1972.</summary>
  static const double rteaHigh[YEAR72 - YEAR61 + 1];
public:
  RteaHigh();
  static int getFraction();
  void initialize();
};
