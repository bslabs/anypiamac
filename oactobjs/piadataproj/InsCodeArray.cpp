// Functions for the <see cref="InsCodeArray"/> class to manage an array
// of insured status codes.
//
// $Id: InsCodeArray.cpp 1.18 2011/07/29 15:29:07EDT 044579 Development  $

#include "InsCodeArray.h"
#include <utility>  // for rel_ops

using namespace std;
using namespace std::rel_ops;

/// <summary>Initializes the first and last years.</summary>
///
/// <remarks>Resizes the data array to cover the specified span of years.
/// </remarks>
///
/// <param name="newFirstYear">New first year of data.</param>
/// <param name="newLastYear">New last year of data.</param>
InsCodeArray::InsCodeArray( int newFirstYear, int newLastYear ) :
theCodes(newLastYear - newFirstYear + 1), firstYear(newFirstYear),
lastYear(newLastYear)
{ }

/// <summary>Zeroes out all data values.</summary>
void InsCodeArray::deleteContents()
{
  // Iterator is used to loop through the vector.
  vector< InsCode >::iterator theIterator;
  for (theIterator = theCodes.begin(); theIterator != theCodes.end();
    theIterator++) {
    *theIterator = InsCode::NOQCS;
  }
}

/// <summary>Sets the data for each year from the values in the argument
/// array.</summary>
///
/// <param name="newDataArray">Array of new values of data.</param>
/// <param name="firstDataYear">First year of data to set.</param>
/// <param name="lastDataYear">Last year of data to set.</param>
void InsCodeArray::assign( const InsCodeArray& newDataArray,
  int firstDataYear, int lastDataYear )
{
  int year1 = max(firstYear, newDataArray.firstYear);
  year1 = max(year1, firstDataYear);
  int year2 = min(lastYear, newDataArray.lastYear);
  year2 = min(year2, lastDataYear);
  copy(newDataArray.theCodes.begin() + (year1 - newDataArray.firstYear),
    newDataArray.theCodes.begin() + (year2 + 1 - newDataArray.firstYear),
    theCodes.begin() + (year1 - firstYear));
}

/// <summary>Sets the data for specified years to one specified value.
/// </summary>
///
/// <param name="firstDataYear">First year of data to set.</param>
/// <param name="lastDataYear">Last year of data to set.</param>
/// <param name="newData">New value of data to be used for all specified
///  years.</param>
void InsCodeArray::assign( const InsCode& newData, int firstDataYear,
int lastDataYear )
{
  int year1 = max(firstYear, firstDataYear);
  int year2 = min(lastYear, lastDataYear);
  fill(theCodes.begin() + year1 - firstYear,
    theCodes.begin() + year2 + 1 - firstYear, newData);
}

/// <summary>Returns true if any insured status code is other than
/// <see cref="InsCode::InsCodeType::NOQCS"/>.</summary>
///
/// <returns>True if any insured status code is other than
/// <see cref="InsCode::InsCodeType::NOQCS"/>.</returns>
bool InsCodeArray::isNonzero() const
{
  for (int i = firstYear; i <= lastYear; i++) {
    if (theCodes[i - firstYear].get() != InsCode::NOQCS)
      return(true);
  }
  return(false);
}

/// <summary>Returns data for one insured status code array as a string.
/// </summary>
///
/// <returns>Data for one insured status code array as a string.</returns>
std::string InsCodeArray::toString() const
{
  string s;
  for (int i = 0; i <= lastYear - firstYear; i++)
    s += theCodes[i].toChar();
  return(s);
}
