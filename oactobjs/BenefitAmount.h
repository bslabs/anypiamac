// Declarations for the <see cref="BenefitAmount"/> class to handle Social
// Security benefit amount.

// $Id: BenefitAmount.h 1.27 2011/07/28 15:19:55EDT 044579 Development  $

#pragma once

#include <cmath>  // for floor and fmod
#include <utility>  // for rel_ops
#include "qtryear.h"
#include "piaparms.h"
#include "Floor.h"

#if !defined(__SGI_STL_INTERNAL_RELOPS)
using namespace std::rel_ops;
#endif

/// <summary>Represents a benefit amount for one year.</summary>
///
/// <remarks>There are instances of this class in <see cref="PiaData"/> and the
/// various <see cref="PiaMethod"/> classes.</remarks>
class BenefitAmount
{
private:
   /// <summary>Year 1973, change in rounding rules.</summary>
   ///
   /// <remarks>The margin for dime rounding was changed from 0.005 to
   /// 0.0001.</remarks>
   static const int amend73year = 1973;
   /// <summary>Year 1982, change in rounding rules.</summary>
   ///
   /// <remarks>Dime rounding was changed from rounding up to rounding down,
   /// when applied to benefit amounts.</remarks>
   static const int amend82year = 1982;
   /// <summary>Year 2000, change in rounding rules for Social Security
   /// Statement.</summary>
   ///
   /// <remarks>Amounts for Statements were rounded down to $5 before 2000, but
   /// to $1 in 2000 and later.</remarks>
   static const int amend2000year = 2000;
   /// <summary>Benefit amount.</summary>
   double theAmt;
public:
  /// <summary>Initializes amount.</summary>
  ///
  /// <remarks>If argument is omitted, sets amount to 0.0.</remarks>
  ///
  /// <param name="newAmount">New amount to use.</param>
  BenefitAmount( double newAmount = 0.0 ) : theAmt(newAmount) { }
  /// <summary>Applies cola percent to stored benefit.</summary>
  ///
  /// <remarks>Uses the stored amount to call the static version with 3
  /// arguments.</remarks>
  ///
  /// <param name="percent">Percentage increase.</param>
  /// <param name="year">Year of increase.</param>
  void applyCola( double percent, int year )
    { theAmt = applyCola(theAmt, percent, year); }
  /// <summary>Returns the benefit amount.</summary>
  ///
  /// <returns>The benefit amount.</returns>
  double get() const { return theAmt; }
  /// <summary>Compares benefit with benefit from another instance.</summary>
  ///
  /// <returns>True if this benefit is less than argument.</returns>
  ///
  /// <param name="bentemp">Benefit to compare to.</param>
  bool operator<( const BenefitAmount& bentemp ) const
    { return(theAmt < bentemp.theAmt); }
  /// <summary>Rounds the stored PIA or MFB to the appropriate multiple of
  /// $0.10.</summary>
  ///
  /// <remarks>Uses the stored amount to call the static version with 2
  /// arguments. Round a cola calculation up for June 1981 and earlier colas,
  /// down for June 1982 and later colas. Round an aime pia calculation up for
  /// 1982 and earlier eligibility, down for 1983 and later eligibility.
  /// </remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_ROUND"/> if year is less than 1937 (only in debug
  /// mode).</exception>
  ///
  /// <param name="year">Year of benefit increase, or year prior to year of
  /// wage-indexed formula.</param>
  void round( int year ) { theAmt = round(theAmt, year); }
  /// <summary>Rounds the stored PIA or MFB down to a whole dollar for June
  /// 1982 and later.</summary>
  ///
  /// <remarks>Uses the stored amount to call the static version with 2
  /// arguments.</remarks>
  ///
  /// <param name="benDate">The date of benefit.</param>
  void round1( const DateMoyr& benDate )
    { if (benDate >= PiaParams::amend82) { theAmt = FLOOR(theAmt); } }
  /// <summary>Rounds stored benefit down to $5 multiple.</summary>
  ///
  /// <returns>The rounded benefit.</returns>
  ///
  /// <remarks>The version with 1 argument uses the stored amount to call the
  /// static version with 2 arguments. Use this function for Social Security
  /// Statements before 2000. For years 2000 and later, the rounding should be
  /// down to $1 multiple.</remarks>
  ///
  /// <param name="year">Year of benefit.</param>
  /// <param name="theAmount">Amount to be rounded.</param>
  int round5( int year ) const { return round5(theAmt, year); }
  /// <summary>Sets the benefit amount.</summary>
  ///
  /// <param name="newAmount">New benefit amount.</param>
  void set( double newAmount ) { theAmt = newAmount; }
  std::string toString() const;
  /// <summary>Removes cola percent from stored quantity.</summary>
  ///
  /// <remarks>Uses the stored amount to call the static version with 3
  /// arguments.</remarks>
  ///
  /// <param name="percent">Percentage decrease.</param>
  /// <param name="year">Year of decrease.</param>
  void unApplyCola( double percent, int year )
    { theAmt = unApplyCola(theAmt, percent, year); }
  /// <summary>Unrounds the stored PIA or MFB to the appropriate multiple of
  /// $0.10.</summary>
  ///
  /// <remarks>Uses the stored amount to call the static version with 2
  /// arguments.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_ROUND"/> if year is less than 1937 (only in debug
  /// mode).</exception>
  ///
  /// <param name="year">Year of benefit increase, or year prior to year of
  /// wage-indexed formula.</param>
  void unround( int year ) { theAmt = unround(theAmt, year); }
  /// <summary>Applies cola percent to specified benefit.</summary>
  ///
  /// <returns>The increased amount.</returns>
  ///
  /// <param name="percent">Percentage increase.</param>
  /// <param name="year">Year of increase.</param>
  /// <param name="theAmount">Amount to increase.</param>
  static double applyCola( double theAmount, double percent, int year )
    { theAmount *= (1. + percent / 100.); return round(theAmount, year); }
  static double round( double theAmount, int year );
  /// <summary>Rounds a specified PIA or MFB down to a whole dollar for June
  /// 1982 and later.</summary>
  ///
  /// <returns>The rounded amount.</returns>
  ///
  /// <param name="benDate">The date of benefit.</param>
  /// <param name="theAmount">Amount to be rounded.</param>
  static double round1( double theAmount, const DateMoyr& benDate )
    { if (benDate >= PiaParams::amend82) { return FLOOR(theAmount); }
      else { return theAmount; } }
  /// <summary>Rounds specified benefit down to $5 multiple.</summary>
  ///
  /// <returns>The rounded benefit.</returns>
  ///
  /// <remarks>Use this function for Social Security Statements before 2000.
  /// For years 2000 and later, the rounding should be down to $1 multiple.
  /// </remarks>
  ///
  /// <param name="year">Year of benefit.</param>
  /// <param name="theAmount">Amount to be rounded.</param>
  static int round5( double theAmount, int year )
    { if (year < amend2000year) {
        return 5 * static_cast<int>((theAmount + .01) / 5.); }
      else { return static_cast<int>(theAmount + .01); } }
  /// <summary>Removes cola percent from specified quantity.</summary>
  ///
  /// <returns>The decreased amount.</returns>
  ///
  /// <param name="percent">Percentage decrease.</param>
  /// <param name="year">Year of decrease.</param>
  /// <param name="theAmount">Amount to decrease.</param>
  static double unApplyCola( double theAmount, double percent, int year)
    { theAmount *= ( 100. / (100. + percent) );
      return unround(theAmount, year); }
  static double unround( double theAmount, int year );
};
