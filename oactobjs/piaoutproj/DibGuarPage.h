// Declarations for the <see cref="DibGuarPage"/> class to manage disability
// guarantee calculation output.

// $Id: DibGuarPage.h 1.7.1.1 2012/03/08 07:17:36EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;

/// <summary>Manages the disability guarantee results from
/// the calculation of a Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
/// </remarks>
///
/// <seealso cref="PiaOut"/>
class DibGuarPage : public PiaPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pia calculation summary data.</summary>
  const PiaData& piaData;
  /// <summary>Pia calculation parameters.</summary>
  const PiaParams& piaParams;
  /// <summary>Pia calculation detailed data.</summary>
  const PiaCalAny& piaCal;
protected:
  void piamfbDeath( const PiaMethod& piaMethod );
public:
  DibGuarPage( const PiaCalAny& newPiaCal );
  ~DibGuarPage();
  void prepareStrings();
private:
  // {secret}
  DibGuarPage& operator=( DibGuarPage& newDibGuarPage );
};
