// Functions for the <see cref="WageIndNonFreeze"/> class to calculate a wage-indexed
// non-freeze PIA.

// $Id: WageIndNonFreeze.cpp 1.4 2017/09/18 10:37:21EDT 277133 Development  $

#include "WageIndNonFreeze.h"
#include "UserAssumptions.h"
#include "piaparms.h"
#include "DebugCase.h"
#if defined(DEBUGCASE)
#include <sstream>
#include "Trace.h"
#endif

using namespace std;

/// <summary>Initializes a WageIndNonFreeze instance.</summary>
///
/// <param name="newWorkerData">Worker basic data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
WageIndNonFreeze::WageIndNonFreeze( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear ) :
WageIndGeneral(newWorkerData, newPiaData, newPiaParams, newMaxyear,
"Wage-Indexed Formula Non-Freeze (1977 Act)", WAGE_IND_NON_FREEZE)
{ }

/// <summary>Destructor.</summary>
WageIndNonFreeze::~WageIndNonFreeze()
{ }

/// <summary>Determines applicability of method using stored values.</summary>
///
/// <remarks>This version calls the static version with 2 arguments.</remarks>
///
/// <returns>True if method is applicable.</returns>
bool WageIndNonFreeze::isApplicable()
{
  if (isApplicable(workerData, piaData)) {
    setApplicable(APPLICABLE);
    return(true);
  }
  else {
    return(false);
  }
}

/// <summary>Determines applicability of method using passed values.</summary>
///
/// <remarks>The wage-indexed method is applicable if the eligibility year is
/// greater than 1978 and there are some earnings after 1950. Note that in a
/// totalization case we use the artificially calculated earnings, which are
/// always present after 1950.</remarks>
///
/// <returns>True if method is applicable.</returns>
///
/// <param name="workerData">Worker basic data.</param>
/// <param name="piaData">Pia calculation data.</param>
bool WageIndNonFreeze::isApplicable( const WorkerDataGeneral& workerData,
const PiaData& piaData )
{
  if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY) {
    if (!piaData.disInsNonFreezeCode.isDisabilityInsured()) {
      return false;
    }
  } else {
    if (workerData.getJoasdi() == WorkerDataGeneral::OLD_AGE) {
      if (!piaData.finsNonFreezeCode.isFullyInsured() || 
        workerData.getValdi() == 0) {
        return false;
      }
    } else {
      if (!piaData.finsNonFreezeCode.isFullyOrCurrentlyInsured() || 
        workerData.getValdi() == 0) {
        return false;
      }
    }
  }
  int elyTemp = min((int)workerData.disabPeriod[0].getWaitperDate().getYear(),
     (int)piaData.getBirthDateMinus1().year()+62);
  return (elyTemp > 1978 && (workerData.getIend() > 1950 ||
    workerData.getTotalize()));
}

/// <summary>Computes wage indexed PIA.</summary>
void WageIndNonFreeze::calculate()
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Starting WageIndNonFreeze::calculate");
  }
#endif
  setTableNum(PL_1977);
  const AverageWage& earnings = workerData.getTotalize() ?
    piaData.earnTotalizedLimited : piaData.earnOasdiLimited;
  const int year1 = piaData.getEarn50(PiaData::EARN_WITH_TOTALIZATION);
  const int year2 = piaData.getEarnYear();
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", first year of earnings " << year1
      << ", last year of earnings " << year2
      << ": Before indexEarnings in WageIndNonFreeze::calculate";
    Trace::writeLine(strm.str());
  }
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm.setf(ios::fixed, ios::floatfield);
    strm.precision(2);
    strm << workerData.getIdString() << ", earnings in " << year1 << " = "
      << earnings[year1] << ": Before indexEarnings in WageIndNonFreeze::calculate";
    Trace::writeLine(strm.str());
  }
#endif 
  const int year5 = piaData.getEligYearNonFreeze();
  indexEarnings(year1, year5 - 2, year2, earnings,
    earnMultiplied, earnIndexed, piaParams.getFqIndex());
  int N = piaData.compPeriodNewNonFreeze.getN();
  orderEarnings(year1, year2, N);
  totalEarnCal(year1, year2, N);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm.setf(ios::fixed, ios::floatfield);
    strm.precision(0);
    strm << workerData.getIdString() << ", aime " << getAme()
      << ": After totalEarnCal in WageIndNonFreeze::calculate";
    Trace::writeLine(strm.str());
  }
#endif
  // project applicable bend points from 1979 bend points
  bendPointCal(year5, bendPia);
  // find portion of AIME in each AIME interval
  setPortionAime(getAme(), portionAime, bendPia);
  // set pia formula percentages
  piaParams.percPiaCal(year5, percPia);
  const int year4 = piaData.getEligYearNonFreeze() - 1;
  yearCpi[FIRST_YEAR] = year4;
  yearCpi[YEAR_ELIG] = year5;
  piaElig[year4] = aimepiaCal(portionAime, percPia, year4);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm.setf(ios::fixed, ios::floatfield);
    strm.precision(2);
    strm << workerData.getIdString() << ", pia at eligibility "
      << piaElig[year4] << ": After aimepiaCal in WageIndNonFreeze::calculate";
    Trace::writeLine(strm.str());
  }
#endif
  // apply windfall elimination provision
  if (wep_app())
    windfallCal();
  // apply benefit increases to PIA
  setYearCpi();
  piaEnt.set(applyColas(piaElig, year5, workerData.getBenefitDate()));
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm.setf(ios::fixed, ios::floatfield);
    strm.precision(2);
    strm << workerData.getIdString() << ", pia at entitlement "
      << piaEnt.toString() << ": After applyColas in WageIndNonFreeze::calculate";
    Trace::writeLine(strm.str());
  }
#endif
  bendMfb.project(year5, piaParams.getFqArray());
  // apply totalization
  if (workerData.getTotalize()) {
    prorate();
    setAmeTotal();
    piasub = piaElig[year4];
    setAme(deconvertAme(bendPia, percPia));
  }
  // calculate the AIME MFB
  setPortionPiaElig(piaElig[year4], portionPiaElig, bendMfb);
  mfbElig[year4] = mfbCal(portionPiaElig, percMfb, year4);
  mfbEnt.set(applyColas(mfbElig, year5, workerData.getBenefitDate()));
  if (year5 > UserAssumptions::getIstart() && isPebsAssumptions()) {
    piaRealWageGain = piaEnt;
    mfbRealWageGain = mfbEnt;
    realWageGainAdj(year5);
  }
  setDirty();
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
void WageIndNonFreeze::indexEarnings( int year1, int year2, int year3,
const DoubleAnnual& earnings, DoubleAnnual& earnMultiplied,
DoubleAnnual& earnIndexed, const DoubleAnnual& avgWage )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", first year " << year1
      << ", base year " << year2 << ", last year " << year3
      << ": Starting WageIndNonFreeze::indexEarnings";
    Trace::writeLine(strm.str());
  }
#endif
  indexYearAvgWage = avgWage[year2];
  for (int year = year1; year <= year2; year++) {
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
  for (int year = year2; year <= year3; year++) {
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
}

