// Declarations for the <see cref="TransGuar2Page"/> class to manage the
// transitional guarantee calculation results page.

// $Id: TransGuar2Page.h 1.10.1.1 2012/03/08 07:17:58EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;

/// <summary>Manages the transitional guarantee results page from the
/// calculation of a Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="TransGuar1Page"/>.</remarks>
///
/// <seealso cref="TransGuar1Page"/>
/// <seealso cref="PiaOut"/>
class TransGuar2Page : public PiaPageOut
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
  TransGuar2Page( const PiaCalAny& newPiaCal );
  ~TransGuar2Page();
  void prepareStrings();
private:
  // {secret}
  TransGuar2Page& operator=( TransGuar2Page& newTransGuar2Page );
};
