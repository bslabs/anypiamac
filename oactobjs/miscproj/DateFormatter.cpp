// Functions for the <see cref="DateFormatter"/> class to format and parse
// dates.
//
// $Id: DateFormatter.cpp 1.12 2011/08/11 15:21:15EDT 044579 Development  $

using namespace std;

#pragma warning( disable : 4127 4100)
#include "DateFormatter.h"
#include "Resource.h"
#include "datemoyr.h"
#include "PiaException.h"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/gregorian/gregorian_io.hpp"

/// <summary>Returns string with formatted date using format
/// "month day, year".</summary>
///
/// <returns>A string with date using format "month day, year".</returns>
///
/// <param name="dateModyyr">The month, day, and year date to format.</param>
std::string DateFormatter::toString(
  const boost::gregorian::date& dateModyyr )
{
  if (dateModyyr.is_not_a_date()) {
    return string("");
  }
  try {
    boost::gregorian::date_facet* output_facet =
      new boost::gregorian::date_facet("%B %d, %Y");
    stringstream ss;
    ss.imbue(locale(locale::classic(), output_facet));
    ss << dateModyyr;
    return ss.str();
  } catch (exception&) {
    // ignore any exception (return empty string)
    return string("");
  }
}

/// <summary>Returns string with formatted date using specified format.
/// </summary>
///
/// <returns>A string with date using specified format.</returns>
///
/// <param name="dateModyyr">The month, day, and year date to format.</param>
/// <param name="formatModyyr">Format to use for month, day, and year dates.
/// Use "s" to format as "mo/dy/year", with leading 0's if necessary. Use "n"
/// to format as "modyyear", with leading 0's if necessary. Any other format
/// will result in a format of "month day, year".</param>
std::string DateFormatter::toString(
  const boost::gregorian::date& dateModyyr, const std::string& formatModyyr )
{
  if (formatModyyr.length() < 1) {
    return toString(dateModyyr);
  }
  if (dateModyyr.is_not_a_date()) {
    switch (formatModyyr[0]) {
    case 's':
      return string("00/00/0000");
    default:
      return string("");
    }
  }
  try {
    boost::gregorian::date_facet* output_facet =
      new boost::gregorian::date_facet("%B %d, %Y");
    ostringstream ss;
    ss.imbue(locale(locale::classic(), output_facet));
    switch (formatModyyr[0]) {
    case 's':
      output_facet->format("%m/%d/%Y");
      ss << dateModyyr;
      return ss.str();
    case 'n':
      output_facet->format("%m%d%Y");
      ss << dateModyyr;
      return ss.str();
    default:
      return toString(dateModyyr);
    }
  } catch (exception&) {
    // ignore any exception (return empty string)
    return string("");
  }
}

/// <summary>Parses a date from an undelimited date string where the order
/// is month (2 digits), day (2 digits), and year (4 digits).</summary>
///
/// <returns>The date parsed from the date string.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DATEDAY"/> if bad day; of type
/// <see cref="PIA_IDS_DATEMONTH"/> if bad month; of type
/// <see cref="PIA_IDS_DATEYEAR"/> if bad year; of type
/// <see cref="PIA_IDS_READERR"/> if fewer than 8 characters.</exception>
///
/// <param name="line">Line to parse.</param>
boost::gregorian::date DateFormatter::date_from_undelimited_us_string(
 const std::string& line )
{
  try {
    const unsigned short month =
      static_cast<unsigned short>(atoi(line.substr(0, 2).c_str()));
    const unsigned short day =
      static_cast<unsigned short>(atoi(line.substr(2, 2).c_str()));
    const unsigned short year =
      static_cast<unsigned short>(atoi(line.substr(4, 4).c_str()));
    return boost::gregorian::date(year, month, day);
  } catch (boost::gregorian::bad_year) {
    throw PiaException(PIA_IDS_DATEYEAR);
  } catch (boost::gregorian::bad_month) {
    throw PiaException(PIA_IDS_DATEMONTH);
  } catch (boost::gregorian::bad_day_of_month) {
    throw PiaException(PIA_IDS_DATEDAY);
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses a month and year date from an undelimited date string
/// where the order is month (2 digits) and year (4 digits).</summary>
///
/// <returns>The month and year date parsed from the date string.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DATEMONTH"/> if bad month; of type
/// <see cref="PIA_IDS_DATEYEAR"/> if bad year; of type
/// <see cref="PIA_IDS_READERR"/> if fewer than 6 characters.</exception>
///
/// <param name="line">Line to parse.</param>
DateMoyr DateFormatter::month_year_from_undelimited_us_string(
 const std::string& line )
{
  try {
    const unsigned month =
      static_cast<unsigned>(atoi(line.substr(0, 2).c_str()));
    const unsigned year =
      static_cast<unsigned>(atoi(line.substr(2, 4).c_str()));
    return DateMoyr(month, year);
  } catch (boost::gregorian::bad_year) {
    throw PiaException(PIA_IDS_DATEYEAR);
  } catch (boost::gregorian::bad_month) {
    throw PiaException(PIA_IDS_DATEMONTH);
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Returns string with formatted date using format "month year".
/// </summary>
///
/// <returns>A string with date using format "month year".</returns>
///
/// <param name="dateMoyr">The month and year date to format.</param>
std::string DateFormatter::toString( const DateMoyr& dateMoyr )
{
  try {
    ostringstream ss;
    ss << boost::gregorian::greg_month(dateMoyr.getMonth()).as_long_string()
      << " " << dateMoyr.getYear();
    return(ss.str());
  } catch (exception&) {
    // ignore any exception (return empty string)
    return string("");
  }
}

/// <summary>Returns string with formatted date using specified format.
/// </summary>
///
/// <returns>A string with date using specified format.</returns>
///
/// <param name="dateMoyr">The month and year date to format.</param>
/// <param name="formatMoyr">Format to use for month and year dates. Use "s"
/// to format as "mo/year", with leading 0 if necessary. Use "n" to format as
/// "moyear", with leading 0 if necessary. Any other format will result in a
/// format of "month year".</param>
std::string DateFormatter::toString( const DateMoyr& dateMoyr,
const std::string& formatMoyr )
{
  if (formatMoyr.length() < 1)
    return toString(dateMoyr);
  try {
    ostringstream ss;
    ss.fill('0');
    switch (formatMoyr[0]) {
    case 's':
      ss << setw(2) << dateMoyr.getMonth() << "/"
        << setw(4) << dateMoyr.getYear();
      return(ss.str());
    case 'n':
      ss << setw(2) << dateMoyr.getMonth()
        << setw(4) << dateMoyr.getYear();
      return(ss.str());
    default:
      return toString(dateMoyr);
    }
  } catch (exception&) {
    // ignore any exception (return empty string)
    return string("");
  }
}
