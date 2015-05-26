// Declarations for the <see cref="PebsPageOut"/> class - parent of classes
// to write out results of Social Security Statement benefit calculation.

// $Id: PebsPageOut.h 1.8.1.1 2012/03/08 07:17:46EST 277133 Development  $

#pragma once

#include "PageOut.h"
class WorkerData;

/// <summary>Parent of classes that manage a page of Social Security Statement
/// output.</summary>
///
/// <remarks>Child classes are <see cref="PebsCreditInfo"/>,
/// <see cref="PebsEarnings"/>, <see cref="PebsEarningsInfo"/>,
/// <see cref="PebsLetter"/>, <see cref="PebsOasdiEst"/>,
/// <see cref="PebsOasdiInfo"/>, and <see cref="PebsWorkerInfo"/>.
///
/// For a similar class that is the parent of classes that manage a page of
/// non-Statement output, see <see cref="PiaPageOut"/>.</remarks>
///
/// <seealso cref="PebsCreditInfo"/>
/// <seealso cref="PebsEarnings"/>
/// <seealso cref="PebsEarningsInfo"/>
/// <seealso cref="PebsLetter"/>
/// <seealso cref="PebsOasdiEst"/>
/// <seealso cref="PebsOasdiInfo"/>
/// <seealso cref="PebsWorkerInfo"/>
/// <seealso cref="PiaPageOut"/>
class PebsPageOut : public PageOut
{
public:
  /// <summary>String of equals signs.</summary>
  static const std::string equals;
  /// <summary>String of hyphens.</summary>
  static const std::string hyphens;
private:
  /// <summary>Page number.</summary>
  int thePage;
public:
  PebsPageOut();
  ~PebsPageOut();
  void prepareHeader( const WorkerData& workerData );
  /// <summary>Sets page number.</summary>
  ///
  /// <param name="newPage">New page number.</param>
  void setPageNum( int newPage ) { thePage = newPage; }
};
