// Functions for the <see cref="FloatMonth"/> class to manage an array
// of monthly floats.

// $Id: floatmth.cpp 1.26 2011/08/11 16:23:34EDT 044579 Development  $

#include <fstream>
#include <iomanip>
#include "floatmth.h"
#include "Resource.h"
#include "PiaException.h"
#include "datemoyr.h"
#include "qtryear.h"

using namespace std;

/// <summary>Initializes the base year and last year to the specified values.
/// </summary>
///
/// <param name="newBaseYear">New base year.</param>
/// <param name="newLastYear">New last year.</param>
FloatMonth::FloatMonth( int newBaseYear, int newLastYear ) :
theData((newLastYear - newBaseYear + 1) * 12), width(9),
baseYear(newBaseYear), lastYear(newLastYear), precision(0), lineWidth(60)
{ }

/// <summary>Returns one value of data.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FLOATMTH1"/> if year is out of range
/// (only in debug mode).</exception>
///
/// <returns>One value of data for the specified year and month.</returns>
///
/// <param name="dateMoyr">Year and month (0-11) for which data is desired.
/// </param>
float FloatMonth::getAt( const DateMoyr& dateMoyr ) const
{
  const int year = (int)dateMoyr.getYear();
#ifndef NDEBUG
  if (year < baseYear || year > lastYear)
    throw PiaException(PIA_IDS_FLOATMTH1);
#endif
  return theData[12 * (year - baseYear) + (int)dateMoyr.getMonth()];
}

/// <summary>Sets one value of data.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FLOATMTH1"/> if year is out of range
/// (only in debug mode).</exception>
///
/// <param name="dateMoyr">Year and month (0-11) for which data is to be set.
/// </param>
/// <param name="newData">New value of data.</param>
void FloatMonth::setAt( const DateMoyr& dateMoyr, float newData )
{
  const int year = (int)dateMoyr.getYear();
#ifndef NDEBUG
  if (year < baseYear || year > lastYear)
    throw PiaException(PIA_IDS_FLOATMTH1);
#endif
  theData[12 * (year - baseYear) + (int)dateMoyr.getMonth()] = newData;
}

/// <summary>Sets data for specified years.</summary>
///
/// <param name="newData">Array of new values of data.</param>
/// <param name="firstYeart">First year of data to set.</param>
/// <param name="lastYeart">Last year of data to set.</param>
void FloatMonth::assign( const FloatMonth& newData, int firstYeart,
int lastYeart )
{
  for (int year = firstYeart; year <= lastYeart; year++) {
    for (int month = 0; month < 12; month++) {
      theData[12 * (year - baseYear) + month] =
        newData.getAt(DateMoyr(month, year));
    }
 }
}

/// <summary>Sets data for specified years.</summary>
///
/// <param name="firstYeart">First year of data to set.</param>
/// <param name="lastYeart">Last year of data to set.</param>
/// <param name="newFloat">New value of data to be used for all years.</param>
void FloatMonth::assign( float newFloat, int firstYeart, int lastYeart )
{
  for (int year = firstYeart; year <= lastYeart; year++) {
    for (int month = 0; month < 12; month++)
      theData[12 * (year - baseYear) + month] = newFloat;
  }
}

/// <summary>Calculates annual average of the 12 monthly figures.</summary>
///
/// <returns>Annual average of the 12 monthly figures.</returns>
///
/// <param name="year">Year for which average is desired.</param>
float FloatMonth::annAvgCal( int year ) const
{
  float temp = 0.0f;
  for (int mon = 0; mon < 12; mon++)
    temp += getAt(DateMoyr(mon, year));
  return(temp / 12.0f);
}

/// <summary>Calculates quarterly average of the 3 monthly figures.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FLOATMTH3"/> if quarter is out of range
/// (only in debug mode).</exception>
///
/// <returns>Quarterly average of the 3 monthly figures.</returns>
///
/// <param name="qtrYear">Year and quarter (0-3) for which average is desired.
/// </param>
float FloatMonth::qtrAvgCal( const QtrYear& qtrYear ) const
{
#ifndef NDEBUG
  if (qtrYear.getQuarter() < 0 || qtrYear.getQuarter() > 3)
    throw PiaException(PIA_IDS_FLOATMTH3);
#endif
  float temp = 0.0f;
  const unsigned firstMon = 3u * qtrYear.getQuarter();
  for (unsigned mon = firstMon; mon < firstMon + 3u; mon++) {
      temp += getAt(DateMoyr(mon, qtrYear.getYear()));
  }
  return(temp / 3.0f);
}

/// <summary>Reads array from an open stream.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FLOATMTH2"/> if unable to read
/// data for all specified years.</exception>
///
/// <param name="in">Input stream.</param>
/// <param name="firstYeart">First year of data to read.</param>
/// <param name="lastYeart">Last year of data to read.</param>
void FloatMonth::read( std::istream& in, int firstYeart, int lastYeart )
{
  float temp;  // temporary value
  for (int yr = firstYeart; yr <= lastYeart; yr++) {
    for (int month = 0; month < 12; month++) {
      if (!(in >> temp))
        throw PiaException(PIA_IDS_FLOATMTH2);
      setAt(DateMoyr(month, yr), temp);
    }
  }
}

/// <summary>Writes array to an open stream.</summary>
///
/// <param name="out">Output stream.</param>
/// <param name="firstYeart">First year of data to write.</param>
/// <param name="lastYeart">Last year of data to write.</param>
void FloatMonth::write( std::ostream& out, int firstYeart, int lastYeart )
{
  const int numperline = lineWidth / width;  // number of numbers per line
  out.precision(precision);
  for (int yr = firstYeart; yr <= lastYeart; yr++) {
    for (int month = 0; month < 12; month++) {
      out << setw(width) << getAt(DateMoyr(month, yr));
      if ((12 * (yr - firstYeart) + month) % numperline == numperline - 1)
        out << endl;
    }
  }
  // Put an endline if an odd number of numbers
  if ((12 * (lastYeart - firstYeart + 1)) % numperline != 0)
    out << endl;
}

/// <summary>Finds number of months of (nonzero) data in a given year.
/// </summary>
///
/// <returns>Number of months of (nonzero) data in a given year.</returns>
///
/// <param name="year">Year to test.</param>
int FloatMonth::nmonCal( int year ) const
{
  for (int month = 0; month < 12; month++) {
    if (getAt(DateMoyr(month, year)) < 1.0)
      return(month);
  }
  return(12);
}
