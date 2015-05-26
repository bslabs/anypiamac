// Functions for the <see cref="WageInd2Page"/> class to manage a wage
// indexed reuslts page.

// $Id: WageInd2Page.cpp 1.26.1.3 2012/09/06 10:39:41EDT 277133 Development  $

#include <iomanip>
#include "WageInd2Page.h"
#include "WageInd.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "UserAssumptions.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes WageInd2Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
WageInd2Page::WageInd2Page( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaParams(newPiaCal.piaParams), piaCal(newPiaCal),
userAssumptions(newPiaCal.userAssumptions)
{ }

/// <summary>Destructor.</summary>
WageInd2Page::~WageInd2Page()
{ }

/// <summary>Prepares wage indexed strings for printing.</summary>
void WageInd2Page::prepareStrings()
{
  try {
    const WageInd* wageInd = (const WageInd*)piaCal.wageInd;
    if (wageInd != (const WageInd*)0 && wageInd->getApplicable() > 0) {
      pageTitle[0] = wageInd->getTitle();
      outputString.clear();
      prepareHeader();
      baseYearOut(piaData.getEligYear() - 2);
      compYear(workerData, piaData, piaData.compPeriodNew);
      strm.precision(2);
      if (workerData.getTotalize()) {
        strm.str("");
        strm << "Theoretical AIME = " << wageInd->getftearn() << "/("
          << piaData.compPeriodNew.getN() << "*12) = " << setprecision(0)
          << wageInd->getAmeTotal();
        outputString.push_back(strm.str());
        outputString.push_back("");
        strm.str("");
        strm << "Pro Rata AIME = " << wageInd->getAme();
      }
      else {
        strm.str("");
        strm << "AIME = " << wageInd->getftearn() << "/("
          << piaData.compPeriodNew.getN() << "*12) = " << setprecision(0)
          << wageInd->getAme();
      }
      outputString.push_back(strm.str());
      outputString.push_back("");
      piaBpOut(wageInd->bendPia);
      mfbBpOut(wageInd->bendMfb);
      double dtemp;
      if ((wageInd->getWindfall() == PiaMethod::HAS30YEARS ||
        wageInd->getWindfall() == PiaMethod::NOWINDFALLELIM) &&
        !workerData.getTotalize()) {
        dtemp = wageInd->piaElig[wageInd->getCpiYear(PiaMethod::FIRST_YEAR)];
      }
      else {
        if (wageInd->getWindfall() == PiaMethod::REDUCEDPERC ||
          wageInd->getWindfall() == PiaMethod::ONEHALFPENSION) {
          dtemp = wageInd->getPiaWindfall().get();
        }
        else {
          dtemp = wageInd->getPiaEligTotal().get();
        }
      }
      piaEligOut(wageInd->percPia, wageInd->portionAime, dtemp);
      if (wageInd->getWindfall() != PiaMethod::NOWINDFALLELIM) {
        pubPenOut(*wageInd,
          static_cast<double>(piaData.getPubpenApp(workerData)),
          workerData.getPubpenDate());
      }
      if (workerData.getTotalize()) {
        if (wageInd->getCpiYear(PiaMethod::YEAR_ENT) >=
          wageInd->getCpiYear(PiaMethod::YEAR_ELIG)) {
          biOut(*wageInd, piaParams, piaData.getEligYear(), 1, 1);
        }
        if (wageInd->getCpiYear(PiaMethod::YEAR_BEN) >
          wageInd->getCpiYear(PiaMethod::YEAR_ENT)) {
          biOut(*wageInd, piaParams, piaData.getEligYear(), 2, 1);
        }
        totPiaOut(*wageInd, piaData.getQcTotalRel(),
          piaData.compPeriodNew.getN());
        rawPiaOut(
          wageInd->piaElig[wageInd->getCpiYear(PiaMethod::FIRST_YEAR)]);
        outputString.push_back("");
      }
      mfbOut(*wageInd, wageInd->portionPiaElig, wageInd->percMfb,
        wageInd->getAme());
      const int i1 = (workerData.getTotalize()) ? 2 : 3;
      if (wageInd->getCpiYear(PiaMethod::YEAR_ENT) >=
        wageInd->getCpiYear(PiaMethod::YEAR_ELIG)) {
        biOut(*wageInd, piaParams, piaData.getEligYear(), 1, i1);
      }
      if (wageInd->getCpiYear(PiaMethod::YEAR_BEN) >
        wageInd->getCpiYear(PiaMethod::YEAR_ENT)) {
        piamfb1(*wageInd);
        biOut(*wageInd, piaParams, piaData.getEligYear(), 2, i1);
      }
      if (piaData.getEligYear() > UserAssumptions::getIstart() &&
        userAssumptions.getIaltaw() == AssumptionType::PEBS_ASSUM) {
        printRealWageGain(piaData.getEligYear(),
          UserAssumptions::getIstart(), *wageInd,
          wageInd->piaRealWageGain.get(), wageInd->mfbRealWageGain.get());
      }
      piamfb2(*wageInd);
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
