// Functions for the <see cref="WageIndGeneral"/> class - parent of classes to
// manage wage-indexed pia calculations.

// $Id: WageIndGeneral.cpp 1.29 2012/07/30 16:26:53EDT 044579 Development  $

#include <cmath>  // for floor
#include <algorithm>
#include "WageIndGeneral.h"
#include "piaparms.h"
#include "UserAssumptions.h"
#include "DebugCase.h"
#if defined(DEBUGCASE)
#include <sstream>
#include "Trace.h"
#endif

using namespace std;

/// <summary>Initializes this instance.</summary>
///
/// <param name="newWorkerData">Worker data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
/// <param name="newTitle">Title of this method.</param>
/// <param name="newMethod">Number of this method.</param>
WageIndGeneral::WageIndGeneral( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear,
const std::string& newTitle, pia_type newMethod ) :
PiaMethod(newWorkerData, newPiaData, newPiaParams, newMaxyear, newTitle,
newMethod), bendMfb(), bendPia(), percMfb(), percPia(), percWind(),
mfbRealWageGain(), piaRealWageGain(), windfallInd(NOWINDFALLELIM),
yearsTotal(0), indexYearAvgWage(0.0)
{
  fill_n(portionAime, 3, 0.0);
  fill_n(portionPiaElig, 4, 0.0);
  BendPoints::resetMfbPerc(percMfb);
  BendPoints::resetPiaPerc(percPia);
  BendPoints::resetPiaPerc(percWind);
}

/// <summary>Destructor.</summary>
WageIndGeneral::~WageIndGeneral()
{ }

/// <summary>Initializes computed values to zero.</summary>
void WageIndGeneral::initialize()
{
  PiaMethod::initialize();
  windfallInd = NOWINDFALLELIM;
  yearsTotal = 0;
  bendMfb.assign(0.0);
  bendPia.assign(0.0);
  fill_n(portionAime, 3, 0.0);
  fill_n(portionPiaElig, 4, 0.0);
  indexYearAvgWage = 0.0;
}

/// <summary>Applies windfall elimination provision for wage-indexed
/// calculation.</summary>
void WageIndGeneral::windfallCal()
{
  // find years of coverage for wep
  yearsTotal = specMinYearsTotalCal();
  BendPoints::setWindfallPerc(percWind, piaData.getEligYear(),
    workerData.getBenefitDate().getYear(), yearsTotal);
  if (yearsTotal >= BendPoints::WINDFALL_YEARS) {
    setWindfall(HAS30YEARS);
  }
  else {
    setPiaWindfall();
    int eligYear = piaData.getEligYear() - 1;
    const double halfpension = BenefitAmount::round(
      0.5 * piaData.getPubpenApp(workerData), eligYear);
    const int year1 = yearCpi[FIRST_YEAR];
    const double piaelt = piaElig[year1] - halfpension;
    piaElig[year1] = piaelt;
    const double test = aimepiaCal(portionAime, percWind, eligYear);
    // check for one-half pension guarantee
    if (test > piaelt) {
      setWindfall(REDUCEDPERC);
      piaElig[year1] = test;
    }
    else {
      setWindfall(ONEHALFPENSION);
      piaElig[year1] = piaelt;
    }
  }
}

/// <summary>Returns number of years of coverage for windfall elimination
/// provision.</summary>
///
/// <returns>Number of years of coverage for windfall elimination provision.
/// </returns>
int WageIndGeneral::getYearsTotal() const
{
  return yearsTotal;
}

/// <summary>Indexes earnings, using specified average wage series.</summary>
///
/// <param name="year1">First year of earnings.</param>
/// <param name="year2">Base year for indexing.</param>
/// <param name="year3">Last year of earnings.</param>
/// <param name="earnings">Unindexed earnings (input).</param>
/// <param name="earnMultiplied">The earnings multiplied by average earnings
/// (output).</param>
/// <param name="earnIndexed">Indexed earnings (output).</param>
/// <param name="avgWage">Average wage indexing series.</param>
void WageIndGeneral::indexEarnings( int year1, int year2, int year3,
const DoubleAnnual& earnings, DoubleAnnual& earnMultiplied,
DoubleAnnual& earnIndexed, const DoubleAnnual& avgWage )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", first year " << year1
      << ", base year " << year2 << ", last year " << year3
      << ": Starting WageIndGeneral::indexEarnings";
    Trace::writeLine(strm.str());
  }
#endif
  indexYearAvgWage = avgWage[year2];
  for (int year = year1; year <= year2; year++) {
    if (!piaData.freezeYears.isFreezeYear(year)) {
      earnMultiplied[year] = indexYearAvgWage * earnings[year];
      const double temp = earnMultiplied[year] / avgWage[year];
      // round to nearest penny
      earnIndexed[year] = floor(temp * 100.0 + 0.5) / 100.0;
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        ostringstream strm;
        strm.setf(ios::fixed, ios::floatfield);
        strm.precision(2);
        strm << workerData.getIdString() << ", year " << year
          << ", earnings " << earnings[year] << ", indexed earnings "
          << earnIndexed[year] << ": Indexing earnings";
        Trace::writeLine(strm.str());
      }
#endif
    }
    else {
      earnMultiplied[year] = earnIndexed[year] = 0.0;
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        ostringstream strm;
        strm.setf(ios::fixed, ios::floatfield);
        strm.precision(2);
        strm << workerData.getIdString() << ", year " << year
          << ", earnings " << earnings[year] << ", indexed earnings "
          << earnIndexed[year] << ": Not using earnings in indexing period";
        Trace::writeLine(strm.str());
      }
#endif
    }
  }
  for (int year = year2; year <= year3; year++) {
    if (!piaData.freezeYears.isFreezeYear(year)) {
      earnMultiplied[year] = 0.0;
      earnIndexed[year] = earnings[year];
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        ostringstream strm;
        strm.setf(ios::fixed, ios::floatfield);
        strm.precision(2);
        strm << workerData.getIdString() << ", year " << year
          << ", earnings " << earnings[year] << ", indexed earnings "
          << earnIndexed[year] << ": Using unindexed earnings";
        Trace::writeLine(strm.str());
      }
#endif
    }
    else {
      earnMultiplied[year] = earnIndexed[year] = 0.0;
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        ostringstream strm;
        strm.setf(ios::fixed, ios::floatfield);
        strm.precision(2);
        strm << workerData.getIdString() << ", year " << year
          << ", earnings " << earnings[year] << ", indexed earnings "
          << earnIndexed[year]
          << ": Not using earnings after indexing period";
        Trace::writeLine(strm.str());
      }
#endif
    }
  }
}

/// <summary>Calculates an AIME PIA under the 1977 law.</summary>
///
/// <returns>The PIA.</returns>
///
/// <param name="portionAime">Portion of AIME in each benefit formula
/// interval.</param>
/// <param name="percPiaTemp">3 benefit formula percentages.</param>
/// <param name="year">Year to use for rounding.</param>
double WageIndGeneral::aimepiaCal( const double portionAime[],
const PercPia& percPiaTemp, int year )
{
  const double piaelt = inner_product(percPiaTemp.begin(), percPiaTemp.end(),
    portionAime, 0.0);
  return BenefitAmount::round(piaelt, year);
}

/// <summary>Projects bend points.</summary>
///
/// <remarks>In this version, that means retrieving the bend points from
/// <see cref="PiaParams::bpPiaOut"/>, where they were projected and stored
/// earlier.</remarks>
///
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="bendPiaTemp">Benefit formula bend points.</param>
void WageIndGeneral::bendPointCal( int eligYear, BendPia& bendPiaTemp ) const
{
  bendPiaTemp[1] = piaParams.bpPiaOut.getBppia1(eligYear);
  bendPiaTemp[2] = piaParams.bpPiaOut.getBppia2(eligYear);
}

/// <summary>Applies real-wage-gain adjustment.</summary>
///
/// <param name="eligYear">Year of eligibility.</param>
void WageIndGeneral::realWageGainAdj( int eligYear )
{
  const double factor =
    1.0 + 0.01 * (eligYear - UserAssumptions::getIstart());
  piaEnt.set(piaEnt.get() * factor);
  piaEnt.round(UserAssumptions::getIstart() - 1);
  if (getMethod() == PiaMethod::WAGE_IND) {
    mfbEnt.set(mfbEnt.get() * factor);
    mfbEnt.round(UserAssumptions::getIstart() - 1);
  }
}

/// <summary>Deconverts AIME from raw PIA.</summary>
///
/// <returns>Deconverted AIME.</returns>
///
/// <param name="bendPiaTemp">Benefit formula bend points (starting with 0).
/// </param>
/// <param name="percPiaTemp">Benefit formula percentages.</param>
double WageIndGeneral::deconvertAme( const BendPia& bendPiaTemp,
const PercPia& percPiaTemp ) const
{
  double rv;  // return value
  if (piasub < percPiaTemp[0] * bendPiaTemp[1]) {
    rv = piasub / percPiaTemp[0];
  }
  else {
    double temp = (percPiaTemp[0] - percPiaTemp[1]) * bendPiaTemp[1];
    if (piasub < (temp + percPiaTemp[1] * bendPiaTemp[2]))
      rv = (piasub - temp) / percPiaTemp[1];
    else {
      rv = (piasub - temp - (percPiaTemp[1] - percPiaTemp[2]) *
        bendPiaTemp[2]) / percPiaTemp[2];
    }
  }
  return (piaData.getEligYear() > 1982) ? ceil(rv) : floor(rv);
}

/// <summary>Calculates portion of aime in each section of pia formula.
/// </summary>
///
/// <param name="amesub">Aime to portion.</param>
/// <param name="aimepart">Parts of the aime.</param>
/// <param name="bendPiaTemp">Bend points to use.</param>
void WageIndGeneral::setPortionAime( double amesub, double aimepart[],
const BendPia& bendPiaTemp )
{
  aimepart[0] = min(amesub, bendPiaTemp[1]);
  aimepart[1] = max(0.0, min(amesub - bendPiaTemp[1],
    bendPiaTemp[2] - bendPiaTemp[1]));
  aimepart[2] = max(amesub - bendPiaTemp[2], 0.);
}
