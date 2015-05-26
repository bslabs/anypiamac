// Declarations for the static <see cref="Date"/> class to manage date and
// time.
//
// $Id: date.h 1.20 2011/08/11 15:21:17EDT 044579 Development  $

#pragma once

#include <string>
#include "boost/date_time/gregorian/greg_date.hpp"

/// <summary>Static variables and functions for classes that hold dates.
/// </summary>
///
/// <remarks>Static variables and functions for classes that hold dates as
/// month, day, and year (boost::gregorian::date).</remarks>
class Date
{
public:
  /// <summary>Lowest year that is acceptable (1800).</summary>
  static unsigned lowyear;
  /// <summary>Highest year that is acceptable (2100).</summary>
  static unsigned highyear;
private:
  Date();
  Date( const Date& );
  ~Date();
  Date& operator=( const Date& ) { return *this; }
public:
  static boost::gregorian::date addYears( boost::gregorian::date theDate,
    int years );
  static void monthCheck( unsigned month );
  static void yearCheck( unsigned year );
};
