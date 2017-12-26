// Functions for the <see cref="RailRoadData"/> class to manage edited
// railroad record.

// $Id: railroad.cpp 1.40 2017/12/12 09:21:43EST 277133 Development  $

#include <sstream>
#include <iomanip>
#include <algorithm>
#include "railroad.h"
#include "oactcnst.h"

using namespace std;

/// <summary>Initializes last year of railroad data.</summary>
///
/// <remarks> First year is always 1937, for simplicity.</remarks>
///
/// <param name="year">New last year of data.</param>
RailRoadData::RailRoadData( int year ) : qc3750(0), earn3750(0),
firstYear(YEAR37), lastYear(YEAR37), qcArray(YEAR37, 1977),
earningsArray(YEAR37, year)
{ }

/// <summary>Destroys railroad data.</summary>
RailRoadData::~RailRoadData()
{ }

/// <summary>Sets this instance equal to another instance.</summary>
///
/// <returns>A reference to this instance.</returns>
///
/// <param name="newData">The instance to copy from.</param>
RailRoadData& RailRoadData::operator=( const RailRoadData& newData )
{
  qc3750 = newData.qc3750;
  earn3750 = newData.earn3750;
  firstYear = newData.firstYear;
  lastYear = newData.lastYear;
  qcArray = newData.qcArray;
  const DoubleAnnual& da = newData.earningsArray;
  earningsArray.assign(da, da.getBaseYear(), da.getLastYear());
  return (*this);
}

/// <summary>Zeroes out all railroad data.</summary>
void RailRoadData::deleteContents()
{
  setQc3750(0);
  qcArray.deleteContents();
  setEarn3750(0.0);
  earningsArray.fill(0.0);
  firstYear = YEAR37;
  lastYear = YEAR37;
}

/// <summary>Parses one line of earnings.</summary>
///
/// <remarks>The decade indicator can be one of the following:
/// <table>
///   0     first 10 years.
///   1     second 10 years.
///   etc.  up to eighth 10 years.
/// </table></remarks>
///
/// <param name="decade">Decade to use.</param>
/// <param name="line">Line to parse.</param>
/// <param name="width">Width of each earnings field.</param>
void RailRoadData::parseEarnings( int decade, const std::string& line,
int width )
{
  const int startyear = firstYear + 10 * decade;
  const int endyear = min(startyear + 9, lastYear);
  for (int yr = startyear; yr <= endyear; yr++) {
    earningsArray[yr] =
      atof(line.substr((yr - startyear) * width, width).c_str());
  }
}

/// <summary>Parses line of qcs.</summary>
///
/// <param name="line">Line to parse.</param>
void RailRoadData::parseQcs( const std::string& line )
{
  for (int yr = YEAR51; yr <= 1977; yr++) {
    qcArray.set(yr,
      static_cast<unsigned>(atoi(line.substr(yr - YEAR51, 1).c_str())));
  }
}

/// <summary>Returns string of qcs.</summary>
///
/// <returns>String of qcs.</returns>
std::string RailRoadData::getQcString() const
{
  ostringstream strm;
  for (int yr = YEAR51; yr <= 1977; yr++) {
    strm << qcArray.get(yr);
  }
  return(strm.str());
}

/// <summary>Returns true if there are any railroad earnings.</summary>
///
/// <returns>True if there are any railroad earnings.</returns>
bool RailRoadData::isNonzero() const
{
  if (earn3750 > 0.0)
    return(true);
  for (int yr = YEAR51; yr <= earningsArray.getLastYear(); yr++) {
    if (earningsArray[yr] > 0.0)
      return(true);
  }
  return(false);
}

/// <summary>Returns number of decades of post-1950 earnings.</summary>
///
/// <returns>Number of decades of post-1950 earnings.</returns>
int RailRoadData::getDecades() const
{
  return (lastYear > 0) ? (1 + (lastYear - firstYear) / 10) : 0;
}

/// <summary>Parses line of first and last years.</summary>
///
/// <param name="line">Line to parse.</param>
void RailRoadData::parseYears( const std::string& line )
{
  firstYear = atoi(line.substr(0, 4).c_str());
  lastYear = atoi(line.substr(4, 4).c_str());
}

/// <summary>Parses line of pre-1951 QCs and earnings.</summary>
///
/// <param name="line">Line to parse.</param>
/// <param name="width">Width of each earnings field.</param>
void RailRoadData::parse3750( const std::string& line, int width )
{
  setQc3750(atoi(line.substr(0, 2).c_str()));
  setEarn3750(atof(line.substr(2, width).c_str()));
}

/// <summary>Returns string of decade of earnings.</summary>
///
/// <returns>String of decade of earnings.</returns>
///
/// <param name="decade">Decade to use.
///     0:  first 10 years.
///     1:  second 10 years.
///     etc. up to eighth 10 years.</param>
/// <param name="width">Width of each earnings field.</param>
/// <param name="decimals">Number of decimals in each earnings field.</param>
std::string RailRoadData::getDecadeEarningsString( int decade, int width,
int decimals ) const
{
  ostringstream strm;
  strm.setf(ios::fixed,ios::floatfield);
  strm.precision(decimals);
  const int startyear = firstYear + 10 * decade;
  const int endyear = min(startyear + 9, lastYear);
  for (int yr = startyear; yr <= endyear; yr++) {
    strm << setw(width) << earningsArray[yr];
  }
  return(strm.str());
}

/// <summary>Returns string of pre-1951 QCs and earnings.</summary>
///
/// <returns>String of pre-1951 QCs and earnings.</returns>
///
/// <param name="width">Width of each earnings field.</param>
/// <param name="decimals">Number of decimals in each earnings field.</param>
std::string RailRoadData::get3750String( int width, int decimals ) const
{
  ostringstream strm;
  strm.precision(decimals);
  if (getEarn3750() > 0.) {
    strm << setw(2) << getQc3750() << setw(width) << getEarn3750();
  }
  return(strm.str());
}

/// <summary>Returns string of first and last years of earnings.</summary>
///
/// <returns>String of first and last years of earnings.</returns>
std::string RailRoadData::getYearsString() const
{
  ostringstream strm;
  if (getLastYear() > 1950) {
    strm << setw(4) << getFirstYear() << setw(4) << getLastYear();
  }
  return(strm.str());
}

    /// <summary>Checks the year of railroad earnings.</summary>
    /// <exception cref="PiaException">If year of railroad earnings is out of
    /// range.</exception>
    /// <param name="year">Year of railroad earnings to check.</param>
void RailRoadData::railroadYearCheck( int year)
{
  if (year < YEAR37 || year > YEAR2100) {
    throw new PiaException(
      "Year of railroad earnings must be between 1951 and 2100");
  }
}
