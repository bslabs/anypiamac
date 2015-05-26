// Declarations for the <see cref="LawChangeCOLACHANGE"/> class to manage
// parameters for a proposal to change the cost-of-living benefit increases.
//
// $Id: LawChangeCOLACHANGE.h 1.10 2011/08/01 10:06:07EDT 044579 Development  $

#pragma once

#include "LawChange.h"

/// <summary>Parameters for a proposal to change the cost-of-living benefit
/// increases.</summary>
///
/// <remarks>Normally the cost-of-living benefit increase equals the inflation
/// rate. This class allows two types of changes to the COLA.
///
/// If the indicator is set to 1, it adjusts the cola up or down by a
/// specified percentage (see <see cref="getColaAdj"/>).
///
/// If the indicator is set to 2, it limits the dollar increase in pia
/// resulting from applying the cola to an amount determined by finding a
/// specified percentile in the pia distribution (see
/// <see cref="getPiaPercentile"/>).</remarks>
class LawChangeCOLACHANGE : public LawChange
{
private:
  /// <summary>Amount by which to adjust the COLA.</summary>
  double colaAdj;
  /// <summary>The PIA percentile to use when indicator == 2.</summary>
  int piaPercentile;
public:
  LawChangeCOLACHANGE();
  /// <summary>Returns the amount of adjustment to COLA.</summary>
  ///
  /// <returns>The amount of adjustment to COLA.</returns>
  double getColaAdj() const { return colaAdj; }
  /// <summary>Returns the PIA percentile to use when indicator == 2.
  /// </summary>
  ///
  /// <returns>The PIA percentile to use when COLACHANGE == 2.</returns>
  int getPiaPercentile() const { return piaPercentile; }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets the amount of adjustment to COLA.</summary>
  ///
  /// <param name="newColaAdj">Amount by which to adjust the COLA (negative
  /// number means a smaller COLA, positive number means a bigger COLA).
  /// </param>
  void setColaAdj( double newColaAdj ) { colaAdj = newColaAdj; }
  /// <summary>Sets the PIA percentile to use when indicator == 2.</summary>
  ///
  /// <param name="newPiaPercentile">The PIA percentile to use.</param>
  void setPiaPercentile( int newPiaPercentile )
  { piaPercentile = newPiaPercentile; }
  void write( std::ostream& out ) const;
};
