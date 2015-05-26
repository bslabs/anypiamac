// Declarations for the <see cref="ChildCareCalcLC"/> class to manage childcare
// dropout year pia calculations, with law changes.
//
// $Id: ChildCareCalcLC.h 1.14 2011/07/29 13:49:26EDT 044579 Development  $

#pragma once

#include "ChildCareCalc.h"
class LawChangeArray;

/// <summary>Manages the functions required for calculation of a wage indexed
/// Social Security benefit using childcare dropout years, with law changes.
/// </summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalLC"/>.
/// </remarks>
class ChildCareCalcLC : public ChildCareCalc
{
protected:
  /// <summary>Law change parameters.</summary>
  const LawChangeArray& lawChange;
public:
  ChildCareCalcLC( const WorkerDataGeneral& newWorkerData,
    const PiaData& newPiaData, const PiaParams& newPiaParams,
    int newMaxyear, const LawChangeArray& newLawChange );
  virtual ~ChildCareCalcLC();
  int childCareDropMaxCal() const;
  bool isApplicable();
  static bool isApplicable( const WorkerDataGeneral& workerData,
    const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi,
    const LawChangeArray& lawChange );
private:
  ChildCareCalcLC operator=( ChildCareCalcLC newChildCareCalcLC );
};
