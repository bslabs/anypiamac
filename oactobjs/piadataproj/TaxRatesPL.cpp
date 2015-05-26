// Functions for the <see cref="TaxRatesPL"/> class to manage the Social
// Security payroll and self-employed tax rates.

// $Id: TaxRatesPL.cpp 1.7 2011/08/11 08:46:57EDT 044579 Development  $

#include "TaxRatesPL.h"

/// <summary>Initializes arrays.</summary>
///
/// <remarks>Calls <see cref="initialize"/>.</remarks>
///
/// <param name="lastYear">Maximum projected year.</param>
TaxRatesPL::TaxRatesPL( int lastYear ) : TaxRates(lastYear)
{
  initialize();
}

/// <summary>Destroys this instance.</summary>
TaxRatesPL::~TaxRatesPL()
{ }

/// <summary>Sets all tax rates and credits to stored values.</summary>
/// <remarks>Calls <see cref="TaxRates::setHistRates"/> and
/// <see cref="TaxRates::setHistCredits"/>, specifying that all years are
/// being set. Finally calls <see cref="TaxRates::setGrossRates"/>.</remarks>
void TaxRatesPL::initialize()
{
  setHistRates(maxyear);
  setHistCredits(maxyear);
  setGrossRates();
}
