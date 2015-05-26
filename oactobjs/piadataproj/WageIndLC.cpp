// Functions for the <see cref="WageIndLC"/> class to manage
// wage-indexed pia calculation, with law change parameters.

// $Id: WageIndLC.cpp 1.23 2011/08/11 13:55:34EDT 044579 Development  $

#include "WageIndLC.h"
#include "LawChangeArray.h"

/// <summary>Initializes a WageIndLC instance.</summary>
///
/// <param name="newWorkerData">Worker basic data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
/// <param name="newLawChange">Law-change parameters.</param>
/// <param name="entitDate">Entitlement date.</param>
WageIndLC::WageIndLC( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear,
const LawChangeArray& newLawChange, const DateMoyr& entitDate ) :
WageInd(newWorkerData, newPiaData, newPiaParams, newMaxyear),
lawChange(newLawChange), entDate(entitDate)
{ }

/// <summary>Destructor.</summary>
WageIndLC::~WageIndLC()
{ }

/// <summary>Determines applicability of method using stored values.</summary>
///
/// <remarks>This version calls the static version with 4 arguments, passing
/// the stored values.</remarks>
///
/// <returns>True if method is applicable.</returns>
bool WageIndLC::isApplicable()
{
  if (isApplicable(workerData, piaData, lawChange, entDate)) {
    setApplicable(APPLICABLE);
    return(true);
  }
  return(false);
}

/// <summary>Determines applicability of method using passed values.</summary>
///
/// <remarks>Returns <see cref="WageInd::isApplicable"/>, unless a law change
/// has eliminated this method.</remarks>
///
/// <returns>True if method is applicable.</returns>
///
/// <param name="workerData">Worker basic data.</param>
/// <param name="piaData">Pia calculation data.</param>
/// <param name="lawChange">Law-change parameters.</param>
/// <param name="entitDate">Entitlement date.</param>
bool WageIndLC::isApplicable( const WorkerDataGeneral& workerData,
const PiaData& piaData, const LawChangeArray& lawChange,
const DateMoyr& entitDate)
{
  LawChange *lawChangePRE1977LAW =
    lawChange.lawChange[LawChange::PRE1977LAW];
  if (lawChangePRE1977LAW->isEffective(entitDate.getYear(),
    workerData.getBenefitDate().getYear())) {
    return(false);
  }
  return WageInd::isApplicable(workerData, piaData);
}
