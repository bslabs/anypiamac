// Declarations for the <see cref="WageIndNonFreeze"/> class to manage wage-indexed
// non-freeze pia calculation functions.

// $Id: WageIndNonFreeze.h 1.2 2014/01/05 07:55:54EST 277133 Development  $

#pragma once

#include "WageIndGeneral.h"

/// <summary>Manages the functions required for calculation of a wage indexed
/// non-freeze Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalPL"/>.
/// </remarks>
///
/// <seealso cref="PiaCalPL"/>
class WageIndNonFreeze : public WageIndGeneral
{
public:
  WageIndNonFreeze( const WorkerDataGeneral& newWorkerData, const PiaData& newPiaData,
    const PiaParams& newPiaParams, int newMaxyear );
  virtual ~WageIndNonFreeze();
  void calculate();
  bool isApplicable();
  static bool isApplicable( const WorkerDataGeneral& workerData,
    const PiaData& piaData );
  void indexEarnings( int year1, int year2, int year3,
    const DoubleAnnual& earnings, DoubleAnnual& earnMultiplied,
    DoubleAnnual& earnIndexed, const DoubleAnnual& avgWage );
private:
  WageIndNonFreeze operator=( WageIndNonFreeze newWageIndNonFreeze );
};
