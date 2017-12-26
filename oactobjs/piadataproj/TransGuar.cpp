// Functions for the <see cref="TransGuar"/> class to manage transitional
// guarantee pia calculations.
//
// $Id: TransGuar.cpp 1.41 2017/09/18 10:37:21EDT 277133 Development  $

#include "TransGuar.h"
#include "piaparms.h"
#include "oactcnst.h"

using namespace std;

/// <summary>Initializes a TransGuar instance.</summary>
///
/// <param name="newWorkerData">Worker basic data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
TransGuar::TransGuar( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear ) :
OldPia(newWorkerData, newPiaData, newPiaParams, newMaxyear,
"Transitional Guarantee (1977 Act)", TRANS_GUAR), bendMfb(), percMfb()
{
  fill_n(portionPiaElig, 4, 0.0);
}

/// <summary>Destructor.</summary>
TransGuar::~TransGuar()
{ }

/// <summary>Initializes computed values to zero.</summary>
void TransGuar::initialize()
{
  PiaMethod::initialize();
  bendMfb.deleteContents();
  fill_n(portionPiaElig, 4, 0.0);
}

/// <summary>Determines applicability of method using stored values.</summary>
///
/// <remarks>This version calls the static version, passing the stored values.
/// </remarks>
///
/// <returns>True if method is applicable.</returns>
bool TransGuar::isApplicable()
{
  if (isApplicable(workerData, piaData, getIoasdi())) {
    setApplicable(APPLICABLE);
    return(true);
  }
  else
    return(false);
}

/// <summary>Determines applicability of method using passed values.</summary>
///
/// <remarks>The year of attainment of age 62 must be between 1979 and 1983,
/// inclusive. The benefit must be old-age, or survivor if death occurred
/// before the month of attainment of age 62. It must not be a totalized
/// benefit, and there must be some earnings before 1979.</remarks>
///
/// <returns>True if method is applicable.</returns>
///
/// <param name="workerData">Worker basic data.</param>
/// <param name="piaData">Pia calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
bool TransGuar::isApplicable( const WorkerDataGeneral& workerData,
const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi )
{
  try {
    int itrans = 5;  // transitional guarantee period
    // date of age 62
    const boost::gregorian::date age62 =
      Date::addYears(piaData.getBirthDateMinus1(), 62);
    if (piaData.getEligYear() >= YEAR79 &&
      static_cast<int>(piaData.getEligDate().getYear()) < YEAR79 + itrans &&
      ioasdi != WorkerDataGeneral::DISABILITY &&
      (ioasdi != WorkerDataGeneral::SURVIVOR ||
      !(workerData.getDeathDate() < age62)) &&
      !workerData.getTotalize() && workerData.getIbegin() < YEAR79) {
      return(true);
    }
    else {
      return(false);
    }
  } catch (exception& e) {
    throw PiaException(string("In TransGuar::isApplicable: ") + e.what());
  }
}

/// <summary>Computes transitional guarantee PIA.</summary>
void TransGuar::calculate()
{
  const AverageWage& earnings = workerData.getTotalize() ?
    piaData.earnTotalizedLimited : piaData.earnOasdiLimited;
  // set earnings to be used up to year prior to eligibility
  int year2 = piaData.getEarn50(PiaData::EARN_NO_TOTALIZATION);
  for (int year1 = year2; year1 < piaData.getEligYear(); year1++) {
    if (!piaData.freezeYears.isFreezeYear(year1))
      earnIndexed[year1] = earnings[year1];
  }
  // order the earnings and compute average monthly earnings
  const int year3 = piaData.getEligYear() - 1;
  const int year4 = piaData.getEligYear();
  const int N = piaData.compPeriodNew.getN();
  orderEarnings(year2, year3, N);
  totalEarnCal(year2, year3, N);
  // calculate Dec 1978 PIA
  yearCpi[FIRST_YEAR] = year3;
  yearCpi[YEAR_ELIG] = year4;
  setTableNum(cpiBase(PiaParams::amend771, true, getAme(), false));
  piaEnt.set(piasub);
  mfbEnt.set(mfbsub);
  bendMfb.project(year4, piaParams.getFqArray());
  piaElig[year3] = piaEnt.get();
  // find AIME MFB from wage-indexed formula
  setPortionPiaElig(piaElig[year3], portionPiaElig, bendMfb);
  mfbElig[year3] = mfbCal(portionPiaElig, percMfb, year3);
  // apply cpi increases to PIA and MFB
  setYearCpi();
  piaEnt.set(applyColas(piaElig, year4, workerData.getBenefitDate()));
  mfbEnt.set(applyColas(mfbElig, year4, workerData.getBenefitDate()));
  setDirty();
}
