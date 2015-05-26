// Declarations for the <see cref="PunctFacet"/> structure to handle numeric
// punctuation.
//
// $Id: NumPunct.h 1.5 2011/08/12 08:46:49EDT 044579 Development  $

#pragma once

#include <locale>
#include "oactcnst.h"

/// <summary>Numeric punctuation facet with 3 digits per thousands separator.
/// </summary>
///
/// <remarks>Numbers are printed with comma separators. Therefore, we store
/// string representations of all the possible years used in our work so they
/// can be printed without comma separators.
///
/// An instance of this structure is created in <see cref="PageOut"/>.
/// </remarks>
struct PunctFacet : std::numpunct<char>
{
  /// <summary>Returns specification of 3 digits per thousands separator.
  /// </summary>
  ///
  /// <returns>Specification of 3 digits per thousands separator.</returns>
  std::string do_grouping() const { return "\3"; }
  /// <summary>String representations of all the years in the output range.
  /// </summary>
  static const std::string yearStr[YEAR2100 - YEAR37 + 1];
  /// <summary>Returns a string representation of the specified year.
  /// </summary>
  ///
  /// <returns>A string representation of the specified year.</returns>
  ///
  /// <param name="year">The year to convert to a string.</param>
  static std::string getYear( int year ) { return yearStr[year - YEAR37]; }
};
