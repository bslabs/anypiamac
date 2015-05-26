// Functions for the <see cref="ChildCareCalc1Page"/> class to manage wage
// indexed earnings with child-care dropout years.

// $Id: ChildCareCalc1Page.cpp 1.17.1.2 2012/08/28 07:58:32EDT 277133 Development  $

#include "ChildCareCalc1Page.h"
#include "ChildCareCalc.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes ChildCareCalc1Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
ChildCareCalc1Page::ChildCareCalc1Page( const PiaCalAny& newPiaCal ) :
PiaPageOut(), workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
ChildCareCalc1Page::~ChildCareCalc1Page()
{ }

/// <summary>Prepares wage indexed strings with childcare dropout years for
/// printing.</summary>
void ChildCareCalc1Page::prepareStrings()
{
  try {
    const ChildCareCalc* childCareCalc = piaCal.childCareCalc;
    if (childCareCalc != (const ChildCareCalc*)0 &&
      childCareCalc->getApplicable() > 0) {
      const AverageWage& earnings = workerData.getTotalize() ?
        piaData.earnTotalizedLimited : piaData.earnOasdiLimited;
      pageTitle[0] = childCareCalc->getTitle();
      outputString.clear();
      prepareHeader();
      const int earn50 = piaData.getEarn50(workerData.getTotalize() ?
        PiaData::EARN_WITH_TOTALIZATION : PiaData::EARN_NO_TOTALIZATION);
      printIndexedEarningsTable(*childCareCalc, piaData,
        piaData.getEligYear(), earnings, earn50);
      // print noninsured message
      nonIns(piaData.getFinsCode2(),workerData,piaData);
      if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY) {
        disInsOut(piaData);
      }
      outputString.push_back("");
      printNameSsn(workerData);
    }
  } catch (exception&) {
    throw PiaException(PIA_IDS_WISTRING);
  }
}
