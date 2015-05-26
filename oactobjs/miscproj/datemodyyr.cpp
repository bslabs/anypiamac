// $Id: datemodyyr.cpp,v 1.8 2010/08/16 18:16:53 djolson Exp $
//
// Functions to handle date as month, day, and year.

#include <vector>
#include <string>
#include <ctime>
#include "datemodyyr.h"
#include "datemoyr.h"
#include "comma.h"
#include "StringParser.h"
#include "Resource.h"
#include "PiaException.h"

using namespace std;

/// <summary>Array of the first day of each month, as day of year.</summary>
const unsigned DateModyyr::monthFirstDays[12] = {
   1, 32, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };

/// <param name="dateMoyr">DateMoyr to copy from.</param>
DateModyyr::DateModyyr( const DateMoyr& dateMoyr ) :
monthOfYear(static_cast<unsigned char>(dateMoyr.getMonth())),
dayOfMonth(static_cast<unsigned char>(dateMoyr.getDay())),
yearPart(static_cast<unsigned short>(dateMoyr.getYear()))
{ }

/// <param name="dateTemp">String with date as mo/dy/year.</param>
DateModyyr::DateModyyr( const std::string& dateTemp )
{
   vector< string > strvec = parseString(dateTemp, '/');
   if (strvec.size() < 3)
      throw PiaException(PIA_IDS_DATEREAD);
   monthOfYear = static_cast<unsigned char>(atoi(strvec[0].c_str()));
   dayOfMonth = static_cast<unsigned char>(atoi(strvec[1].c_str()));
   yearPart = static_cast<unsigned short>(atoi(strvec[2].c_str()));
#ifndef NDEBUG
   check();
#endif
}

/// <summary>Compares (mo,day,year) dates.</summary>
///
/// <returns>True if this date is before argument date, false otherwise.</returns>
///
/// <param name="dateTemp">The date to compare to.</param>
bool DateModyyr::operator<( const DateModyyr& dateTemp ) const
{
   // check year
   if (yearPart > dateTemp.getYear())
      return(false);
   if (yearPart < dateTemp.getYear())
      return(true);
   // same year, so check month
   if (monthOfYear > dateTemp.getMonth())
      return(false);
   if (monthOfYear < dateTemp.getMonth())
      return(true);
   // same month and year, so check day
   if (dayOfMonth < dateTemp.getDay())
      return(true);
   else
      return(false);
}

/// <param name="dateMoyr">The date to copy from. Sets day to 1.</param>
DateModyyr& DateModyyr::operator=( const DateMoyr& dateMoyr )
{
   monthOfYear = static_cast<unsigned char>(dateMoyr.getMonth());
   dayOfMonth = static_cast<unsigned char>(dateMoyr.getDay());
   yearPart = static_cast<unsigned short>(dateMoyr.getYear());
   return *this;
}

/// <summary>Returns the date that is the day before this date.</summary>
///
/// <returns>The date that is the day before this date.</returns>
DateModyyr DateModyyr::getDayBefore() const
{
#ifndef NDEBUG
   check();
#endif
   unsigned month = getMonth();
   unsigned day = getDay() - 1;
   unsigned year = getYear();
   // take care of birth on 1st of month
   if (day < 1) {
      day = 30;
      month -= 1;
   }
   // take care of birth on January 1
   if (month < Date::JANUARY) {
      month = Date::DECEMBER;
      year -= 1;
   }
   return DateModyyr(month, day, year);
}

/// <summary>Returns string with formatted date.</summary>
///
/// <returns>String with date formatted as "month day, year".</returns>
std::string DateModyyr::toString() const
{
   string str("");

   try {
      check();
      str = Date::getMonthLine(monthOfYear);
      str += " ";
      str += poutNoComma(getDay());
      str += ", ";
      str += poutNoComma(getYear());
   } catch (PiaException&) {
      // ignore any exception (return empty string)
   }
   return(str);
}

/// <param name="format"> Format to use.  If none used, formatted as
///     "month day, year".
///
///     @table
///     value  meaning
///     "s"    format as "mo/dy/year" (could be 0/1/0).
///     "ss"   format as "mo/dy/year", with leading 0's if necessary.
///     "n"    format as "modyyear", with leading 0's if necessary.
///     other  format as "month day, year".</param>
std::string DateModyyr::toString( const std::string& format ) const
{
   if (format.length() < 1)
      return toString();
   try {
      string temp;
      switch (format[0]) {
      case 's':
         if (format.length() < 2 || format[1] != 's') {
            temp = poutNoComma(getMonth());
            temp += "/";
            temp += poutNoComma(getDay());
            temp += "/";
            temp += poutNoComma(getYear());
         }
         else {
            temp = poutNoComma(getMonth(), 2, 0, true);
            temp += "/";
            temp += poutNoComma(getDay(), 2, 0, true);
            temp += "/";
            temp += poutNoComma(getYear(), 4, 0, true);
         }
         return(temp);
      case 'n':
         temp = poutNoComma(getMonth(), 2, 0, true);
         temp += poutNoComma(getDay(), 2, 0, true);
         temp += poutNoComma(getYear(), 4, 0, true);
         return(temp);
      default:
         return toString();
      }
   } catch (PiaException&) {
      // ignore any exception (return empty string)
      return string("");
   }
}

/// <summary>Updates date with current date and time.</summary>
void DateModyyr::update()
{
   time_t clock;  // time in seconds
   struct tm *curtime;  // structure holding current time

   time(&clock);  // get time
   curtime = localtime(&clock);
   monthOfYear = static_cast<unsigned char>(curtime->tm_mon + 1);
   dayOfMonth = static_cast<unsigned char>(curtime->tm_mday);
   yearPart = static_cast<unsigned short>(curtime->tm_year + 1900);
}

/// <summary>Converts day of year to month (1-12).</summary>
///
/// <returns>Month (1-12).</returns>
///
/// <param name="days">Day of year (1-366).</param>
unsigned DateModyyr::daysToMonths( unsigned days )
{
#if !defined(NDEBUG)
   if (days == 0 || days > 366) {
      throw PiaException(PIA_IDS_DATEDAY);
   }
#endif
   unsigned rv = 0u;
   // Pass through the months earliest to latest.  Since the index is off
   // by 1, rv will be right at the end.
   while (rv < 12 && days >= monthFirstDays[rv]) {
      rv++;
   }
   return rv;
}

/// <summary>Converts day of year to day of the current month (1-31).</summary>
///
/// <returns>Day of the current month (1-31).</returns>
///
/// <param name="days">Day of year (1-366).</param>
unsigned DateModyyr::daysToMonthDay( unsigned days )
{
   unsigned rv = daysToMonths(days);
   return days - monthFirstDays[rv - 1] + 1;
}

/// <summary>Converts a month and day to a day of year (1-365).</summary>
///
/// <returns>Day of year (1-365).</returns>
///
/// <param name="mon">Month (1-12).</param>
/// <param name="dayOfMonth">Day (1-31).</param>
unsigned DateModyyr::dayOfYear( unsigned mon, unsigned dayOfMonth )
{
#if !defined(NDEBUG)
   Date::monthCheck(mon);
   dayCheck(dayOfMonth);
#endif
   return monthFirstDays[mon - 1] + dayOfMonth - 1;
}

/// <summary>Checks for day in bounds. Note that the version with an
///   argument is static.</summary>
///
/// <exception>PiaException of type PIA_IDS_DATEDAY if bad day.</exception>
void DateModyyr::dayCheck() const
{
   if (dayOfMonth < 1 || dayOfMonth > 31)
      throw PiaException(PIA_IDS_DATEDAY);
}

/// <param name="theDay">Day to check.</param>
void DateModyyr::dayCheck( unsigned theDay )
{
   if (theDay < 1 || theDay > 31)
      throw PiaException(PIA_IDS_DATEDAY);
}
