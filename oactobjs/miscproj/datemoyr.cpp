// Functions needed for the <see cref="DateMoyr"/> class to handle date as
// month and year.

// $Id: datemoyr.cpp 1.40 2010/12/09 16:51:43EST 044579 Development  $

#include <vector>
#include <string>
#include "datemoyr.h"
#include "age.h"
#include "Resource.h"
#include "PiaException.h"
#include "StringParser.h"

using namespace std;

/// <summary>Initializes the month and year from a boost::gregorian::date.
/// </summary>
///
/// <param name="dateModyyr">The date to copy from (month, day, and year).
/// </param>
DateMoyr::DateMoyr( const boost::gregorian::date& dateModyyr ) :
monthOfYear(static_cast<unsigned short>(dateModyyr.month())),
yearPart(static_cast<unsigned short>(dateModyyr.year()))
{ }

/// <summary>Initializes the month and year by parsing a date string (as
/// mo/year).</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DATEREAD"/> if unable to set month and year, or if they
/// have invalid values.</exception>
///
/// <param name="dateTemp">The date string to parse (as mo/year).</param>
DateMoyr::DateMoyr( const std::string& dateTemp )
{
  vector< string > strvec = parseString(dateTemp, '/');
  if (strvec.size() < 2)
    throw PiaException(PIA_IDS_DATEREAD);
  monthOfYear = static_cast<unsigned short>(atoi(strvec[0].c_str()));
  yearPart = static_cast<unsigned short>(atoi(strvec[1].c_str()));
#ifndef NDEBUG
  check();
#endif
}

/// <summary>Compares this date to another one.</summary>
///
/// <returns>True if this date is before argument date.</returns>
///
/// <param name="dateMoyr">The date to compare to.</param>
bool DateMoyr::operator<( const DateMoyr& dateMoyr ) const
{
  // check year
  if (yearPart > dateMoyr.getYear())
    return(false);
  if (yearPart < dateMoyr.getYear())
    return(true);
  // same year, so check month
  if (monthOfYear < dateMoyr.getMonth())
    return(true);
  // same year and month is not less than specified month
  return(false);
}

/// <summary>Sets month and year from a boost::gregorian::date (month, day, and
/// year).</summary>
///
/// <returns>A reference to this instance.<returns>
///
/// <remarks>This version ignores the day.</remarks>
///
/// <param name="dateModyyr">The date to copy from (month, day, and year).
///  Ignores the day.</param>
DateMoyr& DateMoyr::operator=( const boost::gregorian::date& dateModyyr )
{
  monthOfYear = static_cast<unsigned short>(dateModyyr.month());
  yearPart = static_cast<unsigned short>(dateModyyr.year());
  return *this;
}

/// <summary>Checks for valid month and year.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DATEMONTH"/> or <see cref="PIA_IDS_DATEYEAR"/>
/// if bad month or year.</exception>
void DateMoyr::check() const
{
  Date::monthCheck(monthOfYear);
  Date::yearCheck(yearPart);
}

/// <summary>Calculates a date by adding an <see cref="Age"/>.</summary>
///
/// <returns>The instance date plus the argument age.</returns>
///
/// <param name="age">The <see cref="Age"/> to add to this date.</param>
DateMoyr DateMoyr::operator+( const Age& age ) const
{
  unsigned month = monthOfYear + age.getMonths();
  unsigned year = yearPart + age.getYears();
  if (month > 12) {
    month -= 12;
    year += 1;
  }
  return DateMoyr(month, year);
}

/// <summary>Calculates an <see cref="Age"/> by subtracting argument date from
/// this one.</summary>
///
/// <returns>The calculated <see cref="Age"/>.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> if argument is
/// greater than this instance.</exception>
///
/// <remarks>This would typically be used when this instance is the date of
/// entitlement to a benefit and the argument is the date of birth.</remarks>
///
/// <param name="dateMoyr">The date to subtract from this instance.</param>
Age DateMoyr::operator-( const DateMoyr& dateMoyr ) const
{
  if (monthOfYear >= dateMoyr.getMonth()) {
    if (yearPart < dateMoyr.getYear()) {
      throw PiaException("Dates out of order in operator-");
    }
    return Age(yearPart - dateMoyr.getYear(),
      monthOfYear - dateMoyr.getMonth());
  }
  else {
    if (yearPart <= dateMoyr.getYear()) {
      throw PiaException("Dates out of order in operator-");
    }
    return Age(yearPart - dateMoyr.getYear() - 1,
      12u + monthOfYear - dateMoyr.getMonth());
  }
}

/// <summary>Returns number of months from this date to given date.</summary>
///
/// <returns>Number of months from this date to given date. Result is negative
///   if argument date is before this date.</returns>
///
/// <param name="dateMoyr">Given date.</param>
int DateMoyr::getMonths( const DateMoyr& dateMoyr ) const
{
  const int i1 = 12 * yearPart + monthOfYear;
  const int i2 = 12 * dateMoyr.getYear() + dateMoyr.getMonth();
  return(i2 - i1);
}

/// <summary>Returns the date that is the month before this date.</summary>
///
/// <returns>The month and year that is the month before this date.</returns>
DateMoyr DateMoyr::getMonthBefore() const
{
#ifndef NDEBUG
  check();
#endif
  unsigned month = getMonth() - 1;
  unsigned year = getYear();
  // take care of January dates
  if (month < boost::date_time::Jan) {
    month = boost::date_time::Dec;
    year -= 1;
  }
  return DateMoyr(month, year);
}
