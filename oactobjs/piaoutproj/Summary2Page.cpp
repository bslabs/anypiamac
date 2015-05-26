// Functions for the <see cref="Summary2Page"/> class to manage the middle
// part of a summary of results page.

// $Id: Summary2Page.cpp 1.16.1.1 2012/03/08 07:17:54EST 277133 Development  $

#include <iomanip>
#include "Summary2Page.h"
#include "Resource.h"
#include "FormatString.h"
#include "PiaException.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes Summary2Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
Summary2Page::Summary2Page( const PiaCalAny& newPiaCal ) : PageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
Summary2Page::~Summary2Page()
{ }

/// <summary>Prepares middle part of summary page.</summary>
void Summary2Page::prepareStrings()
{
  string s;  // temporary string
  try {
    outputString.clear();
    for (vector< PiaMethod* >::const_iterator iter = piaCal.piaMethod.begin();
      iter != piaCal.piaMethod.end(); iter++) {
      outputString.push_back("");
      outputString.push_back((*iter)->getTitle());
      if ((*iter)->getApplicable()) {
        strm.precision(2);
        strm.str("");
        strm << setw(8) << (*iter)->piaEnt.get();
        AfxFormatString1(s, PIA_IDS_PIAOUT, strm.str());
        outputString.push_back(s);
        strm.str("");
        strm << setw(8) << (*iter)->mfbEnt.get();
        AfxFormatString1(s, PIA_IDS_MFBOUT, strm.str());
        outputString.push_back(s);
      }
      else {
        outputString.push_back("   Not applicable");
      }
    }
    outputString.push_back("");
    outputString.push_back("");
    outputString.push_back("");
    outputString.push_back("");
    strm.str("");
    strm.precision(0);
    switch (piaData.getIappn())
    {
    case PiaMethod::OLD_START:
    case PiaMethod::PIA_TABLE:
    case PiaMethod::TRANS_GUAR:
      strm << setw(8) << piaCal.highPiaMethod->getAme();
      AfxFormatString1(s, PIA_IDS_AMEOUT, strm.str());
      outputString.push_back(s);
      break;
    case PiaMethod::WAGE_IND:
    case PiaMethod::REIND_WID:
    case PiaMethod::CHILD_CARE:
      strm << setw(8) <<  piaCal.highPiaMethod->getAme();
      AfxFormatString1(s, PIA_IDS_AIMEOUT, strm.str());
      outputString.push_back(s);
      break;
    case PiaMethod::SPEC_MIN:
      strm << setw(8) << piaCal.highPiaMethod->getYearsTotal();
      AfxFormatString1(s, PIA_IDS_SPYRSOUT, strm.str());
      outputString.push_back(s);
    }
    strm.precision(2);
    strm.str("");
    strm << setw(8) << piaData.highPia.get();
    AfxFormatString1(s, PIA_IDS_PIALOUT, strm.str());
    outputString.push_back(s);
    // print support PIA, if applicable
    if (piaData.getArfApp() > 0) {
      strm.str("");
      strm << setw(8) << piaData.supportPia.get();
      AfxFormatString1(s, PIA_IDS_SUPPIAOUT, strm.str());
      outputString.push_back(s);
    }
    if (workerData.getJoasdi() != WorkerDataGeneral::SURVIVOR) {
      strm.str("");
      strm << setw(8) << piaData.getMonthsArdri();
      if (piaData.getArf() >= 1.0) {
        AfxFormatString1(s, PIA_IDS_DRCMOUT, strm.str());
        outputString.push_back(s);
        strm.str("");
        strm << setw(8) << setprecision(5) << piaData.getArf();
        AfxFormatString1(s, PIA_IDS_DRCFOUT, strm.str());
      }
      else {
        AfxFormatString1(s, PIA_IDS_ARFMOUT, strm.str());
        outputString.push_back(s);
        strm.str("");
        strm << setw(8) << setprecision(5) << piaData.getArf();
        AfxFormatString1(s, PIA_IDS_ARFFOUT, strm.str());
      }
      outputString.push_back(s);
      strm.precision(2);
      strm.str("");
      strm << setw(8) << piaData.unroundedBenefit.get();
      AfxFormatString1(s, PIA_IDS_BEN1OUT, strm.str());
      outputString.push_back(s);
      strm.str("");
      strm << setw(8) << piaData.roundedBenefit.get();
      AfxFormatString1(s, PIA_IDS_BEN2OUT, strm.str());
      outputString.push_back(s);
    }
    if (piaData.getArfApp() > 0) {
      AfxFormatString1(s, (piaData.getArfApp() == 1) ?
        PIA_IDS_SPPIA1OUT : PIA_IDS_SPPIA2OUT, "");
      outputString.push_back(s);
    }
    strm.precision(2);
    strm.str("");
    strm << setw(8) << piaData.highMfb.get();
    AfxFormatString1(s, PIA_IDS_MFBLOUT, strm.str());
    outputString.push_back(s);
    outputString.push_back("");
    outputString.push_back("");
    outputString.push_back("");
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_TWOPAGE4);
  }
}
