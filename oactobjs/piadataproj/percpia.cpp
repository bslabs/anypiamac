// $Id: percpia.cpp 1.17 2010/09/02 11:39:43EDT 044579 Development  $
//
// Functions to handle PIA formula percentages.

#include <algorithm>

using namespace std;

/// <summary>Sets first PIA formula percentage for windfall
///   elimination provision.</summary>
///
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="benefitDate">Year of benefit.</param>
/// <param name="specMinTot">Years of coverage.</param>
void PercPia::setWindfallPerc( int eligYear, int benefitDate, int specMinTot )
{
   if (eligYear < 1985 || specMinTot >= WINDFALL_YEARS) {
      theData[0] = PERC[0];
      return;
   }
   // set a temporary percentage
   double rv = (eligYear > 1989) ? WINDFALL_PERC :
      PERC[0] - 0.10 * static_cast<double>(eligYear - 1985);
   // check for years of coverage guarantee (changes beginning with
   // benefits paid in 1989)
   const double annualPct = (benefitDate >= 1989) ? 0.05 : 0.10;
   const double totalPct =
      annualPct * static_cast<double>(WINDFALL_YEARS - specMinTot);
   if (PERC[0] - totalPct > rv)
      rv = PERC[0] - totalPct;
   theData[0] = rv;
}
