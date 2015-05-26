// Functions for the <see cref="PiaCalAny"/> class to manage pia
// calculation functions used in Anypia.

// $Id: piacalany.cpp 1.91 2011/08/09 15:55:03EDT 044579 Development  $

#include <cmath>  // definition of floor
#include "PiaCalAny.h"
#include "oactcnst.h"
#include "PiaException.h"
#include "UserAssumptions.h"
#include "EarnProject.h"
#include "WorkerDataArray.h"
#include "SecondaryArray.h"
#include "PiadataArray.h"
#include "piaparmsAny.h"
#include "WageBase.h"
#include "Assumptions.h"
#include "DebugCase.h"
#if defined(DEBUGCASE)
#include <sstream>
#include "Trace.h"
#endif

using namespace std;

/// <summary>Initializes a PiaCalAny instance.</summary>
///
/// <param name="newWorkerData">Worker's basic data.</param>
/// <param name="newPiaData">Worker's calculation data.</param>
/// <param name="newWidowDataArray">Widow and other family members' basic
/// data.</param>
/// <param name="newWidowArray">Widow and other family members' calculation
///  data.</param>
/// <param name="newPiaParamsAny">Pia calculation parameters.</param>
/// <param name="newUserAssumptions">User-specified assumptions.</param>
/// <param name="newSecondary">Array of <see cref="Secondary"/> benefits.
/// </param>
/// <param name="newLawChange">Law change parameters.</param>
/// <param name="newPebs">Statement information.</param>
/// <param name="newEarnProject">Projected earnings information.</param>
PiaCalAny::PiaCalAny( WorkerDataGeneral& newWorkerData, PiaData& newPiaData,
WorkerDataArray& newWidowDataArray, PiaDataArray& newWidowArray,
PiaParamsAny& newPiaParamsAny, UserAssumptions& newUserAssumptions,
SecondaryArray& newSecondary, const LawChangeArray& newLawChange,
Pebs& newPebs, EarnProject& newEarnProject ) :
PiaCalLC(newWorkerData, newPiaData, newPiaParamsAny, newLawChange),
widowDataArray(newWidowDataArray), widowArray(newWidowArray),
secondaryArray(newSecondary), earnProject(newEarnProject),
userAssumptions(newUserAssumptions), piaParamsAny(newPiaParamsAny),
pebs(newPebs)
{ }

/// <summary>Destructor.</summary>
PiaCalAny::~PiaCalAny()
{ }

/// <summary>Calculates preparatory variables.</summary>
///
/// <param name="assumptions">Stored benefit increase and average wage
///  assumptions.</param>
void PiaCalAny::calculate1( Assumptions& assumptions )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", userAssumptions.getJaltbi() "
      << static_cast<int>(userAssumptions.getJaltbi())
      << ", userAssumptions.getIaltbi() "
      << static_cast<int>(userAssumptions.getIaltbi())
      << ": Checking for need to set ialtbi in PiaCalAny::calculate1";
    Trace::writeLine(strm.str());
  }
#endif
  if (userAssumptions.getJaltbi() != userAssumptions.getIaltbi() ||
    userAssumptions.getIaltbi() == AssumptionType::OTHER_ASSUM) {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() +
        ": Setting benefit increase assumptions in PiaCalAny::calculate1");
    }
#endif
    const AssumptionType::assum_type alt = userAssumptions.getIaltbi();
    if (alt == AssumptionType::OTHER_ASSUM) {
      piaParamsAny.updateCpiinc(userAssumptions.biproj,
        userAssumptions.catchup, userAssumptions.titleBi,
        UserAssumptions::getIstart(), userAssumptions.biproj.getLastYear());
    }
    else {
      assumptions.benefitIncMenu(alt);
      piaParamsAny.updateCpiinc(assumptions.getBiProj().theData,
        assumptions.getCatchupDoc().catchup,
        assumptions.getBenefitInc(alt), assumptions.getIstart(),
        assumptions.getBiProj().theData.getLastYear());
    }
  }
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", userAssumptions.getJaltaw() "
      << static_cast<int>(userAssumptions.getJaltaw())
      << ", userAssumptions.getIaltaw() "
      << static_cast<int>(userAssumptions.getIaltaw())
      << ": Checking for need to set ialtaw in PiaCalAny::calculate1";
    Trace::writeLine(strm.str());
  }
#endif
  if (userAssumptions.getJaltaw() != userAssumptions.getIaltaw() ||
    userAssumptions.getIaltaw() == AssumptionType::OTHER_ASSUM) {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() +
        ": Setting average wage assumptions in PiaCalAny::calculate1");
    }
#endif
    const AssumptionType::assum_type alt = userAssumptions.getIaltaw();
    if (alt == AssumptionType::OTHER_ASSUM) {
      piaParamsAny.updateFqinc(userAssumptions.awincproj,
        userAssumptions.titleAw, UserAssumptions::getIstart() - 1,
        userAssumptions.awincproj.getLastYear());
    }
    else {
      assumptions.averageWageMenu(alt);
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        ostringstream strm;
        strm << workerData.getIdString() << ", first year "
          << (assumptions.getIstart() - 1) << ", last year "
          << assumptions.getAwincProj().awinc.getLastYear()
          << ": Calling piaParamsAny.updateFqinc";
        Trace::writeLine(strm.str());
      }
#endif
      piaParamsAny.updateFqinc(assumptions.getAwincProj().awinc,
        assumptions.getAverageWage(alt), assumptions.getIstart() - 1,
        assumptions.getAwincProj().awinc.getLastYear());
    }
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm.precision(2);
      strm << workerData.getIdString() << ", average wage for 2000 "
        << piaParamsAny.getFq(2000) << ": After setting "
        << "average wage assumptions in PiaCalAny::calculate1";
      Trace::writeLine(strm.str());
    }
#endif
  }
  if (userAssumptions.getJbasch() != userAssumptions.getIbasch() ||
    userAssumptions.getJaltaw() != userAssumptions.getIaltaw() ||
    userAssumptions.getIaltaw() == AssumptionType::OTHER_ASSUM ||
    userAssumptions.getIbasch() == 2) {
    piaParamsAny.updateBases(userAssumptions.baseOasdi,
      userAssumptions.base77, userAssumptions.getIbasch(),
      UserAssumptions::getIstart() + 1,
      userAssumptions.baseOasdi.getLastYear());
    piaParamsAny.updateYocAmountSpecMin();
  }
  // This line must come after above conditionals
  userAssumptions.resetTriggers();
}

/// <summary>Calculates PIA and benefit.</summary>
///
/// <param name="entDate">The date of entitlement.</param>
void PiaCalAny::calculate2( const DateMoyr& entDate )
{
  int i2;  // number of calculations to do
  setPebsAssumptions(userAssumptions.getIaltaw() ==
    AssumptionType::PEBS_ASSUM);
  // set number of calculations to do
  if (workerData.getJoasdi() == WorkerData::PEBS_CALC) {
    i2 = Pebs::MAXPEBS;
    pebsOabCal();
  }
  else {
    i2 = 1;
    if (workerData.getIndearn()) {
      workerData.ibeginCheck();
      earnProSteady();
    }
    earnProjection();
  }
  piaData.earlyRetAge = piaParams.earlyAgeOabCal(workerData.getSex(),
    piaData.getBirthDateMinus1());
  for (int i1 = 0; i1 < i2; i1++) {
    const Pebs::pebes_type pebsType = (Pebs::pebes_type)i1;
    if (workerData.getJoasdi() == WorkerData::PEBS_CALC)
      pebsSetup(pebsType, entDate);
    qcCal();
    PiaCalLC::calculate2(entDate);
    // save amounts for PEBES calculation
    if (workerData.getJoasdi() == WorkerData::PEBS_CALC) {
      // save quarters of coverage to date
      if (i1 == 0) {
        const int qctemp =
          workerData.qc.accumulate(WorkerData::getQcLumpYear() + 1,
          UserAssumptions::getIstart(), workerData.getQctottd());
        pebs.setQcTotal(min(40, qctemp));
        // save full retirement age
        pebs.fullRetAge = piaData.fullRetAge;
      }
      pebs.setPiaPebs(pebsType,
        piaData.highPia.round5(UserAssumptions::getIstart()));
      pebs.setMfbPebs(pebsType,
        piaData.highMfb.round5(UserAssumptions::getIstart()));
      pebs.setBenefitPebs(pebsType, (pebsType == Pebs::PEBS_SURV) ?
        BenefitAmount(0.75 * pebs.getPiaPebs(pebsType)).round5(
        UserAssumptions::getIstart()) :
        piaData.unroundedBenefit.round5(UserAssumptions::getIstart()));
      pebs.setQcReq(pebsType, piaData.getQcReq());
      // save DI info
      if (pebsType == Pebs::PEBS_DISAB) {
        pebs.setQcDisReq(piaData.getQcDisReq());
        pebs.setQcDisTotal(piaData.getQcTotalDis());
        pebs.setQcDisYears(piaData.getQcDisYears());
      }
    }
  }
  // return parameters to their original values
  if (workerData.getJoasdi() == WorkerData::PEBS_CALC) {
    earnProject.setPebsData(UserAssumptions::getIstart(),
      workerData.getIbegin());
    workerData.setPebsData();
  }
}

/// <summary>Calculates number of old-age calculations to do.</summary>
void PiaCalAny::pebsOabCal()
{
  DateMoyr datetemp(pebs.getMonthnow(), UserAssumptions::getIstart());
  // calculate age now
  pebs.ageNow = datetemp - DateMoyr(piaData.getBirthDateMinus1());
  // check for worker already at least full retirement age
  if (!(pebs.ageNow < piaData.fullRetAge)) {
    // do not do a disability benefit
    pebs.setPebsDib(0);
    // check for worker already at least age 70
    pebs.setPebsOab((pebs.ageNow < pebs.age70) ? 2 : 1);
  }
  else {
    pebs.setPebsDib(1);
    // check for planned retirement after full retirement age
    pebs.setPebsOab(
      (piaData.fullRetAge < Age(pebs.getAgePlan(), 0)) ? 2 : 3);
  }
  pebs.setAgePlan2(pebs.getAgePlan() > 0 ? pebs.getAgePlan() : 62);
  if (pebs.getAgePlan2() < (int)pebs.ageNow.getYears())
    pebs.setAgePlan2(pebs.ageNow.getYears());
}

/// <summary>Sets information for Statement case.</summary>
///
/// <param name="type">Statement case number.</param>
/// <param name="entDate">Entitlement date.</param>
void PiaCalAny::pebsSetup( Pebs::pebes_type type, const DateMoyr& entDate )
{
  // set current date
  DateMoyr date1(pebs.getMonthnow(), UserAssumptions::getIstart());
  DateMoyr date2;  // temporary date
  switch (type)
  {
    case Pebs::PEBS_OAB_DELAYED:
      setIoasdi(WorkerData::OLD_AGE);
      // set age to 70
      pebs.oab2 = pebs.age70;
      // pebs.getoab2().setyrmo(70, 0);
      // check against current age
      if (pebs.oab2 < pebs.ageNow)
        pebs.oab2 = pebs.ageNow;
      date2 = DateMoyr(piaData.getBirthDateMinus1()) + pebs.oab2;
      workerData.setEntDate(date2);
      workerData.setBenefitDate();
      // set age at entitlement
      piaData.ageEnt = pebs.oab2;
      workerData.setIend(workerData.getBenefitDate().getYear() - 1);
      break;
    case Pebs::PEBS_OAB_FULL:
      if (pebs.getPebsOab() < 2)
        return;
      setIoasdi(WorkerData::OLD_AGE);
      // set age to full retirement age
      pebs.oab1 = piaData.fullRetAge;
      // check against current age
      if (pebs.oab1 < pebs.ageNow)
        pebs.oab1 = pebs.ageNow;
      date2 = DateMoyr(piaData.getBirthDateMinus1()) + pebs.oab1;
      workerData.setEntDate(date2);
      workerData.setBenefitDate();
      // set age at entitlement
      piaData.ageEnt = pebs.oab1;
      workerData.setIend(workerData.getBenefitDate().getYear() - 1);
      break;
    case Pebs::PEBS_OAB_EARLY:
      if (pebs.getPebsOab() < 3)
        return;
      setIoasdi(WorkerData::OLD_AGE);
      // retirement at earliest possible age
      if ((int)piaData.earlyRetAge.getYears() >= pebs.getAgePlan())
        pebs.agePlan1 = piaData.earlyRetAge;
      else
        pebs.agePlan1 = Age(pebs.getAgePlan(), 0);
      // check against current age
      if (pebs.agePlan1 < pebs.ageNow)
        pebs.agePlan1 = pebs.ageNow;
      date2 = DateMoyr(piaData.getBirthDateMinus1()) + pebs.agePlan1;
      workerData.setEntDate(date2);
      workerData.setBenefitDate();
      // set age at entitlement
      piaData.ageEnt = pebs.agePlan1;
      // stop earnings in early retirement year
      if ((int)pebs.agePlan1.getYears() > pebs.getAgePlan2()) {
        workerData.setIend(piaData.getBirthDateMinus1().year() +
          pebs.getAgePlan2() - 1);
      }
      // continue earnings to year before current year
      if (workerData.getIend() < UserAssumptions::getIstart() - 1)
        workerData.setIend(UserAssumptions::getIstart() - 1);
      break;
    case Pebs::PEBS_SURV:
      setIoasdi(WorkerData::SURVIVOR);
      secondaryArray.secondary[0]->bic.setMajorBic('C');
      workerData.setEntDate(date1);
      workerData.setDeathDate(boost::gregorian::date(
        (unsigned short)date1.getYear(), date1.getMonth(), 1));
      workerData.setBenefitDate();
      // set age at entitlement
      piaData.ageEnt = workerData.getEntDate() -
        DateMoyr(piaData.getBirthDateMinus1());
      // set age at benefit date
      piaData.ageBen = workerData.getBenefitDate() -
        DateMoyr(piaData.getBirthDateMinus1());
      // stop earnings in year of death
      workerData.setIend(UserAssumptions::getIstart());
      break;
    case Pebs::PEBS_DISAB:
      if (pebs.getPebsDib() < 1)
        return;
      setIoasdi(WorkerData::DISABILITY);
      secondaryArray.secondary[0]->bic.setMajorBic(' ');
      workerData.deleteDeathDate();
      workerData.setValdi(1);
      workerData.setEntDate(date1);
      workerData.setOnsetDate(0, boost::gregorian::date(
        (unsigned short)date1.getYear(), date1.getMonth(), 1));
      workerData.setBenefitDate();
      // stop earnings in year before disability
      workerData.setIend(UserAssumptions::getIstart() - 1);
      break;
    default:
      break;
  }
  earnProjection();
  eligYearCal(workerData, piaData, getIoasdi());
  if (getIoasdi() != WorkerData::SURVIVOR)
    ardriCal();
  nelapsedCal(piaData.compPeriodNew, entDate);
  // calculate benefit factor for all family members
  for (int i = 0; i < widowArray.getFamSize(); i++) {
    Secondary * secondaryptr = secondaryArray.secondary[i];
    if (secondaryptr->bic.getMajorBic() == ' ')
      break;
    ardriAuxCal(widowDataArray.workerData[i], widowArray.piaData[i],
      secondaryptr, entDate);
  }
  return;
}

/// <summary>Projects steady earnings.</summary>
void PiaCalAny::earnProSteady()
{
  piaData.setOverMax(false);  // indicator for earnings over maximum
  for (int yr = workerData.getIbegin(); yr <= workerData.getIend(); yr++) {
    switch (earnProject.getEarntype()[yr])
    {
    case EarnProject::MAXIMUM:
      // Use earnings a bit over the base so that it will project to the base
      // in other years.
      earnProject.setEarnpebs(yr, (yr < YEAR51) ? 3000.0 :
        1.2 * piaParamsAny.getBaseOasdi(yr));
      if (piaParamsAny.getBaseOasdi(yr) > MAXEARN)
        piaData.setOverMax(true);
      break;
    case EarnProject::HIGH:
      if (piaParamsAny.getWageHigh(yr) <= MAXEARN)
        earnProject.setEarnpebs(yr, piaParamsAny.getWageHigh(yr));
      else {
        earnProject.setEarnpebs(yr, MAXEARN);
        piaData.setOverMax(true);
      }
      break;
    case EarnProject::AVERAGE:
      if (piaParamsAny.getFq(yr) <= MAXEARN)
        earnProject.setEarnpebs(yr, piaParamsAny.getFq(yr));
      else {
        earnProject.setEarnpebs(yr, MAXEARN);
        piaData.setOverMax(true);
      }
      break;
    case EarnProject::LOW:
      if (piaParamsAny.getWageMin(yr) <= MAXEARN)
        earnProject.setEarnpebs(yr, piaParamsAny.getWageMin(yr));
      else {
        earnProject.setEarnpebs(yr, MAXEARN);
        piaData.setOverMax(true);
      }
      break;
    case EarnProject::ENTERED:
      break;
    }  // end switch
  }
  return;
}

/// <summary>Projects earnings and fills in earnings to be used in
/// calculations.</summary>
void PiaCalAny::earnProjection() const
{
  if (workerData.getIndearn()) {
    earnProject.project(workerData, piaParams.getFqincArray());
  }
  PiaCalLC::earnProjection();
}

/// <summary>Checks primary data for validity and does some preliminary
/// calculations.</summary>
///
/// <param name="entDate">Entitlement date.</param>
void PiaCalAny::dataCheck( const DateMoyr& entDate )
{
  if (workerData.getJoasdi() != WorkerData::PEBS_CALC) {
    PiaCal::dataCheck( entDate );
    // We used to change each set of assumptions to flat here if they are not
    // needed for the estimate. This leads to problems in save files when the
    // benefit increase assumptions are needed but the average wage increase
    // assumptions are not needed. The calculator ignores what is entered in
    // the assumptions dialog and uses flat AWI assumptions. This then gets
    // saved to the pia file, which makes it look like the user is not using
    // consistent assumptions. Mike Clingman complained about this, and his
    // objection seems reasonable, so I am commenting out these sections of
    // the code.  DO 04/08/2008.
    //if (!workerData.need_bi(UserAssumptions::getIstart()))
    //   userAssumptions.setIaltbi(AssumptionType::FLAT);
    if (userAssumptions.getAnscch() != 'Y') {
      userAssumptions.catchup.deleteContents();
    }
    userAssumptions.setLastYear(workerData.getBenefitDate().getYear());
    pebs.deleteContents();

//    bool needAw = earnProject.need_aw(UserAssumptions::getIstart(),
//      workerData.getIend());
//#if defined(DEBUGCASE)
//    if (isDebugPid(workerData.getIdNumber())) {
//      ostringstream strm;
//      strm << workerData.getIdString() << ", userAssumptions.getIaltaw() "
//        << static_cast<int>(userAssumptions.getIaltaw())
//        << ": Before calling workerData.need_aw";
//      Trace::writeLine(strm.str());
//    }
//#endif
//    if (!workerData.need_aw(UserAssumptions::getIstart(), needAw)) {
//#if defined(DEBUGCASE)
//      if (isDebugPid(workerData.getIdNumber())) {
//        ostringstream strm;
//        strm << workerData.getIdString()
//          << ": Calling userAssumptions.setIaltaw(UserAssumptions::FLAT)";
//        Trace::writeLine(strm.str());
//      }
//#endif
//      userAssumptions.setIaltaw(AssumptionType::FLAT);
//    }
//#if defined(DEBUGCASE)
//    if (isDebugPid(workerData.getIdNumber())) {
//      ostringstream strm;
//      strm << workerData.getIdString() << ", userAssumptions.getIaltaw() "
//        << static_cast<int>(userAssumptions.getIaltaw())
//        << ": After calling workerData.need_aw";
//      Trace::writeLine(strm.str());
//    }
//#endif

  }
  else {
    workerData.ibeginCheck();
    workerData.birth1Check(UserAssumptions::getIstart());
    pebs.monthnowCheck();
    earnProject.setPebsData(UserAssumptions::getIstart(),
      workerData.getIbegin());
    workerData.setPebsData();
    piaData.setBirthDateMinus1(workerData.getBirthDate());
    // eligibility year is not yet calculated for PEBES case; use
    // piadata.getBirthDateMinus1().year()+62 since there is no prior
    // disability in PEBES case
    piaData.fullRetAge =
      piaParams.fullRetAgeCal(piaData.getBirthDateMinus1().year() + 62);
    piaData.setFullRetDate();
    workerData.qctdCheck2();
    workerData.qcCheck();
    workerData.valdiCheck();
    // set PEBS assumptions
    userAssumptions.pebsasmCheck();
    userAssumptions.setIbasch(1);
  }
}
