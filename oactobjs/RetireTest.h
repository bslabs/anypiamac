// Declarations for the <see cref="RetireTest"/> class to manage retirement
// test calculations.
//
// $Id: RetireTest.h 1.12 2011/08/09 14:58:42EDT 044579 Development  $

#pragma once

#include "rtgenrl.h"
class PiaParams;

/// <summary>Contains all of the retirement test exempt amounts and functions
/// to use them.</summary>
///
/// <remarks>Contains all of the retirement test exempt amounts and functions
/// to use them.</remarks>
class RetireTest
{
public:
  /// <summary>Monthly amounts under full retirement age.</summary>
  Rtea62m rtea62m;
  /// <summary>Annual amounts under full retirement age.</summary>
  Rtea62 rtea62;
  /// <summary>Monthly amounts over full retirement age.</summary>
  Rtea65m rtea65m;
  /// <summary>Annual amounts over full retirement age.</summary>
  Rtea65 rtea65;
  /// <summary>Annual amounts at second tier (effective 1961-1972).</summary>
  RteaHigh rteaHigh;
private:
  /// <summary>Pia calculation parameters.</summary>
  const PiaParams& piaParams;
public:
  RetireTest( const PiaParams& newPiaParams, int newLastYear );
  RetireTest( const PiaParams& newPiaParams, int newBaseYear,
    int newLastYear );
  double earningsTestCal( const Age& age, double earnings, int year,
     double benefit ) const;
  void project( int firstYear );
private:
  RetireTest operator=( RetireTest newRetireTest );
};
