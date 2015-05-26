// Declarations for the <see cref="BpMfbOut"/> class to manage array of mfb
// formula bend points.
//
// $Id: bpmfbout.h 1.13 2011/07/29 11:25:03EDT 044579 Development  $

#pragma once

#include "dbleann.h"
#include "avgwg.h"

/// <summary>Manages the array of mfb formula bend points.</summary>
///
/// <remarks>This class is used to store all the bend points for all years, so
/// they do not have to be recalculated for every benefit calculation.
/// </remarks>
class BpMfbOut
{
private:
  /// <summary>Array of first mfb bend points.</summary>
  DoubleAnnual bpmfb1;
  /// <summary>Array of second mfb bend points.</summary>
  DoubleAnnual bpmfb2;
  /// <summary>Array of third mfb bend points.</summary>
  DoubleAnnual bpmfb3;
public:
  BpMfbOut( int newLastYear );
  BpMfbOut( int newBaseYear, int newLastYear );
  /// <summary>Returns first bend point for specified year.</summary>
  ///
  /// <returns>First bend point for specified year.</returns>
  ///
  /// <param name="year">Year for which first bend point is desired.</param>
  double getBpmfb1( int year ) const { return(bpmfb1[year]); }
  /// <summary>Returns second bend point for specified year.</summary>
  ///
  /// <returns>Second bend point for specified year.</returns>
  ///
  /// <param name="year">Year for which second bend point is desired.</param>
  double getBpmfb2( int year ) const { return(bpmfb2[year]); }
  /// <summary>Returns third bend point for specified year.</summary>
  ///
  /// <returns>Third bend point for specified year.</returns>
  ///
  /// <param name="year">Year for which third bend point is desired.</param>
  double getBpmfb3( int year ) const { return(bpmfb3[year]); }
  void setData( const AverageWage& averageWage, int firstYear = 1979 );
  void setData( int year, double bp1, double bp2, double bp3 );
};
