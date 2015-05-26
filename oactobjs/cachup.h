// Declarations for the <see cref="CatchupIncreases"/> and
// <see cref="Catchup"/> classes to manage catch-up benefit increases.
//
// $Id: cachup.h 1.18 2011/07/29 13:49:26EDT 044579 Development  $

#pragma once

#include <string>

/// <summary>Manages the catch-up benefit increases for one year of
/// eligibility.</summary>
///
/// <remarks>There have not been any catch-up benefit increases to date;
/// any catch-up increases used would be projected ones.</remarks>
class CatchupIncreases
{
public:
  /// <summary>The number of catch-up increases for one year of
  /// eligibility (8).</summary>
  static const int NUM_CATCHUP_INCREASES = 8;
protected:
  /// <summary>Catch-up benefit increases for 8 calendar years.</summary>
  double theData[NUM_CATCHUP_INCREASES];
public:
  CatchupIncreases();
  ~CatchupIncreases();
  /// <summary>Returns one catch-up increase.</summary>
  ///
  /// <returns>One catch-up increase.</returns>
  ///
  /// <param name="yearIndex">Index of calendary year of increase.</param>
  double get( int yearIndex ) const { return(theData[yearIndex]); }
  /// <summary>Sets one catch-up increase.</summary>
  ///
  /// <param name="yearIndex">Index of calendar year of increase.</param>
  /// <param name="newCatchup">New value of catch-up increase.</param>
  void set( int yearIndex, double newCatchup )
  { theData[yearIndex] = newCatchup; }
  void parseString( std::string str );
  std::string toString() const;
};

/// <summary>Manages the catch-up benefit increases required to calculate a
/// Social Security benefit.</summary>
///
/// <remarks>There have not been any catch-up benefit increases to date;
/// any catch-up increases used would be projected ones.</remarks>
class Catchup
{
public:
  /// <summary>The number of years of eligibility considered (10).</summary>
  static const int NUM_CATCHUP_ELIG_YEARS = 10;
private:
  /// <summary>Starting year of catch-up benefit increases.</summary>
  int cstart;
protected:
  /// <summary>Catch-up benefit increases for 10 years of eligibility.
  /// </summary>
  ///
  /// <remarks>Array index is year of eligibility, measured from the starting
  /// year (0 to 9). For a given array index, calendar year of increase in
  /// catchup goes from the starting year plus 2 to the starting year plus 9.
  /// </remarks>
  CatchupIncreases catchup[NUM_CATCHUP_ELIG_YEARS];
public:
  Catchup();
  explicit Catchup( int newCstart );
  ~Catchup();
  void deleteContents();
  bool exist( int eligYear, int calYear ) const;
  /// <summary>Returns one catch-up increase.</summary>
  ///
  /// <returns>One catch-up increase.</returns>
  ///
  /// <param name="eligYear">Year of eligibility.</param>
  /// <param name="calYear">Calendary year of increase.</param>
  double get( int eligYear, int calYear ) const
  { return((eligYear >= cstart && eligYear < cstart + NUM_CATCHUP_ELIG_YEARS
    && calYear >= cstart + 2 &&
    calYear < cstart + 2 + CatchupIncreases::NUM_CATCHUP_INCREASES) ?
    catchup[eligYear-cstart].get(calYear-cstart-2) : 0.0); }
  /// <summary>Returns a <see cref="CatchupIncreases"/> array.</summary>
  ///
  /// <returns>A <see cref="CatchupIncreases"/> array.</returns>
  ///
  /// <param name="index">Index of desired array (0-9).</param>
  const CatchupIncreases& getCatchupIncreases( int index ) const
  { return(catchup[index]); }
  /// <summary>Gets first year of catch-up increases.</summary>
  ///
  /// <returns>First year of catch-up increases.</returns>
  int getCstart() const { return(cstart); }
  void parseString( int index, std::string inputLine );
  void set( int eligYear, int calYear, double newCatchup );
  /// <summary>Sets first year of catch-up increases.</summary>
  ///
  /// <param name="newCstart">New first year of catch-up increases.</param>
  void setCstart( int newCstart ) { cstart = newCstart; }
  static void check( double newCatchup );
};
