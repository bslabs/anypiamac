// Declarations for the <see cref="LawChangeBPFRACWAGE"/> class to manage
// parameters for a proposal to increase the bend points in the benefit
// formula by a fraction of average wage growth.
//
// $Id: LawChangeBPFRACWAGE.h 1.7 2011/07/29 16:27:32EDT 044579 Development  $

#pragma once

#include "LawChange.h"

/// <summary>Parameters for a proposal to increase the bend points in the
/// benefit formula by a fraction of average wage growth.</summary>
///
/// <remarks>There is one additional data field compared to the base class.
/// </remarks>
class LawChangeBPFRACWAGE : public LawChange
{
private:
  /// <summary>Proportion of wage increase applied to bend points.</summary>
  double pbp;
public:
  LawChangeBPFRACWAGE();
  /// <summary>Returns proportion of wage increase applied to bend points.
  /// </summary>
  ///
  /// <returns>Proportion of wage increase applied to bend points.</returns>
  double getProportionBendPoints() const { return(pbp); }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets proportion of wage increase applied to bend points.
  /// </summary>
  ///
  /// <param name="proportion">New proportion of wage increase applied to
  /// bend points.</param>
  void setProportionBendPoints( double proportion ) { pbp = proportion; }
  void write( std::ostream& out ) const;
};
