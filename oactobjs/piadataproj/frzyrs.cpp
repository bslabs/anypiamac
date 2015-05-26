// Functions for the <see cref="FreezeYears"/> class to manage freeze period
// variables.
//
// $Id: frzyrs.cpp 1.7 2011/07/29 15:26:18EDT 044579 Development  $

#include "frzyrs.h"

/// <summary>Creates a FreezeYears.</summary>
///
/// <remarks>All years are initialized to 0.</remarks>
FreezeYears::FreezeYears () : year1(0), year2(0), year3(0), year4(0)
{ }

/// <summary>Returns true if any years in latest freeze period.</summary>
///
/// <returns>True if any years in latest freeze period.</returns>
bool FreezeYears::hasYears1() const
{
  return(year1 > 0 && year2 >= year1);
}

/// <summary>Returns true if any years in prior freeze period.</summary>
///
/// <returns>True if any years in prior freeze period.</returns>
bool FreezeYears::hasYears2() const
{
  return(year3 > 0 && year4 >= year3);
}

/// <summary>Returns true if year is wholly within a period of disability,
/// false otherwise.</summary>
///
/// <returns>True if year is wholly within a period of disability,
/// false otherwise.</returns>
///
/// <param name="year">Year considered.</param>
bool FreezeYears::isFreezeYear( int year ) const
{
  if (year >= year1 && year <= year2)
    return(true);
  if (year >= year3 && year <= year4)
    return(true);
  return(false);
}
