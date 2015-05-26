// Functions for the <see cref="SpecMin2Page"/> class to manage a special
// minimum calculation results page.

// $Id: SpecMin2Page.cpp 1.19.1.2 2012/08/28 07:58:37EDT 277133 Development  $

#include "SpecMin2Page.h"
#include "SpecMin.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "piaparms.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes this instance.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
SpecMin2Page::SpecMin2Page( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaParams(newPiaCal.piaParams), piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
SpecMin2Page::~SpecMin2Page()
{ }

/// <summary>Prepares special minimum strings for printing.</summary>
void SpecMin2Page::prepareStrings()
{
  try {
    const SpecMin* specMin = (const SpecMin*)piaCal.specMin;
    if (specMin != (SpecMin*)0 && specMin->getApplicable() > 0) {
      pageTitle[0] = specMin->getTitle();
      outputString.clear();
      prepareHeader();
      strm.str("");
      strm << "Years of coverage = " << specMin->getYearsTotal();
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.str("");
      strm << "Years of coverage over 10 (maximum 20) = "
        << specMin->getSpecMinYearsExcess();
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.precision(2);
      strm.str("");
      strm << "Amount per year = " << specMin->getSpecMinAmount();
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.str("");
      if (!(workerData.getBenefitDate() < piaParams.amend772)) {
        strm << "January 1979 PIA = " << specMin->getSpecMinYearsExcess()
          << " * " << specMin->getSpecMinAmount() << " = "
          << specMin->piaElig[specMin->getCpiYear(PiaMethod::FIRST_YEAR)];
        outputString.push_back(strm.str());
        outputString.push_back("");
        if (specMin->getCpiYear(PiaMethod::YEAR_BEN) >=
          specMin->getCpiYear(PiaMethod::YEAR_ELIG)) {
          strm.str("");
          strm << "MFB in January 1979 = "
            << specMin->mfbElig[specMin->getCpiYear(PiaMethod::FIRST_YEAR)];
          outputString.push_back(strm.str());
          outputString.push_back("");
          if (specMin->getCpiYear(PiaMethod::YEAR_ENT) >=
            specMin->getCpiYear(PiaMethod::YEAR_ELIG)) {
            biOut(*specMin, piaParams, piaData.getEligYear(), 1, 3);
          }
          if (specMin->getCpiYear(PiaMethod::YEAR_BEN) >
            specMin->getCpiYear(PiaMethod::YEAR_ENT)) {
            piamfb1(*specMin);
            biOut(*specMin, piaParams, piaData.getEligYear(), 2, 3);
          }
        }
        piamfb2(*specMin);
      }
      else {
        strm << "PIA = " << specMin->getSpecMinYearsExcess() << " * "
          << specMin->getSpecMinAmount() << " = " << specMin->piaEnt.get();
        outputString.push_back(strm.str());
        outputString.push_back("");
        strm.str("");
        strm << "MFB = " << specMin->mfbEnt.get();
        outputString.push_back(strm.str());
        outputString.push_back("");
      }
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
