// Declarations for the <see cref="TaxRatesPL"/> class to manage the Social
// Security payroll and self-employed tax rates.

// $Id: TaxRatesPL.h 1.6 2011/05/10 11:40:09EDT 044579 Development  $

#pragma once

#include "TaxRates.h"

/// <summary>Manages the present law Social Security payroll and self-employed
/// tax rates.</summary>
///
/// <remarks>Manages the present law Social Security payroll and self-employed
/// tax rates.</remarks>
class TaxRatesPL : public TaxRates
{
public:
  TaxRatesPL( int lastYear );
  ~TaxRatesPL();
  void initialize();
};
