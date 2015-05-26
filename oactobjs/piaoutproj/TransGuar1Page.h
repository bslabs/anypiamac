// Declarations for the <see cref="TransGuar1Page"/> class to manage the
// transitional guarantee calculation earnings page.

// $Id: TransGuar1Page.h 1.10.1.1 2012/03/08 07:17:57EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;

/// <summary>Manages the transitional guarantee earnings page from the
/// calculation of a Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="TransGuar2Page"/>.</remarks>
///
/// <seealso cref="TransGuar2Page"/>
/// <seealso cref="PiaOut"/>
class TransGuar1Page : public PiaPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pia calculation summary data.</summary>
  const PiaData& piaData;
  /// <summary>Pia calculation detailed data.</summary>
  const PiaCalAny& piaCal;
public:
  TransGuar1Page( const PiaCalAny& newPiaCal );
  ~TransGuar1Page();
  void prepareStrings();
private:
  // {secret}
  TransGuar1Page& operator=( TransGuar1Page& newTransGuar1Page );
};
