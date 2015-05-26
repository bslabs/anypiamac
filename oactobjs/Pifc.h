// Declarations for the <see cref="Pifc"/> class representing a type of
// Social Security pia.
//
// $Id: Pifc.h 1.14 2011/08/08 08:45:15EDT 044579 Development  $

#pragma once

#include "OldStart.h"

/// <summary>Represents a pia factor code (type of Social Security pia).
/// </summary>
///
/// <remarks>Represents a pia factor code (type of Social Security pia).It has
/// the following possible values:
/// <table>
/// A   Transitional uninsured or insured PIA (unused).
/// B   AMW PIA.
/// C   Special minimum PIA.
/// E   AMW survivor PIA greater than life PIA (unused).
/// F   Special minimum survivor PIA greater than life PIA (unused).
/// G   AMW survivor PIA equal to life PIA (unused).
/// H   Special minimum survivor PIA equal to life PIA (unused).
/// K   Totalized (prorated) PIA.
/// L   AIME PIA.
/// M   Frozen minimum PIA.
/// N   New-start guarantee PIA.
/// O   Old-start guarantee PIA.
/// S   DIB guarantee PIA.
/// V   Modified old start windfall PIA.
/// W   Widow(er)'s reindexed PIA (used only in OACT code).
/// Y   Child care dropout years method (used only in OACT code).
/// Z   Northern Mariana Islands computation (unused).
/// 5   Modified new start windfall PIA.
/// 7   1990 new start (unused).
/// 8   1990 old start.
/// </table></remarks>
class Pifc
{
private:
  /// <summary>Code for type of pia.</summary>
  ///
  /// <remarks>It has the following possible values:
  /// <table>
  /// A   Transitional uninsured or insured PIA (unused).
  /// B   AMW PIA.
  /// C   Special minimum PIA.
  /// E   AMW survivor PIA greater than life PIA (unused).
  /// F   Special minimum survivor PIA greater than life PIA (unused).
  /// G   AMW survivor PIA equal to life PIA (unused).
  /// H   Special minimum survivor PIA equal to life PIA (unused).
  /// K   Totalized (prorated) PIA.
  /// L   AIME PIA.
  /// M   Frozen minimum PIA.
  /// N   New-start guarantee PIA.
  /// O   Old-start guarantee PIA.
  /// S   DIB guarantee PIA.
  /// V   Modified old start windfall PIA.
  /// W   Widow(er)'s reindexed PIA (used only in OACT code).
  /// Y   Child care dropout years method (used only in OACT code).
  /// Z   Northern Mariana Islands computation (unused).
  /// 5   Modified new start windfall PIA.
  /// 7   1990 new start (unused).
  /// 8   1990 old start.
  /// </table></remarks>
  char theCode;
public:
  /// <summary>Initializes pia factor code.</summary>
  ///
  /// <param name="newCode">New pia factor code to use. If omitted, uses a
  ///  blank character.</param>
  Pifc( char newCode = ' ' ) : theCode(newCode) { }
  /// <summary>Returns the pifc code.</summary>
  ///
  /// <returns>The pifc code.</returns>
  char get() const { return theCode; }
  /// <summary>Returns true if able to apply cola to this benefit.</summary>
  ///
  /// <returns>True if able to apply cola to this benefit.</returns>
  ///
  /// <remarks>Code must not be special minimum or frozen minimum pia.
  /// </remarks>
  bool isColable() const { return theCode != 'C' && theCode != 'M'; }
  /// <summary>Returns true if wage-indexed mfb applies to this benefit.
  /// </summary>
  ///
  /// <returns>True if wage-indexed mfb applies to this benefit.</returns>
  bool isNewMfb() const { return theCode == 'L' || theCode == 'M' ||
    theCode == 'N' || theCode == 'O' || theCode == 'Y' || theCode == 'V' ||
    theCode == '5'; }
  /// <summary>Sets the pifc code.</summary>
  ///
  /// <param name="newCode">New pifc code.</param>
  void set( char newCode ) { theCode = newCode; }
  static char pifcCal( int appnum, PiaMethod::WindfallElimType indwind,
    OldStart::OldStartType methodOS, bool bamend90, bool btotalize );
};
