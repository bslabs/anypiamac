// Declarations for the <see cref="BiprojNonFile"/> class to manage projected
// benefit increases stored in arrays.
//
// $Id: BiprojNonFile.h 1.19 2011/07/29 11:25:02EDT 044579 Development  $

#pragma once

#include <string>
#include "biproj.h"

/// <summary>Manages the file of projected benefit increases stored in arrays.
/// </summary>
///
/// <remarks>See <see cref="BiprojFile"/> for a version of this class
/// that stores the benefit increases on disk.</remarks>
///
/// <seealso cref="BiprojFile"/>
class BiprojNonFile : public Biproj
{
public:
  /// <summary>Dimension limits.</summary>
  enum DimLimits {
    /// <summary>Number of sets of assumptions (4, for 1, 2, 3, and 2b).
    /// </summary>
    NUMASSUMSETS = 4,
    /// <summary>Number of annual benefit increases. This should not normally
    /// be changed because it is the number of blanks on a form in the
    /// detailed calculator.</summary>
    NUMPROJYEARS = 15
  };
private:
  /// <summary>Original titles for Trustees Report.</summary>
  static const std::string titletrOriginal[NUMASSUMSETS];
  /// <summary>Benefit increases for Trustees Report.</summary>
  static const double biprojtrOriginal[NUMASSUMSETS][NUMPROJYEARS];
  /// <summary>Ultimate benefit increases for Trustees Report.</summary>
  static const double biult[NUMASSUMSETS];
  /// <summary>Stored Trustees Report assumptions titles.</summary>
  std::string titletr[NUMASSUMSETS];
  /// <summary>Stored Trustees Report increases.</summary>
  DoubleAnnual *biprojtr[NUMASSUMSETS];
public:
  BiprojNonFile( int newIstart, int newMaxyear );
  ~BiprojNonFile();
  void read( int altNum );
  void setData ( int altNum );
  void setData ( int altNum, const DoubleAnnual& newData );
  void setTitle( int altNum );
  void setTitle( int altNum, const std::string& newTitle );
  void write( int );
};
