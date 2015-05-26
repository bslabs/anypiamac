// Declarations for the <see cref="PebsEarnings"/> class to manage Social
// Security Statement earnings page.

// $Id: PebsEarnings.h 1.10.1.1 2012/03/08 07:17:42EST 277133 Development  $

#pragma once

#include "PebsPageOut.h"
class PiaData;
class TaxData;
class PiaParams;

/// <summary>Manages the Social Security Statement earnings page.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PebsOut"/>.
/// </remarks>
///
/// <seealso cref="PebsOut"/>
class PebsEarnings : public PebsPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerData& workerData;
  /// <summary>Pia calculation data.</summary>
  const PiaData& piaData;
  /// <summary>Pia calculation parameters.</summary>
  const PiaParams& piaParams;
  /// <summary>Tax amounts.</summary>
  const TaxData& taxData;
public:
  PebsEarnings( const WorkerData& newWorkerData, const PiaData& newPiaData,
    const PiaParams& newPiaParams, const TaxData& newTaxData );
  ~PebsEarnings();
  void prepareStrings();
private:
  // {secret}
  PebsEarnings& operator=( PebsEarnings& newPebsEarnings );
};
