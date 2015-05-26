// Declarations for the <see cref="Summary2Page"/> class to manage the middle
// part of a summary of results page.

// $Id: Summary2Page.h 1.10.1.1 2012/03/08 07:17:54EST 277133 Development  $

#pragma once

#include "PageOut.h"
class PiaCalAny;
class WorkerDataGeneral;
class PiaData;

/// <summary>Manages the middle part of the summary of results page from the
/// calculation of a Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="Summary1Page"/> and
/// <see cref="Summary3Page"/>.</remarks>
///
/// <seealso cref="Summary1Page"/>
/// <seealso cref="Summary3Page"/>
/// <seealso cref="PiaOut"/>
class Summary2Page : public PageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pia calculation summary data.</summary>
  const PiaData& piaData;
  /// <summary>Pia calculation detailed data.</summary>
  const PiaCalAny& piaCal;
public:
  Summary2Page( const PiaCalAny& newPiaCal );
  ~Summary2Page();
  void prepareStrings();
private:
  // {secret}
  Summary2Page& operator=( Summary2Page& newSummary2Page );
};
