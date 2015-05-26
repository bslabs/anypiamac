// Declarations for the <see cref="PiaDataArray"/> class to manage an array
// of <see cref="PiaData"/>'s, representing one family.
//
// $Id: PiadataArray.h 1.12 2011/08/01 16:22:23EDT 044579 Development  $

#pragma once

#include "piadata.h"

/// <summary>Manages an array of <see cref="PiaData"/>'s, representing one
/// family.</summary>
///
/// <remarks>Manages an array of <see cref="PiaData"/>'s, representing one
/// family.</remarks>
class PiaDataArray
{
public:
  /// <summary>Constant for dimensioning.</summary>
  enum DimLimits {
    MAXFAMSIZE = 15  // Maximum family size.
  };
  /// <summary>Array of worker's pia data.</summary>
  PiaData* piaData[MAXFAMSIZE];
private:
  /// <summary>Number of family members (not including worker).</summary>
  int famSize;
public:
  PiaDataArray();
  ~PiaDataArray();
  void deleteContents();
  /// <summary>Returns family size (not including worker).</summary>
  ///
  /// <returns>Family size (not including worker).</returns>
  int getFamSize() const { return famSize; }
  void setFamSize( int newFamSize );
  static void famSizeCheck( int newFamSize );
};
