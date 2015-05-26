// Functions for the <see cref="DisInsCode"/> class representing a
// disability insured status code.
//
// $Id: dinscode.cpp 1.17 2011/07/29 15:26:16EDT 044579 Development  $

#include "dinscode.h"
#include "PiaException.h"

using namespace std;

/// <summary>Initializes data to
/// <see cref="DisInsCode::DisInsCodeType::NOQCS"/>.</summary>
DisInsCode::DisInsCode() : theCode(NOQCS)
{ }

/// <summary>Initializes data to a new code.</summary>
///
/// <param name="newCode">New code to use.</param>
DisInsCode::DisInsCode( DisInsCodeType newCode ) : theCode(newCode)
{ }

/// <param name="newCode">The code to convert to a character.</param>
char DisInsCode::toChar( DisInsCodeType newCode )
{
  switch (newCode) {
    case NOQCS:            return '0';
    case FULLAND2040:      return '1';
    case FULLANDSPECIAL:   return '2';
    case FULLANDDIB:       return '3';
    case FULLANDBLIND:     return '4';
    case NOTFULLNOT2040:   return '5';
    case NOTFULLHAS2040:   return '6';
    case FULLNOT2040:      return '7';
    case PRIMARYDEATH:     return '8';
    case NONPRIMARYDEATH:  return '9';
    case TOTALIZED:        return 'T';
    default:
      throw PiaException("Illegal InsCodeType in InsCode::toChar");
  }
}

/// <summary>Converts a character to a
/// <see cref="DisInsCode::DisInsCodeType"/>.</summary>
///
/// <returns>The converted <see cref="DisInsCode::DisInsCodeType"/>.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> if the specified
/// character is not one of the allowed ones (0 through 9, and T).</exception>
///
/// <param name="dinscodechar">The character to convert.</param>
DisInsCode::DisInsCodeType DisInsCode::ChartoCode( char dinscodechar )
{
  switch (dinscodechar) {
    case '0':  return NOQCS;
    case '1':  return FULLAND2040;
    case '2':  return FULLANDSPECIAL;
    case '3':  return FULLANDDIB;
    case '4':  return FULLANDBLIND;
    case '5':  return NOTFULLNOT2040;
    case '6':  return NOTFULLHAS2040;
    case '7':  return FULLNOT2040;
    case '8':  return PRIMARYDEATH;
    case '9':  return NONPRIMARYDEATH;
    case 'T':  return TOTALIZED;
    default:
      throw PiaException("Illegal char in InsCode::ChartoCode");
  }
}
