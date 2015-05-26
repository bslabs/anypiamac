// Functions for the <see cref="EarningsPage"/> class to manage earnings
// output.

// $Id: EarningsPage.cpp 1.24.1.2 2012/08/28 07:58:33EDT 277133 Development  $

#include <iomanip>
#include "EarningsPage.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "piaparms.h"
#include "WageBase.h"
#include "UserAssumptions.h"
#include "oactcnst.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes EarningsPage.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
EarningsPage::EarningsPage( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaParams(newPiaCal.piaParams), piaCal(newPiaCal),
userAssumptions(newPiaCal.userAssumptions)
{ }

/// <summary>Destructor.</summary>
EarningsPage::~EarningsPage()
{ }

/// <summary>Prepares earnings strings for printing.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_TWOPAGE2"/> if any exceptions are caught.
/// </exception>
void EarningsPage::prepareStrings()
{
  try {
    string s;  // temporary string
    pageTitle[0] = "Earnings Used in PIA Calculation";
    outputString.clear();
    prepareHeader();
    const int i3 = workerData.getTotalize() ? piaData.getIbeginTotal() :
      piaData.getIbeginAll();
    const int i4 = workerData.getTotalize() ? piaData.getIendTotal() :
      piaData.getIendAll();
    s = "                                 Amount    Quarters";
    if (workerData.getTotalize()) {
      s += "  Relative     Attri-";
    }
    outputString.push_back(s);
    s = "          Annual      Maximum     per         of   ";
    if (workerData.getTotalize()) {
      s += "  earnings      buted";
    }
    outputString.push_back(s);
    s = "year     earnings     earnings     QC      coverage";
    if (workerData.getTotalize()) {
      s += "  position    earnings";
    }
    outputString.push_back(s);
    outputString.push_back("");
    for (int i1 = i3; i1 <= i4; ++i1) {
      strm.str("");
      strm << PunctFacet::getYear(i1);
      strm << setw(13);
      strm.precision(2);
      if (piaData.getIbeginAll() != YEAR50 || i1 != YEAR50 ||
        workerData.getTotalize()) {
        strm << piaData.earnOasdiLimited[i1];
      }
      else {
        strm << piaData.getEarnTotal50(PiaData::EARN_NO_TOTALIZATION);
      }
      strm << setw(13) << piaParams.getBaseOasdi(i1);
      if (i1 > workerData.getQcLumpYear() || workerData.hasQcsByYear()) {
        if (i1 > workerData.getQcLumpYear() && i1 > YEAR50) {
          strm << setw(10) << piaParams.qcamt[i1];
        }
        else {
          strm << "          ";
        }
        if (i1 > YEAR50) {
          strm << setw(9) << piaData.qcov.get(i1);
        }
        else {
          if (i1 < YEAR50) {
            strm << "         ";
          }
          else {
            strm << setw(9) << piaData.getQcTotal50();
          }
        }
        if (workerData.getTotalize() && i1 >= piaData.getIbeginTotal() && 
            i1 <= piaData.getIendTotal()) {
          strm << setprecision(7) << setw(11) << piaData.relEarnPosition[i1]
            << setprecision(2) << setw(13)
            << piaData.earnTotalizedLimited[i1];
        }
      }
      outputString.push_back(strm.str());
    }
    outputString.push_back("");
    if (!workerData.hasQcsByYear()) {
      strm.str("");
      strm << "QC's for " << PunctFacet::getYear(workerData.getQcLumpYear())
        << " and earlier                    " << setw(4)
        << workerData.getQctottd();
      outputString.push_back(strm.str());
      outputString.push_back("");
    }
    if (workerData.getTotalize()) {
      strm.str("");
      strm.precision(7);
      strm << setw(4) << piaData.getRelEarnPositionAverage();
      AfxFormatString1(s, PIA_IDS_RELERNOUT, strm.str());
      outputString.push_back(s);
    }
    if ((int)workerData.getBenefitDate().getYear() >
      UserAssumptions::getIstart() &&
      workerData.getIend() > UserAssumptions::getIstart()) {
        AfxFormatString1(s, PIA_IDS_PROBASOUT, BaseChangeType::toString(
          userAssumptions.getIbasch()));
      outputString.push_back(s);
    }
    // print noninsured message
    nonIns(piaData.getFinsCode2(),workerData,piaData);
    // print unused earnings message
    if (piaData.getEarnYear() < workerData.getIend()) {
      outputString.push_back("");
      AfxFormatString1(s, PIA_IDS_UNEARNOUT, "");
      outputString.push_back(s);
    }
    if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY) {
      // print disability-insured message
      outputString.push_back("");
      disInsOut(piaData);
    }
    if (piaData.getOverMax()) {
      // print truncated earnings message
      outputString.push_back("");
      AfxFormatString1(s, PIA_IDS_ERNLIMOUT, "");
      outputString.push_back(s);
    }
    outputString.push_back("");
    printNameSsn(workerData);
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_TWOPAGE2);
  }
}
