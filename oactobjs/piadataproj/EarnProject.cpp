// Functions for the <see cref="EarnProject"/> class to handle an array of
// earnings, with forward and backward projections and specified levels of
// earnings.

// $Id: EarnProject.cpp 1.29 2012/07/24 11:49:22EDT 044579 Development  $

#include "EarnProject.h"
#include "wbgenrl.h"
#include "awinc.h"
#include "WorkerDataGeneral.h"
#include "AssumptionType.h"

using namespace std;

/// <summary>Maximum year of projection.</summary>
int EarnProject::maxyear;

// <summary>Earnings selection titles.</summary>
const char *EarnProject::earnmal[NUM_TYPES] = {
  "entered earnings", "maximum earnings", "high earnings", "average earnings",
  "low earnings", "old-law maximum earnings", "childcare year"
};
// <summary>Lowest acceptable projection percentage.</summary>
const double EarnProject::PERC_LOW = 0.0;
// <summary>Highest acceptable projection percentage.</summary>
const double EarnProject::PERC_HIGH = 9.99;

/// <summary>Initializes this structure.</summary>
///
/// <param name="newMaxYear">Maximum projected year. Arrays start in 1937.
/// </param>
EarnProject::EarnProject( int newMaxYear ) :
earnpebs(YEAR37, newMaxYear), earntype(YEAR37, newMaxYear),
firstYear(YEAR37), lastYear(newMaxYear), projback(NO_PROJ),
percback(PERC_LOW), projfwrd(NO_PROJ), percfwrd(PERC_LOW)
{
  maxyear = newMaxYear;
}

/// <summary>Destroys this structure.</summary>
EarnProject::~EarnProject()
{ }

/// <summary>Initializes all variables to default values.</summary>
void EarnProject::deleteContents()
{
  percback = percfwrd = PERC_LOW;
  projback = projfwrd = NO_PROJ;
  earntype.fill(0);
  earnpebs.fill(0.0);
}

/// <summary>Sets some Statement data.</summary>
///
/// <remarks>There is no backward projection for a Statement. There is a
/// forward projection, at the average wage rate.</remarks>
///
/// <param name="year">First year of projection period (used to set last year
/// of earnings).</param>
/// <param name="ibegint">First year of earnings.</param>
void EarnProject::setPebsData( int year, int ibegint )
{
  // AssumptionType::EXPANDED_PEBES is constant, so ignore warning
#pragma warning(disable:4127)
  setPebsEarn(year);
  // only allow entered earnings historically, unless doing an expanded PEBES
  if (!AssumptionType::EXPANDED_PEBES) {
    firstYear = ibegint;
    projback = NO_PROJ;
    percback = PERC_LOW;
    projfwrd = AVGWAGE_PROJ;
    percfwrd = PERC_LOW;
    earntype.assign(ENTERED, YEAR37, lastYear);
  }
  else {
    // must have a forward projection even in expanded PEBES
    if (projfwrd == NO_PROJ) {
      projfwrd = AVGWAGE_PROJ;
    }
  }
#pragma warning(default:4127)
}

/// <summary>Sets last year of Statement earnings when computation year
/// changes, or after a Statement calculation has been done.</summary>
///
/// <param name="year">First year of projection period.</param>
void EarnProject::setPebsEarn( int year )
{
  const int iend2 = lastYear;  // last year of earnings in file
  lastYear = 1 + year;
  const double lastEarn = earnpebs[iend2];
  if (lastYear > iend2) {
    earnpebs.assign(lastEarn, iend2 + 1, lastYear);
  }
}

/// <summary>Returns true if earnings are projected in last two years of
/// historical period.</summary>
///
/// <returns>True if earnings are projected in last two years of historical
/// period.</returns>
///
/// <param name="year">First year of projection period.</param>
/// <param name="iendt">Last year of projected earnings.</param>
bool EarnProject::need_aw( int year, int iendt ) const
{
  // if faulty starting date for assumptions, return false
  if (year < YEAR79) {
    return(false);
  }
  const int earntype1 = earntype[year];
  const int earntype2 = earntype[year - 1];
  if (earntype1 == HIGH || earntype1 == AVERAGE || earntype1 == LOW ||
    earntype2 == HIGH || earntype2 == AVERAGE || earntype2 == LOW ||
    (projfwrd == 1 && iendt > year - 2) ||
    (projback == 1 && firstYear > year - 2)) {
    return true;
  }
  else {
    return false;
  }
}

/// <summary>Returns description of type of earnings.</summary>
///
/// <returns>Description of type of earnings.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_EARNMAL"/> if year is before
/// 1937 (only in debug mode).</exception>
///
/// <param name="year">Year for which type of earnings is desired.</param>
std::string EarnProject::toString( int year ) const
{
#ifndef NDEBUG
  if (year < YEAR37) {
    throw PiaException(PIA_IDS_EARNMAL);
  }
#endif
  return(string(earnmal[earntype[year]]));
}

/// <summary>Checks earnings.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_EARNPEBS"/> if earnings are out of range.</exception>
///
/// <param name="earnpebst">The earnings to check.</param>
void EarnProject::earnpebsCheck( double earnpebst )
{
  try {
    WageBaseGeneral::check(earnpebst);
  } catch (exception&) {
    throw PiaException(PIA_IDS_EARNPEBS);
  }
}

/// <summary>Projects OASDI-covered earnings for the specified worker.
/// </summary>
///
/// <returns>True if any earnings go over the maximum allowed,
///  false otherwise.</returns>
///
/// <remarks>First sets all the earnings in the
/// <see cref="WorkerDataGeneral"/> for the period within the non-projected
/// period. Then projects backwards, if the trigger is not
/// <see cref="earn_proj_type::NO_PROJ"/>. The projection goes backwards from
/// the beginning of the non-projected period, using annual factors that
/// depend on the type of projection. Then projects forwards, in a similar
/// fashion.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_EARNPROJ"/> if projection year is out of range (only
/// in debug mode).</exception>
///
/// <param name="workerData">Worker basic data.</param>
/// <param name="fqinc">Average wage increases.</param>
bool EarnProject::project( WorkerDataGeneral& workerData,
const Awinc& fqinc ) const
{
  bool overMax = false;  // indicator for earnings over the maximum allowed
  workerData.zeroEarnOasdi(firstYear, lastYear);
  for (int yr1 = firstYear; yr1 <= lastYear; yr1++) {
    workerData.setEarnOasdi(yr1, earnpebs[yr1]);
  }
  if (getProjback() != NO_PROJ) {
    for (int yr1 = firstYear - 1; yr1 >= workerData.getIbegin(); yr1--) {
      const double basePerc = (getProjback() == CONSTANT_PROJ) ?
        0.0 : fqinc[yr1 + 1];
      const double factor = 1.0 + (basePerc + getPercback()) / 100.0;
      const double amount = AverageWage::projectBackwardStatic(
        workerData.getEarnOasdi(yr1 + 1), factor);
      if (amount > MAXEARN) {
        workerData.setEarnOasdi(yr1, MAXEARN);
        overMax = true;
      }
      else {
        workerData.setEarnOasdi(yr1, amount);
      }
    }
  }
  if (getProjfwrd() != NO_PROJ) {
    for (int yr1 = lastYear + 1; yr1 <= workerData.getIend(); yr1++) {
      const double basePerc = (getProjfwrd() == CONSTANT_PROJ) ?
        0.0 : fqinc[yr1];
      const double factor = 1.0 + (basePerc + getPercfwrd()) / 100.0;
      const double amount = AverageWage::projectForwardStatic(
        workerData.getEarnOasdi(yr1 - 1), factor);
      if (amount > MAXEARN) {
        workerData.setEarnOasdi(yr1, MAXEARN);
#if !defined(NDEBUG)
        if (lastYear < YEAR37)
          throw PiaException(PIA_IDS_EARNPROJ);
#endif
        if (earntype[lastYear] != MAXIMUM) {
          overMax = true;
        }
      }
      else {
        workerData.setEarnOasdi(yr1, amount);
      }
    }
  }
  return overMax;
}
