// Functions to handle MFB formula percentages.
//
// $Id: percmfb.cpp 1.13 2017/09/18 10:37:22EDT 277133 Development 277133(2017/11/14 07:34:04EST) $

#include <algorithm>
#include "percmfb.h"

using namespace std;

const double PercMfb::PERC[] = { 1.5, 2.72, 1.34, 1.75 };

/// <summary>Initializes size of array and all values.</summary>
///
/// <remarks>It does that by calling the reset function.</remarks>
PercMfb::PercMfb()
{
	reset();
}

/// <summary>Destroys percentages array.</summary>
PercMfb::~PercMfb()
{ }

/// <summary>Resets percentages to original values.</summary>
void PercMfb::reset()
{
	copy(PERC, PERC + MAXPERC, theData);
}
