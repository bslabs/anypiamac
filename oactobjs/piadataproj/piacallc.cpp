// Functions for the <see cref="PiaCalLC"/> class to manage law-change pia
// calculation functions.
//
// $Id: piacallc.cpp 1.107 2013/10/29 13:12:46EDT 277133 Development  $

#include <algorithm>  // for sort
#include <functional> // for greater()
#include <utility>  // for rel_ops and pair
#include "PiaCalLC.h"
#include "LawChangeAGE65COMP.h"
#include "LawChangeCHILDCARECREDIT.h"
#include "OldStartLC.h"
#include "PiaTableLC.h"
#include "WageIndLC.h"
#include "ReindWidLC.h"
#include "TransGuarLC.h"
#include "SpecMinLC.h"
#include "FrozMin.h"
#include "DibGuarLC.h"
#include "ChildCareCalcLC.h"
#include "WageIndNonFreeze.h"
#include "SecondaryArray.h"
#include "WageBase.h"
#include "LawChangeArray.h"
#include "oactcnst.h"
#include "DebugCase.h"
#if defined(DEBUGCASE) || !defined(NDEBUG)
#include <sstream>
#include "Trace.h"
#endif

using namespace std;
#if !defined(__SGI_STL_INTERNAL_RELOPS)
using namespace std::rel_ops;
#endif

/// <summary>Initializes a PiaCalLC instance.</summary>
///
/// <param name="newWorkerData">Worker's basic data.</param>
/// <param name="newPiaData">Worker's calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newLawChange">Law change parameters.</param>
PiaCalLC::PiaCalLC( WorkerDataGeneral& newWorkerData, PiaData& newPiaData,
const PiaParams& newPiaParams, const LawChangeArray& newLawChange ) :
PiaCal(newWorkerData, newPiaData, newPiaParams), lawChange(newLawChange)
{ }

/// <summary>Destructor.</summary>
///
/// <remarks>Calls <see cref="releaseMemory"/>.</remarks>
PiaCalLC::~PiaCalLC()
{
  releaseMemory();
}

/// <summary>Returns ending elapsed year for a beneficiary case.</summary>
///
/// <returns>Ending elapsed year for a beneficiary case.</returns>
///
/// <param name="entDate">The date of entitlement.</param>
int PiaCalLC::nelapsed2Cal( const DateMoyr& entDate )
{
  int eligYearTemp;
  LawChangeAGE65COMP *lawChangeAGE65COMP = lawChange.lawChangeAGE65COMP;
  // account for age-65 computation point if proposed law
  if (lawChangeAGE65COMP->isEffective(piaData.getEligDate().getYear(),
    workerData.getBenefitDate().getYear())) {
    // phase in the new computation point
    const int i2 = (static_cast<int>(piaData.getEligDate().getYear()) -
      lawChangeAGE65COMP->getStartYear()) /
      lawChangeAGE65COMP->getStep() + 1;
    const int i1 = min(i2, lawChangeAGE65COMP->getInd());
    DateMoyr eligDateTemp =
      eligYearCal1(workerData, piaData, getIoasdi(), i1);
    eligYearTemp = eligYearCal2(workerData, piaData, getIoasdi(),
      eligDateTemp.getYear());
  }
  else
    eligYearTemp = piaData.getEligYear();
  // account for 1960 ending year for 1960 or later method, nondisability
  int elapsed2 = (entDate.getYear() > 1960 && piaData.getEligYear() < 1961 &&
    workerData.getValdi() == 0) ? 1960 : eligYearTemp - 1;
  if (getIoasdi() == WorkerDataGeneral::SURVIVOR &&
    elapsed2 > static_cast<int>(workerData.getDeathDate().year()) - 1) {
    elapsed2 = static_cast<int>(workerData.getDeathDate().year()) - 1;
  }
  return(elapsed2);
}

/// <summary>Returns ending elapsed year for a beneficiary case.</summary>
///
/// <returns>Ending elapsed year for a beneficiary case.</returns>
///
/// <param name="entDate">The date of entitlement.</param>
int PiaCalLC::nelapsed2NonFreezeCal( const DateMoyr& entDate )
{
  int eligYearTemp;
  LawChangeAGE65COMP *lawChangeAGE65COMP = lawChange.lawChangeAGE65COMP;
  // account for age-65 computation point if proposed law
  if (lawChangeAGE65COMP->isEffective(piaData.getEligDate().getYear(),
    workerData.getBenefitDate().getYear())) {
    // phase in the new computation point
    const int i2 = (static_cast<int>(piaData.getEligDate().getYear()) -
      lawChangeAGE65COMP->getStartYear()) /
      lawChangeAGE65COMP->getStep() + 1;
    const int i1 = min(i2, lawChangeAGE65COMP->getInd());
    DateMoyr eligDateTemp =
      eligYearCal1(workerData, piaData, getIoasdi(), i1);
    eligYearTemp = eligYearCal3(workerData, piaData, getIoasdi(),
      eligDateTemp.getYear());
  } else {
    eligYearTemp = piaData.getEligYearNonFreeze();
  }
  // account for 1960 ending year for 1960 or later method, nondisability
  int elapsed2 = (entDate.getYear() > 1960 && piaData.getEligYearNonFreeze() < 1961 &&
    workerData.getValdi() == 0) ? 1960 : eligYearTemp - 1;
  if (getIoasdi() == WorkerDataGeneral::SURVIVOR &&
    elapsed2 > static_cast<int>(workerData.getDeathDate().year()) - 1) {
    elapsed2 = static_cast<int>(workerData.getDeathDate().year()) - 1;
  }
  return(elapsed2);
}

/// <summary>Determine number of computation years and dropout years.
/// </summary>
///
/// <param name="compPeriod">Computation period to use (new or old).</param>
/// <param name="entDate">Entitlement date.</param>
void PiaCalLC::nCal( CompPeriod& compPeriod, const DateMoyr& entDate )
{
  compPeriod.setNDrop(nDropCal(entDate));
  // Apply DI 1-for-5 rule if:
  // old-age case with prior disability entitlement in June 1980 or later
  // that lasted to age 61, or
  // disability case with entitlement in June 1980 or later
  const DisabPeriod& disabPeriod = workerData.disabPeriod[0];
  if ((getIoasdi() == WorkerDataGeneral::OLD_AGE &&
    workerData.getValdi() > 0 &&
    disabPeriod.getEntDate() >= PiaParams::amend80 &&
    disabPeriod.getCessationDate() >=
    DateMoyr(workerData.getBirthDate().month(),
    workerData.getBirthDate().year() + 61)) ||
    (workerData.getEntDate() >= PiaParams::amend80 &&
    getIoasdi() == WorkerDataGeneral::DISABILITY)) {
    // 1-for-5 dropout rule for DI, if not survivor
    compPeriod.setNDrop(min(compPeriod.getNElapsed() / 5, 5));
  }
  LawChange *lawChangeDIDROP5 = lawChange.lawChange[LawChange::DIDROP5];
  if (lawChangeDIDROP5->getInd() > 0 &&
    static_cast< int >(entDate.getYear()) >=
    lawChangeDIDROP5->getStartYear() &&
    piaData.getEligYear() >= lawChangeDIDROP5->getStartYear() - 2) {
    compPeriod.setNDrop(5);
  }
  compPeriod.setN(compPeriod.getNElapsed() - compPeriod.getNDrop());
  if (compPeriod.getN() < 2) {  // nx is minimum of 2
    compPeriod.setN(2);
    compPeriod.setNDrop(compPeriod.getNElapsed() - compPeriod.getN());
  }
  if (getIoasdi() == WorkerDataGeneral::OLD_AGE &&
    workerData.getEntDate().getYear() < 1958 &&
    compPeriod.getBaseYear() == YEAR50) {
    // old-age modification prior to 1958
    compPeriod.setN(piaData.getEligYear() - 1953);
    if (compPeriod.getN() < 2) {
      compPeriod.setN(2);
      compPeriod.setNDrop(compPeriod.getNElapsed() - compPeriod.getN());
    }
  }
  return;
}

/// <summary>Projects earnings and fills in earnings to be used in
/// calculations.</summary>
void PiaCalLC::earnProjection() const
{
  earnProjectionStatic(workerData, piaData, piaParams, lawChange);
}

/// <summary>Adds childcare credits into regular OASDI earnings, using
/// passed arguments rather than member variables.</summary>
///
/// <remarks> Note that this function is static.</remarks>
///
/// <param name="workerData">Basic worker data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="lawChange">Law-change parameters.</param>
void PiaCalLC::childCareEarningsCal( const WorkerDataGeneral& workerData,
PiaData& piaData, const LawChangeArray& lawChange )
{
  LawChangeCHILDCARECREDIT *lawChangeCHILDCARECREDIT =
    lawChange.lawChangeCHILDCARECREDIT;
  if (lawChangeCHILDCARECREDIT->isEffective(piaData.getEligYear(),
    workerData.getBenefitDate().getYear())) {
    const int firstYear = workerData.getIbegin();
    const int lastYear = workerData.getIend();
    for (int yr = firstYear; yr <= lastYear; yr++) {
      if (piaData.childCareOrder.getBit(yr)) {
#if defined(DEBUGCASE)
        if (isDebugPid(workerData.getIdNumber())) {
          ostringstream strm;
          strm.precision(2);
          strm << "Id " << workerData.getIdString()
            << ": In PiaCalLC::childCareEarningsCal: earnings in year " << yr
            << " were " << piaData.earnOasdi[yr]
            << " and are being bumped up to "
            << piaData.getChildCareMaxCredit(yr);
          Trace::writeLine(strm.str());
        }
#endif
        piaData.earnOasdi[yr] = piaData.getChildCareMaxCredit(yr);
      }
    }
  }
}

/// <summary>Adjusts QCs for child care credits, using passed arguments
/// rather than member variables.</summary>
///
/// <remarks> Note that this function is static.</remarks>
///
/// <param name="workerData">Basic worker data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="piaParams">Benefit calculation parameters.</param>
/// <param name="lawChange">Law-change parameters.</param>
void PiaCalLC::childCareQcCal( const WorkerDataGeneral& workerData,
PiaData& piaData, const PiaParams& piaParams,
const LawChangeArray& lawChange )
{
  LawChangeCHILDCARECREDIT *lawChangeCHILDCARECREDIT =
    lawChange.lawChangeCHILDCARECREDIT;
  if (lawChangeCHILDCARECREDIT->isEffective(piaData.getEligYear(),
    workerData.getBenefitDate().getYear())) {
    const int firstYear = workerData.getIbegin();
    // For simplicity, we won't allow QC's to be credited for the current
    // year.
    const int lastYear = workerData.getIend() - 1;
    for (int yr = firstYear; yr <= lastYear; yr++) {
      // The childCareOrder bits should have already been set
      // by a previous call to childCareCreditCal().
      if (piaData.childCareOrder.getBit(yr)) {
        piaData.qcov.set(yr, Qc::qcCal(
          piaData.getChildCareMaxCredit(yr), piaParams.qcamt[yr]));
      }
    }
  }
}

/// <summary>Sets best child care years to use, using passed arguments
/// rather than member variables.</summary>
///
/// <remarks> Note that this function is static.</remarks>
///
/// <param name="workerData">Basic worker data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="piaParams">Benefit calculation parameters.</param>
/// <param name="lawChange">Law-change parameters.</param>
void PiaCalLC::childCareCreditCal( const WorkerDataGeneral& workerData,
PiaData& piaData, const PiaParams& piaParams,
const LawChangeArray& lawChange )
{
  LawChangeCHILDCARECREDIT *lawChangeCHILDCARECREDIT =
    lawChange.lawChangeCHILDCARECREDIT;
  if (lawChangeCHILDCARECREDIT->isEffective(piaData.getEligYear(),
    workerData.getBenefitDate().getYear())) {
    // The maximum credit this person can get for a child care year.
    DoubleAnnual maxCredit(workerData.getIbegin(), workerData.getIend());
    // array of ratios (earnings/maxCredit)
    vector< pair< double, int > > earnRatio;
    int totalYears = 0;  // total number of child care years
    const int startYear =
      lawChangeCHILDCARECREDIT->getChildCareYearsBeforeEffYear() ?
      workerData.getIbegin() :
      max(workerData.getIbegin(), lawChangeCHILDCARECREDIT->getStartYear());
    for (int yr = startYear; yr <= workerData.getIend(); yr++) {
      if (workerData.childCareYears.getBit(yr)) {
        maxCredit[yr] = childCareMaxCreditCal(workerData, piaData.earnOasdi,
          piaParams.getFqArray(), *lawChangeCHILDCARECREDIT, yr);
        if (piaData.earnOasdi[yr] < maxCredit[yr]) {
          earnRatio.push_back(
            make_pair(piaData.earnOasdi[yr] / maxCredit[yr], yr));
          totalYears++;
        }
      }
    }
    // If person could possibly get more child care years than
    // the maximum allowed, then sort for the best ones.
    if (totalYears > lawChangeCHILDCARECREDIT->getMaxYears()) {
      sort(earnRatio.begin(), earnRatio.end());
      // Cap total number of child care years.
      totalYears = lawChangeCHILDCARECREDIT->getMaxYears();
    }
    // Set total number of child care years awarded.
    piaData.setChildCareYearsTotal(totalYears);
    // Take lowest ratios, and indicate their years with
    // booleans in childCareOrder.
    for (int i = 0; i < totalYears; i++) {
      piaData.childCareOrder.setBit(earnRatio[i].second, true);
      piaData.childCareMaxCredit.push_back(
        make_pair(maxCredit[earnRatio[i].second], earnRatio[i].second));
    }
  }
}

/// <summary>Calculates the maximum child care credit.</summary>
///
/// <returns>The maximum child care credit.</returns>
///
/// <remarks> Note that this function is static.</remarks>
///
/// <param name="workerData">Basic worker data.</param>
/// <param name="earnOasdi">OASDI covered earnings.</param>
/// <param name="awi">Average wage indexing array.</param>
/// <param name="lawChange">Law-change parameters for CHILDCARECREDIT.</param>
/// <param name="year">The year being considered.</param>
double PiaCalLC::childCareMaxCreditCal( const WorkerDataGeneral& workerData,
const DoubleAnnual& earnOasdi, const AverageWage& awi,
const LawChangeCHILDCARECREDIT& lawChange, int year )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": At top of PiaCalLC::childCareMaxCreditCal");
  }
#endif
#if !defined(NDEBUG)
  if (workerData.getIbegin() < earnOasdi.getBaseYear()) {
    ostringstream strm;
    strm << "In PiaCalLC::childCareMaxCreditCal, ibegin can not be less "
      << "than baseYear: ibegin " << workerData.getIbegin() << ", baseYear "
      << earnOasdi.getBaseYear();
    throw PiaException(strm.str());
  }
  if (year > earnOasdi.getLastYear()) {
    ostringstream strm;
    strm << "In PiaCalLC::childCareMaxCreditCal, year can not be more "
      << "than lastYear: year " << year << ", lastYear "
      << earnOasdi.getLastYear();
    throw PiaException(strm.str());
  }
#endif
  double maxCredit;
  if (lawChange.getInd() <= 2) {
    // For ind == 1 or 2, the maxCredit is a specified percentage of the AWI.
    maxCredit = awi[year] * lawChange.getFqRatio();
  }
  else {
    // For ind == 3 or 4, the maxCredit is a specified percentage of the
    // average of some of the highest years of (indexed) earnings for the
    // worker.
    // Array of indexed earnings.
    vector<double> earnInd;
    for (int i = workerData.getIbegin(); i < year; i++) {
      earnInd.push_back(earnOasdi[i] * awi[year] / awi[i]);
    }
    // Sort the earnInd from highest to lowest.
    sort(earnInd.begin(), earnInd.end(), greater<double>());
    // Number of years to drop off the beginning of the numHigh years.
    const int numToDrop = lawChange.getNumDropExtremeEarnYears();
    // Number of highest earning years to use.
    const int numHigh = lawChange.getNumTopEarnYears() - numToDrop;
    // Total earnings for the high years we're using.
    double totalEarn = 0.0;
    for (int i = 0 + numToDrop; i < min(numHigh, (int)earnInd.size()); i++) {
      totalEarn += earnInd[i];
    }
    // Average the earnings.
    const double avgEarn = totalEarn / (numHigh - numToDrop);
    maxCredit = avgEarn * lawChange.getAvgEarnPct();
  }
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm.precision(2);
    strm << workerData.getIdString()
      << ": At bottom of PiaCalLC::childCareMaxCreditCal, earnings:";
    for (int i = workerData.getIbegin(); i < year; i++) {
      strm << " " << earnOasdi[i];
    }
    strm << ", maxCredit " << maxCredit << " for " << year;
    Trace::writeLine(strm.str());
  }
#endif
  return maxCredit;
}

/// <summary>Projects earnings and fills in earnings to be used in
/// calculations, using passed arguments rather than member variables.
/// </summary>
///
/// <remarks>Note that this function is static.</remarks>
///
/// <param name="workerData">Basic worker data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="piaParams">Benefit calculation parameters.</param>
/// <param name="lawChange">Law-change parameters.</param>
void PiaCalLC::earnProjectionStatic( const WorkerDataGeneral& workerData,
PiaData& piaData, const PiaParams& piaParams,
const LawChangeArray& lawChange )
{
  piaData.earnProjection(workerData);
  // add in child care credits, if any
  childCareEarningsCal(workerData, piaData, lawChange);
  piaData.earnHiCal(workerData);
  // limit earnings to wage base
  piaParams.getBaseOasdiArray().earnLimit(piaData.earnOasdiLimited,
    piaData.earnOasdi);
  piaParams.getBaseHiArray().earnLimit(piaData.earnHiLimited,
    piaData.earnHi);
}

/// <summary>Calculates PIA and MFB, not including re-indexed widow(er).
/// </summary>
///
/// <param name="entDate">The date of entitlement.</param>
void PiaCalLC::piaCal( const DateMoyr& entDate )
{
  try {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() +
         ": At top of PiaCalLC::piaCal");
    }
#endif
    piaData.initialize();
    piaData.earn50Cal(workerData);
    setAmend90(entDate);
    // release any memory from a prior case
    releaseMemory();
    // get new memory
    const int maxYear = piaParams.getMaxyear();
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm << workerData.getIdString() << ", piaData.getQcTotal50() = "
        << piaData.getQcTotal50()
        << ", piaData.getBirthDateMinus1().getYear() = "
        << piaData.getBirthDateMinus1().year()
        << ": Calling OldStartLC::isApplicable";
      Trace::writeLine(strm.str());
    }
#endif
    if (OldStartLC::isApplicable(entDate, workerData.getBenefitDate(),
      piaData, lawChange)) {
      PiaMethod* method = new OldStartLC(workerData, piaData, piaParams,
        maxYear, entDate, lawChange);
      // find old-start n
      nelapsedCal(piaData.compPeriodOld, entDate);
      nCal(piaData.compPeriodOld, entDate);
      piaMethod.push_back(method);
      oldStart = dynamic_cast<OldStart *>(method);
    }
    if (PiaTableLC::isApplicable(workerData, piaData, getIoasdi(), lawChange,
      entDate)) {
      PiaMethod* method = new PiaTableLC(workerData, piaData, piaParams,
        maxYear, lawChange, entDate);
      piaMethod.push_back(method);
      piaTable = dynamic_cast<PiaTable *>(method);
    }
    if (WageIndLC::isApplicable(workerData, piaData, lawChange, entDate)) {
      PiaMethod* method = new WageIndLC(workerData, piaData, piaParams,
        maxYear, lawChange, entDate);
      piaMethod.push_back(method);
      wageInd = dynamic_cast<WageInd *>(method);
    }
    if (TransGuarLC::isApplicable(workerData, piaData, getIoasdi(),
      lawChange)) {
      PiaMethod* method = new TransGuarLC(workerData, piaData, piaParams,
        maxYear, lawChange);
      piaMethod.push_back(method);
      transGuar = dynamic_cast<TransGuar *>(method);
    }
    if (SpecMinLC::isApplicable(workerData, lawChange)) {
      PiaMethod* method = new SpecMinLC(workerData, piaData, piaParams,
        maxYear, lawChange);
      piaMethod.push_back(method);
      specMin = dynamic_cast<SpecMin *>(method);
    }
    if (FrozMin::isApplicable(workerData, piaData)) {
      PiaMethod* method = new FrozMin(workerData, piaData, piaParams,
        maxYear);
      piaMethod.push_back(method);
      frozMin = dynamic_cast<FrozMin *>(method);
    }
    if (ChildCareCalcLC::isApplicable(workerData, piaData, getIoasdi(),
      lawChange)) {
      PiaMethod* method = new ChildCareCalcLC(workerData, piaData,
        piaParams, maxYear, lawChange);
      piaMethod.push_back(method);
      childCareCalc = dynamic_cast<ChildCareCalc *>(method);
    }
    if (DibGuarLC::isApplicable(entDate, workerData, piaData, getIoasdi(),
      lawChange)) {
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
        ": Returning from PiaCalLC::piaCal");
    }
#endif
  } catch (exception& e) {
    throw PiaException(string("In PiaCalLC::piaCal: ") + e.what());
  }
}

/// <summary>Calculates PIA and benefit.</summary>
///
/// <param name="entDate">The date of entitlement.</param>
void PiaCalLC::calculate2( const DateMoyr& entDate )
{
  try {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() +
        ": At top of PiaCalLC::calculate2");
    }
#endif
    PiaCal::calculate2(entDate);
    // calculate benefit
    piaCal(entDate);
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() +
        ": Returning from PiaCalLC::calculate2");
    }
#endif
  } catch (exception& e) {
    throw PiaException(string("In PiaCalLC::calculate2: ") + e.what());
  }
}

/// <summary>Calculates PIA for one re-indexed widow(er).</summary>
///
/// <param name="widowData">Widow(er) basic data.</param>
/// <param name="widowPiaData">Calculation information for widow(er).</param>
/// <param name="secondary">Benefits for widow(er).</param>
void PiaCalLC::reindWidCal( const WorkerDataGeneral& widowData,
const PiaData& widowPiaData, Secondary& secondary )
{
  if (ReindWidLC::isApplicable(workerData, piaData,
    widowPiaData.getEligYear(), secondary, lawChange)) {
    ReindWid *reindWid = new ReindWidLC(workerData, piaData, piaParams,
      piaParams.getMaxyear(), widowPiaData, widowData, secondary, lawChange);
    widowMethod.push_back(reindWid);
    reindWid->setApplicable(PiaMethod::APPLICABLE);
    reindWid->setIoasdi(getIoasdi());
    reindWid->setPebsAssumptions(isPebsAssumptions());
    reindWid->calculate();
  }
}
