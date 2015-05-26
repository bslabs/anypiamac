// Functions for the <see cref="PercPiaOut"/> class to manage array of pia
// formula percentages.
//
// $Id: PercPiaOut.cpp 1.7 2011/08/01 11:38:34EDT 044579 Development  $

#include "PercPiaOut.h"
#include "oactcnst.h"

using namespace std;

/// <summary>Initializes arrays starting in 1979.</summary>
///
/// <param name="newLastYear">Maximum year of projection.</param>
PercPiaOut::PercPiaOut( int newLastYear ) :
percPia1(YEAR79, newLastYear), percPia2(YEAR79, newLastYear),
percPia3(YEAR79, newLastYear)
{ }

/// <summary>Initializes arrays with specified start year.</summary>
///
/// <param name="newLastYear">Maximum year of projection.</param>
/// <param name="newBaseYear">Starting year of array.</param>
PercPiaOut::PercPiaOut( int newBaseYear, int newLastYear ) :
percPia1(newBaseYear, newLastYear), percPia2(newBaseYear, newLastYear),
percPia3(newBaseYear, newLastYear)
{ }

/// <summary>Sets data for one year.</summary>
///
/// <param name="year">Year for which data is set.</param>
/// <param name="perc1">Value of first percentage.</param>
/// <param name="perc2">Value of second percentage.</param>
/// <param name="perc3">Value of third percentage.</param>
void PercPiaOut::setData( int year, double perc1, double perc2, double perc3 )
{
  percPia1[year] = perc1;
  percPia2[year] = perc2;
  percPia3[year] = perc3;
}
