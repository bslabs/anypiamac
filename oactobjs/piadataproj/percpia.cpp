// Functions to handle PIA formula percentages.
//
// $Id: percpia.cpp 1.20 2017/10/12 13:06:33EDT 277133 Development 277133(2017/11/14 07:34:05EST) $

#include <algorithm>
#include "percpia.h"
#include "PiaException.h"

using namespace std;

// <summary>The three percentages for the regular benefit formula.</summary>
const double PercPia::PERC[] = { 0.90, 0.32, 0.15 };
// <summary>The first percentage under the windfall elimination provision.
// </summary>
const double PercPia::WINDFALL_PERC = 0.40;

/// <summary>Initializes size of array and all values.</summary>
///
/// <remarks>It does that by calling the reset function.</remarks>
PercPia::PercPia()
{
	reset();
}

/// <summary>Destroys percentages array</summary>
PercPia::~PercPia()
{ }

/// <summary>Resets percentages to original 1977 amendments values.</summary>
void PercPia::reset()
{
  numPerc = BASEPERC;
	copy(PERC, PERC + BASEPERC, theData);
  fill(begin() + BASEPERC, begin() + MAXPERC, 0.0);
}

/// <summary>Sets first PIA formula percentage for windfall
///   elimination provision.</summary>
///
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="benefitDate">Year of benefit.</param>
/// <param name="specMinTot">Years of coverage.</param>
void PercPia::setWindfallPerc(int eligYear, int benefitDate, int specMinTot)
{
	if (eligYear < 1985 || specMinTot >= WINDFALL_YEARS) {
		return;
	}
	// set a temporary percentage
	double rv = (eligYear > 1989) ? (theData[0] - 0.5) :
		theData[0] - 0.10 * static_cast<double>(eligYear - 1985);
	// check for years of coverage guarantee (changes beginning with
	// benefits paid in 1989)
	const double annualPct = (benefitDate >= 1989) ? 0.05 : 0.10;
	const double totalPct =
		annualPct * static_cast<double>(WINDFALL_YEARS - specMinTot);
	if (theData[0] - totalPct > rv) {
		rv = theData[0] - totalPct;
  }
  if (rv < 0.0) {
    rv = 0.0;
  }
	theData[0] = rv;
}

/// <summary>Sets number of percentages.</summary>
///
/// <param name="newNumPerc">New number of percentages in formula.</param>
///
/// <remarks>Zeroes out all values in formula, so these have to be set after calling.</remarks>
void PercPia::setNumPerc( int newNumPerc )
{
   if (newNumPerc > MAXPERC) {
     throw PiaException("Number of percentages is too large in PercPia::setNumPerc.");
   }
   numPerc = newNumPerc;
   fill(begin(), begin() + MAXPERC, 0.0);
}