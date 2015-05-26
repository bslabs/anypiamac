// Functions for the <see cref="QcArray"/> class to handle an array
// of annual number of quarters of coverage.

// $Id: QcArray.cpp 1.38 2011/08/09 14:59:55EDT 044579 Development  $

#include <sstream>
#include <utility>  // for rel_ops
#include "QcArray.h"
#include "oactcnst.h"
#include "PiaException.h"
#include "Resource.h"

using namespace std;
#if !defined(__SGI_STL_INTERNAL_RELOPS)
using namespace std::rel_ops;
#endif

/// <summary>Constructor.</summary>
///
/// <remarks>Initializes the specified first and last years, resizes the data
/// array, and sets all data values to zero.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCARRAY1"/> if first year is out
/// of range (only in debug mode).</exception>
///
/// <param name="firstyr">New first year of data.</param>
/// <param name="lastyr">New last year of data.</param>
QcArray::QcArray( int firstyr, int lastyr ) :
firstYear((unsigned short)firstyr), lastYear((unsigned short)lastyr),
theData((lastyr - firstyr + 1) / QcPacked::NUM_QCS + 1)
{
#ifndef NDEBUG
  if (firstyr < YEAR37)
    throw PiaException(PIA_IDS_QCARRAY1);
#endif
}

/// <summary>Copy constructor.</summary>
///
/// <remarks>Copies all values from the argument to this instance.</summary>
///
/// <param name="qcArray">The QcArray to copy from.</param>
QcArray::QcArray( const QcArray& qcArray ) : firstYear(qcArray.firstYear),
lastYear(qcArray.lastYear), theData(qcArray.theData)
{ }

/// <summary>Zeroes out all data.</summary>
///
/// <param name="firstyr">First year to zero.</param>
/// <param name="lastyr">Last year to zero.</param>
void QcArray::deleteContents( int firstyr, int lastyr )
{
  const int year1 = max((int)firstYear, firstyr);
  const int year2 = min(lastyr, (int)lastYear);
  for (int yr = year1; yr <= year2; yr++) {
    set(yr, 0u);
  }
}

/// <summary>Converts a quarters of coverage array to a string.</summary>
///
/// <returns>A quarters of coverage array as a string.</returns>
///
/// <param name="firstyr">First year to write.</param>
/// <param name="lastyr">Last year to write.</param>
std::string QcArray::toString( int firstyr, int lastyr ) const
{
  ostringstream temp;
  const int year1 = max((int)firstYear, firstyr);
  const int year2 = min(lastyr, (int)lastYear);
  for (int yr = year1; yr <= year2; yr++) {
    temp << get(yr);
  }
  return temp.str();
}

/// <summary>Sets values for the specified years to one specified value.
/// </summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCARRAY1"/> if year is out of range
/// (only in debug mode).</exception>
///
/// <param name="value">Amount to use in each year.</param>
/// <param name="firstYeart">First year for which to <see cref="set"/> QCs.
/// </param>
/// <param name="lastYeart">Last year for which to <see cref="set"/> QCs.
/// </param>
void QcArray::assign( unsigned value, int firstYeart, int lastYeart )
{
#if !defined(NDEBUG)
  if (firstYeart < firstYear || lastYeart > lastYear) {
    throw PiaException(PIA_IDS_QCARRAY1);
  }
#endif
  for (int year = firstYeart; year <= lastYeart; year++) {
    const int yearInd = (year - firstYear) % QcPacked::NUM_QCS;
    const int dataInd = (year - firstYear) / QcPacked::NUM_QCS;
    theData[dataInd].set(yearInd, value);
  }
}

/// <summary>Sets values for the specified years from another
/// <see cref="QcArray"/>.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCARRAY1"/> if year is out of range
/// (only in debug mode).</exception>
///
/// <param name="firstYeart">First year for which to <see cref="set"/> QCs.
/// </param>
/// <param name="lastYeart">Last year for which to <see cref="set"/> QCs.
/// </param>
/// <param name="qcArray">The <see cref="QcArray"/> from which numbers of
/// quarters of coverage are gotten.</param>
void QcArray::assign( const QcArray &qcArray, int firstYeart, int lastYeart )
{
#if !defined(NDEBUG)
  if (firstYeart < firstYear || lastYeart > lastYear ||
    lastYeart > qcArray.lastYear) {
    throw PiaException(PIA_IDS_QCARRAY1);
  }
#endif
  for (int year = firstYeart; year <= lastYeart; year++) {
    const int yearInd = (year - firstYear) % QcPacked::NUM_QCS;
    const int dataInd = (year - firstYear) / QcPacked::NUM_QCS;
    theData[dataInd].set(yearInd, qcArray.theData[dataInd].get(yearInd));
  }
}

/// <summary>Sums quarters of coverage over a period of years or quarters.
/// </summary>
///
/// <returns>Sum of quarters of coverage.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCARRAY3"/> if years are out of range (only in debug
/// mode); of type <see cref="PIA_IDS_QCARRAY4"/> if quarters are out of range
/// (only in debug mode).</exception>
///
/// <param name="firstyr">First year to sum.</param>
/// <param name="lastyr">Last year to sum.</param>
/// <param name="startValue">Starting value.</param>
unsigned QcArray::accumulate( int firstyr, int lastyr,
unsigned startValue ) const
{
#ifndef NDEBUG
  if (firstyr < firstYear || lastyr < firstYear || firstyr > lastYear ||
    lastyr > lastYear)
    throw PiaException(PIA_IDS_QCARRAY3);
#endif
  for (int year = firstyr; year <= lastyr; year++) {
    const int yearInd = (year - firstYear) % QcPacked::NUM_QCS;
    const int dataInd = (year - firstYear) / QcPacked::NUM_QCS;
    startValue += theData[dataInd].get(yearInd);
  }
  return(startValue);
}

/// <summary>Sums quarters of coverage over a period of years or quarters.
/// </summary>
///
/// <returns>Sum of quarters of coverage.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCARRAY3"/> if years are out of range (only in debug
/// mode); of type <see cref="PIA_IDS_QCARRAY4"/> if quarters are out of range
/// (only in debug mode).</exception>
///
/// <param name="qtrYear1">First quarter and year to sum.</param>
/// <param name="qtrYear2">Last quarter and year to sum.</param>
/// <param name="startValue">Starting value.</param>
unsigned QcArray::accumulate( const QtrYear& qtrYear1,
const QtrYear& qtrYear2, unsigned startValue ) const
{
  const int year1 = (int)qtrYear1.getYear();
  const int year2 = (int)qtrYear2.getYear();
#ifndef NDEBUG
  qtrYear1.check();
  qtrYear2.check();
  if (year1 < firstYear || year2 < firstYear ||
    year1 > lastYear || year2 > lastYear)
    throw PiaException(PIA_IDS_QCARRAY4);
#endif
  if (qtrYear1 > qtrYear2)
    return(startValue);
  // handle case with two different years
  if (year1 < year2) {
    // start with qc's in first year, starting with first quarter of period
    startValue += min(get(year1), 4u - qtrYear1.getQuarter());
    // continue with full years (use =, not +=, because accumulate adds
    // onto starting value of startValue)
    startValue = accumulate(year1 + 1, qtrYear2.getYear() - 1, startValue);
    // end with qc's in last year, ending with last quarter of period
    startValue += min(get(year2), qtrYear2.getQuarter() + 1);
  }
  // handle case within same year
  else {
    startValue += min(qtrYear2.getQuarter() - qtrYear1.getQuarter() + 1,
      get(year1));
  }
  return(startValue);
}

/// <summary>Returns the number of quarters of coverage for one year.
/// </summary>
///
/// <returns>The number of quarters of coverage for one year.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCARRAY1"/> if year is out of range
/// (only in debug mode).</exception>
///
/// <param name="year">Year for which number of quarters of coverage is
/// desired.</param>
unsigned QcArray::get( int year ) const
{
#if !defined(NDEBUG)
  if (year < firstYear || year > lastYear)
    throw PiaException(PIA_IDS_QCARRAY1);
#endif
  const int index = (year - firstYear) / QcPacked::NUM_QCS;
  return theData[index].get(year - firstYear - QcPacked::NUM_QCS * index);
}

/// <summary>Sets the number of quarters of coverage for one year.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCARRAY1"/> if year is out of range
/// (only in debug mode).</exception>
///
/// <param name="year">Year for which number of quarters of coverage is set.
/// </param>
/// <param name="value">Number of quarters of coverage for this year.</param>
void QcArray::set( int year, unsigned value )
{
#if !defined(NDEBUG)
  if (year < firstYear || year > lastYear)
    throw PiaException(PIA_IDS_QCARRAY1);
#endif
  const int index = (year - firstYear) / QcPacked::NUM_QCS;
  theData[index].set(year - firstYear - QcPacked::NUM_QCS * index, value);
}
