// Declarations for the <see cref="EarningsPage"/> class to manage earnings
// output.

// $Id: EarningsPage.h 1.9.1.1 2012/03/08 07:17:37EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;
class UserAssumptions;

/// <summary>Manages printing of earnings used in the calculation of a Social
/// Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
/// </remarks>
///
/// <seealso cref="PiaOut"/>
class EarningsPage : public PiaPageOut
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
  /// <summary>User-specified assumptions</summary>
  const UserAssumptions& userAssumptions;
public:
  EarningsPage( const PiaCalAny& newPiaCal );
  ~EarningsPage();
  void prepareStrings();
private:
  // {secret}
  EarningsPage& operator=( EarningsPage& newEarningsPage );
};
