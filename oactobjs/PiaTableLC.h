// Declarations for the <see cref="PiaTableLC"/> class to manage Pia Table pia
// calculations, with changes from present law.

// $Id: PiaTableLC.h 1.16 2011/08/08 08:45:14EDT 044579 Development  $

#pragma once

#include "PiaTable.h"
class LawChangeArray;

/// <summary>Manages the functions required for calculation of a Pia Table
///   Social Security benefit, including changes from present law.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalLC"/>.
/// </remarks>
class PiaTableLC : public PiaTable
{
private: 
  /// <summary>Entitlement date.</summary>
  const DateMoyr& entDate;
  /// <summary>Law change parameters.</summary>
  const LawChangeArray& lawChange;
public:
  PiaTableLC( const WorkerDataGeneral& newWorkerData,
    const PiaData& newPiaData, const PiaParams& newPiaParams,
    int newMaxyear, const LawChangeArray& newLawChange,
    const DateMoyr& entitDate);
  virtual ~PiaTableLC();
  bool isApplicable();
  static bool isApplicable( const WorkerDataGeneral& workerData,
    const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi,
    const LawChangeArray& lawChange, const DateMoyr& entitDate );
private:
  PiaTableLC operator=( PiaTableLC newPiaTableLC );
};
