// Declarations for the <see cref="DibGuarLC"/> class to manage disability
// guarantee pia calculations with law changes.
//
// $Id: DibGuarLC.h 1.5 2011/07/29 15:26:16EDT 044579 Development  $

#pragma once

#include "DibGuar.h"
class LawChangeArray;

/// <summary>Manages the functions required for calculation of a disability
/// guarantee Social Security benefit with law changes.</summary>
///
/// <remarks>This method is used when a person has recovered from one period of
/// disability and is claiming a subsequent benefit (old-age, disability, or
/// death). An instance of this class is created in <see cref="PiaCalLC"/>.
/// </remarks>
class DibGuarLC : public DibGuar
{
private:
  /// <summary>Law change parameters.</summary>
  const LawChangeArray& lawChange;
public:
  DibGuarLC( const WorkerDataGeneral& newWorkerData,
    const PiaData& newPiaData, const PiaParams& newPiaParams,
    int newMaxyear, const LawChangeArray& newLawChange );
  virtual ~DibGuarLC();
  bool isApplicable();
  static bool isApplicable( const DateMoyr& entDate,
    const WorkerDataGeneral& workerData, const PiaData& piaData,
    WorkerDataGeneral::ben_type ioasdi, const LawChangeArray& lawChange );
private:
  DibGuarLC operator=( DibGuarLC newDibGuarLC );
};
