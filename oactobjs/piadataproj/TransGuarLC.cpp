// Functions for the <see cref="TransGuarLC"/> class to manage transitional
// guarantee pia calculations, with changes from present law.
//
// $Id: TransGuarLC.cpp 1.28 2011/08/11 08:46:59EDT 044579 Development  $

#include "TransGuarLC.h"
#include "LawChangeArray.h"
#include "oactcnst.h"

using namespace std;

/// <summary>Initializes a TransGuarLC instance.</summary>
///
/// <param name="newWorkerData">Worker basic data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
/// <param name="newLawChange">Law-change parameters.</param>
TransGuarLC::TransGuarLC( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear,
const LawChangeArray& newLawChange ) :
TransGuar(newWorkerData, newPiaData, newPiaParams, newMaxyear),
lawChange(newLawChange)
{ }

/// <summary>Destructor.</summary>
TransGuarLC::~TransGuarLC()
{ }

/// <summary>Determines applicability of method using stored values.</summary>
///
/// <remarks>Thia version calls the static version with 4 arguments, passing
/// the stored values as arguments.</remarks>
///
/// <returns>True if method is applicable.</returns>
bool TransGuarLC::isApplicable()
{
  if (isApplicable(workerData, piaData, getIoasdi(), lawChange)) {
    setApplicable(APPLICABLE);
    return(true);
  }
  else
    return(false);
}

/// <summary>Determines applicability of method using passed values.</summary>
///
/// <remarks>The year of attainment of age 62 must be between 1979 and the
/// last year specified in the law change, inclusive. The benefit must be
/// old-age, or survivor if death occurred before the month of attainment of
/// age 62. It must not be a totalized benefit, and there must be some earnings
/// before 1979.</remarks>
///
/// <returns>True if method is applicable.</returns>
///
/// <param name="workerData">Worker basic data.</param>
/// <param name="piaData">Pia calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
/// <param name="lawChange">Law-change parameters.</param>
bool TransGuarLC::isApplicable( const WorkerDataGeneral& workerData,
const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi,
const LawChangeArray& lawChange )
{
  try {
    int itrans = 5;  // transitional guarantee period
    LawChange *lawChangeTRANSGUAR1 = lawChange.lawChange[LawChange::TRANSGUAR1];
    if (lawChangeTRANSGUAR1->getInd() == 2)
      itrans = 10;
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
    throw PiaException(string("In TransGuarLC::isApplicable: ") + e.what());
  }
}
