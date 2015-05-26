// Declarations for the <see cref="FreezeYears"/> class to manage freeze period
// variables.
//
// $Id: frzyrs.h 1.11 2011/07/29 15:26:15EDT 044579 Development  $

#pragma once

/// <summary>Manages the years in a freeze period. The years may be wholly or
/// partially within a freeze period.</summary>
///
/// <remarks>Two instances of this class are used in <see cref="PiaData"/>.
/// </remarks>
class FreezeYears
{
private:
  /// <summary>First year in latest period of disability.</summary>
  int year1;
  /// <summary>Last year in latest period of disability.</summary>
  int year2;
  /// <summary>First year in prior period of disability.</summary>
  int year3;
  /// <summary>Last year in prior period of disability.</summary>
  int year4;
public:
  FreezeYears();
  /// <summary>Zeroes all variables.</summary>
  void deleteContents() { year1 = year2 = year3 = year4 = 0; }
  /// <summary>Returns first year in latest period of disability.</summary>
  ///
  /// <returns>First year in latest period of disability.</returns>
  int getYear1() const { return(year1); }
  /// <summary>Returns last year in latest period of disability.</summary>
  ///
  /// <returns>Last year in latest period of disability.</returns>
  int getYear2() const { return(year2); }
  /// <summary>Returns first year in prior period of disability.</summary>
  ///
  /// <returns>First year in prior period of disability.</returns>
  int getYear3() const { return(year3); }
  /// <summary>Returns last year in prior period of disability.</summary>
  ///
  /// <returns>Last year in prior period of disability.</returns>
  int getYear4() const { return(year4); }
  bool hasYears1() const;
  bool hasYears2() const;
  bool isFreezeYear ( int year ) const;
  /// <summary>Sets first year in latest period of disability.</summary>
  ///
  /// <param name="newYear1">New first year in latest period.</param>
  void setYear1( int newYear1 ) { year1 = newYear1; }
  /// <summary>Sets last year in latest period of disability.</summary>
  ///
  /// <param name="newYear2">New last year in latest period.</param>
  void setYear2( int newYear2 ) { year2 = newYear2; }
  /// <summary>Sets first year in prior period of disability.</summary>
  ///
  /// <param name="newYear3">New first year in prior period.</param>
  void setYear3( int newYear3 ) { year3 = newYear3; }
  /// <summary>Sets last year in prior period of disability.</summary>
  ///
  /// <param name="newYear4">New last year in prior period.</param>
  void setYear4( int newYear4 ) { year4 = newYear4; }
};
