// Declarations for the <see cref="DateFormatter"/> class to format and parse
// dates.
//
// $Id: DateFormatter.h 1.7 2011/08/11 15:21:17EDT 044579 Development  $

#pragma once

#include "boost/date_time/gregorian/greg_date.hpp"
class DateMoyr;

/// <summary>Manages static functions to format and parse dates.</summary>
//
/// <remarks>The dates can be of type boost::gregorian::date for month, day,
/// and year, or <see cref="DateMoyr"/> for month and year.</remarks>
///
/// <seealso cref="DateMoyr"/>
class DateFormatter
{
public:
  static boost::gregorian::date date_from_undelimited_us_string(
    const std::string& line );
  static DateMoyr month_year_from_undelimited_us_string(
    const std::string& line );
  static std::string toString( const boost::gregorian::date& dateModyyr );
  static std::string toString( const boost::gregorian::date& dateModyyr,
    const std::string& formatModyyr );
  static std::string toString( const DateMoyr& dateMoyr );
  static std::string toString( const DateMoyr& dateMoyr,
    const std::string& formatMoyr );
};
