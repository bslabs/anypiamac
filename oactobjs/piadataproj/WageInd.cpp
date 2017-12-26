// Functions for the <see cref="WageInd"/> class to calculate a wage-indexed
// PIA.

// $Id: WageInd.cpp 1.40 2017/09/18 10:37:22EDT 277133 Development  $

#include "WageInd.h"
#include "UserAssumptions.h"
#include "piaparms.h"
#include "DebugCase.h"
#if defined(DEBUGCASE)
#include <sstream>
#include "Trace.h"
#endif

using namespace std;

/// <summary>Initializes a WageInd instance.</summary>
///
/// <param name="newWorkerData">Worker basic data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
WageInd::WageInd( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear ) :
WageIndGeneral(newWorkerData, newPiaData, newPiaParams, newMaxyear,
"Wage-Indexed Formula (1977 Act)", WAGE_IND)
{ }

/// <summary>Destructor.</summary>
WageInd::~WageInd()
{ }

/// <summary>Determines applicability of method using stored values.</summary>
///
/// <remarks>This version calls the static version with 2 arguments.</remarks>
///
/// <returns>True if method is applicable.</returns>
bool WageInd::isApplicable()
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
bool WageInd::isApplicable( const WorkerDataGeneral& workerData,
const PiaData& piaData )
{
  return (piaData.getEligYear() > 1978 && (workerData.getIend() > 1950 ||
    workerData.getTotalize()));
}

/// <summary>Computes wage indexed PIA.</summary>
void WageInd::calculate()
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Starting WageInd::calculate");
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
      << ": Before indexEarnings in WageInd::calculate";
    Trace::writeLine(strm.str());
  }
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm.setf(ios::fixed, ios::floatfield);
    strm.precision(2);
    strm << workerData.getIdString() << ", earnings in " << year1 << " = "
      << earnings[year1] << ": Before indexEarnings in WageInd::calculate";
    Trace::writeLine(strm.str());
  }
#endif
  indexEarnings(year1, piaData.getEligYear() - 2, year2, earnings,
    earnMultiplied, earnIndexed, piaParams.getFqIndex());
  const int N = piaData.compPeriodNew.getN();
  orderEarnings(year1, year2, N);
  totalEarnCal(year1, year2, N);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm.setf(ios::fixed, ios::floatfield);
    strm.precision(0);
    strm << workerData.getIdString() << ", aime " << getAme()
      << ": After totalEarnCal in WageInd::calculate";
    Trace::writeLine(strm.str());
  }
#endif
  const int year5 = piaData.getEligYear();
  // project applicable bend points from 1979 bend points
  bendPointCal(year5, bendPia);
  // find portion of AIME in each AIME interval
  setPortionAime(getAme(), portionAime, bendPia);
  // set pia formula percentages
  piaParams.percPiaCal(year5, percPia);
  const int year4 = piaData.getEligYear() - 1;
  yearCpi[FIRST_YEAR] = year4;
  yearCpi[YEAR_ELIG] = year5;
  piaElig[year4] = aimepiaCal(portionAime, percPia, year4);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm.setf(ios::fixed, ios::floatfield);
    strm.precision(2);
    strm << workerData.getIdString() << ", pia at eligibility "
      << piaElig[year4] << ": After aimepiaCal in WageInd::calculate";
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
      << piaEnt.toString() << ": After applyColas in WageInd::calculate";
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
