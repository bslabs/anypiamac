// Functions for the <see cref="TransGuar1Page"/> class to manage the
// transitional guarantee calculation earnings page.

// $Id: TransGuar1Page.cpp 1.15.1.2 2012/08/28 07:58:37EDT 277133 Development  $

#include <iomanip>
#include "TransGuar1Page.h"
#include "TransGuar.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes TransGuar1Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
TransGuar1Page::TransGuar1Page( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
TransGuar1Page::~TransGuar1Page()
{ }

/// <summary>Prepares transitional guarantee strings for printing.</summary>
void TransGuar1Page::prepareStrings()
{
  try {
    const TransGuar* transGuar = piaCal.transGuar;
    if (transGuar != (const TransGuar*)0 && transGuar->getApplicable() > 0) {
      const AverageWage& earnings = workerData.getTotalize() ?
        piaData.earnTotalizedLimited : piaData.earnOasdiLimited;
      pageTitle[0] = transGuar->getTitle();
      outputString.clear();
      prepareHeader();
      tableHead2();
      for (int yr = piaData.getEarn50(PiaData::EARN_NO_TOTALIZATION);
        yr <= piaData.getEarnYear(); ++yr) {
        strm.str("");
        strm.precision(2);
		strm << PunctFacet::getYear(yr) << setw(14) << earnings[yr];
        if (transGuar->iorder[yr] > 0) {
          strm << setw(14) << transGuar->earnIndexed[yr];
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
    throw PiaException(PIA_IDS_TGSTRING);
  }
}
