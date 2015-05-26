// Declarations for the <see cref="ReindWid"/> class to manage reindexed
// widow(er) pia calculations.
//
// $Id: ReindWid.h 1.21 2011/08/09 14:58:42EDT 044579 Development  $

#pragma once

#include "WageIndGeneral.h"
#include "Secondary.h"

/// <summary>Manages the functions required for calculation of a reindexed
/// widow(er) Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalPL"/>.
/// </remarks>
class ReindWid : public WageIndGeneral
{
private:
  /// <summary>Year of eligibility.</summary>
  int eligYear;
protected:
  /// <summary>Widow calculation data.</summary>
  const PiaData& widowPiaData;
public:
  /// <summary>Widow basic data.</summary>
  ///
  /// <remarks>This is not used by this class, but it is handy to have
  /// when doing output.</remarks>
  const WorkerDataGeneral& widowData;
  /// <summary><see cref="Secondary"/> benefit data.</summary>
  Secondary& secondary;
public:
  ReindWid( const WorkerDataGeneral& newWorkerData,
    const PiaData& newPiaData, const PiaParams& newPiaParams,
    int newMaxyear, const PiaData& newWidowPiaData,
    const WorkerDataGeneral& newWidowData, Secondary& newSecondary );
  virtual ~ReindWid();
  void calculate();
  /// <summary>Returns eligibility year for reindexed widow(er)
  /// calculation.</summary>
  ///
  /// <returns>Eligibility year for reindexed widow(er) calculation.</returns>
  int getEligYear() const { return(eligYear); }
  void initialize();
  bool isApplicable();
  void setEligYear();
  static bool isApplicable( const WorkerDataGeneral& workerData,
    const PiaData& piaData, int widowEligYear, const Secondary& secondary );
private:
  ReindWid operator=( ReindWid newReindWid );
};
