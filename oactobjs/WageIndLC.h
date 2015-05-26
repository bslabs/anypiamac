// Declarations for the <see cref="WageIndLC"/> class to manage
// wage-indexed pia calculation, with law change parameters.

// $Id: WageIndLC.h 1.16 2011/08/11 13:55:33EDT 044579 Development  $

#pragma once

#include "WageInd.h"
class LawChangeArray;

/// <summary>Manages the functions required for calculation of a wage indexed
/// Social Security benefit, with changes from present law.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalLC"/>.
/// </remarks>
///
/// <seealso cref="PiaCalLC"/>
class WageIndLC : public WageInd
{
private:
  /// <summary>Entitlement date.</summary>
  const DateMoyr& entDate;
  /// <summary>Law change parameters.</summary>
  const LawChangeArray& lawChange;
public:
  WageIndLC( const WorkerDataGeneral& newWorkerData,
    const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear,
    const LawChangeArray& newLawChange, const DateMoyr& entitDate);
  ~WageIndLC();
  bool isApplicable();
  static bool isApplicable( const WorkerDataGeneral& workerData,
    const PiaData& piaData, const LawChangeArray& lawChange,
    const DateMoyr& entitDate );
private:
  WageIndLC operator=( WageIndLC newWageIndLC );
};
