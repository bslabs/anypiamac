// Functions for the <see cref="CompPeriod"/> class to manage computation
// period variables.
//
// $Id: CompPeriod.cpp 1.5 2011/07/29 13:49:26EDT 044579 Development 277133(2013/09/25 09:22:20EDT) $

#include "comppd.h"

/// <summary>Creates a CompPeriod with a given base year.</summary>
///
/// <param name="newBaseYear">Base year to use (1936 or 1950).</param>
CompPeriod::CompPeriod ( int newBaseYear ) : baseYear(newBaseYear), diYears(0),
n(0), nDrop(0), nElapsed(0)
{ }

/// <summary>Zeroes all variables.</summary>
void CompPeriod::deleteContents()
{
  n = nDrop = nElapsed = diYears = 0;
}
