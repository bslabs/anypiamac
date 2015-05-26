// Declarations for the <see cref="BpPiaOut"/> class to manage array of pia
// formula bend points.
//
// $Id: bppiaout.h 1.13 2011/07/29 11:25:03EDT 044579 Development  $

#pragma once

#include "dbleann.h"
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
public:
  BpPiaOut( int newLastYear );
  BpPiaOut( int newBaseYear, int newLastYear );
  /// <summary>Returns first bend point for specified year.</summary>
  ///
  /// <returns>First bend point for specified year.</returns>
  ///
  /// <param name="year">Year for which first bend point is desired.</param>
  double getBppia1( int year ) const { return(bppia1[year]); }
  /// <summary>Returns second bend point for specified year.</summary>
  ///
  /// <returns>Second bend point for specified year.</returns>
  ///
  /// <param name="year">Year for which second bend point is desired.</param>
  double getBppia2( int year ) const { return(bppia2[year]); }
  void setData( const AverageWage& averageWage, int firstYear = 1979 );
  void setData( int year, double bp1, double bp2 );
};
