// Functions that return strings with (or without) embedded
// commas in any type of number (floating or integral).
//
// $Id: comma.cpp 1.15 2011/08/11 15:21:17EDT 044579 Development  $

#include "comma.h"

using namespace std;

/// <summary>Inserts commas into a formatted string representing a number.
/// </summary>
///
/// <param name="start">String without commas, to be returned with commas.
/// </param>
/// <param name="dec">Number of decimal places.</param>
/// <param name="point">Indicator for decimal point.</param>
/// <param name="sign">Indicator for negative sign.</param>
void pcomma( std::string& start, int dec, int point, int sign )
{
  const int len = static_cast<int>(start.length());
  // number of commas or blanks to insert
  const int ncomma = (len - sign - dec - point - 1) / 3;
  if (ncomma > 0) {
    // insert commas
    for (int i1 = 1; i1 <= ncomma; i1++) {
      start.insert(len - 3 * i1 - dec - point, ",");
    }
  }
}

/// <summary>Inserts blanks or zeroes on the left of a formatted string
/// representing a number.</summary>
///
/// <param name="start">String without blanks, to be returned with
///  blanks.</param>
/// <param name="width">Minimum width of field.</param>
/// <param name="zeroFill">True if should zero fill on left.</param>
void pblank( std::string& start, int width, bool zeroFill )
{
  const char *fillchar = zeroFill ? "0" : " ";
  const int len = static_cast<int>(start.length());
  // number of blanks needed
  const int nblank = width - len;
  if (nblank > 0) {
    for (int i1 = 0; i1 < nblank; i1++)
      start.insert(i1, fillchar);
  }
}
