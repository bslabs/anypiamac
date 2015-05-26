// Declarations for the <see cref="Biproj"/> class to manage projected benefit
// increases.
//
// $Id: biproj.h 1.24 2011/07/29 11:25:03EDT 044579 Development  $

#pragma once

#include "dbleann.h"

/// <summary>Abstract parent of classes that manage the file of projected
/// benefit increases.</summary>
///
/// <remarks>One of its subclasses should be instantiated:
/// <see cref="BiprojFile"/> if it is to be gotten from disk,
/// <see cref="BiprojNonFile"/> otherwise.</remarks>
///
/// <seealso cref="BiprojFile"/>
/// <seealso cref="BiprojNonFile"/>
class Biproj
{
private:
  /// <summary>First year of data.</summary>
  int firstYear;
  /// <summary>Title of old PEBES assumptions.</summary>
  static const char* beninc1;
  /// <summary>Title of "other" assumptions.</summary>
  static const char* otherTitle;
protected:
  /// <summary>Title of projected benefit increases.</summary>
  std::string biprojTitle;
  /// <summary>Titles of projected benefit increase assumptions, other
  /// than the four TR assumptions.</summary>
  std::string beninc[3];
public:
  /// <summary>Array of projected benefit increases.</summary>
  DoubleAnnual theData;
public:
  Biproj( int newIstart, int newMaxyear );
  virtual ~Biproj();
  /// <summary>Returns first year of data.</summary>
  //
  /// <returns>First year of data.</returns>
  int getFirstYear() const { return firstYear; }
  /// <summary>Returns title of assumptions.</summary>
  //
  /// <returns>Title of assumptions.</returns>
  const std::string& getTitle() const { return biprojTitle; }
  /// <summary>Read benefit increases.</summary>
  virtual void read( int ) = 0;
  void setCpiinc( int year, double newCpiinc );
  /// <summary>Set benefit increases for one alternative.</summary>
  virtual void setData( int ) = 0;
  /// <summary>Set benefit increases for one alternative.</summary>
  virtual void setData( int, const DoubleAnnual& ) = 0;
  void setFirstYear( int newFirstYear );
  /// <summary>Sets title for one alternative.</summary>
  virtual void setTitle( int ) = 0;
  /// <summary>Sets title for one alternative.</summary>
  virtual void setTitle( int, const std::string& ) = 0;
  /// <summary>Save benefit increases.</summary>
  virtual void write( int ) = 0;
  static void cpiincCheck( double cpiinct );
};
