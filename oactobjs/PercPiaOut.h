// Declarations for the <see cref="PercPiaOut"/> class to manage array of pia
// formula percentages.
//
// $Id: PercPiaOut.h 1.7 2011/08/01 11:38:36EDT 044579 Development  $

#pragma once

#include "dbleann.h"
#include "avgwg.h"

/// <summary>Manages the array of pia formula percentages, projected for all
/// years.</summary>
///
/// <remarks>An instance of this class is used in <see cref="PiaParams"/>.
/// </remarks>
class PercPiaOut
{
private:
  /// <summary>Array of first pia percentages.</summary>
  DoubleAnnual percPia1;
  /// <summary>Array of second pia percentages.</summary>
  DoubleAnnual percPia2;
  /// <summary>Array of third pia percentages.</summary>
  DoubleAnnual percPia3;
public:
  PercPiaOut( int newLastYear );
  PercPiaOut( int newBaseYear, int newLastYear );
  /// <summary>Returns first percentage for specified year.</summary>
  ///
  /// <returns>First percentage for specified year.</returns>
  ///
  /// <param name="year">Year for which first percentage is desired.</param>
  double getPercPia1( int year ) const { return(percPia1[year]); }
  /// <summary>Returns second percentage for specified year.</summary>
  ///
  /// <returns>Second percentage for specified year.</returns>
  ///
  /// <param name="year">Year for which second percentage is desired.</param>
  double getPercPia2( int year ) const { return(percPia2[year]); }
  /// <summary>Returns third percentage for specified year.</summary>
  ///
  /// <returns>Third percentage for specified year.</returns>
  ///
  /// <param name="year">Year for which third percentage is desired.</param>
  double getPercPia3( int year ) const { return(percPia3[year]); }
  void setData( int year, double perc1, double perc2, double perc3 );
};
