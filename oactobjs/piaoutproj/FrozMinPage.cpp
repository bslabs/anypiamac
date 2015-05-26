// Functions for the <see cref="FrozMinPage"/> class to manage frozen
// minimum calculation output.

// $Id: FrozMinPage.cpp 1.15.1.2 2012/08/28 07:58:33EDT 277133 Development  $

#include "FrozMinPage.h"
#include "FrozMin.h"
#include "PiaException.h"
#include "Resource.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes FrozMinPage.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
FrozMinPage::FrozMinPage( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaParams(newPiaCal.piaParams), piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
FrozMinPage::~FrozMinPage()
{ }

/// <summary>Prepares frozen minimum strings for printing.</summary>
void FrozMinPage::prepareStrings()
{
  try {
    const FrozMin* frozMin = piaCal.frozMin;
    if (frozMin != (const FrozMin *)0 && frozMin->getApplicable() > 0) {
      pageTitle[0] = frozMin->getTitle();
      const int year0 = frozMin->getCpiYear(PiaMethod::FIRST_YEAR);
      const int year1 = frozMin->getCpiYear(PiaMethod::YEAR_ELIG);
      const int year2 = frozMin->getCpiYear(PiaMethod::YEAR_ENT);
      const int year3 = frozMin->getCpiYear(PiaMethod::YEAR_BEN);
      outputString.clear();
      prepareHeader();
      strm.str("");
      strm.precision(2);
      strm << "PIA at eligibility = " << frozMin->piaElig[year0];
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.str("");
      strm << "MFB at eligibility = " << frozMin->mfbElig[year0];
      outputString.push_back(strm.str());
      outputString.push_back("");
      if (year2 >= year1) {
        biOut(*frozMin, piaParams, piaData.getEligYear(), 1, 3);
      }
      if (year3 > year2) {
        piamfb1(*frozMin);
        biOut(*frozMin, piaParams, piaData.getEligYear(), 2, 3);
      }
      piamfb2(*frozMin);
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
