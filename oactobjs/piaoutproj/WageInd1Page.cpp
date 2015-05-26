// Functions for the <see cref="WageInd1Page"/> class to manage a wage
// indexed earnings page.

// $Id: WageInd1Page.cpp 1.17.1.2 2012/08/28 07:58:38EDT 277133 Development  $

#include "WageInd1Page.h"
#include "WageInd.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes WageInd1Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
WageInd1Page::WageInd1Page( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
WageInd1Page::~WageInd1Page()
{ }

/// <summary>Prepares wage indexed strings for printing.</summary>
void WageInd1Page::prepareStrings()
{
  try {
    const WageInd* wageInd = (const WageInd*)piaCal.wageInd;
    if (wageInd != (const WageInd*)0 && wageInd->getApplicable() > 0) {
      const AverageWage& earnings = workerData.getTotalize() ?
        piaData.earnTotalizedLimited : piaData.earnOasdiLimited;
      pageTitle[0] = wageInd->getTitle();
      outputString.clear();
      prepareHeader();
      const int earn50 = piaData.getEarn50(workerData.getTotalize() ?
        PiaData::EARN_WITH_TOTALIZATION : PiaData::EARN_NO_TOTALIZATION);
      printIndexedEarningsTable(*wageInd, piaData, piaData.getEligYear(),
        earnings, earn50);
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
