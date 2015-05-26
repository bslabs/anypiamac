// Declarations for the <see cref="DisInsCode"/> class representing a
// disability insured status code.
//
// $Id: dinscode.h 1.20 2011/07/29 15:26:17EDT 044579 Development  $

#pragma once

/// <summary>Represents one disability insured status code.</summary>
///
/// <remarks>An instance of this class is used in <see cref="PiaData"/>. An
/// array of instances is used in <see cref="DisInsCodeArray"/>.</remarks>
class DisInsCode
{
public:
  /// <summary>Disability insured status code types.</summary>
  ///
  /// <remarks>Most of these codes come from the CWHS.  The last one is
  /// added for use in OACT programs.</remarks>
  enum DisInsCodeType {
    /// <summary>0 - active (no QC's to date).</summary>
    NOQCS,
    /// <summary>1 - disability insured (fully insured and meets 20/40).
    /// </summary>
    FULLAND2040,
    /// <summary>2 - disability insured (fully insured and meets
    /// special test).</summary>
    FULLANDSPECIAL,
    /// <summary>3 - disability insured (current DI beneficiary).</summary>
    FULLANDDIB,
    /// <summary>4 - disability insured (based on blindness).</summary>
    FULLANDBLIND,
    /// <summary>5 - not fully insured or 20/40 (but some QC's to date).
    /// </summary>
    NOTFULLNOT2040,
    /// <summary>6 - not fully insured, but meets 20/40.</summary>
    NOTFULLHAS2040,
    /// <summary>7 - fully insured, but does not meet 20/40 or special test.
    /// </summary>
    FULLNOT2040,
    /// <summary>8 - death (was a primary beneficiary).</summary>
    PRIMARYDEATH,
    /// <summary>9 - death (was never a primary beneficiary).</summary>
    NONPRIMARYDEATH,
    /// <summary>T - insured for totalization benefits.</summary>
    TOTALIZED
  };
private:
  /// <summary>Disability insured status code.</summary>
  DisInsCodeType theCode;
public:
  DisInsCode();
  DisInsCode( DisInsCodeType newCode );
  /// <summary>Returns true if the <see cref="DisInsCode::DisInsCodeType"/>'s
  /// are equal.</summary>
  ///
  /// <returns>True if the <see cref="DisInsCode::DisInsCodeType"/>'s are
  /// equal.</returns>
  ///
  /// <param name="newCode">The disability insured status code
  /// to compare against.</param>
  bool operator==( const DisInsCode& newCode ) const
  { return (theCode == newCode.theCode); }
  /// <summary>Returns the disability insured status code.</summary>
  ///
  /// <returns>The disability insured status code.</returns>
  DisInsCodeType get() const { return theCode; }
  /// <summary>Returns disability insured status.</summary>
  ///
  /// <returns>True if disability insured, false otherwise.</returns>
  bool isDisabilityInsured() const
  { return(theCode == FULLAND2040 || theCode == FULLANDSPECIAL ||
    theCode == FULLANDDIB || theCode == FULLANDBLIND); }
  /// <summary>Returns disability benefit receipt status.</summary>
  ///
  /// <returns>True if receiving disability benefit, false otherwise.</returns>
  bool isDiBenie() const
  { return(theCode == TOTALIZED || theCode == FULLANDDIB ||
    theCode == FULLANDBLIND); }
  /// <summary>Sets the disability insured status code.</summary>
  ///
  /// <param name="newCode">New code to use.</param>
  void set( DisInsCodeType newCode ) { theCode = newCode; }
  /// <summary>Returns the stored disability insured status code as
  /// a character.</summary>
  ///
  /// <remarks>Calls the static version of this function using the
  /// stored code.</remarks>
  ///
  /// <returns>The disability insured status code as a character.</returns>
  char toChar() const { return(toChar(theCode)); }
  /// <summary>Returns the passed disability insured status code as
  /// a character.</summary>
  ///
  /// <returns>The disability insured status code as a character.</returns>
  static char toChar( DisInsCodeType newCode );
  static DisInsCodeType ChartoCode( char dinscodechar );
};
