// Declarations for the <see cref="LawChangeDECLINEPERC"/> class to manage
// parameters for a proposal to decrease the benefit formula percentages by
// various rates.
//
// $Id: LawChangeDECLINEPERC.h 1.9 2011/08/01 10:06:08EDT 044579 Development  $

#pragma once

#include "LawChange.h"
#include "dbleann.h"

/// <summary>Parameters for a proposal to decrease the benefit formula
/// percentages by a constant rate.</summary>
///
/// <remarks>If the indicator is greater than zero, it indicates the number
/// of intervals of declining benefit formula percents. There are three
/// additional data fields compared to the base class, for the initial period
/// of decline. If there are multiple intervals of decline, there are four
/// additional data fields for each interval. The rates of decline can vary
/// for each benefit formula percentage within each interval, and between
/// intervals.</remarks>
class LawChangeDECLINEPERC : public LawChange
{
public:
  /// <summary>Array sizes.</summary>
  enum DimLimits {
    /// <summary>Maximum number of declining formula percentage intervals.
    /// </summary>
    MAXDECLINEPERC = 5
  };
private:
  /// <summary>Percentage decline in benefit formula percents (3 percentages
  /// for up to <see cref="DimLimits::MAXDECLINEPERC"/> intervals).</summary>
  double factorDeclinePerc[3][MAXDECLINEPERC];
  /// <summary>Vectors of benefit formula percentages for PIA formula.
  /// </summary>
  DoubleAnnual* percpa[4];
  /// <summary>Years benefit formula percentage declines are effective.
  /// </summary>
  int yearDeclinePerc[MAXDECLINEPERC];
public:
  LawChangeDECLINEPERC();
  ~LawChangeDECLINEPERC();
  /// <summary>Returns specified benefit formula percentage for specified
  /// year.</summary>
  ///
  /// <returns>Benefit formula percentage for specified year.</returns>
  ///
  /// <param name="year">Year of desired benefit formula percentage.</param>
  /// <param name="percNum">Percentage number (0, 1, or 2 for 3-part formula;
  /// 0, 1, 2, or 3 for 4-part formula).</param>
  double getAltPercPia( int year, int percNum ) const
  { if (percNum >= 0 && percNum < 4) return((*percpa[percNum])[year]);
    else return(0.0); }
  /// <summary>Returns percentage decline in specified benefit formula
  /// percent for specified year.</summary>
  ///
  /// <returns>Percentage decline in benefit formula percent.</returns>
  ///
  /// <param name="yearNum">Number of declining percentage interval (0 to
  /// <see cref="DimLimits::MAXDECLINEPERC"/> - 1).</param>
  /// <param name="percNum">Percentage number (0, 1, or 2 for 3-part
  /// formula).</param>
  double getFactor( int yearNum, int percNum ) const
  { return factorDeclinePerc[percNum][yearNum]; }
  /// <summary>Returns year that declining benefit formula percentage is
  /// effective.</summary>
  ///
  /// <returns>Year that declining benefit formula percentage is
  /// effective.</returns>
  ///
  /// <param name="yearNum">Number of declining benefit formula percentage
  /// interval (0 to <see cref="DimLimits::MAXDECLINEPERC"/> - 1).</param>
  int getYear( int yearNum ) const { return yearDeclinePerc[yearNum]; }
  void percPiaCal();
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets percentage decline in benefit formula percent.</summary>
  ///
  /// <param name="yearNum">Number of declining percentage interval (0 to
  /// <see cref="DimLimits::MAXDECLINEPERC"/> - 1).</param>
  /// <param name="percNum">Percentage number (0, 1, or 2 for 3-part
  /// formula).</param>
  /// <param name="newFactor">New percentage decline.</param>
  void setFactor( int yearNum, int percNum, double newFactor )
  { factorDeclinePerc[percNum][yearNum] = newFactor; }
  /// <summary>Sets year that declining benefit formula percentage is
  /// effective.</summary>
  ///
  /// <param name="yearNum">Number of declining benefit formula percentage
  /// interval (0 to <see cref="DimLimits::MAXDECLINEPERC"/> - 1).</param>
  /// <param name="year">New year of declining benefit formula percentage.
  /// </param>
  void setYear( int yearNum, int year ) { yearDeclinePerc[yearNum] = year; }
  void write( std::ostream& out ) const;
};
