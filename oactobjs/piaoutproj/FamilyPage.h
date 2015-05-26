// Declarations for the <see cref="FamilyPage"/> class to manage family
// members' benefits output.

// $Id: FamilyPage.h 1.9.1.1 2012/03/08 07:17:37EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;
class WorkerDataArray;
class PiaDataArray;
class SecondaryArray;

/// <summary>Manages the results for the auxiliary beneficiaries from the
/// calculation of a Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
/// </remarks>
///
/// <seealso cref="PiaOut"/>
class FamilyPage : public PiaPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pia calculation summary data.</summary>
  const PiaData& piaData;
  /// <summary>Widow and other family members' basic data.</summary>
  const WorkerDataArray& widowDataArray;
  /// <summary>Widow and other family members' calculation data.</summary>
  const PiaDataArray& widowArray;
  /// <summary>Pia calculation detailed data.</summary>
  const PiaCalAny& piaCal;
  /// <summary>Array of <see cref="Secondary"/> benefits.</summary>
  const SecondaryArray& secondaryArray;
public:
  FamilyPage( const PiaCalAny& newPiaCal );
  ~FamilyPage();
  void prepareStrings();
private:
  // {secret}
  FamilyPage& operator=( FamilyPage& newFamilyPage );
};
