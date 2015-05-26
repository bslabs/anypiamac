// Functions for the <see cref="ChildCareCalc2Page"/> class to manage wage
// indexed results with child-care dropout years.

// $Id: ChildCareCalc2Page.cpp 1.21.1.2 2012/08/28 07:58:33EDT 277133 Development  $

#include <iomanip>
#include "ChildCareCalc2Page.h"
#include "ChildCareCalc.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "UserAssumptions.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes ChildCareCalc2Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
ChildCareCalc2Page::ChildCareCalc2Page( const PiaCalAny& newPiaCal ) :
PiaPageOut(), workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaParams(newPiaCal.piaParams), piaCal(newPiaCal),
userAssumptions(newPiaCal.userAssumptions)
{ }

/// <summary>Destructor.</summary>
ChildCareCalc2Page::~ChildCareCalc2Page()
{ }

/// <summary>Prepares wage indexed strings with childcare dropout years for
/// printing.</summary>
void ChildCareCalc2Page::prepareStrings()
{
  try {
    const ChildCareCalc* childCareCalc = piaCal.childCareCalc;
    if (childCareCalc != (const ChildCareCalc*)0 &&
      childCareCalc->getApplicable() > 0) {
      pageTitle[0] = childCareCalc->getTitle();
      const int year0 = childCareCalc->getCpiYear(PiaMethod::FIRST_YEAR);
      const int year1 = childCareCalc->getCpiYear(PiaMethod::YEAR_ELIG);
      const int year2 = childCareCalc->getCpiYear(PiaMethod::YEAR_ENT);
      const int year3 = childCareCalc->getCpiYear(PiaMethod::YEAR_BEN);
      outputString.clear();
      prepareHeader();
      baseYearOut(piaData.getEligYear() - 2);
      compYear(workerData, piaData, piaData.compPeriodNew);
      // prepare childcare dropout years
      strm.str("");
      strm << "Maximum number of childcare dropout years = "
        << childCareCalc->getChildCareDropMax();
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.str("");
      strm << "Number of usable childcare dropout years = "
        << childCareCalc->getChildCareDrop();
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.str("");
      strm << "Adjusted number of computation years = "
        << childCareCalc->getAdjustedN();
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.str("");
      strm.precision(2);
      strm << "AIME = " << childCareCalc->getftearn() << "/("
        << childCareCalc->getAdjustedN() << "*12) = " << setprecision(0)
        << childCareCalc->getAme();
      outputString.push_back(strm.str());
      outputString.push_back("");
      piaBpOut(childCareCalc->bendPia);
      mfbBpOut(childCareCalc->bendMfb);
      const double dtemp =
        (childCareCalc->getWindfall() == PiaMethod::HAS30YEARS ||
        childCareCalc->getWindfall() == PiaMethod::NOWINDFALLELIM) ?
        childCareCalc->piaElig[year0] :
        childCareCalc->getPiaWindfall().get();
      piaEligOut(childCareCalc->percPia, childCareCalc->portionAime, dtemp);
      if (childCareCalc->getWindfall() != PiaMethod::NOWINDFALLELIM) {
        pubPenOut(*childCareCalc,
          static_cast<double>(piaData.getPubpenApp(workerData)),
          workerData.getPubpenDate());
      }
      mfbOut(*childCareCalc, childCareCalc->portionPiaElig,
        childCareCalc->percMfb, childCareCalc->getAme());
      if (year2 >= year1) {
        biOut(*childCareCalc, piaParams, piaData.getEligYear(), 1, 3);
      }
      if (year3 > year2) {
        piamfb1(*childCareCalc);
        biOut(*childCareCalc, piaParams, piaData.getEligYear(), 2, 3);
      }
      if (piaData.getEligYear() > UserAssumptions::getIstart() &&
        userAssumptions.getIaltaw() == AssumptionType::PEBS_ASSUM) {
        printRealWageGain(piaData.getEligYear(),
          UserAssumptions::getIstart(), *childCareCalc,
          childCareCalc->piaRealWageGain.get(),
          childCareCalc->mfbRealWageGain.get());
      }
      piamfb2(*childCareCalc);
      // print noninsured message
      nonIns(piaData.getFinsCode2(),workerData,piaData);
      if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY) {
        disInsOut(piaData);
      }
      outputString.push_back("");
      printNameSsn(workerData);
    }
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_WISTRING);
  }
}
