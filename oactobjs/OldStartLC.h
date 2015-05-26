// Declarations for the <see cref="OldStartLC"/> class to manage old-start pia
// calculations, with changes from present law.
//
// $Id: OldStartLC.h 1.17 2011/08/01 11:38:36EDT 044579 Development  $

#pragma once

#include "OldStart.h"
class LawChangeArray;

/// <summary>Manages the functions required for calculation of an old start
/// Social Security benefit with law changes.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalLC"/>.
/// </remarks>
class OldStartLC : public OldStart
{
protected:
  /// <summary>Law change parameters.</summary>
  const LawChangeArray& lawChange;
public:
  OldStartLC( const WorkerDataGeneral& newWorkerData,
    const PiaData& newPiaData, const PiaParams& newPiaParams,
    int newMaxyear, const DateMoyr& newEntDate,
    const LawChangeArray& newLawChange );
  virtual ~OldStartLC();
  bool isApplicable();
  static bool isApplicable( const DateMoyr& entDate,
    const DateMoyr& benefitDate, const PiaData& piaData,
    const LawChangeArray& lawChange );
private:
  OldStartLC operator=( OldStartLC newOldStartLC );
};
