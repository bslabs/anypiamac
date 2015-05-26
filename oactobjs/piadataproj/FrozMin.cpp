// Functions for the <see cref="FrozMin"/> class to manage frozen minimum
// pia calculations.
//
// $Id: FrozMin.cpp 1.35 2011/07/29 15:26:16EDT 044579 Development  $

#include "FrozMin.h"
#include "DebugCase.h"
#if defined(DEBUGCASE)
#include "Trace.h"
#endif

using namespace std;

/// <summary>Initializes a frozen minimum method.</summary>
///
/// <remarks>Calls <see cref="PiaMethod::initialize"/>.</remarks>
///
/// <param name="newWorkerData">Worker data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
FrozMin::FrozMin( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear ) :
PiaMethod(newWorkerData, newPiaData, newPiaParams, newMaxyear,
"Frozen Minimum (1977 Act)", FROZ_MIN)
{
  initialize();
}

/// <summary>Destructor.</summary>
FrozMin::~FrozMin()
{ }

/// <summary>Determines applicability of method using stored values.</summary>
///
/// <remarks>This version with no arguments calls the static version with 2
/// arguments. The year of eligibility must be between 1979 and 1981,
/// inclusive, and it must not be a totalization benefit.</remarks>
///
/// <returns>True if method is applicable, false otherwise.</returns>
bool FrozMin::isApplicable()
{
  if (isApplicable(workerData, piaData)) {
    setApplicable(APPLICABLE);
    return(true);
  }
  else {
    return(false);
  }
}

/// <summary>Determines applicability of method using passed values.</summary>
///
/// <remarks>The year of eligibility must be between 1979 and 1981,
/// inclusive, and it must not be a totalization benefit.</remarks>
///
/// <returns>True if method is applicable, false otherwise.</returns>
///
/// <param name="workerData">Worker data.</param>
/// <param name="piaData">Pia calculation data.</param>
bool FrozMin::isApplicable( const WorkerDataGeneral& workerData,
const PiaData& piaData )
{
  return (piaData.getEligYear() > 1978 && piaData.getEligYear() < 1982 &&
    !workerData.getTotalize());
}

/// <summary>Computes frozen minimum PIA.</summary>
void FrozMin::calculate()
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Starting FrozMin::calculate");
  }
#endif
  setFrozMin();
  // apply cpi increases beginning with earlier of year of entitlement.
  // or year of age 65
  setYearCpi();
  piaEnt.set(applyColasElig(piaElig, yearCpi[YEAR_ELIG],
    workerData.getBenefitDate(), piaData.getEligYear()));
  mfbEnt.set(applyColasElig(mfbElig, yearCpi[YEAR_ELIG],
    workerData.getBenefitDate(), piaData.getEligYear()));
  setDirty();
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from FrozMin::calculate");
  }
#endif
}

/// <summary>Sets amounts for frozen minimum before benefit increases.
/// </summary>
void FrozMin::setFrozMin()
{
  // do not use date of entitlement in a survivor case
  const int year = (getIoasdi() == WorkerDataGeneral::SURVIVOR) ?
    (int)workerData.getDeathDate().year() :
    (int)workerData.getEntDate().getYear();
  int yearCpiTemp = min(year, piaData.getBirthDateMinus1().year() + 65);
  yearCpi[YEAR_ELIG] = yearCpiTemp;
  yearCpiTemp--;
  yearCpi[FIRST_YEAR] = yearCpiTemp;
  piaElig[yearCpiTemp] = 122.0;
  mfbElig[yearCpiTemp] = 183.0;
}
