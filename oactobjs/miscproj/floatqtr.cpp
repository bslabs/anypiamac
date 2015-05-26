// Functions for the <see cref="FloatQuarter"/> class to manage an
// array of quarterly floats.

// $Id: floatqtr.cpp 1.23 2011/08/11 16:23:34EDT 044579 Development  $

#include <fstream>
#include <iomanip>
#include <cmath>  // for pow
#include "floatmth.h"
#include "floatqtr.h"
#include "qtryear.h"
#include "Resource.h"
#include "PiaException.h"

using namespace std;

/// <summary>Initializes the base year and last year to the specified values.
/// </summary>
///
/// <param name="newBaseYear">New base year.</param>
/// <param name="newLastYear">New last year.</param>
FloatQuarter::FloatQuarter( int newBaseYear, int newLastYear ) :
theData((newLastYear - newBaseYear + 1) * 4), width(9),
baseYear(newBaseYear), lastYear(newLastYear), precision(0), lineWidth(75)
{ }

/// <summary>Gets one value of data.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FLOATQTR1"/> if year is out of range
/// (only in debug mode).</exception>
///
/// <returns>One value of data for the specified year and quarter.</returns>
///
/// <param name="qtrYear">Year and quarter (0-3) for which data is desired.
/// </param>
float FloatQuarter::getAt( const QtrYear& qtrYear ) const
{
#ifndef NDEBUG
  if (static_cast<int>(qtrYear.getYear()) < baseYear ||
    static_cast<int>(qtrYear.getYear()) > lastYear)
    throw PiaException(PIA_IDS_FLOATQTR1);
#endif
  return theData[4 * (qtrYear.getYear() - baseYear) + qtrYear.getQuarter()];
}

/// <summary>Sets one value of data.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FLOATQTR1"/> if year is out of range
/// (only in debug mode).</exception>
///
/// <param name="qtrYear">Year and quarter (0-3) for which data is desired.
/// </param>
/// <param name="newData">New value of data.</param>
void FloatQuarter::setAt( const QtrYear& qtrYear, float newData )
{
#ifndef NDEBUG
  if (static_cast<int>(qtrYear.getYear()) < baseYear ||
    static_cast<int>(qtrYear.getYear()) > lastYear)
    throw PiaException(PIA_IDS_FLOATQTR1);
#endif
  theData[4 * (qtrYear.getYear() - baseYear) + qtrYear.getQuarter()] =
    newData;
}

/// <summary>Gets one value of data after specified quarter.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FLOATQTR1"/> if year is out of range,
/// or of type <see cref="PIA_IDS_FLOATQTR3"/> if quarter is out of range
/// (only in debug mode).</exception>
///
/// <returns>One value of data after specified quarter.</returns>
///
/// <param name="qtrYear">Year and quarter (0-3) after which data is desired.
/// </param>
float FloatQuarter::getAfter( const QtrYear& qtrYear ) const
{
#ifndef NDEBUG
  if (static_cast<int>(qtrYear.getYear()) < baseYear ||
    static_cast<int>(qtrYear.getYear()) > lastYear)
    throw PiaException(PIA_IDS_FLOATQTR1);
  if (qtrYear.getQuarter() < 0u || qtrYear.getQuarter() > 3u)
    throw PiaException(PIA_IDS_FLOATQTR3);
#endif
  const unsigned year = qtrYear.getYear();
  const unsigned qtr = qtrYear.getQuarter();
  if (static_cast<int>(year) >= lastYear && qtr >= 3u) {
    return theData[4 * (year - baseYear) + qtr];
  }
  else {
    return theData[4 * (year - baseYear) + qtr + 1];
  }
}

/// <summary>Gets one value of data before specified quarter.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FLOATQTR1"/> if year is out of range,
/// or of type <see cref="PIA_IDS_FLOATQTR3"/> if quarter is out of range
/// (only in debug mode).</exception>
///
/// <returns>One value of data before specified quarter.</returns>
///
/// <param name="qtrYear">Year and quarter (0-3) before which data is desired.
/// </param>
float FloatQuarter::getBefore( const QtrYear& qtrYear ) const
{
#ifndef NDEBUG
  if (static_cast<int>(qtrYear.getYear()) < baseYear ||
    static_cast<int>(qtrYear.getYear()) > baseYear)
    throw PiaException(PIA_IDS_FLOATQTR1);
  if (qtrYear.getQuarter() < 0u || qtrYear.getQuarter() > 3u)
    throw PiaException(PIA_IDS_FLOATQTR3);
#endif
  const unsigned year = qtrYear.getYear();
  const unsigned qtr = qtrYear.getQuarter();
  if (static_cast<int>(year) <= baseYear && qtr <= 0u) {
    return theData[4 * (year - baseYear) + qtr];
  }
  else {
    return theData[4 * (year - baseYear) + qtr - 1];
  }
}

/// <summary>Sets data for specified years.</summary>
///
/// <param name="newData">New value of data to be used for all years.</param>
/// <param name="firstYeart">First year of data to set.</param>
/// <param name="lastYeart">Last year of data to set.</param>
void FloatQuarter::assign( float newData, int firstYeart, int lastYeart )
{
  for (int year = firstYeart; year <= lastYeart; year++) {
    for (int quarter = 0; quarter < 4; quarter++)
      theData[4 * (year - baseYear) + quarter] = newData;
  }
}

/// <summary>Sets data for specified years.</summary>
///
/// <param name="firstYeart">First year of data to set.</param>
/// <param name="lastYeart">Last year of data to set.</param>
/// <param name="floatQuarter">Array of new quarterly values of data.</param>
void FloatQuarter::assign( const FloatQuarter& floatQuarter, int firstYeart,
int lastYeart )
{
  for (int year = firstYeart; year <= lastYeart; year++) {
    for (int quarter = 0; quarter < 4; quarter++) {
      theData[4 * (year - baseYear) + quarter] =
        floatQuarter.getAt(QtrYear(quarter, year));
    }
  }
}

/// <summary>Sets data for specified years.</summary>
///
/// <param name="floatMonth">Array of monthly values. The three values
/// in each quarter are averaged.</param>
/// <param name="firstQtrYear">First year and quarter of data to set.</param>
/// <param name="lastQtrYear">Last year and quarter of data to set.</param>
void FloatQuarter::assign( const FloatMonth& floatMonth,
const QtrYear& firstQtrYear, const QtrYear& lastQtrYear )
{
  // Handle first year
  unsigned yr = firstQtrYear.getYear();
  for (unsigned qtr = firstQtrYear.getQuarter(); qtr < 4u; qtr++) {
    setAt(QtrYear(qtr, yr), floatMonth.qtrAvgCal(QtrYear(qtr, yr)));
  }
  // Handle full years
  for (yr = firstQtrYear.getYear() + 1u; yr < lastQtrYear.getYear(); yr++) {
    for (unsigned qtr = 0u; qtr < 4u; qtr++) {
      setAt(QtrYear(qtr, yr), floatMonth.qtrAvgCal(QtrYear(qtr, yr)));
    }
  }
  // Handle last year
  yr = lastQtrYear.getYear();
  for (unsigned qtr = 0u; qtr <= lastQtrYear.getQuarter(); qtr++) {
    setAt(QtrYear(qtr, yr), floatMonth.qtrAvgCal(QtrYear(qtr, yr)));
  }
}

/// <summary>Multiplies data by another <see cref="FloatQuarter"/>.</summary>
///
/// <param name="floatQuarter">Array of multipliers.</param>
/// <param name="firstYeart">First year of data to multiply.</param>
/// <param name="lastYeart">Last year of data to multiply.</param>
void FloatQuarter::multiply( const FloatQuarter& floatQuarter,
int firstYeart, int lastYeart )
{
  for (int year = firstYeart; year <= lastYeart; year++) {
    for (int quarter = 0; quarter < 4; quarter++) {
      theData[4 * (year - baseYear) + quarter] *=
        floatQuarter.getAt(QtrYear(quarter, year));
    }
  }
}

/// <summary>Calculates annual average.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FLOATQTR1"/> if year is out of range
/// (only in debug mode).</exception>
///
/// <returns>Annual average.</returns>
///
/// <param name="year">Year for which average is desired.</param>
float FloatQuarter::average( int year ) const
{
#ifndef NDEBUG
  if (year < baseYear || year > lastYear)
    throw PiaException(PIA_IDS_FLOATQTR1);
#endif
  return((getAt(QtrYear(0, year)) + getAt(QtrYear(1, year)) +
    getAt(QtrYear(2, year)) + getAt(QtrYear(3, year))) / 4.0f);
}

/// <summary>Calculates annual percentage increase.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FLOATQTR1"/> if year is out of range, or of type
/// <see cref="PIA_IDS_FLOATQTR3"/> if quarter is out of range, or of type
/// <see cref="PIA_IDS_FLOATQTR4"/> if value is zero in prior year (only
/// in debug mode). </exception>
///
/// <returns>Annual percentage increase.</returns>
///
/// <param name="qtrYear">Year and quarter (0-3) for which percentage
/// increase is desired.</param>
float FloatQuarter::anninc( const QtrYear& qtrYear ) const
{
#ifndef NDEBUG
  if (static_cast<int>(qtrYear.getYear()) < baseYear ||
    static_cast<int>(qtrYear.getYear()) > lastYear)
    throw PiaException(PIA_IDS_FLOATQTR1);
  if (qtrYear.getQuarter() < 0u || qtrYear.getQuarter() > 3u)
    throw PiaException(PIA_IDS_FLOATQTR3);
  if (getAt(QtrYear(qtrYear.getQuarter(), qtrYear.getYear() - 1)) < .001)
    throw PiaException(PIA_IDS_FLOATQTR4);
#endif
  return(100.0f * (getAt(qtrYear) /
    getAt(QtrYear(qtrYear.getQuarter(), qtrYear.getYear() - 1)) - 1.0f));
}

/// <summary>Calculates 3-year percentage increase at annual rate.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FLOATQTR1"/> if year is out of range, or of type
/// <see cref="PIA_IDS_FLOATQTR3"/> if quarter is out of range, or of type
/// <see cref="PIA_IDS_FLOATQTR4"/> if value is zero in third prior year
/// (only in debug mode).</exception>
///
/// <returns>3-year percentage increase at annual rate.</returns>
///
/// <param name="qtrYear">Year and quarter (0-3) for which percentage
/// increase is desired.</param>
float FloatQuarter::anninc3( const QtrYear& qtrYear ) const
{
#ifndef NDEBUG
  if (static_cast<int>(qtrYear.getYear()) < baseYear + 3 ||
    static_cast<int>(qtrYear.getYear()) > lastYear)
    throw PiaException(PIA_IDS_FLOATQTR1);
  if (qtrYear.getQuarter() < 0u || qtrYear.getQuarter() > 3u)
    throw PiaException(PIA_IDS_FLOATQTR3);
  if (getAt(QtrYear(qtrYear.getQuarter(), qtrYear.getYear() - 3)) < .001)
    throw PiaException(PIA_IDS_FLOATQTR4);
#endif
  return((float)(100.0 * (pow((double)getAt(qtrYear) /
    (double)getAt(QtrYear(qtrYear.getQuarter(), qtrYear.getYear() - 3)),
    0.333333) - 1.0)));
}

/// <summary>Reads array from an open stream.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FLOATQTR2"/> if unable to read
/// data for all specified years.</exception>
///
/// <param name="in">Input stream.</param>
/// <param name="firstYeart">First year of data to read.</param>
/// <param name="lastYeart">Last year of data to read.</param>
void FloatQuarter::read( std::istream& in, int firstYeart, int lastYeart )
{
  float temp;  // temporary value
  for (int yr = firstYeart; yr <= lastYeart; yr++) {
    for (int quarter = 0; quarter < 4; quarter++) {
      if (!(in >> temp))
        throw PiaException(PIA_IDS_FLOATQTR2);
      setAt(QtrYear(quarter, yr), temp);
    }
  }
}

/// <summary>Writes array to an open stream.</summary>
///
/// <param name="out">Output stream.</param>
/// <param name="firstYeart">First year of data to write.</param>
/// <param name="lastYeart">Last year of data to write.</param>
void FloatQuarter::write( std::ostream& out, int firstYeart, int lastYeart )
{
  int numperline = lineWidth / width;  // number of numbers per line
  out.precision(precision);
  for (int yr = firstYeart; yr <= lastYeart; yr++) {
    for (int quarter = 0; quarter < 4; quarter++) {
      out << setw(width) << getAt(QtrYear(quarter, yr));
      if ((4 * (yr - firstYeart) + quarter) % numperline == numperline - 1)
        out << endl;
    }
  }
  if ((4 * (lastYeart - firstYeart) + 3) % numperline != numperline - 1) {
    out << endl;
  }
}
