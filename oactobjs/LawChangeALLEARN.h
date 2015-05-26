// Declarations for the <see cref="LawChangeALLEARN"/> class to manage
// parameters for a proposal to use all years of earnings in an AIME
// calculation.
//
// $Id: LawChangeALLEARN.h 1.7 2011/07/29 16:27:32EDT 044579 Development  $

#pragma once

#include "LawChange.h"

/// <summary>Parameters for a proposal to use all years of earnings in an
/// AIME calculation.</summary>
///
/// <remarks>There are two additional data fields compared to the base class.
///
/// If the indicator is 1, we use all earnings in the numerator of the AIME
/// calculation. If the indicator is 2, we use all earnings in the numerator
/// after a "phase-in" period. The number of years of earnings used increases
/// by 1 every step years, until we use all earnings in the specified
/// year.</remarks>
class LawChangeALLEARN : public LawChange
{
private:
  /// <summary>Step size when phasing in use of all earnings in AIME
  /// calculation.</summary>
  int step;
  /// <summary>Year when phase-in begins, when phasing in the use of all
  /// earnings in AIME calculation.</summary>
  int yearPhasein;
public:
  LawChangeALLEARN();
  /// <summary>Returns step size when increasing number of earnings years.
  /// </summary>
  ///
  /// <returns>Step size when increasing number of earnings years.</returns>
  int getStep() const { return step; }
  /// <summary>Returns starting year when increasing number of earnings
  /// years.</summary>
  ///
  /// <returns>Starting year when increasing number of earnings years.
  /// </returns>
  int getYear() const { return yearPhasein; }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets step size when increasing number of earnings years.
  /// </summary>
  ///
  /// <param name="newStep">New step size when increasing number of earnings
  /// years.</param>
  void setStep( int newStep ) { step = newStep; }
  /// <summary>Sets starting year when increasing number of earnings years.
  /// </summary>
  ///
  /// <param name="newYear">New starting year when increasing number of
  /// earnings years.</param>
  void setYear( int newYear ) { yearPhasein = newYear; }
  void write( std::ostream& out ) const;
};
