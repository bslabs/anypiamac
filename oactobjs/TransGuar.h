// Declarations for the <see cref="TransGuar"/> class to manage transitional
// guarantee pia calculations.
//
// $Id: TransGuar.h 1.15 2011/08/11 08:46:59EDT 044579 Development  $

#pragma once

#include "OldPia.h"

/// <summary>Manages the functions required for calculation of a transitional
/// guarantee Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalPL"/>.
/// </remarks>
///
/// <seealso cref="PiaCalPL"/>
class TransGuar : public OldPia
{
public:
  /// <summary>Bend points for MFB formula.</summary>
  BendMfb bendMfb;
  /// <summary>Wage-indexed MFB formula percentages.</summary>
  PercMfb percMfb;
  /// <summary>Portion of AIME PIA in each interval of MFB formula.</summary>
  double portionPiaElig[4];
public:
  TransGuar( const WorkerDataGeneral& newWorkerData,
    const PiaData& newPiaData, const PiaParams& newPiaParams,
    int newMaxyear );
  virtual ~TransGuar();
  void calculate();
  void initialize();
  bool isApplicable();
  static bool isApplicable( const WorkerDataGeneral& workerData,
    const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi );
private:
  TransGuar operator=( TransGuar newTransGuar );
};
