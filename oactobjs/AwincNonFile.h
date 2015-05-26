// Declarations for the <see cref="AwincNonFile"/> class to manage average
// wage increases stored in static arrays.

// $Id: AwincNonFile.h 1.17 2011/07/28 11:50:14EDT 044579 Development  $

#pragma once

#include "AwincDoc.h"

/// <summary>Manages the file of projected average wage increases stored in
/// static arrays.</summary>
///
/// <remarks>See <see cref="AwincFile"/> for a version of this class
/// that stores the average wage increases on disk.</remarks>
///
/// <seealso cref="AwincFile"/>
class AwincNonFile : public AwincDoc
{
public:
  /// <summary>Dimension limits.</summary>
  enum DimLimits {
    /// Number of sets of assumptions (4 for 1, 2, 3, and 2b).
    NUMASSUMSETS = 4,
    /// Number of annual average wage increases. This should not normally be
    /// changed because it is the number of blanks on a form in the
    /// detailed calculator.
    NUMPROJYEARS = 16
  };
private:
  /// <summary>Historical average wage increases.</summary>
  static const double awincHist[];
  /// <summary>Original titles for Trustees Report.</summary>
  static const std::string titletrOriginal[NUMASSUMSETS];
  /// <summary>Average wage increases for Trustees Report.</summary>
  static const double awincprojtrOriginal[NUMASSUMSETS][NUMPROJYEARS];
  /// <summary>Ultimate average wage increases for Trustees Report.</summary>
  static const double awincult[NUMASSUMSETS];
  /// <summary>Stored Trustees Report assumptions titles.</summary>
  std::string titletr[NUMASSUMSETS];
  /// <summary>Vectors of stored Trustees Report assumptions.</summary>
  DoubleAnnual *awincprojtr[NUMASSUMSETS];
public:
  AwincNonFile( int newIstart, int newMaxyear );
  ~AwincNonFile();
  void read( int altNum );
  void setData ( int altNum );
  virtual void setData( int altNum, const DoubleAnnual& newData );
  void setTitle( int altNum );
  void setTitle( int altNum, const std::string& newTitle );
  void write( int );
};
