// Functions for the <see cref="CatchupIncreases"/> and
// <see cref="Catchup"/> classes to manage catch-up benefit increases.
//
// $Id: cachup.cpp 1.20 2011/07/29 13:49:25EDT 044579 Development  $

#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include "cachup.h"
#include "oactcnst.h"
#include "Resource.h"
#include "PiaException.h"
#include "StringParser.h"

using namespace std;

/// <summary>Initializes catch-up benefit increase data.</summary>
CatchupIncreases::CatchupIncreases()
{
  fill(theData, theData + NUM_CATCHUP_INCREASES, 0.0);
}

/// <summary>Destroys catch-up benefit increase data.</summary>
CatchupIncreases::~CatchupIncreases()
{ }

/// <summary>Parses catch-up increases for one year of eligibility from a
/// string.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_READERR"/> if unable to parse all required data.
/// </exception>
///
/// <param name="str">String to parse.</param>
void CatchupIncreases::parseString( std::string str )
{
  vector< string > strvec = ::parseString(str);
  if (strvec.size() < NUM_CATCHUP_INCREASES) {
    throw PiaException(PIA_IDS_READERR);
  }
  for (int i = 0; i < NUM_CATCHUP_INCREASES; i++) {
    double dtemp = atof(strvec[i].c_str());
    Catchup::check(dtemp);
    set(i, dtemp);
  }
}

/// <summary>Returns catch-up increases for one year of eligibility as a
/// string.</summary>
///
/// <returns>Catch-up increases for one year of eligibility as a string.
/// </returns>
std::string CatchupIncreases::toString() const
{
  ostringstream strm;
  strm.precision(1);
  for (int i1 = 0; i1 < NUM_CATCHUP_INCREASES; i1++) {
    strm << setw(4) << theData[i1];
  }
  return strm.str();
}

/// <summary>Initializes catch-up benefit increase data with starting year 0.
/// </summary>
Catchup::Catchup() : cstart(0)
{ }

/// <summary>Initializes catch-up benefit increase data with specified
/// starting year.</summary>
///
/// <param name="newCstart">Starting year of catch-up benefit increases.
/// </param>
Catchup::Catchup( int newCstart ) : cstart(newCstart)
{ }

/// <summary>Destroys catch-up benefit increases.</summary>
Catchup::~Catchup()
{ }

/// <summary>Checks for existence of catch-up benefit increase.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_CACHUP4"/> if either argument
/// year is out of range (only in debug mode).</exception>
///
/// <returns>False if there is no catch-up increase, true otherwise.</returns>
///
/// <param name="eligYear">Year of eligibility for catch-up benefit
///  increases.</param>
/// <param name="calYear">Calendar year of catch-up benefit increase.</param>
bool Catchup::exist( int eligYear, int calYear ) const
{
#if !defined(NDEBUG)
  if (eligYear < 1900 || calYear < YEAR37)
    throw PiaException(PIA_IDS_CACHUP4);
#endif
  const int cstart2 = cstart + 2;
  if (calYear < cstart2 ||
    calYear >= cstart2 + CatchupIncreases::NUM_CATCHUP_INCREASES) {
    return(false);
  }
  if (eligYear < cstart || eligYear >= cstart + NUM_CATCHUP_ELIG_YEARS) {
    return(false);
  }
  if (catchup[eligYear - cstart].get(calYear - cstart2) < 0.05) {
    return(false);
  }
  return(true);
}

/// <summary>Sets one catch-up increase.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_CACHUP4"/> if either argument
///   year is out of range (only in debug mode).</exception>
///
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="calYear">Calendar year of increase.</param>
/// <param name="newCatchup">New value for catch-up increase.</param>
void Catchup::set( int eligYear, int calYear, double newCatchup )
{
#if !defined(NDEBUG)
  if (eligYear < 1900 || calYear < YEAR37)
    throw PiaException(PIA_IDS_CACHUP4);
#endif
  check(newCatchup);
  const int cstart2 = cstart + 2;
  if (eligYear >= cstart && eligYear < cstart + NUM_CATCHUP_ELIG_YEARS &&
    calYear >= cstart2 &&
    calYear < cstart2 + CatchupIncreases::NUM_CATCHUP_INCREASES) {
    catchup[eligYear - cstart].set(calYear - cstart2, newCatchup);
  }
}

/// <summary>Checks a catch-up benefit increase.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_CACHUP1"/> if catch-up benefit increase is out of range.
/// </exception>
///
/// <param name="newCatchup">Value to check.</param>
void Catchup::check( double newCatchup )
{
  if (newCatchup > 99.99 || newCatchup < 0.0)
    throw PiaException(PIA_IDS_CACHUP1);
}

/// <summary>Zeroes out catch-up benefit increases.</summary>
void Catchup::deleteContents()
{
  for (int i2 = 0; i2 < NUM_CATCHUP_ELIG_YEARS; i2++) {
    for (int i1 = 0; i1 < CatchupIncreases::NUM_CATCHUP_INCREASES; i1++) {
      catchup[i2].set(i1, 0.0);
    }
  }
}

/// <summary>Parses all catch-up increases for one year of eligibility
///   from a string.</summary>
///
/// <param name="index">Index of year of eligibility to parse (0-9).</param>
/// <param name="inputLine">Line to parse.</param>
void Catchup::parseString( int index, std::string inputLine )
{
  catchup[index].parseString(inputLine);
}
