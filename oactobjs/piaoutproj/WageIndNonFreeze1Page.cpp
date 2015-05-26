// Functions for the <see cref="WageIndNonFreeze1Page"/> class to manage a wage
// indexed earnings page.

// $Id: WageIndNonFreeze1Page.cpp 1.2 2014/01/05 07:54:54EST 277133 Development  $

#include "WageIndNonFreeze1Page.h"
#include "WageIndNonFreeze.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes WageIndNonFreeze1Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
WageIndNonFreeze1Page::WageIndNonFreeze1Page( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
WageIndNonFreeze1Page::~WageIndNonFreeze1Page()
{ }

/// <summary>Prepares wage indexed strings for printing.</summary>
void WageIndNonFreeze1Page::prepareStrings()
{
  try {
    const WageIndNonFreeze* wageIndNonFreeze = (const WageIndNonFreeze*)piaCal.wageIndNonFreeze;
    if (wageIndNonFreeze != (const WageIndNonFreeze*)0 && wageIndNonFreeze->getApplicable() > 0) {
      const AverageWage& earnings = workerData.getTotalize() ?
        piaData.earnTotalizedLimited : piaData.earnOasdiLimited;
      pageTitle[0] = wageIndNonFreeze->getTitle();
      outputString.clear();
      prepareHeader();
      const int earn50 = piaData.getEarn50(workerData.getTotalize() ?
        PiaData::EARN_WITH_TOTALIZATION : PiaData::EARN_NO_TOTALIZATION);
      printIndexedEarningsTable(*wageIndNonFreeze, piaData, piaData.getEligYearNonFreeze(),
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
