// Functions for the <see cref="WageIndNonFreeze2Page"/> class to manage a wage
// indexed results page.

// $Id: WageIndNonFreeze2Page.cpp 1.2 2014/01/05 07:54:56EST 277133 Development  $

#include <iomanip>
#include "WageIndNonFreeze2Page.h"
#include "WageIndNonFreeze.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "UserAssumptions.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes WageIndNonFreeze2Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
WageIndNonFreeze2Page::WageIndNonFreeze2Page( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaParams(newPiaCal.piaParams), piaCal(newPiaCal),
userAssumptions(newPiaCal.userAssumptions)
{ }

/// <summary>Destructor.</summary>
WageIndNonFreeze2Page::~WageIndNonFreeze2Page()
{ }

/// <summary>Prepares wage indexed strings for printing.</summary>
void WageIndNonFreeze2Page::prepareStrings()
{
  try {
    const WageIndNonFreeze* wageIndNonFreeze = (const WageIndNonFreeze*)piaCal.wageIndNonFreeze;
    if (wageIndNonFreeze != (const WageIndNonFreeze*)0 && wageIndNonFreeze->getApplicable() > 0) {
      pageTitle[0] = wageIndNonFreeze->getTitle();
      outputString.clear();
      prepareHeader();
      baseYearOut(piaData.getEligYearNonFreeze() - 2);
      strm.str("");
      strm << "Number of di dropout years = " 
        << piaData.compPeriodNewNonFreeze.getDiYears();
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.str("");
      strm << "Number of elapsed years = " 
        << piaData.compPeriodNewNonFreeze.getNElapsed();
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.str("");
      strm << "Number of dropout years = " 
        << piaData.compPeriodNewNonFreeze.getNDrop();
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.str("");
      strm << "Number of computation years = " 
        << piaData.compPeriodNewNonFreeze.getNElapsed()
        << " - " << piaData.compPeriodNewNonFreeze.getNDrop() 
        << " = " << piaData.compPeriodNewNonFreeze.getN();
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.precision(2);
      if (workerData.getTotalize()) {
        strm.str("");
        strm << "Theoretical AIME = " << wageIndNonFreeze->getftearn() << "/("
          << piaData.compPeriodNewNonFreeze.getN() << "*12) = " 
          << setprecision(0) << wageIndNonFreeze->getAmeTotal();
        outputString.push_back(strm.str());
        outputString.push_back("");
        strm.str("");
        strm << "Pro Rata AIME = " << wageIndNonFreeze->getAme();
      }
      else {
        strm.str("");
        strm << "AIME = " << wageIndNonFreeze->getftearn() << "/("
          << piaData.compPeriodNewNonFreeze.getN() << "*12) = " << setprecision(0)
          << wageIndNonFreeze->getAme();
      }
      outputString.push_back(strm.str());
      outputString.push_back("");
      piaBpOut(wageIndNonFreeze->bendPia);
      mfbBpOut(wageIndNonFreeze->bendMfb);
      double dtemp;
      if ((wageIndNonFreeze->getWindfall() == PiaMethod::HAS30YEARS ||
        wageIndNonFreeze->getWindfall() == PiaMethod::NOWINDFALLELIM) &&
        !workerData.getTotalize()) {
        dtemp = wageIndNonFreeze->piaElig[wageIndNonFreeze->getCpiYear(PiaMethod::FIRST_YEAR)];
      }
      else {
        if (wageIndNonFreeze->getWindfall() == PiaMethod::REDUCEDPERC ||
          wageIndNonFreeze->getWindfall() == PiaMethod::ONEHALFPENSION) {
          dtemp = wageIndNonFreeze->getPiaWindfall().get();
        }
        else {
          dtemp = wageIndNonFreeze->getPiaEligTotal().get();
        }
      }
      piaEligOut(wageIndNonFreeze->percPia, wageIndNonFreeze->portionAime, dtemp);
      if (wageIndNonFreeze->getWindfall() != PiaMethod::NOWINDFALLELIM) {
        pubPenOut(*wageIndNonFreeze,
          static_cast<double>(piaData.getPubpenApp(workerData)),
          workerData.getPubpenDate());
      }
      if (workerData.getTotalize()) {
        if (wageIndNonFreeze->getCpiYear(PiaMethod::YEAR_ENT) >=
          wageIndNonFreeze->getCpiYear(PiaMethod::YEAR_ELIG)) {
          biOut(*wageIndNonFreeze, piaParams, piaData.getEligYear(), 1, 1);
        }
        if (wageIndNonFreeze->getCpiYear(PiaMethod::YEAR_BEN) >
          wageIndNonFreeze->getCpiYear(PiaMethod::YEAR_ENT)) {
          biOut(*wageIndNonFreeze, piaParams, piaData.getEligYear(), 2, 1);
        }
        totPiaOut(*wageIndNonFreeze, piaData.getQcTotalRel(),
          piaData.compPeriodNew.getN());
        rawPiaOut(
          wageIndNonFreeze->piaElig[wageIndNonFreeze->getCpiYear(PiaMethod::FIRST_YEAR)]);
        outputString.push_back("");
      }
      mfbOut(*wageIndNonFreeze, wageIndNonFreeze->portionPiaElig, wageIndNonFreeze->percMfb,
        wageIndNonFreeze->getAme());
      const int i1 = (workerData.getTotalize()) ? 2 : 3;
      if (wageIndNonFreeze->getCpiYear(PiaMethod::YEAR_ENT) >=
        wageIndNonFreeze->getCpiYear(PiaMethod::YEAR_ELIG)) {
        biOut(*wageIndNonFreeze, piaParams, piaData.getEligYear(), 1, i1);
      }
      if (wageIndNonFreeze->getCpiYear(PiaMethod::YEAR_BEN) >
        wageIndNonFreeze->getCpiYear(PiaMethod::YEAR_ENT)) {
        piamfb1(*wageIndNonFreeze);
        biOut(*wageIndNonFreeze, piaParams, piaData.getEligYear(), 2, i1);
      }
      if (piaData.getEligYear() > UserAssumptions::getIstart() &&
        userAssumptions.getIaltaw() == AssumptionType::PEBS_ASSUM) {
        printRealWageGain(piaData.getEligYear(),
          UserAssumptions::getIstart(), *wageIndNonFreeze,
          wageIndNonFreeze->piaRealWageGain.get(), wageIndNonFreeze->mfbRealWageGain.get());
      }
      piamfb2(*wageIndNonFreeze);
      // print noninsured message
      nonIns(piaData.getFinsCode2(),workerData,piaData);
      if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY)
        disInsOut(piaData);
      outputString.push_back("");
      printNameSsn(workerData);
    }
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_WISTRING);
  }
}
