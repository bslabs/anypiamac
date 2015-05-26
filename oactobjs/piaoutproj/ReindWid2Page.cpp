// Functions for the <see cref="ReindWid2Page"/> class to manage a
// re-indexed widow calculation results page.

// $Id: ReindWid2Page.cpp 1.26.1.2 2012/08/28 07:58:36EDT 277133 Development  $

#include <iomanip>
#include "ReindWid2Page.h"
#include "ReindWid.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "UserAssumptions.h"
#include "DateFormatter.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes ReindWid2Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
ReindWid2Page::ReindWid2Page( const PiaCalAny& newPiaCal ) :
PiaPageOut(), workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaParams(newPiaCal.piaParams), piaCal(newPiaCal),
userAssumptions(newPiaCal.userAssumptions)
{ }

/// <summary>Destructor.</summary>
ReindWid2Page::~ReindWid2Page()
{ }

/// <summary>Prepares reindexed widow strings for printing.</summary>
void ReindWid2Page::prepareStrings()
{
  try {
    string s;  // temporary string
    if (reindWid != (const ReindWid*)0 && reindWid->getApplicable() > 0) {
      const WorkerDataGeneral& widowData = reindWid->widowData;
      const Secondary& secondary = reindWid->secondary;
      pageTitle[0] = reindWid->getTitle();
      outputString.clear();
      prepareHeader();
      outputString.push_back("Widow(er) information:");
      printNameSsn(widowData);
      AfxFormatString1(s, PIA_IDS_DOBOUT,
        DateFormatter::toString(widowData.getBirthDate()));
      outputString.push_back(s);
      outputString.push_back("");
      if (secondary.bic.getMajorBic() == 'W') {
        AfxFormatString1(s, PIA_IDS_WIDDIBOUT,
          DateFormatter::toString(widowData.disabPeriod[0].getOnsetDate()));
        outputString.push_back(s);
        outputString.push_back("");
      }
      baseYearOut(reindWid->getEligYear() - 2);
      compYear(workerData, piaData, piaData.compPeriodNew);
      strm.precision(2);
      strm.str("");
      strm << "AIME = " << reindWid->getftearn() << "/("
        << piaData.compPeriodNew.getN() << "*12) = " << setprecision(0)
        << reindWid->getAme();
      outputString.push_back(strm.str());
      outputString.push_back("");
      piaBpOut(reindWid->bendPia);
      piaEligOut(reindWid->percPia, reindWid->portionAime,
        reindWid->piaElig[reindWid->getCpiYear(PiaMethod::FIRST_YEAR)]);
      if (reindWid->getCpiYear(PiaMethod::YEAR_ENT) >=
        reindWid->getCpiYear(PiaMethod::YEAR_ELIG)) {
        biOut(*reindWid, piaParams, reindWid->getEligYear(), 1, 3);
      }
      if (reindWid->getCpiYear(PiaMethod::YEAR_BEN) >
        reindWid->getCpiYear(PiaMethod::YEAR_ENT)) {
        biOut(*reindWid, piaParams, reindWid->getEligYear(), 2, 3);
      }
      if (reindWid->getEligYear() > UserAssumptions::getIstart() &&
        userAssumptions.getIaltaw() == AssumptionType::PEBS_ASSUM) {
        printRealWageGain(reindWid->getEligYear(),
          UserAssumptions::getIstart(), *reindWid,
          reindWid->piaRealWageGain.get(), reindWid->mfbRealWageGain.get());
      }
      piaEntOut(reindWid->piaEnt.get());
      outputString.push_back("");
      outputString.push_back(
        "MFB at entitlement = (same as for wage-indexed)");
      // print noninsured message
      nonIns(piaData.getFinsCode2(),workerData,piaData);
      if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY)
        disInsOut(piaData);
      outputString.push_back("");
      printNameSsn(workerData);
    }
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_RWSTRING);
  }
}
