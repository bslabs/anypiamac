// Functions for the <see cref="OldStart2Page"/> class to manage old start
// calculation results.

// $Id: OldStart2Page.cpp 1.26.1.3 2012/09/06 10:39:40EDT 277133 Development  $

#include <iomanip>
#include "OldStart2Page.h"
#include "OldStart.h"
#include "WageInd.h"
#include "PiaException.h"
#include "PiaCalAny.h"

using namespace std;

// <summary>Years of old-start Social Security Acts.</summary>
const char *OldStart2Page::iyros[OldStart::NUM_OLD_START_TYPES] = {
  "1939", "1950", "1950", "1965", "1967", "1977", "1977", "1990"
};

// <summary>Sub-titles of old-start Social Security Acts.</summary>
const char *OldStart2Page::ios77[OldStart::NUM_OLD_START_TYPES] = {
  "", "1950 Conversion Table", "1958 Conversion Table", "", "",
  "Pre-1977 Act PIA Table", "December 1978 PIA Table",
  "Pre-1977 Act PIA Table"
};

/// <summary>Initializes OldStart2Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
OldStart2Page::OldStart2Page( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaParams(newPiaCal.piaParams), piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
OldStart2Page::~OldStart2Page()
{ }

/// <summary>Prepares old start strings for printing.</summary>
void OldStart2Page::prepareStrings()
{
  try {
    const OldStart* oldStart = (const OldStart*)piaCal.oldStart;
    if (oldStart != (const OldStart*)0 && oldStart->getApplicable() > 0) {
      pageTitle[0] = oldStart->getTitle();
      outputString.clear();
      prepareHeader();
      strm.str("");
      strm << "Quarters of coverage after 1950 = " << piaData.getQcTotal51();
      outputString.push_back(strm.str());
      outputString.push_back("");
      compYear(workerData, piaData, piaData.compPeriodOld);
      strm.str("");
      strm.precision(2);
      strm << "AME = " << oldStart->getftearn() << "/("
        << piaData.compPeriodOld.getN() << "*12) = " << oldStart->getAmeOs();
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.str("");
      strm << "Applicable method: " << iyros[oldStart->getMethodOs()]
        << " Act";
      outputString.push_back(strm.str());
      if (strlen(ios77[oldStart->getMethodOs()])) {
        strm.str("");
        strm << "  " << ios77[oldStart->getMethodOs()];
        outputString.push_back(strm.str());
      }
      outputString.push_back("");
      outputString.push_back("PIB before applying increment years = ");
      strm.str("");
      strm << "   " << piaParams.getPercOS(0) << " * " << setprecision(0)
        << oldStart->portionAme[0] << " +";
      outputString.push_back(strm.str());
      strm.str("");
      strm.precision(2);
      strm << "   " << piaParams.getPercOS(1) << " * " << setprecision(0)
        << oldStart->portionAme[1] << " = " << setprecision(2)
        << oldStart->getPib();
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.str("");
      strm << "Increment years = ";
      if (oldStart->isOs1977_90()) {
        const double earnTotal50 = piaData.getEarnTotal50(
          workerData.getTotalize() ? PiaData::EARN_WITH_TOTALIZATION :
          PiaData::EARN_NO_TOTALIZATION);
        strm.precision(0);
        strm << earnTotal50 << "/" << PiaParams::amtperincyr << " = ";
      }
      strm << oldStart->getIncyrs();
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.str("");
      strm.precision(2);
      strm << "PIB after applying increment years = "
        << oldStart->getPibInc();
      outputString.push_back(strm.str());
      outputString.push_back("");
      const int year0 = oldStart->getCpiYear(PiaMethod::FIRST_YEAR);
      const int year2 = oldStart->getCpiYear(PiaMethod::YEAR_ENT);
      const int year3 = oldStart->getCpiYear(PiaMethod::YEAR_BEN);
      strm.str("");
      switch (oldStart->getMethodOs())
      {
      case OldStart::OS1950:
        if (workerData.getBenefitDate() < PiaParams::amend58) {
          strm.precision(2);
          strm << "1950 PIA = " << oldStart->piaElig[year0];
          outputString.push_back(strm.str());
          outputString.push_back("");
          strm.str("");
          strm << "1950 MFB = " << oldStart->mfbElig[year0];
          outputString.push_back(strm.str());
          outputString.push_back("");
          break;
        }
      case OldStart::OS1958:
      case OldStart::OS1965:
      case OldStart::OS1967:
      case OldStart::OS1977_78:
      case OldStart::OS1977_79:
      case OldStart::OS1990:
        strm.precision(0);
        strm << "New-start AME = " << oldStart->getAme();
        outputString.push_back(strm.str());
        outputString.push_back("");
        switch (oldStart->getMethodOs())
        {
        case OldStart::OS1977_78:
        case OldStart::OS1990:
          osTableOut(*oldStart);
          if (year2 >= year0)
            biOut(*oldStart, piaParams, piaData.getEligYear(), 0, 0);
          if (year3 > year2)
            biOut(*oldStart, piaParams, piaData.getEligYear(), 2, 3);
          break;
        case OldStart::OS1977_79:
          strm.str("");
          strm << "PIA at eligibility = ";
          if (oldStart->getWindfall() == PiaMethod::NOWINDFALLELIM &&
            !workerData.getTotalize()) {
            strm.precision(2);
            strm << oldStart->piaElig[year0];
          }
          else {
            if (oldStart->getWindfall() != PiaMethod::NOWINDFALLELIM) {
              strm.precision(2);
              strm << oldStart->getPiaWindfall().get();
            }
            else {
              strm.precision(2);
              strm << oldStart->getPiaEligTotal().get();
            }
          }
          outputString.push_back(strm.str());
          outputString.push_back("");
          if (oldStart->getWindfall() != PiaMethod::NOWINDFALLELIM) {
            pubPenOut(*oldStart,
              static_cast<double>(piaData.getPubpenApp(workerData)),
              workerData.getPubpenDate());
          }
          if (workerData.getTotalize()) {
            totPiaOut(*oldStart, piaData.getQcTotalRel(),
              piaData.compPeriodNew.getN());
            rawPiaOut(oldStart->piaElig[year0]);
          }
          mfbBpOut(oldStart->bendMfb);
          if (piaCal.wageInd == (WageInd*)0) {
            mfbOut(*oldStart, oldStart->portionPiaElig, oldStart->percMfb);
          }
          else {
            mfbOut(*oldStart, oldStart->portionPiaElig, oldStart->percMfb,
              piaCal.wageInd->getAme());
          }
          if (year2 >= oldStart->getCpiYear(PiaMethod::YEAR_ELIG))
            biOut(*oldStart, piaParams, piaData.getEligYear(), 1, 3);
          if (year3 > year2) {
            piamfb1(*oldStart);
            biOut(*oldStart, piaParams, piaData.getEligYear(), 2, 3);
          }
          break;
        default:
          break;
        }
        break;
      default:
        break;
      }
      piamfb2(*oldStart);
      // print noninsured message
      nonIns(piaData.getFinsCode2(),workerData,piaData);
      if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY) {
        disInsOut(piaData);
      }
      outputString.push_back("");
      printNameSsn(workerData);
    }
  } catch (exception&) {
    throw PiaException(PIA_IDS_OSSTRING);
  }
}

/// <summary>Prepares windfall calculation.</summary>
///
/// <param name="oldStart">Old-start calculation method.</param>
/// <param name="pubpen">Amount of public pension.</param>
/// <param name="pubpenDate">Starting date of public pension.</param>
void OldStart2Page::pubPenOut( const OldStart& oldStart, double pubpen,
   const DateMoyr& pubpenDate )
{
  nonpenOut(pubpen, pubpenDate);
  outputString.push_back("");
  outputString.push_back("PIA after windfall:");
  strm.precision(2);
  strm.str("");
  strm << "   " << oldStart.getPiaWindfall().toString() << " - 0.5*"
    << min(oldStart.getPiaWindfall().get(), pubpen);
  outputString.push_back(strm.str());
  strm.str("");
  const int year = oldStart.getCpiYear(PiaMethod::FIRST_YEAR);
  strm << "   = " << oldStart.piaElig[year];
  outputString.push_back(strm.str());
  outputString.push_back("");
}
