// Declarations for the <see cref="SpecMinLC"/> class to manage special minimum
// pia calculations, including changes from present law.
//
// $Id: SpecMinLC.h 1.14 2011/08/09 15:55:30EDT 044579 Development  $

#pragma once

#include "SpecMin.h"
class LawChangeArray;

/// <summary>Manages the functions required for calculation of a special
/// minimum Social Security benefit, including changes from present law.
/// </summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalLC"/>.
/// </remarks>
///
/// <seealso cref="PiaCalLC"/>
class SpecMinLC : public SpecMin
{
private:
  /// <summary>Law change parameters.</summary>
  const LawChangeArray& lawChange;
public:
  SpecMinLC( const WorkerDataGeneral& newWorkerData,
    const PiaData& newPiaData, const PiaParams& newPiaParams,
    int newMaxyear, const LawChangeArray& newLawChange );
  ~SpecMinLC();
  int specMinYearsExcessCal() const;
  bool isApplicable();
  static bool isApplicable( const WorkerDataGeneral& workerData,
    const LawChangeArray& );
private:
  SpecMinLC operator=( SpecMinLC newSpecMinLC );
};
