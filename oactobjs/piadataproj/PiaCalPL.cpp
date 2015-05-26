// Functions for the <see cref="PiaCalPL"/> class to manage present law
// pia calculation functions.
//
// $Id: PiaCalPL.cpp 1.56 2013/10/29 13:12:47EDT 277133 Development  $

#include <utility>  // for rel_ops
#include "PiaCalPL.h"
#include "WageInd.h"
#include "FrozMin.h"
#include "SpecMin.h"
#include "PiaTable.h"
#include "TransGuar.h"
#include "ReindWid.h"
#include "OldStart.h"
#include "DibGuar.h"
#include "ChildCareCalc.h"
#include "WageIndNonFreeze.h"
#include "PiaException.h"
#include "UserAssumptions.h"
#include "SecondaryArray.h"
#include "piaparms.h"
#include "DebugCase.h"
#if defined(DEBUGCASE)
#include "Trace.h"
#endif

using namespace std;
#if !defined(__SGI_STL_INTERNAL_RELOPS)
using namespace std::rel_ops;
#endif

/// <summary>Initializes this instance.</summary>
///
/// <param name="newWorkerData">Worker's basic data.</param>
/// <param name="newPiaData">Worker's calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
PiaCalPL::PiaCalPL( WorkerDataGeneral& newWorkerData, PiaData& newPiaData,
const PiaParams& newPiaParams ) :
PiaCal(newWorkerData, newPiaData, newPiaParams)
{ }

/// <summary>Destructor.</summary>
///
/// <remarks>Calls <see cref="releaseMemory"/>.</remarks>
PiaCalPL::~PiaCalPL()
{
  releaseMemory();
}

/// <summary>Calculates PIA and MFB, not including re-indexed widow(er).
/// </summary>
///
/// <param name="entDate">The date of entitlement.</param>
void PiaCalPL::piaCal( const DateMoyr& entDate )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": At top of PiaCalPL::piaCal");
  }
#endif
  piaData.initialize();
  piaData.earn50Cal(workerData);
  setAmend90(entDate);
  // release any memory from a prior case
  releaseMemory();
  // get new memory
  int maxYear = piaParams.getMaxyear();
  if (OldStart::isApplicable(piaData)) {
    PiaMethod* method = new OldStart(workerData, piaData, piaParams, maxYear,
      entDate);
    // find old-start n
    nelapsedCal(piaData.compPeriodOld, entDate);
    nCal(piaData.compPeriodOld, entDate);
    piaMethod.push_back(method);
    oldStart = dynamic_cast<OldStart *>(method);
  }
  if (PiaTable::isApplicable(workerData, piaData, getIoasdi())) {
    PiaMethod* method = new PiaTable(workerData, piaData, piaParams,
      maxYear);
    piaMethod.push_back(method);
    piaTable = dynamic_cast<PiaTable *>(method);
  }
  if (WageInd::isApplicable(workerData, piaData)) {
    PiaMethod* method = new WageInd(workerData, piaData, piaParams, maxYear);
    piaMethod.push_back(method);
    wageInd = dynamic_cast<WageInd *>(method);
  }
  if (TransGuar::isApplicable(workerData, piaData, getIoasdi())) {
    PiaMethod* method = new TransGuar(workerData, piaData, piaParams,
      maxYear);
    piaMethod.push_back(method);
    transGuar = dynamic_cast<TransGuar *>(method);
  }
  if (SpecMin::isApplicable(workerData)) {
    PiaMethod* method = new SpecMin(workerData, piaData, piaParams, maxYear);
    piaMethod.push_back(method);
    specMin = dynamic_cast<SpecMin *>(method);
  }
  if (FrozMin::isApplicable(workerData, piaData)) {
    PiaMethod* method = new FrozMin(workerData, piaData, piaParams, maxYear);
    piaMethod.push_back(method);
    frozMin = dynamic_cast<FrozMin *>(method);
  }
  if (ChildCareCalc::isApplicable(workerData, piaData, getIoasdi())) {
    PiaMethod* method = new ChildCareCalc(workerData, piaData, piaParams,
      maxYear);
    piaMethod.push_back(method);
    childCareCalc = dynamic_cast<ChildCareCalc *>(method);
  }
  if (DibGuar::isApplicable(workerData, piaData, getIoasdi())) {
    PiaMethod* method = new DibGuar(workerData, piaData, piaParams, maxYear);
    piaMethod.push_back(method);
    dibGuar = dynamic_cast<DibGuar *>(method);
  }
  if (WageIndNonFreeze::isApplicable( workerData, piaData)) {
    PiaMethod* method = new WageIndNonFreeze(workerData, piaData, piaParams, maxYear);
    piaMethod.push_back(method);
    wageIndNonFreeze = dynamic_cast<WageIndNonFreeze *>(method);
  }
  for (vector< PiaMethod * >::iterator iter = piaMethod.begin();
    iter != piaMethod.end(); iter++) {
    (*iter)->setApplicable(PiaMethod::APPLICABLE);
    (*iter)->setIoasdi(getIoasdi());
    (*iter)->setPebsAssumptions(isPebsAssumptions());
    (*iter)->calculate();
  }
  resetAmend90();
  piaCal1();
  piaCal2();
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from PiaCalPL::piaCal");
  }
#endif
}

/// <summary>Calculates PIA and benefit.</summary>
///
/// <param name="entDate">The date of entitlement.</param>
void PiaCalPL::calculate2( const DateMoyr& entDate )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": At top of PiaCalPL::calculate2");
  }
#endif
  PiaCal::calculate2(entDate);
  // calculate benefit
  piaCal(entDate);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from PiaCalPL::calculate2");
  }
#endif
}

/// <summary>Calculates PIA for one re-indexed widow(er).</summary>
///
/// <param name="widowData">Widow basic data.</param>
/// <param name="widowPiaData">Computed information for widow.</param>
/// <param name="secondary">Benefits for widow.</param>
void PiaCalPL::reindWidCal( const WorkerDataGeneral& widowData,
const PiaData& widowPiaData, Secondary& secondary )
{
  if (ReindWid::isApplicable(workerData, piaData, widowPiaData.getEligYear(),
    secondary)) {
    ReindWid *reindWid = new ReindWid(workerData, piaData, piaParams,
      piaParams.getMaxyear(), widowPiaData, widowData, secondary);
    widowMethod.push_back(reindWid);
    reindWid->setApplicable(PiaMethod::APPLICABLE);
    reindWid->setIoasdi(getIoasdi());
    reindWid->setPebsAssumptions(isPebsAssumptions());
    reindWid->calculate();
  }
}
