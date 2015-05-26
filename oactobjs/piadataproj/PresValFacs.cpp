// Functions for the <see cref="PresValFacs"/> class to handle present
// value factors.
//
// $Id: PresValFacs.cpp 1.6 2011/08/08 08:45:16EDT 044579 Development  $

#include "PresValFacs.h"

/// <summary>Constructor.</summary>
///
/// <remarks>Calculates the present value factors based on the interest
/// rates passed in.</remarks>
///
/// <param name="intRates">Array of interest rates.</param>
PresValFacs::PresValFacs( const DoubleAnnual& intRates ) :
DoubleAnnual(intRates.getBaseYear() - 1, intRates.getLastYear())
{
  // This factor is needed as a starting point for the multiplicative
  // present value factors.
  theData[0] = 1.0;
  project(intRates);
}

/// <summary>Destroys present value factors array.</summary>
PresValFacs::~PresValFacs()
{ }

/// <summary>Projects present value factors for the given interest rates.
/// </summary>
///
/// <param name="intRates">An array of interest rates.</param>
void PresValFacs::project( const DoubleAnnual& intRates )
{
  for (int yr = getBaseYear() + 1; yr <= getLastYear(); yr++) {
    const int index = yr - getBaseYear();
    theData[index] = theData[index - 1] / (1.0 + intRates[yr]);
  }
}
