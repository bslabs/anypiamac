// Declarations for the <see cref="LawChangeTAXBENCHG"/> class to manage
// parameters for a proposal to change the taxation of benefits.

// $Id: LawChangeTAXBENCHG.h 1.5 2011/08/01 10:06:09EDT 044579 Development  $

#pragma once

#include "LawChange.h"

/// <summary>Parameters for a proposal to change the taxation of benefits.
/// </summary>
///
/// <remarks>If the indicator is greater than zero, it means the following:
/// <table>
///   1   phase thresholds down to zero over 10 years (Ball proposal).
///   2   tax 100 percent of benefits, but allow a deduction for OASDI contributions similar to that for private pensions.
///   3   combination of 1 and 2.
/// </table></remarks>
class LawChangeTAXBENCHG : public LawChange
{
public:
  LawChangeTAXBENCHG();
  void prepareStrings( std::vector< std::string >& outputString );
};
