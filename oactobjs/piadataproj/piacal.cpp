// Functions for the <see cref="PiaCal"/> class - parent of all classes
// that manage the calculation of a Social Security benefit.
//
// $Id: piacal.cpp 1.174 2018/08/06 06:53:55EDT 277133 Development  $

#include <cmath>  // for floor
#include <utility>  // for rel_ops
#include "piacal.h"
#include "oactcnst.h"
#include "OldStart.h"
#include "PiaException.h"
#include "WorkerDataArray.h"
#include "PiadataArray.h"
#include "Resource.h"
#include "piaparms.h"
#include "WageBase.h"
#include "ReindWid.h"
#include "WageInd.h"
#include "SpecMin.h"
#include "PiaTable.h"
#include "ChildCareCalc.h"
#include "WageIndNonFreeze.h"
#include "Pifc.h"
#include "DebugCase.h"
#if defined(DEBUGCASE)
#include <sstream>
#include "Trace.h"
#include "DateFormatter.h"
#endif

using namespace std;
#if !defined(__SGI_STL_INTERNAL_RELOPS)
using namespace std::rel_ops;
#endif

/// <summary>Initializes this instance.</summary>
///
/// <param name="newWorkerData">Worker's basic data.</param>
/// <param name="newPiaData">Worker's pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
PiaCal::PiaCal( WorkerDataGeneral& newWorkerData, PiaData& newPiaData,
const PiaParams& newPiaParams ) :
workerData(newWorkerData), piaData(newPiaData), piaParams(newPiaParams),
piaMethod(), widowMethod(), wageInd(0), oldStart(0), specMin(0), piaTable(0),
frozMin(0), childCareCalc(0), transGuar(0), highPiaMethod(0),
ioasdi(WorkerData::NO_BEN), pebsAssumptions(false)
{ }

/// <summary>Destructor.</summary>
PiaCal::~PiaCal()
{ }

/// <summary>Figures high pia and mfb.</summary>
void PiaCal::piaCal1()
{
  try {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() +
        ": At top of PiaCal::piaCal1");
    }
#endif
    setHighPia();
    // find support PIA
    setSupportPia();
    // Apply DI maximum.
    if (ioasdi == WorkerDataGeneral::DISABILITY &&
      piaData.getEligYear() > 1978 &&
      workerData.getEarliestEntDate() >= PiaParams::amend80) {
      double highAime = 0.0;
      double dimaxTemp = 0.0;
      double highMax = 0.0;
      if (wageInd != static_cast<WageInd *>(0)) {
        dimaxTemp = wageInd->diMax(wageInd->getAme(), piaData.highPia.get());
        wageInd->mfbEnt.set(dimaxTemp);
        if (highAime < wageInd->getAme()) {
          highAime = wageInd->getAme();
          highMax = dimaxTemp;
        }
      }
      if (childCareCalc != static_cast<ChildCareCalc *>(0)) {
        dimaxTemp = childCareCalc->diMax(childCareCalc->getAme(), piaData.highPia.get());
        childCareCalc->mfbEnt.set(dimaxTemp);
        if (highAime < childCareCalc->getAme()) {
          highAime = childCareCalc->getAme();
          highMax = dimaxTemp;
        }
      }
      if (wageIndNonFreeze != static_cast<WageIndNonFreeze *>(0)) {
        dimaxTemp = wageIndNonFreeze->diMaxNonFreeze(wageIndNonFreeze->getAme(),
          piaData.highPia.get());
        wageIndNonFreeze->mfbEnt.set(dimaxTemp);
        if (highAime < wageIndNonFreeze->getAme()) {
          highAime = wageIndNonFreeze->getAme();
          highMax = dimaxTemp;
        }
      }
      for (vector< PiaMethod * >::iterator iter = piaMethod.begin();
        iter != piaMethod.end(); iter++) {
        if ((*iter)->getApplicable() > 0 &&
          (*iter)->getMethod() != PiaMethod::DIB_GUAR &&
          (*iter)->getMethod() != PiaMethod::WAGE_IND &&
          (*iter)->getMethod() != PiaMethod::CHILD_CARE &&
          (*iter)->getMethod() != PiaMethod::DIB_GUAR) {
          (*iter)->mfbEnt.set(highMax);
        }
      }
    }
    setHighMfb();
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() +
        ": Returning from PiaCal::piaCal1");
    }
#endif
  } catch (exception& e) {
    throw PiaException(string("In PiaCal::piaCal1: ") + e.what());
  }
}

/// <summary>Figures benefit for primary.</summary>
void PiaCal::piaCal2()
{
  try {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() +
        ": At top of PiaCal::piaCal2");
    }
#endif
    int i1 = workerData.getBenefitDate().getYear();
    if (i1 >= YEAR51) {
      if (static_cast<int>(workerData.getBenefitDate().getMonth()) <
        piaParams.getMonthBeninc(i1)) {
        i1--;
      }
    }
    // calculate benefit payable for primary
    if (ioasdi != WorkerData::SURVIVOR) {
      ardriCal();
      // check for special minimum with delayed retirement credit
      if (piaData.getIapps() >= 0) {
        piaData.unroundedBenefit.set(BenefitAmount::round(piaData.getArf() *
          piaData.supportPia.get(), i1));
        setArfApp();
      }
      else {
        if (workerData.getEntDate() < PiaParams::amend50) {
          piaData.unroundedBenefit = piaData.highPia;
        }
        else {
          piaData.unroundedBenefit.set(BenefitAmount::round(piaData.getArf() *
            piaData.highPia.get(), i1));
        }
      }
      // round to lower dollar if June 1982 or later
      piaData.roundedBenefit = piaData.unroundedBenefit;
      piaData.roundedBenefit.round1(workerData.getBenefitDate());
    }
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() +
        ": Returning from PiaCal::piaCal2");
    }
#endif
  } catch (exception& e) {
    throw PiaException(string("In PiaCal::piaCal2: ") + e.what());
  }
}

/// <summary>Figures benefits for auxiliaries.</summary>
///
/// <param name="widowArray">Widow and other family members' calculation data.
/// </param>
/// <param name="secondaryArray">Array of <see cref="Secondary"/> benefit
/// data.</param>
void PiaCal::piaCal3( PiaDataArray& widowArray,
SecondaryArray& secondaryArray )
{
  int i1 = workerData.getBenefitDate().getYear();
  if (i1 >= YEAR51) {
   if (static_cast<int>(workerData.getBenefitDate().getMonth()) <
     piaParams.getMonthBeninc(i1)) {
     i1--;
   }
  }
  // calculate benefit for each family member using usual pia
  for (int i2 = 0; i2 < widowArray.getFamSize(); i2++) {
    Secondary * secondaryptr = secondaryArray.secondary[i2];
    // check for special minimum with delayed retirement credit
    // this currently does not work because there is no arf in a survivor
    // case
    //if (secondaryptr->bic.getMajorBic() == 'D' && piaData.getIapps() >= 0){
    //   bentemp = piaData.getArf() * piaData.supportPia.get();
    //   bentemp.round(i1);
    //   setArfApp();
    //}
    const double bentemp = piaData.highPia.get();
    secondaryptr->pifc.set(piaData.getPifc());
    // calculate full benefit
    secondaryptr->setFullBenefit(BenefitAmount::round(bentemp *
      secondaryptr->getBenefitFactor(), i1));
  }
  // reset benefit for widow(er)s using widow(er) pia
  for (vector< ReindWid * >::iterator iter = widowMethod.begin();
    iter != widowMethod.end(); iter++) {
    Secondary& secondary = (*iter)->secondary;
    // check for re-indexed widow(er) method for widow(er)s
    if (piaData.highPia < (*iter)->piaEnt) {
      const double bentemp = (*iter)->piaEnt.get();
      secondary.pifc.set('W');
      // calculate full benefit
      secondary.setFullBenefit(BenefitAmount::round(bentemp *
        secondary.getBenefitFactor(), i1));
    }
  }
  // reduce benefit for family maximum
  if (ioasdi == WorkerData::SURVIVOR)
    applySurvivorMfb(secondaryArray, widowArray.getFamSize(), i1);
  else
    applyLifeMfb(secondaryArray, widowArray.getFamSize(), i1);
  // reduce benefits for age, then round
  for (int i2 = 0; i2 < widowArray.getFamSize(); i2++) {
    Secondary * secondaryptr = secondaryArray.secondary[i2];
    if (secondaryptr->bic.isReducible()) {
      secondaryptr->setReducedBenefit(BenefitAmount::round(
        secondaryptr->getArf() * secondaryptr->getBenefit(), i1));
    }
    else {
      secondaryptr->setReducedBenefit(secondaryptr->getBenefit());
    }
    // round to lower dollar if June 1982 or later
    double bentemp = secondaryptr->getReducedBenefit();
    bentemp = BenefitAmount::round1(bentemp, workerData.getBenefitDate());
    secondaryptr->setRoundedBenefit(bentemp);
  }
}

/// <summary>Sets support pia, if applicable pia is special minimum and
/// there is delayed retirement credit involved.</summary>
///
/// <remarks>Delayed retirement credit cannot be applied to a special minimum
/// pia, but it can be applied to the support pia (the next-highest pia) when
/// calculating the old-age benefit or widow(er) benefit.</remarks>
void PiaCal::setSupportPia()
{
  if (piaData.fullRetAge < piaData.ageEnt &&
    piaData.getIappn() == PiaMethod::SPEC_MIN) {
    // method producing support pia
    PiaMethod *supportMethod = static_cast<PiaMethod *>(0);
    for (vector< PiaMethod * >::iterator iter = piaMethod.begin();
      iter != piaMethod.end(); iter++) {
      if (piaData.supportPia < (*iter)->piaEnt &&
        (*iter)->getMethod() != PiaMethod::SPEC_MIN) {
        piaData.supportPia = (*iter)->piaEnt;
        piaData.setIapps((*iter)->getMethod());
        supportMethod = (*iter);
#if defined(DEBUGCASE)
        if (isDebugPid(workerData.getIdNumber())) {
          ostringstream strm;
          strm << workerData.getIdString() << ", " << (*iter)->getTitle()
            << ", piaEnt " << (*iter)->piaEnt.toString()
            << ": Setting iapps in setSupportPia";
          Trace::writeLine(strm.str());
        }
#endif
      }
      else {
#if defined(DEBUGCASE)
        if (isDebugPid(workerData.getIdNumber())) {
          ostringstream strm;
          strm << workerData.getIdString() << ", " << (*iter)->getTitle()
            << ", piaEnt " << (*iter)->piaEnt.toString()
            << ": Not setting iapps in setSupportPia";
          Trace::writeLine(strm.str());
        }
#endif
      }
    }
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm << workerData.getIdString() << ", getIapps() "
        << piaData.getIapps() << ": Ready to call setApplicable";
      Trace::writeLine(strm.str());
    }
#endif
    if (supportMethod != static_cast<PiaMethod *>(0)) {
      supportMethod->setApplicable(PiaMethod::SUPPORT_PIA);
    }
  }
}

/// <summary>Sets applicable pia.</summary>
///
/// <remarks>This should be called after all the pia's have been calculated.
/// It also sets the indicator for type of pia.</remarks>
void PiaCal::setHighPia()
{
  piaData.setIappn(-1);
  for (vector< PiaMethod * >::iterator iter = piaMethod.begin();
  iter != piaMethod.end(); iter++) {
    // set highest PIA
    if (piaData.highPia < (*iter)->piaEnt) {
      piaData.highPia = (*iter)->piaEnt;
      piaData.setIappn((*iter)->getMethod());
      highPiaMethod = (*iter);
    }
  }
  if (highPiaMethod != static_cast<PiaMethod *>(0)) {
    highPiaMethod->setApplicable(PiaMethod::HIGH_PIA);
  }
  setPifc();
}

/// <summary>Sets applicable mfb.</summary>
///
/// <remarks>This should be called after the high pia is set. The mfb is based
/// on the same type as the high pia, except when the high pia is based on the
/// re-indexed widow(er) method. In that case, the high mfb is gotten from the
/// wage-indexed method.</remarks>
void PiaCal::setHighMfb()
{
  if (piaData.getIappn() > -1) {
    if (piaData.getIappn() == PiaMethod::REIND_WID) {
      piaData.highMfb = wageInd->mfbEnt;
    }
    else {
      piaData.highMfb = highPiaMethod->mfbEnt;
    }
  }
}

/// <summary>Sets indicator for delayed retirement credit in special minimum
/// case.</summary>
void PiaCal::setArfApp()
{
  if (specMin != static_cast<SpecMin *>(0)) {
    if (specMin->piaEnt < piaData.unroundedBenefit) {
      piaData.setArfApp(PiaData::SUPPORT_BEN);
    }
    else {
      piaData.unroundedBenefit = specMin->piaEnt;
      piaData.setArfApp(PiaData::SPEC_MIN_BEN);
    }
  }
}

/// <summary>Sets applicable pifc.</summary>
void PiaCal::setPifc()
{
  const OldStart::OldStartType methodOs =
    (oldStart == static_cast<OldStart *>(0)) ?
    OldStart::OS1939 : oldStart->getMethodOs();
  PiaMethod::WindfallElimType windfall =
    (wageInd == static_cast<WageInd *>(0)) ?
    PiaMethod::NOWINDFALLELIM : wageInd->getWindfall();
  if (piaData.getIappn() == PiaMethod::WAGE_IND_NON_FREEZE) {
    windfall = (wageIndNonFreeze == static_cast<WageIndNonFreeze *>(0)) ?
      PiaMethod::NOWINDFALLELIM : wageIndNonFreeze->getWindfall();
  }
  piaData.setPifc(Pifc::pifcCal(piaData.getIappn(), windfall,
    methodOs, piaData.getAmend90(), workerData.getTotalize()));
}

/// <summary>Checks date of entitlement for a primary benie.</summary>
///
/// <remarks>Assumes type of benefit is
/// <see cref="WorkerDataGeneral::ben_type::OLD_AGE"/> or
/// <see cref="WorkerDataGeneral::ben_type::DISABILITY"/>.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_ARDRI1"/> if age is out of range; of type
/// <see cref="PIA_IDS_ARDRI2"/> if age is before early retirement age for
/// OAB; of type <see cref="PIA_IDS_ARDRI7"/> if DIB entitlement before
/// 1957; of type <see cref="PIA_IDS_ARDRI8"/> if age under 50 for DIB before
/// 1960; of type <see cref="PIA_IDS_ARDRI9"/> if age over full retirement age
/// for DIB.</exception>
void PiaCal::ageEntCheck() const
{
  if (piaData.ageEnt.getYears() <= 0)  // impossible age
    throw PiaException(PIA_IDS_ARDRI1);
  // old-age
  if (ioasdi == WorkerDataGeneral::OLD_AGE) {
    // retirement before earliest possible age
    if (piaData.ageEnt < piaData.earlyRetAge)
      throw PiaException(PIA_IDS_ARDRI2);
  }
  // disability
  if (ioasdi == WorkerDataGeneral::DISABILITY) {
    if (workerData.getEntDate().getYear() <= 1959) {
      if (workerData.getEntDate().getYear() < 1957)
        throw PiaException(PIA_IDS_ARDRI7);
      if (piaData.ageEnt.getYears() < 50)
        throw PiaException(PIA_IDS_ARDRI8);
      return;
    }
    if (!(piaData.ageEnt < piaData.fullRetAge)) {
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        string s("");
        s += workerData.getIdString();
        s += ", ageEnt ";
        s += piaData.ageEnt.toString();
        s += ", fullRetAge ";
        s += piaData.fullRetAge.toString();
        s += ": DIB too old in PiaCal::ardriCal";
        Trace::writeLine(s);
      }
#endif
      throw PiaException(PIA_IDS_ARDRI9);
    }
  }
}

/// <summary>Calculates the number of months of early or delayed retirement
/// for a primary benie.</summary>
///
/// <remarks>If old-age, it calls <see cref="monthsArCal"/> or
/// <see cref="monthsDriCal"/>, if entitlement is before or after full
/// retirement age, respectively. If disability, it calls
/// <see cref="monthsArDICal"/>.
///
/// Assumes type of benefit is
/// <see cref="WorkerDataGeneral::ben_type::OLD_AGE"/> or
/// <see cref="WorkerDataGeneral::ben_type::DISABILITY"/>.</remarks>
void PiaCal::ardriCal() const
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(
      workerData.getIdString() + ": At top of PiaCal::ardriCal");
  }
#endif
  // delayed retirement increment or early retirement reduction
  if (ioasdi == WorkerData::OLD_AGE) {  // only for old-age
    // early retirement reduction
    if (piaData.ageEnt < piaData.fullRetAge) {
      monthsArCal();
    }
    else {  // delayed retirement credit
      monthsDriCal();
    }
  }
  // disability
  if (ioasdi == WorkerData::DISABILITY) {
    monthsArDICal();
  }
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(
      workerData.getIdString() + ": Returning from PiaCal::ardriCal");
  }
#endif
}

/// <summary>Calculates the benefit factor and number of months of early or
/// delayed retirement for a dependent or survivor.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_ARDRI10"/> if aged wife age is before early retirement
/// age; of type <see cref="PIA_IDS_JSURV"/> if invalid bic; of type
/// <see cref="PIA_IDS_ARDRI4"/> if disabled widow(er) age is before earliest
/// possible age; of type <see cref="PIA_IDS_ARDRI5"/> if disabled widow(er)
/// entitlement is before benefit date; of type <see cref="PIA_IDS_ARDRI6"/>
/// if aged widow(er) age is before earliest possible age.</exception>
///
/// <param name="widowData">Family member's basic information.</param>
/// <param name="widowPiaData">Family member's primary benefit data.</param>
/// <param name="secondaryData">Family member's secondary benefit data.
/// </param>
/// <param name="entDate">Entitlement date.</param>
void PiaCal::ardriAuxCal( WorkerDataGeneral *widowData, PiaData *widowPiaData,
Secondary *secondaryData, const DateMoyr& entDate ) const
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(
      workerData.getIdString() + ": At top of PiaCal::ardriAuxCal");
  }
#endif
  // life cases
  if (ioasdi == WorkerData::OLD_AGE || ioasdi == WorkerData::DISABILITY) {
    switch (secondaryData->bic.getMajorBic())
    {
    case 'B':  // all wives
      if (!secondaryData->bic.isYoungSpouse()) {
        // handle aged wife (assumes earliest possible retirement age is
        // same as for old-age benie; true so far)
        widowPiaData->earlyRetAge = piaParams.earlyAgeOabCal(
          widowData->getSex(), widowPiaData->getBirthDateMinus1());
        if (widowPiaData->ageEnt < widowPiaData->earlyRetAge)
          throw PiaException(PIA_IDS_ARDRI10);
        // widow(er)'s full retirement age
        widowPiaData->fullRetAge = piaParams.fullRetAgeCal(
          widowPiaData->getBirthDateMinus1().year() + 62);
        monthsArAgedSpouseCal(widowPiaData, secondaryData);
#if defined(DEBUGCASE)
        if (isDebugPid(workerData.getIdNumber())) {
          Trace::writeLine(workerData.getIdString() +
            ": Returning from PiaCal::ardriAuxCal for case aged B");
        }
#endif
        return;
      }
      // fall through for young wives
    case 'C':  // children
      monthsYngLifeCal(secondaryData);
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        Trace::writeLine(workerData.getIdString() +
          ": Returning from PiaCal::ardriAuxCal for case C or young B");
      }
#endif
      return;
    default:  // no life auxiliary
      throw PiaException(PIA_IDS_JSURV);
    }
  }
  // survivors
  if (ioasdi == WorkerData::SURVIVOR) {
    switch (secondaryData->bic.getMajorBic())
    {
    case 'C':  // young survivors
    case 'E':
      monthsYngSurvCal(secondaryData);
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        Trace::writeLine(workerData.getIdString() +
          ": Returning from PiaCal::ardriAuxCal for case C or E");
      }
#endif
      return;
    case 'W':  // disabled widow
      widowPiaData->earlyRetAge = piaParams.earlyAgeDisWidCal(
        workerData.getBenefitDate());
      if (widowPiaData->ageEnt < widowPiaData->earlyRetAge)
        throw PiaException(PIA_IDS_ARDRI4);
      if (!(widowPiaData->ageEnt < piaParams.earlyAgeWidCal(
        workerData.getBenefitDate()))) {
        throw PiaException(PIA_IDS_ARDRI5);
      }
      // disabled widow's full retirement age
      widowPiaData->fullRetAge = piaParams.fullRetAgeCal(
        widowPiaData->getBirthDateMinus1().year() + 60);
      monthsArDisWidCal(widowPiaData, secondaryData, entDate);
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        Trace::writeLine(workerData.getIdString() +
          ": Returning from PiaCal::ardriAuxCal for case W");
      }
#endif
      return;
    case 'D':  // aged widow
      widowPiaData->earlyRetAge =
        piaParams.earlyAgeWidCal(workerData.getBenefitDate());
      if (widowPiaData->ageEnt < widowPiaData->earlyRetAge)
        throw PiaException(PIA_IDS_ARDRI6);
      // widow's full retirement age
      widowPiaData->fullRetAge = piaParams.fullRetAgeCal(
        widowPiaData->getBirthDateMinus1().year() + 60);
      monthsArWidCal(widowPiaData, secondaryData);
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        Trace::writeLine(workerData.getIdString() +
          ": Returning from PiaCal::ardriAuxCal for case D");
      }
#endif
      return;
    default:  // no survivor
      throw PiaException(PIA_IDS_JSURV);
    }
  }
}

/// <summary>Calculates number of months of delayed retirement credit for
/// OAB and sets increment factor.</summary>
///
/// <remarks>First finds the date first fully insured by calling
/// <see cref="fullInsDateCal"/>, since delayed retirement credits are only
/// available if fully insured. Then it calls the static
/// <see cref="PiaParams::monthsDriCal"/> to get the number of months. It
/// uses that number to calculate the total amount of delayed retirement
/// credit by calling the static <see cref="PiaParams::factorDriCal"/>.
///
/// This is called from <see cref="ardriCal"/>.</remarks>
void PiaCal::monthsDriCal() const
{
  const int eligYear = piaData.getEligDate().getYear();
  piaData.setFullInsDate(fullInsDateCal());
  piaData.setMonthsArdri(PiaParams::monthsDriCal(piaData.getFullRetDate(),
    eligYear, piaData.getBirthDateMinus1(), workerData.getEntDate(),
    workerData.getBenefitDate(), piaData.getFullInsDate()));
  piaData.setArf(PiaParams::factorDriCal(piaData.getMonthsArdri(),
    eligYear));
}

/// <summary>Calculates number of months of actuarial reduction for OAB and
/// sets reduction factor.</summary>
void PiaCal::monthsArCal() const
{
  piaData.setMonthsArdri(piaParams.monthsArCal(piaData.ageEnt,
    piaData.fullRetAge));
  piaData.setArf(piaParams.factorArCal(piaData.getMonthsArdri()));
}

/// <summary>Calculates number of months of actuarial reduction for DIB and
/// sets reduction factor.</summary>
void PiaCal::monthsArDICal() const
{
  piaData.setMonthsArdri(piaParams.monthsArDICal(workerData.getOabEntDate(),
    workerData.getOabCessDate()));
  piaData.setArf(piaParams.factorArCal(piaData.getMonthsArdri()));
}

/// <summary>Calculates number of months of actuarial reduction for disabled
/// widow(er) and sets reduction factor.</summary>
///
/// <param name="widowData">Family member's information.</param>
/// <param name="secondaryData">Family member's secondary benefit.</param>
/// <param name="entDate">The date of entitlement.</param>
void PiaCal::monthsArDisWidCal( PiaData *widowData,
Secondary *secondaryData, const DateMoyr& entDate ) const
{
  secondaryData->setMonthsArdri(piaParams.monthsArDisWidCal(
    widowData->ageEnt, workerData.getBenefitDate(), widowData->fullRetAge));
  secondaryData->setBenefitFactor(piaParams.factorDisWidCal(
    widowData->getEligYear(), workerData.getBenefitDate()));
  secondaryData->setArf(piaParams.factorArDisWidCal(
    secondaryData->getMonthsArdri(), entDate));
}

/// <summary>Calculates number of months of actuarial reduction for young
/// survivor and sets benefit factor.</summary>
///
/// <param name="secondaryData">Family member's secondary benefit.</param>
void PiaCal::monthsYngSurvCal( Secondary *secondaryData ) const
{
  secondaryData->setMonthsArdri(0);
  secondaryData->setBenefitFactor(piaParams.factorYngSurvCal());
}

/// <summary>Calculates number of months of actuarial reduction for young
/// auxiliary and sets benefit factor.</summary>
///
/// <param name="secondaryData">Family member's secondary benefit.</param>
void PiaCal::monthsYngLifeCal( Secondary *secondaryData ) const
{
  secondaryData->setMonthsArdri(0);
  secondaryData->setBenefitFactor(piaParams.factorYngLifeCal());
}

/// <summary>Calculates full benefit factor and number of months of actuarial
/// reduction for aged widow(er) and sets reduction factor.</summary>
///
/// <param name="widowData">Family member's information.</param>
/// <param name="secondaryData">Family member's secondary benefit.</param>
void PiaCal::monthsArWidCal( PiaData *widowData,
Secondary *secondaryData ) const
{
  secondaryData->setMonthsArdri(piaParams.monthsArWidCal(widowData->ageEnt,
    workerData.getBenefitDate(), widowData->fullRetAge));
  secondaryData->setBenefitFactor(piaParams.factorAgedWidCal(
    secondaryData->getMonthsArdri(), widowData->ageEnt,
    widowData->getEligYear(), workerData.getBenefitDate()));
  secondaryData->setArf(piaParams.factorArWidCal(
    secondaryData->getMonthsArdri(), widowData->ageEnt,
    workerData.getBenefitDate(), widowData->fullRetAge));
}

/// <summary>Calculates full benefit factor and number of months of actuarial
/// reduction for aged wife or husband and sets reduction factor.</summary>
///
/// <param name="widowData">Family member's information.</param>
/// <param name="secondaryData">Family member's secondary benefit.</param>
void PiaCal::monthsArAgedSpouseCal( PiaData *widowData,
Secondary *secondaryData ) const
{
  secondaryData->setMonthsArdri(piaParams.monthsArAgedSpouseCal(
    widowData->ageEnt, workerData.getBenefitDate(), widowData->fullRetAge));
  secondaryData->setBenefitFactor(piaParams.factorAgedSpouseCalPL());
  secondaryData->setArf(piaParams.factorArAgedSpouseCal(
    secondaryData->getMonthsArdri()));
}

/// <summary>Calculates PIA and benefit.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCTOT0"/> if no quarters of
/// coverage in a totalization case.</exception>
///
/// <param name="entDate">The date of entitlement.</param>
void PiaCal::calculate2( const DateMoyr& entDate )
{
  try {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() +
        ": At top of PiaCal::calculate2");
    }
#endif
    piaData.earnTotal50Cal0();
    piaData.freezeYearsCal(workerData, ioasdi, piaParams.fullRetAgeCalDI(
      piaData.getBirthDateMinus1().year() + 62, entDate.getYear()));
    piaData.earnYearCal(workerData, ioasdi);
    if (workerData.getTotalize()) {
      if (piaData.getQcTotal() == 0)
        throw PiaException(PIA_IDS_QCTOT0);
      piaData.setRelEarnPositionAverage(relEarnPositionCal());
      piaData.earnTotal50Cal1();
      // limit earnings attributed for totalization to wage base
      piaParams.getBaseOasdiArray().earnLimit(piaData.earnTotalizedLimited,
        piaData.earnTotalized);
    }
    nCal(piaData.compPeriodNew, entDate);
    nNonFreezeCal(piaData.compPeriodNewNonFreeze, entDate);
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm << workerData.getIdString() << ", n "
        << piaData.compPeriodNew.getN()
        << ": After nCal in PiaCal::calculate2";
      Trace::writeLine(strm.str());
    }
#endif
  } catch (exception& e) {
    throw PiaException(string("In PiaCal::calculate2: ") + e.what());
  }
}

/// <summary>Calculates annual quarters of coverage and insured status.
/// </summary>
void PiaCal::qcCal()
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() + ": At top of PiaCal::qcCal");
  }
#endif
  piaData.earnTotal50Cal0();
  piaData.setQc3750simp(qc3750simpCal(
    piaData.getEarnTotal50(PiaData::EARN_NO_TOTALIZATION)));
  piaData.qcCal(workerData, piaParams.qcamt);
  piaData.finsCode.set(insCal(ioasdi == WorkerData::SURVIVOR ?
    workerData.getDeathDate() : boost::gregorian::date(
    static_cast<unsigned short>(workerData.getEntDate().getYear()),
    static_cast<unsigned short>(workerData.getEntDate().getMonth()), 1u),
    workerData.isPrimary(ioasdi)));
  piaData.setFinsCode2(fins2Cal(workerData, piaData, ioasdi));
  // calculate non-freeze insured status.
  piaData.finsNonFreezeCode.set(insNonFreezeCal(ioasdi == WorkerData::SURVIVOR ?
    workerData.getDeathDate() : boost::gregorian::date(
    static_cast<unsigned short>(workerData.getEntDate().getYear()),
    static_cast<unsigned short>(workerData.getEntDate().getMonth()), 1u),
    workerData.isPrimary(ioasdi)));
  piaData.setFinsNonFreezeCode2(finsNonFreeze2Cal(workerData, piaData, ioasdi));
  // calculate DI quarters of coverage
  if (ioasdi == WorkerData::DISABILITY) {
    piaData.disInsCode.set(disInsCal(workerData, piaData,
      workerData.getEntDate(), 1));
    piaData.disInsNonFreezeCode.set(disInsNonFreezeCal(workerData, piaData,
      workerData.getEntDate(), 1));
  }
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(
      workerData.getIdString() + ": Returning from PiaCal::qcCal");
  }
#endif
}

/// <summary>Calculates relative earnings position.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_RELERNPOS"/> if first year of
/// earnings is out of range (only in debug mode).</exception>
///
/// <returns>Relative earnings position.</returns>
double PiaCal::relEarnPositionCal()
{
  double reptot = 0.0;  // total of annual rep's
  // set years of earnings
  piaData.setIbeginTotal(workerData);
#if !defined(NDEBUG)
  if (piaData.getIbeginTotal() < YEAR37)
    throw PiaException(PIA_IDS_RELERNPOS);
#endif
  piaData.setIendTotal(workerData);
  // find last year of AWI to use for the artificial earnings record.  
  // Totalization computations use special rules which can effectively ignore
  // one or two AWI values, even after they are known.  
  // I'm not sure this works, exactly.  DJO 11/1/2012.
  int awiYear = piaData.getEligYear() - 2;
  if (ioasdi == WorkerData::SURVIVOR) {
    awiYear = static_cast<int>(workerData.getDeathDate().year()) - 2;
  } else {
    // Check for entitlement after initial eligibility.
    if (static_cast<int>(workerData.getEntDate().getYear()) > piaData.getEligYear()){
      awiYear = workerData.getEntDate().getYear() - 2;
    }
    // Check for AERO recomputation after initial entitlement.
    if (workerData.getBenefitDate().getYear() > workerData.getEntDate().getYear()){
      for (int yr = static_cast<int>(workerData.getEntDate().getYear());
           yr <= min(static_cast<int>(workerData.getBenefitDate().getYear()-1),
           piaData.getIendTotal()); yr++) {
        if (piaData.earnOasdi[yr] > 0.005) {
          awiYear = yr-1;
        }
      }
    }
  }
  
  // calculate relative earnings position and special qc total for later comps.
  int qcTotalRel = 0;
  for (int yr = piaData.getIbeginTotal(); yr <= piaData.getIendTotal();
    yr++) {
    if (piaData.qcov.get(yr) > 0) {
      qcTotalRel += piaData.qcov.get(yr);
      // set proportioned wage base
      const double test = static_cast<double>(piaData.qcov.get(yr)) *
        piaParams.getBaseOasdi(yr) / 4.0;
      const double earnstt = (yr < workerData.getIbegin() ||
        yr > workerData.getIend()) ? 0.0 : piaData.earnOasdi[yr];
      const double temprep = min(earnstt, test) / piaParams.getFq(min(yr,awiYear));
      piaData.relEarnPosition[yr] =
        static_cast<double>(floor(temprep * 10000000.0 + 0.5) / 10000000.0);
      reptot += piaData.relEarnPosition[yr];
    } else {
      piaData.relEarnPosition[yr] = 0.0;
    }
  }
  piaData.setQcTotalRel(qcTotalRel);
  const double rv = floor(40000000.0 * reptot /
    static_cast<double>(piaData.getQcTotalRel()) + 0.5) / 10000000.0;
  const int temp = (ioasdi == WorkerData::SURVIVOR &&
    static_cast<int>(workerData.getDeathDate().year()) ==
    piaData.getEligYear()) ?
    piaData.getEligYear() + 1 : piaData.getEligYear();
  for (int yr = piaData.getIbeginTotal(); yr <= piaData.getIendTotal();
    yr++) {
    // fill in earnings from 22 to eligibility, plus other years with
    // at least 1 qc
    double earnstt = 0.0;
    const int birthYear = piaData.getBirthDateMinus1().year();
    if (((yr < birthYear + 22 || yr >= temp) && piaData.qcov.get(yr) > 0) ||
      (yr > birthYear + 21 && yr < temp)) {
      earnstt = rv * piaParams.getFq(min(yr,awiYear));
    }
    // prorate by quarter in year of death
    if (!workerData.getDeathDate().is_not_a_date()) {
      if (yr == static_cast<int>(workerData.getDeathDate().year())) {
        const int qtr = (workerData.getDeathDate().month() + 2) / 3;
        earnstt *= qtr / 4.0;
      }
    }
    piaData.earnTotalized[yr] = floor(100.0 * earnstt + 0.5) / 100.0;
  }
  return(rv);
}

/// <summary>Returns ending elapsed year for a beneficiary case.</summary>
///
/// <returns>Ending elapsed year for a beneficiary case.</returns>
///
/// <remarks>Assumes eligibility year has already been calculated.</remarks>
/// 
/// <param name="entDate">Entitlement date.</param>
int PiaCal::nelapsed2Cal( const DateMoyr& entDate )
{
  // account for 1960 ending year for 1960 or later method, nondisability
  int elap2 = (entDate.getYear() > 1960 && piaData.getEligYear() < 1961 &&
    workerData.getValdi() == 0) ? 1960 : piaData.getEligYear() - 1;
  if (ioasdi == WorkerData::SURVIVOR &&
    elap2 > static_cast<int>(workerData.getDeathDate().year()) - 1)
    elap2 = static_cast<int>(workerData.getDeathDate().year()) - 1;
  return(elap2);
}

/// <summary>Determine number of elapsed years.</summary>
///
/// <remarks>Calculates the ending elapsed year, then calls the version with
/// an ending elapsed year argument.</remarks>
///
/// <param name="compPeriod">Computation period to use (new or old).</param>
/// <param name="entDate">Entitlement date.</param>
void PiaCal::nelapsedCal( CompPeriod& compPeriod, const DateMoyr& entDate )
{
  // calculate last elapsed year
  int elapsed2 = nelapsed2Cal(entDate);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString()
      << ": In PiaCal::nelapsedCal, elapsed2 = " << elapsed2;
    Trace::writeLine(strm.str());
  }
#endif
  nelapsedCal(compPeriod, elapsed2);
}

/// <summary>Determine number of elapsed years.</summary>
///
/// <param name="compPeriod">Computation period to use (new or old).</param>
/// <param name="elapsed2">Ending elapsed year.</param>
void PiaCal::nelapsedCal( CompPeriod& compPeriod, int elapsed2 )
{
  // calculate first elapsed year
  const int elapsed1 =
    max(static_cast<int>(piaData.getBirthDateMinus1().year()) + 21,
    compPeriod.getBaseYear());
  compPeriod.setNElapsed(elapsed2 - elapsed1);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": In PiaCal::nelapsedCal, calling didropCal");
  }
#endif
  compPeriod.setDiYears(didropCal(workerData, elapsed1, elapsed2,
    piaData.partialFreezeYears));
  if (compPeriod.getDiYears() > 0) {
    compPeriod.setNElapsed(compPeriod.getNElapsed() -
      compPeriod.getDiYears());
  }
  if (compPeriod.getNElapsed() < 2)
    compPeriod.setNElapsed(2);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString()
      << ": At bottom of PiaCal::nelapsedCal, nelapsed = "
      << compPeriod.getNElapsed();
    Trace::writeLine(strm.str());
  }
#endif
}

/// <summary>Returns ending elapsed year for a non-freeze case.</summary>
///
/// <returns>Ending elapsed year for a non-freeze case.</returns>
///
/// <remarks>Assumes eligibility year has already been calculated.</remarks>
/// 
/// <param name="entDate">Entitlement date.</param>
int PiaCal::nelapsed2NonFreezeCal( const DateMoyr& entDate )
{
  // account for 1960 ending year for 1960 or later method, nondisability
  int elap2 = (entDate.getYear() > 1960 && piaData.getEligYearNonFreeze() < 1961 &&
    workerData.getValdi() == 0) ? 1960 : piaData.getEligYearNonFreeze() - 1;
  if (ioasdi == WorkerData::SURVIVOR &&
    elap2 > static_cast<int>(workerData.getDeathDate().year()) - 1)
    elap2 = static_cast<int>(workerData.getDeathDate().year()) - 1;
  return(elap2);
}

/// <summary>Determine number of elapsed years for a non-freeze case.</summary>
///
/// <remarks>Calculates the ending elapsed year, then calls the version with
/// an ending elapsed year argument.</remarks>
///
/// <param name="compPeriod">Computation period to use (new or old).</param>
/// <param name="entDate">Entitlement date.</param>
void PiaCal::nelapsedNonFreezeCal( CompPeriod& compPeriod, const DateMoyr& entDate )
{
  // calculate last elapsed year
  int elapsed2 = nelapsed2NonFreezeCal(entDate);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString()
      << ": In PiaCal::nelapsedCal, elapsed2 = " << elapsed2;
    Trace::writeLine(strm.str());
  }
#endif
  nelapsedNonFreezeCal(compPeriod, elapsed2);
}

/// <summary>Determine number of elapsed years.</summary>
///
/// <param name="compPeriod">Computation period to use (new or old).</param>
/// <param name="elapsed2">Ending elapsed year.</param>
void PiaCal::nelapsedNonFreezeCal( CompPeriod& compPeriod, int elapsed2 )
{
  // calculate first elapsed year
  const int elapsed1 =
    max(static_cast<int>(piaData.getBirthDateMinus1().year()) + 21,
    compPeriod.getBaseYear());
  compPeriod.setNElapsed(elapsed2 - elapsed1);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": In PiaCal::nelapsedNonFreezeCal, setting DI dropout years to zero.");
  }
#endif
  compPeriod.setDiYears(0);
  if (compPeriod.getNElapsed() < 2)
    compPeriod.setNElapsed(2);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString()
      << ": At bottom of PiaCal::nelapsedNonFreezeCal, nelapsed = "
      << compPeriod.getNElapsed();
    Trace::writeLine(strm.str());
  }
#endif
}

/// <summary>Determine number of dropout years, before considering 1-for-5
/// dropout.</summary>
///
/// <remarks>Assumes elapsed years have already been computed.</remarks>
///
/// <returns>Number of dropout years.</returns>
/// <param name="entDate">Entitlement date.</param>
int PiaCal::nDropCal( const DateMoyr& entDate )
{
  // if entitlement in June 1992 or later, use 5 dropout
  int NDrop = 5;
  if (entDate < PiaParams::amend90) {
    // if death is in Sept 1954 or later, use 5 dropout
    if (ioasdi == WorkerData::SURVIVOR) {
      // if disability freeze, use 5 dropout for disability in Sept 1954 or
      // later
      if (workerData.getValdi() > 0) {
        NDrop = (DateMoyr(workerData.disabPeriod[0].getOnsetDate()) >=
          PiaParams::amend54) ? 5 : 0;
      } else {
        NDrop = (DateMoyr(workerData.getDeathDate()) >=
          PiaParams::amend54) ? 5 : 0;
      }
    } else {
      // if disability freeze, use 5 dropout for disability in Sept 1954 or
      // later
      if (workerData.getValdi() > 0) {
        NDrop = (DateMoyr(workerData.disabPeriod[0].getOnsetDate()) >=
          PiaParams::amend54) ? 5 : 0;
      } else {
        // if entitlement is in Sept 1954 or later, use 5 dropout
        NDrop =
          (workerData.getEntDate() >= PiaParams::amend54) ? 5 : 0;
      }
    }
  }
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() <<
      ": Returning from PiaCal::nDropCal, NDrop = " << NDrop;
    Trace::writeLine(strm.str());
  }
#endif
  return NDrop;
}

/// <summary>Determine number of dropout years, before considering 1-for-5
/// dropout.</summary>
///
/// <remarks>Assumes elapsed years have already been computed.</remarks>
///
/// <returns>Number of dropout years.</returns>
/// <param name="entDate">Entitlement date.</param>
int PiaCal::nDropNonFreezeCal( const DateMoyr& entDate )
{
  // if entitlement in June 1992 or later, use 5 dropout
  int NDrop = 5;
  if (entDate < PiaParams::amend90)
  {
    // if death is in Sept 1954 or later, use 5 dropout
    if (ioasdi == WorkerData::SURVIVOR) {
      NDrop = (DateMoyr(workerData.getDeathDate()) >= PiaParams::amend54) ? 5 : 0;
    } else {
      if (ioasdi == WorkerData::OLD_AGE) {
        // if entitlement is in Sept 1954 or later, use 5 dropout
        NDrop = (workerData.getEntDate() >= PiaParams::amend54) ? 5 : 0;
      } else {
        // for DI cases, use 5 dropout.
        // I think that the 1954 change is irrelevant to DI non-freeze 
        // cases because DI didn't start until 1957, and non-freeze 
        // computations are based on the start of the waiting period.
        NDrop = 5;
     }
    }
  }
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() <<
      ": Returning from PiaCal::nDropNonFreezeCal, NDrop = " << NDrop;
    Trace::writeLine(strm.str());
  }
#endif
  return NDrop;
}

/// <summary>Determine number of computation years and dropout years.
/// </summary>
///
/// <remarks>Assumes elapsed years have already been computed.</remarks>
///
/// <param name="compPeriod">Computation period to use (new or old).</param>
/// <param name="entDate">Entitlement date.</param>
void PiaCal::nCal( CompPeriod& compPeriod, const DateMoyr& entDate )
{
  compPeriod.setNDrop(nDropCal(entDate));
  // Apply DI 1-for-5 rule if:
  // old-age case with prior disability entitlement in July 1980 or later
  // that lasted to age 61, or
  // disability case with entitlement in July 1980 or later
  if ((ioasdi == WorkerDataGeneral::OLD_AGE && workerData.getValdi() > 0 &&
    workerData.disabPeriod[0].getEntDate() >= PiaParams::amend80 &&
    workerData.disabPeriod[0].getCessationDate() >=
    DateMoyr(workerData.getBirthDate().month(),
    workerData.getBirthDate().year() + 61)) ||
    (workerData.getEntDate() >= PiaParams::amend80 &&
    ioasdi == WorkerData::DISABILITY)) {
    // 1-for-5 dropout rule for DI, if not survivor
    compPeriod.setNDrop(min(compPeriod.getNElapsed() / 5, 5));
  }
  compPeriod.setN(compPeriod.getNElapsed() - compPeriod.getNDrop());
  if (compPeriod.getN() < 2) {  // nx is minimum of 2
     compPeriod.setN(2);
     compPeriod.setNDrop(compPeriod.getNElapsed() - compPeriod.getN());
  }
  if (ioasdi == WorkerData::OLD_AGE &&
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

/// <summary>Determine number of computation years and dropout years.
/// </summary>
///
/// <remarks>Assumes elapsed years have already been computed.</remarks>
///
/// <param name="compPeriod">Computation period to use (new or old).</param>
/// <param name="entDate">Entitlement date.</param>
void PiaCal::nNonFreezeCal( CompPeriod& compPeriod, const DateMoyr& entDate )
{
  compPeriod.setNDrop(nDropNonFreezeCal(entDate));
  // Apply DI 1-for-5 rule if disability case with entitlement in July 1980 or later.
  // For other cases, we're disregarding the disability period in the
  // non-freeze computation.
  if (workerData.getEntDate() >= PiaParams::amend80 &&
    ioasdi == WorkerData::DISABILITY) {
    // 1-for-5 dropout rule for DI, if not survivor
    compPeriod.setNDrop(min(compPeriod.getNElapsed() / 5, 5));
  }
  compPeriod.setN(compPeriod.getNElapsed() - compPeriod.getNDrop());
  if (compPeriod.getN() < 2) {  // nx is minimum of 2
     compPeriod.setN(2);
     compPeriod.setNDrop(compPeriod.getNElapsed() - compPeriod.getN());
  }
  if (ioasdi == WorkerData::OLD_AGE &&
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
void PiaCal::earnProjection() const
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(
      workerData.getIdString() + ": At top of PiaCal::earnProjection");
  }
#endif
  piaData.earnProjection(workerData);
  piaData.earnHiCal(workerData);
  // limit earnings to wage base
  piaParams.getBaseOasdiArray().earnLimit(piaData.earnOasdiLimited,
    piaData.earnOasdi);
  piaParams.getBaseHiArray().earnLimit(piaData.earnHiLimited,
    piaData.earnHi);
}

/// <summary>Checks primary data for validity and does some preliminary
/// calculations.</summary>
///
/// <param name="entDate">Entitlement date.</param>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of many
/// different types if any data is out of range.</exception>
void PiaCal::dataCheck( const DateMoyr& entDate )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": At top of PiaCal::dataCheck");
  }
#endif
  workerData.joasdiCheck();
  // check for no earnings of any type
  workerData.earningsCheck();
  // check birthdate before anything whose check depends on it being valid.
  workerData.birth2Check();
  // check first year of earnings, if there are any earnings
  if (workerData.getIndearn())
    workerData.ibeginCheck();
  setIoasdi(workerData.getJoasdi());
  if (ioasdi != WorkerData::SURVIVOR) {
    workerData.entCheck();
    if (workerData.getRecalc()) {
      workerData.benefitDate2Check();
    }
    else {
      workerData.setBenefitDate();
    }
  }
  //if (piaData.getIndearn()) {
  //   earnProject.iendCheck(piaData.getIbegin(), piaData.getIend());
  //}
  piaData.setBirthDateMinus1(workerData.getBirthDate());
  piaData.fullRetAge =
    piaParams.fullRetAgeCal(piaData.getBirthDateMinus1().year() + 62);
  piaData.setFullRetDate();
  // calculate early retirement age
  if (ioasdi == WorkerData::OLD_AGE)  // only for old-age
    piaData.earlyRetAge = piaParams.earlyAgeOabCal(workerData.getSex(),
      piaData.getBirthDateMinus1());
  if (ioasdi != WorkerData::SURVIVOR) {
    // calculate age at entitlement
    piaData.ageEnt = workerData.getEntDate() -
      DateMoyr(piaData.getBirthDateMinus1());
    // calculate age at benefit date
    piaData.ageBen = workerData.getBenefitDate() -
      DateMoyr(piaData.getBirthDateMinus1());
  }
  workerData.qctdCheck2();
  workerData.qcCheck();
  // check noncovered pension
  if (getPubpenEnable(workerData)) {
    WorkerData::pubpenCheck(workerData.getPubpen());
    WorkerData::pubpenCheck(workerData.getPubpenReservist());
  }
  else {
    workerData.setPubpen(0.0f);
    workerData.setPubpenReservist(0.0f);
  }
  workerData.valdiCheck();
  if (workerData.getValdi()) {
    workerData.disCheck();
    if (DateMoyr(workerData.disabPeriod[0].getOnsetDate()) >=
      piaData.getFullRetDate()) {
      // disability after full retirement age
      throw PiaException(PIA_IDS_ONSET11);
    }
    if (ioasdi == WorkerDataGeneral::OLD_AGE && workerData.getValdi() > 0) {
      if (workerData.disabPeriod[0].getEntDate() >=
        piaData.getFullRetDate()) {
        // prior entitlement after full retirement age
        throw PiaException(PIA_IDS_PRRENT7);
      }
      workerData.priorentCheck();
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        Trace::writeLine(workerData.getIdString() +
          ": At end of workerData.priorentCheck of PiaCal::dataCheck");
      }
#endif
    }
    if (workerData.needDiCess()) {
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        ostringstream strm;
        strm << workerData.getIdString() << ", cessationDate "
          << DateFormatter::toString(
          workerData.disabPeriod[0].getCessationDate())
          << ", onsetDate "
          << DateFormatter::toString(workerData.disabPeriod[0].getOnsetDate())
          << ", deathDate "
          << DateFormatter::toString(workerData.getDeathDate())
          << ", entDate " << DateFormatter::toString(workerData.getEntDate())
          << ", joasdi " << static_cast<int>(workerData.getJoasdi())
          << ": Ready to call workerData.cessationCheck in PiaCal::dataCheck";
        Trace::writeLine(strm.str());
      }
#endif
      workerData.cessationCheck();
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        Trace::writeLine(workerData.getIdString() +
          ": Returned from workerData.cessationCheck in PiaCal::dataCheck");
      }
#endif
    }
    if (workerData.getValdi() > 1) {
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        ostringstream strm;
        strm << workerData.getIdString() << ", cessationDate 1 "
          << DateFormatter::toString(
          workerData.disabPeriod[1].getCessationDate())
          << ", onsetDate 1 "
          << DateFormatter::toString(workerData.disabPeriod[1].getOnsetDate())
          << ", entDate 1 "
          << DateFormatter::toString(workerData.disabPeriod[1].getEntDate())
          << ", onsetDate 0 "
          << DateFormatter::toString(workerData.disabPeriod[0].getOnsetDate())
          << ", entDate 0 "
          << DateFormatter::toString(workerData.disabPeriod[0].getEntDate())
          << ", deathDate "
          << DateFormatter::toString(workerData.getDeathDate())
          << ", entDate " << DateFormatter::toString(workerData.getEntDate())
          << ", joasdi " << static_cast<int>(workerData.getJoasdi())
          << ": Ready to call workerData.dis1Check in PiaCal::dataCheck";
        Trace::writeLine(strm.str());
      }
#endif
      workerData.dis1Check();
      workerData.cessation1Check();
    }
  }
  if (ioasdi == WorkerData::DISABILITY) {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() +
        ": Ready to call workerData.waitpdCheck in PiaCal::dataCheck");
    }
#endif
    workerData.waitpdCheck();
  }
  eligYearCal(workerData, piaData, ioasdi);
  if (ioasdi != WorkerData::SURVIVOR) {
    ageEntCheck();
  }
  nelapsedCal(piaData.compPeriodNew, entDate);
  nelapsedNonFreezeCal(piaData.compPeriodNewNonFreeze, entDate);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from PiaCal::dataCheck");
  }
#endif
}

/// <summary>Checks auxiliary data for validity and does some preliminary
/// calculations.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BENDATE6"/> if secondary entitlement
/// after benefit date.</exception>
///
/// <param name="widowDataArray">Basic information for all family
///  members.</param>
/// <param name="widowArray">Calculated data for all family members.</param>
/// <param name="secondaryArray">Benefits for all family members.</param>
void PiaCal::dataCheckAux( WorkerDataArray& widowDataArray,
PiaDataArray& widowArray, SecondaryArray& secondaryArray )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString()
      << ": At top of PiaCal::dataCheckAux, widowArray.getFamSize = "
      << widowArray.getFamSize();
    Trace::writeLine(strm.str());
  }
#endif
  // check auxiliary dates of entitlement against benefit date
  for (int i = 0; i < widowArray.getFamSize(); i++) {
    Secondary *secondaryptr = secondaryArray.secondary[i];
    if (secondaryptr->bic.getMajorBic() == ' ')
      break;
    try {
      WorkerData::benefitDate2Check(secondaryptr->entDate,
        workerData.getBenefitDate());
    } catch (PiaException&) {
      throw PiaException(PIA_IDS_BENDATE6);
    }
  }
  // check survivor information
  if (workerData.getJoasdi() == WorkerData::SURVIVOR) {
    // check date of birth against date of death
    workerData.deathCheck();
    // check for good survivor bics and dates of entitlement against death
    // date
    for (int i = 0; i < widowArray.getFamSize(); i++) {
      Secondary *secondaryptr = secondaryArray.secondary[i];
      if (secondaryptr->bic.getMajorBic() == ' ')
        break;
      Bic::survivorCheck(secondaryptr->bic.getMajorBic());
      WorkerData::deathCheck3(workerData.getDeathDate(),
        secondaryptr->entDate);
    }
  }
  for (int i = 0; i < widowArray.getFamSize(); i++) {
    Secondary *secondaryptr = secondaryArray.secondary[i];
    WorkerData *widowDataptr = widowDataArray.workerData[i];
    PiaData *widowPiaDataptr = widowArray.piaData[i];
    secondaryptr->check();
    if (secondaryptr->bic.isReducible()) {
      widowPiaDataptr->setBirthDateMinus1(widowDataptr->getBirthDate());
      // calculate spouse's or widow(er)'s age at entitlement
      widowPiaDataptr->ageEnt = secondaryptr->entDate -
        DateMoyr(widowPiaDataptr->getBirthDateMinus1());
      if (secondaryptr->bic.getMajorBic() == 'W') {
        widowCheck(*widowDataptr, *secondaryptr);
      }
    }
  }
  if (workerData.getJoasdi() != WorkerData::PEBS_CALC) {
    // determine widow's year of eligibility
    setEligYearWidow(widowDataArray, widowArray, secondaryArray);
    // calculate benefit factor for all family members
    for (int i = 0; i < widowArray.getFamSize(); i++) {
      WorkerData *widowDataptr = widowDataArray.workerData[i];
      PiaData *widowPiaDataptr = widowArray.piaData[i];
      Secondary *secondaryptr = secondaryArray.secondary[i];
      if (secondaryptr->bic.getMajorBic() == ' ') {
#if defined(DEBUGCASE)
        if (isDebugPid(workerData.getIdNumber())) {
          Trace::writeLine(workerData.getIdString() +
            ": Not calling ardriAuxCal because bic is blank");
        }
#endif
        break;
      }
      ardriAuxCal(widowDataptr, widowPiaDataptr, secondaryptr,
        secondaryptr->entDate);
    }
  }
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from PiaCal::dataCheckAux");
  }
#endif
}

/// <summary>Calculates the insured status code for one date.</summary>
///
/// <returns>The insured status code for one date.</returns>
///
/// <remarks>Requires adjusted date of birth, quarters of coverage filled in
/// from 1937 to date, total quarters of coverage for 1937 to 1950, dates of
/// disability onset and cessation. This version calls the static version with
/// 5 arguments.
///
/// The isWasPrimary argument has the following possible values:
/// <table>
///     0   never a primary beneficiary.
///     1   is a primary beneficiary.
///     2   is a deceased primary beneficiary.
/// </table></remarks>
///
/// <param name="dateModyyr">The date for which insured status code is
///  required.</param>
/// <param name="isWasPrimary">Worker's primary beneficiary status.</param>
InsCode::InsCodeType PiaCal::insCal( const boost::gregorian::date& dateModyyr,
int isWasPrimary )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": At top of PiaCal::insCal(dateModyyr, isWasPrimary)");
  }
#endif
  // set quarter and year of desired insured status
  QtrYear qtrYear(dateModyyr);
  piaData.qcTotalCal(workerData, qtrYear);
  InsCode::InsCodeType rv = insCal(workerData, piaData, ioasdi, dateModyyr,
    isWasPrimary);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from PiaCal::insCal(dateModyyr, isWasPrimary)");
  }
#endif
  return rv;
}

/// <summary>Calculates the insured status code for one date.</summary>
///
/// <returns>The insured status code for one date.</returns>
///
/// <remarks>Requires adjusted date of birth, quarters of coverage filled in
/// from 1937 to date, total quarters of coverage for 1937 to 1950, dates of
/// disability onset and cessation.
///
/// The isWasPrimary argument has the following possible values:
/// <table>
///     0   never a primary beneficiary.
///     1   is a primary beneficiary.
///     2   is a deceased primary beneficiary.
/// </table></remarks>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
/// <param name="qtrYear">The date for which insured status code is required.
/// </param>
/// <param name="isWasPrimary">Worker's primary beneficiary status.</param>
InsCode::InsCodeType PiaCal::insCal( const WorkerDataGeneral& workerData,
PiaData& piaData, WorkerDataGeneral::ben_type ioasdi, const QtrYear& qtrYear,
int isWasPrimary )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() + ": At top of PiaCal::" +
      "insCal(workerData, piaData, ioasdi, qtrYear, isWasPrimary)");
  }
#endif
  piaData.setQcReq(qcReqCal(workerData, piaData, ioasdi, qtrYear.getYear()));
  piaData.setQcReqPerm(qcReqPermCal(workerData, piaData, ioasdi));
  // calculate quarters of coverage for currently insured status
  piaData.setQcCurrent(qcCurrentCal(piaData, qtrYear));
  // deaths
  //if (isdead(date))
  //   return(deadIns(isWasPrimary));
  InsCode::InsCodeType rv = fins1Cal(workerData, piaData, qtrYear,
    isWasPrimary);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() + ": Returning from PiaCal::" +
      "insCal(workerData, piaData, ioasdi, qtrYear, isWasPrimary)");
  }
#endif
  return rv;
}

/// <summary>Calculates the insured non-freeze status code for one date.</summary>
///
/// <returns>The insured non-freeze status code for one date.</returns>
///
/// <remarks>Requires adjusted date of birth, quarters of coverage filled in
/// from 1937 to date, total quarters of coverage for 1937 to 1950, dates of
/// disability onset and cessation. This version calls the static version with
/// 5 arguments.
///
/// The isWasPrimary argument has the following possible values:
/// <table>
///     0   never a primary beneficiary.
///     1   is a primary beneficiary.
///     2   is a deceased primary beneficiary.
/// </table></remarks>
///
/// <param name="dateModyyr">The date for which non-freeze insured status code is
///  required.</param>
/// <param name="isWasPrimary">Worker's primary beneficiary status.</param>
InsCode::InsCodeType PiaCal::insNonFreezeCal( const boost::gregorian::date& dateModyyr,
int isWasPrimary )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": At top of PiaCal::insNonFreezeCal(dateModyyr, isWasPrimary)");
  }
#endif
  // set quarter and year of desired insured status
  QtrYear qtrYear(dateModyyr);
  piaData.qcTotalNonFreezeCal(workerData, qtrYear);
  InsCode::InsCodeType rv = insNonFreezeCal(workerData, piaData, ioasdi, dateModyyr,
    isWasPrimary);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from PiaCal::insCal(dateModyyr, isWasPrimary)");
  }
#endif
  return rv;
}

/// <summary>Calculates the insured non-freeze status code for one date.</summary>
///
/// <returns>The insured non-freeze status code for one date.</returns>
///
/// <remarks>Requires adjusted date of birth, quarters of coverage filled in
/// from 1937 to date, total quarters of coverage for 1937 to 1950, dates of
/// disability onset and cessation.
///
/// The isWasPrimary argument has the following possible values:
/// <table>
///     0   never a primary beneficiary.
///     1   is a primary beneficiary.
///     2   is a deceased primary beneficiary.
/// </table></remarks>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
/// <param name="qtrYear">The date for which insured non-freeze status code is required.
/// </param>
/// <param name="isWasPrimary">Worker's primary beneficiary status.</param>
InsCode::InsCodeType PiaCal::insNonFreezeCal( const WorkerDataGeneral& workerData,
PiaData& piaData, WorkerDataGeneral::ben_type ioasdi, const QtrYear& qtrYear,
int isWasPrimary )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() + ": At top of PiaCal::" +
      "insCal(workerData, piaData, ioasdi, qtrYear, isWasPrimary)");
  }
#endif
  piaData.setQcReqNonFreeze(qcReqNonFreezeCal(workerData, piaData, ioasdi, qtrYear.getYear()));
  piaData.setQcReqPermNonFreeze(qcReqPermNonFreezeCal(workerData, piaData, ioasdi));
  // calculate quarters of coverage for currently insured status
  piaData.setQcCurrentNonFreeze(qcCurrentNonFreezeCal(piaData, qtrYear));
  // deaths
  //if (isdead(date))
  //   return(deadIns(isWasPrimary));
  InsCode::InsCodeType rv = finsNonFreeze1Cal(workerData, piaData, qtrYear,
    isWasPrimary);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() + ": Returning from PiaCal::" +
      "insNonFreezeCal(workerData, piaData, ioasdi, qtrYear, isWasPrimary)");
  }
#endif
  return rv;
}

/// <summary>Calculates the date fully insured status was first attained
/// (at full retirement age or later).</summary>
///
/// <remarks>First checks for fully insured status as of full retirement age.
/// If uninsured at that time, checks every future quarter through the quarter
/// of the benefit date. For this purpose, being insured for totalization
/// benefits is considered to be fully insured.
///
/// Requires adjusted date of birth, quarters of coverage filled in from 1937
/// to date, total quarters of coverage from 1937 to 1950, dates of disability
/// onset and cessation.
///
/// This is called from <see cref="monthsDriCal"/>.</remarks>
///
/// <returns>The date fully insured status was first attained
/// (at full retirement age or later).</returns>
DateMoyr PiaCal::fullInsDateCal() const
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": At top of PiaCal::fullInsDateCal");
  }
#endif
  // quarter and year of insured status (start with date full retirement
  // age is attained)
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", piaData.getFullRetDate() "
      << DateFormatter::toString(piaData.getFullRetDate())
      << ": Creating qtrYear";
    Trace::writeLine(strm.str());
  }
#endif
  QtrYear qtrYear(piaData.getFullRetDate());
  do {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm << workerData.getIdString() << ", qtrYear "
        << qtrYear.toString() << ": Calling piaData.qcTotalCal";
      Trace::writeLine(strm.str());
    }
#endif
    piaData.qcTotalCal(workerData, qtrYear);
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm << workerData.getIdString() << ", qcTotal "
        << piaData.getQcTotal() << ", qtrYear " << qtrYear.toString()
        << ": Calling insCal";
      Trace::writeLine(strm.str());
    }
#endif
    const InsCode::InsCodeType rv =
      insCal(workerData, piaData, ioasdi, qtrYear, 1);
    if (InsCode(rv).isFullyOrTotalInsured()) {
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        Trace::writeLine(workerData.getIdString() + ", fullInsDate " +
          DateFormatter::toString(qtrYear.toDateMoyr()) +
          ": Successfully returning from PiaCal::fullInsDateCal");
      }
#endif
      return qtrYear.toDateMoyr();
    }
    else {
      // advance to next quarter and try again
      qtrYear.add(1);
    }
  } while (qtrYear.toDateMoyr() < workerData.getBenefitDate());
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() + ", fullInsDate " +
      DateFormatter::toString(qtrYear.toDateMoyr()) +
      ": Unsuccessfully returning from PiaCal::fullInsDateCal");
  }
#endif
  // stop checking at benefit date
  return qtrYear.toDateMoyr();
}

/// <summary>Sets eligibility year for widow.</summary>
///
/// <param name="widowDataArray">Basic information for all family members.
/// </param>
/// <param name="widowArray">Calculated data for all family members.</param>
/// <param name="secondaryArray">Benefits for all family members.</param>
void PiaCal::setEligYearWidow( WorkerDataArray& widowDataArray,
PiaDataArray& widowArray, SecondaryArray& secondaryArray ) const
{
  for (int i = 0; i < widowArray.getFamSize(); i++) {
    Secondary * secondaryptr = secondaryArray.secondary[i];
    if (secondaryptr->bic.isWidow()) {
      WorkerData * widowDataptr = widowDataArray.workerData[i];
      PiaData * widowPiaDataptr = widowArray.piaData[i];
      widowPiaDataptr->setEligYear(eligYearWidowCal(
        secondaryptr->bic.getMajorBic(),
        widowPiaDataptr->getBirthDateMinus1().year(),
        widowDataptr->disabPeriod[0].getOnsetDate()));
    }
  }
}

/// <summary>Returns eligibility year for widow(er).</summary>
///
/// <returns>Eligibility year for widow(er).</returns>
///
/// <remarks>The type of widow(er) is as follows:
/// <table>
///     W   disabled widow(er).
///     D   aged widow(er).
/// </table></remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> if the onset
/// date is not a date in a disabled widow(er) case.</exception>
///
/// <param name="majorBic">Type of widow(er).</param>
/// <param name="byear">Adjusted year of birth of widow.</param>
/// <param name="onsetDate">The date of onset of disability (unused if not
/// disabled widow(er)).</param>
int PiaCal::eligYearWidowCal( char majorBic, int byear,
boost::gregorian::date onsetDate )
{
  try {
    if (majorBic == 'W') {
      return(max(static_cast<int>(onsetDate.year()), byear + 50));
    }
    else {
      return(byear + 60);
    }
  } catch (exception& e) {
    throw PiaException(string("In PiaCal::eligYearWidowCal: ") + e.what());
  }
}

/// <summary>Applies maximum family benefit to each family benefit, after
/// ratio has been determined.</summary>
///
/// <param name="secondaryArray">Benefits for all family members.</param>
/// <param name="famSize">Number of family members.</param>
/// <param name="ratio">Ratio to be applied to each family benefit.</param>
/// <param name="year">Year of rounding.</param>
void PiaCal::applyMfb( SecondaryArray& secondaryArray, int famSize,
double ratio, int year )
{
  // do not increase benefits
  if (ratio > 1.0)
    ratio = 1.0;
  // do not set benefits less than zero
  if (ratio < 0.0)
    ratio = 0.0;
  // apply ratio to full benefit
  for (int i1 = 0; i1 < famSize; i1++) {
    Secondary *secondary = secondaryArray.secondary[i1];
    if (secondary->eligibleForMax()) {
      secondary->setBenefit(BenefitAmount::round(
        ratio * secondary->getFullBenefit(), year));
    }
    else {
      secondary->setBenefit(secondary->getFullBenefit());
    }
  }
}

/// <summary>Calculates PIA for all re-indexed widow(er)s.</summary>
///
/// <remarks> This should be called immediately after <see cref="piaCal"/> in
/// survivor cases. (It doesn't hurt to call it in other than survivor cases,
/// but it will simply return without doing anything.)</remarks>
///
/// <param name="widowDataArray">Basic information for all family members.
/// </param>
/// <param name="widowArray">Calculated information for all family members.
/// </param>
/// <param name="secondaryArray">Benefits for all family members.</param>
void PiaCal::reindWidCalAll( const WorkerDataArray& widowDataArray,
PiaDataArray& widowArray, SecondaryArray& secondaryArray )
{
  for (int i = 0; i < PiaDataArray::MAXFAMSIZE; i++) {
    if (secondaryArray.secondary[i]->isWidow()) {
      reindWidCal(*widowDataArray.workerData[i], *widowArray.piaData[i],
        *secondaryArray.secondary[i]);
    }
  }
}

/// <summary>Sets indicator for entitlement after 1990 amendments.</summary>
///
/// <param name="entDate">Entitlement date.</param>
void PiaCal::setAmend90( const DateMoyr& entDate )
{
  // use 1990 methods if entitlement in June 1992 or later, unless dib
  // conversion with dib entitlement prior to June 1992
  const bool b1 = (entDate >= PiaParams::amend90);
  const bool b2 = (workerData.getValdi() == 0 ||
    workerData.disabPeriod[0].getCessationDate() < entDate);
  piaData.setAmend90(b1 && b2);
}

/// <summary>Resets indicator for earnings used after 1991.</summary>
void PiaCal::resetAmend90()
{
  try {
    // if already true, return
    if (piaData.getAmend90()) {
      return;
    }
    // if pia table method unused, return
    if (piaTable == static_cast<PiaTable *>(0) ||
      piaTable->getApplicable() == 0) {
      return;
    }
    const int lastYear = piaTable->iorder.getLastYear();
    bool b1 = false;
    // check for any earnings in 1992 or later
    for (int i = 1992; i <= lastYear; i++) {
      if (piaTable->iorder[i] > 0 && piaTable->earnIndexed[i] > 0.5) {
        b1 = true;
      }
    }
    piaData.setAmend90(b1);
  } catch (exception& e) {
    throw PiaException(string("In PiaCal::resetAmend90: ") + e.what());
  }
}

/// <summary>Returns true if ok to enable noncovered pension entry box.
/// </summary>
///
/// <returns>True if ok to enable noncovered pension entry box.</returns>
///
/// <remarks>Note that this is a static function.</remarks>
///
/// <param name="workerData">Worker's basic data.</param>
bool PiaCal::getPubpenEnable( const WorkerDataGeneral& workerData )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Starting PiaCal::getPubpenEnable");
  }
#endif
  if (workerData.getBirthDate().is_not_a_date()) {
    throw PiaException("Birth date out of range");
  }
  try {
    boost::gregorian::date birthDate1 = workerData.getBirthDate() -
      boost::gregorian::date_duration(1);
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm << workerData.getIdString()
        << ": in PiaCal::getPubpenEnable.  Joasdi = "
        << workerData.getJoasdi();
      Trace::writeLine(strm.str());
    }
#endif
    return (workerData.getJoasdi() != WorkerData::SURVIVOR &&
      birthDate1.year() + 62 > 1985);
  } catch (PiaException&) {
    // if date of birth has not yet been set, do not allow noncovered
    // pension entry
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() + ": exception in " +
        "PiaCal::getPubpenEnable.  Returning false by default.");
    }
#endif
    return false;
  }
}

/// <summary>Calculates required quarters of coverage for fully insured
/// status at end of year.</summary>
///
/// <remarks>Note that this is a static function.</remarks>
///
/// <returns>Required quarters of coverage for fully insured status at end of
/// year.</returns>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
/// <param name="year">Year for which number is required.</param>
int PiaCal::qcReqCal( const WorkerDataGeneral& workerData, PiaData& piaData,
WorkerDataGeneral::ben_type ioasdi, int year )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": At top of PiaCal::qcReqCal");
  }
#endif
  // set last elapsed year by looking at year of eligibility to old-age
  // benefits
  piaData.setEligDate(eligYearCal1(workerData, piaData, ioasdi, 0));
  const int elapsed2 =
    min(year, static_cast<int>(piaData.getEligDate().getYear()) - 1);
  // set first elapsed year
  const int elapsed1 = max(
    static_cast<int>(piaData.getBirthDateMinus1().year() + 21), 1950);
  // check for years wholly or partially within period of disability
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": In PiaCal::qcReqCal, calling didropCal");
  }
#endif
  const int diYears = didropCal(workerData, elapsed1, elapsed2,
    piaData.partialFreezeYears);
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from PiaCal::qcReqCal");
  }
#endif
  return min(40, max(6, elapsed2 - elapsed1 - diYears));
}

/// <summary>Calculates required quarters of coverage for fully insured
/// status for non-freeze at end of year.</summary>
///
/// <remarks>Note that this is a static function.</remarks>
///
/// <returns>Required quarters of coverage for fully insured status 
/// for non-freeze at end of year.</returns>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
/// <param name="year">Year for which number is required.</param>
int PiaCal::qcReqNonFreezeCal( const WorkerDataGeneral& workerData, PiaData& piaData,
WorkerDataGeneral::ben_type ioasdi, int year )
{
  int diYears = 0;
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": At top of PiaCal::qcReqNonFreezeCal");
  }
#endif
  // set last elapsed year by looking at year of eligibility to old-age
  // benefits
  piaData.setEligDate(eligYearCal1(workerData, piaData, ioasdi, 0));
  const int elapsed2 =
    min(year, static_cast<int>(piaData.getEligDate().getYear()) - 1);
  // set first elapsed year
  const int elapsed1 = max(
    static_cast<int>(piaData.getBirthDateMinus1().year() + 21), 1950);
  if (ioasdi == WorkerDataGeneral::DISABILITY) {
    diYears = didropNonFreezeCal(workerData, elapsed1, elapsed2);
  }
  return min(40, max(6, elapsed2 - elapsed1 - diYears));
}

/// <summary>Calculates required quarters of coverage for permanently
/// insured status.</summary>
///
/// <remarks>Note that this is a static function.</remarks>
///
/// <returns>Required quarters of coverage for permanently insured status.
/// </returns>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
int PiaCal::qcReqPermCal( const WorkerDataGeneral& workerData,
PiaData& piaData, WorkerDataGeneral::ben_type ioasdi )
{
  int didropout = 0;
  const int elap1 = max(
    static_cast<int>(piaData.getBirthDateMinus1().year() + 21), 1950);
  piaData.setEligDate(eligYearCal1(workerData, piaData, ioasdi, 0));
  const int elap2 = piaData.getEligDate().getYear() - 1;
  if (workerData.getValdi()) {
    FreezeYears frzyrst;  // temporary freeze years
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      Trace::writeLine(workerData.getIdString() +
        ": In PiaCal::qcReqPermCal, calling didropCal");
    }
#endif
    didropout = didropCal(workerData, elap1, elap2, frzyrst);
  }
  return min(40, max(6, elap2 - elap1 - didropout));
}

/// <summary>Calculates required quarters of coverage for permanently
/// insured status for non-freeze.</summary>
///
/// <remarks>Note that this is a static function.</remarks>
///
/// <returns>Required quarters of coverage for permanently insured status
///  for non-freeze.</returns>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
int PiaCal::qcReqPermNonFreezeCal( const WorkerDataGeneral& workerData,
PiaData& piaData, WorkerDataGeneral::ben_type ioasdi )
{
  int diYears = 0;
  const int elap1 = max(
    static_cast<int>(piaData.getBirthDateMinus1().year() + 21), 1950);
  piaData.setEligDate(eligYearCal1(workerData, piaData, ioasdi, 0));
  const int elap2 = piaData.getEligDate().getYear() - 1;
  if (ioasdi == WorkerDataGeneral::DISABILITY) {
    diYears = didropNonFreezeCal(workerData, elap1, elap2);
  }
  return min(40, max(6, elap2 - elap1 - diYears));
}

/// <summary>Calculates quarters of coverage earned for currently insured
/// status.</summary>
///
/// <remarks>Note that this is a static function.</remarks>
///
/// <returns>Quarters of coverage earned for currently insured status.
/// </returns>
///
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="qtrYear">Calendar quarter and year through which qc's are
///  required.</param>
int PiaCal::qcCurrentCal( const PiaData& piaData, const QtrYear& qtrYear )
{
#if !defined(NDEBUG)
  qtrYear.check();
#endif
  // starting quarter and year of measuring period
  QtrYear startQtrYear = qtrYear;
  // use 13-quarter measuring period
  startQtrYear.subtract(12);
  if (startQtrYear < PiaParams::year37qtr1)
    startQtrYear = PiaParams::year37qtr1;
  const int tempqccur = piaData.qcov.accumulate(startQtrYear, qtrYear, 0u);
  return tempqccur;
}

/// <summary>Calculates quarters of coverage earned for currently insured
/// status for non-freeze.</summary>
///
/// <remarks>Note that this is a static function.</remarks>
///
/// <returns>Quarters of coverage earned for currently insured status for non-freeze.
/// </returns>
///
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="qtrYear">Calendar quarter and year through which qc's are
///  required.</param>
int PiaCal::qcCurrentNonFreezeCal( const PiaData& piaData, const QtrYear& qtrYear )
{
#if !defined(NDEBUG)
  qtrYear.check();
#endif
  // starting quarter and year of measuring period
  QtrYear startQtrYear = qtrYear;
  // use 13-quarter measuring period
  startQtrYear.subtract(12);
  if (startQtrYear < PiaParams::year37qtr1)
    startQtrYear = PiaParams::year37qtr1;
  const int tempqccur = piaData.qcov.accumulate(startQtrYear, qtrYear, 0u);
  return tempqccur;
}

/// <summary>Returns the insured status code for a deceased worker.</summary>
///
/// <remarks>Note that this is a static function.
///
/// The iswasPrimary argument has the following possible values:
/// <table>
///     0   never a primary beneficiary.
///     1   is a primary beneficiary.
///     2   is a deceased primary beneficiary.
/// </table></remarks>
///
/// <returns>The insured status code for a deceased worker.</returns>
///
/// <param name="iswasPrimary">Worker's primary beneficiary status.</param>
InsCode::InsCodeType PiaCal::deadIns( int iswasPrimary )
{
  return((iswasPrimary == 2) ?
    InsCode::PRIMARYDEATH : InsCode::NONPRIMARYDEATH);
}

/// <summary>Computes year of eligibility.</summary>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
void PiaCal::eligYearCal( const WorkerDataGeneral& workerData,
PiaData& piaData, WorkerDataGeneral::ben_type ioasdi )
{
  // determine worker's month and year of eligibility
  piaData.setEligDate(eligYearCal1(workerData, piaData, ioasdi, 0));
  // take disability into account
  piaData.setEligYear(eligYearCal2(workerData, piaData, ioasdi,
    piaData.getEligDate().getYear()));
  // also set for non-freeze computations
  piaData.setEligYearNonFreeze(eligYearCal3(workerData, piaData, ioasdi,
    piaData.getEligDate().getYear()));
}

/// <summary>Computes date of eligibility before considering disability.
/// </summary>
///
/// <remarks>Note that this is a static function.</remarks>
///
/// <returns>The date of eligibility before considering disability.</returns>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
/// <param name="jind7">0 for present law, 1-3 for change to age-65 comp point.
/// </param>
DateMoyr PiaCal::eligYearCal1( const WorkerDataGeneral& workerData,
PiaData& piaData, WorkerDataGeneral::ben_type ioasdi, int jind7 )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": At top of PiaCal::eligYearCal1");
  }
#endif
#if !defined(NDEBUG)
  if (piaData.getBirthDateMinus1().is_not_a_date()) {
    throw PiaException("birthDateMinus1 is not a date");
  }
#endif
  const int birthYear = piaData.getBirthDateMinus1().year();
  // start with month and year of age 62, plus any additional amount
  DateMoyr eligDateTemp(piaData.getBirthDateMinus1().month(),
    birthYear + 62 + jind7);
  // account for age-65 computation point for older male workers
  if (workerData.isMale()) {
    if (birthYear < 1914 && birthYear > 1909)
      eligDateTemp = DateMoyr(12, 1975);
    if (birthYear < 1910)
      eligDateTemp = DateMoyr(eligDateTemp.getMonth(), birthYear + 65);
  }
  if (ioasdi == WorkerData::SURVIVOR) {
    DateMoyr deathDateTemp(workerData.getDeathDate());
    if (deathDateTemp < eligDateTemp) {
      // for survivors, use month and year of death, if earlier
      eligDateTemp = deathDateTemp;
    }
  }
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from PiaCal::eligYearCal1");
  }
#endif
  return eligDateTemp;
}

/// <summary>Computes year of eligibility after considering disability.
/// </summary>
///
/// <remarks>Note that this is a static function.</remarks>
///
/// <returns>Year of eligibility after considering disability.</returns>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="ioasdi">Type of benefit (old-age, survivor, or disability).
/// </param>
/// <param name="eligYear">Year of attainment of age 62, for old-age and
///     disability, or year of death, for survivor.</param>
int PiaCal::eligYearCal2( const WorkerDataGeneral& workerData,
PiaData& piaData, WorkerDataGeneral::ben_type ioasdi, int eligYear )
{
  int rv = eligYear;
  if (workerData.getValdi() > 0) {
    // determine which year of onset to use
    int onsetyear;
    const DisabPeriod& disabPeriod0 = workerData.disabPeriod[0];
    const DisabPeriod& disabPeriod1 = workerData.disabPeriod[1];
    if (workerData.getValdi() < 2 ||
      disabPeriod1.getCessationDate().getMonths(
      disabPeriod0.getOnsetDate()) > 12) {
      onsetyear = disabPeriod0.getOnsetDate().year();
    }
    else {
      onsetyear = disabPeriod1.getOnsetDate().year();
    }
    if (ioasdi != WorkerData::DISABILITY) {
      // consider disability onset prior to eligibility
      if (disabPeriod0.getCessationDate().getMonths(
        piaData.getEligDate()) < 13 && onsetyear < rv) {
        rv = onsetyear;
      }
    }
    if (ioasdi == WorkerData::DISABILITY && rv > onsetyear) {
      rv = onsetyear;
    }
  }
  return(rv);
}

/// <summary>Computes year of eligibility for non-freeze computations.
/// </summary>
///
/// <remarks>Note that this is a static function.</remarks>
///
/// <returns>Year of eligibility for non-freeze computations.</returns>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="ioasdi">Type of benefit (old-age, survivor, or disability).
/// </param>
/// <param name="eligYear">Year of attainment of age 62, for old-age and
///     disability, or year of death, for survivor.</param>
int PiaCal::eligYearCal3( const WorkerDataGeneral& workerData,
PiaData& piaData, WorkerDataGeneral::ben_type ioasdi, int eligYear )
{
  int rv = eligYear;
  if (ioasdi == WorkerData::DISABILITY) {
    int waitYear = workerData.disabPeriod[0].getWaitperDate().getYear();
    // consider disability onset prior to eligibility
    if (waitYear < rv) {
      rv = waitYear;
    }
  }
  return(rv);
}

/// <summary>Calculates and returns the insured status code for one date.
/// </summary>
///
/// <remarks>Requires that total and required quarters of coverage have been
/// calculated, and that worker is alive. Note that this is a static function.
///
/// The iswasPrimary argument has the following possible values:
/// <table>
///     0   never a primary beneficiary.
///     1   is a primary beneficiary.
/// </table></remarks>
///
/// <returns>The insured status code for one date.</returns>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="qtrYear">Quarter and year of desired insured status.</param>
/// <param name="iswasPrimary">Worker's primary beneficiary status.</param>
InsCode::InsCodeType PiaCal::fins1Cal( const WorkerDataGeneral& workerData,
PiaData& piaData, const QtrYear& qtrYear, int iswasPrimary )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", qcTotal " << piaData.getQcTotal()
      << ", qcReq " << piaData.getQcReq() << ": At top of PiaCal::fins1Cal";
    Trace::writeLine(strm.str());
  }
#endif
  if (piaData.getQcTotal() < piaData.getQcReq()) {
    // insured for totalization benefits
    if (iswasPrimary > 0 && workerData.getTotalize()) {
      return(InsCode::TOTALIZED);
    }
    // check for deemed insured status
    if (workerData.getDeemedind() && qtrYear.getYear() > 1983) {
      if (deemedInsCal(piaData, qtrYear)) {
        return(InsCode::DEEMED);
      }
    }
    // not fully insured
    if (piaData.getQcTotal() == 0) {
      return(InsCode::NOQCS);
    }
    if (piaData.getQcCurrent() > 5) {
      // currently insured
      return(InsCode::CURRENTNOTFULL);
    } else {
      if (piaData.getQcTotal() > 2 && piaData.getQcTotal() +
        (workerData.isMale() ? 1887 : 1890) >
        static_cast<int>(piaData.getBirthDateMinus1().year())) {
        // transitionally insured
        return(InsCode::TRANSITIONAL);
      } else {
        //uninsured in all ways
        return(InsCode::UNINSURED);
      }
    }
  } else {
    // fully insured
    if (piaData.getQcTotal() < piaData.getQcReqPerm()) {
      // not permanently insured
      if (piaData.getQcCurrent() > 5) {
        // currently insured
        return(InsCode::FULLANDCURRENT);
      } else {
        return(InsCode::FULLNOTCURRENT);
      }
    } else {
      if (piaData.getQcCurrent() > 5) {
        // currently insured
        return(InsCode::PERMANDCURRENT);
      } else {
        return(InsCode::PERMNOTCURRENT);
      }
    }
  }
}

/// <summary>Calculates and returns the insured status code for non-freeze
/// for one date.</summary>
///
/// <remarks>Requires that total and required quarters of coverage have been
/// calculated, and that worker is alive. Note that this is a static function.
///
/// The iswasPrimary argument has the following possible values:
/// <table>
///     0   never a primary beneficiary.
///     1   is a primary beneficiary.
/// </table></remarks>
///
/// <returns>The insured status code for one date.</returns>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="qtrYear">Quarter and year of desired insured status.</param>
/// <param name="iswasPrimary">Worker's primary beneficiary status.</param>
InsCode::InsCodeType PiaCal::finsNonFreeze1Cal( const WorkerDataGeneral& workerData,
PiaData& piaData, const QtrYear& qtrYear, int iswasPrimary )
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", qcTotalNonFreeze " 
      << piaData.getQcTotalNonFreeze()
      << ", qcReq " << piaData.getQcReqNonFreeze() 
      << ": At top of PiaCal::finsNonFreeze1Cal";
    Trace::writeLine(strm.str());
  }
#endif
  if (piaData.getQcTotalNonFreeze() < piaData.getQcReqNonFreeze()) {
    // insured for totalization benefits
    if (iswasPrimary > 0 && workerData.getTotalize()) {
      return(InsCode::TOTALIZED);
    }
    // check for deemed insured status
    if (workerData.getDeemedind() && qtrYear.getYear() > 1983) {
      if (deemedInsCal(piaData, qtrYear)) {
        return(InsCode::DEEMED);
      }
    }
    // not fully insured
    if (piaData.getQcTotalNonFreeze() == 0) {
      return(InsCode::NOQCS);
    }
    if (piaData.getQcCurrentNonFreeze() > 5) {
      // currently insured
      return(InsCode::CURRENTNOTFULL);
    } else {
      if (piaData.getQcTotalNonFreeze() > 2 && piaData.getQcTotalNonFreeze() +
        (workerData.isMale() ? 1887 : 1890) >
        static_cast<int>(piaData.getBirthDateMinus1().year())) {
        // transitionally insured
        return(InsCode::TRANSITIONAL);
      } else {
        //uninsured in all ways
        return(InsCode::UNINSURED);
      }
    }
  } else {
    // fully insured
    if (piaData.getQcTotalNonFreeze() < piaData.getQcReqPermNonFreeze()) {
      // not permanently insured
      if (piaData.getQcCurrentNonFreeze() > 5) {
        // currently insured
        return(InsCode::FULLANDCURRENT);
      } else {
        return(InsCode::FULLNOTCURRENT);
      }
    } else {
      if (piaData.getQcCurrentNonFreeze() > 5) {
        // currently insured
        return(InsCode::PERMANDCURRENT);
      } else {
        return(InsCode::PERMNOTCURRENT);
      }
    }
  }
}

/// <summary>Calculates and returns the OACT insured status code for one
/// date.</summary>
///
/// <remarks>Requires that CWHS insured status code has been calculated.
/// Note that this is a static function.</remarks>
///
/// <returns>The OACT insured status code, as follows:
/// <table>
///   1   Fully insured in nontotalization case.
///   2   Not fully insured, but currently insured, survivor.
///   3   Totalization case with more than 5 qc's, but not fully insured.
///   4   Not fully insured nontotalization case other than survivor.
///   5   Not fully or currently insured survivor.
///   6   Totalization case with less than 6 qc's.
///   7   Fully insured totalization case.
/// </table></returns>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
char PiaCal::fins2Cal( const WorkerDataGeneral& workerData,
const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi )
{
  if (ioasdi == WorkerData::SURVIVOR) {
    if (piaData.finsCode.isFullyInsured()) {
      return('1');
    } else {
      return(piaData.finsCode.isCurrentlyInsured() ? '2' : '5');
    }
  } else {
    if (workerData.getTotalize()) {
      if (piaData.getQcTotal() < 6) {
        return('6');
      }
      return(piaData.finsCode.isFullyInsured() ? '7' : '3');
    } else {
      return(piaData.finsCode.isFullyInsured() ? '1' : '4');
    }
  }
}

/// <summary>Calculates and returns the OACT insured status code for
/// non-freeze for one date.</summary>
///
/// <remarks>Requires that CWHS insured status code has been calculated.
/// Note that this is a static function.</remarks>
///
/// <returns>The OACT insured status code, as follows:
/// <table>
///   1   Fully insured in nontotalization case.
///   2   Not fully insured, but currently insured, survivor.
///   3   Totalization case with more than 5 qc's, but not fully insured.
///   4   Not fully insured nontotalization case other than survivor.
///   5   Not fully or currently insured survivor.
///   6   Totalization case with less than 6 qc's.
///   7   Fully insured totalization case.
/// </table></returns>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
char PiaCal::finsNonFreeze2Cal( const WorkerDataGeneral& workerData,
const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi )
{
  if (ioasdi == WorkerData::SURVIVOR) {
    if (piaData.finsNonFreezeCode.isFullyInsured()) {
      return('1');
    } else {
      return(piaData.finsNonFreezeCode.isCurrentlyInsured() ? '2' : '5');
    }
  } else {
    if (workerData.getTotalize()) {
      if (piaData.getQcTotalNonFreeze() < 6) {
        return('6');
      }
      return(piaData.finsNonFreezeCode.isFullyInsured() ? '7' : '3');
    } else {
      return(piaData.finsNonFreezeCode.isFullyInsured() ? '1' : '4');
    }
  }
}

/// <summary>Checks for deemed insured status, assuming eligible for deemed
/// insured status provision to apply.</summary>
///
/// <returns>True if deemed insured, false otherwise.</returns>
///
/// <param name="piaData">Worker's calculated data.</param>
/// <param name="qtrYear">Quarter and year for which insured status code
/// is required.</param>
bool PiaCal::deemedInsCal( PiaData& piaData, const QtrYear& qtrYear )
{
  // determine number of QC's required
  piaData.setDeemedQcReq(PiaParams::deemedQcReqCal(
    piaData.getBirthDateMinus1().year()));
  if (piaData.getDeemedQcReq() < 0) {
    return(false);
  }
#if !defined(NDEBUG)
  qtrYear.check();
#endif
  const int tempQctot =
    piaData.qcov.accumulate(PiaParams::qtr184, qtrYear, 0u);
  return (tempQctot >= piaData.getDeemedQcReq());
}

/// <summary>Calculates the disability insured status code for one year.
/// </summary>
///
/// <remarks>Note that this is a static function.
///
/// The iswasPrimary argument has the following possible values:
/// <table>
///     0   never a primary beneficiary.
///     1   is a primary beneficiary.
///     2   is a deceased primary beneficiary.
/// </table></remarks>
///
/// <returns>The disability insured status code.</returns>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="dateMoyr">The date for which insured status code is required.
/// </param>
/// <param name="iswasPrimary">Worker's primary beneficiary status.</param>
DisInsCode::DisInsCodeType PiaCal::disInsCal(
const WorkerDataGeneral& workerData, PiaData& piaData,
const DateMoyr& dateMoyr, int iswasPrimary )
{
  // handle the cases that do not require any computation first
  if (workerData.isdead(dateMoyr)) {
    // deaths
    return (iswasPrimary == 2) ?
      DisInsCode::PRIMARYDEATH : DisInsCode::NONPRIMARYDEATH;
  }
  // check for no quarters of coverage
  if (piaData.getQcTotal() == 0) {
    return DisInsCode::NOQCS;
  }
  // set quarter and year after attainment of age 21
  QtrYear tempQtrYear(piaData.getBirthDateMinus1());
  tempQtrYear.add(QtrYear(1,21));
  // look at period after cessation
  int trial = 0;
  const DisabPeriod& disabPeriod = workerData.disabPeriod[0];
  piaData.qcDisReq2Cal(workerData, dateMoyr);
  if (piaData.getQcDisQtr2() < 40 && workerData.getValdi() > 0 &&
    disabPeriod.getWaitperDate().getYear() > 0) {
    trial = 4 * (disabPeriod.getWaitperDate().getYear() -
      disabPeriod.getOnsetDate().year()) +
      (disabPeriod.getWaitperDate().getMonth() + 2) / 3 -
      (disabPeriod.getOnsetDate().month() + 2) / 3;
  }
  // try ending quarters from onset to waiting period, in reverse order so
  // end up at earliest age (to see if special test is required)
  for (int i = trial; i >= 0; i--) {
    piaData.qcDisReqCal(workerData, dateMoyr, i, false, false);
    piaData.qcDisCal(workerData);
    if (piaData.getQcTotalDis() >= piaData.getQcDisReq()) {
      break;
    }
  }
  // check special insured status, if necessary and beginning quarter is
  // before age 21
  if (piaData.getQcTotalDis() < piaData.getQcDisReq() &&
    ((piaData.getQcDisDate5().getYear() > 0 &&
    piaData.getQcDisDate5() < tempQtrYear) ||
    (piaData.getQcDisDate5().getYear() == 0 &&
    piaData.getQcDisDate1() < tempQtrYear))) {
    for (int i = 0; i <= trial; i++) {
      piaData.qcDisReqCal(workerData, dateMoyr, i, true, false);
      piaData.qcDisCal(workerData);
      if (piaData.getQcTotalDis() >= piaData.getQcDisReq()) {
        break;
      }
    }
  }
  // if still not insured, try using freeze period in previous disability
  if (piaData.getQcTotalDis() < piaData.getQcDisReq() &&
    piaData.getQcDisDate5().getYear() > 0) {
    for (int i = trial; i >= 0; i--) {
      piaData.qcDisReqCal(workerData, dateMoyr, i, false, true);
      piaData.qcDisCal(workerData);
      if (piaData.getQcTotalDis() >= piaData.getQcDisReq()) {
        break;
      }
    }
  }
  // check special insured status, if necessary and beginning quarter is
  // before age 21, using freeze period in previous disability
  if (piaData.getQcTotalDis() < piaData.getQcDisReq() &&
    piaData.getQcDisDate5().getYear() > 0 &&
    piaData.getQcDisDate5() < tempQtrYear) {
    for (int i = 0; i <= trial; i++) {
      piaData.qcDisReqCal(workerData, dateMoyr, i, true, true);
      piaData.qcDisCal(workerData);
      if (piaData.getQcTotalDis() >= piaData.getQcDisReq()) {
        break;
      }
    }
  }
  // not fully insured
  if (piaData.getQcTotal() < piaData.getQcReq()) {
    if (iswasPrimary == 1 && workerData.getTotalize()) {
      return(DisInsCode::TOTALIZED);  // totalization benefits
    }
    if (piaData.getQcTotalDis() < piaData.getQcDisReq()) {
      return(DisInsCode::NOTFULLNOT2040);  // uninsured in all ways
    } else {
      return(DisInsCode::NOTFULLHAS2040);  // not fully insured, meets 20/40
    }
  } else {
    // fully insured
    if (piaData.getQcTotalDis() < piaData.getQcDisReq()) {
      if (iswasPrimary == 1 && workerData.getTotalize()) {
        return(DisInsCode::TOTALIZED);  // totalization benefits
      }
      if (workerData.getBlindind()) {
        return(DisInsCode::FULLANDBLIND);  // fully insured (blindness)
      } else {
        // fully insured, not 20/40 insured
        return(DisInsCode::FULLNOT2040);
      }
    } else {
      // primary beneficiary
      if (iswasPrimary == 1) {
        return(DisInsCode::FULLANDDIB);  // currently a DIB
      } else {
        if (piaData.getQcDisQtr() == 40) {
          // 20/40 disability insured status
          return(DisInsCode::FULLAND2040);
        } else {
          // special disability insured status
          return(DisInsCode::FULLANDSPECIAL);
        }
      }
    }
  }
}

/// <summary>Calculates the disability insured status code for non-freeze
/// for one year.</summary>
///
/// <remarks>Note that this is a static function.
///
/// The iswasPrimary argument has the following possible values:
/// <table>
///     0   never a primary beneficiary.
///     1   is a primary beneficiary.
///     2   is a deceased primary beneficiary.
/// </table></remarks>
///
/// <returns>The disability insured status code.</returns>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
/// <param name="dateMoyr">The date for which insured status code is required.
/// </param>
/// <param name="iswasPrimary">Worker's primary beneficiary status.</param>
DisInsCode::DisInsCodeType PiaCal::disInsNonFreezeCal(
const WorkerDataGeneral& workerData, PiaData& piaData,
const DateMoyr& dateMoyr, int iswasPrimary )
{
  // handle the cases that do not require any computation first
  if (workerData.isdead(dateMoyr)) {
    // deaths
    return (iswasPrimary == 2) ?
      DisInsCode::PRIMARYDEATH : DisInsCode::NONPRIMARYDEATH;
  }
  // check for no quarters of coverage
  if (piaData.getQcTotal() == 0) {
    return DisInsCode::NOQCS;
  }
  // set quarter and year after attainment of age 21
  QtrYear tempQtrYear(piaData.getBirthDateMinus1());
  tempQtrYear.add(QtrYear(1,21));
  // try 20/40 beginning with waiting period, before checking
  // special test.
  piaData.qcDisReqNonFreezeCal(workerData, dateMoyr, 0, false, false);
  piaData.qcDisNonFreezeCal(workerData);
  // check special insured status, if necessary and beginning quarter is
  // before age 21
  if (piaData.getQcTotalDisNonFreeze() < piaData.getQcDisReqNonFreeze() &&
    (piaData.getQcDisDateNonFreeze1() < tempQtrYear)) {
      piaData.qcDisReqNonFreezeCal(workerData, dateMoyr, 0, true, false);
      piaData.qcDisNonFreezeCal(workerData);
  }
  // not fully insured
  if (piaData.getQcTotalNonFreeze() < piaData.getQcReqNonFreeze()) {
    if (iswasPrimary == 1 && workerData.getTotalize()) {
      return(DisInsCode::TOTALIZED);  // totalization benefits
    }
    if (piaData.getQcTotalDisNonFreeze() < piaData.getQcDisReqNonFreeze()) {
      return(DisInsCode::NOTFULLNOT2040);  // uninsured in all ways
    } else {
      return(DisInsCode::NOTFULLHAS2040);  // not fully insured, meets 20/40
    }
  } else {
    // fully insured
    if (piaData.getQcTotalDisNonFreeze() < piaData.getQcDisReqNonFreeze()) {
      if (iswasPrimary == 1 && workerData.getTotalize()) {
        return(DisInsCode::TOTALIZED);  // totalization benefits
      }
      if (workerData.getBlindind()) {
        return(DisInsCode::FULLANDBLIND);  // fully insured (blindness)
      } else {
        // fully insured, not 20/40 insured
        return(DisInsCode::FULLNOT2040);
      }
    } else {
      // primary beneficiary
      if (iswasPrimary == 1) {
        return(DisInsCode::FULLANDDIB);  // currently a DIB
      } else {
        if (piaData.getQcDisQtrNonFreeze() == 40) {
          // 20/40 disability insured status
          return(DisInsCode::FULLAND2040);
        } else {
          // special disability insured status
          return(DisInsCode::FULLANDSPECIAL);
        }
      }
    }
  }
}

/// <summary>Calculates number of years to drop out of elapsed period due to
/// disability.</summary>
///
/// <returns>Number of years to drop out of elapsed period due to disability.
/// </returns>
///
/// <remarks>Note that this is a static function.</remarks>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="elapsed1">First year of elapsed period.</param>
/// <param name="elapsed2">Last year of elapsed period.</param>
/// <param name="frzyrst">Freeze years information.</param>
int PiaCal::didropCal( const WorkerDataGeneral& workerData, int elapsed1,
int elapsed2, FreezeYears& frzyrst )
{
  int rv = 0;  // return value
  frzyrst.deleteContents();
  if (workerData.getValdi() == 0)
    return(rv);
  // check latest period of disability
  const int onsetYear = workerData.disabPeriod[0].getOnsetDate().year();
  const int cessYear =
    workerData.disabPeriod[0].getCessationDate().getYear();
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ": In PiaCal::didropCal, elapsed2 = "
      << elapsed2 << ", onsetYear = " << onsetYear;
    Trace::writeLine(strm.str());
  }
#endif
  if (onsetYear <= elapsed2) {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm << workerData.getIdString()
        << ": In PiaCal::didropCal, setting year1 to max of "
        << onsetYear << " and " << (elapsed1 + 1);
      Trace::writeLine(strm.str());
    }
#endif
    frzyrst.setYear1(max(onsetYear, elapsed1 + 1));
    // set last year of di dropout period
    frzyrst.setYear2((cessYear >= onsetYear && cessYear <= elapsed2) ?
      cessYear : elapsed2);
    rv = frzyrst.getYear2() - frzyrst.getYear1() + 1;
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm << workerData.getIdString() << ": In PiaCal::didropCal, "
        << "after most recent disability, rv = " << rv;
      Trace::writeLine(strm.str());
    }
#endif
  }
  // check prior period of disability
  if (workerData.getValdi() == 2) {
    const int onset1Year = workerData.disabPeriod[1].getOnsetDate().year();
    const int cess1Year =
      workerData.disabPeriod[1].getCessationDate().getYear();
    if (onset1Year <= elapsed2 && (frzyrst.getYear1() == 0 ||
      onset1Year < frzyrst.getYear1())) {
      // set first year of prior di dropout period
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        ostringstream strm;
        strm << workerData.getIdString()
          << ": In PiaCal::didropCal, setting year3 to max of "
          << onset1Year << " and " << (elapsed1 + 1);
        Trace::writeLine(strm.str());
      }
#endif
      frzyrst.setYear3(max(onset1Year, elapsed1 + 1));
      // set last year of prior di dropout period
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        ostringstream strm;
        strm << workerData.getIdString()
          << ": In PiaCal::didropCal, setting year4 to " << cess1Year;
        Trace::writeLine(strm.str());
      }
#endif
      frzyrst.setYear4((cess1Year >= onset1Year && cess1Year <= elapsed2) ?
        cess1Year : elapsed2);
      if (frzyrst.getYear1() > 0 &&
        frzyrst.getYear4() >= frzyrst.getYear1()) {
        frzyrst.setYear4(frzyrst.getYear1() - 1);
      }
      rv += frzyrst.getYear4() - frzyrst.getYear3() + 1;
    }
  }
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ": In PiaCal::didropCal, year1 = "
      << frzyrst.getYear1() << ", year2 = " << frzyrst.getYear2()
      << ", returning " << rv;
    Trace::writeLine(strm.str());
  }
#endif
  return(rv);
}

/// <summary>Calculates number of years to drop out of elapsed period for
/// fully insured status due to disability in a non-freeze computation.</summary>
///
/// <returns>Number of years to drop out of elapsed period for fully insured
/// status due to disability in a non-freeze computation.</returns>
///
/// <remarks>Note that this is a static function.  
/// Also note that this should only be used for computing
/// fully insured status in a non-freeze computation.</remarks>
///
/// <param name="workerData">Worker's basic data.</param>
/// <param name="elapsed1">First year of elapsed period.</param>
/// <param name="elapsed2">Last year of elapsed period.</param>
int PiaCal::didropNonFreezeCal( const WorkerDataGeneral& workerData, 
  int elapsed1, int elapsed2 )
{
  int rv = 0;  // return value
  if (workerData.getValdi() == 0)
    return(rv);
  // check latest period of disability
  const int waitYear = workerData.disabPeriod[0].getWaitperDate().getYear();
  int year1 = 0;
  int year2 = 0;
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ": In PiaCal::didropNonFreezeCal, elapsed2 = "
      << elapsed2 << ", waitYear = " << waitYear;
    Trace::writeLine(strm.str());
  }
#endif
  if (waitYear <= elapsed2) {
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm << workerData.getIdString()
        << ": In PiaCal::didropNonFreezeCal, setting year1 to max of "
        << waitYear << " and " << (elapsed1 + 1);
      Trace::writeLine(strm.str());
    }
#endif
    year1 = (max(waitYear, elapsed1 + 1));
    // set last year of di dropout period
    year2 = elapsed2;
    rv = year2 - year1 + 1;
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm << workerData.getIdString() << ": In PiaCal::didropCal, "
        << "based on most recent waiting period, rv = " << rv;
      Trace::writeLine(strm.str());
    }
#endif
  }
  return(rv);
}

/// <summary>Releases memory.</summary>
void PiaCal::releaseMemory()
{
  // clear general methods
  for (vector< PiaMethod * >::iterator iter1 = piaMethod.begin();
  iter1 != piaMethod.end(); iter1++) {
    delete *iter1;
  }
  piaMethod.clear();
  wageInd = static_cast<WageInd *>(0);
  oldStart = static_cast<OldStart *>(0);
  specMin = static_cast<SpecMin *>(0);
  piaTable = static_cast<PiaTable *>(0);
  highPiaMethod = static_cast<PiaMethod *>(0);
  frozMin = static_cast<FrozMin *>(0);
  childCareCalc = static_cast<ChildCareCalc *>(0);
  transGuar = static_cast<TransGuar *>(0);
  dibGuar = static_cast<DibGuar *>(0);
  wageIndNonFreeze = static_cast<WageIndNonFreeze *>(0);
  // clear widow methods
  for (vector< ReindWid * >::iterator iter2 = widowMethod.begin();
    iter2 != widowMethod.end(); iter2++) {
    delete *iter2;
  }
  widowMethod.clear();
}

/// <summary>Calculates simplified quarters of coverage, 1937-50.</summary>
///
/// <returns>Simplified quarters of coverage, 1937-50.</returns>
///
/// <param name="earn3750">Total earnings in the period 1937-50.
/// </param>
int PiaCal::qc3750simpCal( double earn3750 )
{
  return min(static_cast<int>(earn3750 / 400.0), 56);
}
