// Declarations for the <see cref="FrozMinPage"/> class to manage frozen
// minimum calculation output.

// $Id: FrozMinPage.h 1.10.1.1 2012/03/08 07:17:38EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;

/// <summary>Manages the frozen minimum results from the calculation of a
/// Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
/// </remarks>
///
/// <seealso cref="PiaOut"/>
class FrozMinPage : public PiaPageOut
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
public:
  FrozMinPage( const PiaCalAny& newPiaCal );
  ~FrozMinPage();
  void prepareStrings();
private:
  // {secret}
  FrozMinPage& operator=( FrozMinPage& newFrozMinPage );
};
