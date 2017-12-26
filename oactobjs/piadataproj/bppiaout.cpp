// Functions for the <see cref="BpPiaOut"/> class to manage array of pia
// formula bend points.
//
// $Id: bppiaout.cpp 1.19 2017/10/02 14:04:25EDT 277133 Development  $

#include "bppiaout.h"
#include "bendpia.h"
#include "oactcnst.h"

/// <summary>Initializes arrays with a starting year of 1979.</summary>
///
/// <param name="newLastYear">Maximum year of projection.</param>
BpPiaOut::BpPiaOut( int newLastYear ) :
bppia1(YEAR79, newLastYear), bppia2(YEAR79, newLastYear),
bppia3(YEAR79,newLastYear), bppia4(YEAR79,newLastYear),
numBendPoints(YEAR79,newLastYear)
{
  // Default to the number in the standard PIA formula.
  numBendPoints.fill(2);
}

/// <summary>Initializes arrays with a specified starting year.</summary>
///
/// <param name="newLastYear">Maximum year of projection.</param>
/// <param name="newBaseYear">Starting year of array.</param>
BpPiaOut::BpPiaOut( int newBaseYear, int newLastYear ) :
bppia1(newBaseYear, newLastYear), bppia2(newBaseYear, newLastYear),
bppia3(newBaseYear,newLastYear), bppia4(newBaseYear,newLastYear),
numBendPoints(newBaseYear,newLastYear)
{
  // Default to the number in the standard PIA formula.
  numBendPoints.fill(2);
 }

/// <summary>Returns bend point for specified year.</summary>
///
/// <returns>Bend point number index for specified year.</returns>
///
/// <param name="year">Year for which bend point is desired.</param>
/// <param name="index">Index of bend point desired.</param>
double BpPiaOut::getBppia( int year, int index ) const
{ 
  switch (index) {
    case 1: return bppia1[year];
    case 2: return bppia2[year];
    case 3: return bppia3[year];
    case 4: return bppia4[year];
    default: return 0.0;
  }
}

/// <summary>Sets data for all years based on the wage-indexed 1979 formula
/// starting in the year of the firstYear argument,
/// using the specified average wages for projection.</summary>
///
/// <param name="averageWage">The average wage series.</param>
/// <param name="firstYear">The first year to project.</param>
void BpPiaOut::set1979Data( const AverageWage& averageWage, int firstYear )
{
  BendPia bendpts(3);  // temporary bendpoints
  for (int year = firstYear; year <= bppia1.getLastYear(); year++) {
    numBendPoints[year] = 2;
    bendpts.project(year, averageWage);
    bppia1[year] = bendpts[1];
    bppia2[year] = bendpts[2];
  }
}

/// <summary>Sets data for all years beginning with the firstYear argument,
/// based on the bendpoints specified in the baseYear argument, and
/// using the specified average wages for projection.</summary>
///
/// <param name="averageWage">The average wage series.</param>
/// <param name="firstYear">The first year to project.</param>
/// <param name="baseYear">The year of bend points to project from.</param>
/// <param name="numBp">The number of bend points in the formula.</param>
void BpPiaOut::setIndexedData( const AverageWage& averageWage, int firstYear, int baseYear, int numBp )
{
  double wageFactor;
  for (int year = firstYear; year <= bppia1.getLastYear(); year++) {
    numBendPoints[year] = numBp;
	  wageFactor = averageWage[year - 2] / averageWage[baseYear - 2];
    if (numBp >= 1) {
	    bppia1[year] = floor(bppia1[baseYear] * wageFactor + 0.5);
    }
    if (numBp >= 2) {
	    bppia2[year] = floor(bppia2[baseYear] * wageFactor + 0.5);
    }
    if (numBp >= 3) {
	    bppia3[year] = floor(bppia3[baseYear] * wageFactor + 0.5);
    }
    if (numBp >= 4) {
	    bppia4[year] = floor(bppia4[baseYear] * wageFactor + 0.5);
    }
  }
}

/// <summary>Sets data for one year using specified bendpoint values.
/// </summary>
///
/// <param name="year">Year for which data is set.</param>
/// <param name="numBp">Number of bendpoints in the formula.</param>
/// <param name="bp">Array of values of bendpoints.</param>
void BpPiaOut::setSingleYearData( int year, int numBp, double bp[] )
{
  numBendPoints[year] = numBp;
  if (numBp >= 1) {
	  bppia1[year] = bp[0];
  }
  if (numBp >= 2) {
	  bppia2[year] = bp[1];
  }
  if (numBp >= 3) {
	  bppia3[year] = bp[2];
  }
  if (numBp >= 4) {
	  bppia4[year] = bp[3];
  }
}
