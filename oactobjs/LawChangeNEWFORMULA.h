// Declarations for the <see cref="LawChangeNEWFORMULA"/> class to manage
// parameters for a proposal to change the pia formula bend points and/or
// percentages.
//
// $Id: LawChangeNEWFORMULA.h 1.9 2011/08/01 10:06:10EDT 044579 Development  $

#pragma once

#include "LawChange.h"
#include "dbleann.h"

/// <summary>Parameters for a proposal to change the pia formula bend points
/// and/or percentages.</summary>
///
/// <remarks>There are two additional data fields compared to the base class.
///
/// Currently only an indicator of 1 is allowed. Then a new benefit formula of
/// 3 percentages and 2 bend points is used for each year in the specified
/// period. In the future we hope to allow for an indicator of 2, for a
/// benefit formula of 4 percentages and 3 bend points.</remarks>
class LawChangeNEWFORMULA : public LawChange
{
private:
  /// <summary>Alternative PIA formula bend points.</summary>
  DoubleAnnual* bendpAlt[3];
  /// <summary>Alternative benefit formula percentages for PIA formula.
  /// </summary>
  DoubleAnnual* percpAlt[4];
public:
  LawChangeNEWFORMULA();
  ~LawChangeNEWFORMULA();
  /// <summary>Returns specified alternate bend point for specified year.
  /// </summary>
  ///
  /// <returns>Specified alternate bend point for specified year.</returns>
  ///
  /// <param name="year">Year for which bend point is desired.</param>
  /// <param name="bendNum">Bend point number (1 or 2 for 3-part formula;
  /// 1, 2, or 3 for 4-part formula).</param>
  double getAltBendPia( int year, int bendNum ) const
  { if (bendNum > 0 && bendNum < 4) return((*bendpAlt[bendNum - 1])[year]);
    else return(0.0); }
  /// <summary>Returns specified benefit formula percentage for specified
  /// year.</summary>
  ///
  /// <returns>Specified benefit formula percentage for specified year.
  /// </returns>
  ///
  /// <param name="year">Year of desired benefit formula percentage.</param>
  /// <param name="percNum">Percentage number (0, 1, or 2 for 3-part formula;
  /// 0, 1, 2, or 3 for 4-part formula).</param>
  double getAltPercPia( int year, int percNum ) const
  { if (percNum >= 0 && percNum < 4) return((*percpAlt[percNum])[year]);
    else return(0.0); }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets specified alternate bend point for specified year.
  /// </summary>
  ///
  /// <param name="year">Year for which alternate bend point is being set.
  /// </param>
  /// <param name="bendNum">Bend point number (1 or 2 for 3-part formula;
  /// 1, 2, or 3 for 4-part formula).</param>
  /// <param name="newAltBendPia">New alternate bend point.</param>
  void setAltBendPia( int year, int bendNum, double newAltBendPia )
  { if (bendNum > 0 && bendNum < 4)
    (*bendpAlt[bendNum - 1])[year] = newAltBendPia; }
  /// <summary>Sets specified alternate benefit formula percentage for
  /// specified year.</summary>
  ///
  /// <param name="year">Year for which percentage is set.</param>
  /// <param name="percNum">Percentage number (0, 1, or 2 for 3-part formula;
  /// 0, 1, 2, or 3 for 4-part formula).</param>
  /// <param name="newPerc">New alternate percentage.</param>
  void setAltPercPia( int year, int percNum, double newPerc )
  { if (percNum >= 0 && percNum < 4) (*percpAlt[percNum])[year] = newPerc; }
  void write( std::ostream& out ) const;
};
