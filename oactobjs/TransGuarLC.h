// Declarations for the <see cref="TransGuarLC"/> class to manage transitional
// guarantee pia calculations, with changes from present law.
//
// $Id: TransGuarLC.h 1.14 2011/08/11 08:46:58EDT 044579 Development  $

#pragma once

#include "TransGuar.h"
class LawChangeArray;

/// <summary>Manages the functions required for calculation of a transitional
/// guarantee Social Security benefit, with changes from present law.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalLC"/>.
/// </remarks>
class TransGuarLC : public TransGuar
{
private:
  /// <summary>Law change parameters.</summary>
  const LawChangeArray& lawChange;
public:
  TransGuarLC( const WorkerDataGeneral& newWorkerData,
    const PiaData& newPiaData, const PiaParams& newPiaParams,
    int newMaxyear, const LawChangeArray& newLawChange );
  ~TransGuarLC();
  bool isApplicable();
  static bool isApplicable( const WorkerDataGeneral& workerData,
    const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi,
    const LawChangeArray& lawChange );
private:
  TransGuarLC operator=( TransGuarLC newTransGuarLC );
};
