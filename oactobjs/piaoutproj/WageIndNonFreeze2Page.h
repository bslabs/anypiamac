// Declarations for the <see cref="WageIndNonFreeze2Page"/> class to manage a wage
// indexed results page.

// $Id: WageIndNonFreeze2Page.h 1.2 2014/01/05 07:54:56EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;
class UserAssumptions;

/// <summary>Manages a wage indexed non-freeze results page from the calculation of a
/// Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="WageIndNonFreeze1Page"/>.</remarks>
///
/// <seealso cref="WageIndNonFreeze1Page"/>
/// <seealso cref="PiaOut"/>
class WageIndNonFreeze2Page : public PiaPageOut
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
  WageIndNonFreeze2Page( const PiaCalAny& newPiaCal );
  ~WageIndNonFreeze2Page();
  void prepareStrings();
private:
  // {secret}
  WageIndNonFreeze2Page& operator=( WageIndNonFreeze2Page& newWageIndNonFreeze2Page );
};
