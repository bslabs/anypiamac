// $Id: datemodyyr.h,v 1.8 2010/11/08 15:02:37 djolson Exp $
//
// Declarations for class to manage date as month, day, and year.

#pragma once

#include "date.h"
#include "boost/serialization/access.hpp"
class DateMoyr;

// {group:Miscellaneous Classes}
/// <summary>Manages a month, day, and year stored as unsigned
///  integers.</summary>
///
/// <remarks>The month and day are stored as unsigned chars, and
///   the year is stored as an unsigned short, to save space.</remarks>
class DateModyyr
{
   friend class boost::serialization::access;
   /// <summary>Archives the data.</summary>
   ///
   /// <remarks>When the class Archive corresponds to an output archive, the
   /// & operator is defined similar to <<. Likewise, when the class Archive is
   /// a type of input archive, the & operator is defined similar
   ///   to >>.</remarks>
   ///
   /// <param name="ar">The archive to use.</param>
   /// <param name="version">The version of the archive to use.</param>
   template< class Archive >
   void serialize( Archive& ar, const unsigned int version )
      { ar & monthOfYear & dayOfMonth & yearPart; }
public:
   /// <summary>Array of the first day of each month, as day of year.</summary>
   static const unsigned monthFirstDays[12];
protected:
   /// <summary>Month of year (1-12).</summary>
   unsigned char monthOfYear;
   /// <summary>Day of month (1-31).</summary>
   unsigned char dayOfMonth;
   /// <summary>The year (1-65000).</summary>
   unsigned short yearPart;
public:
   /// <summary>Constructor.</summary>
   ///
   /// <remarks>The default constructor initializes the month, day, and year to
   /// zero. The version with a string argument expects a string with date as
   /// mo/dy/year.</remarks>
   ///
   /// <exception cref="PiaException">PiaException of type
   /// <see cref="PIA_IDS_DATEREAD"/> if unable to set month and
   /// year, or if they have invalid values.</exception>
   DateModyyr() : monthOfYear(0), dayOfMonth(0), yearPart(0) { }
   /// <param name="dateModyyr">DateModyyr to copy from.</param>
   DateModyyr( const DateModyyr& dateModyyr ) :
      monthOfYear(dateModyyr.monthOfYear), dayOfMonth(dateModyyr.dayOfMonth),
      yearPart(dateModyyr.yearPart) { }
   DateModyyr( const DateMoyr& dateMoyr );
   /// <param name="newModayyr">The month, day, and year in an array.</param>
   DateModyyr( unsigned newModayyr[3]) :
      monthOfYear((unsigned char)newModayyr[0]),
      dayOfMonth((unsigned char)newModayyr[1]),
      yearPart((unsigned short)newModayyr[2]) { }
   /// <param name="newMonth">New month of year (1-12).</param>
   /// <param name="newDay">New day of month (1-31).</param>
   /// <param name="newYear">New year.</param>
   DateModyyr( unsigned newMonth, unsigned newDay, unsigned newYear ) :
      monthOfYear((unsigned char)newMonth), dayOfMonth((unsigned char)newDay),
      yearPart((unsigned short)newYear) { }
   DateModyyr( const std::string& dateTemp );
   /// <summary>Destroys date.</summary>
   ~DateModyyr() { }
   bool operator<( const DateModyyr& dateTemp ) const;
   /// <summary>Compares (mo,day,year) dates.</summary>
   ///
   /// <returns>True if this date equals the argument date,
   ///   false otherwise.</returns>
   ///
   /// <param name="dateTemp">The date to compare to.</param>
   bool operator==( const DateModyyr& dateTemp ) const
      { return (yearPart == dateTemp.getYear() &&
        monthOfYear == dateTemp.getMonth() &&
        dayOfMonth == dateTemp.getDay()); }
   /// <summary>Sets month, day, and year.</summary>
   ///
   /// <returns>This date.</returns>
   ///
   /// <remarks>The version that copies from a DateMoyr sets the
   ///   day to 1.</remarks>
   ///
   /// <param name="dateTemp">The date to copy from.</param>
   DateModyyr& operator=( const DateModyyr& dateTemp )
      { monthOfYear = (unsigned char)dateTemp.getMonth();
        dayOfMonth = (unsigned char)dateTemp.getDay();
        yearPart = (unsigned short)dateTemp.getYear(); return *this; }
   DateModyyr& operator=( const DateMoyr& dateMoyr );
   /// <summary>Checks for month, day, and year in bounds.</summary>
   ///
   /// <exception cref="PiaException">PiaException of type
   /// <see cref="PIA_IDS_DATEMONTH"/>, <see cref="PIA_IDS_DATEDAY, or
   /// <see cref="PIA_IDS_DATEYEAR"/> if bad month, bad day, or bad year.
   /// </exception>
   void check() const
      { Date::monthCheck(monthOfYear); dayCheck(); Date::yearCheck(yearPart); }
   void dayCheck() const;
   static void dayCheck( unsigned dy );
   static unsigned dayOfYear( unsigned mon, unsigned dayOfMonth );
   static unsigned daysToMonthDay( unsigned days );
   static unsigned daysToMonths( unsigned days );
   /// <summary>Zeroes month, day, and year.</summary>
   void deleteContents() { monthOfYear = dayOfMonth = 0u; yearPart = 0u; }
   /// <summary>Returns day of month.</summary>
   ///
   /// <returns>Day of month (1-31).</returns>
   unsigned getDay() const { return dayOfMonth; }
   DateModyyr getDayBefore() const;
   /// <summary>Returns month of year (1-12).</summary>
   ///
   /// <returns>Month of year (1-12).</returns>
   unsigned getMonth () const { return monthOfYear; }
   /// <summary>Returns the year.</summary>
   ///
   /// <returns>The year.</returns>
   unsigned getYear() const { return yearPart; }
   /// <summary>Checks for month in bounds.</summary>
   ///
   /// <exception cref="PiaException">PiaException of type
   /// <see cref="PIA_IDS_DATEMONTH"/> if month is
   ///   out of range.</exception>
   void monthCheck() const { Date::monthCheck(monthOfYear); }
   std::string toString() const;
   std::string toString( const std::string& format ) const;
   void update();
};
