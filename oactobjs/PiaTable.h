// Declarations for the <see cref="PiaTable"/> class to manage Pia Table pia
// calculations.
//
// $Id: PiaTable.h 1.14 2011/08/08 08:45:15EDT 044579 Development  $

#pragma once

#include "OldPia.h"

/// <summary>Manages the functions required for calculation of a Pia Table
/// Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalPL"/>.
/// </remarks>
class PiaTable : public OldPia
{
public:
  PiaTable( const WorkerDataGeneral& newWorkerData, const PiaData& newPiaData,
    const PiaParams& newPiaParams, int newMaxyear );
  virtual ~PiaTable();
  void calculate();
  bool isApplicable();
  static bool isApplicable( const WorkerDataGeneral& workerData,
    const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi );
private:
  PiaTable operator=( PiaTable newPiaTable );
};
