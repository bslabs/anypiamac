// Declarations for the <see cref="ReindWidLC"/> class to manage reindexed
// widow(er) pia calculation with law changes.

// $Id: ReindWidLC.h 1.10 2011/08/09 14:58:43EDT 044579 Development  $

#pragma once

#include "ReindWid.h"
class LawChangeArray;

/// <summary>Manages the functions required for calculation of a reindexed
/// widow(er) Social Security benefit with law changes.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalLC"/>.
/// </remarks>
class ReindWidLC : public ReindWid
{
private:
  /// <summary>Law change parameters.</summary>
  const LawChangeArray& lawChange;
public:
  ReindWidLC( const WorkerDataGeneral& newWorkerData,
    const PiaData& newPiaData, const PiaParams& newPiaParams,
    int newMaxyear, const PiaData& newWidowPiaData,
    const WorkerDataGeneral& newWidowData, Secondary& newSecondary,
    const LawChangeArray& newLawChange );
  virtual ~ReindWidLC();
  bool isApplicable();
  static bool isApplicable( const WorkerDataGeneral& workerData,
    const PiaData& piaData, int widowEligYear,
    const Secondary& secondary, const LawChangeArray& lawChange );
private:
   ReindWidLC operator=( ReindWidLC newReindWidLC );
};
