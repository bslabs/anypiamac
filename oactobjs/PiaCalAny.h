// Declarations for the <see cref="PiaCalAny"/> class to manage pia
// calculation functions used in Anypia.

// $Id: PiaCalAny.h 1.35 2011/08/01 16:22:23EDT 044579 Development  $

#pragma once

#include "PiaCalLC.h"
#include "pebs.h"
class PiaParamsAny;
class EarnProject;
class UserAssumptions;
class Assumptions;

/// <summary>Manages the additional functions required for calculation of a
/// Social Security benefit in the Anypia program.</summary>
///
/// <remarks>An instance of this class is created in the Anypia program.
/// </remarks>
class PiaCalAny : public PiaCalLC
{
public:
  /// <summary>Widow and other family member calculation data.</summary>
  WorkerDataArray& widowDataArray;
  /// <summary>Widow and other family member calculation data.</summary>
  PiaDataArray& widowArray;
  /// <summary>Array of <see cref="Secondary"/> benefits.</summary>
  SecondaryArray& secondaryArray;
  /// <summary>Projected earnings.</summary>
  EarnProject& earnProject;
  /// <summary>User-specified assumptions.</summary>
  UserAssumptions& userAssumptions;
private:
  /// <summary>Pia calculation parameters.</summary>
  PiaParamsAny& piaParamsAny;
  /// <summary>Statement data.</summary>
  Pebs& pebs;
public:
  PiaCalAny( WorkerDataGeneral& newWorkerData, PiaData& newPiaData,
    WorkerDataArray& newWidowDataArray, PiaDataArray& newWidowArray,
    PiaParamsAny& newPiaParamsAny, UserAssumptions& newUserAssumptions,
    SecondaryArray& newSecondary, const LawChangeArray& newLawChange,
    Pebs& newPebs, EarnProject& newEarnProject );
  ~PiaCalAny();
  void calculate1( Assumptions& assumptions );
  void calculate2( const DateMoyr& entDate );
  void dataCheck( const DateMoyr& entDate );
  void earnProjection() const;
  void earnProSteady();
  void pebsOabCal();
  void pebsSetup( Pebs::pebes_type type, const DateMoyr& entDate );
private:
  PiaCalAny operator=( PiaCalAny newPiaCalAny );
};
