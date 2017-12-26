// Functions for the <see cref="BendPia"/> class to handle PIA formula bend points.

// $Id: bendpia.cpp 1.18 2017/11/14 12:57:32EST 277133 Development  $


#include <cmath>  // for floor
#include <algorithm>
#include "bendpia.h"

using namespace std;

const double BendPia::BP1979[] = { 0.0, 180.0, 1085.0 };

/// <summary>Constructor.</summary>
///
/// <param name="newNumBend">Number of bend points in formula.</param>
///
/// <remarks>Sets first bend point to 0.0.</remarks>
BendPia::BendPia( int newNumBend)
{
	if (newNumBend > MAXBP) {
    throw PiaException("Number of bend points is too large in BendPia::BendPia.");
  }
  numBend = newNumBend;
   fill(begin(), begin() + MAXBP, 0.0);
}

/// <summary>Destroys bend point array.</summary>
BendPia::~BendPia()
{ }

/// <summary>Projects bend points, using 1979 bend points as a base.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> if average wages
/// are not initialized (only in debug mode).</exception>
///
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="fq">Average wage series.</param>
void BendPia::project(int eligYear, const AverageWage& fq)
{
#if !defined(NDEBUG)
   if (fq[1977] == 0.0) {
     throw PiaException("fq is not initialized in BendPia::project");
   }
#endif
   const double temp = fq[eligYear - 2] / fq[1977];
   theData[1] = floor(BP1979[1] * temp + 0.5);
   theData[2] = floor(BP1979[2] * temp + 0.5);
}

/// <summary>Zeroes out all values.</summary>
void BendPia::deleteContents()
{
   fill(begin(), begin() + MAXBP, 0.0);
}

/// <summary>Sets number of bend points.</summary>
///
/// <param name="newNumBend">Number of bend points in formula.</param>
void BendPia::setNumBend( int newNumBend )
{
   if (newNumBend > MAXBP) {
     throw PiaException("Number of bend points is too large in BendPia::setNumBend.");
   }
   numBend = newNumBend;
   fill(begin(), begin() + MAXBP, 0.0);
}