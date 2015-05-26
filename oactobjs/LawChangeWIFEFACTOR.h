// Declarations for the <see cref="LawChangeWIFEFACTOR"/> class to manage
// parameters for a proposal to change the factor for an aged spouse benefit.

// $Id: LawChangeWIFEFACTOR.h 1.7 2011/08/01 11:38:35EDT 044579 Development  $

#pragma once

#include "LawChange.h"

/// <summary>Parameters for a proposal to change the factor for an aged
/// spouse benefit.</summary>
///
/// <remarks>There are no additional data fields compared to the base class.
///
/// If the indicator is greater than zero, it means that the present-law
/// factor for an aged spouse of 50 percent is reduced to 33 percent, in one
/// of the following ways:
/// <table>
///  1   effective immediately.
///  2   phased in over 17 years at 1 percent per year.
/// </table></remarks>
class LawChangeWIFEFACTOR : public LawChange
{
public:
  /// <summary>The ultimate changed factor for an aged spouse benefit.
  /// </summary>
  static const double newSpouseFactor;
  /// <summary>The annual amount of reduction in the factor for an aged spouse
  /// benefit.</summary>
  static const double amtPerYear;
public:
  LawChangeWIFEFACTOR();
  void prepareStrings( std::vector< std::string >& outputString );
};
