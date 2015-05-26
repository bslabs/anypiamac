// Functions for the <see cref=Pib58Ame"/> class to handle 1958 pib-pia
// conversion table.
//
// $Id: pib58pib.cpp 1.12 2011/08/08 08:45:13EDT 044579 Development  $

#include "pibtable.h"
#include "Resource.h"
#include "PiaException.h"

// <summary>The 1958 pib's.</summary>
const double Pib58Pib::pib58[] = {
  10.00, 10.48, 11.00, 11.48, 12.00, 12.48, 13.00, 13.48, 14.00, 14.48,
  15.00, 15.60, 16.20, 16.84, 17.60, 18.40, 19.24, 20.00, 20.64, 21.28,
  21.88, 22.28, 22.68, 23.08, 23.44, 23.76, 24.20, 24.60, 25.00, 25.48,
  25.92, 26.40, 26.94, 27.46, 28.00, 28.68, 29.25, 29.68, 30.36, 30.92,
  31.36, 32.00, 32.60, 33.20, 33.88, 34.50, 35.00, 35.80, 36.40, 37.08,
  37.60, 38.20, 39.12, 39.68, 40.33, 41.12, 41.76, 42.44, 43.20, 43.76,
  44.44, 44.58, 45.60
};

/// <summary>Returns 1958 conversion table pib.</summary>
///
/// <returns>1958 conversion table pib.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PIB58PIB"/> if index is out of range
/// (only in debug mode).</exception>
///
/// <param name="index">Number of pib desired.</param>
double Pib58Pib::getAt( int index )
{
#ifndef NDEBUG
  if (index < 0 || index > 62)
    throw PiaException(PIA_IDS_PIB58PIB);
#endif
  return(pib58[index]);
}
