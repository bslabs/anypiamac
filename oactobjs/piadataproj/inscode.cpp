// Functions for the <see cref="InsCode"/> class to manage an insured
// status code.
//
// $Id: inscode.cpp 1.18 2011/07/29 15:26:17EDT 044579 Development  $

#include "inscode.h"
#include "PiaException.h"

/// <summary>Initializes data to <see cref="InsCode::InsCodeType::NOQCS"/>.
/// </summary>
InsCode::InsCode() : theCode(NOQCS)
{ }

/// <summary>Initializes data to new code.</summary>
///
/// <param name="newCode">Insured status code to use.</param>
InsCode::InsCode( InsCodeType newCode ) : theCode(newCode)
{ }

/// <param name="newCode">The code to convert to a character.</param>
char InsCode::toChar( InsCodeType newCode )
{
  switch (newCode) {
    case NOQCS:            return '0';
    case FULLANDCURRENT:   return '1';
    case CURRENTNOTFULL:   return '2';
    case FULLNOTCURRENT:   return '3';
    case UNINSURED:        return '4';
    case PERMANDCURRENT:   return '5';
    case PERMNOTCURRENT:   return '6';
    case PRIMARYDEATH:     return '7';
    case TRANSITIONAL:     return '8';
    case NONPRIMARYDEATH:  return '9';
    case DEEMED:           return 'D';
    case TOTALIZED:        return 'T';
    default:
      throw PiaException("Illegal InsCodeType in InsCode::toChar");
  }
}

/// <summary>Converts a character to an <see cref="InsCode::InsCodeType"/>.
/// </summary>
///
/// <returns>The converted <see cref="InsCode::InsCodeType"/>.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> if the specified
/// character is not one of the allowed ones (0 through 9, D, and T).
/// </exception>
///
/// <param name="inscodechar">The character to convert.</param>
InsCode::InsCodeType InsCode::ChartoCode( char inscodechar )
{
  switch (inscodechar) {
    case '0':  return NOQCS;
    case '1':  return FULLANDCURRENT;
    case '2':  return CURRENTNOTFULL;
    case '3':  return FULLNOTCURRENT;
    case '4':  return UNINSURED;
    case '5':  return PERMANDCURRENT;
    case '6':  return PERMNOTCURRENT;
    case '7':  return PRIMARYDEATH;
    case '8':  return TRANSITIONAL;
    case '9':  return NONPRIMARYDEATH;
    case 'D':  return DEEMED;
    case 'T':  return TOTALIZED;
    default:
      throw PiaException("Illegal char in InsCode::ChartoCode");
  }
}
