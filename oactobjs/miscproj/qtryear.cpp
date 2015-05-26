// Functions for the <see cref="QtrYear"/> class to handle a year and
// quarter.

// $Id: qtryear.cpp 1.26 2011/08/19 15:57:45EDT 044579 Development  $

#include <sstream>
#include "qtryear.h"
#include "datemoyr.h"
#include "Resource.h"
#include "PiaException.h"

using namespace std;

/// <summary>Initializes the quarter and year from a <see cref="DateMoyr"/>.
/// </summary>
///
/// <remarks>Quarter is set to 0 for months 0-2, 1 for months 3-5, 2 for
/// months 6-8, or 3 for months 9-11.</remarks>
///
/// <param name="dateMoyr">The date to use.</param>
QtrYear::QtrYear( const DateMoyr& dateMoyr ) :
quarterOfYear(static_cast<unsigned short>((dateMoyr.getMonth() -
boost::date_time::Jan) / 3)),
yearPart(static_cast<unsigned short>(dateMoyr.getYear()))
{ }

/// <summary>Initializes the quarter and year from a boost::gregorian::date.
/// </summary>
///
/// <remarks>Quarter is set to 0 for months 0-2, 1 for months 3-5, 2 for
/// months 6-8, or 3 for months 9-11.</remarks>
///
/// <param name="dateModyyr">The date to use.</param>
QtrYear::QtrYear( const boost::gregorian::date& dateModyyr ) :
quarterOfYear(static_cast<unsigned short>((dateModyyr.month() -
boost::date_time::Jan) / 3)),
yearPart(static_cast<unsigned short>(dateModyyr.year()))
{ }

/// <summary>Sets quarter and year from a <see cref="QtrYear"/>.</summary>
///
/// <returns>A reference to this instance.</returns>
///
/// <param name="qtrYear">The other <see cref="QtrYear"/> to use.</param>
QtrYear& QtrYear::operator=( const QtrYear& qtrYear )
{
  quarterOfYear = qtrYear.quarterOfYear;
  yearPart = qtrYear.yearPart;
  return *this;
}

/// <summary>Sets quarter and year from a date with months and years.
/// </summary>
///
/// <returns>A reference to this instance.</returns>
///
/// <param name="date">The date to use.</param>
QtrYear& QtrYear::operator=( const DateMoyr& date )
{
  quarterOfYear = static_cast<unsigned short>((date.getMonth() -
    boost::date_time::Jan) / 3);
  yearPart = static_cast<unsigned short>(date.getYear());
  return *this;
}

/// <summary>Sets quarter and year from a date.</summary>
///
/// <returns>A reference to this instance.</returns>
///
/// <param name="date">The date to use.</param>
QtrYear& QtrYear::operator=( const boost::gregorian::date& date )
{
  quarterOfYear = static_cast<unsigned short>((date.month() -
    boost::date_time::Jan) / 3);
  yearPart = static_cast<unsigned short>(date.year());
  return *this;
}

/// <summary>Adds a number of quarters this one.</summary>
///
/// <param name="addQuarter">The number of quarters to add.</param>
void QtrYear::add( unsigned addQuarter )
{
  quarterOfYear = static_cast<unsigned short>(quarterOfYear + addQuarter);
  if (quarterOfYear > 3u) {
    const unsigned temp = quarterOfYear / 4u;
    quarterOfYear = static_cast<unsigned short>(quarterOfYear - 4u * temp);
    yearPart = static_cast<unsigned short>(yearPart + temp);
  }
}

/// <summary>Adds a (quarter,year) date to this one.</summary>
///
/// <param name="addQtrYear">The (quarter,year) date to add.</param>
void QtrYear::add( const QtrYear& addQtrYear )
{
  quarterOfYear =
    static_cast<unsigned short>(quarterOfYear + addQtrYear.quarterOfYear);
  if (quarterOfYear > 3u) {
    const unsigned temp = quarterOfYear / 4u;
    quarterOfYear = static_cast<unsigned short>(quarterOfYear - 4u * temp);
    yearPart = static_cast<unsigned short>(yearPart + temp);
  }
  yearPart = static_cast<unsigned short>(yearPart + addQtrYear.yearPart);
}

/// <summary>Subtracts a number of quarters from this one.</summary>
///
/// <param name="subQuarter">Number of quarters to subtract.</param>
void QtrYear::subtract( unsigned subQuarter )
{
  if (quarterOfYear >= subQuarter) {
    quarterOfYear = static_cast<unsigned short>(quarterOfYear - subQuarter);
  }
  else {
    const unsigned temp = (subQuarter + 3u - quarterOfYear) / 4u;
    quarterOfYear =
      static_cast<unsigned short>(quarterOfYear + 4u * temp - subQuarter);
    if (yearPart >= temp) {
      yearPart = static_cast<unsigned short>(yearPart - temp);
    }
    else {
      throw PiaException(PIA_IDS_QTRYRQTR);
    }
  }
}

/// <summary>Subtracts a number of (quarter,year) date from this one.</summary>
///
/// <param name="subQtrYear">(quarters,year) date to subtract.</param>
void QtrYear::subtract( const QtrYear& subQtrYear )
{
  const unsigned subQuarter = subQtrYear.quarterOfYear;
  if (quarterOfYear >= subQuarter) {
    quarterOfYear = static_cast<unsigned short>(quarterOfYear - subQuarter);
  }
  else {
    const unsigned temp = (subQuarter + 3u - quarterOfYear) / 4u;
    quarterOfYear =
      static_cast<unsigned short>(quarterOfYear + 4u * temp - subQuarter);
    if (yearPart >= temp) {
      yearPart = static_cast<unsigned short>(yearPart - temp);
    }
    else {
      throw PiaException(PIA_IDS_QTRYRQTR);
    }
  }
  if (yearPart >= subQtrYear.yearPart) {
    yearPart = static_cast<unsigned short>(yearPart - subQtrYear.yearPart);
  }
  else {
    throw PiaException(PIA_IDS_QTRYRQTR);
  }
}

/// <summary>Compares this date to another one.</summary>
///
/// <returns>True if this date is before argument, false otherwise.</returns>
///
/// <param name="qtrYear">The date to compare to.</param>
bool QtrYear::operator<( const QtrYear& qtrYear ) const
{
  // check year
  if (yearPart != qtrYear.yearPart)
    return(yearPart < qtrYear.yearPart);
  // same year, so check quarter
  return(quarterOfYear < qtrYear.quarterOfYear);
}

/// <summary>Checks for quarter in bounds.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QTRYRQTR"/> if quarter is greater than 3.</exception>
void QtrYear::qtrCheck() const
{
  if (quarterOfYear > 3u)
    throw PiaException(PIA_IDS_QTRYRQTR);
}

/// <summary>Checks for year in bounds.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QTRYRYR"/> if year is greater than
/// <see cref="Date::highyear"/>.</exception>
void QtrYear::yearCheck() const
{
  if (yearPart > Date::highyear)
    throw PiaException(PIA_IDS_QTRYRYR);
}

/// <summary>Returns string representation of quarter and year.</summary>
///
/// <returns>String representation of quarter and year, for example
///   "qtr 3 of 1999".</returns>
std::string QtrYear::toString() const
{
  ostringstream strm;
  strm << "qtr " << quarterOfYear + 1 << " of " << yearPart;
  return strm.str();
}

/// <summary>Calculates difference of two quarter-year periods as an integer
///   number of quarters.</summary>
///
/// <returns>Difference as an integer number of quarters. Result is negative
/// if second period is before first period.</returns>
///
/// <param name="qtrYear1">First period.</param>
/// <param name="qtrYear2">Second period.</param>
int QtrYear::diff( const QtrYear& qtrYear1, const QtrYear& qtrYear2 )
{
  int rv = static_cast<int>(qtrYear2.getQuarter()) -
    static_cast<int>(qtrYear1.getQuarter());
  rv += 4 * (static_cast<int>(qtrYear2.getYear()) -
    static_cast<int>(qtrYear1.getYear()));
  return rv;
}

/// <summary>Converts this quarter and year to a month and year.</summary>
///
/// <returns>The converted month and year.</returns>
DateMoyr QtrYear::toDateMoyr() const
{
  return DateMoyr(quarterOfYear * 3 + boost::date_time::Jan, yearPart);
}
