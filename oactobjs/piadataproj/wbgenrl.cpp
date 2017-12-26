// Functions for the <see cref="WageBaseGeneral"/> class - parent of classes
// that manage wage base projections.

// $Id: wbgenrl.cpp 1.30 2017/08/29 09:01:24EDT 277133 Development  $

#include <algorithm>
#include <cmath>  // for floor
#include "wbgenrl.h"
#include "Resource.h"
#include "oactcnst.h"
#include "PiaException.h"

using namespace std;

// <summary>Increase in average wage due to deferred comp in 1990.</summary>
const double WageBaseGeneral::defcomp = 0.0149249;

/// <summary>The data array starts in 1937 and goes to the specified last
/// year.</summary>
///
/// <remarks>The type of wage base has the following meanings:
/// <table>
///     0      present law OASDI.
///     1      prior law (ignore 1990-92 ad hoc increases).
///     2      old law (ignore 1979-81 ad hoc increases).
///     3      HI.
/// </table></remarks>
///
/// <param name="newWageBaseInd">Type of wage base.</param>
/// <param name="newLastYear">Last year of projection.</param>
WageBaseGeneral::WageBaseGeneral( int newWageBaseInd, int newLastYear ) :
DoubleAnnual(YEAR37, newLastYear), wageBaseInd(newWageBaseInd)
{ }

/// <summary>The data array starts and ends in the specified years.</summary>
///
/// <remarks>The type of wage base has the following meanings:
/// <table>
///     0      present law OASDI.
///     1      prior law (ignore 1990-92 ad hoc increases).
///     2      old law (ignore 1979-81 ad hoc increases).
///     3      HI.
/// </table></remarks>
///
/// <param name="newWageBaseInd">Type of wage base.</param>
/// <param name="newLastYear">Last year of projection.</param>
/// <param name="newBaseYear">First year of data.</param>
WageBaseGeneral::WageBaseGeneral( int newWageBaseInd, int newBaseYear,
int newLastYear ) : DoubleAnnual(newBaseYear, newLastYear),
wageBaseInd(newWageBaseInd)
{ }

/// <summary>Destructor.</summary>
WageBaseGeneral::~WageBaseGeneral()
{ }

/// <summary>Projects wage bases.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BASE1"/> if first year is out of range (only in debug
/// mode).</exception>
///
/// <param name="averageWages">Average wages for wage base projection.</param>
/// <param name="cpi">Benefit increases.</param>
/// <param name="firstYeart">First year of projection.</param>
/// <param name="lastYeart">Last year of projection.</param>
void WageBaseGeneral::project( const DoubleAnnual& averageWages,
const DoubleAnnual& cpi, int firstYeart, int lastYeart )
{
  int iflag; // number of bases considered since last increase
  double factor;  // factor to apply to previous base
#ifndef NDEBUG
  if (firstYeart < 1975)
    throw PiaException(PIA_IDS_BASE1);
#endif
  for (int yr = firstYeart; yr <= lastYeart; yr++) {
    const int index = yr - getBaseYear();
    iflag = 0;
    while (static_cast<double>(cpi[yr + iflag - 1]) < 0.1) {
      // if no benefit increase, wage base equals last previously set base
      theData[index + iflag] = theData[index - 1];
      iflag++;
      if (yr + iflag > getLastYear())
        return;
    }
    int i3 = yr + iflag;
    // set intermediate, unrounded wage base
    double baseun = theData[index - 1];
    // apply factor prior to 1995, project from average wage thereafter
    if (i3 < 1995) {
      for (int i2 = 0; i2 <= iflag; i2++) {
        if (wageBaseInd != 1 && yr + i2 > 1989 && yr + i2 < 1993) {
          if (yr + i2 == 1990) {
            factor = averageWages[yr+i2-2] / averageWages[yr+i2-3] + 0.02;
          }
          else if (yr + i2 == 1991) {
            factor = (averageWages[yr+i2-2] + 0.02 * averageWages[yr+i2-3]) /
              (averageWages[yr+i2-3] + 0.02 * averageWages[yr+i2-4]);
          }
          else {
            factor = averageWages[yr+i2-2] * (1.0 + defcomp) /
              (averageWages[yr+i2-3] + 0.02 * averageWages[yr+i2-4]);
          }
        }
        else {
          factor = averageWages[yr+i2-2] / averageWages[yr+i2-3];
        }
        baseun = (baseun + 0.001) * factor;
      }
    }
    else {
      factor = averageWages[i3 - 2] / averageWages[1992];
      if (wageBaseInd == 2) {
        baseun = 45000.0 * factor;
      }
      else {
        baseun = (wageBaseInd < 2) ? 60600.0 * factor : 99999999.0;
      }
    }
    // handle ad hoc increases 1979-81
    if (wageBaseInd != 2 && i3 >= YEAR79 && i3 < 1982) {
      if (i3 == YEAR79)
        theData[index + iflag] = 22900.0;
      else if (i3 == 1980)
        theData[index + iflag] = 25900.0;
      else
        theData[index + iflag] = 29700.0;
    }
    else {
      theData[index + iflag] = 300.0 * floor(baseun / 300.0 + 0.5);
    }
    // do not allow a decrease in the base
    if (theData[index + iflag] < theData[index + iflag - 1]) {
      theData[index + iflag] = theData[index + iflag - 1];
    }
    yr = i3;
  } // end for yr
}

/// <summary>Checks a wage base.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BASE2"/> if wage base is out of range.</exception>
///
/// <param name="baset">Value to check.</param>
void WageBaseGeneral::check( double baset )
{
  if (baset > MAXEARN || baset < 0.0)
    throw PiaException(PIA_IDS_BASE2);
}

/// <summary>Limits earnings to wage base.</summary>
///
/// <param name="earnings1">Array of limited earnings.</param>
/// <param name="earnings2">Array of unlimited earnings.</param>
void WageBaseGeneral::earnLimit( DoubleAnnual& earnings1,
const DoubleAnnual& earnings2 ) const
{
  const int yr1 = min(earnings1.getLastYear(), earnings2.getLastYear());
  const int yr2 = max(earnings1.getBaseYear(), earnings2.getBaseYear());
  for (int yr3 = yr2; yr3 <= yr1; yr3++) {
    earnings1[yr3] = min(earnings2[yr3], theData[yr3 - getBaseYear()]);
  }
}

/// <summary>Projects wage bases after a period of ad hoc wage bases.
/// </summary>
///
/// <param name="averageWages">Average wages for wage base projection.</param>
/// <param name="cpi">Benefit increases.</param>
/// <param name="lastYearLC"> Last year of ad hoc wage bases.</param>
void WageBaseGeneral::projectLC( const DoubleAnnual& averageWages,
const DoubleAnnual& cpi, int lastYearLC )
{
  for (int yr = lastYearLC + 1; yr <= getLastYear(); yr++) {
    const int index = yr - getBaseYear();
    if (cpi[yr - 1] < 0.1) {
      // if no benefit increase, wage base equals last previously set base
      theData[index] = theData[index - 1];
      continue;
    }
    // project by average wage after ad hoc increases
    const double factor =
      averageWages[yr - 2] / averageWages[lastYearLC - 2];
    double baseun = theData[lastYearLC - getBaseYear()] * factor;
    // do not allow a decrease in the base
    baseun = max(300.0 * floor(baseun / 300.0 + 0.5), theData[index - 1]);
    theData[index] = baseun;
  }
}
