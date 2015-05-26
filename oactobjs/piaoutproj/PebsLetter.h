// Declarations for the <see cref="PebsLetter"/> class to manage the Social
// Security Statement letter page.

// $Id: PebsLetter.h 1.9.1.1 2012/03/08 07:17:44EST 277133 Development  $

#pragma once

#include "PebsPageOut.h"
class FieldOfficeInfo;

/// <summary>Manages the Social Security Statement letter page.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PebsOut"/>.
/// </remarks>
///
/// <seealso cref="PebsOut"/>
class PebsLetter : public PebsPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerData& workerData;
  /// <summary>Field office information.</summary>
  const FieldOfficeInfo& fieldOfficeInfo;
public:
  PebsLetter( const WorkerData& newWorkerData,
    const FieldOfficeInfo& newFieldOfficeInfo );
  ~PebsLetter();
  void prepareStrings();
private:
  // {secret}
  PebsLetter& operator=( PebsLetter& newPebsLetter );
};
