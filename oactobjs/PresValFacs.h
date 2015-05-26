// Declarations for the <see cref="PresValFacs"/> class to handle present
// value factors.
//
// $Id: PresValFacs.h 1.5 2011/08/08 08:45:11EDT 044579 Development  $

#pragma once

#include "dbleann.h"

/// <summary>Manages an array of present value factors.</summary>
///
/// <remarks>The factors are used to discount money from December 31 of a year
/// back to January 1 of the base year (or, equivalently, back to December 31
/// of the year prior to the base year).</remarks>
class PresValFacs : public DoubleAnnual
{
public:
  PresValFacs( const DoubleAnnual& intRates );
  ~PresValFacs();
  void project( const DoubleAnnual& intRates );
};
