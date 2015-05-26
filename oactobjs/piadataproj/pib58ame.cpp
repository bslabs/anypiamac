// Functions for the <see cref=Pib58Ame"/> class to handle 1958 pib-ame
// conversion table.
//
// $Id: pib58ame.cpp 1.12 2011/08/08 08:45:14EDT 044579 Development  $

#include "pibtable.h"
#include "Resource.h"
#include "PiaException.h"

// <summary>The 1958 ame's.</summary>
const double Pib58Ame::ame58[] = {
  54.0, 56.0, 58.0, 60.0, 61.0, 63.0, 65.0, 67.0, 69.0, 70.0, 72.0, 74.0,
  76.0, 78.0, 80.0, 81.0, 83.0, 85.0, 87.0, 89.0, 90.0, 92.0, 94.0, 96.0,
  97.0, 99.0, 101.0, 102.0, 104.0, 106.0, 107.0, 109.0, 113.0, 118.0, 122.0,
  127.0, 132.0, 136.0, 141.0, 146.0, 150.0, 155.0, 160.0, 164.0, 169.0,
  174.0, 178.0, 183.0, 188.0, 193.0, 197.0, 202.0, 207.0, 211.0, 216.0,
  221.0, 225.0, 230.0, 235.0, 239.0, 244.0, 249.0, 250.0
};

/// <summary>Returns 1958 conversion table ame.</summary>
///
/// <returns>1958 conversion table ame.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PIB58AME"/> if index is out of range
/// (only in debug mode).</exception>
///
/// <param name="index">Number of ame desired.</param>
double Pib58Ame::getAt( int index )
{
#ifndef NDEBUG
  if (index < 0 || index > 62)
    throw PiaException(PIA_IDS_PIB58AME);
#endif
  return(ame58[index]);
}
