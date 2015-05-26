// Declarations for the <see cref="SummaryPage"/> class to manage the summary
// page of results.

// $Id: SummaryPage.h 1.7.1.1 2012/03/08 07:17:56EST 277133 Development  $

#pragma once

#include "Summary1Page.h"
#include "Summary2Page.h"
#include "Summary3Page.h"

/// <summary>Manages the summary page of results from the calculation of a
/// Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// It contains references to a <see cref="Summary1Page"/>,
/// <see cref="Summary2Page"/>, and <see cref="Summary3Page"/>.</remarks>
///
/// <seealso cref="Summary1Page"/>
/// <seealso cref="Summary2Page"/>
/// <seealso cref="Summary3Page"/>
/// <seealso cref="PiaOut"/>
class SummaryPage : public PageOut
{
private:
  /// <summary>Top part of summary page.</summary>
  const Summary1Page& summary1Page;
  /// <summary>Middle part of summary page.</summary>
  const Summary2Page& summary2Page;
  /// <summary>Bottom part of summary page.</summary>
  const Summary3Page& summary3Page;
public:
  SummaryPage( const Summary1Page& newSummary1Page,
    const Summary2Page& newSummary2Page,
    const Summary3Page& newSummary3Page );
  ~SummaryPage();
  void prepareStrings();
private:
  // {secret}
  SummaryPage& operator=( SummaryPage& newSummaryPage );
};
