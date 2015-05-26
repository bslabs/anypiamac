// Declarations for the <see cref="PiaTable2Page"/> class to manage a pia table
// calculation results page.

// $Id: PiaTable2Page.h 1.10.1.1 2012/03/08 07:17:50EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;

/// <summary>Manages the pia table calculation results page from the
/// calculation of a Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="PiaTable1Page"/>.</remarks>
///
/// <seealso cref="PiaTable1Page"/>
/// <seealso cref="PiaOut"/>
class PiaTable2Page : public PiaPageOut
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
  PiaTable2Page( const PiaCalAny& newPiaCal );
  ~PiaTable2Page();
  void prepareStrings();
private:
  // {secret}
  PiaTable2Page& operator=( PiaTable2Page& newPiaTable2Page );
};
