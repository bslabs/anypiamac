// Declarations for the <see cref="PiaOut"/> class to manage output for a
// non-Statement calculation.

// $Id: PiaOut.h 1.11.1.2 2013/10/29 13:14:32EDT 277133 Development  $

#pragma once

#include "Summary1Page.h"
#include "Summary2Page.h"
#include "Summary3Page.h"
#include "SummaryPage.h"
#include "EarningsPage.h"
#include "InsuredPage.h"
#include "FamilyPage.h"
#include "OldStart1Page.h"
#include "OldStart2Page.h"
#include "PiaTable1Page.h"
#include "PiaTable2Page.h"
#include "WageInd1Page.h"
#include "WageInd2Page.h"
#include "TransGuar1Page.h"
#include "TransGuar2Page.h"
#include "SpecMin1Page.h"
#include "SpecMin2Page.h"
#include "ReindWid1Page.h"
#include "ReindWid2Page.h"
#include "FrozMinPage.h"
#include "ChildCareCalc1Page.h"
#include "ChildCareCalc2Page.h"
#include "WageIndNonFreeze1Page.h"
#include "WageIndNonFreeze2Page.h"
#include "TaxesPage.h"
#include "OnePage.h"
#include "DibGuarPage.h"
class TaxData;
class PiaCalAny;

/// <summary>>Manages the printing of the results of a Social Security
/// non-Statement calculation.</summary>
///
/// <remarks>It contains an instance of each of the pages of output related
/// to a non-Statement calculation. For a similar class that contains an
/// instance of each of the pages of output related to a Statement
/// calculation, see <see cref="PebsOut"/>.</remarks>
///
/// <seealso cref="PebsOut"/>
class PiaOut
{
public:
  /// <summary>Maximum number of re-indexed widow(er)s.</summary>
  static const int MAX_REINDWID = 5;
private:
  /// <summary>Number of reindexed widow(er) calculations.</summary>
  int widowCount;
public:
  /// <summary>Top part of summary page.</summary>
  Summary1Page summary1Page;
  /// <summary>Middle part of summary page.</summary>
  Summary2Page summary2Page;
  /// <summary>Bottom part of summary page.</summary>
  Summary3Page summary3Page;
  /// <summary>Summary page of results.</summary>
  SummaryPage summaryPage;
  /// <summary>Earnings used in calculation.</summary>
  EarningsPage earningsPage;
  /// <summary>Insured status calculation.</summary>
  InsuredPage insuredPage;
  /// <summary>Family members calculation.</summary>
  FamilyPage familyPage;
  /// <summary>Old-start earnings.</summary>
  OldStart1Page oldStart1Page;
  /// <summary>Old-start calculation.</summary>
  OldStart2Page oldStart2Page;
  /// <summary>Pia table earnings.</summary>
  PiaTable1Page piaTable1Page;
  /// <summary>Pia table calculation.</summary>
  PiaTable2Page piaTable2Page;
  /// <summary>Wage-indexed earnings.</summary>
  WageInd1Page wageInd1Page;
  /// <summary>Wage-indexed calculation.</summary>
  WageInd2Page wageInd2Page;
  /// <summary>Transitional guarantee earnings.</summary>
  TransGuar1Page transGuar1Page;
  /// <summary>Transitional guarantee calculation.</summary>
  TransGuar2Page transGuar2Page;
  /// <summary>Special minimum earnings.</summary>
  SpecMin1Page specMin1Page;
  /// <summary>Special minimum calculation.</summary>
  SpecMin2Page specMin2Page;
  /// <summary>Re-indexed widow guarantee earnings.</summary>
  ReindWid1Page* reindWid1Page[MAX_REINDWID];
  /// <summary>Re-indexed widow guarantee calculation.</summary>
  ReindWid2Page* reindWid2Page[MAX_REINDWID];
  /// <summary>Frozen minimum calculation.</summary>
  FrozMinPage frozMinPage;
  /// <summary>Disability guarantee calculation.</summary>
  DibGuarPage dibGuarPage;
  /// <summary>Wage-indexed earnings with child-care dropout years.</summary>
  ChildCareCalc1Page childCareCalc1Page;
  /// <summary>Wage-indexed calculation with child-care dropout years.
  /// </summary>
  ChildCareCalc2Page childCareCalc2Page;
  /// <summary>Wage-indexed earnings.</summary>
  WageIndNonFreeze1Page wageIndNonFreeze1Page;
  /// <summary>Wage-indexed calculation.</summary>
  WageIndNonFreeze2Page wageIndNonFreeze2Page;
  /// <summary>Taxes calculation.</summary>
  TaxesPage taxesPage;
  /// <summary>One-page output.</summary>
  OnePage onePage;
public:
  PiaOut( const PiaCalAny& piaCal, const TaxData& taxData );
  ~PiaOut();
  void prepareStrings();
  void setPageNum( const PiaCalAny& piaCal );
};
