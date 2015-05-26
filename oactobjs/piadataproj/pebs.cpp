// Functions for the <see cref="Pebs"/> class to handle Social Security
// Statement data.

// $Id: pebs.cpp 1.22 2012/09/24 16:36:03EDT 044579 Development  $

#pragma warning( disable : 4244 4127 )
#include "pebs.h"
#include "PiaException.h"
#include "Resource.h"

using namespace std;
#include "boost/date_time/gregorian/gregorian.hpp"

const Age Pebs::age70 = Age(70, 0);

/// <summary>Initializes Pebs instance.</summary>
///
/// <remarks>Initializes size of arrays to
/// <see cref="Pebs::pebes_type::MAXPEBS"/> and zeroes out all data.</remarks>
Pebs::Pebs() : benefitPebs(MAXPEBS), agePlan(0), agePlan2(0), mfbPebs(MAXPEBS),
monthnow(0), piaPebs(MAXPEBS), qcTotal(0), qcDisReq(0), qcDisTotal(0),
qcDisYears(0), qcReq(MAXPEBS), pebsOab(0), pebsDib(0)
{ }

/// <summary>Initializes all variables to default values.</summary>
void Pebs::deleteContents()
{
  agePlan = monthnow = 0;
}

/// <summary>Sets month of Statement processing with a specified month.
/// </summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PEBESMONTH"/> if month is out of range (only in debug
/// mode).</exception>
///
/// <param name="newMonthnow">New month of processing.</param>
void Pebs::setMonthnow( int newMonthnow )
{
#ifndef NDEBUG
  monthnowCheck(newMonthnow);
#endif
  monthnow = newMonthnow;
}

/// <summary>Sets month of Statement processing with the current month.
/// </summary>
void Pebs::setMonthnow()
{
  boost::gregorian::date datetemp = boost::gregorian::day_clock::local_day();
  setMonthnow(datetemp.month());
}

/// <summary>Sets planned age of retirement in Statement case.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_AGEPLAN"/> if planned age of retirement is out of
/// range (debug mode only).</exception>
///
/// <param name="newAgePlan">New planned age of retirement.</param>
void Pebs::setAgePlan( int newAgePlan )
{
#ifndef NDEBUG
  agePlanCheck(newAgePlan);
#endif
  agePlan = newAgePlan;
}

/// <summary>Checks for correct month of Statement processing, using the
/// stored month of processing.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PEBESMONTH"/> if month is out of range.</exception>
void Pebs::monthnowCheck() const
{
  monthnowCheck(monthnow);
}

/// <summary>Checks for correct month of Statement processing, using a
/// specified month of processing.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PEBESMONTH"/> if month is out of range.</exception>
///
/// <param name="monthnowt">Month to check.</param>
void Pebs::monthnowCheck( int monthnowt )
{
  if (monthnowt < boost::date_time::Jan || monthnowt > boost::date_time::Dec)
    throw PiaException(PIA_IDS_PEBESMONTH);
}

/// <summary>Checks planned age of retirement in Statement case.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_AGEPLAN"/> if planned age of retirement is out of
/// range.</exception>
///
/// <param name="agePlant">The age to check.</param>
void Pebs::agePlanCheck( int agePlant )
{
  if (agePlant < 0 || agePlant > 99)
    throw PiaException(PIA_IDS_AGEPLAN);
}
