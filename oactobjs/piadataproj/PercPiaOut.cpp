// Functions for the <see cref="PercPiaOut"/> class to manage array of pia
// formula percentages.
//
// $Id: PercPiaOut.cpp 1.9 2017/11/14 12:57:32EST 277133 Development  $

#include "PercPiaOut.h"
#include "oactcnst.h"

using namespace std;

/// <summary>Initializes arrays starting in 1979.</summary>
///
/// <param name="newLastYear">Maximum year of projection.</param>
PercPiaOut::PercPiaOut( int newLastYear ) :
percPia1(YEAR79, newLastYear), percPia2(YEAR79, newLastYear),
percPia3(YEAR79, newLastYear), percPia4(YEAR79, newLastYear),
percPia5(YEAR79, newLastYear), numPercs(YEAR79, newLastYear)
{ 
  // Default to the number of percentages in the standard PIA formula. 
  numPercs.fill(3);
}

/// <summary>Initializes arrays with specified start year.</summary>
///
/// <param name="newLastYear">Maximum year of projection.</param>
/// <param name="newBaseYear">Starting year of array.</param>
PercPiaOut::PercPiaOut( int newBaseYear, int newLastYear ) :
percPia1(newBaseYear, newLastYear), percPia2(newBaseYear, newLastYear),
percPia3(newBaseYear, newLastYear), percPia4(newBaseYear, newLastYear),
percPia5(newBaseYear, newLastYear), numPercs(newBaseYear, newLastYear)
{ 
  // Default to the number of percentages in the standard PIA formula. 
  numPercs.fill(3);
}

/// <summary>Returns an indexed percentage for specified year.</summary>
///
/// <returns>Indexed percentage for specified year.</returns>
///
/// <param name="year">Year for which first percentage is desired.</param>
/// <param name="index">Index of desired percentage.</param>
double PercPiaOut::getPercPia( int year, int index ) const
{
  switch (index) {
    case 1: return percPia1[year];
    case 2: return percPia2[year];
    case 3: return percPia3[year];
    case 4: return percPia4[year];
    case 5: return percPia5[year];
    default: return 0.0;
  }
}

/// <summary>Sets data for one year.</summary>
///
/// <param name="year">Year for which data is set.</param>
/// <param name="numPerc">Number of percentages in the formula.</param>
/// <param name="perc">Array of percentages.</param>
void PercPiaOut::setData( int year, int numPerc, double perc[] )
{
  numPercs[year] = numPerc;
  if (numPerc >= 1) {
    percPia1[year] = perc[0];
  }
  if (numPerc >= 2) {
    percPia2[year] = perc[1];
  }
  if (numPerc >= 3) {
    percPia3[year] = perc[2];
  }
  if (numPerc >= 4) {
    percPia4[year] = perc[3];
  }
  if (numPerc >= 5) {
    percPia5[year] = perc[4];
  }
}
