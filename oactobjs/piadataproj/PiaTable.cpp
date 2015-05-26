// Functions for the <see cref="PiaTable"/> class to manage Pia Table pia
// calculations.

// $Id: PiaTable.cpp 1.45 2011/08/09 14:59:55EDT 044579 Development  $

#include <fstream>
#include <utility>  // for rel_ops
#include "PiaTable.h"
#include "oactcnst.h"
#include "PiaException.h"
#include "piaparms.h"
#include "DebugCase.h"
#if defined(DEBUGCASE)
#include <sstream>
#include "Trace.h"
#endif

using namespace std;
#if !defined(__SGI_STL_INTERNAL_RELOPS)
using namespace std::rel_ops;
#endif

/// <summary>Initializes this instance.</summary>
///
/// <remarks>Calls <see cref="initialize"/>.</remarks>
///
/// <param name="newWorkerData">Worker basic data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
PiaTable::PiaTable( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear ) :
OldPia(newWorkerData, newPiaData, newPiaParams, newMaxyear,
"New-Start Calculation (pre-1977 Act)", PIA_TABLE)
{
  initialize();
}

/// <summary>Destructor.</summary>
PiaTable::~PiaTable()
{ }

/// <summary>Determines applicability of method, using stored values.</summary>
///
/// <remarks>This version calls the static version with 3 arguments.</remarks>
///
/// <returns>True if method is applicable.</returns>
bool PiaTable::isApplicable()
{
  if (isApplicable(workerData, piaData, getIoasdi())) {
    setApplicable(APPLICABLE);
    return(true);
  }
  return(false);
}

/// <summary>Determines applicability of method, using passed values.</summary>
///
/// <returns>True if method is applicable.</returns>
///
/// <param name="workerData">Worker basic data.</param>
/// <param name="piaData">Pia calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
bool PiaTable::isApplicable( const WorkerDataGeneral& workerData,
const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi )
{
  return (piaData.getEligYear() < YEAR79 &&
    workerData.getBenefitDate() >= PiaParams::amend52 &&
    (ioasdi != WorkerDataGeneral::SURVIVOR ||
    DateMoyr(workerData.getDeathDate()) >= PiaParams::amend52) );
}

/// <summary>Computes Pia Table PIA.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PIATABLE1"/> if year of entitlement is out of range
/// (only in debug mode); of type <see cref="PIA_IDS_PIATABLE2"/> if year of
/// benefit is out of range (only in debug mode).</exception>
void PiaTable::calculate()
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Starting PiaTable::calculate");
  }
#endif
  DateMoyr dateMoyr = (getIoasdi() == WorkerDataGeneral::SURVIVOR) ?
    DateMoyr(workerData.getDeathDate()) : workerData.getEntDate();
#if !defined(NDEBUG)
  if (dateMoyr.getYear() < YEAR37)
    throw PiaException(PIA_IDS_PIATABLE1);
#endif
  int i1 = dateMoyr.getYear();
  yearCpi[YEAR_ENT] = (i1 < YEAR51) ? i1 :
    (static_cast<int>(dateMoyr.getMonth()) >=
    piaParams.getMonthBeninc(i1)) ? i1 : i1 - 1;
#if !defined(NDEBUG)
  if (workerData.getBenefitDate().getYear() < YEAR37)
    throw PiaException(PIA_IDS_PIATABLE2);
#endif
  i1 = workerData.getBenefitDate().getYear();
  yearCpi[YEAR_BEN] =
    (static_cast<int>(workerData.getBenefitDate().getMonth()) >=
    piaParams.getMonthBeninc(i1)) ? i1 : i1 - 1;
  yearCpi[FIRST_YEAR] = 1975;
  const AverageWage& earnings = workerData.getTotalize() ?
    piaData.earnTotalizedLimited : piaData.earnOasdiLimited;
  i1 = piaData.getEarn50(PiaData::EARN_WITH_TOTALIZATION);
  const int i4 = piaData.getEarnYear();
  for (int i2 = i1; i2 <= i4; i2++) {
    if (!piaData.freezeYears.isFreezeYear(i2))
      earnIndexed[i2] = earnings[i2];
  }
  const int i3 = piaData.compPeriodNew.getN();
  orderEarnings(i1, i4, i3);
  totalEarnCal(i1, i4, i3);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", ame " << getAme()
      << ": After totalEarnCal";
    Trace::writeLine(strm.str());
  }
#endif
  if (workerData.getBenefitDate() < PiaParams::amend742) {
    setTableNum(oldPiaCal());
  }
  else {
    setTableNum(cpiBase(workerData.getBenefitDate(), false, getAme(),
      !workerData.getTotalize()));
  }
  piaEnt.set(piasub);
  mfbEnt.set(mfbsub);
  if (workerData.getTotalize()) {
    piaElig[yearCpi[FIRST_YEAR]] = piaEnt.get();
    prorate();
    piaEnt.set(piaElig[yearCpi[FIRST_YEAR]]);
    static_cast<void>(mfbOldCal(true));
  }
  setDirty();
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from PiaTable::calculate");
  }
#endif
}
