// Declarations for the <see cref="PebsOasdiEst"/> class to manage the Social
// Security Statement oasdi estimates info page.

// $Id: PebsOasdiEst.h 1.9.1.1 2012/03/08 07:17:44EST 277133 Development  $

#pragma once

#include "PebsPageOut.h"
class Pebs;

/// <summary>Manages the Social Security Statement oasdi estimates page.
/// </summary>
///
/// <remarks>An instance of this class is created in <see cref="PebsOut"/>.
/// </remarks>
///
/// <seealso cref="PebsOut"/>
class PebsOasdiEst : public PebsPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerData& workerData;
  /// <summary>Statement results.</summary>
  const Pebs& pebs;
public:
  PebsOasdiEst( const WorkerData& newWorkerData, const Pebs& newPebs );
  ~PebsOasdiEst();
  void prepareStrings();
private:
  // {secret}
  PebsOasdiEst& operator=( PebsOasdiEst& newPebsOasdiEst );
};
