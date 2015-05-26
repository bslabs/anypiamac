// Functions for the <see cref="OldStart1Page"/> class to manage old start
// calculation earnings output.

// $Id: OldStart1Page.cpp 1.15.1.2 2012/08/28 07:58:34EDT 277133 Development  $

#include <iomanip>
#include "OldStart1Page.h"
#include "OldStart.h"
#include "PiaException.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes OldStart1Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
OldStart1Page::OldStart1Page( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaParams(newPiaCal.piaParams), piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
OldStart1Page::~OldStart1Page()
{ }

/// <summary>Prepares old start strings for printing.</summary>
void OldStart1Page::prepareStrings()
{
  try {
    const OldStart* oldStart = (const OldStart*)piaCal.oldStart;
    if (oldStart != (const OldStart*)0 && oldStart->getApplicable() > 0) {
      pageTitle[0] = oldStart->getTitle();
      outputString.clear();
      const AverageWage& earnings = workerData.getTotalize() ?
        piaData.earnTotalizedLimited : piaData.earnOasdiLimited;
      prepareHeader();
      outputString.push_back(
        "                         Imputed      High n");
      outputString.push_back(
        "Year      Earnings      Earnings      Years");
      outputString.push_back("");
      for (int i1 = YEAR37; i1 <= piaData.getEarnYear(); i1++) {
        strm.precision(2);
        strm.str("");
        strm << PunctFacet::getYear(i1);
        if (workerData.getIbegin() != YEAR50 || i1 != YEAR50) {
          strm << setw(14) << earnings[i1];
        }
        else {
          const double earnTotal50 = piaData.getEarnTotal50(
            workerData.getTotalize() ? PiaData::EARN_WITH_TOTALIZATION :
            PiaData::EARN_NO_TOTALIZATION);
          strm << setw(14) << earnTotal50;
        }
        strm << setw(14) << oldStart->earnIndexed[i1];
        if (oldStart->iorder[i1] > 0) {
          strm << setw(14) << oldStart->earnIndexed[i1];
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
    throw PiaException(PIA_IDS_OSSTRING);
  }
}
