// Declarations for the <see cref="PebsOasdiInfo"/> class to manage the Social
// Security Statement oasdi benefit info page.

// $Id: PebsOasdiInfo.h 1.9.1.1 2012/03/08 07:17:45EST 277133 Development  $

#pragma once

#include "PebsPageOut.h"

/// <summary>Manages the Social Security Statement oasdi benefit info page.
/// </summary>
///
/// <remarks>An instance of this class is created in <see cref="PebsOut"/>.
/// </remarks>
///
/// <seealso cref="PebsOut"/>
class PebsOasdiInfo : public PebsPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerData& workerData;
public:
  PebsOasdiInfo( const WorkerData& newWorkerData );
  ~PebsOasdiInfo();
  void prepareStrings();
private:
  // {secret}
  PebsOasdiInfo& operator=( PebsOasdiInfo& newPebsOasdiInfo );
};
