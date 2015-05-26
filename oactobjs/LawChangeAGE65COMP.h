// Declarations for the <see cref="LawChangeAGE65COMP"/> class to manage
// parameters for a proposal to increase the number of elapsed years in a pia
// calculation.
//
// $Id: LawChangeAGE65COMP.h 1.6 2011/07/29 16:27:32EDT 044579 Development  $

#pragma once

#include "LawChange.h"

/// <summary>Parameters for a proposal to increase the number of elapsed
/// years in a pia calculation.</summary>
///
/// <remarks>There is one additional data field compared to the base class.
///
/// If the indicator is greater than zero, its value of n is the number of
/// years of increase in the elapsed years, so that the computation period is
/// measured to age 62 + n. The change is phased in over a period of years
/// according to the step size. (The computation increases by 1 year every
/// step years.)</remarks>
class LawChangeAGE65COMP : public LawChange
{
private:
  /// <summary>Step size for increase in number of computation years.
  /// </summary>
  int step;
public:
  LawChangeAGE65COMP();
  /// <summary>Returns step size when increasing computation period.</summary>
  ///
  /// <returns>Step size when increasing computation period.</returns>
  int getStep() const { return step; }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets step size when increasing computation period.</summary>
  ///
  /// <param name="newStep">New step size when increasing computation period.
  /// </param>
  void setStep( int newStep ) { step = newStep; }
  void write( std::ostream& out ) const;
};
