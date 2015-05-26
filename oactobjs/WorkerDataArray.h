// Declarations for the <see cref="WorkerDataArray"/> class to manage array
// of <see cref="WorkerData"/>'s.

// $Id: WorkerDataArray.h 1.11 2011/08/11 13:55:33EDT 044579 Development  $

#pragma once

#include "wrkrdata.h"

/// <summary>Manages an array of <see cref="WorkerData"/>'s, representing one
/// family.</summary>
///
/// <remarks>Manages an array of <see cref="WorkerData"/>'s, representing one
/// family.</remarks>
class WorkerDataArray
{
public:
  /// <summary>Constant for dimensioning.</summary>
  enum DimLimits {
    MAXFAMSIZE = 15  // Maximum family size.
  };
  /// <summary>Array of worker's pia data.</summary>
  WorkerData * workerData[MAXFAMSIZE];
private:
  /// <summary>Number of family members (not including worker).</summary>
  int famSize;
public:
  WorkerDataArray();
  ~WorkerDataArray();
  void deleteContents();
  /// <summary>Returns family size (not including worker).</summary>
  ///
  /// <returns>Family size (not including worker).</returns>
  int getFamSize() const { return famSize; }
  void setFamSize( int newFamSize );
  static void famSizeCheck( int newFamSize );
};
