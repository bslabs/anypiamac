// Functions for the <see cref="BendPoints"/> class to handle PIA and MFB
// formula bend points.

// $Id: BendPoints.cpp 1.5 2011/07/28 15:19:55EDT 044579 Development  $

#include <cmath>  // for floor
#include "BendPoints.h"
#include "avgwg.h"

using namespace std;

const double BendPoints::MFBBP1979[] = { 0.0, 230.0, 332.0, 433.0 };

const double BendPoints::PIABP1979[] = { 0.0, 180.0, 1085.0 };

const double BendPoints::MFBPERC[] = { 1.5, 2.72, 1.34, 1.75 };

const double BendPoints::PIAPERC[] = { 0.90, 0.32, 0.15 };

const double BendPoints::WINDFALL_PERC = 0.40;

/// <summary>Projects bend points, using 1979 bend points as a base.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> if average wages
/// are not initialized (only in debug mode).</exception>
///
/// <param name="bendMfb">The array of bend points to be set.</param>
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="fq">Average wage series.</param>
void BendPoints::projectMfb( BendMfb& bendMfb, int eligYear,
const AverageWage& fq )
{
#if !defined(NDEBUG)
  if (fq[1977] == 0.0)
    throw PiaException("fq is not initialized in BendPoints::project");
#endif
  const double temp = fq[eligYear - 2] / fq[1977];
  bendMfb[1] = floor(MFBBP1979[1] * temp + 0.5);
  bendMfb[2] = floor(MFBBP1979[2] * temp + 0.5);
  bendMfb[3] = floor(MFBBP1979[3] * temp + 0.5);
}

/// <summary>Projects bend points, using 1979 bend points as a base.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> if average wages
/// are not initialized (only in debug mode).</exception>
///
/// <param name="bendPia">The array of bend points to be set.</param>
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="fq">Average wage series.</param>
void BendPoints::projectPia( BendPia& bendPia, int eligYear,
const AverageWage& fq )
{
#if !defined(NDEBUG)
  if (fq[1977] == 0.0)
    throw PiaException("fq is not initialized in BendPia::project");
#endif
  const double temp = fq[eligYear - 2] / fq[1977];
  bendPia[1] = floor(PIABP1979[1] * temp + 0.5);
  bendPia[2] = floor(PIABP1979[2] * temp + 0.5);
}

/// <summary>Resets MFB formula percentages to original values.</summary>
///
/// <param name="percMfb">The array of formula percentages to be set.</param>
void BendPoints::resetMfbPerc( PercMfb& percMfb )
{
  copy(MFBPERC, MFBPERC + 4, percMfb.begin());
}

/// <summary>Resets percentages to original values.</summary>
///
/// <param name="percPia">The array of formula percentages to be set.</param>
void BendPoints::resetPiaPerc( PercPia& percPia )
{
  copy(PIAPERC, PIAPERC + 3, percPia.begin());
}

/// <summary>Sets first PIA formula percentage for windfall
///   elimination provision.</summary>
///
/// <param name="percPia">The array of formula percentages to be set.</param>
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="benefitDate">Year of benefit.</param>
/// <param name="specMinTot">Years of coverage.</param>
void BendPoints::setWindfallPerc( PercPia& percPia, int eligYear,
int benefitDate, int specMinTot )
{
  if (eligYear < 1985 || specMinTot >= WINDFALL_YEARS) {
    percPia[0] = PIAPERC[0];
    return;
  }
  // set a temporary percentage
  double rv = (eligYear > 1989) ? WINDFALL_PERC :
    PIAPERC[0] - 0.10 * static_cast<double>(eligYear - 1985);
  // check for years of coverage guarantee (changes beginning with
  // benefits paid in 1989)
  const double annualPct = (benefitDate >= 1989) ? 0.05 : 0.10;
  const double totalPct =
    annualPct * static_cast<double>(WINDFALL_YEARS - specMinTot);
  if (PIAPERC[0] - totalPct > rv)
    rv = PIAPERC[0] - totalPct;
  percPia[0] = rv;
}
