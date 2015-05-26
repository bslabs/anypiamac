// Declarations for the <see cref="BaseYear"/> class - parent of classes that
// manage the computation year for a pia program.
//
// $Id: baseyear.h 1.16 2011/07/28 11:50:15EDT 044579 Development  $

#pragma once

/// <summary>Parent of classes that manage the computation year for a pia
/// program.</summary>
///
/// <remarks>One of its subclasses should be instantiated:
/// <see cref="BaseYearFile"/> if it is to be gotten from disk,
/// <see cref="BaseYearNonFile"/> otherwise.</remarks>
///
/// <seealso cref="BaseYearFile"/>
/// <seealso cref="BaseYearNonFile"/>
class BaseYear
{
protected:
  /// <summary>Computation year.</summary>
  int theYear;
public:
  BaseYear();
  virtual ~BaseYear();
  /// <summary>Returns computation year.</summary>
  ///
  /// <returns>Computation year.</returns>
  int getYear() const { return(theYear); }
  /// <summary>Read base year.</summary>
  virtual void read() = 0;
  /// <summary>Sets computation year.</summary>
  ///
  /// <param name="newYear">New computation year to use.</param>
  void setYear( int newYear ) { theYear = newYear; }
  /// <summary>Save base year.</summary>
  virtual void write() = 0;
  static void yearCheck( int newYear );
};
