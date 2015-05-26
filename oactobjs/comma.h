// Declarations of functions that return strings with (or without) embedded
// commas in any type of number (floating or integral).
//
// $Id: comma.h 1.21 2011/08/11 15:21:16EDT 044579 Development  $

#pragma once

#include "CommaBase.h"
#include <iomanip>
#include <typeinfo> /// for typeid
#include <string>
#include "PiaException.h"
#include "Resource.h"

void pcomma( std::string& start, int dec, int point, int sign );
void pblank( std::string& start, int width, bool zeroFill );

/// <summary>Converts any type of number to a string with or without
/// commas and a given number of decimal places.</summary>
///
/// <remarks>Converts any type of number to a string with or without commas
/// and a given number of decimal places. Generally you would not use this
/// function, preferring one of the wrapper functions poutComma or
/// poutNoComma.</remarks>
///
/// <returns>Formatted string.</returns>
///
/// <exception cref="PiaException">PiaException of type
/// <see cref="PIA_IDS_COMMA"/> if number of decimals is
/// nonzero for other than a float type (only in debug mode).</exception>
///
/// <param name="temp">Number to be formatted.</param>
/// <param name="width">Minimum width of field.</param>
/// <param name="dec">Number of decimal places (must be 0 for integral number
/// types).</param>
/// <param name="useComma">True if commas should be inserted.</param>
/// <param name="zeroFill">True if should zero fill on left.</param>
template <typename T>
std::string pout( T temp, int width, int dec, bool useComma, bool zeroFill )
{
  static const char *floatName = typeid(float).name();
  static const char *doubleName = typeid(double).name();
  static const char *longDoubleName = typeid(long double).name();
  OSTREAM start; //// string to be returned

  std::string type(typeid(temp).name());
  bool isFloatType = (type == floatName || type == doubleName ||
    type == longDoubleName) ? true : false;
#if !defined(NDEBUG)
  // For all integer types, dec should be 0.
  if (!isFloatType && dec != 0)
    throw PiaException(PIA_IDS_COMMA);
#endif

  int sign = (temp < 0) ? 1 : 0;  //// indicator for negative sign
  int point = (dec == 0) ? 0 : 1; //// indicator for decimal point
  if (isFloatType) {
    start.setf(std::ios::fixed, std::ios::floatfield);
    start.precision(dec);
    start << std::setw(dec+point);
  }
  start << temp;
  ENDS;
  std::string output(start.str());
  FREEZE(start);
  if (useComma)
    pcomma(output, dec, point, sign);
  pblank(output, width, zeroFill);
  return(output);
}

/// <summary>Wrapper function to convert any type of number to a string
/// with commas and a given number of decimal places.</summary>
///
/// <remarks>Wrapper function to convert any type of number to a string
/// with commas and a given number of decimal places.  The template
/// type T is the numerical type to format.</remarks>
///
/// <returns>Formatted string.</returns>
///
/// <exception cref="PiaException">PiaException of type
/// <see cref="PIA_IDS_COMMA"/> if number of decimals is
/// nonzero for other than a float type (only in debug mode).</exception>
///
/// <param name="temp">Number to be formatted.</param>
/// <param name="width">Minimum width of field.</param>
/// <param name="dec">Number of decimal places (must be 0 for integral
/// number types).</param>
/// <param name="zeroFill">True if should zero fill on left.</param>
template <typename T>
inline std::string poutComma( T temp, int width = 1, int dec = 0,
bool zeroFill = false )
{ return pout(temp, width, dec, true, zeroFill); }

/// <summary>Wrapper function to convert any type of number to a string
/// (without commas and) with a given number of decimal places.</summary>
///
/// <remarks>Wrapper function to convert any type of number to a string
/// (without commas and) with a given number of decimal places.  The
/// template type T is the numerical type to format.</remarks>
///
/// <returns>Formatted string.</returns>
///
/// <exception cref="PiaException">PiaException of type
/// <see cref="PIA_IDS_COMMA"/> if number of decimals is
/// nonzero for other than a float type (only in debug mode).</exception>
///
/// <param name="temp">Number to be formatted.</param>
/// <param name="width">Minimum width of field.</param>
/// <param name="dec">Number of decimal places (must be 0 for
/// integral number types).</param>
/// <param name="zeroFill">True if should zero fill on left.</param>
template <typename T>
inline std::string poutNoComma( T temp, int width = 1, int dec = 0,
bool zeroFill = false )
{ return pout(temp, width, dec, false, zeroFill); }
