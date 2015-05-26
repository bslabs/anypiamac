// Declarations for the <see cref="OldStart1Page"/> class to manage old start
// calculation earnings output.

// $Id: OldStart1Page.h 1.10.1.1 2012/03/08 07:17:39EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;

/// <summary>Manages old start earnings output from the calculation of a
/// Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="OldStart2Page"/>.</remarks>
///
/// <seealso cref="OldStart2Page"/>
/// <seealso cref="PiaOut"/>
class OldStart1Page : public PiaPageOut
{
private:
  /// <summary>Basic worker data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pia calculation summary data.</summary>
  const PiaData& piaData;
  /// <summary>Pia calculation parameters.</summary>
  const PiaParams& piaParams;
  /// <summary>Pia calculation detailed data.</summary>
  const PiaCalAny& piaCal;
public:
  OldStart1Page( const PiaCalAny& newPiaCal );
  ~OldStart1Page();
  void prepareStrings();
private:
  // {secret}
  OldStart1Page& operator=( OldStart1Page& newOldStart1Page );
};
