// Functions for the <see cref="OldPia"/> class - parent of all methods
// using pia tables in the calculation of a Social Security benefit.

// $Id: oldpia.cpp 1.36 2011/08/01 11:38:35EDT 044579 Development  $

#include <cmath>  // for floor
#include <utility>  // for rel_ops
#include "OldPia.h"
#include "date.h"
#include "piaparms.h"

using namespace std;
#if !defined(__SGI_STL_INTERNAL_RELOPS)
using namespace std::rel_ops;
#endif

/// <summary>Initializes this instance.</summary>
///
/// <param name="newWorkerData">Worker data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
/// <param name="newTitle">Title of this method.</param>
/// <param name="newMethod">Number of this method.</param>
OldPia::OldPia( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear,
const std::string& newTitle, pia_type newMethod ) :
PiaMethod(newWorkerData, newPiaData, newPiaParams, newMaxyear, newTitle,
newMethod), iamemax(0)
{ }

/// <summary>Destructor.</summary>
OldPia::~OldPia()
{ }

/// <summary>Selects correct PIA table.</summary>
///
/// <returns>Number of correct PIA table.</returns>
PiaMethod::table_type OldPia::oldPiaCal()
{
  if (workerData.getBenefitDate() < PiaParams::amend52)
    return(NO_TABLE);
  const int intAme = static_cast<int>(getAme());
  if (workerData.getBenefitDate() < PiaParams::amend54)
    return(pl1952(intAme));
  if (workerData.getBenefitDate() < PiaParams::amend58)
    return(pl1954(intAme));
  if (workerData.getBenefitDate() < PiaParams::amend651)
    return(pl1958(intAme));
  if (workerData.getBenefitDate() < PiaParams::amend672)
    return(pl1965(intAme));
  if (workerData.getBenefitDate() < PiaParams::amend69)
    return(pl1967(intAme));
  if (workerData.getBenefitDate() < PiaParams::amend70)
    return(pl1969(intAme));
  if (workerData.getBenefitDate() < PiaParams::amend721)
    return(pl1971(intAme));
  if (workerData.getBenefitDate() < PiaParams::amend742)
    return(pl1972(intAme));
  return(NO_TABLE);
}

/// <summary>Applies CPI and wage base increases to 1973 Act PIA Table.
/// </summary>
///
/// <returns>Number of correct PIA table.</returns>
///
/// <param name="benefitDate">The date for which PIA table is desired.</param>
/// <param name="freeze">True for transitional guarantee or 1977 old-start
/// calculation with 1979 or later eligibility, false otherwise.</param>
/// <param name="amesub">AME in this function.</param>
/// <param name="saveInfo">True to save PIA and MFB when doing CPI
/// adjustments.</param>
PiaMethod::table_type OldPia::cpiBase( const DateMoyr& benefitDate,
 bool freeze, double amesub, bool saveInfo )
{
  int i21;  // year benefit increases are first applied
  int i24 = 0;  // year that AME is first included in PIA table as of
    // January, if AME is greater than $1100 (otherwise 0)
  const int eligYear = piaData.getEligYear();
  int i1 = benefitDate.getYear();
  const int lastYear = (static_cast<int>(benefitDate.getMonth()) <
    piaParams.getMonthBeninc(i1)) ? i1 - 1 : i1;
  i1 = static_cast<int>(amesub + 0.1);
  if (i1 > 1100) {
    for (int i2 = 1975; i2 <= lastYear; i2++) {
      if (i1 <= piaParams.lastAmw(i2)) {
        i24 = i2;
        break;
      }
    }
    // set i24 if not already set
    if (i24 == 0)
      i24 = lastYear;
    // find last PIA in June 1974 PIA Table
    pl1973(1100);
    if (saveInfo) {
      piaElig[yearCpi[FIRST_YEAR] - 1] = piasub;
      mfbElig[yearCpi[FIRST_YEAR] - 1] = mfbsub;
    }
    if (i24 > 1975) {
      if (saveInfo) {
        yearCpi[YEAR_TABLE] = i24;
      }
      // extend table and apply increases up to year prior to when
      // AME is first included
      for (int i2 = 1975; i2 < i24; i2++) {
        // extension amount depends on two successive wage bases
        piasub += 0.2 * static_cast<double>(piaParams.lastAmw(i2) -
          piaParams.lastAmw(i2 - 1));
        mfbsub =
          BenefitAmount::round(PiaParams::factor175 * piasub, i2 - 1);
        // apply special benefit increase for 1999 if benefit is in August
        // 2001 or later
        if (piaParams.isApplicableCola99(i2, benefitDate)) {
          piasub = piaParams.applyCola99(piasub);
          mfbsub = piaParams.applyColaMfb99(mfbsub, piasub);
        }
        // otherwise apply regular benefit increase
        else {
          piasub = piaParams.applyCola(piasub, i2, eligYear);
          mfbsub = piaParams.applyColaMfb(mfbsub, i2, eligYear, piasub);
        }
        // after applying June 1978 increase, set transitional guarantee
        // or 1977 old-start (1979 or later eligibility) PIA at eligibility
        if (i2 == 1978 && freeze) {
          piaElig[yearCpi[FIRST_YEAR]] = piasub;
        }
        if (saveInfo) {
          piaElig[i2] = piasub;
          mfbElig[i2] = mfbsub;
        }
      }
    }
    // apply extension in year AME is first included in table
    piasub += static_cast<double>((i1 - piaParams.lastAmw(i24 - 1) + 4) / 5);
    mfbsub = BenefitAmount::round(PiaParams::factor175 * piasub, i24);
    // set year benefit increases are first applied
    i21 = i24;
  } // end if i1 > 1000
  else {
    // use downward-extended PIA table if transitional guarantee or
    // 1977 old-start in 1982 or later
    if (eligYear < 1982 || !freeze || i1 > 75) {
      pl1973(i1);
      if (saveInfo) {
        piaElig[yearCpi[FIRST_YEAR] - 1] = piasub;
        mfbElig[yearCpi[FIRST_YEAR] - 1] = mfbsub;
      }
      piaElig[yearCpi[FIRST_YEAR]] = piasub;
      mfbElig[yearCpi[FIRST_YEAR]] = mfbsub;
      // set year benefit increases are first applied
      i21 = 1975;
    }
    else {
      // calculate downward-extended PIA table values as of Dec 1978
      piasub = BenefitAmount::round(static_cast<double>(i1) * 121.8 / 76.0,
        1978);
      piaElig[yearCpi[FIRST_YEAR]] = piasub;
      mfbElig[yearCpi[FIRST_YEAR]] =
        BenefitAmount::round(PiaParams::factor150 * piasub, 1978);
      // do not apply any benefit increases to extended minimum
      return(PL_1973);
    }
  }
  // apply benefit increases to PIA table
  for (int i2 = i21; i2 <= lastYear; i2++) {
    // apply special benefit increase for 1999 if benefit is in August 2001
    // or later
    if (piaParams.isApplicableCola99(i2, benefitDate)) {
      piasub = piaParams.applyCola99(piasub);
      mfbsub = piaParams.applyColaMfb99(mfbsub, piasub);
    }
    // otherwise apply regular benefit increase
    else {
      piasub = piaParams.applyCola(piasub, i2, eligYear);
      mfbsub = piaParams.applyColaMfb(mfbsub, i2, eligYear, piasub);
    }
    // after applying June 1978 increase, set transitional guarantee or 1977
    // old-start (1979 or later eligibility) PIA at eligibility
    if (i2 == 1978 && freeze) {
      piaElig[yearCpi[FIRST_YEAR]] = piasub;
    }
    if (saveInfo) {
      piaElig[i2] = piasub;
      mfbElig[i2] = mfbsub;
    }
  } // end for
  return(PL_1973);
}

/// <summary>Finds AME from PIA in PIA table.</summary>
///
/// <returns>AME corresponding to the PIA.</returns>
///
/// <param name="belowMin">False to not adjust for PIA below minimum,
/// true to adjust.</param>
double OldPia::mfbOldCal( bool belowMin )
{
  // start with lowest applicable AME
  int rv = 75;
  do {
    rv++;
    // test to see which PIA table to use
    if (workerData.getBenefitDate() < PiaParams::amend742)
      static_cast<void>(pl1972(rv));
    else {
      // apply benefit increases if June 1975 or later
      setTableNum(cpiBase(workerData.getBenefitDate(), false,
        static_cast<double>(rv), false));
    }
  } while (piaEnt.get() > piasub && rv < 1000);
  if (belowMin && rv == 76) {
    mfbEnt.set(BenefitAmount::round(PiaParams::factor150 * piaEnt.get(),
      yearCpi[YEAR_BEN]));
  }
  else {
    mfbEnt.set(mfbsub);
  }
  return static_cast<double>(rv);
}

/// <summary>Calculates PIAs under 1973 Act.</summary>
///
/// <param name="amesub">AME in this function.</param>
void OldPia::pl1973( int amesub )
{
  // see if AME is in extension of table above $1000
  if (amesub < 1001) {
    // if under $1000 AME, first get 1972 Act value
    static_cast<void>(pl1972(amesub));
    // increase by 11%
    piasub = piaParams.applyCola(piasub, 1974);
    mfbsub = piaParams.applyColaMfb(mfbsub, 1974, piasub);
  }
  else {
    // if AME over $1000, extend beyond $1000 in $5 intervals
    pl1973ext(amesub);
  }
}

/// <summary>Calculates PIAs under 1973 Act, extension to table.</summary>
///
/// <param name="amesub">AME in this function.</param>
void OldPia::pl1973ext( int amesub )
{
  piasub = static_cast<double>((amesub + 4) / 5) + 249.0;
  mfbsub = BenefitAmount::round(PiaParams::factor175 * piasub, 1974);
}

/// <summary>Calculates PIAs under 1972 Act, effective Sept 1972.</summary>
///
/// <returns>Number of correct PIA table.</returns>
///
/// <param name="amesub">AME in this function.</param>
PiaMethod::table_type OldPia::pl1972( int amesub )
{
  // see if AME is in extension of table above $750
  if (amesub < 751) {
    // if under $750 AME, first get 1971 Act value
    static_cast<void>(pl1971(amesub));
    // increase by 20%
    piasub = piaParams.applyCola(piasub, 1972);
    mfbsub = piaParams.applyColaMfb(mfbsub, 1972, piasub);
  }
  else {
    // if AME over $750, extent beyond $750 in $5 intervals
    piasub = static_cast<double>((amesub + 4) / 5) + 204.5;
    // MFB is 175% of PIA in extension
    mfbsub = BenefitAmount::round(PiaParams::factor175 * piasub, 1972);
  }
  // check to see if temporary 7% increase applies (Mar-May 1974)
  if (workerData.getBenefitDate() >= PiaParams::amend741 &&
    workerData.getBenefitDate() < PiaParams::amend742) {
    piasub = BenefitAmount::applyCola(piasub, piaParams.beninc74, 1974);
    mfbsub = BenefitAmount::applyCola(mfbsub, piaParams.beninc74, 1974);
  }
  return(PL_1972);
}

/// <summary>Calculates PIAs under 1971 Act.</summary>
///
/// <returns>Number of correct PIA table.</returns>
///
/// <param name="amesub">AME in this function.</param>
PiaMethod::table_type OldPia::pl1971( int amesub )
{
  if (amesub < 652) {
    // if under $652, first get 1969 Act value
    static_cast<void>(pl1969(amesub));
    // increase by 10%
    piasub = piaParams.applyCola(piasub, 1971);
  }
  else {
    // extend beyond $651, using 20% extension starting at $657
    if (amesub > 656)
      piasub = static_cast<double>((amesub + 4) / 5) + 145.4;
    // from $652 to $656, use ad hoc values smoothing to 20% extension
    if (amesub < 657 && amesub > 652)
      piasub = 276.6;
    if (amesub == 652)
      piasub = 275.8;
  }
  // calculate MFB
  if (amesub < 628) {
    // for AME up to $436, MFB is 88% of AME
    if (amesub < 437)
      mfbsub = .88 * static_cast<double>(iamemax);
    // for AME from $437 to $627, MFB increases at 44% of AME
    if (amesub > 436)
      mfbsub = 383.68 + 0.44 * static_cast<double>(iamemax - 436);
    mfbsub = BenefitAmount::round(mfbsub, 1971);
    const double mfb71 =
      BenefitAmount::round(PiaParams::factor150 * piasub, 1971);
    if (amesub < 240 || mfbsub < mfb71)
      mfbsub = mfb71;
  }
  else {
    mfbsub = BenefitAmount::round(PiaParams::factor175 * piasub, 1971);
 }
  return(PL_1971);
}

/// <summary>Calculates PIAs under 1969 Act.</summary>
///
/// <returns>Number of correct PIA table.</returns>
///
/// <param name="amesub">AME in this function.</param>
PiaMethod::table_type OldPia::pl1969( int amesub )
{
  // there is no extension of table, so get 1967 Act value
  static_cast<void>(pl1967(amesub));
  // increase by 15%
  piasub = piaParams.applyCola(piasub, 1970);
  // minimum PIA of $64
  piasub = max(64.0, piasub);
  // MFB does not change from 1967 Act for AME over $239
  if (static_cast<double>(amesub) < 239.5) {
    // below $240, MFB is 150 percent of PIA
    mfbsub = BenefitAmount::round(PiaParams::factor150 * piasub, 1970);
  }
  return(PL_1969);
}

/// <summary>Calculates PIAs under 1967 Act.</summary>
///
/// <returns>Number of correct PIA table.</returns>
///
/// <param name="amesub">AME in this function.</param>
PiaMethod::table_type OldPia::pl1967( int amesub )
{
  double pia67;  // test PIA in MFB calculation
  // test to see if AME is in extension of table above $550 (treat $551 to
  // $553 as being in table before extension, since correct value results)
  iamemax = amesub;
  if (iamemax < 554) {
    // if under $554, first get 1965 Act value
    static_cast<void>(pl1965(amesub));
    // increase by 13%
    piasub = piaParams.applyCola(piasub, 1968);
    // minimum PIA of $55
    piasub = max(55.0, piasub);
  }
  else {
    // extend table beyond $553 at 28.43%
    piasub = floor(189.598 + .2843 * (amesub - 550) + 0.5);
    do {
      // find the highest AME giving that same PIA
      pia67 = floor(189.598 + .2843 * static_cast<double>(++iamemax - 550) +
        0.5);
    } while (fabs(pia67 - piasub) < 0.1);
    iamemax--;
  }
  // find MFB for AME up to $370
  if (amesub < 371) {
    // for AME from $179 to $370, MFB is same as in 1965 Act
    if (amesub < 179) {
      mfbsub = BenefitAmount::round(PiaParams::factor150 * piasub, 1968);
    }
    return(PL_1967);
  }
  // for AME above $436, MFB is $348.80 plus 40% of AME above $436
  if (amesub > 436) {
    // MFB is maximum of $434.40
    mfbsub = min(434.4, 348.8 + 0.4 * static_cast<double>(iamemax - 436));
  }
  else
    // for AME from $371 to $436, MFB is 80% of AME
    mfbsub = 0.8 * static_cast<double>(iamemax);
  return(PL_1967);
}

/// <summary>Calculates PIAs under 1965 Act.</summary>
///
/// <returns>Number of correct PIA table.</returns>
///
/// <param name="amesub">AME in this function.</param>
PiaMethod::table_type OldPia::pl1965( int amesub )
{
  // start with 1958 Act, even for values above the original maximum
  // AMW of $400 in the 1958 Act
  static_cast<void>(pl1958(amesub));
  // for AME up to $94, increase PIA by $4
  if (amesub < 95) {
    piasub += 4.0;
    // minimum PIA of $44
    piasub = max(44.0, piasub);
    // MFB of 150% of PIA
    mfbsub = PiaParams::factor150 * piasub;
    return(PL_1965);
  }
  // for AME from $95 to $403, increase PIA by 7% (include $401 to $403
  // as if from 1958 Act)
  if (amesub < 404) {
    piasub = piaParams.applyCola(piasub, 1965);
  }
  // for AME above $403, increase PIA by $9 to match increase at $403
  // (7% of $127.00 is $8.89), rounded to a dollar
  else
    piasub += 9.0;
  // find MFB for AME up to $314
  if (amesub < 315) {
    // for AME from $142 to $314, MFB is same as in 1958 Act
    if (amesub < 142) {
      mfbsub = BenefitAmount::round(PiaParams::factor150 * piasub, 1965);
    }
    return(PL_1965);
  }
  // for AME above $370, MFB is $296 plus 40% of AME above $370
  if (amesub > 370) {
    // MFB is maximum of $368.00
    mfbsub = min(368.0, 296.0 + 0.4 * static_cast<double>(iamemax - 370));
  }
  else
    // for AME from $315 to $370, MFB is 80% of AME
    mfbsub = 0.8 * static_cast<double>(iamemax);
  return(PL_1965);
}

/// <summary>Calculates PIAs under 1958 Act.</summary>
///
/// <returns>Number of correct PIA table.</returns>
///
/// <param name="amesub">AME in this function.</param>
PiaMethod::table_type OldPia::pl1958( int amesub )
{
  double pia58;  // test PIA in MFB calculation
  if (amesub <= 84)
    // Up to $84, 1958 Act increased PIAs by $3, rounded up to even dollar
    piasub = 3.49 + 0.55 * static_cast<double>(amesub);
  else {
    // Over $84, 1958 Act increased PIAs by 7% (.5885 equals 1.07 times
    // the .55 in 1954 Act
    double round58 = (amesub > 110) ? 110.0 : static_cast<double>(amesub);
    piasub = 0.5885 * round58;
    round58 = max(static_cast<double>(amesub - 110), 0.0);
    piasub += 0.214 * round58;
  }
  // round to whole dollar, with minimum PIA of $33
  piasub = max(33.0, floor(piasub + 0.5));
  // PIA for AME of $553 was made ad hoc in 1967 Act
  if (amesub == 553)
    piasub = 159.0;
  // minimum PIA increased as of August 1961 to $40
  if (workerData.getBenefitDate() >= PiaParams::amend61 && piasub < 40.0)
    piasub = 40.0;
  // calculate maximum family benefit
  iamemax = amesub;
  if (iamemax > 127) {
    do {
      // find the highest AME giving that same PIA
      pia58 = floor(41.195 + 0.214 * static_cast<double>(++iamemax) + 0.5);
    } while (fabs(pia58 - piasub) < 0.1);
    if (iamemax != 553)
      iamemax--;
    mfbsub = min(254.0, 0.8 * static_cast<double>(iamemax));
  }
  else {
    // for AME up to $127, MFB is 150% of PIA, but at least a $20 increase
    mfbsub = max(PiaParams::factor150 * piasub, piasub + 20.0);
  }
  return(PL_1958);
}

/// <summary>Calculates PIAs under 1954 Act.</summary>
///
/// <returns>Number of correct PIA table.</returns>
///
/// <param name="amesub">AME in this function.</param>
PiaMethod::table_type OldPia::pl1954( int amesub )
{
  // PIA is 55% of first $110 of AMW plus 20% of excess
  int i1 = (amesub > 110) ? 110 : amesub;
  piasub = 0.55 * static_cast<double>(i1);
  i1 = (amesub > 110) ? amesub - 110 : 0;
  piasub += 0.2 * static_cast<double>(i1);
  // minimum PIA of $30
  const double piat = BenefitAmount::round(piasub, 1954);
  piasub = max(30.0, piat);
  mfbsub = max(PiaParams::factor150 * piasub,
    max(0.8 * static_cast<double>(amesub), 50.0));
  mfbsub = min(200.0, mfbsub);
  return(PL_1954);
}

/// <summary>Calculates PIAs under 1952 Act.</summary>
///
/// <returns>Number of correct PIA table.</returns>
///
/// <param name="amesub">AME in this function.</param>
PiaMethod::table_type OldPia::pl1952( int amesub )
{
  // PIA is 55% of first $100 of AMW plus 15% of excess
  int i1 = min(amesub, 100);
  piasub = 0.55 * static_cast<double>(i1);
  i1 = max(amesub - 100, 0);
  piasub += 0.15 * static_cast<double>(i1);
  // minimum PIA of $25
  const double piat = BenefitAmount::round(piasub, 1952);
  piasub = max(25.0, piat);
  mfbsub = min(168.75, max(0.8 * static_cast<double>(amesub), 45.0));
  return(PL_1952);
}
