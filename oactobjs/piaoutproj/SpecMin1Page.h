// Declarations for the <see cref="SpecMin1Page"/> class to manage a special
// minimum calculation earnings page.

// $Id: SpecMin1Page.h 1.10.1.1 2012/03/08 07:17:52EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;

/// <summary>Manages a special minimum earnings page from the
/// calculation of a Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="SpecMin2Page"/>.</remarks>
///
/// <seealso cref="SpecMin2Page"/>
/// <seealso cref="PiaOut"/>
class SpecMin1Page : public PiaPageOut
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
public:
  SpecMin1Page( const PiaCalAny& newPiaCal );
  ~SpecMin1Page();
  void prepareStrings();
private:
  // {secret}
  SpecMin1Page& operator=( SpecMin1Page& newSpecMin1Page );
};
