// Functions for <see cref="Age"/> class to manage age in years and months.

// $Id: age.cpp 1.25 2010/12/09 16:51:43EST 044579 Development  $

#include <sstream>
#include <iomanip>
#include "age.h"
#include "PiaException.h"
#include "Resource.h"

using namespace std;

/// <summary>Compares (years,months) ages.</summary>
///
/// <returns>True if this age is less than argument, false otherwise.</returns>
///
/// <param name="newAge">The age to compare this one to.</param>
bool Age::operator<( const Age& newAge ) const
{
  // check years
  if (yearsOfAge != newAge.yearsOfAge) {
    return(yearsOfAge < newAge.yearsOfAge);
  }
  else {
    // same years, so check months
    return(monthsOfAge < newAge.monthsOfAge);
  }
}

/// <summary>Calculates a number of months by subtracting argument age from
/// this one.</summary>
///
/// <returns>The calculated number of months.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> if argument is
/// greater than this instance.</exception>
///
/// <param name="newAge">The age to subtract from this one.</param>
int Age::operator-( const Age& newAge ) const
{
  if (*this < newAge) {
    throw PiaException("Ages out of order in Age::operator-");
  }
  return (toMonths() - newAge.toMonths());
}

/// <summary>Checks for months in bounds.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_AGEMONTH"/> if months is greater than 11.</exception>
void Age::monthsCheck() const
{
  if (getMonths() > 11)
    throw PiaException(PIA_IDS_AGEMONTH);
}

/// <summary>Returns string with age as years and months.</summary>
///
/// <returns>A string such as "65 and 6 months".</returns>
std::string Age::toString() const
{
  monthsCheck();
  ostringstream strm;
  strm << getYears() << " and " << getMonths()
    << ((getMonths() == 1) ? " month" : " months");
  return(strm.str());
}

/// <summary>Returns formatted string with age as years and months, suitable
/// for a fixed-width record.</summary>
///
/// <returns>A seventeen character string such as " 65 and  6 months".</returns>
///
/// <remarks>If the format string begins with "f", create a fixed-width string;
/// otherwise create a string with no extra spaces.</remarks>
///
/// <param name="format">Format to use.</param>
std::string Age::toString( const std::string& format ) const
{
  if (format.length() < 1)
    return toString();
  switch (format[0]) {
  case 'f':
    {
      monthsCheck();
      ostringstream strm;
      strm << setfill(' ') << setw(3) << getYears() << " and " << setw(2)
        << getMonths() << ((getMonths() == 1) ? " month " : " months");
      return(strm.str());
    }
  default:
    return toString();
  }
}
