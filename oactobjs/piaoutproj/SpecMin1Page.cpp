// Functions for the <see cref="SpecMin1Page"/> class to manage a special
// minimum calculation earnings page.

// $Id: SpecMin1Page.cpp 1.17.1.2 2012/08/28 07:58:37EDT 277133 Development  $

#include <iomanip>
#include "SpecMin1Page.h"
#include "SpecMin.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "piaparms.h"
#include "oactcnst.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes SpecMin1Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
SpecMin1Page::SpecMin1Page( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaParams(newPiaCal.piaParams), piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
SpecMin1Page::~SpecMin1Page()
{ }

/// <summary>Prepares special minimum strings for printing.</summary>
void SpecMin1Page::prepareStrings()
{
  try {
    const SpecMin* specMin = (const SpecMin*)piaCal.specMin;
    if (specMin != (SpecMin*)0 && specMin->getApplicable() > 0) {
      pageTitle[0] = specMin->getTitle();
      outputString.clear();
      prepareHeader();
      outputString.push_back("                     percent of");
      outputString.push_back("                       old-law");
      outputString.push_back(
        "                        base      required   years of");
      outputString.push_back(
        "year      earnings    required    earnings   coverage");
      outputString.push_back("");
      strm.precision(2);
      strm.str("");
      strm << "1937-50 " << setw(10)
        << piaData.getEarnTotal50(PiaData::EARN_NO_TOTALIZATION)
        << "                              "
        << setw(2) << specMin->iorder[YEAR50];
      outputString.push_back(strm.str());
      int i3 = min(workerData.getIend(), piaData.getEarnYear());
      for (int yr = piaData.getEarn50(PiaData::EARN_NO_TOTALIZATION);
        yr <= i3; ++yr) {
        strm.str("");
        strm << PunctFacet::getYear(yr) << setw(14)
          << piaData.earnOasdiLimited[yr] << setw(10)
          << piaParams.getPercSpecMin(yr) << setw(14)
          << piaParams.getYocAmountSpecMin(yr) << setw(8)
          << specMin->iorder[yr];
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
    throw PiaException(PIA_IDS_SMSTRING);
  }
}
