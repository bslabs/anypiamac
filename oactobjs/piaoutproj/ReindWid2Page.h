// Declarations for the <see cref="ReindWid2Page"/> class to manage a
// re-indexed widow calculation results page.

// $Id: ReindWid2Page.h 1.13.1.1 2012/03/08 07:17:52EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;
class UserAssumptions;
class ReindWid;

/// <summary>Manages a re-indexed widow calculation results page from the
/// calculation of a Social Security benefit.</summary>
///
/// <remarks>Five instances of this class are created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="ReindWid1Page"/>.</remarks>
///
/// <seealso cref="ReindWid1Page"/>
/// <seealso cref="PiaOut"/>
class ReindWid2Page : public PiaPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pia calculation summary data.</summary>
  const PiaData& piaData;
  /// <summary>Pia calculation parameters.</summary>
  const PiaParams& piaParams;
  /// <summary>Pia calculation detailed data.</summary>
  const PiaCalAny& piaCal;
  /// <summary>User-specified assumptions.</summary>
  const UserAssumptions& userAssumptions;
public:
  /// <summary>Reindexed widow(er) calculation.</summary>
  const ReindWid* reindWid;
public:
  ReindWid2Page( const PiaCalAny& newPiaCal );
  ~ReindWid2Page();
  void prepareStrings();
private:
  // {secret}
  ReindWid2Page& operator=( ReindWid2Page& newReindWid2Page );
};
