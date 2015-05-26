// Declarations for the <see cref="TaxRatesLC"/> class to manage the Social
// Security payroll and self-employed tax rates, with changes from present law.

// $Id: TaxRatesLC.h 1.10 2011/08/11 08:46:58EDT 044579 Development  $

#pragma once

#include "TaxRates.h"
class LawChangeArray;

/// <summary>Manages the Social Security payroll and self-employed tax rates,
/// with changes from present law.</summary>
///
/// <remarks>Manages the Social Security payroll and self-employed tax rates,
/// with changes from present law.</remarks>
class TaxRatesLC : public TaxRates
{
protected:
  /// <summary>Law change data.</summary>
  const LawChangeArray& lawChange;
public:
  TaxRatesLC( int lastYear, const LawChangeArray& newLawChange );
  ~TaxRatesLC();
  void initialize();
private:
  TaxRatesLC operator=( TaxRatesLC newTaxRatesLC );
};
