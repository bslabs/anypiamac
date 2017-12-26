// Declarations for the <see cref="LawChangeNEWFORMULA"/> class to manage
// parameters for a proposal to change the pia formula bend points and/or
// percentages.
//
// $Id: LawChangeNEWFORMULA.h 1.12 2017/11/14 12:56:11EST 277133 Development  $

#pragma once

#include "LawChange.h"
#include "dbleann.h"

/// <summary>Parameters for a proposal to change the pia formula bend points
/// and/or percentages.</summary>
///
/// <remarks>There are two additional data fields compared to the base class.
///
/// Currently only an indicator of 1 is allowed. Then a new benefit formula of
/// up to 5 percentages and 4 bend points is used for each year in the specified
/// period.  The number of bendpoints in the new formula is specified
/// on the first line of additional information, then each year's formula
/// is on each line after that.  For years after the last specified formula,
/// the bendpoints are indexed from the last values using the AWI and the 
/// percentages are held constant at the last specified values.</remarks>
class LawChangeNEWFORMULA : public LawChange
{
private:
  /// <summary>Alternative PIA formula bend points.</summary>
  DoubleAnnual* bendpAlt[4];
  /// <summary>Alternative benefit formula percentages for PIA formula.
  /// </summary>
  DoubleAnnual* percpAlt[5];
  /// <summary>Number of alternative PIA formula bend points.</summary>
  int numBp;
public:
  LawChangeNEWFORMULA();
  ~LawChangeNEWFORMULA();
  /// <summary>Returns specified alternate bend point for specified year.
  /// </summary>
  ///
  /// <returns>Specified alternate bend point for specified year.</returns>
  ///
  /// <param name="year">Year for which bend point is desired.</param>
  /// <param name="bendNum">Bend point number (1 to n-1 for n-part formula).</param>
  double getAltBendPia( int year, int bendNum ) const
  { if (bendNum > 0 && bendNum < 5) return((*bendpAlt[bendNum - 1])[year]);
    else return(0.0); }
  /// <summary>Returns specified benefit formula percentage for specified
  /// year.</summary>
  ///
  /// <returns>Specified benefit formula percentage for specified year.
  /// </returns>
  ///
  /// <param name="year">Year of desired benefit formula percentage.</param>
  /// <param name="percNum">Percentage number (0 to n-1 for n-part formula).</param>
  double getAltPercPia( int year, int percNum ) const
  { if (percNum >= 0 && percNum < 5) return((*percpAlt[percNum])[year]);
    else return(0.0); }
  /// <summary>Returns number of bendpoints in the alternate formula.</summary>
  ///
  /// <returns>Number of bendpoints in the alternate formula.</returns>
  int getNumBp() const { return numBp; }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets specified alternate bend point for specified year.
  /// </summary>
  ///
  /// <param name="year">Year for which alternate bend point is being set.
  /// </param>
  /// <param name="bendNum">Bend point number (1 to n-1 for n-part formula).</param>
  /// <param name="newAltBendPia">New alternate bend point.</param>
  void setAltBendPia( int year, int bendNum, double newAltBendPia )
  { if (bendNum > 0 && bendNum < 5)
    (*bendpAlt[bendNum - 1])[year] = newAltBendPia; }
  /// <summary>Sets specified alternate benefit formula percentage for
  /// specified year.</summary>
  ///
  /// <param name="year">Year for which percentage is set.</param>
  /// <param name="percNum">Percentage number (0, to n-1 for n-part formula).</param>
  /// <param name="newPerc">New alternate percentage.</param>
  void setAltPercPia( int year, int percNum, double newPerc )
  { if (percNum >= 0 && percNum < 5) (*percpAlt[percNum])[year] = newPerc; }
  /// <summary>Sets number of bendpoints in the alternate formulas.</summary>
  ///
  /// <param name="newNumBp">Number of bendpoints in the alternate formulas.</param>
  void setNumBp( int newNumBp )  { numBp = newNumBp; }
  void write( std::ostream& out ) const;
};
