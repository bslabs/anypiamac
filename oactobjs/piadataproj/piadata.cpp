// Functions for the <see cref="PiaData"/> class to set up data to
// calculate and print out a PIA.

// $Id: piadata.cpp 1.148 2014/01/05 07:55:16EST 277133 Development  $

#include <utility>  // for rel_ops
#include "piadata.h"
#include "piacal.h"
#include "oactcnst.h"
#include "qc.h"
#include "PiaException.h"
#include "Resource.h"
#include "DebugCase.h"
#if defined(DEBUGCASE)
#include <sstream>
#include "DateFormatter.h"
#include "Trace.h"
#include "comma.h"
#endif

using namespace std;
#if !defined(__SGI_STL_INTERNAL_RELOPS)
using namespace std::rel_ops;
#endif

// <summary>Starting point of annual qc's, if using sum of qc's
// through 1950.</summary>
const QtrYear PiaData::qtr151 = QtrYear(0, 1951);
// <summary>Starting point of annual qc's, if using sum of qc's
// through 1977.</summary>
const QtrYear PiaData::qtr178 = QtrYear(0, 1978);
// <summary>The earliest possible retirement date.</summary>
const DateMoyr PiaData::jan1937 = DateMoyr(1, 1937);

/// <summary>Initializes arrays.</summary>
PiaData::PiaData() : ageBen(), ageEnt(), unroundedBenefit(),
childCareOrder(YEAR37, WorkerDataGeneral::getMaxyear()),
childCareMaxCredit(),compPeriodNew(YEAR50),compPeriodNewNonFreeze(YEAR50),
compPeriodOld(YEAR36), disInsCode(), earlyRetAge(),
earnMilServ(YEAR37, WorkerDataGeneral::getMaxyear()),
earnHi(YEAR37, WorkerDataGeneral::getMaxyear()),
earnHiLimited(YEAR37, WorkerDataGeneral::getMaxyear()),
earnOasdi(YEAR37, WorkerDataGeneral::getMaxyear()),
earnOasdiLimited(YEAR37, WorkerDataGeneral::getMaxyear()),
earnTotalized(YEAR37, WorkerDataGeneral::getMaxyear()),
earnTotalizedLimited(YEAR37, WorkerDataGeneral::getMaxyear()),
finsCode(), fullRetAge(),
freezeYears(), highMfb(), highPia(), partialFreezeYears(),
qcov(YEAR37, WorkerDataGeneral::getMaxyear()), qcovMilServ(YEAR37, 1956),
relEarnPosition(YEAR37, WorkerDataGeneral::getMaxyear()), roundedBenefit(),
supportPia(), amend90(false), arf(0.0), arfApp(NO_SPEC_MIN_DRC),
childCareYearsTotal(0), deemedQcReq(0), deemedQctot(0), doEarlyQcs(true),
earn3750ms(0.0), earnYear(0), eligDate(), finsCode2(' '), fullInsDate(),
fullRetDate(), iappn(0), iapps(0), ibeginAll(0), ibeginTotal(0), ielgyr(0),
ielgyrnf(0), iendAll(0), iendTotal(0), kbirth(), monthsArdri(0), 
overMax(false), pifc(' '), qc3750ms(0), qc3750simp(0), qcCurrent(0), 
qcDisDate1(), qcDisDate2(), qcDisDate3(), qcDisDate4(), qcDisDate5(), 
qcDisDate6(), qcDisQtr(0), qcDisQtr2(0),qcDisReq(0), qcDisYears(0), 
qcReq(0), qcReqPerm(0), qcTotal(0), qcTotal50(0), qcTotal51(0), 
qcTotalDis(0), qcTotalRel(0), relEarnPositionAverage(0.0)
{ }

/// <summary>Destroys arrays.</summary>
PiaData::~PiaData( )
{ }

/// <summary>Sets this instance equal to another instance.</summary>
///
/// <returns>A reference to this instance.</returns>
///
/// <param name="newData">The instance to copy from.</param>
PiaData& PiaData::operator=( const PiaData& newData )
{
  ageBen = newData.ageBen;
  ageEnt = newData.ageEnt;
  unroundedBenefit = newData.unroundedBenefit;
  childCareOrder = newData.childCareOrder;
  childCareMaxCredit = newData.childCareMaxCredit;
  compPeriodNew = newData.compPeriodNew;
  compPeriodNewNonFreeze = newData.compPeriodNewNonFreeze;
  compPeriodOld = newData.compPeriodOld;
  disInsCode = newData.disInsCode;
  earlyRetAge = newData.earlyRetAge;
  const DoubleAnnual& da1 = newData.earnMilServ;
  earnMilServ.assign(da1, da1.getBaseYear(), da1.getLastYear());
  earnHi = newData.earnHi;
  earnHiLimited = newData.earnHiLimited;
  earnOasdi = newData.earnOasdi;
  earnOasdiLimited = newData.earnOasdiLimited;
  earnTotalized = newData.earnTotalized;
  earnTotalizedLimited = newData.earnTotalizedLimited;
  finsCode = newData.finsCode;
  fullRetAge = newData.fullRetAge;
  freezeYears = newData.freezeYears;
  highMfb = newData.highMfb;
  highPia = newData.highPia;
  partialFreezeYears = newData.partialFreezeYears;
  qcov = newData.qcov;
  qcovMilServ = newData.qcovMilServ;
  const DoubleAnnual& da2 = newData.relEarnPosition;
  relEarnPosition.assign(da2, da2.getBaseYear(), da2.getLastYear());
  roundedBenefit = newData.roundedBenefit;
  supportPia = newData.supportPia;
  amend90 = newData.amend90;
  arf = newData.arf;
  arfApp = newData.arfApp;
  childCareYearsTotal = newData.childCareYearsTotal;
  deemedQcReq = newData.deemedQcReq;
  deemedQctot = newData.deemedQctot;
  doEarlyQcs = newData.doEarlyQcs;
  earn3750ms = newData.earn3750ms;
  const int *ip1 = newData.earn50;
  copy(ip1, ip1 + NUM_EARN_TOTALIZATION, earn50);
  const double *dp1 = newData.earnTotal50;
  copy(dp1, dp1 + NUM_EARN_TOTALIZATION, earnTotal50);
  earnYear = newData.earnYear;
  eligDate = newData.eligDate;
  finsCode2 = newData.finsCode2;
  fullInsDate = newData.fullInsDate;
  fullRetDate = newData.fullRetDate;
  iappn = newData.iappn;
  iapps = newData.iapps;
  ibeginAll = newData.ibeginAll;
  ibeginTotal = newData.ibeginTotal;
  ielgyr = newData.ielgyr;
  ielgyrnf = newData.ielgyrnf;
  iendAll = newData.iendAll;
  iendTotal = newData.iendTotal;
  kbirth = newData.kbirth;
  monthsArdri = newData.monthsArdri;
  overMax = newData.overMax;
  pifc = newData.pifc;
  qc3750ms = newData.qc3750ms;
  qc3750simp = newData.qc3750simp;
  qcCurrent = newData.qcCurrent;
  qcDisDate1 = newData.qcDisDate1;
  qcDisDate2 = newData.qcDisDate2;
  qcDisDate3 = newData.qcDisDate3;
  qcDisDate4 = newData.qcDisDate4;
  qcDisDate5 = newData.qcDisDate5;
  qcDisDate6 = newData.qcDisDate6;
  qcDisQtr = newData.qcDisQtr;
  qcDisQtr2 = newData.qcDisQtr2;
  qcDisReq = newData.qcDisReq;
  qcDisYears = newData.qcDisYears;
  qcReq = newData.qcReq;
  qcReqPerm = newData.qcReqPerm;
  qcTotal = newData.qcTotal;
  qcTotal50 = newData.qcTotal50;
  qcTotal51 = newData.qcTotal51;
  qcTotalDis = newData.qcTotalDis;
  qcTotalRel = newData.qcTotalRel;
  relEarnPositionAverage = newData.relEarnPositionAverage;
  return (*this);
}

/// <summary>Initializes variables to default values.</summary>
void PiaData::deleteContents()
{
  pifc = ' ';
  iappn = 0;
  ageEnt.deleteContents();
  ageBen.deleteContents();
  fullRetAge.deleteContents();
  earlyRetAge.deleteContents();
  eligDate.deleteContents();
  fullInsDate.deleteContents();
  fullRetDate.deleteContents();
  compPeriodNew.deleteContents();
  compPeriodNewNonFreeze.deleteContents();
  qcov.deleteContents();
  qcovMilServ.deleteContents();
  childCareOrder.deleteContents();
  childCareMaxCredit.clear();
  childCareYearsTotal = 0;
  qc3750simp = qc3750ms = qcTotal = qcTotal50 = 0;
  earn3750ms = earnTotal50[0] = earnTotal50[1] = 0.;
}

/// <summary>Calculates sum of regular pre-1951 earnings.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> if first year
/// of all earnings is out of range (only in debug mode).</exception>
void PiaData::earnTotal50Cal0()
{
#if !defined(NDEBUG)
  WorkerDataGeneral::ibegin2Check(ibeginAll);
#endif
  if (ibeginAll < 1937 || ibeginAll > 1950) {
    earnTotal50[0] = 0.0;
  }
  else {
    earnTotal50[0] = min(earnOasdi.accumulate(ibeginAll, 1950u, 0.0),
      42000.0);
  }
}

/// <summary>Calculates sum of pre-1951 totalization earnings.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> if first year of
/// totalization earnings is out of range (only in debug mode).</exception>
void PiaData::earnTotal50Cal1()
{
#if !defined(NDEBUG)
  WorkerDataGeneral::ibegin2Check(ibeginTotal);
#endif
  if (ibeginTotal < 1937 || ibeginTotal > 1950) {
    earnTotal50[1] = 0.0;
  }
  else {
    earnTotal50[1] = min(earnTotalized.accumulate(ibeginTotal, 1950u, 0.0),
      42000.0);
  }
}

/// <summary>Determines number of required quarters of coverage for DI
/// insured status.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DISREQCAL1"/> if year in most recent
/// freeze period is out of range (only in debug mode); of type
/// <see cref="PIA_IDS_DISREQCAL2"/> if year in next most recent
/// freeze period is out of range (only in debug mode).</exception>
///
/// <param name="workerData">Basic worker data.</param>
/// <param name="date">The date for which required quarters of coverage are
///  desired.</param>
/// <param name="trial">Number of quarters to go beyond onset quarter.</param>
/// <param name="dospecial">Indicator for special insured status calculation.
/// If false, do not do special insured status calculation. If true, do
/// special insured status calculation.</param>
/// <param name="dofreeze">Indicator for assuming period between onset and
/// waiting period in a prior period of disability is a freeze period. If
/// false, do not assume it is a freeze period (excluded period starts with
/// waiting period). If true, assume it is a freeze period (excluded period
/// starts with onset).</param>
void PiaData::qcDisReqCal( const WorkerDataGeneral& workerData,
const DateMoyr& date, int trial, bool dospecial, bool dofreeze )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    stringstream strm;
    strm << workerData.getIdString()
      << ": Entering PiaData::qcDisReqCal for date"
      << DateFormatter::toString(date);
    Trace::writeLine(strm.str());
    strm.str("");
    strm << workerData.getIdString() << ": qcDisDate1 = "
      << qcDisDate1.toString() << ", qcDisDate2 = " << qcDisDate2.toString()
      << ", qcDisDate3 = " << qcDisDate3.toString() << ", qcDisDate4 = "
      << qcDisDate4.toString() << ", qcDisDate5 = " << qcDisDate5.toString()
      << ", qcDisDate6 = " << qcDisDate6.toString();
    Trace::writeLine(strm.str());
  }
#endif
  QtrYear tempQtrYear;  // temporary (quarter,year) date
  if (workerData.getValdi() > 0) {
    qcDisDate2 = workerData.disabPeriod[0].getOnsetDate();
    qcDisDate2.add(trial);
  } else {
    qcDisDate2 = date;
  }
  // set quarter and year after attainment of age 21
  tempQtrYear = getBirthDateMinus1();
  tempQtrYear.add(QtrYear(1,21));
  // calculate number of elapsed quarters
  if (qcDisQtr2 == 40) {
    // only use period after disability
    qcDisQtr = 40;
    qcDisDate1.deleteContents();
    qcDisDate2.deleteContents();
  } else {
    // set beginning quarter and year, based on (40 - qcdiqtr2) quarters
    qcDisDate1 = qcDisDate2;
    qcDisDate1.subtract(39 - qcDisQtr2);
    // check for special insured status if period starts before age 21
    if (dospecial && qcDisDate1 < tempQtrYear) {
      qcDiSpec(qcDisQtr2, 0);
    }
    if (qcDisDate1.getYear() == 0) {
      qcDisQtr = qcDisQtr2;
    } else {
      // check for period beginning before 1937
      if (static_cast<int>(qcDisDate1.getYear()) < YEAR37) {
        qcDisDate1.setQuarter(0);
        qcDisDate1.setYear(YEAR37);
      }
      qcDisQtr = QtrYear::diff(qcDisDate1, qcDisDate2) + 1 + qcDisQtr2;
    }
  }
  // consider period before prior disability
  if (workerData.getValdi() < 2 || qcDisDate1.getYear() == 0 ||
    !(qcDisDate1 < QtrYear(workerData.disabPeriod[1].getCessationDate()))) {
    // do not need period before prior disability
    qcDisDate5.deleteContents();
    qcDisDate6.deleteContents();
  } else {
    // reset beginning quarter after prior disability, starting with quarter
    // of cessation
    qcDisDate1 = workerData.disabPeriod[1].getCessationDate();
    // if cessation quarter is not a qc, go to next quarter
    const int tempqcs = (qcDisDate1.getYear() < qcDisDate2.getYear()) ?
      4 - qcDisDate1.getQuarter() :
      qcDisDate2.getQuarter() - qcDisDate1.getQuarter();
#if !defined(NDEBUG)
    if (qcDisDate1.getYear() < YEAR37)
      throw PiaException(PIA_IDS_DISREQCAL1);
#endif
    if (static_cast<int>(qcov.get(qcDisDate1.getYear())) < tempqcs) {
      qcDisDate1.add(1);
    }
    // check that period is still consistent
    if (qcDisDate1 > qcDisDate2) {
      qcDisDate1.deleteContents();
      qcDisDate2.deleteContents();
    }
    // recalculate qcs elapsed after prior disability
    if (qcDisDate1.getYear() == 0) {
      qcDisQtr = qcDisQtr2;
    } else {
      qcDisQtr = QtrYear::diff(qcDisDate1, qcDisDate2) + 1 + qcDisQtr2;
    }
    // set ending quarter and year
    // if first quarter is a qc, use it
    qcDisDate6 = (dofreeze ?
      DateMoyr(workerData.disabPeriod[1].getOnsetDate()) :
      workerData.disabPeriod[1].getWaitperDate());
    // if first quarter is not a qc, stop in preceding quarter
#if !defined(NDEBUG)
    if (qcDisDate6.getYear() <= 0) {
      throw PiaException(PIA_IDS_DISREQCAL2);
    }
#endif
    if (qcov.get(qcDisDate6.getYear()) <= qcDisDate6.getQuarter()) {
      qcDisDate6.subtract(1);
    }
    // set beginning quarter and year, based on (40 - qcdiqtr) quarters
    qcDisDate5 = qcDisDate6;
    qcDisDate5.subtract(39 - qcDisQtr);
    // check for special insured status if period starts before age 21
    if (dospecial && qcDisDate5 < tempQtrYear) {
      qcDiSpec(qcDisQtr, 1);
    }
    if (qcDisDate5.getYear() > 0) {
      // check for period beginning before 1937
      if (static_cast<int>(qcDisDate5.getYear()) < YEAR37) {
        qcDisDate5.setQuarter(0);
        qcDisDate5.setYear(YEAR37);
      }
      qcDisQtr += QtrYear::diff(qcDisDate5, qcDisDate6) + 1;
    }
  }
  // calculate number of elapsed years
  qcDisYears = (qcDisQtr + 2) / 4;
  // calculate number of qc's required
  qcDisReq = qcDisQtr / 2;
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    stringstream strm;
    strm << workerData.getIdString()
      << ": Exiting PiaData::qcDisReqCal for date"
      << DateFormatter::toString(date);
    Trace::writeLine(strm.str());
    strm.str("");
    strm << workerData.getIdString() << ": qcDisDate1 = "
      << qcDisDate1.toString() << ", qcDisDate2 = " << qcDisDate2.toString()
      << ", qcDisDate3 = " << qcDisDate3.toString() << ", qcDisDate4 = "
      << qcDisDate4.toString() << ", qcDisDate5 = " << qcDisDate5.toString()
      << ", qcDisDate6 = " << qcDisDate6.toString();
    Trace::writeLine(strm.str());
  }
#endif
}

/// <summary>Determines number of required quarters of coverage for DI
/// insured status for non-freeze.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DISREQCAL1"/> if year in most recent
/// freeze period is out of range (only in debug mode); of type
/// <see cref="PIA_IDS_DISREQCAL2"/> if year in next most recent
/// freeze period is out of range (only in debug mode).</exception>
///
/// <param name="workerData">Basic worker data.</param>
/// <param name="date">The date for which required quarters of coverage are
///  desired.</param>
/// <param name="trial">Number of quarters to go beyond onset quarter.</param>
/// <param name="dospecial">Indicator for special insured status calculation.
/// If false, do not do special insured status calculation. If true, do
/// special insured status calculation.</param>
/// <param name="dofreeze">Indicator for assuming period between onset and
/// waiting period in a prior period of disability is a freeze period. If
/// false, do not assume it is a freeze period (excluded period starts with
/// waiting period). If true, assume it is a freeze period (excluded period
/// starts with onset).</param>
void PiaData::qcDisReqNonFreezeCal( const WorkerDataGeneral& workerData,
const DateMoyr& date, int trial, bool dospecial, bool dofreeze )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    stringstream strm;
    strm << workerData.getIdString()
      << ": Entering PiaData::qcDisReqNonFreezeCal for date"
      << DateFormatter::toString(date);
    Trace::writeLine(strm.str());
    strm.str("");
    strm << workerData.getIdString() 
      << ": qcDisDateNonFreeze1 = " << qcDisDateNonFreeze1.toString() 
      << ", qcDisDateNonFreeze2 = " << qcDisDateNonFreeze2.toString();
    Trace::writeLine(strm.str());
  }
#endif
  QtrYear tempQtrYear;  // temporary (quarter,year) date
  if (workerData.getValdi() > 0) {
    qcDisDateNonFreeze2 = workerData.disabPeriod[0].getWaitperDate();
  } else {
    qcDisDateNonFreeze2 = date;
  }
  // set quarter and year after attainment of age 21
  tempQtrYear = getBirthDateMinus1();
  tempQtrYear.add(QtrYear(1,21));
  // set beginning quarter and year, based on 40 quarters
  qcDisDateNonFreeze1 = qcDisDateNonFreeze2;
  qcDisDateNonFreeze1.subtract(39);
  // check for special insured status if period starts before age 21
  if (dospecial && qcDisDateNonFreeze1 < tempQtrYear) {
    qcDiSpecNonFreeze(0, 0);
  }
  // check for period beginning before 1937
  if (static_cast<int>(qcDisDateNonFreeze1.getYear()) < YEAR37) {
    qcDisDateNonFreeze1.setQuarter(0);
    qcDisDateNonFreeze1.setYear(YEAR37);
  }
  qcDisQtrNonFreeze = QtrYear::diff(qcDisDateNonFreeze1, qcDisDateNonFreeze2) + 1;
  // calculate number of elapsed years
  qcDisYearsNonFreeze = (qcDisQtrNonFreeze + 2) / 4;
  // calculate number of qc's required
  qcDisReqNonFreeze = qcDisQtrNonFreeze / 2;
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    stringstream strm;
    strm << workerData.getIdString()
      << ": Exiting PiaData::qcDisReqNonFreezeCal for date"
      << DateFormatter::toString(date);
    Trace::writeLine(strm.str());
    strm.str("");
    strm << workerData.getIdString() 
      << ": qcDisDateNonFreeze1 = " << qcDisDateNonFreeze1.toString() 
      << ", qcDisDateNonFreeze2 = " << qcDisDateNonFreeze2.toString();
    Trace::writeLine(strm.str());
  }
#endif
}

/// <summary>Determines number of required quarters of coverage for DI
/// insured status, based on elapsed quarters after disability cessation.
/// </summary>
///
/// <param name="workerData">Basic worker data.</param>
/// <param name="date">The date for which required quarters of coverage are
/// desired.</param>
void PiaData::qcDisReq2Cal( const WorkerDataGeneral& workerData,
const DateMoyr& date )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    stringstream strm;
    strm << workerData.getIdString()
      << ": Entering PiaData::qcDisReq2Cal for date"
      << DateFormatter::toString(date);
    Trace::writeLine(strm.str());
    strm.str("");
    strm << workerData.getIdString() << ": qcDisDate1 = "
      << qcDisDate1.toString() << ", qcDisDate2 = " << qcDisDate2.toString()
      << ", qcDisDate3 = " << qcDisDate3.toString() << ", qcDisDate4 = "
      << qcDisDate4.toString() << ", qcDisDate5 = " << qcDisDate5.toString()
      << ", qcDisDate6 = " << qcDisDate6.toString();
    Trace::writeLine(strm.str());
  }
#endif
  qcDisDate3 = workerData.disabPeriod[0].getCessationDate();
  qcDisDate4 = date;
  if (workerData.getValdi() == 0 ||
    workerData.disabPeriod[0].getCessationDate().getYear() == 0 ||
    !(qcDisDate3 < qcDisDate4)) {
    qcDisQtr2 = 0;
    qcDisDate3.deleteContents();
    qcDisDate4.deleteContents();
  }
  else {
    qcDisDate3.add(1);
    // check for no more than 40 quarters
    qcDisQtr2 = QtrYear::diff(qcDisDate3, qcDisDate4) + 1;
    if (qcDisQtr2 > 40) {
      // set beginning quarter and year, based on 40 quarters
      qcDisDate3 = qcDisDate4;
      qcDisDate3.subtract(39);
      qcDisQtr2 = 40;
    }
  }
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    stringstream strm;
    strm << workerData.getIdString()
      << ": Exiting PiaData::qcDisReq2Cal for date"
      << DateFormatter::toString(date);
    Trace::writeLine(strm.str());
    strm.str("");
    strm << workerData.getIdString() << ": qcDisDate1 = "
      << qcDisDate1.toString() << ", qcDisDate2 = " << qcDisDate2.toString()
      << ", qcDisDate3 = " << qcDisDate3.toString() << ", qcDisDate4 = "
      << qcDisDate4.toString() << ", qcDisDate5 = " << qcDisDate5.toString()
      << ", qcDisDate6 = " << qcDisDate6.toString();
    Trace::writeLine(strm.str());
  }
#endif
}

/// <summary>Sets first quarter and year of disability insured period for
/// special insured status (called only if calculated first quarter and year
/// are in quarter of age 21 or earlier).</summary>
///
/// <param name="qcdiqtrt">Number of elapsed quarters accounted for before
/// considering special insured status.</param>
/// <param name="period">Period to consider (0 for prior to most recent
/// disability, 1 for prior to next most recent disability).</param>
void PiaData::qcDiSpec( int qcdiqtrt, int period )
{
  QtrYear* tempQcDisDate1;
  QtrYear* tempQcDisDate2;
  if (period == 0) {
    tempQcDisDate1 = &qcDisDate1;
    tempQcDisDate2 = &qcDisDate2;
  }
  else {
    tempQcDisDate1 = &qcDisDate5;
    tempQcDisDate2 = &qcDisDate6;
  }
  // start with quarter after attainment of age 21
  *tempQcDisDate1 = getBirthDateMinus1();
  tempQcDisDate1->add(QtrYear(1,21));
  // set minimum number of qcs that must be in special period
  const int tempqcs = 12 - qcdiqtrt;
  if (tempqcs <= 0) {
    tempQcDisDate1->deleteContents();
    tempQcDisDate2->deleteContents();
    return;
  }
  // check for fewer than tempqcs qc's
  if (QtrYear::diff(*tempQcDisDate1, *tempQcDisDate2) < tempqcs - 1) {
    // use tempqcs quarters
    *tempQcDisDate1 = *tempQcDisDate2;
    tempQcDisDate1->subtract(tempqcs - 1);
  }
}

/// <summary>Sets first quarter and year of disability insured period for
/// special insured status (called only if calculated first quarter and year
/// are in quarter of age 21 or earlier).</summary>
///
/// <param name="qcdiqtrt">Number of elapsed quarters accounted for before
/// considering special insured status.</param>
/// <param name="period">Period to consider (0 for prior to most recent
/// disability, 1 for prior to next most recent disability).</param>
void PiaData::qcDiSpecNonFreeze( int qcdiqtrt, int period )
{
  QtrYear* tempQcDisDate1;
  QtrYear* tempQcDisDate2;
  tempQcDisDate1 = &qcDisDateNonFreeze1;
  tempQcDisDate2 = &qcDisDateNonFreeze2;
  // start with quarter after attainment of age 21
  *tempQcDisDate1 = getBirthDateMinus1();
  tempQcDisDate1->add(QtrYear(1,21));
  // set minimum number of qcs that must be in special period
  const int tempqcs = 12 - qcdiqtrt;
  if (tempqcs <= 0) {
    tempQcDisDate1->deleteContents();
    tempQcDisDate2->deleteContents();
    return;
  }
  // check for fewer than tempqcs qc's
  if (QtrYear::diff(*tempQcDisDate1, *tempQcDisDate2) < tempqcs - 1) {
    // use tempqcs quarters
    *tempQcDisDate1 = *tempQcDisDate2;
    tempQcDisDate1->subtract(tempqcs - 1);
  }
}

/// <summary>Calculates earned quarters of coverage in disability insured
/// period.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCDISCAL"/> if any starting or ending dates are out of
/// range (only in debug mode).</exception>
///
/// <remarks>Assumes starting and ending dates have already been set, by
/// calling <see cref="PiaData::qcDisReqCal"/>.</remarks>
///
/// <param name="workerData">Basic worker data.</param>
void PiaData::qcDisCal( const WorkerDataGeneral& workerData )
{
  qcTotalDis = 0;
#ifndef NDEBUG
  if (qcDisDate1.getYear() < 0)
    throw PiaException(PIA_IDS_QCDISCAL);
#endif
  if (qcDisDate1.getYear() > 0) {
#ifndef NDEBUG
    if (qcDisDate2.getYear() <= 0)
      throw PiaException(PIA_IDS_QCDISCAL);
#endif
    qcTotalDis = qcov.accumulate(qcDisDate1, qcDisDate2, qcTotalDis);
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm << workerData.getIdString();
      for (unsigned year = qcDisDate1.getYear();
        year <= qcDisDate2.getYear(); year++) {
        strm << ", qcov[" << year << "] = " << qcov.get(year);
      }
      strm << ": Using qcov in PiaData::qcDisCal";
      Trace::writeLine(strm.str());
      strm.str("");
      strm << "Id " << workerData.getIdString() << ", qcDisDate1 = "
        << qcDisDate1.toString() << ", qcDisDate2 = "
        << qcDisDate2.toString() << ": Set qcTotalDis to " << qcTotalDis;
      Trace::writeLine(strm.str());
    }
#endif
  }
#ifndef NDEBUG
  if (qcDisDate3.getYear() < 0)
    throw PiaException(PIA_IDS_QCDISCAL);
#endif
  if (qcDisDate3.getYear() > 0) {
#ifndef NDEBUG
    if (qcDisDate4.getYear() <= 0)
      throw PiaException(PIA_IDS_QCDISCAL);
#endif
    qcTotalDis = qcov.accumulate(qcDisDate3, qcDisDate4, qcTotalDis);
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm << workerData.getIdString();
      for (unsigned year = qcDisDate3.getYear();
        year <= qcDisDate4.getYear(); year++) {
        strm << ", qcov[" << year << "] = " << qcov.get(year);
      }
      strm << ": Using qcov in PiaData::qcDisCal";
      Trace::writeLine(strm.str());
      strm.str("");
      strm << "Id " << workerData.getIdString() << ", qcDisDate3 = "
        << qcDisDate3.toString() << ", qcDisDate4 = "
        << qcDisDate4.toString() << ": Set qcTotalDis to " << qcTotalDis;
      Trace::writeLine(strm.str());
    }
#endif
  }
#ifndef NDEBUG
  if (qcDisDate5.getYear() < 0)
    throw PiaException(PIA_IDS_QCDISCAL);
#endif
  if (qcDisDate5.getYear() > 0) {
#ifndef NDEBUG
    if (qcDisDate6.getYear() <= 0)
      throw PiaException(PIA_IDS_QCDISCAL);
#endif
    qcTotalDis = qcov.accumulate(qcDisDate5, qcDisDate6, qcTotalDis);
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm << workerData.getIdString();
      for (unsigned year = qcDisDate5.getYear();
        year <= qcDisDate6.getYear(); year++) {
        strm << ", qcov[" << year << "] = " << qcov.get(year);
      }
      strm << ": Using qcov in PiaData::qcDisCal";
      Trace::writeLine(strm.str());
      strm.str("");
      strm << "Id " << workerData.getIdString() << ", qcDisDate5 = "
        << qcDisDate5.toString() << ", qcDisDate6 = "
        << qcDisDate6.toString() << ": Set qcTotalDis to " << qcTotalDis;
      Trace::writeLine(strm.str());
    }
#endif
  }
}

/// <summary>Calculates earned quarters of coverage in disability insured
/// period for non-freeze.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCDISCAL"/> if any starting or ending dates are out of
/// range (only in debug mode).</exception>
///
/// <remarks>Assumes starting and ending dates have already been set, by
/// calling <see cref="PiaData::qcDisReqNonFreezeCal"/>.</remarks>
///
/// <param name="workerData">Basic worker data.</param>
void PiaData::qcDisNonFreezeCal( const WorkerDataGeneral& workerData )
{
  qcTotalDisNonFreeze = 0;
#ifndef NDEBUG
  if (qcDisDateNonFreeze1.getYear() <= 0)
    throw PiaException(PIA_IDS_QCDISCAL);
#endif
  if (qcDisDateNonFreeze1.getYear() > 0) {
#ifndef NDEBUG
    if (qcDisDateNonFreeze2.getYear() <= 0)
      throw PiaException(PIA_IDS_QCDISCAL);
#endif
    qcTotalDisNonFreeze = qcov.accumulate(qcDisDateNonFreeze1, 
      qcDisDateNonFreeze2, qcTotalDisNonFreeze);
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm << workerData.getIdString();
      for (unsigned year = qcDisDateNonFreeze1.getYear();
        year <= qcDisDateNonFreeze2.getYear(); year++) {
        strm << ", qcov[" << year << "] = " << qcov.get(year);
      }
      strm << ": Using qcov in PiaData::qcDisnonFreezeCal";
      Trace::writeLine(strm.str());
      strm.str("");
      strm << "Id " << workerData.getIdString() 
        << ", qcDisDateNonFreeze1 = " << qcDisDateNonFreeze1.toString() 
        << ", qcDisDateNonFreeze2 = " << qcDisDateNonFreeze2.toString() 
        << ": Set qcTotalDisNonFreeze to " << qcTotalDisNonFreeze;
      Trace::writeLine(strm.str());
    }
#endif
  }
}

/// <summary>Returns first year with a quarter of coverage; 1937 if any QC's
/// before 1951, 1936 if no QC's.</summary>
///
/// <returns>First year with a quarter of coverage; 1937 if any QC's
/// before 1951, 1936 if no QC's.</returns>
///
/// <param name="workerData">Basic worker data.</param>
int PiaData::getFirstQcYear( const WorkerDataGeneral& workerData ) const
{
  if (getQcTotal50() > 0 || workerData.getQcovRR3750() > 0)
    return(YEAR37);
  for (int yr = 1951; yr <= WorkerDataGeneral::getMaxyear(); yr++) {
    if (workerData.qc.get(yr) > 0)
      return(yr);
  }
  return(1936);
}

/// <summary>Returns last year with a quarter of coverage, but at least
/// 1950.</summary>
///
/// <returns>Last year with a quarter of coverage, but at least 1950.
/// </returns>
///
/// <param name="workerData">Basic worker data.</param>
int PiaData::getLastQcYear( const WorkerDataGeneral& workerData ) const
{
  for (int yr = WorkerDataGeneral::getMaxyear(); yr > 1950; yr--) {
    if (workerData.qc.get(yr) > 0)
      return(yr);
  }
  return(1950);
}

/// <summary>Calculates total quarters of coverage. Assumes annual quarters
/// of coverage are already known, either from 1951 or from 1978.</summary>
///
/// <param name="workerData">Basic worker data.</param>
/// <param name="qtrYear">Calendar quarter and year through which qc's are
///  calculated.</param>
void PiaData::qcTotalCal( const WorkerDataGeneral& workerData,
const QtrYear& qtrYear )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": At top of PiaData::qcTotalCal");
  }
#endif
#if !defined(NDEBUG)
  qtrYear.check();
#endif
  setQc3750simp(PiaCal::qc3750simpCal(getEarnTotal50(EARN_NO_TOTALIZATION)));
  setQcTotal50(workerData.getQctottd() - workerData.getQctot51td() +
    workerData.getQcovRR3750() + qc3750ms);
  const int i1 = max(qcTotal50, qc3750simp);
  // use annual quarters of coverage, if necessary
  if (workerData.hasQcsByYear()) {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() + ", starting qtrYear " +
        qtr151.toString() + ", ending qtrYear " + qtrYear.toString() +
        ": Calling qcov.accumulate");
    }
#endif
    qcTotal51 = qcov.accumulate(qtr151, qtrYear, 0u);
  } else {
  // use lump sum to 1977, annual quarters thereafter
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() + ", starting year 1951" +
        ", ending year 1956: Calling qcovMilServ.accumulate");
    }
#endif
    qcTotal51 = qcovMilServ.accumulate(1951, 1956,
      workerData.getQctot51td());
    qcTotal51 += workerData.getQcovRR5177();
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() + ", starting qtrYear " +
        qtr178.toString() + ", ending qtrYear " + qtrYear.toString() +
        ": Calling qcov.accumulate");
    }
#endif
    qcTotal51 = qcov.accumulate(qtr178, qtrYear, qcTotal51);
  }
  setQcTotal(i1 + qcTotal51);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from PiaData::qcTotalCal");
  }
#endif
}

/// <summary>Calculates total quarters of coverage for non-freeze. Assumes 
/// annual quarters of coverage are already known, either from 1951 or 
/// from 1978, and that qcs prior to 1951 have already been computed 
/// as well by <see cref="PiaData::qcTotalCal"/>.</summary>
///
/// <param name="workerData">Basic worker data.</param>
/// <param name="qtrYear">Calendar quarter and year through which qc's are
///  calculated.</param>
void PiaData::qcTotalNonFreezeCal( const WorkerDataGeneral& workerData,
const QtrYear& qtrYear )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": At top of PiaData::qcTotalNonFreezeCal");
  }
#endif
#if !defined(NDEBUG)
  qtrYear.check();
#endif
  const int i1 = max(qcTotal50, qc3750simp);
  // use annual quarters of coverage, if necessary
  if (workerData.hasQcsByYear()) {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() + ", starting qtrYear " +
        qtr151.toString() + ", ending qtrYear " + qtrYear.toString() +
        ": Calling qcov.accumulate");
    }
#endif
    qcTotal51NonFreeze = qcov.accumulate(qtr151, qtrYear, 0u);
  } else {
  // use lump sum to 1977, annual quarters thereafter
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() + ", starting year 1951" +
        ", ending year 1956: Calling qcovMilServ.accumulate");
    }
#endif
    qcTotal51NonFreeze = qcovMilServ.accumulate(1951, 1956,
      workerData.getQctot51td());
    qcTotal51NonFreeze += workerData.getQcovRR5177();
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() + ", starting qtrYear " +
        qtr178.toString() + ", ending qtrYear " + qtrYear.toString() +
        ": Calling qcov.accumulate");
    }
#endif
    qcTotal51NonFreeze = qcov.accumulate(qtr178, qtrYear, qcTotal51NonFreeze);
  }
  setQcTotalNonFreeze(i1 + qcTotal51NonFreeze);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from PiaData::qcTotalNonFreezeCal");
  }
#endif
}

/// <summary>Calculates first year of earnings after 1950, with or without
/// totalization.</summary>
///
/// <param name="workerData">Basic worker data.</param>
void PiaData::earn50Cal( const WorkerDataGeneral& workerData )
{
  const int yr = getIbeginAll();
  earn50[0] = max(yr, 1951);
  earn50[1] = max(workerData.getTotalize() ? ibeginTotal : yr, 1951);
}

/// <summary>Returns applicable noncovered pension.</summary>
///
/// <remarks>As of January 1995, the rules for determining the noncovered
/// pension amount changed, so someone with a noncovered pension before 1995
/// could have a different noncovered pension as of January 1995. This
/// function uses the benefit date to determine the correct noncovered
/// pension.</remarks>
///
/// <returns>Applicable noncovered pension.</returns>
///
/// <param name="workerData">Basic worker data.</param>
float PiaData::getPubpenApp( const WorkerDataGeneral& workerData ) const
{
  if (workerData.getReservist() &&
    workerData.getBenefitDate().getYear() > 1994) {
    return(workerData.getPubpenReservist());
  }
  else {
    return(workerData.getPubpen());
  }
}

/// <summary>Initializes computed values to zero.</summary>
void PiaData::initialize()
{
  compPeriodOld.deleteContents();
  arfApp = NO_SPEC_MIN_DRC;
  childCareYearsTotal = 0;
  supportPia = 0.0;
  highMfb.set(0.0);
  highPia.set(0.0);
  iapps = iappn = -1;
}

/// <summary>Determines freeze period.</summary>
///
/// <param name="workerData">Basic worker data.</param>
/// <param name="ioasdi">Type of benefit.</param>
/// <param name="fullRetAgeDI">Full retirement age for benefit
///  calculation purposes.</param>
void PiaData::freezeYearsCal( const WorkerDataGeneral& workerData,
WorkerDataGeneral::ben_type ioasdi, const Age& fullRetAgeDI )
{
  int year1;  // year before attainment of full retirement age
  int year2;  // year before assumed attainment of full retirement age
  freezeYears.deleteContents();
  // determine years of freeze for latest period of disability
  if (workerData.getValdi() > 0) {
    const DisabPeriod& disabPeriod = workerData.disabPeriod[0];
    freezeYears.setYear1((disabPeriod.getOnsetDate().month() ==
      static_cast< int >(boost::date_time::Jan) &&
      disabPeriod.getOnsetDate().day() == 1) ?
      (int)disabPeriod.getOnsetDate().year() :
      (int)disabPeriod.getOnsetDate().year() + 1);
    if (ioasdi == WorkerDataGeneral::DISABILITY) {
      year1 = kbirth.year() + fullRetAgeDI.getYears() - 1;
      if (fullRetAgeDI.getMonths() + kbirth.month() > 12) {
        year1++;
      }
      year2 = disabPeriod.getWaitperDate().getYear() +
        fullRetAgeDI.getYears() - 63;
      if (fullRetAgeDI.getMonths() +
        disabPeriod.getWaitperDate().getMonth() > 12) {
        year2++;
      }
      freezeYears.setYear2(
        min(static_cast<int>(workerData.getBenefitDate().getYear()) - 1,
        min(year2, year1)));
    }
    else {
      freezeYears.setYear2((disabPeriod.getCessationDate().getMonth() ==
      boost::date_time::Dec) ? disabPeriod.getCessationDate().getYear() :
      disabPeriod.getCessationDate().getYear() - 1);
    }
  }
  // determine years of freeze for prior period of disability
  if (workerData.getValdi() > 1) {
    const DisabPeriod& disabPeriod = workerData.disabPeriod[1];
    freezeYears.setYear3((disabPeriod.getOnsetDate().month() ==
      static_cast<int>(boost::date_time::Jan) &&
      disabPeriod.getOnsetDate().day() == 1) ?
      (int)disabPeriod.getOnsetDate().year() :
      (int)disabPeriod.getOnsetDate().year() + 1);
    freezeYears.setYear4((disabPeriod.getCessationDate().getMonth() ==
      static_cast<int>(boost::date_time::Dec)) ?
      (int)disabPeriod.getCessationDate().getYear() :
      (int)disabPeriod.getCessationDate().getYear() - 1);
  }
}

/// <summary>Determines last year of allowable earnings.</summary>
///
/// <remarks>In survivor case, use earnings through year of death; otherwise
/// stop in year before year of benefit</remarks>
///
/// <param name="workerData">Basic worker data.</param>
/// <param name="ioasdi">Type of benefit.</param>
void PiaData::earnYearCal( const WorkerDataGeneral& workerData,
WorkerDataGeneral::ben_type ioasdi )
{
  earnYear = (ioasdi == WorkerDataGeneral::SURVIVOR) ?
    (int)workerData.getDeathDate().year() :
    (int)workerData.getBenefitDate().getYear() - 1;
}

/// <summary>Sets last year of attributed earnings in totalization case.
/// </summary>
///
/// <remarks>This is set to the last year of earnings for the record,
/// or the last year actually used in the current computation, whichever
/// is earlier.</remarks>
///
/// <param name="workerData">Basic worker data.</param>
void PiaData::setIendTotal( const WorkerDataGeneral& workerData )
{
  iendTotal = max(ielgyr, workerData.getIend());
  if (workerData.getIndrr())
    iendTotal = max(iendTotal, workerData.getLastYearRR());
  iendTotal = min(iendTotal,earnYear);
}

/// <summary>Sets first year of attributed earnings in totalization case.
/// </summary>
///
/// <param name="workerData">Basic worker data.</param>
void PiaData::setIbeginTotal( const WorkerDataGeneral& workerData )
{
  ibeginTotal = max(static_cast<int>(kbirth.year() + 22), YEAR37);
  if (workerData.getIndearn() && ibeginTotal > workerData.getIbegin())
    ibeginTotal = workerData.getIbegin();
}

/// <summary>Sets date of attainment of normal retirement age, but no
/// earlier than January 1937.</summary>
///
/// <remarks> Assumes that day before birth and full retirement age have
/// already been calculated.</remarks>
void PiaData::setFullRetDate()
{
  fullRetDate = DateMoyr(getBirthDateMinus1()) + fullRetAge;
  if (fullRetDate < jan1937)
    fullRetDate = jan1937;
}

/// <summary>Sets some quarter of coverage amounts.</summary>
///
/// <param name="workerData">Basic worker data.</param>
void PiaData::setQc3( WorkerDataGeneral& workerData )
{
  if (WorkerDataGeneral::getQcLumpYear() > 1950) {
    qcTotal51 = workerData.getQctot51td();
  }
  else {
    setQcTotal50((getEarnTotal50(EARN_NO_TOTALIZATION) < 49.9) ?
      0 : static_cast<int>(getEarnTotal50(EARN_NO_TOTALIZATION)) / 500 + 1);
    workerData.setQctd(getQcTotal50());
    workerData.setQc51td(0);
  }
}

/// <summary>Updates military service earnings and quarters of coverage.
/// </summary>
///
/// <remarks>This is called in <see cref="earnProjection"/>.</remarks>
///
/// <param name="workerData">Basic worker data.</param>
void PiaData::updateMilServ( const WorkerDataGeneral& workerData )
{
  qc3750ms = workerData.getQcovMS3750();
  for (int year = 1951; year <= 1956; year++) {
    qcovMilServ.set(year, workerData.getQcovMS(year));
  }
  earn3750ms = workerData.getEarnMS3750();
  earnMilServ.fill(0.0);
  for (int year = 1951; year <= workerData.getLastYearMS(); year++) {
    earnMilServ[year] = workerData.getEarnMS(year);
  }
}

/// <summary>Calculates annual quarters of coverage.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCCAL"/> if amount required for quarter of coverage is
/// not initialized (only in debug mode); if years of earnings extend beyond
/// years of amount required for quarter of coverage (only in debug mode).
/// </exception>
///
/// <param name="workerData">Basic worker data.</param>
/// <param name="qcamt">Amount required for quarter of coverage.</param>
void PiaData::qcCal( WorkerDataGeneral& workerData, const Qcamt& qcamt )
{
  unsigned qctemp;  // temporary quarters of coverage
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": At top of PiaData::qcCal");
  }
#endif
  try {
    workerData.qc.deleteContents(WorkerDataGeneral::getQcLumpYear() + 1,
      workerData.qc.getLastYear());
    // at this point, qctot51 has qcs up to 1977, excluding military service
    // and railroad
    int i2 = max(WorkerDataGeneral::getQcLumpYear(), 1950);
    if (doEarlyQcs) {
      for (int yr = YEAR37; yr <= i2; yr++) {
        qctemp = workerData.qc.get(yr);
        if (workerData.getIndrr()) {
          qctemp += workerData.getQcovRR(yr);
#if defined(DEBUGCASE)
          if (isDebugPid(workerData.getIdNumber())) {
            ostringstream strm;
            strm << workerData.getIdString() << ": Adding "
              << workerData.getQcovRR(yr) << " railroad qcs for year " << yr;
            Trace::writeLine(strm.str());
          }
#endif
        }
        if (workerData.getIndms() && yr < 1957) {
          qctemp += qcovMilServ.get(yr);
#if defined(DEBUGCASE)
          if (isDebugPid(workerData.getIdNumber())) {
            ostringstream strm;
            strm << workerData.getIdString() << ": Adding "
              << qcovMilServ.get(yr) << " military service qcs for year "
              << yr;
            Trace::writeLine(strm.str());
          }
#endif
        }
        qcov.set(yr, min(4u, qctemp));
#if defined(DEBUGCASE)
        if (isDebugPid(workerData.getIdNumber())) {
          ostringstream strm;
          strm << workerData.getIdString() << ": Setting qcs for year " << yr
            << " to " << qctemp;
          Trace::writeLine(strm.str());
        }
#endif
      }
    }
    for (int yr = i2 + 1; yr <= iendAll; yr++) {
#if !defined(NDEBUG)
      if (yr > qcamt.getLastYear())
        throw PiaException("Year is out of range");
      if (qcamt[yr] < 1.0)
        throw PiaException(PIA_IDS_QCCAL);
#endif
      qctemp = Qc::qcCal(earnOasdi[yr], qcamt[yr]);
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        ostringstream strm;
        strm << workerData.getIdString() << ": Setting qcs for year " << yr
          << " to " << qctemp;
        Trace::writeLine(strm.str());
      }
#endif
      workerData.qc.set(yr, qctemp);
      qcov.set(yr, qctemp);
    }
  } catch (PiaException& e) {
    throw PiaException(string(e.what()) + " in PiaData::qcCal");
  }
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from PiaData::qcCal");
  }
#endif
}

/// <summary>Projects earnings and fills in earnings to be used in
/// calculations.</summary>
///
/// <param name="workerData">Basic worker data.</param>
void PiaData::earnProjection( const WorkerDataGeneral& workerData )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": At top of PiaData::earnProjection");
  }
#endif
  earnOasdi.fill(0.0);
  earnHi.fill(0.0);
  earnTotalized.fill(0.0);
  earnOasdiLimited.fill(0.0);
  earnHiLimited.fill(0.0);
  earnTotalizedLimited.fill(0.0);
  // start with actual earnings
  ibeginAll = workerData.getIbegin();
  iendAll = workerData.getIend();
  if (workerData.getIndearn()) {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() +
        ": Ready to set regular earnings");
    }
#endif
    for (int yr = ibeginAll; yr <= iendAll; yr++) {
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        ostringstream strm;
        strm << workerData.getIdString() << ", year " << yr << ", earnings "
          << poutNoComma(workerData.getEarnOasdi(yr),11,2) << ": Setting regular earnings";
        Trace::writeLine(strm.str());
      }
#endif
      earnOasdi[yr] = workerData.getEarnOasdi(yr);
    }
  }
  // add in railroad earnings
  if (workerData.getIndrr()) {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() +
        ": Handling railroad earnings");
    }
#endif
    if (workerData.getLastYearRR() > 1950) {
      const int i2 = max(workerData.getFirstYearRR(), 1951);
      const int i3 = workerData.getLastYearRR();
      if (!workerData.getIndearn()) {
        ibeginAll = i2;
        iendAll = i3;
      }
      else {
        if (ibeginAll > i2)
          ibeginAll = i2;
        if (iendAll < i3)
          iendAll = i3;
      }
      for (int yr = i2; yr <= i3; yr++) {
        const double dtemp = earnOasdi[yr] + workerData.getEarnRR(yr);
        earnOasdi[yr] = dtemp;
      }
    }
    // add in pre-1951 earnings
    if (workerData.getEarnRR3750() > 0.0) {
      if (ibeginAll < 1937 || ibeginAll > 1950)
        ibeginAll = 1950;
      if (iendAll < 1950)
        iendAll = 1950;
      const double dtemp = earnOasdi[1950] + workerData.getEarnRR3750();
      earnOasdi[1950] = dtemp;
    }
  }
  // add in military service credits
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Ready to call updateMilServ");
  }
#endif
  updateMilServ(workerData);
  if (workerData.getIndms()) {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() +
        ": Handling military service");
    }
#endif
    if (getEarn3750ms() > 0.) {
      if (ibeginAll > 1950)
        ibeginAll = 1950;
      if (iendAll < 1950)
        iendAll = 1950;
      const double dtemp = earnOasdi[1950] + getEarn3750ms();
      earnOasdi[1950] = dtemp;
    }
    int lastYear = workerData.getLastYearMS();
    if (lastYear >= 1951) {
      int firstYear = workerData.getFirstYearMS();
      if (firstYear < ibeginAll || ibeginAll == 0)
        ibeginAll = firstYear;
      if (lastYear > iendAll)
        iendAll = lastYear;
      for (int yr = firstYear; yr <= lastYear; yr++) {
        const double dtemp = earnOasdi[yr] + earnMilServ[yr];
        earnOasdi[yr] = dtemp;
      }
    }
  }
}

/// <summary>Calculates HI earnings by adding OASDI earnings and excess HI
/// earnings.</summary>
///
/// <remarks>This is called in <see cref="PiaCal::earnProjection"/>.</remarks>
///
/// <param name="workerData">Basic worker data.</param>
void PiaData::earnHiCal( const WorkerDataGeneral& workerData )
{
  // initialize HI earnings
  earnHi.fill(0.0);
  // add OASDI earnings plus excess HI earnings
  if (workerData.getIndearn()) {
    for (int yr = workerData.getIbegin(); yr <= workerData.getIend(); yr++) {
      earnHi[yr] = earnOasdi[yr] + workerData.getEarnHi(yr);
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        ostringstream strm;
        strm << workerData.getIdString() << ", year " << yr
          << ", HI earnings " << poutNoComma(workerData.getEarnHi(yr),11,2)
          << ": PiaData::earnHiCal";
        Trace::writeLine(strm.str());
      }
#endif
    }
  }
}

/// <summary>Returns maximum child care credit amount for a specified year.
/// </summary>
///
/// <returns>Maximum child care credit amount for a specified year.</returns>
///
/// <param name="yearOfCredit">Year of the credit</param>
double PiaData::getChildCareMaxCredit( int yearOfCredit ) const
{
  for (unsigned i = 0; i < childCareMaxCredit.size(); i++) {
    if (yearOfCredit == childCareMaxCredit[i].second) {
      return childCareMaxCredit[i].first;
    }
  }
  return 0.0;
}

/// <summary>Sets day before worker's birth.</summary>
///
/// <param name="birthDate">Worker's date of birth.</param>
void PiaData::setBirthDateMinus1( const boost::gregorian::date& birthDate )
{
  kbirth = birthDate - boost::gregorian::date_duration(1);
}
