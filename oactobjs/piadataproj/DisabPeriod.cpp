// Functions for the <see cref="DisabPeriod"/> class to manage a period
// of disability.

// $Id: DisabPeriod.cpp 1.12 2011/07/29 15:26:15EDT 044579 Development  $

#include <sstream>
#include <iomanip>
#include "DisabPeriod.h"
#include "Trace.h"
#include "oactcnst.h"
#include "DateFormatter.h"
#include "PiaException.h"
#include "Resource.h"

using namespace std;

/// <summary>Constructor.</summary>
///
/// <remarks>Initializes onsetDate to not_a_date_time and all other
/// fields to 0.</remarks>
DisabPeriod::DisabPeriod() :
onsetDate(boost::date_time::not_a_date_time), cessationDate(), entDate(),
waitperDate(), cessationPia(0.0f), cessationMfb(0.0f)
{ }

/// <summary>Sets all variables to default values.</summary>
void DisabPeriod::deleteContents()
{
  onsetDate = boost::gregorian::date(boost::date_time::not_a_date_time);
  cessationDate.deleteContents();
  entDate.deleteContents();
  waitperDate.deleteContents();
  cessationPia = cessationMfb = 0.0f;
}

/// <summary>Checks a first month of waiting period for general
/// reasonableness.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_WAITPD1"/> if bad month; of type
/// <see cref="PIA_IDS_WAITPD2"/> if year is before 1940; of type
/// <see cref="PIA_IDS_WAITPD3"/> if year is after maximum allowed.
/// </exception>
///
/// <param name="dateMoyr">The date to check.</param>
void DisabPeriod::waitperDateCheck( const DateMoyr& dateMoyr )
{
  try {
    dateMoyr.monthCheck();
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_WAITPD1);
  }
  if (dateMoyr.getYear() < 1940u) {
    ostringstream strm;
    strm << "Wait period year is " << dateMoyr.getYear();
    Trace::writeLine(strm.str());
    throw PiaException(PIA_IDS_WAITPD2);
  }
  try {
    Date::yearCheck(dateMoyr.getYear());
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_WAITPD3);
  }
}

/// <summary>Calculates first month of waiting period.</summary>
///
/// <returns>First month of waiting period, given the date of onset.</returns>
///
/// <remarks>Returns the first month after onset. If onset is on the first of
/// the month, returns the date of onset (converted to month/year format).
/// Otherwise, returns the month after the date of onset.</remarks>
DateMoyr DisabPeriod::waitperDateCal() const
{
  if (onsetDate.day() == 1) {
    return DateMoyr(onsetDate);
  }
  else {
    if (onsetDate.month() == boost::date_time::Dec) {
      return DateMoyr(boost::date_time::Jan, onsetDate.year() + 1);
    }
    else {
      return DateMoyr(onsetDate.month() + 1, onsetDate.year());
    }
  }
}

/// <summary>Checks a date of disability onset.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_ONSET1"/> if month of onset is out of range; of type
/// <see cref="PIA_IDS_ONSET2"/> if day of onset is out of range; of type
/// <see cref="PIA_IDS_ONSET3"/> if year of onset is before 1937; of type
/// <see cref="PIA_IDS_ONSET4"/> if year of onset is after
/// maximum allowed.</exception>
///
/// <param name="dateModyyr">The date to check.</param>
void DisabPeriod::onsetDateCheck( const boost::gregorian::date& dateModyyr )
{
  if (dateModyyr.is_not_a_date()) {
    throw PiaException(PIA_IDS_ONSET1);
  }
  if (dateModyyr.year() < 1937) {
    // disability onset before 1937
    throw PiaException(PIA_IDS_ONSET3);
  }
  try {
    Date::yearCheck(dateModyyr.year());
  } catch (PiaException&) {
    // disability onset after last possible year
    throw PiaException(PIA_IDS_ONSET4);
  }
}

/// <summary>Checks a date of disability cessation.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DICESS1"/> if month of cessation is out of range; of
/// type <see cref="PIA_IDS_DICESS2"/> if year of cessation is before 1937; of
/// type <see cref="PIA_IDS_DICESS3"/> if year of cessation is after maximum
/// allowed.</exception>
///
/// <param name="dateMoyr">The date to check.</param>
void DisabPeriod::cessationDateCheck( const DateMoyr& dateMoyr )
{
  try {
    dateMoyr.monthCheck();
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_DICESS1);
  }
  if (static_cast< int >(dateMoyr.getYear()) < YEAR37) {
    // disability cessation before 1937
    throw PiaException(PIA_IDS_DICESS2);
  }
  try {
    Date::yearCheck(dateMoyr.getYear());
  } catch (PiaException&) {
    // disability cessation after last possible year
    throw PiaException(PIA_IDS_DICESS3);
  }
}

/// <summary>Checks a date of entitlement.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PRRENT1"/> if bad month; of type
/// <see cref="PIA_IDS_PRRENT2"/> if year is before 1940; of type
/// <see cref="PIA_IDS_PRRENT3"/> if year is after maximum allowed.
/// </exception>
///
/// <param name="dateMoyr">The date to check.</param>
void DisabPeriod::entDateCheck( const DateMoyr& dateMoyr )
{
  try {
    dateMoyr.monthCheck();
  } catch (PiaException) {
    throw PiaException(PIA_IDS_PRRENT1);
  }
  if (static_cast< int >(dateMoyr.getYear()) < 1940)
    throw PiaException(PIA_IDS_PRRENT2);
  try {
    Date::yearCheck(dateMoyr.getYear());
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_PRRENT3);
  }
}

/// <summary>Returns the period of disability as a string, using default
/// format.</summary>
///
/// <returns>The period of disability as a string.</returns>
///
/// <remarks>The onset date is formatted as "modyyear", followed by the
/// entitlement date, the first month of the waiting period, and the cessation
/// date, in that order, formatted as "moyear", with a leading 0 on each if
/// necessary.</remarks>
std::string DisabPeriod::toString() const
{
  ostringstream strm;
  strm.setf(ios::fixed,ios::floatfield);
  strm.precision(2);
  strm << DateFormatter::toString(getOnsetDate(), "n")
    << DateFormatter::toString(getEntDate(), "n")
    << DateFormatter::toString(getWaitperDate(), "n")
    << DateFormatter::toString(getCessationDate(), "n")
    << setw(10) << getCessationPia()
    << setw(10) << getCessationMfb();
  return strm.str();
}

/// <summary>Returns the period of disability as a string, using specified
/// format.</summary>
///
/// <returns>The period of disability as a string.</returns>
///
/// <remarks>The format has the following meanings:
/// <table>
///    "c"     show only the onset date, the entitlement date, and the first month of the waiting period.
///    other   show all four dates.
/// </table></remarks>
///
/// <param name="format">Format to use.</param>
std::string DisabPeriod::toString( const std::string& format ) const
{
  if (format.length() < 1)
    return toString();
  try {
    switch (format[0]) {
    case 'c':
      return DateFormatter::toString(getOnsetDate(), "n") +
        DateFormatter::toString(getEntDate(), "n") +
        DateFormatter::toString(getWaitperDate(), "n");
    default:
      return toString();
    }
  } catch (PiaException&) {
    // ignore any exception (return empty string)
    return string("");
  }
}

/// <summary>Parses a string into a <see cref="DisabPeriod"/>.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_READERR"/> if string is not in the correct format.
/// </exception>
///
/// <param name="line">The line to parse.</param>
void DisabPeriod::parseString( const std::string& line )
{
  try {
    // parse date of onset
    onsetDate = DateFormatter::date_from_undelimited_us_string(
      line.substr(0, 8));
    // parse date of prior entitlement
    entDate = DateFormatter::month_year_from_undelimited_us_string(
      line.substr(8, 6));
    // parse first month of waiting period
    waitperDate = DateFormatter::month_year_from_undelimited_us_string(
      line.substr(14, 6));
    if (line.length() >= 26) {
      // parse month and year of disability cessation
      cessationDate = DateFormatter::month_year_from_undelimited_us_string(
        line.substr(20, 6));
    }
    if (line.length() >= 36) {
      // parse pia at cessation
      cessationPia = (float)atof(line.substr(26, 10).c_str());
    }
    if (line.length() >= 46) {
      // parse mfb at cessation
      cessationMfb = (float)atof(line.substr(36, 10).c_str());
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}
