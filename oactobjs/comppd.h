// Declarations for the <see cref="CompPeriod"/> class to manage computation
// period variables.
//
// $Id: comppd.h 1.9 2011/07/29 13:49:26EDT 044579 Development  $

#pragma once

/// <summary>Manages the number of elapsed years, dropout years, and
/// computation years.</summary>
///
/// <remarks>Two instances of this class are used in <see cref="PiaData"/>.
/// </remarks>
class CompPeriod
{
private:
  /// <summary>Base year (1936 (for old-start) or 1950 (for new-start)).
  /// </summary>
  int baseYear;
  /// <summary>Number of years wholly or partially within a period of
  /// disability, within the elapsed years.</summary>
  int diYears;
  /// <summary>Number of computation years.</summary>
  int n;
  /// <summary>Number of dropout years.</summary>
  int nDrop;
  /// <summary>Number of elapsed years.</summary>
  int nElapsed;
public:
  CompPeriod( int newBaseYear );
  void deleteContents();
  /// <summary>Returns base year (1936 or 1950).</summary>
  ///
  /// <returns>Base year (1936 or 1950).</returns>
  int getBaseYear() const { return(baseYear); }
  /// <summary>Returns number of years wholly or partially within a
  /// period of disability, within the elapsed years.</summary>
  ///
  /// <returns>Number of years wholly or partially within a
  /// period of disability, within the elapsed years.</returns>
  int getDiYears() const { return(diYears); }
  /// <summary>Returns number of computation years.</summary>
  ///
  /// <returns>Number of computation years.</returns>
  int getN() const { return(n); }
  /// <summary>Returns number of dropout years.</summary>
  ///
  /// <returns>Number of dropout years.</returns>
  int getNDrop() const { return(nDrop); }
  /// <summary>Returns number of elapsed years.</summary>
  ///
  /// <returns>Number of elapsed years.</returns>
  int getNElapsed() const { return(nElapsed); }
  /// <summary>Sets number of years wholly or partially within a
  /// period of disability, within the elapsed years.</summary>
  ///
  /// <param name="newDiYears">New number of disability years.</param>
  void setDiYears( int newDiYears ) { diYears = newDiYears; }
  /// <summary>Sets number of computation years.</summary>
  ///
  /// <param name="newN">New number of computation years.</param>
  void setN( int newN ) { n = newN; }
  /// <summary>Sets number of dropout years.</summary>
  ///
  /// <param name="newNDrop">New number of dropout years.</param>
  void setNDrop( int newNDrop ) { nDrop = newNDrop; }
  /// <summary>Sets number of elapsed years.</summary>
  ///
  /// <param name="newNElapsed">New number of elapsed years.</param>
  void setNElapsed( int newNElapsed ) { nElapsed = newNElapsed; }
};
