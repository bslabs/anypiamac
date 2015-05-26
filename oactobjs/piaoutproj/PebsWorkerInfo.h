// Declarations for the <see cref="PebsWorkerInfo"/> class to manage the Social
// Security Statement worker info page.

// $Id: PebsWorkerInfo.h 1.10.1.1 2012/03/08 07:17:47EST 277133 Development  $

#pragma once

#include "PebsPageOut.h"
class Pebs;
class PiaParams;
class EarnProject;

/// <summary>Manages the Social Security Statement worker info page.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PebsOut"/>.
/// </remarks>
///
/// <seealso cref="PebsOut"/>
class PebsWorkerInfo : public PebsPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerData& workerData;
  /// <summary>Statement results.</summary>
  const Pebs& pebs;
  /// <summary>Pia calculation parameters.</summary>
  const PiaParams& piaParams;
  /// <summary>Annual earnings.</summary>
  const EarnProject& earnProject;
public:
  PebsWorkerInfo( const WorkerData& newWorkerData, const Pebs& newPebs,
    const PiaParams& newPiaParams, const EarnProject& newEarnProject );
  ~PebsWorkerInfo();
  void prepareStrings();
private:
  // {secret}
  PebsWorkerInfo& operator=( PebsWorkerInfo& newPebsWorkerInfo );
};
