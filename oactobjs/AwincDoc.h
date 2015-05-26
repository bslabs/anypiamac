// Declarations for the <see cref="AwincDoc"/> class to manage average wage
// increases.

// $Id: AwincDoc.h 1.19 2011/07/28 11:50:14EDT 044579 Development  $

#pragma once

#include "awinc.h"

/// <summary>Abstract parent of classes that manage the file of projected
/// average wage increases.</summary>
///
/// <remarks>One of its subclasses should be instantiated:
/// <see cref="AwincFile"/> if it is to be retrieved from disk,
/// <see cref="AwincNonFile"/> otherwise.</remarks>
///
/// <seealso cref="AwincFile"/>
/// <seealso cref="AwincNonFile"/>
class AwincDoc
{
private:
  /// <summary>First year of data.</summary>
  int firstYear;
  /// <summary>Title of "other" assumptions.</summary>
  static const char* otherTitle;
  /// <summary>Title of old PEBES assumptions.</summary>
  static const char* avgwage1;
protected:
  /// <summary>Title of projected average wage increases.</summary>
  std::string awincTitle;
  /// <summary>Titles of projected average wage increase assumptions, other
  /// than the four TR assumptions.</summary>
  std::string avgwage[3];
public:
  /// <summary>Array of average wage increases.</summary>
  Awinc awinc;
public:
  AwincDoc( int newIstart, int newMaxyear );
  virtual ~AwincDoc();
  /// <summary>Returns first year of data.</summary>
  ///
  /// <returns>First year of data.</returns>
  int getFirstYear() const { return firstYear; }
  /// <summary>Returns title of assumptions.</summary>
  ///
  /// <returns>Title of assumptions.</returns>
  const std::string& getTitle() const { return awincTitle; }
  /// <summary>Read data.</summary>
  virtual void read( int ) = 0;
  /// <summary>Set data for one alternative.</summary>
  virtual void setData( int ) = 0;
  /// <summary>Set data for one alternative.</summary>
  virtual void setData( int, const DoubleAnnual& ) = 0;
  void setFirstYear( int newFirstYear );
  void setFqinc( int year, double newFqinc );
  /// <summary>Sets title for one alternative.</summary>
  virtual void setTitle( int ) = 0;
  /// <summary>Sets title for one alternative.</summary>
  virtual void setTitle( int, const std::string& ) = 0;
  /// <summary>Write data.</summary>
  virtual void write( int ) = 0;
};
