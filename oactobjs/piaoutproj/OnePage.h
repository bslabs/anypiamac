// Declarations for the <see cref="OnePage"/> class to manage one-page summary
// of results.

// $Id: OnePage.h 1.10.1.1 2012/03/08 07:17:41EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;

/// <summary>Manages one-page summary of results from the calculation of a
/// Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
/// </remarks>
///
/// <seealso cref="PiaOut"/>
class OnePage : public PiaPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pia calculation summary data.</summary>
  const PiaData& piaData;
  /// <summary>Pia calculation detailed data.</summary>
  const PiaCalAny& piaCal;
public:
  OnePage( const PiaCalAny& newPiaCal );
  ~OnePage();
  void prepareStrings();
  void printEarn( int firstyr, int lastyr, int tier );
private:
  // {secret}
  OnePage& operator=( OnePage& newOnePage );
};
