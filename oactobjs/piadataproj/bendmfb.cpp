// Functions to handle MFB formula bend points.
//
// $Id: bendmfb.cpp 1.18 2017/09/18 10:37:20EDT 277133 Development 277133(2017/11/14 07:33:56EST) $


#include <cmath>  // for floor
#include <algorithm>
#include "bendmfb.h"

using namespace std;

const double BendMfb::BP1979[] = { 0.0, 230.0, 332.0, 433.0 };

/// <summary>Constructor.</summary>
///
/// <remarks>Sets first bend point to 0.0.</remarks>
BendMfb::BendMfb()
{
	theData[0] = 0.0;
}

/// <summary>Destroys bend point array.</summary>
BendMfb::~BendMfb()
{ }

/// <summary>Projects bend points, using 1979 bend points as a base.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> if average wages
/// are not initialized (only in debug mode).</exception>
///
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="fq">Average wage series.</param>
void BendMfb::project(int eligYear, const AverageWage& fq)
{
#if !defined(NDEBUG)
	if (fq[1977] == 0.0)
		throw PiaException("fq is not initialized in BendMfb::project");
#endif
	const double temp = fq[eligYear - 2] / fq[1977];
	theData[1] = floor(BP1979[1] * temp + 0.5);
	theData[2] = floor(BP1979[2] * temp + 0.5);
	theData[3] = floor(BP1979[3] * temp + 0.5);
}

/// <summary>Zeroes out all values.</summary>
void BendMfb::deleteContents()
{
	fill(begin(), end(), 0.0);
}
