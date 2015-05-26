// Functions needed for <see cref="DoubleAnnual"/> class to manage an array
// of annual doubles.

// $Id: dbleann.cpp 1.38 2011/08/11 15:21:16EDT 044579 Development  $

#include <algorithm>
#if !defined(SHORTFILENAMES)
#include "dbleann.h"
#else
#include "dbleannh"
#endif

using namespace std;

/// <summary>Constructor.</summary>
///
/// <remarks>Sets the base year of data, and initializes the size of the data
/// vector to the span of years.</remarks>
///
/// <param name="newBaseYear">New base year.</param>
/// <param name="newLastYear">New last year.</param>
DoubleAnnual::DoubleAnnual( int newBaseYear, int newLastYear ) :
theData(newLastYear - newBaseYear + 1), baseYear(newBaseYear)
{ }

/// <summary>Sets data for specified years from another
/// <see cref="DoubleAnnual"/>.</summary>
///
/// <param name="doubleAnnual">Array of new values of data.</param>
/// <param name="firstYeart">First year of data to set.</param>
/// <param name="lastYeart">Last year of data to set.</param>
void DoubleAnnual::assign( const DoubleAnnual& doubleAnnual, int firstYeart,
int lastYeart )
{
  int year1 = max(baseYear, doubleAnnual.baseYear);
  year1 = max(year1, firstYeart);
  int year2 = min(getLastYear(), doubleAnnual.getLastYear());
  year2 = min(year2, lastYeart);
  copy(doubleAnnual.theData.begin() + (year1 - doubleAnnual.baseYear),
    doubleAnnual.theData.begin() + (year2 + 1 - doubleAnnual.baseYear),
    theData.begin() + (year1 - baseYear));
}

/// <summary>Sets data for specified years from one
/// value that is constant for all years.</summary>
///
/// <param name="newData">New value of data to be used for all specified
/// years.</param>
/// <param name="firstYeart">First year of data to set.</param>
/// <param name="lastYeart">Last year of data to set.</param>
void DoubleAnnual::assign( double newData, int firstYeart, int lastYeart )
{
  const int year1 = max(baseYear, firstYeart);
  const int year2 = min(getLastYear(), lastYeart);
  std::fill(theData.begin() + (year1 - baseYear),
    theData.begin() + (year2 + 1- baseYear), newData);
}
