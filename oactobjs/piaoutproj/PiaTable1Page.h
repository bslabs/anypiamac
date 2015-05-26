// Declarations for the <see cref="PiaTable1Page"/> class to manage a pia table
// calculation earnings page.

// $Id: PiaTable1Page.h 1.10.1.1 2012/03/08 07:17:50EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;

/// <summary>Manages the pia table calculation earnings page from the
/// calculation of a Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="PiaTable2Page"/>.</remarks>
///
/// <seealso cref="PiaTable2Page"/>
/// <seealso cref="PiaOut"/>
class PiaTable1Page : public PiaPageOut
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
  PiaTable1Page( const PiaCalAny& newPiaCal );
  ~PiaTable1Page();
  void prepareStrings();
private:
  // {secret}
  PiaTable1Page& operator=( PiaTable1Page& newPiaTable1Page );
};
