// Functions for the <see cref="SpecMinLC"/> class to manage special minimum
// pia calculations, including changes from present law.
//
// $Id: SpecMinLC.cpp 1.19 2011/08/09 15:55:29EDT 044579 Development  $

#include "SpecMinLC.h"

using namespace std;

/// <summary>Initializes all fields.</summary>
///
/// <param name="newWorkerData">Worker basic data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
/// <param name="newLawChange">Law-change parameters.</param>
SpecMinLC::SpecMinLC( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams,
int newMaxyear, const LawChangeArray& newLawChange ) :
SpecMin(newWorkerData, newPiaData, newPiaParams, newMaxyear),
lawChange(newLawChange)
{ }

/// <summary>Destructor.</summary>
SpecMinLC::~SpecMinLC()
{ }

/// <summary>Limits usable years of coverage.</summary>
///
/// <returns>Usable years of coverage (in excess of 10, up to 20, or however
/// changed by law change).</returns>
int SpecMinLC::specMinYearsExcessCal() const
{
  return(min(max(getYearsTotal() - 10, 0), piaParams.specMinMaxYears(
    piaData.getEligDate().getYear(),
    workerData.getBenefitDate().getYear())));
}

/// <summary>Determines applicability of method using stored data.</summary>
///
/// <remarks>This version calls the static version with 2 arguments.</remarks>
///
/// <returns>True if method is applicable.</returns>
bool SpecMinLC::isApplicable()
{
  if (isApplicable(workerData, lawChange)) {
    setApplicable(APPLICABLE);
    return(true);
  }
  else
    return(false);
}

/// <summary>Determines applicability of method using passed data.</summary>
///
/// <remarks>This version simply returns the applicability determined under
/// present law.</remarks>
///
/// <returns>True if method is applicable.</returns>
///
/// <param name="workerData">Worker basic data.</param>
/// <param name="LawChangeArray">Law Change parameters.</param>
bool SpecMinLC::isApplicable( const WorkerDataGeneral& workerData,
const LawChangeArray& )
{
  return SpecMin::isApplicable(workerData);
}
