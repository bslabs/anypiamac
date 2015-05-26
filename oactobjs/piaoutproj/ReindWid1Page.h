// Declarations for the <see cref="ReindWid1Page"/> class to manage a
// re-indexed widow earnings page.

// $Id: ReindWid1Page.h 1.12.1.1 2012/03/08 07:17:51EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;
class UserAssumptions;
class ReindWid;

/// <summary>Manages a re-indexed widow earnings page from the
/// calculation of a Social Security benefit.</summary>
///
/// <remarks>Five instances of this class are created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="ReindWid2Page"/>.</remarks>
///
/// <seealso cref="ReindWid2Page"/>
/// <seealso cref="PiaOut"/>
class ReindWid1Page : public PiaPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pia calculation summary data.</summary>
  const PiaData& piaData;
  /// <summary>Pia calculation detailed data.</summary>
  const PiaCalAny& piaCal;
  /// <summary>User-specified assumptions.</summary>
  const UserAssumptions& userAssumptions;
public:
  /// <summary>Reindexed widow(er) calculation.</summary>
  const ReindWid* reindWid;
public:
  ReindWid1Page( const PiaCalAny& newPiaCal );
  ~ReindWid1Page();
  void prepareStrings();
private:
  // {secret}
  ReindWid1Page& operator=( ReindWid1Page& newReindWid1Page );
};
