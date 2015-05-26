// Functions for the <see cref="DibGuarLC"/> class to manage disability
// guarantee pia calculations with law changes.
//
// $Id: DibGuarLC.cpp 1.9 2011/07/29 15:26:16EDT 044579 Development  $

#include "DibGuarLC.h"
#include "UserAssumptions.h"
#include "Resource.h"
#include "piaparms.h"
#include "LawChangeArray.h"

using namespace std;

/// <summary>Initializes a DibGuarLC instance.</summary>
///
/// <param name="newWorkerData">Worker basic data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
/// <param name="newLawChange">Law change parameters.</param>
DibGuarLC::DibGuarLC( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear,
const LawChangeArray& newLawChange ) :
DibGuar(newWorkerData, newPiaData, newPiaParams, newMaxyear),
lawChange(newLawChange)
{ }

/// <summary>Destructor.</summary>
DibGuarLC::~DibGuarLC()
{ }

/// <summary>Determines applicability of method using stored data.</summary>
///
/// <remarks>The version with no arguments calls the static version with 5
/// arguments.</remarks>
///
/// <returns>True if method is applicable.</returns>
bool DibGuarLC::isApplicable()
{
  if (isApplicable(workerData.getEntDate(), workerData, piaData, getIoasdi(),
    lawChange)) {
    setApplicable(APPLICABLE);
    return(true);
  }
  else {
    return(false);
  }
}

/// <summary>Determines applicability of method using passed data.</summary>
///
/// <returns>True if method is applicable.</returns>
///
/// <param name="entDate">The date of entitlement.</param>
/// <param name="workerData">Worker basic data.</param>
/// <param name="piaData">Pia calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
/// <param name="lawChange">Law change parameters.</param>
bool DibGuarLC::isApplicable( const DateMoyr& entDate,
const WorkerDataGeneral& workerData, const PiaData& piaData,
WorkerDataGeneral::ben_type ioasdi, const LawChangeArray& lawChange )
{
  LawChange *lawChangeNODIBGUAR = lawChange.lawChange[LawChange::NODIBGUAR];
  // skip DIB guarantee calculation if NODIBGUAR lawchange is set
  if (lawChangeNODIBGUAR->isEffective(entDate.getYear(),
    workerData.getBenefitDate().getYear())) {
    return(false);
  }
  return DibGuar::isApplicable(workerData, piaData, ioasdi);
}
