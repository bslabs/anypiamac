// Functions for the <see cref="AnypiaOut"/> class to manage all output
// pages from Anypia.

// $Id: AnypiaOut.cpp 1.16.1.2 2013/10/29 13:14:31EDT 277133 Development  $

#include <vector>
#include "AnypiaOut.h"
#include "EarnProject.h"
#include "PiadataArray.h"
#include "PiaTable.h"
#include "TransGuar.h"
#include "FrozMin.h"
#include "ReindWid.h"
#include "SpecMin.h"
#include "WageInd.h"
#include "DibGuar.h"
#include "WageIndNonFreeze.h"
#include "PebsOut.h"
#include "PiaCalAny.h"
#include "PiaOut.h"
#include "ChildCareCalc.h"
#include "wrkrdata.h"

using namespace std;

/// <summary>Constructor.</summary>
AnypiaOut::AnypiaOut() :
pages(), piaData(0), piaCal(0), piaOut(0), pebsOut(0)
{ }

/// <summary>Fills out page slots.</summary>
///
/// <param name="valid">True if calculated results are valid.</param>
/// <param name="longOutput">True if long output desired.</param>
void AnypiaOut::fillPageSlots( bool valid, bool longOutput )
{
  pages.clear();
  if (valid) {
    if (workerData->getJoasdi() == WorkerData::PEBS_CALC) {
      pages.push_back(&pebsOut->pebsLetter);
      pages.push_back(&pebsOut->pebsWorkerEarnings);
      pages.push_back(&pebsOut->pebsEarnings);
      pages.push_back(&pebsOut->pebsOasdiEst);
      pages.push_back(&pebsOut->pebsCreditInfo);
      pages.push_back(&pebsOut->pebsOasdiInfo);
    }
    else if (longOutput) {
      pages.push_back(&piaOut->summaryPage);
      pages.push_back(&piaOut->earningsPage);
      pages.push_back(&piaOut->insuredPage);
      if (piaCal->widowArray.getFamSize() > 0) {
        pages.push_back(&piaOut->familyPage);
      }
      if (piaCal->oldStart != (PiaMethod*)0 &&
        piaCal->oldStart->getApplicable() > 0) {
        pages.push_back(&piaOut->oldStart1Page);
        pages.push_back(&piaOut->oldStart2Page);
      }
      if (piaCal->piaTable != (PiaMethod*)0 &&
        piaCal->piaTable->getApplicable() > 0) {
        pages.push_back(&piaOut->piaTable1Page);
        pages.push_back(&piaOut->piaTable2Page);
      }
      if (piaCal->wageInd != (PiaMethod*)0 &&
        piaCal->wageInd->getApplicable() > 0) {
        pages.push_back(&piaOut->wageInd1Page);
        pages.push_back(&piaOut->wageInd2Page);
      }
      if (piaCal->transGuar != (PiaMethod*)0 &&
        piaCal->transGuar->getApplicable() > 0) {
        pages.push_back(&piaOut->transGuar1Page);
        pages.push_back(&piaOut->transGuar2Page);
      }
      if (piaCal->specMin != (PiaMethod*)0 &&
        piaCal->specMin->getApplicable() > 0) {
        pages.push_back(&piaOut->specMin1Page);
        pages.push_back(&piaOut->specMin2Page);
      }
      if (piaCal->frozMin != (PiaMethod*)0 &&
        piaCal->frozMin->getApplicable() > 0) {
        pages.push_back(&piaOut->frozMinPage);
      }
      if (piaCal->dibGuar != (PiaMethod*)0 &&
        piaCal->dibGuar->getApplicable() > 0) {
        pages.push_back(&piaOut->dibGuarPage);
      }
      if (piaCal->wageIndNonFreeze != (PiaMethod*)0 &&
        piaCal->wageIndNonFreeze->getApplicable() > 0) {
        pages.push_back(&piaOut->wageIndNonFreeze1Page);
        pages.push_back(&piaOut->wageIndNonFreeze2Page);
      }
      if (piaCal->childCareCalc != (PiaMethod*)0 &&
        piaCal->childCareCalc->getApplicable() > 0) {
        pages.push_back(&piaOut->childCareCalc1Page);
        pages.push_back(&piaOut->childCareCalc2Page);
      }
      for (unsigned int i = 0u; i < piaCal->widowMethod.size(); i++) {
        pages.push_back(piaOut->reindWid1Page[i]);
        pages.push_back(piaOut->reindWid2Page[i]);
      }
      pages.push_back(&piaOut->taxesPage);
    }
    else {
      pages.push_back(&piaOut->onePage);
    }
  }
  else {
    if (workerData->getJoasdi() == WorkerData::PEBS_CALC)
      pages.push_back(&pebsOut->pebsWorkerInfo);
    else
      pages.push_back(&piaOut->summary1Page);
  }
}
