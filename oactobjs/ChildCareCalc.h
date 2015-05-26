// Declarations for the <see cref="ChildCareCalc"/> class to manage childcare
// dropout year pia calculations.
//
// $Id: ChildCareCalc.h 1.16 2011/07/29 13:49:25EDT 044579 Development  $

#pragma once

#include "WageIndGeneral.h"

/// <summary>Manages the functions required for calculation of a wage indexed
/// Social Security benefit using childcare dropout years.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalPL"/>.
/// </remarks>
class ChildCareCalc : public WageIndGeneral
{
private:
  /// <summary>Number of computation years adjusted for childcare dropout
  /// years.</summary>
  int adjustedN;
  /// <summary>Maximum number of childcare dropout years.</summary>
  ///
  /// <remarks>Under present law, this is a maximum of 3. But it is less than
  /// that for a specific person if he/she already has some regular dropout
  /// years, or has a low number of computation years.</remarks>
  int childCareDropMax;
  /// <summary>Number of usable childcare dropout years.</summary>
  int childCareDrop;
public:
  ChildCareCalc( const WorkerDataGeneral& newWorkerData,
    const PiaData& newPiaData, const PiaParams& newPiaParams,
    int newMaxyear );
  virtual ~ChildCareCalc();
  void calculate();
  virtual int childCareDropMaxCal() const;
  void childCareDropoutCal();
  /// <summary>Returns number of computation years adjusted for childcare
  /// dropout years.</summary>
  ///
  /// <returns>Number of computation years adjusted for childcare
  /// dropout years.</returns>
  int getAdjustedN() const { return adjustedN; }
  /// <summary>Returns number of usable childcare dropout years.</summary>
  ///
  /// <returns>Number of usable childcare dropout years.</returns>
  int getChildCareDrop() const { return childCareDrop; }
  /// <summary>Returns maximum number of childcare dropout years.</summary>
  ///
  /// <returns>Maximum number of childcare dropout years.</returns>
  int getChildCareDropMax() const { return childCareDropMax; }
  void initialize();
  bool isApplicable();
  static bool isApplicable( const WorkerDataGeneral& workerData,
    const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi );
private:
  ChildCareCalc operator=( ChildCareCalc newChildCareCalc );
};
