// Functions for the <see cref="BpPiaOut"/> class to manage array of pia
// formula bend points.
//
// $Id: bppiaout.cpp 1.16 2011/07/29 11:25:02EDT 044579 Development  $

#include "bppiaout.h"
#include "BendPoints.h"
#include "oactcnst.h"

/// <summary>Initializes arrays with a starting year of 1979.</summary>
///
/// <param name="newLastYear">Maximum year of projection.</param>
BpPiaOut::BpPiaOut( int newLastYear ) :
bppia1(YEAR79, newLastYear), bppia2(YEAR79, newLastYear)
{ }

/// <summary>Initializes arrays with a specified starting year.</summary>
///
/// <param name="newLastYear">Maximum year of projection.</param>
/// <param name="newBaseYear">Starting year of array.</param>
BpPiaOut::BpPiaOut( int newBaseYear, int newLastYear ) :
bppia1(newBaseYear, newLastYear), bppia2(newBaseYear, newLastYear)
{ }

/// <summary>Sets data for all years beginning with the firstYear argument,
/// using the specified average wages for projection.</summary>
///
/// <param name="averageWage">The average wage series.</param>
/// <param name="firstYear">The first year to project.</param>
void BpPiaOut::setData( const AverageWage& averageWage, int firstYear )
{
  BendPia bendpts;  // temporary bendpoints
  for (int year = firstYear; year <= bppia1.getLastYear(); year++) {
    BendPoints::projectPia(bendpts, year, averageWage);
    bppia1[year] = bendpts[1];
    bppia2[year] = bendpts[2];
  }
}

/// <summary>Sets data for one year using two specified bend point values.
/// </summary>
///
/// <param name="year">Year for which data is set.</param>
/// <param name="bp1">Value of first bend point.</param>
/// <param name="bp2">Value of second bend point.</param>
void BpPiaOut::setData( int year, double bp1, double bp2 )
{
  bppia1[year] = bp1;
  bppia2[year] = bp2;
}
