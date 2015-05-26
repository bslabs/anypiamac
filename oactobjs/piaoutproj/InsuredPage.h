// Declarations for the <see cref="InsuredPage"/> class to manage insured
// status output.

// $Id: InsuredPage.h 1.9.1.1 2012/03/08 07:17:39EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;
class SecondaryArray;

/// <summary>Manages insured status output determined in the calculation of a
/// Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
/// </remarks>
///
/// <seealso cref="PiaOut"/>
class InsuredPage : public PiaPageOut
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
  /// <summary>Array of <see cref="Secondary"/> benefits.</summary>
  const SecondaryArray& secondaryArray;
public:
  InsuredPage( const PiaCalAny& newPiaCal );
  ~InsuredPage();
  void prepareStrings();
private:
  // {secret}
  InsuredPage& operator=( InsuredPage& newInsuredPage );
};
