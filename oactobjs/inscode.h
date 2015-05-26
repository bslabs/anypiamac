// Declarations for the <see cref="InsCode"/> class to manage an insured
// status code.
//
// $Id: inscode.h 1.20 2011/07/29 15:26:16EDT 044579 Development  $

#pragma once

/// <summary>Represents one insured status code.</summary>
///
/// <remarks>An instance of this class is used in <see cref="PiaData"/>. An
/// array of instances is used in <see cref="InsCodeArray"/>.</remarks>
class InsCode
{
public:
  /// <summary>Insured status code types.</summary>
  ///
  /// <remarks>Most of these codes come from the CWHS.  The last two are added
  /// for use in OACT programs.</remarks>
  enum InsCodeType {
    NOQCS,            // 0 - active (no QC's to date).
    FULLANDCURRENT,   // 1 - fully and currently insured.
    CURRENTNOTFULL,   // 2 - currently but not fully insured.
    FULLNOTCURRENT,   // 3 - fully but not currently insured.
    UNINSURED,        // 4 - uninsured (but some QC's to date).
    PERMANDCURRENT,   // 5 - permanently and currently insured.
    PERMNOTCURRENT,   // 6 - permanently but not currently insured.
    PRIMARYDEATH,     // 7 - primary-entitled death.
    TRANSITIONAL,     // 8 - transitionally insured.
    NONPRIMARYDEATH,  // 9 - non-primary-entitled death.
    DEEMED,           // D - deemed fully insured (1983 amendments).
    TOTALIZED         // T - insured for totalization benefits.
  };
private:
  /// <summary>Insured status code.</summary>
  InsCodeType theCode;
public:
  InsCode();
  InsCode( InsCodeType newCode );
  /// <summary>Returns true if the <see cref="InsCode::InsCodeType"/>'s are
  /// equal.</summary>
  ///
  /// <returns>True if the <see cref="InsCode::InsCodeType"/>'s are equal.
  /// </returns>
  ///
  /// <param name="newCode">The insured status code to compare against.
  /// </param>
  bool operator==( const InsCode& newCode ) const
  { return (theCode == newCode.theCode); }
  /// <summary>Returns the insured status code.</summary>
  ///
  /// <returns>The insured status code.</returns>
  InsCodeType get() const { return theCode; }
  /// <summary>Returns the currently insured status.</summary>
  ///
  /// <returns>True if currently insured, false otherwise.</returns>
  bool isCurrentlyInsured() const
  { return(theCode == FULLANDCURRENT || theCode == CURRENTNOTFULL ||
    theCode == PERMANDCURRENT); }
  /// <summary>Returns the fully insured status.</summary>
  ///
  /// <returns>True if fully insured, false otherwise.</returns>
  bool isFullyInsured() const
  { return(theCode == FULLANDCURRENT || theCode == FULLNOTCURRENT ||
    theCode == PERMANDCURRENT || theCode == PERMNOTCURRENT ||
    theCode == DEEMED); }
  /// <summary>Returns the fully or totalization insured status.</summary>
  ///
  /// <returns>True if fully insured or insured for totalization
  /// benefits, false otherwise.</returns>
  bool isFullyOrTotalInsured() const
  { return(isFullyInsured() || theCode == TOTALIZED); }
  /// <summary>Returns the fully or currently insured status.</summary>
  ///
  /// <returns>True if currently insured or fully insured, false otherwise.
  /// </returns>
  bool isFullyOrCurrentlyInsured() const
  { return(isFullyInsured() || isCurrentlyInsured()); }
  /// <summary>Returns permanently insured status.</summary>
  ///
  /// <returns>True if permanently insured, false otherwise.</returns>
  bool isPermanentlyInsured() const
  { return(theCode == PERMANDCURRENT || theCode == PERMNOTCURRENT); }
  /// <summary>Sets the insured status code.</summary>
  ///
  /// <param name="newCode">New insured status code.</param>
  void set( InsCodeType newCode ) { theCode = newCode; }
  /// <summary>Returns the stored insured status code as a character.
  /// </summary>
  ///
  /// <remarks>This version calls the static version of this function using
  /// the stored code.</remarks>
  ///
  /// <returns>The insured status code as a character.</returns>
  char toChar() const { return(toChar(theCode)); }
  /// <summary>Returns the passed insured status code as a character.
  /// </summary>
  ///
  /// <returns>The insured status code as a character.</returns>
  static char toChar( InsCodeType newCode );
  static InsCodeType ChartoCode( char inscodechar );
};
