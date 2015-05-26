// Functions for the <see cref="MilServDatesVec"/> class to manage an
// array of military service dates records.

// $Id: msdatesvec.cpp 1.20 2011/08/01 11:38:37EDT 044579 Development  $

#include "MilServDatesVec.h"
#include "PiaException.h"
#include "Resource.h"

using namespace std;

/// <summary>Creates an array with a zero record count.</summary>
MilServDatesVec::MilServDatesVec() : msCount(0)
{ }

/// <summary>Destroys this instance.</summary>
MilServDatesVec::~MilServDatesVec()
{ }

/// <summary>Checks a multiline record.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_MILSERV1"/> if starting date is after ending date in
/// any <see cref="MilServDates"/>.</exception>
void MilServDatesVec::check() const
{
  for (unsigned i = 0; i < getMSCount(); i++) {
    msdates[i].check();
  }
}

/// <summary>Returns pre-1951 quarters of coverage in a multiline record.
/// </summary>
///
/// <returns>Pre-1951 quarters of coverage in a multiline record.</returns>
unsigned MilServDatesVec::getQc3750() const
{
  unsigned rv = 0;  // return value
  for (unsigned i = 0; i < getMSCount(); i++) {
    rv += msdates[i].getQc3750();
  }
  return(rv);
}

/// <summary>Returns quarters of coverage for one year in a multiline record.
/// </summary>
///
/// <returns>Quarters of coverage for one year in a multiline record.
/// </returns>
///
/// <param name="year">Year for which quarters are desired.</param>
unsigned MilServDatesVec::getQcov( unsigned year ) const
{
  unsigned rv = 0;  // return value
  for (unsigned i = 0; i < getMSCount(); i++) {
    rv += msdates[i].getQcov(year);
  }
  return(min(rv, 4u));
}

/// <summary>Returns pre-1951 wage credits in a multiline record.</summary>
///
/// <returns>Pre-1951 wage credits in a multiline record.</returns>
double MilServDatesVec::getEarn3750() const
{
  double rv = 0.0;  // return value
  for (unsigned i = 0; i < getMSCount(); i++) {
    rv += msdates[i].getEarn3750();
  }
  return(rv);
}

/// <summary>Returns wage credits for one year in a multiline record.
/// </summary>
///
/// <returns>Wage credits for one year in a multiline record.</returns>
///
/// <param name="year">Year for which wage credits are desired.</param>
double MilServDatesVec::getEarn( unsigned year ) const
{
  double rv = 0.0;  // return value
  for (unsigned i = 0; i < getMSCount(); i++) {
    rv += msdates[i].getEarn(year);
  }
  // eliminate any possible double-counting
  if (year > 1956) {
    return min(rv, 1200.0);
  }
  else {
    return min(rv, 1920.0);
  }
}

/// <summary>Zeroes out dates of military service.</summary>
void MilServDatesVec::deleteContents()
{
  msCount = 0;
  for (unsigned i = 0; i < MAX_MILSERVDATES; i++) {
    msdates[i].deleteContents();
  }
}

/// <summary>Sets the number of military service records.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_MILSERV2"/> if number of military
///   service periods is out of range.</exception>
///
/// <param name="newMSCount">New number of military service records.</param>
void MilServDatesVec::setMSCount( unsigned newMSCount )
{
  msCountCheck(newMSCount);
  msCount = newMSCount;
}

/// <summary>Checks the number of military service periods.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_MILSERV2"/> if number of military service periods is
/// out of range.</exception>
///
/// <param name="newMSCount">Number of military service periods.</param>
void MilServDatesVec::msCountCheck( unsigned newMSCount )
{
  if (newMSCount > MAX_MILSERVDATES)
    throw PiaException(PIA_IDS_MILSERV2);
}
