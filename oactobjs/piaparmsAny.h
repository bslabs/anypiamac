// Declarations for the <see cref="PiaParamsAny"/> class to manage the Social
// Security parameters required to calculate Social Security benefits in the
// Anypia program.
//
// $Id: piaparmsAny.h 1.17 2011/08/05 16:30:20EDT 044579 Development  $

#pragma once

#include "dbleann.h"
#include "piaparmsLC.h"

/// <summary>Manages the additional Social Security parameters required to
/// calculate Social Security benefits in the Anypia program.</summary>
///
/// <remarks>Manages the additional Social Security parameters required to
/// calculate Social Security benefits in the Anypia program.</remarks>
class PiaParamsAny : public PiaParamsLC
{
private:
  /// <summary>High wage, 1937 to maxyear.</summary>
  DoubleAnnual wageHigh;
  /// <summary>Low wage, 1937 to maxyear.</summary>
  DoubleAnnual wageMin;
public:
  PiaParamsAny( int newIstart, int newMaxyear, AwbiData& awbiData,
    const LawChangeArray& newLawChange );
  virtual ~PiaParamsAny();
  /// <summary>Returns array of high earnings.</summary>
  ///
  /// <returns>Array of high earnings.</returns>
  const DoubleAnnual& getWageHighArray() const { return(wageHigh); }
  /// <summary>Returns high earnings for one year.</summary>
  ///
  /// <returns>High earnings for specified year.</returns>
  ///
  /// <param name="year"> Year for which high earnings are desired.</param>
  double getWageHigh( int year ) const { return(wageHigh[year]); }
  /// <summary>Returns array of low earnings.</summary>
  ///
  /// <returns>Array of low earnings.</returns>
  const DoubleAnnual& getWageMinArray() const { return(wageMin); }
  /// <summary>Returns low earnings for one year.</summary>
  ///
  /// <returns>Low earnings for specified year.</returns>
  ///
  /// <param name="year"> Year for which low earnings are desired.</param>
  double getWageMin( int year ) const { return(wageMin[year]); }
  void highProj();
  void minProj();
  void projectFq();
private:
  PiaParamsAny operator=( PiaParamsAny newPiaParamsAny );
};
