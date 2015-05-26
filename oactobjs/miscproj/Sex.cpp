// Functions for the <see cref="Sex"/> class holding a sex indicator.

// $Id: Sex.cpp 1.8 2011/08/19 15:57:44EDT 044579 Development  $

#include "Sex.h"
#include "PiaException.h"
#include "Resource.h"

/// <summary>Checks sex.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_SEX"/> if the argument is not
/// <see cref="Sex::sex_type::MALE"/> or <see cref="Sex::sex_type::FEMALE"/>.
/// </exception>
///
/// <param name="newSex">The sex value to check.</param>
void Sex::check( int newSex )
{
  if (newSex != static_cast<int>(MALE) && newSex != static_cast<int>(FEMALE))
    throw PiaException(PIA_IDS_SEX);
}

/// <summary>Returns specified output label for sex of worker.</summary>
///
/// <returns>Output label for sex of worker.</returns>
///
/// <param name="format">Format to use. If "c", return capitalized string.
/// Otherwise, return lower-case string.</param>
std::string Sex::toString( const std::string& format ) const
{
  if (format.length() < 1 || format[0] != 'c')
    return toString();
  return(theCode == MALE ? std::string("Male") : std::string("Female") );
}
