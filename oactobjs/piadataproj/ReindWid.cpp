// Functions for the <see cref="ReindWid"/> class to manage reindexed
// widow(er) pia calculations.
//
// $Id: ReindWid.cpp 1.49 2011/10/11 15:23:09EDT 044579 Development  $

#include "ReindWid.h"
#include "UserAssumptions.h"
#include "Resource.h"
#include "piaparms.h"
#include "oactcnst.h"
#include "DebugCase.h"
#if defined(DEBUGCASE)
#include <sstream>
#include "Trace.h"
#endif

using namespace std;

/// <summary>Initializes this instance.</summary>
///
/// <param name="newWorkerData">Worker basic data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
/// <param name="newWidowPiaData">Widow calculation data.</param>
/// <param name="newWidowData">Widow basic data.</param>
/// <param name="newSecondary"><see cref="Secondary"/> benefit data.</param>
ReindWid::ReindWid( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear,
const PiaData& newWidowPiaData, const WorkerDataGeneral& newWidowData,
Secondary& newSecondary ) :
WageIndGeneral(newWorkerData, newPiaData, newPiaParams, newMaxyear,
"Re-indexed Widow (1983 Act)", REIND_WID), eligYear(0),
widowPiaData(newWidowPiaData), widowData(newWidowData),
secondary(newSecondary)
{ }

/// <summary>Destructor.</summary>
ReindWid::~ReindWid()
{ }

/// <summary>Initializes computed values to zero.</summary>
void ReindWid::initialize()
{
  WageIndGeneral::initialize();
  eligYear = 0;
}

/// <summary>Determines applicability of method, using stored valules.
/// </summary>
///
/// <remarks>This version calls the static version with 4 arguments.</remarks>
///
/// <returns>True if method is applicable.</returns>
bool ReindWid::isApplicable()
{
  if (isApplicable(workerData, piaData, widowPiaData.getEligYear(),
    secondary)) {
    setApplicable(APPLICABLE);
    return(true);
  }
  else {
    return(false);
  }
}

/// <summary>Determines applicability of method, using passed values.
/// </summary>
///
/// <returns>True if method is applicable.</returns>
///
/// <param name="workerData">Worker basic data.</param>
/// <param name="piaData">Pia calculation data.</param>
/// <param name="widowEligYear">Widow(er)'s year of eligibility.</param>
/// <param name="secondary">Widow(er)'s benefit information.</param>
bool ReindWid::isApplicable( const WorkerDataGeneral& workerData,
const PiaData& piaData, int widowEligYear, const Secondary& secondary )
{
  try {
    // date of age 62
    boost::gregorian::date age62 =
      Date::addYears(piaData.getBirthDateMinus1(), 62);
    if (piaData.getEligYear() > 1978 && workerData.getIend() > YEAR50 &&
      secondary.bic.isWidow() && workerData.getDeathDate() < age62 &&
      !workerData.getTotalize()) {
      // re-indexed widow guarantee
      const int year = (secondary.bic.getMajorBic() == 'W') ?
        secondary.entDate.getYear() : widowEligYear;
      return (year > 1984 || workerData.getDeathDate().year() >= 1985);
    }
    else {
      return(false);
    }
  } catch (exception& e) {
    throw PiaException(string("In ReindWid::isApplicable: ") + e.what());
  } catch (...) {
    throw PiaException("In ReindWid::isApplicable: unknown exception");
  }
}

/// <summary>Computes reindexed widow PIA.</summary>
void ReindWid::calculate()
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Starting ReindWid::calculate");
  }
#endif
  setEligYear();
  const AverageWage& earnings = workerData.getTotalize() ?
    piaData.earnTotalizedLimited : piaData.earnOasdiLimited;
  const int yr1 = piaData.getEarn50(PiaData::EARN_NO_TOTALIZATION);
  const int yr2 = piaData.getEarnYear();
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", first year of earnings " << yr1
      << ", last year of earnings " << yr2
      << ": Before indexEarnings in ReindWid::calculate";
    Trace::writeLine(strm.str());
  }
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", earnings in " << yr1 << " = "
      << earnings[yr1] << ": Before indexEarnings in ReindWid::calculate";
    Trace::writeLine(strm.str());
  }
#endif
  indexEarnings(yr1, eligYear - 2, yr2, earnings,
    earnMultiplied, earnIndexed, piaParams.getFqIndex());
  const int N = piaData.compPeriodNew.getN();
  orderEarnings(yr1, yr2, N);
  totalEarnCal(yr1, yr2, N);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm.setf(ios::fixed, ios::floatfield);
    strm.precision(2);
    strm << workerData.getIdString() << ", pia at entitlement " << piaEnt.get()
      << ": After totalEarnCal in ReindWid::calculate";
    Trace::writeLine(strm.str());
  }
#endif
  // project applicable bend points from 1979 bend points
  bendPointCal(eligYear, bendPia);
  // find portion of AIME in each AIME interval
  setPortionAime(getAme(), portionAime, bendPia);
  // set pia formula percentages
  piaParams.percPiaCal(eligYear, percPia);
  yearCpi[FIRST_YEAR] = eligYear - 1;
  yearCpi[YEAR_ELIG] = eligYear;
  piaEnt.set(aimepiaCal(portionAime, percPia, eligYear - 1));
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm.setf(ios::fixed, ios::floatfield);
    strm.precision(2);
    strm << workerData.getIdString() << ", pia at entitlement "
      << piaEnt.get() << ", yearCpi[FIRST_YEAR] " << yearCpi[FIRST_YEAR]
      << ": After aimepiaCal in ReindWid::calculate";
    Trace::writeLine(strm.str());
  }
#endif
  piaElig[yearCpi[FIRST_YEAR]] = piaEnt.get();
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Ready to call setYearCpi in ReindWid::calculate");
  }
#endif
  setYearCpi();
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": After setYearCpi in ReindWid::calculate");
  }
#endif
  piaEnt.set(applyColas(piaElig, eligYear, workerData.getBenefitDate()));
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm.setf(ios::fixed, ios::floatfield);
    strm.precision(2);
    strm << workerData.getIdString() << ", pia at entitlement "
      << piaEnt.get() << ": After applyColas in ReindWid::calculate";
    Trace::writeLine(strm.str());
  }
#endif
  if (eligYear > UserAssumptions::getIstart() && isPebsAssumptions()) {
    piaRealWageGain = piaEnt;
    realWageGainAdj(eligYear);
  }
  setDirty();
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from ReindWid::calculate");
  }
#endif
}

/// <summary>Sets year of eligibility in reindexed widow guarantee method.
/// </summary>
void ReindWid::setEligYear()
{
  eligYear = min(max(static_cast<int>(piaData.getEligDate().getYear()),
    widowPiaData.getEligYear()),
    static_cast<int>(piaData.getBirthDateMinus1().year()) + 62);
}
