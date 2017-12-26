// Functions for the <see cref="BpMfbOut"/> class to manage array of mfb
// formula bend points.
//
// $Id: bpmfbout.cpp 1.18 2017/09/18 10:37:21EDT 277133 Development  $

#include "bpmfbout.h"
#include "bendmfb.h"
#include "oactcnst.h"

/// <summary>Initializes arrays with a starting year of 1979.</summary>
///
/// <param name="newLastYear">Maximum year of projection.</param>
BpMfbOut::BpMfbOut( int newLastYear ) :
bpmfb1(YEAR79, newLastYear), bpmfb2(YEAR79, newLastYear),
bpmfb3(YEAR79, newLastYear)
{ }

/// <summary>Initializes arrays with a specified starting year.</summary>
///
/// <param name="newLastYear">Maximum year of projection.</param>
/// <param name="newBaseYear">Starting year of array.</param>
BpMfbOut::BpMfbOut( int newBaseYear, int newLastYear ) :
bpmfb1(newBaseYear, newLastYear), bpmfb2(newBaseYear, newLastYear),
bpmfb3(newBaseYear, newLastYear)
{ }

/// <summary>Sets data for all years beginning with the firstYear argument,
/// using the specified average wages for projection.</summary>
///
/// <param name="averageWage">Average wage series used to project
/// bend points, starting in specified year.</param>
/// <param name="firstYear">Starting year of projection.</param>
void BpMfbOut::setData( const AverageWage& averageWage, int firstYear )
{
  BendMfb bendpts;  // temporary bendpoints
  for (int year = firstYear; year <= bpmfb1.getLastYear(); year++) {
    bendpts.project(year, averageWage);
    bpmfb1[year] = bendpts[1];
    bpmfb2[year] = bendpts[2];
    bpmfb3[year] = bendpts[3];
  }
}

/// <summary>Sets data for one year using three specified bend point values.
/// </summary>
///
/// <param name="year">Year for which data is set.</param>
/// <param name="bp1">Value of first bend point.</param>
/// <param name="bp2">Value of second bend point.</param>
/// <param name="bp3">Value of third bend point.</param>
void BpMfbOut::setData( int year, double bp1, double bp2, double bp3 )
{
  bpmfb1[year] = bp1;
  bpmfb2[year] = bp2;
  bpmfb3[year] = bp3;
}
