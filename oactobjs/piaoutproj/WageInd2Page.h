// Declarations for the <see cref="WageInd2Page"/> class to manage a wage
// indexed reuslts page.

// $Id: WageInd2Page.h 1.11.1.1 2012/03/08 07:17:59EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;
class UserAssumptions;

/// <summary>Manages a wage indexed results page from the calculation of a
/// Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="WageInd1Page"/>.</remarks>
///
/// <seealso cref="WageInd1Page"/>
/// <seealso cref="PiaOut"/>
class WageInd2Page : public PiaPageOut
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
  /// <summary>User-specified assumptions.</summary>
  const UserAssumptions& userAssumptions;
public:
  WageInd2Page( const PiaCalAny& newPiaCal );
  ~WageInd2Page();
  void prepareStrings();
private:
  // {secret}
  WageInd2Page& operator=( WageInd2Page& newWageInd2Page );
};
