// Declarations for the <see cref="LawChangeBPSPECRATE"/> class to manage
// parameters for a proposal to increase the bend points in the benefit
// formula by a specified growth rate.
//
// $Id: LawChangeBPSPECRATE.h 1.9 2011/07/29 16:27:31EDT 044579 Development  $

#pragma once

#include "LawChange.h"
#include "dbleann.h"

/// <summary>Parameters for a proposal to increase the bend points in the
/// benefit formula by a specified growth rate.</summary>
///
/// <remarks>There is one additional data field compared to the base class,
/// to hold the vector of bend point increase percentages for the PIA
/// formula.</remarks>
class LawChangeBPSPECRATE : public LawChange
{
private:
  /// <summary>Bend point increase percentages for the PIA formula.</summary>
  DoubleAnnual bpinc;
public:
  LawChangeBPSPECRATE();
  /// <summary>Returns bend point increase percentage for specified year.
  /// </summary>
  ///
  /// <returns>Bend point increase percentage for specified year.</returns>
  ///
  /// <param name="year">Year for which bend point increase percentage is
  /// desired.</param>
  double getBendPiaInc( int year ) const { return(bpinc[year]); }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets bend point increase percentage for specified year.
  /// </summary>
  ///
  /// <param name="year">Year for which bend point increase percentage is
  /// set.</param>
  /// <param name="newData">New bend point increase percentage.</param>
  void setBendPiaInc( int year, double newData ) { bpinc[year] = newData; }
  void write( std::ostream& out ) const;
};
