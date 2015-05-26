// Functions for the <see cref="ChildCareCalcLC"/> class to manage childcare
// dropout year pia calculations, with law changes.
//
// $Id: ChildCareCalcLC.cpp 1.19 2011/07/29 13:49:24EDT 044579 Development  $

#include "ChildCareCalcLC.h"
#include "LawChangeArray.h"
#include "LawChangeCHILDCAREDROPOUT.h"

using namespace std;

/// <summary>Initializes a ChildCareCalcLC instance.</summary>
///
/// <param name="newWorkerData">Worker data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
/// <param name="newLawChange">Law change parameters.</param>
ChildCareCalcLC::ChildCareCalcLC( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear,
const LawChangeArray& newLawChange ) :
ChildCareCalc(newWorkerData, newPiaData, newPiaParams, newMaxyear),
lawChange(newLawChange)
{ }

/// <summary>Destructor.</summary>
ChildCareCalcLC::~ChildCareCalcLC()
{ }

/// <summary>Determines applicability of method, using stored data.</summary>
///
/// <remarks>The version with no arguments calls the static version with 5
/// arguments.</remarks>
///
/// <returns>True if this method is applicable, false otherwise.</returns>
bool ChildCareCalcLC::isApplicable()
{
  if (isApplicable(workerData, piaData, getIoasdi(), lawChange)) {
    setApplicable(APPLICABLE);
    return(true);
  }
  return(false);
}

/// <summary>Determines applicability of method, using passed data.</summary>
///
/// <returns>True if this method is applicable, false otherwise.</returns>
///
/// <param name="workerData">Worker data.</param>
/// <param name="piaData">Pia calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
/// <param name="lawChange">Law change parameters.</param>
bool ChildCareCalcLC::isApplicable( const WorkerDataGeneral& workerData,
const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi,
const LawChangeArray& lawChange )
{
  // see if lawchange is effective
  LawChangeCHILDCAREDROPOUT *lawChangeCHILDCAREDROPOUT =
    lawChange.lawChangeCHILDCAREDROPOUT;
  if (lawChangeCHILDCAREDROPOUT->isEffective(
    piaData.getEligDate().getYear(),
    workerData.getBenefitDate().getYear())) {
    return(true);
  }
  return ChildCareCalc::isApplicable(workerData, piaData, ioasdi);
}

/// <summary>Returns maximum number of childcare dropout years.</summary>
///
/// <returns>Maximum number of childcare dropout years.</returns>
int ChildCareCalcLC::childCareDropMaxCal() const
{
  const int i1 = piaData.compPeriodNew.getNDrop();
  // get maximum number of childcare dropout years
  const int i2 = piaParams.getMaxChildcareDropoutYears(
    piaData.getEligDate().getYear(),
    workerData.getBenefitDate().getYear());
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
