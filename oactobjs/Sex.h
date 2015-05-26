// Declarations needed for the <see cref="Sex"/> class holding a sex indicator.

// $Id: Sex.h 1.14 2010/11/23 10:21:00EST 044579 Development  $

#pragma once

#include <string>

/// <summary>Indicator for sex type.</summary>
///
/// <remarks>Most functions are inline for efficiency.</remarks>
class Sex
{
public:
  /// <summary>The enumerated sex indicator.</summary>
  enum sex_type {
    MALE,  // Male.
    FEMALE,  // Female.
    NUM_SEX  // Number of sexes.
  };
private:
  /// <summary>The sex code (see <see cref="sex_type"/>).</summary>
  int theCode;
public:
  /// <summary>Sets the sex code to <see cref="Sex::sex_type::MALE"/>.
  /// </summary>
  Sex() : theCode(MALE) { }
  /// <summary>Sets the sex code to a new sex code.</summary>
  ///
  /// <param name="newCode">New sex code.</param>
  Sex( sex_type newCode ) : theCode(newCode) { }
  /// <summary>Copies the sex code from another <see cref="Sex"/>.</summary>
  ///
  /// <param name="sex">The <see cref="Sex"/> to copy from.</param>
  Sex( const Sex& sex ) : theCode(sex.theCode) { }
  /// <summary>Destructor.</summary>
  ///
  /// <remarks>Does nothing.</remarks>
  ~Sex() { }
  /// <summary>Copies from another <see cref="Sex"/>.</summary>
  ///
  /// <returns>A reference to this instance.</returns>
  ///
  /// <param name="sex">The <see cref="Sex"/> to copy from.</param>
  Sex& operator=( const Sex& sex ) { theCode = sex.theCode; return *this; }
  /// <summary>Returns default output label for sex of worker.</summary>
  ///
  /// <returns>Output label for sex of worker.</returns>
  ///
  /// <remarks>Returns a lower-case string.</remarks>
  std::string toString() const { return(theCode == MALE ?
    std::string("male") : std::string("female") ); }
  std::string toString( const std::string& format ) const;
  static void check( int newSex );
};
