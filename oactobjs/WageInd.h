// Declarations for the <see cref="WageInd"/> class to manage wage-indexed
// pia calculation functions.

// $Id: WageInd.h 1.15 2011/08/11 13:55:32EDT 044579 Development  $

#pragma once

#include "WageIndGeneral.h"

/// <summary>Manages the functions required for calculation of a wage indexed
/// Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalPL"/>.
/// </remarks>
///
/// <seealso cref="PiaCalPL"/>
class WageInd : public WageIndGeneral
{
public:
  WageInd( const WorkerDataGeneral& newWorkerData, const PiaData& newPiaData,
    const PiaParams& newPiaParams, int newMaxyear );
  virtual ~WageInd();
  void calculate();
  bool isApplicable();
  static bool isApplicable( const WorkerDataGeneral& workerData,
    const PiaData& piaData );
private:
  WageInd operator=( WageInd newWageInd );
};
