// Declarations for the <see cref="SpecMin2Page"/> class to manage a special
// minimum calculation results page.

// $Id: SpecMin2Page.h 1.10.1.1 2012/03/08 07:17:53EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;

/// <summary>Manages a special minimum calculation details page from the
/// calculation of a Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="SpecMin1Page"/>.</remarks>
///
/// <seealso cref="SpecMin1Page"/>
/// <seealso cref="PiaOut"/>
class SpecMin2Page : public PiaPageOut
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
  SpecMin2Page( const PiaCalAny& newPiaCal );
  ~SpecMin2Page();
  void prepareStrings();
private:
  // {secret}
  SpecMin2Page& operator=( SpecMin2Page& newSpecMin2Page );
};
