// Declarations for the <see cref="TaxesPage"/> class to manage the taxes
// output page.

// $Id: TaxesPage.h 1.10.1.1 2012/03/08 07:17:56EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class TaxData;

/// <summary>Manages the taxes output page.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
/// </remarks>
///
/// <seealso cref="PiaOut"/>
class TaxesPage : public PiaPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pia calculation summary data.</summary>
  const PiaData& piaData;
  /// <summary>Pia calculation parameters.</summary>
  const TaxData& taxData;
public:
  TaxesPage( const WorkerDataGeneral& newWorkerData,
    const PiaData& newPiaData, const TaxData& newTaxData );
  ~TaxesPage();
  void prepareStrings();
private:
  // {secret}
  TaxesPage& operator=( TaxesPage& newTaxesPage );
};
