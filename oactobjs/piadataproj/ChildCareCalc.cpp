// Functions for the <see cref="ChildCareCalc"/> class to manage childcare
// dropout year pia calculations.
//
// $Id: ChildCareCalc.cpp 1.37 2017/09/18 10:37:20EDT 277133 Development  $

#include "ChildCareCalc.h"
#include "UserAssumptions.h"
#include "piaparms.h"
#include "DebugCase.h"
#include "oactcnst.h"
#if defined(DEBUGCASE)
#include "Trace.h"
#endif

using namespace std;
#if !defined(__SGI_STL_INTERNAL_RELOPS)
using namespace std::rel_ops;
#endif

/// <summary>Initializes a ChildCareCalc instance.</summary>
///
/// <param name="newWorkerData">Worker data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
ChildCareCalc::ChildCareCalc( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear ) :
WageIndGeneral(newWorkerData, newPiaData, newPiaParams, newMaxyear,
"Child-Care Dropout Year Method", CHILD_CARE), adjustedN(0), 
childCareDropMax(0), childCareDrop(0)
{ }

/// <summary>Destructor.</summary>
ChildCareCalc::~ChildCareCalc()
{ }

/// <summary>Initializes computed values to zero.</summary>
void ChildCareCalc::initialize()
{
  WageIndGeneral::initialize();
  adjustedN = childCareDropMax = childCareDrop = 0;
}

/// <summary>Determines applicability of method using stored data.</summary>
///
/// <remarks>The version with no arguments calls the static version with 3
/// arguments.
///
/// This method is applicable if worker is eligible after 1978, it is not a
/// totalization benefit, and it is an old-age case with prior disability
/// entitlement in June 1980 or later that lasted to age 61, or a disability
/// case with entitlement in June 1980 or later.</remarks>
///
/// <returns>True if this method is applicable, false otherwise.</returns>
bool ChildCareCalc::isApplicable()
{
  if (isApplicable(workerData, piaData, getIoasdi())) {
    setApplicable(APPLICABLE);
    return(true);
  }
  return(false);
}

/// <summary>Determines applicability of method using passed data.</summary>
///
/// <remarks>This method is applicable if worker is eligible after 1978, it
/// is not a totalization benefit, and it is an old-age case with prior
/// disability entitlement in June 1980 or later that lasted to age 61, or a
/// disability case with entitlement in June 1980 or later.</remarks>
///
/// <returns>True if this method is applicable, false otherwise.</returns>
///
/// <param name="workerData">Worker data.</param>
/// <param name="piaData">Pia calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
bool ChildCareCalc::isApplicable( const WorkerDataGeneral& workerData,
const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi )
{
  const DisabPeriod& disabPeriod = workerData.disabPeriod[0];
  return (piaData.getEligYear() >= YEAR79 && workerData.getIend() > YEAR50 &&
    !workerData.getTotalize() && workerData.childCareYears.any() &&
    ((ioasdi == WorkerDataGeneral::OLD_AGE && workerData.getValdi() > 0 &&
    disabPeriod.getEntDate() >= PiaParams::amend80 &&
    disabPeriod.getCessationDate() >=
    DateMoyr(workerData.getBirthDate().month(),
    workerData.getBirthDate().year() + 61))
    || (workerData.getEntDate() >= PiaParams::amend80 &&
    ioasdi == WorkerDataGeneral::DISABILITY)));
}

/// <summary>Computes wage indexed PIA with childcare dropout years.</summary>
void ChildCareCalc::calculate()
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Starting ChildCareCalc::calculate");
  }
#endif
  setTableNum(PL_1977);
  const int yr1 = piaData.getEarn50(PiaData::EARN_WITH_TOTALIZATION);
  const int yr2 = piaData.getEarnYear();
  indexEarnings(yr1, piaData.getEligYear() - 2, yr2, piaData.earnOasdiLimited,
    earnMultiplied, earnIndexed, piaParams.getFqIndex());
  const int i3 = piaData.compPeriodNew.getN();
  orderEarnings(yr1, yr2, i3);
  // find childcare dropout years
  childCareDropoutCal();
  // adjust number of years by number of childcare dropout years
  adjustedN = i3 - childCareDrop;
  totalEarnCal(yr1, yr2, adjustedN);
  const int yr5 = piaData.getEligYear();
  // project applicable bend points from 1979 bend points
  bendPointCal(yr5, bendPia);
  // find portion of AIME in each AIME interval
  setPortionAime(getAme(), portionAime, bendPia);
  // set pia formula percentages
  piaParams.percPiaCal(yr5, percPia);
  const int yr4 = piaData.getEligYear() - 1;
  yearCpi[FIRST_YEAR] = yr4;
  yearCpi[YEAR_ELIG] = yr5;
  piaElig[yr4] = aimepiaCal(portionAime, percPia, yr4);
  // apply windfall elimination provision
  if (wep_app())
    windfallCal();
  // apply benefit increases to PIA
  setYearCpi();
  piaEnt.set(applyColas(piaElig, yr5, workerData.getBenefitDate()));
  bendMfb.project(yr5, piaParams.getFqArray());
  // calculate the AIME MFB
  setPortionPiaElig(piaElig[yr4], portionPiaElig, bendMfb);
  mfbElig[yr4] = mfbCal(portionPiaElig, percMfb, yr4);
  mfbEnt.set(applyColas(mfbElig, yr5, workerData.getBenefitDate()));
  if (yr5 > UserAssumptions::getIstart() && isPebsAssumptions()) {
    piaRealWageGain = piaEnt;
    mfbRealWageGain = mfbEnt;
    realWageGainAdj(yr5);
  }
  setDirty();
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from ChildCareCalc::calculate");
  }
#endif
}

/// <summary>Adjust for childcare dropout years.</summary>
void ChildCareCalc::childCareDropoutCal()
{
  childCareDrop = 0;
  childCareDropMax = childCareDropMaxCal();
  // if no allowable childcare years, return
  if (childCareDropMax <= 0)
    return;
  // find amount of earnings allowed in a childcare dropout year
  const double childcareAmount = piaParams.getChildcareDropoutAmount(
    piaData.getEligDate().getYear(), workerData.getBenefitDate().getYear()) +
    0.01;
  // Look for usable childcare years and remove them.  Must have
  // child in care and no earnings
  const int earn50 = piaData.getEarn50(PiaData::EARN_WITH_TOTALIZATION);
  for (int yr = earn50; yr <= piaData.getEarnYear(); ++yr) {
    if (iorder[yr] == 1 && workerData.childCareYears.getBit(yr) &&
      piaData.earnOasdiLimited[yr] <= childcareAmount) {
      iorder[yr] = -1;
      childCareDrop++;
      if (childCareDrop >= childCareDropMax) {
        break;
      }
    }
  }
  // look for more childcare years, if we have not found enough and there
  // are some zero years without a child in care in the selected years,
  // and there are some years with a child in care in the non-selected years
  if (childCareDrop < childCareDropMax) {
    // create a vector of years with zero earnings and no child in
    // care in the selected years
    vector< int > count1;
    for (int yr = earn50; yr <= piaData.getEarnYear(); ++yr) {
      if (iorder[yr] == 1 && !workerData.childCareYears.getBit(yr) &&
        piaData.earnOasdiLimited[yr] <= childcareAmount) {
        count1.push_back(yr);
      }
    }
    // nothing more to do if there are no years with zero earnings
    // and no child in care in the selected years
    if (count1.size() == 0)
      return;
    // create a vector of years with zero earnings and a child in
    // care in the non-selected years
    vector< int > count2;
    for (int yr = earn50; yr <= piaData.getEarnYear(); ++yr) {
      if (iorder[yr] == 0 && workerData.childCareYears.getBit(yr) &&
        piaData.earnOasdiLimited[yr] <= childcareAmount) {
        count2.push_back(yr);
      }
    }
    // nothing more to do if there are no years with zero earnings and a
    // child in care in the non-selected years
    if (count2.size() == 0)
      return;
    // number swappable is minimum of the two counts
    int numswap = static_cast<int>(min(count1.size(), count2.size()));
    // limit to maximum number of childcare dropout years
    numswap = min(numswap, childCareDropMax - childCareDrop);
    // swap the years in and out of the selected period
    for (int i1 = 0; i1 < numswap; ++i1) {
      iorder[count1[i1]] = 0;
      iorder[count2[i1]] = -1;
      childCareDrop++;
    }
  }
}

/// <summary>Returns maximum number of childcare dropout years.</summary>
///
/// <remarks>Assumes that there is a specified maximum for combined regular and
///   childcare dropout years, and that number of regular dropout years
///   has already been calculated.</remarks>
///
/// <returns>Maximum number of childcare dropout years.</returns>
int ChildCareCalc::childCareDropMaxCal() const
{
  const int i1 = piaData.compPeriodNew.getNDrop();
  // under present law, number does not vary by year
  const int i2 = PiaParams::getMaxChildcareDropoutYearsPL();
  if (i1 < i2) {
    // can have up to 3 total dropout years, but must
    // have at least 2 computation years
    return min(i2 - i1, piaData.compPeriodNew.getN() - 2);
  }
  else {
    // already have maximum dropout years
    return 0;
  }
}
