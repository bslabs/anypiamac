// Functions for the <see cref="Pifc"/> class representing a type of
// Social Security pia.
//
// $Id: Pifc.cpp 1.14 2018/08/06 06:53:56EDT 277133 Development  $

#include "Pifc.h"
#include "PiaMethod.h"

/// <summary>Calculates applicable pifc.</summary>
///
/// <remarks>Note that this is a static function.</remarks>
///
/// <returns>Applicable pifc.</returns>
///
/// <param name="appnum">Number of applicable pia method (see
/// <see cref="PiaMethod::pia_type"/>).</param>
/// <param name="indwind">Indicator for windfall elimination provision.
/// </param>
/// <param name="methodOS">Old-start PIA calculation method (unused if no
/// old start involved).</param>
/// <param name="bamend90">True if entitlement after 1990 amendments, or if
/// earnings in 1992 or later were used to increase the new-start PIA.</param>
/// <param name="btotalize">True if totalization applies.</param>
char Pifc::pifcCal( int appnum, PiaMethod::WindfallElimType indwind,
OldStart::OldStartType methodOS, bool bamend90, bool btotalize )
{
  // Totalization pifc is always K, even though the underlying calculation
  // could be AMW or AIME method.
  if (btotalize)
    return('K');
  // Not totalization, so look at appnum
  switch (appnum)
  {
  case PiaMethod::OLD_START:
    if (indwind == PiaMethod::ONEHALFPENSION ||
      indwind == PiaMethod::REDUCEDPERC)
      return('V');
    else if (methodOS == OldStart::OS1990)
      return('8');
    else if (methodOS == OldStart::OS1977_79)
      return('O');
    else
      return('B');
    break;
  case PiaMethod::PIA_TABLE:
    return(bamend90 ? '7' : 'B');
    break;
  case PiaMethod::WAGE_IND:
    return(indwind > 0 ? '5' : 'L');
    break;
  case PiaMethod::TRANS_GUAR:
    return('N');
    break;
  case PiaMethod::SPEC_MIN:
    return('C');
    break;
  case PiaMethod::REIND_WID:
    return('W');
    break;
  case PiaMethod::FROZ_MIN:
    return('M');
    break;
  case PiaMethod::CHILD_CARE:
    return ('Y');
    break;
  case PiaMethod::DIB_GUAR:
    return ('S');
    break;
  case PiaMethod::WAGE_IND_NON_FREEZE:
    return(indwind > 0 ? '5' : 'L');
    break;
  default:
    return(' ');
  }
}
