// Declarations for the <see cref="BpPiaOut"/> class to manage array of pia
// formula bend points.
//
// $Id: bppiaout.h 1.17 2017/11/14 12:56:10EST 277133 Development  $

#pragma once

#include "dbleann.h"
#include "intann.h"
#include "avgwg.h"

/// <summary>Manages the array of pia formula bend points.</summary>
///
/// <remarks>This class is used to store all the bend points for all years, so
/// they do not have to be recalculated for every benefit calculation.
/// </remarks>
class BpPiaOut
{
private:
  /// <summary>Array of first pia bend points.</summary>
  DoubleAnnual bppia1;
  /// <summary>Array of second bend points.</summary>
  DoubleAnnual bppia2;
  /// <summary>Array of third bend points.</summary>
  DoubleAnnual bppia3;
  /// <summary>Array of fourth bend points.</summary>
  DoubleAnnual bppia4;
  /// <summary>Array of number of bend points in each year.</summary>
  IntAnnual numBendPoints;
public:
  BpPiaOut( int newLastYear );
  BpPiaOut( int newBaseYear, int newLastYear );
  double getBppia( int year, int index ) const;
	/// <summary>Returns number of bend points.</summary>
	///
	/// <returns>Number of bend points.</returns>
	///
	/// <param name="year">Year of bendpoint formula.</param>
  int getNumBend( int year ) const { return numBendPoints[year]; };
  void set1979Data( const AverageWage& averageWage, int firstYear = 1979 );
  void setIndexedData( const AverageWage& averageWage, int firstYear, int baseYear, int numBp = 2 );
  void setSingleYearData( int year, int numBp, double bp[]);
};
