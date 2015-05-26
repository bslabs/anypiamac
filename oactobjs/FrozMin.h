// Declarations for the <see cref="FrozMin"/> class to manage frozen minimum
// pia calculations.
//
// $Id: FrozMin.h 1.12 2011/07/29 15:26:17EDT 044579 Development  $

#pragma once

#include "PiaMethod.h"

/// <summary>Manages the functions required for calculation of a frozen minimum
/// Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalPL"/> and
/// <see cref="PiaCalLC"/>.</remarks>
class FrozMin : public PiaMethod
{
public:
  FrozMin( const WorkerDataGeneral& newWorkerData,
    const PiaData& newPiaData, const PiaParams& newPiaParams,
    int newMaxyear );
  virtual ~FrozMin();
  void calculate();
  bool isApplicable();
  void setFrozMin();
  static bool isApplicable( const WorkerDataGeneral& workerData,
    const PiaData& piaData );
private:
  FrozMin operator=( FrozMin newFrozMin );
};
