// Declarations needed for <see cref="DateMoyr"/> class to manage date as
// month and year.

// $Id: datemoyr.h 1.37 2010/12/09 16:51:43EST 044579 Development  $

#pragma once

#include "date.h"
#include "boost/serialization/access.hpp"
#include "boost/date_time/gregorian/greg_date.hpp"
class Age;

/// <summary>Manages a month and year stored as unsigned integers.</summary>
///
/// <remarks>The two data fields are stored as unsigned shorts to save space.
/// The month is 1 to 12, while the year is 1 to 65000. If the date is
/// uninitialized (tested by <see cref="DateMoyr::isNotADate"/>, the month and
/// year are both zero.</remarks>
class DateMoyr
{
  /// <summary>Friend class to allow serialization.</summary>
  friend class boost::serialization::access;
  // <summary>Archives the data.</summary>
  //
  // <remarks>When the class Archive corresponds to an output archive, the
  // & operator is defined similar to <<. Likewise, when the class Archive is
  // a type of input archive, the & operator is defined
  // similar to >>.</remarks>
  //
  // <param name="ar">The archive to use.</param>
  // <param name="version">The version of the archive to use.</param>
  template< class Archive >
  void serialize( Archive& ar, const unsigned int version )
  { ar & monthOfYear & yearPart; }
private:
  /// <summary>Month of year (1-12).</summary>
  unsigned short monthOfYear;
  /// <summary>The year (1-65000).</summary>
  unsigned short yearPart;
public:
  /// <summary>Initializes the month and year to zero.</summary>
  DateMoyr() : monthOfYear(0), yearPart(0) { }
  /// <summary>Initializes the month and year from another
  /// <see cref="DateMoyr"/>.</summary>
  ///
  /// <param name="dateMoyr">The date to copy from (month and year).</param>
  DateMoyr( const DateMoyr& dateMoyr ) :
  monthOfYear(dateMoyr.monthOfYear), yearPart(dateMoyr.yearPart) { }
  DateMoyr( const boost::gregorian::date& dateModyyr );
  /// <summary>Initializes the month and year from specified values.</summary>
  ///
  /// <param name="newMonth">New month of year (1-12).</param>
  /// <param name="newYear">New year.</param>
  DateMoyr( unsigned newMonth, unsigned newYear ) :
    monthOfYear((unsigned short)newMonth),
    yearPart((unsigned short)newYear) { }
  DateMoyr( const std::string& dateTemp );
  /// <summary>Destroys date.</summary>
  ~DateMoyr() { }
  bool operator<( const DateMoyr& dateMoyr ) const;
  /// <summary>Compares this date to another one.</summary>
  ///
  /// <returns>True if this date equals argument date.</returns>
  ///
  /// <param name="dateMoyr">The date to compare to.</param>
  bool operator==( const DateMoyr& dateMoyr ) const
  { return (yearPart == dateMoyr.getYear() &&
    monthOfYear == dateMoyr.getMonth()); }
  /// <summary>Sets month and year from another <see cref="DateMoyr"/>.
  /// </summary>
  ///
  /// <returns>A reference to this instance.<returns>
  ///
  /// <param name="dateMoyr">The date to copy from (month and year).</param>
  DateMoyr& operator=( const DateMoyr& dateMoyr )
  { monthOfYear = dateMoyr.monthOfYear; yearPart = dateMoyr.yearPart;
    return *this; }
  DateMoyr& operator=( const boost::gregorian::date& dateModyyr );
  DateMoyr operator+( const Age& age ) const;
  Age operator-( const DateMoyr& dateMoyr ) const;
  void check() const;
  /// <summary>Zeroes month and year.</summary>
  void deleteContents() { monthOfYear = yearPart = 0; }
  int getMonths( const DateMoyr& dateMoyr ) const;
  /// <summary>Returns month of year.</summary>
  ///
  /// <returns>Month of year (1-12).</returns>
  unsigned short getMonth() const { return monthOfYear; }
  DateMoyr getMonthBefore() const;
  /// <summary>Returns the year.</summary>
  ///
  /// <returns>The year.</returns>
  unsigned getYear() const { return yearPart; }
  /// <summary>Returns index of the month and year, based on January 1971
  ///   being 0.</summary>
  ///
  /// <returns>Index of the month and year, based on January 1971 being 0.
  /// </returns>
  int index() const
  { return(12 * (yearPart - 1971) + monthOfYear - boost::date_time::Jan); }
  /// <summary>Returns true if the default values of 0 for the year and 
  ///   month are present, which shows that it has never been set.</summary>
  ///
  /// <returns>A bool that says whether the default (invalid) values are
  ///   present.</returns>
  bool isNotADate() const { return (monthOfYear == 0 && yearPart == 0); }
  /// <summary>Checks for month in bounds.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_DATEMONTH"/> if month is out of range.</exception>
  void monthCheck() const { Date::monthCheck(monthOfYear); }
};
