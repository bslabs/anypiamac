// Functions for the <see cref="MilServDates"/> class to manage one
// military service dates record.

// $Id: msdates.cpp 1.35 2013/07/18 08:05:09EDT 540050 Development  $

#include <sstream>
#include <iomanip>
#include <utility>  // for rel_ops
#include "msdates.h"
#include "PiaException.h"
#include "Resource.h"
#include "DateFormatter.h"
#include "age.h"

using namespace std;
#if !defined(__SGI_STL_INTERNAL_RELOPS)
using namespace std::rel_ops;
#endif

// <summary>First possible date of military service wage credits.</summary>
const DateMoyr MilServDates::firstMilServDate = DateMoyr(9, 1940);
// <summary>Last possible date of military service wage credits.</summary>
const DateMoyr MilServDates::lastMilServDate = DateMoyr(12, 2001);
// <summary>Last possible date of pre-1951 military service wage credits.
// </summary>
const DateMoyr MilServDates::dec1950 = DateMoyr(12, 1950);
// <summary>Amount of credit per month for pre-1951 military service wage
// credits.</summary>
const double MilServDates::amtPerMonth37_56 = 160.0;
// <summary>Amount of credit per quarter for military service wage credits
// from 1957 through 1977.</summary>
const double MilServDates::amtPerQtr57_77 = 300.0;
// <summary>Amount of credit per year for military service wage credits
// from 1978 through 2001.</summary>
const double MilServDates::amtPerYear78_01 = 1200.0;

/// <summary>Creates this record with zero or blank entries.</summary>
MilServDates::MilServDates() : startDate(), endDate(),
count1(0), count2(0), theType(' ')
{ }

/// <summary>Destroys this instance.</summary>
MilServDates::~MilServDates()
{ }

/// <summary>Parses a line of data into a record, including the record number,
/// the total record count, and the record type.</summary>
///
/// <param name="line">Line to parse.</param>
void MilServDates::parseLine( const std::string& line )
{
  setCount1(atoi(line.substr(0, 2).c_str()));
  setCount2(atoi(line.substr(1, 2).c_str()));
  parseDates(line.substr(4));
  setType(line[16]);
}

/// <summary>Parses a line of data into two dates.</summary>
///
/// <param name="line">Line to parse.</param>
void MilServDates::parseDates( const std::string& line )
{
  startDate = DateFormatter::month_year_from_undelimited_us_string(
    line.substr(0, 6));
  endDate = DateFormatter::month_year_from_undelimited_us_string(
    line.substr(6, 6));
}

/// <summary>Returns a string with the two dates, without spaces.</summary>
///
/// <returns>A string with the two dates, without spaces.</returns>
std::string MilServDates::getDatesString() const
{
  string s(DateFormatter::toString(startDate, "n"));
  s += DateFormatter::toString(endDate, "n");
  return s;
}

/// <summary>Returns a string with the military service info,
/// with spaces appropriate for a print output.</summary>
///
/// <remarks>If the format string begins with "f", create a fixed-width string;
/// otherwise create a string with no extra spaces.</remarks>
///
/// <returns>A string with the military service info.</returns>
///
/// <param name="format">Format to use.</param>
std::string MilServDates::toString( const std::string& format ) const
{
  if (format.length() < 1)
    return toString();
  switch (format[0]) {
  case 'f':
    {
      ostringstream strm;
      strm << getCount1() << setw(4) << getCount2()
        << setw(5) << startDate.getMonth()
        << setw(9) << startDate.getYear()
        << setw(4) << endDate.getMonth()
        << setw(8) << endDate.getYear()
        << setw(4) << getType();
      return(strm.str());
    }
  default:
    return toString();
  }
}

/// <summary>Returns a string with the military service info.</summary>
///
/// <returns>A string A string with the military service info.</returns>
std::string MilServDates::toString() const
{
  ostringstream strm;
  strm << setw(2) << getCount1() << setw(2) << getCount2()
    << getDatesString() << getType();
  return strm.str();
}

/// <summary>Checks a military service record for consistent dates.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_MILSERV1"/> if starting date is after ending date.
/// </exception>
void MilServDates::check() const
{
  startDateCheck();
  endDateCheck();
  // check for consistent dates
  if (startDate > endDate)
    throw PiaException(PIA_IDS_MILSERV1);
}

/// <summary>Returns pre-1951 quarters of coverage in a one-line record.
/// </summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_MILSERV6"/> if years are out of range (only in debug
/// mode).</exception>
///
/// <returns>Pre-1951 quarters of coverage in a one-line record.</returns>
int MilServDates::getQc3750() const
{
  if (startDate <= dec1950) {
    const DateMoyr lastDate = (endDate <= dec1950) ? endDate : dec1950;
    // index of first qc earned
    const int firstqc =
      4 * startDate.getYear() + (startDate.getMonth() - 1) / 3;
    // index of last qc earned
    const int lastqc =
      (4 * lastDate.getYear() + (lastDate.getMonth() - 1) / 3);
#if !defined(NDEBUG)
    if (lastqc - firstqc + 1 > 56)
      throw PiaException(PIA_IDS_MILSERV6);
#endif
    return(lastqc - firstqc + 1);
  }
  else {
    return(0);
  }
}

/// <summary>Returns quarters of coverage for one year in a one-line
///  record.</summary>
///
/// <returns>Quarters of coverage for one year in a one-line record.</returns>
///
/// <param name="year">Year for which quarters are desired.</param>
int MilServDates::getQcov( unsigned year ) const
{
  if (startDate.getYear() <= year && endDate.getYear() >= year) {
    // index of first qc earned
    const int firstqc = (startDate.getYear() < year) ?
      0 : (startDate.getMonth() - 1) / 3;
    // index of last qc earned
    const int lastqc = (endDate.getYear() > year) ?
      3 : (endDate.getMonth() - 1) / 3;
    return(lastqc - firstqc + 1);
  }
  else {
    return(0);
  }
}

/// <summary>Returns pre-1951 wage credits in a one-line record.</summary>
///
/// <returns>Pre-1951 wage credits in a one-line record.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_MILSERV7"/> if years are out
///   of range (only in debug mode).</exception>
double MilServDates::getEarn3750() const
{
  if (startDate <= dec1950 && startDate <= endDate) {
    const DateMoyr lastDate = (endDate <= dec1950) ? endDate : dec1950;
    const Age age = lastDate - startDate;
    const int months = age.toMonths() + 1;
#if !defined(NDEBUG)
    if (months > 168)
      throw PiaException(PIA_IDS_MILSERV7);
#endif
    return(months * amtPerMonth37_56);
  }
  else {
    return(0.0);
  }
}

/// <summary>Returns wage credits for one year in a one-line record.</summary>
///
/// <returns>Wage credits for one year in a one-line record.</returns>
///
/// <param name="year">Year for which wage credits are desired.</param>
double MilServDates::getEarn( unsigned year ) const
{
  if (startDate.getYear() <= year && endDate.getYear() >= year) {
    // Based on earnings, not months, after 1977.  But we don't have
    // earnings, so we just return the maximum.
    if (year > 1977) {
      return amtPerYear78_01;
    }
    // index of first month of credits
    const int firstmonth =
      (startDate.getYear() < year) ? 1 : startDate.getMonth();
    // index of last month of credits
    const int lastmonth = (endDate.getYear() > year) ?
      12 : endDate.getMonth();
    // convert to quarters after 1956
    if (year > 1956) {
      const int firstqtr = (firstmonth + 2) / 3;
      const int lastqtr = (lastmonth + 2) / 3;
      return((lastqtr - firstqtr + 1) * amtPerQtr57_77);
    }
    else {
      return((lastmonth - firstmonth + 1) * amtPerMonth37_56);
    }
  }
  else {
    return(0.0);
  }
}

/// <summary>Zeroes out dates of military service.</summary>
void MilServDates::deleteContents()
{
  count1 = count2 = 0;
  theType = ' ';
  startDate.deleteContents();
  endDate.deleteContents();
}

/// <summary>Checks a specified starting date for validity.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_MILSERV4"/> if month or year is out of range.
/// </exception>
///
/// <param name="date">The starting date.</param>
void MilServDates::startDateCheck( const DateMoyr& date )
{
  if (date < firstMilServDate || date > lastMilServDate)
    throw PiaException(PIA_IDS_MILSERV4);
}

/// <summary>Checks a specified ending date for validity.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_MILSERV5"/> if month or year is out of range.
/// </exception>
///
/// <param name="date">The ending date.</param>
void MilServDates::endDateCheck( const DateMoyr& date )
{
  if (date < firstMilServDate || date > lastMilServDate)
    throw PiaException(PIA_IDS_MILSERV5);
}
