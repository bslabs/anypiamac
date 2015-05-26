// Declarations for the <see cref="CatchupDoc"/> class to manage a set of
// catch-up benefit increases.
//
// $Id: cachupf.h 1.17 2011/07/29 13:49:25EDT 044579 Development  $

#pragma once

#include "cachup.h"

/// <summary>Abstract parent of classes that manage a file of catch-up
/// benefit increases.</summary>
///
/// <remarks>To implement this class, use <see cref="CatchupFile"/> or
/// <see cref="CatchupNonFile"/>.</remarks>
///
/// <seealso cref="CatchupFile"/>
/// <seealso cref="CatchupNonFile"/>
class CatchupDoc
{
public:
  /// <summary>Catch-up benefit increases.</summary>
  Catchup catchup;
  /// <summary>True to allow nonzero catch-up increases.</summary>
  bool allownonzero[4];
public:
  CatchupDoc();
  explicit CatchupDoc( int newCstart );
  virtual ~CatchupDoc();
  /// <summary>Checks for existence of catch-up increase.</summary>
  ///
  /// <returns>True if specified catch-up increase exists, false otherwise.
  /// </returns>
  ///
  /// <param name="eligYear">Year of eligibility.</param>
  /// <param name="calYear">Calendar year of increase.</param>
  bool exist( int eligYear, int calYear ) const
  { return(catchup.exist(eligYear, calYear)); }
  /// <summary>Returns one catch-up increase.</summary>
  ///
  /// <returns>One catch-up increase.</returns>
  ///
  /// <param name="eligYear">Year of eligibility.</param>
  /// <param name="calYear">Calendar year of increase.</param>
  double get( int eligYear, int calYear ) const
  { return(catchup.get(eligYear, calYear)); }
  /// <summary>Reads catch-up increases.</summary>
  virtual void read( int ) = 0;
  /// <summary>Sets catch-up increases for one alternative.</summary>
  virtual void setData ( int ) = 0;
  /// <summary>Sets catch-up increases for one alternative.</summary>
  virtual void setData ( int, const Catchup& ) = 0;
  /// <summary>Writes catch-up increases.</summary>
  virtual void write( int ) = 0;
};
