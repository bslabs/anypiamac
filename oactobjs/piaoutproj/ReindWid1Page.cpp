// Functions for the <see cref="ReindWid1Page"/> class to manage a
// re-indexed widow earnings page.

// $Id: ReindWid1Page.cpp 1.18.1.2 2012/08/28 07:58:35EDT 277133 Development  $

#include "ReindWid1Page.h"
#include "ReindWid.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes ReindWid1Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
ReindWid1Page::ReindWid1Page( const PiaCalAny& newPiaCal ) :
PiaPageOut(), workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaCal(newPiaCal), userAssumptions(newPiaCal.userAssumptions)
{ }

/// <summary>Destructor.</summary>
ReindWid1Page::~ReindWid1Page()
{ }

/// <summary>Prepares reindexed widow strings for printing.</summary>
void ReindWid1Page::prepareStrings()
{
  try {
    if (reindWid != (const ReindWid*)0 && reindWid->getApplicable() > 0) {
      const AverageWage& earnings = workerData.getTotalize() ?
        piaData.earnTotalizedLimited : piaData.earnOasdiLimited;
      pageTitle[0] = reindWid->getTitle();
      outputString.clear();
      prepareHeader();
      printIndexedEarningsTable(*reindWid, piaData, reindWid->getEligYear(),
        earnings, piaData.getEarn50(PiaData::EARN_NO_TOTALIZATION));
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
