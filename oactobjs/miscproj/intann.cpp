// Functions for the <see cref="IntAnnual"/> class to manage an array of
// annual integers.
//
// $Id: intann.cpp 1.35 2011/08/11 16:23:34EDT 044579 Development  $

#include <algorithm>
#if !defined(SHORTFILENAMES)
#include "intann.h"
#else
#include "intannh"
#endif

using namespace std;

/// <summary>Constructor.</summary>
///
/// <remarks>Sets the base year of data, and initializes the
/// size of the data vector to the span of years.</remarks>
///
/// <param name="newBaseYear">New base year.</param>
/// <param name="newLastYear">New last year.</param>
IntAnnual::IntAnnual( int newBaseYear, int newLastYear ) :
theData(newLastYear - newBaseYear + 1), baseYear(newBaseYear)
{ }

/// <summary>Sets data for specified years from another
/// <see cref="IntAnnual"/>.</summary>
///
/// <param name="intAnnual">Array of new values of data.</param>
/// <param name="firstYear">First year of data to set.</param>
/// <param name="lastYear">Last year of data to set.</param>
void IntAnnual::assign( const IntAnnual& intAnnual, int firstYear,
int lastYear )
{
  int year1 = max(baseYear, intAnnual.baseYear);
  year1 = max(year1, firstYear);
  int year2 = min(getLastYear(), intAnnual.getLastYear());
  year2 = min(year2, lastYear);
  copy(intAnnual.theData.begin() + (year1 - intAnnual.baseYear),
    intAnnual.theData.begin() + (year2 + 1 - intAnnual.baseYear),
    theData.begin() + (year1 - baseYear));
}

/// <summary>Sets data for specified years from one
/// value that is constant for all years.</summary>
///
/// <param name="newData">New value of data to be used for all specified
/// years.</param>
/// <param name="firstYear">First year of data to set.</param>
/// <param name="lastYear">Last year of data to set.</param>
void IntAnnual::assign( int newData, int firstYear, int lastYear )
{
  const int year1 = max(baseYear, firstYear);
  const int year2 = min(getLastYear(), lastYear);
  std::fill(theData.begin() + (year1 - baseYear),
    theData.begin() + (year2 + 1 - baseYear), newData);
}
