// Declarations for the <see cref="LawChangeNRACHANGE"/> class to manage
// parameters for a proposal to change the full retirement age.

// $Id: LawChangeNRACHANGE.h 1.7 2011/08/01 10:06:07EDT 044579 Development  $

#pragma once

#include "LawChange.h"
#include "age.h"

/// <summary>Parameters for a proposal to change the full retirement age.
/// </summary>
///
/// <remarks>If the indicator is greater than zero, it means the following:
/// <table>
///   1   keep retirement age at 65 (eliminate the present-law increase to 67).
///   2   eliminate plateau between 66 and 67, then remain at 67.
///   3   eliminate plateau between 66 and 67, then index to life expectancy (increase by 1 month every 2 years).
/// </table>
/// The actuarial reduction for years 6 and 7 is 3/8 percent per month. Beyond
/// that, it is 1/3 percent per month.</remarks>
class LawChangeNRACHANGE : public LawChange
{
public:
  /// <summary>The age at which the actuarial reduction factor changes.
  /// </summary>
  static const Age ageArChange;
  /// <summary>Monthly actuarial reduction factor between ages 67 and 69.
  /// </summary>
  static const double arMonthlyFactor67_69;
  /// <summary>Monthly actuarial reduction factor over age 69.</summary>
  static const double arMonthlyFactor69Plus;
public:
  LawChangeNRACHANGE();
  void prepareStrings( std::vector< std::string >& outputString );
};
