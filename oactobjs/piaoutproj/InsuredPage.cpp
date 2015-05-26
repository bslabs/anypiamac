// Functions for the <see cref="InsuredPage"/> class to manage insured
// status output.

// $Id: InsuredPage.cpp 1.11.1.1 2012/03/08 07:17:38EST 277133 Development  $

#include "InsuredPage.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes InsuredPage.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
InsuredPage::InsuredPage( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaParams(newPiaCal.piaParams), piaCal(newPiaCal),
secondaryArray(newPiaCal.secondaryArray)
{ }

/// <summary>Destructor.</summary>
InsuredPage::~InsuredPage()
{ }

/// <summary>Prepares strings for insured status page.</summary>
void InsuredPage::prepareStrings()
{
  char str[20];  // temporary string
  string s;  // temporary string
  try {
    outputString.clear();
    pageTitle[0] = "Insured Status";
    prepareHeader();
    outputString.push_back("Fully insured status:");
    strm.str("");
    strm << piaData.getQcReq();
    AfxFormatString1(s, PIA_IDS_REQQCOUT, strm.str());
    outputString.push_back(s);
    strm.str("");
    strm << piaData.getQcTotal();
    AfxFormatString1(s, PIA_IDS_ACTQCOUT, strm.str());
    outputString.push_back(s);
    switch (piaData.getFinsCode2())
    {
    case '1':
      AfxFormatString1(s, PIA_IDS_INSTATOUT, "Fully insured");
      break;
    case '3':
      AfxFormatString1(s, PIA_IDS_INSTATOUT,
        "Not fully insured (eligible for totalization benefits)");
      break;
    case '2':
    case '4':
    case '5':
      AfxFormatString1(s, PIA_IDS_INSTATOUT, "Not fully insured");
      break;
    case '6':
      AfxFormatString1(s, PIA_IDS_INSTATOUT,
        "Not insured for totalization benefits");
      break;
    case '7':
      AfxFormatString1(s, PIA_IDS_INSTATOUT,
        "Fully insured (not eligible for totalization benefits)");
      break;
    default:
      s = "";
      break;
    }
    outputString.push_back(s);
    if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY) {
      outputString.push_back("");
      outputString.push_back("Disability insured status:");
      if (piaData.getQcDisDate3().getYear() > 0) {
        strncpy(str, piaData.getQcDisDate3().toString().c_str(), 20);
        AfxFormatString2(s, PIA_IDS_DINS1OUT, str,
          piaData.getQcDisDate4().toString());
        outputString.push_back(s);
      }
      if (piaData.getQcDisDate1().getYear() > 0) {
        strncpy(str, piaData.getQcDisDate1().toString().c_str(), 20);
        AfxFormatString2(s, PIA_IDS_DINS2OUT, str,
          piaData.getQcDisDate2().toString());
        outputString.push_back(s);
      }
      if (piaData.getQcDisDate5().getYear() > 0) {
        strncpy(str, piaData.getQcDisDate5().toString().c_str(), 20);
        AfxFormatString2(s, PIA_IDS_DINS3OUT, str,
          piaData.getQcDisDate6().toString());
        outputString.push_back(s);
      }
      strm.str("");
      strm << piaData.getQcDisQtr();
      AfxFormatString1(s, PIA_IDS_ELAPSOUT, strm.str());
      outputString.push_back(s);
      strm.str("");
      strm << piaData.getQcDisReq();
      AfxFormatString1(s, PIA_IDS_REQQCOUT, strm.str());
      outputString.push_back(s);
      strm.str("");
      strm << piaData.getQcTotalDis();
      AfxFormatString1(s, PIA_IDS_ACTQCOUT, strm.str());
      outputString.push_back(s);
      if (piaData.finsCode.isFullyInsured()) {
        if (piaData.disInsCode.isDisabilityInsured()) {
          AfxFormatString1(s, PIA_IDS_INSTATOUT, "Disability insured");
        }
        else {
          AfxFormatString1(s, PIA_IDS_INSTATOUT, "Not disability insured");
        }
      }
      else {
        AfxFormatString1(s, PIA_IDS_INSTATOUT,
          "Not disability insured (not fully insured)");
      }
      outputString.push_back(s);
    }
    if (workerData.getJoasdi() == WorkerDataGeneral::SURVIVOR &&
      secondaryArray.secondary[0]->bic.isYoungSurvivor()) {
      outputString.push_back("");
      outputString.push_back("Currently insured status:");
      outputString.push_back("   Required quarters of coverage: 6");
      strm.str("");
      strm << piaData.getQcCurrent();
      AfxFormatString1(s, PIA_IDS_ACTQCOUT, strm.str());
      outputString.push_back(s);
      if (piaData.getQcCurrent() > 5) {
        AfxFormatString1(s, PIA_IDS_INSTATOUT, "Currently insured");
      }
      else {
        AfxFormatString1(s, PIA_IDS_INSTATOUT, "Not currently insured");
      }
      outputString.push_back(s);
    }
    outputString.push_back("");
    printNameSsn(workerData);
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_TWOPAGE1);
  }
}
