// Functions for the <see cref="DibGuarPage"/> class to manage disability
// guarantee calculation output.

// $Id: DibGuarPage.cpp 1.9.1.2 2012/08/28 07:58:33EDT 277133 Development  $

#include "DibGuarPage.h"
#include "DibGuar.h"
#include "PiaException.h"
#include "Resource.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes DibGuarPage.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.</param>
DibGuarPage::DibGuarPage( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaParams(newPiaCal.piaParams), piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
DibGuarPage::~DibGuarPage()
{ }

/// <summary>Prepares frozen minimum strings for printing.</summary>
void DibGuarPage::prepareStrings()
{
  try {
    const DibGuar* dibGuarPtr = piaCal.dibGuar;
    if (dibGuarPtr != (const DibGuar *)0 &&
      dibGuarPtr->getApplicable() > 0) {
      const DibGuar& dibGuar = *dibGuarPtr;
      pageTitle[0] = dibGuar.getTitle();
      const int year0 = dibGuar.getCpiYear(PiaMethod::FIRST_YEAR);
      const int year1 = dibGuar.getCpiYear(PiaMethod::YEAR_ELIG);
      const int year2 = dibGuar.getCpiYear(PiaMethod::YEAR_ENT);
      const int year3 = dibGuar.getCpiYear(PiaMethod::YEAR_BEN);
      outputString.clear();
      prepareHeader();
      strm.str("");
      strm.precision(2);
      strm << "PIA at cessation of prior disability = "
        << dibGuar.piaElig[year0];
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.str("");
      strm << "MFB at cessation of prior disability = "
        << dibGuar.getCessationMfbOrig();
      outputString.push_back(strm.str());
      if (dibGuar.getDiMaxApplies()) {
        outputString.push_back("(Disability Maximum applies)");
      }
      else {
        outputString.push_back("(Disability Maximum does not apply)");
      }
      outputString.push_back("");
      switch (dibGuar.getConvertedMfbType()) {
      case DibGuar::POST1995_NOCHANGE:
        break;
      case DibGuar::POST1995_DECONVERTED:
        strm.str("");
        strm << "Deconverted PIA at onset of prior disability = "
          << dibGuar.getRawPia();
        outputString.push_back(strm.str());
        outputString.push_back("");
        strm.str("");
        strm << "Raw MFB at onset of prior disability = "
          << dibGuar.getRawMfb();
        outputString.push_back(strm.str());
        outputString.push_back("");
        strm.str("");
        strm << "Converted MFB at cessation of prior disability = "
          << dibGuar.getCessationMfbConv();
        outputString.push_back(strm.str());
        outputString.push_back("");
        break;
      case DibGuar::PRE1996_PRE1979_LAST12:
      case DibGuar::PRE1996_POST1978_LAST12:
      case DibGuar::PRE1996_NOTLAST12:
        strm.str("");
        strm << "Converted MFB at cessation of prior disability = "
          << dibGuar.getCessationMfbConv();
        outputString.push_back(strm.str());
        outputString.push_back("");
        break;
      default:
        throw PiaException(
          "DibGuarPage::prepareStrings: Illegal convertedMfbType");
      }
      if (year2 >= year1) {
        biOut(dibGuar, piaParams, piaData.getEligYear(), 1, 3);
      }
      else {
        outputString.push_back(
          "No CPI increases applied before entitlement");
        if (dibGuar.getColasApply()) {
          outputString.push_back(
            "(no CPI increase occurred before entitlement)");
        }
        else {
          outputString.push_back(
            "(more than 12 months between cessation and new entitlement)");
        }
        outputString.push_back("");
      }
      if (year3 > year2) {
        if (dibGuar.getIoasdi() == WorkerDataGeneral::SURVIVOR) {
          piamfbDeath(dibGuar);
        }
        else {
          piamfb1(dibGuar);
        }
        biOut(dibGuar, piaParams, piaData.getEligYear(), 2, 3);
      }
      piamfb2(dibGuar);
      // print noninsured message
      nonIns(piaData.getFinsCode2(),workerData,piaData);
      if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY) {
        disInsOut(piaData);
      }
      outputString.push_back("");
      printNameSsn(workerData);
    }
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_FMSTRING);
  }
}

/// <summary>Prepares PIA and MFB at death in a string table.</summary>
///
/// <param name="piaMethod">Pia calculation method.</param>
void DibGuarPage::piamfbDeath( const PiaMethod& piaMethod )
{
  const int year = piaMethod.getCpiYear(PiaMethod::YEAR_ENT);
  strm.str("");
  strm << "PIA at death = " << piaMethod.piaElig[year];
  outputString.push_back(strm.str());
  outputString.push_back("");
  strm.str("");
  strm << "MFB at death = " << piaMethod.mfbElig[year];
  outputString.push_back(strm.str());
  outputString.push_back("");
}
