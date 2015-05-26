// Declarations for the <see cref="AssumptionsNonFile"/> class that contains
// the average wage and benefit increase parameters (it does not read from
// disk).
//
// $Id: AssumptionsNonFile.h 1.11 2011/07/28 08:55:09EDT 044579 Development  $

#pragma once

#include "Assumptions.h"
#include "AwincNonFile.h"
#include "BiprojNonFile.h"
#include "CatchupNonFile.h"

/// <summary>Contains the average wage and benefit 
///   increase assumptions (it does not read from disk).</summary>
///
/// <remarks>Contains the average wage and benefit 
///   increase assumptions (it does not read from disk).</remarks>
class AssumptionsNonFile : public Assumptions
{
public:
  /// <summary>Projected average wage increases.</summary>
 AwincNonFile awincProj;
   /// <summary>Projected benefit increases.</summary>
  BiprojNonFile biProj;
  /// <summary>Catch-up benefit increases.</summary>
  CatchupNonFile catchupDoc;
public:
  AssumptionsNonFile( int newIstart, int newMaxyear );
  AssumptionsNonFile( int newMaxyear );
  ~AssumptionsNonFile();
  void averageWageMenu( int altNum );
  void benefitIncMenu( int altNum );
  /// <summary>Returns projected average wage increases.</summary>
  ///
  /// <returns>Projected average wage increases.</returns>
  const AwincDoc& getAwincProj() const { return awincProj; }
  /// <summary>Returns projected benefit increases.</summary>
  ///
  /// <returns>Projected benefit increases.</returns>
  const Biproj& getBiProj() const { return biProj; }
  /// <summary>Returns catch-up benefit increases.</summary>
  ///
  /// <returns>Catch-up benefit increases.</returns>
  const CatchupDoc& getCatchupDoc() const { return catchupDoc; }
  /// <summary>Set title of projected average wage increases.</summary>
  ///
  /// <param name="number">Trustees Report assumption.</param>
  void setAwincProjTitle( int number ) { awincProj.setTitle(number); }
  /// <summary>Set title of projected benefit increases.</summary>
  ///
  /// <param name="number">Trustees Report assumption.</param>
  void setBiProjTitle( int number ) { biProj.setTitle(number); }
  void setIstart( int newIstart );
};
