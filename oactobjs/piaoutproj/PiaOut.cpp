// Functions for the <see cref="PiaOut"/> class to manage output for a
// non-Statement calculation.

// $Id: PiaOut.cpp 1.16.1.2 2013/10/29 13:14:32EDT 277133 Development  $

#include "PiaOut.h"
#include "FormatString.h"
#include "OldStart.h"
#include "PiaTable.h"
#include "WageInd.h"
#include "TransGuar.h"
#include "SpecMin.h"
#include "ReindWid.h"
#include "FrozMin.h"
#include "oactcnst.h"
#include "PiaException.h"
#include "Resource.h"
#include "EarnProject.h"
#include "PiadataArray.h"
#include "WorkerDataArray.h"
#include "SecondaryArray.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes PiaOut.</summary>
///
/// <remarks>Creates the five <see cref="ReindWid1Page"/>'s and
/// <see cref="ReindWid2Page"/>'s.</remarks>
///
/// <param name="piaCal">Benefit calculation parameters and results.</param>
/// <param name="taxData">Taxes paid.</param>
PiaOut::PiaOut( const PiaCalAny& piaCal, const TaxData& taxData ) :
summary1Page(piaCal),
summary2Page(piaCal),
summary3Page(piaCal),
summaryPage(summary1Page, summary2Page, summary3Page),
earningsPage(piaCal),
insuredPage(piaCal),
familyPage(piaCal),
oldStart1Page(piaCal),
oldStart2Page(piaCal),
piaTable1Page(piaCal),
piaTable2Page(piaCal),
wageInd1Page(piaCal),
wageInd2Page(piaCal),
transGuar1Page(piaCal),
transGuar2Page(piaCal),
specMin1Page(piaCal),
specMin2Page(piaCal),
frozMinPage(piaCal),
dibGuarPage(piaCal),
childCareCalc1Page(piaCal),
childCareCalc2Page(piaCal),
wageIndNonFreeze1Page(piaCal),
wageIndNonFreeze2Page(piaCal),
taxesPage(piaCal.workerData, piaCal.piaData, taxData),
onePage(piaCal)
{
  for (int i = 0; i < MAX_REINDWID; i++) {
    reindWid1Page[i] = new ReindWid1Page(piaCal);
    reindWid2Page[i] = new ReindWid2Page(piaCal);
  }
}

/// <summary>Destructor.</summary>
///
/// <remarks>Deletes the five <see cref="ReindWid1Page"/>'s and
/// <see cref="ReindWid2Page"/>'s.</remarks>
PiaOut::~PiaOut()
{
  for (int i = 0; i < MAX_REINDWID; i++) {
    delete reindWid1Page[i];
    delete reindWid2Page[i];
  }
}

/// <summary>Prepares strings for printing.</summary>
///
/// <remarks>This should be called after <see cref="setPageNum"/>, so the
/// number of reindexed widow(er) calculations will be set.</remarks>
void PiaOut::prepareStrings()
{
  summary1Page.prepareStrings();
  summary2Page.prepareStrings();
  summary3Page.prepareStrings();
  summaryPage.prepareStrings();
  earningsPage.prepareStrings();
  insuredPage.prepareStrings();
  familyPage.prepareStrings();
  oldStart1Page.prepareStrings();
  oldStart2Page.prepareStrings();
  piaTable1Page.prepareStrings();
  piaTable2Page.prepareStrings();
  wageInd1Page.prepareStrings();
  wageInd2Page.prepareStrings();
  transGuar1Page.prepareStrings();
  transGuar2Page.prepareStrings();
  specMin1Page.prepareStrings();
  specMin2Page.prepareStrings();
  for (int i = 0; i < widowCount; i++) {
    reindWid1Page[i]->prepareStrings();
    reindWid2Page[i]->prepareStrings();
  }
  frozMinPage.prepareStrings();
  dibGuarPage.prepareStrings();
  wageIndNonFreeze1Page.prepareStrings();
  wageIndNonFreeze2Page.prepareStrings();
  childCareCalc1Page.prepareStrings();
  childCareCalc2Page.prepareStrings();
  taxesPage.prepareStrings();
  //onePage.prepareStrings();
}

/// <summary>Fills out page numbers.</summary>
///
/// <remarks>This should be called before <see cref="prepareStrings"/>, to set
/// the number of reindexed widow(er) calculations.</remarks>
///
/// <param name="piaCal">Benefit calculation parameters and results.</param>
void PiaOut::setPageNum( const PiaCalAny& piaCal )
{
  int pageCount = 0;
  summary1Page.setPageNum(++pageCount);
  earningsPage.setPageNum(++pageCount);
  insuredPage.setPageNum(++pageCount);
  if (piaCal.widowArray.getFamSize() > 0) {
    familyPage.setPageNum(++pageCount);
  }
  for (vector< PiaMethod * >::const_iterator iter = piaCal.piaMethod.begin();
    iter != piaCal.piaMethod.end(); iter++) {
    switch ((*iter)->getMethod()) {
    case PiaMethod::OLD_START:
      oldStart1Page.setPageNum(++pageCount);
      oldStart2Page.setPageNum(++pageCount);
      break;
    case PiaMethod::PIA_TABLE:
      piaTable1Page.setPageNum(++pageCount);
      piaTable2Page.setPageNum(++pageCount);
      break;
    case PiaMethod::WAGE_IND:
      wageInd1Page.setPageNum(++pageCount);
      wageInd2Page.setPageNum(++pageCount);
      break;
    case PiaMethod::TRANS_GUAR:
      transGuar1Page.setPageNum(++pageCount);
      transGuar2Page.setPageNum(++pageCount);
      break;
    case PiaMethod::SPEC_MIN:
      specMin1Page.setPageNum(++pageCount);
      specMin2Page.setPageNum(++pageCount);
      break;
    case PiaMethod::FROZ_MIN:
      frozMinPage.setPageNum(++pageCount);
      break;
    case PiaMethod::DIB_GUAR:
      dibGuarPage.setPageNum(++pageCount);
      break;
    case PiaMethod::WAGE_IND_NON_FREEZE:
      wageIndNonFreeze1Page.setPageNum(++pageCount);
      wageIndNonFreeze2Page.setPageNum(++pageCount);
      break;
    case PiaMethod::CHILD_CARE:
      childCareCalc1Page.setPageNum(++pageCount);
      childCareCalc2Page.setPageNum(++pageCount);
    }
  }
  widowCount = (int)piaCal.widowMethod.size();
  for (int i = 0; i < widowCount; i++) {
    reindWid1Page[i]->setPageNum(++pageCount);
    reindWid1Page[i]->reindWid = piaCal.widowMethod[i];
    reindWid2Page[i]->setPageNum(++pageCount);
    reindWid2Page[i]->reindWid = piaCal.widowMethod[i];
  }
  taxesPage.setPageNum(++pageCount);
}
