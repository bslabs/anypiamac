// Declarations for the <see cref="LawChangeBPMINCONST"/> class to manage
// parameters for a proposal to increase the bend points in the benefit
// formula by the growth in wages minus a specified rate.
//
// $Id: LawChangeBPMINCONST.h 1.8 2011/07/29 16:27:32EDT 044579 Development  $

#pragma once

#include "LawChange.h"

/// <summary>Parameters for a proposal to increase the bend points in the
/// benefit formula by the growth in wages minus a specified rate.</summary>
///
/// <remarks>There is one additional data field compared to the base class,
/// to hold the constant percentage subtracted from bend point increase.
/// </remarks>
class LawChangeBPMINCONST : public LawChange
{
private:
  /// <summary>Constant percentage subtracted from bend point increase.
  /// </summary>
  double cp;
public:
  LawChangeBPMINCONST();
  /// <summary>Returns constant percentage subtracted from bend point
  /// increase.</summary>
  ///
  /// <returns>Constant percentage subtracted from bend point increase.
  /// </returns>
  double getConstantPerc() const { return(cp); }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets constant percentage subtracted from bend point increase.
  /// </summary>
  ///
  /// <param name="newConstantPerc">New constant percentage subtracted from
  /// bend point increase.</param>
  void setConstantPerc( double newConstantPerc ) { cp = newConstantPerc; }
  void write( std::ostream& out ) const;
};
