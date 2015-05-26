// Functions for the <see cref="PiaTableLC"/> class to manage Pia Table pia
// calculations, with changes from present law.

// $Id: PiaTableLC.cpp 1.27 2011/08/08 08:45:13EDT 044579 Development  $

#include "PiaTableLC.h"
#include "LawChangeArray.h"

/// <summary>Initializes a PiaTableLC instance.</summary>
///
/// <param name="newWorkerData">Worker basic data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
/// <param name="newLawChange">Law change parameters.</param>
/// <param name="entitDate">Entitlement date of benefit.</param>
PiaTableLC::PiaTableLC( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear,
const LawChangeArray& newLawChange, const DateMoyr& entitDate ) :
PiaTable(newWorkerData, newPiaData, newPiaParams, newMaxyear),
lawChange(newLawChange), entDate(entitDate)
{ }

/// <summary>Destructor.</summary>
PiaTableLC::~PiaTableLC()
{ }

/// <summary>Determines applicability of method with no arguments.</summary>
///
/// <remarks>This version with no arguments calls the static
/// version with 5 arguments.</remarks>
///
/// <returns>True if method is applicable.</returns>
bool PiaTableLC::isApplicable()
{
  if (isApplicable(workerData, piaData, getIoasdi(), lawChange, entDate)) {
    setApplicable(APPLICABLE);
    return(true);
  }
  return(false);
}

/// <summary>Determines applicability of method with 5 arguments.</summary>
///
/// <returns>True if method is applicable.</returns>
///
/// <param name="workerData">Worker basic data.</param>
/// <param name="piaData">Pia calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
/// <param name="lawChange">Law change parameters.</param>
/// <param name="entitDate">Entitlement date.</param>
bool PiaTableLC::isApplicable( const WorkerDataGeneral& workerData,
const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi,
const LawChangeArray& lawChange, const DateMoyr& entitDate )
{
  LawChange *lawChangePRE1977LAW =
    lawChange.lawChange[LawChange::PRE1977LAW];
  // do old-law calculation if PRE1977LAW lawchange is set
  if (lawChangePRE1977LAW->isEffective(entitDate.getYear(),
    workerData.getBenefitDate().getYear())) {
    return(true);
  }
  return PiaTable::isApplicable(workerData, piaData, ioasdi);
}
