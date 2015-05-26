// Declarations for the <see cref="PebsWorkerEarnings"/> class to manage the
// Social Security Statement worker info and earnings page.

// $Id: PebsWorkerEarnings.h 1.7.1.1 2012/03/08 07:17:47EST 277133 Development  $

#pragma once

#include "PageOut.h"
class PebsWorkerInfo;
class PebsEarningsInfo;

/// <summary>Manages the Social Security Statement worker info and earnings
/// info page.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PebsOut"/>.
/// </remarks>
///
/// <seealso cref="PebsOut"/>
class PebsWorkerEarnings : public PageOut
{
private:
  /// <summary>Social Security Statement worker info.</summary>
  const PebsWorkerInfo& pebsWorkerInfo;
  /// <summary>Social Security Statement earnings info.</summary>
  const PebsEarningsInfo& pebsEarningsInfo;
public:
  PebsWorkerEarnings( const PebsWorkerInfo& newPebsWorkerInfo,
    const PebsEarningsInfo& newPebsEarningsInfo );
  ~PebsWorkerEarnings();
  void prepareStrings();
private:
  // {secret}
  PebsWorkerEarnings& operator=( PebsWorkerEarnings& newPebsWorkerEarnings );
};
