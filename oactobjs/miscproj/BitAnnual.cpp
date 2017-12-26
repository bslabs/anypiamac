// Functions for the <see cref="BitAnnual"/> class to manage an array of
// annual bits.
//
// $Id: BitAnnual.cpp 1.18 2017/08/29 09:03:51EDT 277133 Development  $

#include <algorithm>
#include "BitAnnual.h"

using namespace std;

// GENERAL NOTE:
// When adding functions to this class, or changing the implementation
// of existing functions, be careful not to use square brackets ( [] )
// to access 'data' unless you're really sure of what you're doing.
// This is because there are different implementations of this class
// depending on whether you're using NT or Linux. Instead, use the
// setBit() and getBit() functions.

/// <summary>Deletes allocated memory.</summary>
BitAnnual::~BitAnnual()
{ }

/// <summary>Sets data from another <see cref="BitAnnual"/> for specified
/// years.</summary>
///
/// <param name="bitAnnual">Array of new values of data.</param>
/// <param name="firstYeart">First year of data to set.</param>
/// <param name="lastYeart">Last year of data to set.</param>
void BitAnnual::assign( const BitAnnual& bitAnnual, int firstYeart,
int lastYeart )
{
  int year1 = max(baseYear, bitAnnual.baseYear);
  year1 = max(year1, firstYeart);
  int year2 = min(lastYear, bitAnnual.lastYear);
  year2 = min(year2, lastYeart);
  for (int year = year1; year <= year2; year++)
    setBit(year, bitAnnual.getBit(year));
}
