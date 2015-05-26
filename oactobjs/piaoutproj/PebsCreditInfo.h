// Declarations for the <see cref="PebsCreditInfo"/> class to manage the Social
// Security Statement credit info page.

// $Id: PebsCreditInfo.h 1.9.1.1 2012/03/08 07:17:42EST 277133 Development  $

#pragma once

#include "PebsPageOut.h"
class PiaParams;

/// <summary>Manages the Social Security Statement credit info page.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PebsOut"/>.
/// </remarks>
///
/// <seealso cref="PebsOut"/>
class PebsCreditInfo : public PebsPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerData& workerData;
  /// <summary>Pia calculation parameters.</summary>
  const PiaParams& piaParams;
public:
  PebsCreditInfo( const WorkerData& newWorkerData,
    const PiaParams& newPiaParams );
  ~PebsCreditInfo();
  void prepareStrings();
private:
  // {secret}
  PebsCreditInfo& operator=( PebsCreditInfo& newPebsCreditInfo );
};
