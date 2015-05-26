// Functions for the <see cref="BenefitAmount"/> class to handle Social
// Security benefit amount.
//
// $Id: BenefitAmount.cpp 1.25 2011/07/28 15:19:55EDT 044579 Development  $

#include <iomanip>
#include <sstream>
#include "BenefitAmount.h"
#include "Resource.h"
#include "oactcnst.h"
#include "PiaException.h"

using namespace std;

/// <summary>Rounds a specified PIA or MFB to the appropriate multiple of
/// $0.10.</summary>
///
/// <remarks>Round a cola calculation up for June 1981 and earlier colas, down
/// for June 1982 and later colas. Round an aime pia calculation up for 1982
/// and earlier eligibility, down for 1983 and later eligibility.</remarks>
///
/// <returns>The rounded amount.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_ROUND"/> if year is less than 1937 (only in debug mode).
/// </exception>
///
/// <param name="year">Year of benefit increase, or year prior to year of
/// wage-indexed formula.</param>
/// <param name="theAmount">Amount to be rounded.</param>
double BenefitAmount::round( double theAmount, int year )
{
#ifndef NDEBUG
  if (year < YEAR37)
    throw PiaException(PIA_IDS_ROUND);
#endif
  // round down to lower dime for June 1982 and later increases
  if (year >= amend82year) {
    return floor(10.0 * theAmount + 0.0005) / 10.0;
  }
  // for rounding-up to dime, use half-cent rule for 1972 and earlier
  // increases; for 1973-81 benefit increases, round up to dime in any case
  // not already an exact multiple of $.10. (q is fraction of $.01.)
  const double q = (year >= amend73year) ? 0.009 : 0.499;
  // number of cents that PIA exceeds a multiple of a dime
  const double x100 = fmod(theAmount * 100.0, 10.0);
  if (x100 < q) {
  // if within tolerance of q, do not round up
    theAmount -= x100 / 100.0;
  }
  // otherwise round up to dime
  else {
    theAmount += 0.10 - x100 / 100.0;
  }
  return theAmount;
}

/// <summary>Unrounds a specified PIA or MFB to the appropriate multiple of
/// $0.10.</summary>
///
/// <returns>The unrounded amount.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_ROUND"/> if year is less than 1937 (only in debug mode).
/// </exception>
///
/// <param name="year">Year of benefit increase, or year prior to year of
/// wage-indexed formula.</param>
/// <param name="theAmount">Amount to be unrounded.</param>
double BenefitAmount::unround( double theAmount, int year )
{
#ifndef NDEBUG
  if (year < YEAR37)
    throw PiaException(PIA_IDS_ROUND);
#endif
  // round up to higher dime for June 1982 and later increases
  if (year > amend82year) {
    return floor(10.0 * theAmount + 0.999) / 10.0;
  }
  // for rounding-down to dime, use half-cent rule for 1972 and earlier
  // increases; for 1973-81 benefit increases, round down to dime in any case
  // not already an exact multiple of $.10. (q is fraction of $.01.)
  const double q = (year >= amend73year) ? 9.99 : 9.501;
  // number of cents that PIA is less than a multiple of a dime
  const double x100 = 10.0 - fmod(theAmount * 100.0, 10.0);
  if (x100 > q) {
  // if within tolerance of q, do not round down
    theAmount += x100 / 100.0;
  }
  // otherwise round down to dime
  else {
    theAmount += x100 / 100.0 - 0.10;
  }
  return theAmount;
}

/// <summary>Returns benefit amount as a string with 2 decimal places.
/// </summary>
///
/// <returns>Benefit amount as a string with 2 decimal places.</returns>
std::string BenefitAmount::toString() const
{
  ostringstream strm;
  strm.setf(ios::fixed,ios::floatfield);
  strm << setprecision(2) << theAmt;
  return strm.str();
}
