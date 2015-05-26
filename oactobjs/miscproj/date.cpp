// Functions for the static <see cref="Date"/> class to manage date and
// time.
//
// $Id: date.cpp 1.20 2011/08/11 16:23:33EDT 044579 Development  $

#include "date.h"
#include "Resource.h"
#include "PiaException.h"
#include "boost/date_time/gregorian/greg_date.hpp"

using namespace std;

// <summary>Lowest year that is acceptable (1800).</summary>
unsigned Date::lowyear = 1800;
// <summary>Highest year that is acceptable (2100).</summary>
unsigned Date::highyear = 2100;

/// <summary>Checks for month in bounds.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DATEMONTH"/> if bad month.</exception>
///
/// <param name="month">Month to check.</param>
void Date::monthCheck( unsigned month )
{
   if (month < boost::date_time::Jan || month > boost::date_time::Dec)
      throw PiaException(PIA_IDS_DATEMONTH);
}

/// <summary>Checks for year in bounds.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DATEYEAR"/> if bad year.</exception>
///
/// <param name="year">Year to check.</param>
void Date::yearCheck( unsigned year )
{
   if (year < lowyear || year > highyear)
      throw PiaException(PIA_IDS_DATEYEAR);
}

/// <summary>Adds years to the specified date.</summary>
///
/// <returns>The incremented date.</returns>
///
/// <remarks>The years could be negative. A day of February 29 is changed to
/// February 28 to be sure there is no leap-year problem.</remarks>
///
/// <param name="theDate">The date to add to.</param>
/// <param name="years">The number of years to add.</param>
boost::gregorian::date Date::addYears( boost::gregorian::date theDate,
 int years )
{
   const unsigned short month = theDate.month();
   const unsigned short day = theDate.day();
   const unsigned short day1 = (month == boost::date_time::Feb && day == 29) ?
      28 : day;
   return boost::gregorian::date(
      static_cast<unsigned short>(theDate.year() + years), month, day1);
}
