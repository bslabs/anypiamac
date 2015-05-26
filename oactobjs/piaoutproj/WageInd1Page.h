// Declarations for the <see cref="WageInd1Page"/> class to manage a wage
// indexed earnings page.

// $Id: WageInd1Page.h 1.11.1.1 2012/03/08 07:17:58EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;

/// <summary>Manages a wage indexed earnings page from the calculation of a
/// Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="WageInd2Page"/>.</remarks>
///
/// <seealso cref="WageInd2Page"/>
/// <seealso cref="PiaOut"/>
class WageInd1Page : public PiaPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pia calculation summary data.</summary>
  const PiaData& piaData;
  /// <summary>Pia calculation detailed data.</summary>
  const PiaCalAny& piaCal;
public:
  WageInd1Page( const PiaCalAny& newPiaCal );
  ~WageInd1Page();
  void prepareStrings();
private:
   // {secret}
   WageInd1Page& operator=( WageInd1Page& newWageInd1Page );
};
