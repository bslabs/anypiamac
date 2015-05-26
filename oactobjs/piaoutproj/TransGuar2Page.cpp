// Functions for the <see cref="TransGuar2Page"/> class to manage the
// transitional guarantee calculation results page.

// $Id: TransGuar2Page.cpp 1.18.1.2 2012/08/28 07:58:37EDT 277133 Development  $

#include <iomanip>
#include "TransGuar2Page.h"
#include "TransGuar.h"
#include "WageInd.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes TransGuar2Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
TransGuar2Page::TransGuar2Page( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaParams(newPiaCal.piaParams), piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
TransGuar2Page::~TransGuar2Page()
{ }

/// <summary>Prepares transitional guarantee strings for printing.</summary>
void TransGuar2Page::prepareStrings()
{
  try {
    const TransGuar* transGuar = piaCal.transGuar;
    if (transGuar != (const TransGuar*)0 && transGuar->getApplicable() > 0) {
      pageTitle[0] = transGuar->getTitle();
      outputString.clear();
      prepareHeader();
      compYear(workerData, piaData, piaData.compPeriodNew);
      strm.str("");
      strm << "AME = " << setprecision(2) << transGuar->getftearn() << "/("
        << piaData.compPeriodNew.getN() << "*12) = " << setprecision(0)
        << transGuar->getAme();
      outputString.push_back(strm.str());
      outputString.push_back("");
      strm.str("");
      strm.precision(2);
      strm << "Dec 1978 PIA = "
        << transGuar->piaElig[transGuar->getCpiYear(PiaMethod::FIRST_YEAR)];
      outputString.push_back(strm.str());
      outputString.push_back("");
      mfbBpOut(transGuar->bendMfb);
      // Transitional guarantee is never a disability calculation, so we
      // can call mfbOut without the ame argument
      mfbOut(*transGuar, transGuar->portionPiaElig, transGuar->percMfb);
      if (transGuar->getCpiYear(PiaMethod::YEAR_ENT) >=
        transGuar->getCpiYear(PiaMethod::YEAR_ELIG)) {
        biOut(*transGuar, piaParams, piaData.getEligYear(), 1, 3);
      }
      if (transGuar->getCpiYear(PiaMethod::YEAR_BEN) >
        transGuar->getCpiYear(PiaMethod::YEAR_ENT)) {
        piamfb1(*transGuar);
        biOut(*transGuar, piaParams, piaData.getEligYear(), 2, 3);
      }
      piamfb2(*transGuar);
      // print noninsured message
      nonIns(piaData.getFinsCode2(),workerData,piaData);
      if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY) {
        disInsOut(piaData);
      }
      outputString.push_back("");
      printNameSsn(workerData);
    }
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_TGSTRING);
  }
}
