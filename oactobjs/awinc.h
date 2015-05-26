// Declarations for the <see cref="Awinc"/> class to manage a vector of
// average wage increases.
//
// $Id: awinc.h 1.14 2011/07/28 10:16:26EDT 044579 Development  $

#pragma once

#include "dbleann.h"

class AverageWage;

/// <summary>Manages a vector of projected average wage increases.</summary>
///
/// <remarks>Manages a vector of projected average wage increases.</remarks>
class Awinc : public DoubleAnnual
{
public:
  Awinc( int newLastYear );
  Awinc( int newBaseYear, int newLastYear );
  void project( const AverageWage& averageWages, int firstYeart,
    int lastYeart );
  void project( double factor, int firstYeart );
  static void fqincCheck( double fqinct );
};
