// Functions for the <see cref="ReindWidLC"/> class to manage reindexed
// widow(er) pia calculation with law changes.

// $Id: ReindWidLC.cpp 1.19 2011/08/09 14:59:54EDT 044579 Development  $

#include "ReindWidLC.h"
#include "UserAssumptions.h"
#include "Resource.h"
#include "piaparms.h"
#include "LawChangeArray.h"

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
/// <param name="newLawChange">Law change parameters.</param>
ReindWidLC::ReindWidLC( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear,
const PiaData& newWidowPiaData, const WorkerDataGeneral& newWidowData,
Secondary& newSecondary, const LawChangeArray& newLawChange ) :
ReindWid(newWorkerData, newPiaData, newPiaParams, newMaxyear,
newWidowPiaData, newWidowData, newSecondary), lawChange(newLawChange)
{ }

/// <summary>Destructor.</summary>
ReindWidLC::~ReindWidLC()
{ }

/// <summary>Determines applicability of method, using stored values.
/// </summary>
///
/// <remarks>This version calls the static version with 5 arguments.</remarks>
///
/// <returns>True if method is applicable.</returns>
bool ReindWidLC::isApplicable()
{
  if (isApplicable(workerData, piaData, widowPiaData.getEligYear(),
    secondary, lawChange)) {
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
/// <param name="lawChange">Law change parameters.</param>
bool ReindWidLC::isApplicable( const WorkerDataGeneral& workerData,
const PiaData& piaData, int widowEligYear, const Secondary& secondary,
const LawChangeArray& lawChange )
{
  LawChange *lawChangeNOREINDWID =
    lawChange.lawChange[LawChange::NOREINDWID];
  // skip re-indexed widow calculation if NOREINDWID lawchange is set
  if (lawChangeNOREINDWID->isEffective(secondary.entDate.getYear(),
    workerData.getBenefitDate().getYear())) {
    return(false);
  }
  return ReindWid::isApplicable(workerData, piaData, widowEligYear,
    secondary);
}
