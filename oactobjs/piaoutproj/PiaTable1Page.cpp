// Functions for the <see cref="PiaTable1Page"/> class to manage a pia table
// calculation earnings page.

// $Id: PiaTable1Page.cpp 1.16.1.2 2012/08/28 07:58:35EDT 277133 Development  $

#include <iomanip>
#include "PiaTable1Page.h"
#include "PiaTable.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes PiaTable1Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
PiaTable1Page::PiaTable1Page( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaParams(newPiaCal.piaParams), piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
PiaTable1Page::~PiaTable1Page()
{ }

/// <summary>Prepares pia table strings for printing.</summary>
void PiaTable1Page::prepareStrings()
{
  try {
    const PiaTable* piaTable = piaCal.piaTable;
    if (piaTable != (const PiaTable*)0 && piaTable->getApplicable() > 0) {
      pageTitle[0] = piaTable->getTitle();
      outputString.clear();
      const AverageWage& earnings = workerData.getTotalize() ?
        piaData.earnTotalizedLimited : piaData.earnOasdiLimited;
      prepareHeader();
      tableHead2();
      strm.precision(2);
      for (int yr = piaData.getEarn50(PiaData::EARN_WITH_TOTALIZATION);
        yr <= piaData.getEarnYear(); ++yr) {
        strm.str("");
        strm << PunctFacet::getYear(yr) << setw(14) << earnings[yr];
        if (piaTable->iorder[yr] > 0) {
          strm << setw(14) << piaTable->earnIndexed[yr];
        }
        outputString.push_back(strm.str());
      }
      outputString.push_back("");
      // print noninsured message
      nonIns(piaData.getFinsCode2(),workerData,piaData);
      if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY) {
        disInsOut(piaData);
      }
      outputString.push_back("");
      printNameSsn(workerData);
    }
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_PTSTRING);
  }
}
