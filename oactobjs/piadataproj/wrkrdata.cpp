// Functions for the <see cref="WorkerData"/> class to manage worker data.

// $Id: wrkrdata.cpp 1.90 2011/08/11 13:55:35EDT 044579 Development  $

#include "wrkrdata.h"
#include "PiaException.h"
#include "Resource.h"
#include "DebugCase.h"
#include "oactcnst.h"
#include "DateFormatter.h"
#if defined(DEBUGCASE)
#include <sstream>
#include "Trace.h"
#endif

using namespace std;

/// <summary>Initializes all values.</summary>
///
/// <remarks>Should call <see cref="WorkerDataGeneral::setMaxyear"/> before
/// constructing any <see cref="WorkerData"/>.</remarks>
WorkerData::WorkerData() : WorkerDataGeneral(),
ssn(), milServDatesVec(), railRoadData(maxyear), earnOasdi(YEAR37, maxyear),
earnHi(YEAR37, maxyear), taxType(YEAR37, maxyear), nhname(""), pubpen(0.0f),
pubpenDate(), pubpenReservist(0.0f), oabEntDate(), oabCessDate()
{
  for (int i = 0; i < NUM_ADDRESS_LINES; i++) {
    nhaddr[i].resize(0);
  }
}

/// <summary>Destroys this structure.</summary>
WorkerData::~WorkerData( )
{ }

/// <summary>Initializes all variables to default values.</summary>
void WorkerData::deleteContents()
{
  WorkerDataGeneral::deleteContents();
  ssn.deleteContents();
  nhname.resize(0);
  for (int i = 0; i < NUM_ADDRESS_LINES; i++)
    nhaddr[i].resize(0);
  milServDatesVec.deleteContents();
  railRoadData.deleteContents();
  earnOasdi.fill(0.0);
  earnHi.fill(0.0);
  taxType.deleteContents();
  pubpen = pubpenReservist = 0.0f;
  pubpenDate.deleteContents();
  oabCessDate.deleteContents();
  oabEntDate.deleteContents();
}

/// <summary>Zeroes out earnings and quarters of coverage before first year
/// and after last year of specified earnings.</summary>
///
/// <remarks>Call <see cref="WorkerDataGeneral::zeroEarn"/> to zero out
/// quarters of coverage, then <see cref="zeroEarnOasdi"/> and
/// <see cref="zeroEarnHi"/> to zero out the earnings.</remarks>
///
/// <param name="ibegint">First year of specified earnings.</param>
/// <param name="iendt">Last year of specified earnings.</param>
void WorkerData::zeroEarn( int ibegint, int iendt )
{
  WorkerDataGeneral::zeroEarn(ibegint, iendt);
  zeroEarnOasdi(ibegint, iendt);
  zeroEarnHi(ibegint, iendt);
}

/// <summary>Zeroes out OASDI earnings before first year
/// and after last year of specified earnings.</summary>
///
/// <param name="ibegint">First year of specified earnings.</param>
/// <param name="iendt">Last year of specified earnings.</param>
void WorkerData::zeroEarnOasdi( int ibegint, int iendt )
{
  earnOasdi.assign(0.0, YEAR37, ibegint - 1);
  earnOasdi.assign(0.0, iendt + 1, maxyear);
}

/// <summary>Zeroes out HI earnings before first year and after last year of
/// specified earnings.</summary>
///
/// <param name="ibegint">First year of specified earnings.</param>
/// <param name="iendt">Last year of specified earnings.</param>
void WorkerData::zeroEarnHi( int ibegint, int iendt )
{
  earnHi.assign(0.0, YEAR37, ibegint - 1);
  earnHi.assign(0.0, iendt + 1, maxyear);
}

/// <summary>Sets date of cessation of oab prior to most recent dib.</summary>
///
/// <remarks>Sets date of cessation of oab to specified date, if there is at
/// least one period of disability. Otherwise the oab cessation date is zeroed
/// out (the argument date is ignored).</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DICESS1"/> if month of cessation is out of range; of
/// type <see cref="PIA_IDS_DICESS2"/> if year of cessation is before 1937; of
/// type <see cref="PIA_IDS_DICESS3"/> if year of cessation is after maximum
/// allowed. (All only in debug mode, and only if there is at least one period
/// of disability.</exception>
///
/// <param name="datetemp">The date to use.</param>
void WorkerData::setOabCessDate( const DateMoyr& datetemp )
{
  if (getValdi() > 0) {
#ifndef NDEBUG
    DisabPeriod::cessationDateCheck(datetemp);
#endif
    oabCessDate = datetemp;
  }
  else
    oabCessDate.deleteContents();
}

/// <summary>Sets date of oab entitlement prior to most recent dib.</summary>
///
/// <remarks>Sets date of entitlement of oab to specified date, if there is at
/// least one period of disability. Otherwise the oab entitlement date is
/// zeroed out (the argument date is ignored).</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DICESS1"/> if month of entitlement is out of range; of
/// type <see cref="PIA_IDS_DICESS2"/> if year of entitlement is before 1937;
/// of type <see cref="PIA_IDS_DICESS3"/> if year of entitlement is after
/// maximum allowed. (All only in debug mode, and only if there is at least
/// one period of disability.</exception>
///
/// <param name="datetemp">The date to use.</param>
void WorkerData::setOabEntDate( const DateMoyr& datetemp )
{
  if (getValdi() > 0) {
#ifndef NDEBUG
    DisabPeriod::cessationDateCheck(datetemp);
#endif
    oabEntDate = datetemp;
  }
  else
    oabEntDate.deleteContents();
}

/// <summary>Sets OASDI-covered earnings.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_EARNPEBS"/> if earnings are out
/// of range (only in debug mode).</exception>
///
/// <param name="year">Year of earnings.</param>
/// <param name="newEarn">Amount of earnings.</param>
void WorkerData::setEarnOasdi( int year, double newEarn )
{
#ifndef NDEBUG
  try {
    WageBaseGeneral::check(newEarn);
  } catch (PiaException e) {
    throw PiaException(PIA_IDS_EARNPEBS);
  }
#endif
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm.setf(ios::fixed, ios::floatfield);
    strm.precision(2);
    strm << getIdString() << ", year " << year << ", earnings " << newEarn
      << ": Setting OASDI earnings";
    Trace::writeLine(strm.str());
  }
#endif
  earnOasdi[year] = newEarn;
}

/// <summary>Sets Medicare earnings for the specified year.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_EARNHI"/> if earnings are out
/// of range (only in debug mode).</exception>
///
/// <param name="year">Year of earnings.</param>
/// <param name="newEarnHi">Amount of earnings.</param>
void WorkerData::setEarnHi( int year, double newEarnHi )
{
#ifndef NDEBUG
  try {
    WageBaseGeneral::check(newEarnHi);
  } catch (PiaException e) {
    throw PiaException(PIA_IDS_EARNHI);
  }
#endif
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm.setf(ios::fixed, ios::floatfield);
    strm.precision(2);
    strm << getIdString() << ", year " << year << ", earnings " << newEarnHi
      << ": Setting HI earnings";
    Trace::writeLine(strm.str());
  }
#endif
  earnHi[year] = newEarnHi;
}

/// <summary>Sets type of taxes for the specified year.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_TAXTYPEYR"/> if year is out of range (only in debug
/// mode).</exception>
///
/// <param name="year">Year of taxes.</param>
/// <param name="newTaxType">Type of taxes.</param>
void WorkerData::setTaxType( int year, int newTaxType )
{
#ifndef NDEBUG
  if (year < YEAR37)
    throw PiaException(PIA_IDS_TAXTYPEYR);
  taxTypeCheck(newTaxType);
#endif
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm << getIdString() << ", year " << year << ", tax type " << newTaxType
      << ": Setting tax type";
    Trace::writeLine(strm.str());
  }
#endif
  taxType.setBit(year, newTaxType);
}

/// <summary>Sets amount of monthly noncovered pension.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PUBPEN"/> if pension is out
/// of range (only in debug mode).</exception>
///
/// <param name="newPubpen">New amount of monthly noncovered pension.</param>
void WorkerData::setPubpen( float newPubpen )
{
#ifndef NDEBUG
  pubpenCheck(newPubpen);
#endif
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm.setf(ios::fixed, ios::floatfield);
    strm.precision(2);
    strm << getIdString() << ", pubpen " <<newPubpen << ": Setting pubpen";
    Trace::writeLine(strm.str());
  }
#endif
  pubpen = newPubpen;
}

/// <summary>Checks the date of noncovered pension entitlement for
/// consistency with date of birth and date of death.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_NONPENDATE1"/> or <see cref="PIA_IDS_NONPENDATE2"/>
/// if public pension entitlement date is out of range (before date of
/// birth or after date of death).</exception>
void WorkerData::pubpenDateCheck2() const
{
  pubpenDateCheck(pubpenDate, getBirthDate(), getDeathDate());
}

/// <summary>Sets date of noncovered pension entitlement.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_NONPENDATE1"/> or <see cref="PIA_IDS_NONPENDATE2"/>
/// if public pension entitlement date is out of range (before date of
/// birth or after date of death).</exception>
///
/// <param name="newPubpenDate">New date of noncovered pension entitlement.
/// </param>
void WorkerData::setPubpenDate( const DateMoyr& newPubpenDate )
{
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm << getIdString() << ", pubpenDate "
      << DateFormatter::toString(newPubpenDate) << ": Setting pubpenDate";
    Trace::writeLine(strm.str());
  }
#endif
  pubpenDateCheck(newPubpenDate, getBirthDate(), getDeathDate());
  pubpenDate = newPubpenDate;
}

/// <summary>Sets amount of monthly noncovered pension after December 1994,
/// after removing military reservist pension.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PUBPEN"/> if pension is out of range
/// (only in debug mode).</exception>
///
/// <param name="newPubpen">New amount of noncovered pension after
///  December 1994.</param>
void WorkerData::setPubpenReservist( float newPubpen )
{
#ifndef NDEBUG
  pubpenCheck(newPubpen);
#endif
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm.setf(ios::fixed, ios::floatfield);
    strm.precision(2);
    strm << getIdString() << ", pubpen " << newPubpen
      << ": Setting pubpenReservist";
    Trace::writeLine(strm.str());
  }
#endif
  pubpenReservist = newPubpen;
}
