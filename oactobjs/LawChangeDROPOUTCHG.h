// Declarations for the <see cref="LawChangeDROPOUTCHG"/> class to manage
// parameters for a proposal to reduce the number of dropout years in a pia
// calculation.
//
// $Id: LawChangeDROPOUTCHG.h 1.7 2011/08/01 10:06:08EDT 044579 Development  $

#pragma once

#include "LawChange.h"

/// <summary>Parameters for a proposal to reduce the number of dropout
/// years in a pia calculation.</summary>
///
/// <remarks>There is one additional data field compared to the base class.
///
/// If the indicator is greater than zero, it indicates the number of years by
/// which drop out years are to be decreased, phased in over period of years.
/// The number of drop out years decreases by 1 every step years.</remarks>
class LawChangeDROPOUTCHG : public LawChange
{
private:
  /// <summary>Step size for reduction in number of dropout years.</summary>
  int step;
public:
  LawChangeDROPOUTCHG();
  /// <summary>Returns step size when reducing number of dropout years.
  /// </summary>
  ///
  /// <returns>Step size when reducing number of dropout years.</returns>
  int getStep() const { return step; }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets step size when reducing number of dropout years.</summary>
  ///
  /// <param name="newStep">New step size when reducing number of dropout
  /// years.</param>
  void setStep( int newStep ) { step = newStep; }
  void write( std::ostream& out ) const;
};
