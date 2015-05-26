// Functions for the <see cref="Awinc"/> class to manage a vector of
// average wage increases.
//
// $Id: awinc.cpp 1.18 2011/07/28 10:16:26EDT 044579 Development  $

#include "awinc.h"
#include "avgwg.h"
#include "Resource.h"
#include "oactcnst.h"
#include "PiaException.h"

/// <summary>Initializes the first year of data array to 1937.</summary>
///
/// <param name="newLastYear">Last year of array.</param>
Awinc::Awinc( int newLastYear ) : DoubleAnnual(YEAR37, newLastYear)
{ }

/// <summary>Initializes the first year of data array to the specified year.
/// </summary>
///
/// <param name="newLastYear">Last year of array.</param>
/// <param name="newBaseYear">First year of array.</param>
Awinc::Awinc( int newBaseYear, int newLastYear ) :
DoubleAnnual(newBaseYear, newLastYear)
{ }

/// <summary>Checks an average wage increase.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FQINC1"/> if average wage increase is out of range.
/// </exception>
///
/// <param name="fqinct">Value to check.</param>
void Awinc::fqincCheck( double fqinct )
{
  if (fqinct > 99.99 || fqinct < -9.99) {
    throw PiaException(PIA_IDS_FQINC1);
  }
}

/// <summary>Calculates average wage increases from the
/// <see cref="AverageWage"/> argument and assigns each one to the
/// corresponding slot in this array.</summary>
///
/// <param name="averageWages">Series of average wages from which to calculate
/// increase percentages.</param>
/// <param name="firstYeart">First year of calculation.</param>
/// <param name="lastYeart">Last year of calculation.</param>
void Awinc::project( const AverageWage& averageWages, int firstYeart,
int lastYeart )
{
  for (int yr = firstYeart; yr <= lastYeart; yr++) {
    if (averageWages[yr - 1] > 1.0) {
      theData[yr - getBaseYear()] =
        100.0 * (averageWages[yr] / averageWages[yr - 1] - 1.0);
    }
  }
}

/// <summary>Calculates average wage increases by assigning all years the same
/// percent increase.</summary>
///
/// <param name="firstYeart">First year of calculation.</param>
/// <param name="factor">Multiplicative increase factor.</param>
void Awinc::project( double factor, int firstYeart )
{
  const double increase = (factor - 1.0) * 100.0;
  for (int yr = firstYeart; yr <= getLastYear(); yr++) {
    theData[yr - getBaseYear()] = increase;
  }
}
