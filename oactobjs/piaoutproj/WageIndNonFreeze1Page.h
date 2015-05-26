// Declarations for the <see cref="WageIndNonFreeze1Page"/> class to manage a wage
// indexed earnings page.

// $Id: WageIndNonFreeze1Page.h 1.2 2014/01/05 07:54:55EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;

/// <summary>Manages a wage indexed non-freeze page from the calculation of a
/// Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="WageIndNonFreeze2Page"/>.</remarks>
///
/// <seealso cref="WageIndNonFreeze2Page"/>
/// <seealso cref="PiaOut"/>
class WageIndNonFreeze1Page : public PiaPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pia calculation summary data.</summary>
  const PiaData& piaData;
  /// <summary>Pia calculation detailed data.</summary>
  const PiaCalAny& piaCal;
public:
  WageIndNonFreeze1Page( const PiaCalAny& newPiaCal );
  ~WageIndNonFreeze1Page();
  void prepareStrings();
private:
   // {secret}
   WageIndNonFreeze1Page& operator=( WageIndNonFreeze1Page& newWageIndNonFreeze1Page );
};
