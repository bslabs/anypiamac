// Declarations for the <see cref="LawChangeWIDFACTOR"/> class to manage
// parameters for a proposal to change the factor for an aged widow(er)
// benefit.

// $Id: LawChangeWIDFACTOR.h 1.7 2011/08/01 11:38:36EDT 044579 Development  $

#pragma once

#include "LawChange.h"

/// <summary>Parameters for a proposal to change the factor for an aged
/// widow(er) benefit.</summary>
///
/// <remarks>There are no additional data fields compared to the base class.
///
/// If the indicator is greater than zero, it means that the present-law factor
/// for an aged widow(er) of 100 percent is increased to 112.5 percent, in one
/// of the following ways:
/// <table>
///  1   effective immediately.
///  2   phased in over 40 years.
/// </table></remarks>
class LawChangeWIDFACTOR : public LawChange
{
public:
  /// <summary>The ultimate changed factor for an aged widow(er) benefit.
  /// </summary>
  static const double newAgedWidFactor;
  /// <summary>The number of years over which the factor for an aged widow(er)
  /// benefit is phased in.</summary>
  static const int numYears;
public:
  LawChangeWIDFACTOR();
  void prepareStrings( std::vector< std::string >& outputString );
};
