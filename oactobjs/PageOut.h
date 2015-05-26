// Declarations for the <see cref="PageOut"/> class - parent of classes to
// manage a page of printed output.

// $Id: PageOut.h 1.18 2011/08/19 15:57:45EDT 044579 Development  $

#pragma once

#include <sstream>
#include <string>
#include <vector>
#include "NumPunct.h"
class Outfile;

/// <summary>Abstract parent of classes to manage a page of printed output.
/// </summary>
///
/// <remarks>This is the parent of each page of printed output in the Anypia
/// program.</remarks>
class PageOut
{
public:
  /// <summary>The maximum number of lines on a page.</summary>
  static const int MAXLINES = 100;
protected:
  /// <summary>Strings for output.</summary>
  std::vector<std::string> outputString;
  /// <summary>Numeric punctuation facet with 3 digits per thousands
  /// separator.</summary>
  std::numpunct<char> *punct;
  /// <summary>A stream to insert commas in a formatted number.</summary>
  std::ostringstream strm;
  /// <summary>A locale used to imbue <see cref="PageOut::strm"/>.</summary>
  const std::locale loc;
public:
  PageOut();
  virtual ~PageOut();
  /// <summary>Returns number of lines of output.</summary>
  ///
  /// <returns>Number of lines of output.</returns>
  unsigned getCount() const { return (unsigned)outputString.size(); }
  /// <summary>Returns one line of output.</summary>
  ///
  /// <returns>The specified line of output.</returns>
  ///
  /// <param name="index">Index of desired line (0-based).</param>
  std::string getString( unsigned index ) const
    { return outputString[index]; }
  /// <summary>Prepares strings for output.</summary>
  virtual void prepareStrings() = 0;
  void print( Outfile& outfile ) const;
  void print( std::ostream& ostrm ) const;
};
