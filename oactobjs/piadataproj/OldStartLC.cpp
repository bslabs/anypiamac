// Functions for the <see cref="OldStartLC"/> class to manage old-start pia
// calculations, with changes from present law.
//
// $Id: OldStartLC.cpp 1.27 2011/08/01 11:38:36EDT 044579 Development  $

#include "OldStartLC.h"
#include "pibtable.h"
#include "LawChangeArray.h"

/// <summary>Initializes an old start instance with law changes.</summary>
///
/// <param name="newWorkerData">Worker data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum projection year.</param>
/// <param name="newEntDate">The date of entitlement.</param>
/// <param name="newLawChange">Law-change parameters.</param>
OldStartLC::OldStartLC( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams,
int newMaxyear, const DateMoyr& newEntDate,
const LawChangeArray& newLawChange ) :
OldStart(newWorkerData, newPiaData, newPiaParams, newMaxyear, newEntDate),
lawChange(newLawChange)
{ }

/// <summary>Destructor.</summary>
OldStartLC::~OldStartLC()
{ }

/// <summary>Determines applicability of method using stored data.</summary>
///
/// <remarks>This version, with no arguments, calls the static version with 4
/// arguments.</remarks>
///
/// <returns>True if method is applicable, false otherwise.</returns>
bool OldStartLC::isApplicable()
{
  if (isApplicable(entDate, workerData.getBenefitDate(), piaData,
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
/// <returns>True if method is applicable, false otherwise.</returns>
///
/// <param name="entDate">The date of entitlement.</param>
/// <param name="benefitDate">The date of benefit.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="lawChange">Law change parameters.</param>
bool OldStartLC::isApplicable( const DateMoyr& entDate,
const DateMoyr& benefitDate, const PiaData& piaData,
const LawChangeArray& lawChange )
{
  if (lawChange.lawChange[LawChange::NOOLDSTART]->isEffective(
    entDate.getYear(), benefitDate.getYear())) {
    return(false);
  }
  return OldStart::isApplicable(piaData);
}
