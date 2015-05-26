// Functions for the <see cref="Summary3Page"/> class to manage the bottom
// part of a summary of results page.

// $Id: Summary3Page.cpp 1.18.1.2 2012/08/28 07:58:37EDT 277133 Development  $

#include "Summary3Page.h"
#include "Resource.h"
#include "FormatString.h"
#include "PiaException.h"
#include "piaparms.h"
#include "UserAssumptions.h"
#include "EarnProject.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes Summary3Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
Summary3Page::Summary3Page( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaCal(newPiaCal), userAssumptions(newPiaCal.userAssumptions),
earnProject(newPiaCal.earnProject), biProj(false), awiProj(false)
{ }

/// <summary>Destructor.</summary>
Summary3Page::~Summary3Page()
{ }

/// <summary>Prepares bottom part of summary page.</summary>
void Summary3Page::prepareStrings()
{
  string s;  // temporary output string
  biProj = false;
  awiProj = false;
  outputString.clear();
  try {
    const PiaParams& piaParams = piaCal.piaParams;
    if (workerData.need_bi(UserAssumptions::getIstart())) {
      biProj = true;
      // benefit increase assumptions
      AfxFormatString1(s, PIA_IDS_BIOUT, "");
      outputString.push_back(s);
      s = "Actual benefit increases through December " +
        PunctFacet::getYear(UserAssumptions::getIstart()-1);
      outputString.push_back(s);
      outputString.push_back(piaParams.getBenefitInc());
    }
    if (workerData.need_aw(UserAssumptions::getIstart(),
      earnProject.need_aw(UserAssumptions::getIstart(),
      workerData.getIend()))) {
      awiProj = true;
      // average wage assumptions
      AfxFormatString1(s, PIA_IDS_AWOUT, "");
      outputString.push_back(s);
      s = "Actual average wage increases through " +
        PunctFacet::getYear(UserAssumptions::getIstart()-2);
      outputString.push_back(s);
      outputString.push_back(piaParams.getAverageWage());
    }
    // print noninsured paragraph
    nonInsPara();
    if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY) {
      outputString.push_back("");
      disInsOut(piaData);
    }
    disclaimer();
    outputString.push_back("");
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_TWOPAGE5);
  }
}

/// <summary>Prints noninsured paragraph.</summary>
void Summary3Page::nonInsPara()
{
  const char *heshe = workerData.isMale() ? "he" : "she";
  switch (piaData.getFinsCode2())
  {
  case '2':
    outputString.push_back("");
    outputString.push_back(
      "This estimate may be theoretical because the worker is currently");
    outputString.push_back("insured but not fully insured.");
    strm.str("");
    strm << "A total of " << piaData.getQcReq() << " quarters are needed; "
      << heshe << " has only " << piaData.getQcTotal() << ".";
    outputString.push_back(strm.str());
    break;
  case '4':
    outputString.push_back("");
    outputString.push_back(
      "This estimate is theoretical because the worker is not fully insured.");
    strm.str("");
    strm << "A total of " << piaData.getQcReq() << " quarters are needed; "
      << heshe << " has only " << piaData.getQcTotal() << ".";
    outputString.push_back(strm.str());
    break;
  case '5':
    outputString.push_back("");
    outputString.push_back(
      "This estimate is theoretical because the worker is not fully or");
    strm.str("");
    strm << "currently insured. A total of " << piaData.getQcReq()
      << " quarters are needed for fully insured";
    outputString.push_back(strm.str());
    strm.str("");
    strm << "status; " << heshe << " has only " << piaData.getQcTotal()
      << ". A total of 6 quarters are needed for ";
    outputString.push_back(strm.str());
    strm.str("");
    strm << "currently insured status; " << heshe << " has only "
      << piaData.getQcCurrent() << ".";
    outputString.push_back(strm.str());
    break;
  case '6':
    outputString.push_back("");
    outputString.push_back(
      "This estimate is theoretical because 6 quarters are needed to be");
    strm.str("");
    strm << "eligible for a totalization benefit; the worker has only "
      << piaData.getQcTotal() << ".";
    outputString.push_back(strm.str());
    break;
  case '7':
    if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY) { 
      if (piaData.disInsCode == DisInsCode::TOTALIZED) {
        outputString.push_back("");
        outputString.push_back("Note that this worker is fully insured, but not disability insured.");
        outputString.push_back("Totalization can be used, but only for a disabled worker benefit.");
      } else {
        outputString.push_back("");
        outputString.push_back(
          "This estimate is theoretical because the worker is fully insured and ");
        outputString.push_back(
          "disability insured.  A worker cannot be both fully insured and ");
        outputString.push_back(
          "disability insured for a totalization benefit.");
      }
    } else {
      outputString.push_back("");
      outputString.push_back(
        "This estimate is theoretical because the worker is fully insured.");
      strm.str("");
      strm << "The worker must have fewer than " << piaData.getQcReq()
        << " quarters; " << heshe << " has " << piaData.getQcTotal() << ".";
      outputString.push_back(strm.str());
    }
    break;
  default:
    break;
  }
}

/// <summary>Prints bottom of summary.</summary>
///
/// <param name="isPebs">True if old PEBES assumptions, false otherwise.
/// </param>
void Summary3Page::bottom3( bool isPebs )
{
  outputString.push_back("");
  outputString.push_back(
    "The estimates on this statement are based on your earnings");
  outputString.push_back(
    "record and any information you gave us.  We used current");
  if (isPebs) {
    outputString.push_back(
      "dollar values for these estimates.  We adjusted all future");
    outputString.push_back(
      "estimates to account for average wage growth in the national");
    outputString.push_back(
      "economy by increasing your benefit by 1 percent for each");
    outputString.push_back(
      "year between now and when you turn age 62.");
  }
  else {
    outputString.push_back("dollar values for these estimates.");
  }
}

/// <summary>Prints disclaimer.</summary>
///
/// <remarks>The disclaimer mentions that the benefit is an estimate, and
/// varies slightly depending on the economic assumptions used, if any.
/// </remarks>
void Summary3Page::disclaimer()
{
  outputString.push_back("");
  if ((awiProj == false ||
    AssumptionType::isFlatOrPebs(userAssumptions.getIaltaw())) &&
    (biProj == false ||
    AssumptionType::isFlatOrPebs(userAssumptions.getIaltbi()))) {
    if (userAssumptions.getIaltaw() == AssumptionType::PEBS_ASSUM) {
      outputString.push_back(
        "The estimates on this statement are based on your earnings");
      outputString.push_back(
        "record and any information you gave us.  We used current");
      outputString.push_back(
        "dollar values for these estimates.  We adjusted all future");
      outputString.push_back(
        "estimates to account for average wage growth in the national");
      outputString.push_back(
        "economy by increasing your benefit by 1 percent for each");
      outputString.push_back(
        "year between now and when you turn age 62.");
    }
    else {
      outputString.push_back(
        "The estimates on this statement are based on your earnings");
      outputString.push_back(
        "record and any information you gave us.  We used current");
      outputString.push_back("dollar values for these estimates.");
    }
  }
  else {
    outputString.push_back(
      "The estimates we gave are based on your earnings record and any infor-");
    outputString.push_back(
      "mation you gave us.  The estimate provided could change--it could");
    outputString.push_back(
      "increase or decrease--depending on your actual future earnings,");
    outputString.push_back(
      "future changes in the average wages of all employed persons, and on");
    outputString.push_back("future rates of inflation.");
  }
}
