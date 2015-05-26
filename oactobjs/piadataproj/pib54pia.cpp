// Functions for the <see cref=Pib54Pia"/> class to handle 1954 pib-pia
// conversion table.
//
// $Id: pib54pia.cpp 1.10 2011/08/08 08:45:13EDT 044579 Development  $

#include "pibtable.h"
#include "Resource.h"
#include "PiaException.h"

// <summary>The 1954 pia's.</summary>
const double Pib54Pia::pia54[] = {
  64.70, 64.70, 64.90, 65.10, 65.10, 65.30, 65.50, 65.50, 65.90, 65.90,
  66.10, 66.30, 66.30, 66.50, 66.70, 66.70, 67.10, 67.10, 67.30, 67.50,
  67.50, 67.70, 67.90, 67.90, 68.30, 68.30, 68.50, 68.70, 68.70, 68.90,
  69.10, 69.10, 69.50, 69.50, 69.70, 69.90, 69.90, 70.10, 70.30, 70.30,
  70.70, 70.70, 70.90, 71.10, 71.10, 71.30, 71.50, 71.50, 71.90, 71.90,
  72.10, 72.30, 72.30, 72.50, 72.70, 72.70, 73.10, 73.10, 73.30, 73.50,
  73.50, 73.70, 73.90, 73.90, 74.30, 74.30, 74.50, 74.70, 74.70, 74.90,
  75.10, 75.10, 75.50, 75.50, 75.70, 75.90, 75.90, 76.10, 76.30, 76.30,
  76.70, 76.70, 76.90, 77.10, 77.10, 77.30, 77.50, 77.50, 77.90, 77.90,
  78.10, 78.30, 78.30, 78.50, 78.70, 78.70, 79.10, 79.10, 79.30, 79.50,
  79.50, 79.70, 79.90, 79.90, 80.30, 80.30, 80.50, 80.70, 80.70, 80.90,
  81.10, 81.10, 81.50, 81.50, 81.70, 81.90, 81.90, 82.10, 82.30, 82.30,
  82.70, 82.70, 82.90, 83.10, 83.10, 83.30, 83.50, 83.50, 83.90, 83.90,
  84.10, 84.30, 84.30, 84.50, 84.70, 84.70, 85.10, 85.10, 85.30, 85.50,
  85.50, 85.70, 85.90, 85.90, 86.30, 86.30, 86.50, 86.70, 86.70, 86.90,
  87.10, 87.10, 87.50, 87.50, 87.70, 87.90, 88.50
};

/// <summary>Returns 1954 conversion table pia.</summary>
///
/// <returns>1954 conversion table pia.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PIB54PIA"/> if index is out
/// of range (only in debug mode).</exception>
///
/// <param name="index">Number of pia desired.</param>
double Pib54Pia::getAt( int index )
{
#ifndef NDEBUG
  if (index < 0 || index > 156)
    throw PiaException(PIA_IDS_PIB54PIA);
#endif
  return(pia54[index]);
}
