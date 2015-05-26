// Declarations for the <see cref="Sga"/> class to manage SGA amounts.

// $Id: Sga.h 1.8 2011/08/09 15:55:30EDT 044579 Development  $

#pragma once

#include "SgaDerived.h"

/// <summary>Contains all of the SGA amounts and functions to use them.
/// </summary>
///
/// <remarks>Contains all of the SGA amounts and functions to use them.
/// Note that any users of this class should make sure to call the
/// <see cref="project"/> function because the constructor only initializes
/// the historical data.</remarks>
class Sga
{
public:
  /// <summary>SGA annual amounts for the blind.</summary>
  SgaBlindA sgaBlindA;
  /// <summary>SGA monthly (December) amounts for the blind.</summary>
  SgaBlindM sgaBlindM;
  /// <summary>SGA annual minimum amounts for the non-blind.</summary>
  SgaMinA sgaMinA;
  /// <summary>SGA monthly (December) minimum amounts for the non-blind.
  /// </summary>
  SgaMinM sgaMinM;
  /// <summary>SGA annual maximum amounts for the non-blind.</summary>
  SgaMaxA sgaMaxA;
  /// <summary>SGA monthly (December) maximum amounts for the non-blind.
  /// </summary>
  SgaMaxM sgaMaxM;
public:
  Sga( int newLastYear );
  void project( const DoubleAnnual& averageWages );
};
