// Declarations for the <see cref="PercPiaOut"/> class to manage array of pia
// formula percentages.
//
// $Id: PercPiaOut.h 1.9 2017/10/12 12:48:33EDT 277133 Development 277133(2017/11/14 10:37:42EST) $

#pragma once

#include "dbleann.h"
#include "intann.h"
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
  /// <summary>Array of fourth pia percentages.</summary>
  DoubleAnnual percPia4;
  /// <summary>Array of fifth pia percentages.</summary>
  DoubleAnnual percPia5;
  /// <summary>Array of number of percentages in each year.</summary>
  IntAnnual numPercs;
public:
  PercPiaOut( int newLastYear );
  PercPiaOut( int newBaseYear, int newLastYear );
  double getPercPia( int year, int index ) const;
  /// <summary>Returns number of percentages in the PIA formula for a year.
  /// </summary>
  ///
  /// <returns>Number of percentages in PIA formula for a year.</returns>
  ///
  /// <param name="year">Year of formula.</param>
  int getNumPercs( int year ) const { return numPercs[year]; }
  void setData( int year, int numPerc, double perc[] );
};
