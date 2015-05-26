// Functions for the <see cref="WorkerDataGeneral"/> class - parent of
// classes to manage worker data.

// $Id: WorkerDataGeneral.cpp 1.59 2011/08/11 13:55:34EDT 044579 Development  $

#include <utility>  // for rel_ops
#include "WorkerDataGeneral.h"
#include "oactcnst.h"
#include "wbgenrl.h"
#include "PiaException.h"
#include "Resource.h"
#include "DebugCase.h"
#if defined(DEBUGCASE)
#include <sstream>
#include "Trace.h"
#include "DateFormatter.h"
#endif

using namespace std;
#if !defined(__SGI_STL_INTERNAL_RELOPS)
using namespace std::rel_ops;
#endif

// Maximum year of projection is 2100, unless changed.
int WorkerDataGeneral::maxyear = YEAR2100;
// Quarters of coverage are summed before 1978, unless changed.
int WorkerDataGeneral::qclumpyear = 1977;

/// <summary>Initializes a WorkerDataGeneral structure with default values.
/// </summary>
///
/// <remarks>Should call <see cref="WorkerDataGeneral::setMaxyear"/> before
/// constructing any WorkerDataGeneral.</remarks>
WorkerDataGeneral::WorkerDataGeneral() :
childCareYears(YEAR37, maxyear), qc(YEAR37, maxyear), qctot51td(0),
qctottd(0), joasdi(NO_BEN), valdi(0), ibegin(0), iend(0),
theBits(string("00010000001")), birthDate(), benefitDate(), entDate(),
deathDate()
{ }

/// <summary>Initializes a WorkerDataGeneral structure by copying all the data
/// in the argument to this instance.</summary>
///
/// <remarks>Should call <see cref="WorkerDataGeneral::setMaxyear"/> before
/// constructing any WorkerDataGeneral.</remarks>
///
/// <param name="workerData">The structure to copy from.</param>
WorkerDataGeneral::WorkerDataGeneral( const WorkerDataGeneral& workerData ) :
childCareYears(workerData.childCareYears), qc(workerData.qc),
qctot51td(workerData.qctot51td), qctottd(workerData.qctottd),
joasdi(workerData.joasdi), valdi(workerData.valdi), ibegin(workerData.ibegin),
iend(workerData.iend), theBits(workerData.theBits),
birthDate(workerData.birthDate), benefitDate(workerData.benefitDate),
entDate(workerData.entDate), deathDate(workerData.deathDate)
{
  for (int i = 0; i < NUM_DISAB_PERIODS; i++) {
    disabPeriod[i] = workerData.disabPeriod[i];
  }
}

/// <summary>Destroys this structure.</summary>
WorkerDataGeneral::~WorkerDataGeneral( )
{ }

/// <summary>Initializes all variables to default values.</summary>
void WorkerDataGeneral::deleteContents()
{
  zeroOasdi();
  valdi = 0;
  ibegin = iend = 0;
  qctottd = qctot51td = 0;
  theBits.reset();
  theBits[SEX_BIT] = Sex::FEMALE;
  theBits[INDEARN_BIT] = true;
  birthDate = boost::gregorian::date(boost::date_time::not_a_date_time);
  benefitDate.deleteContents();
  entDate.deleteContents();
  deathDate = boost::gregorian::date(boost::date_time::not_a_date_time);
  for (int i = 0; i < 2; i++) {
    disabPeriod[i].deleteContents();
  }
  qc.deleteContents();
  childCareYears.deleteContents();
}

/// <summary>Checks for first year of earnings within bounds, using passed
/// values.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BEGIN1"/> if first year of earnings is out of range.
/// </exception>
///
/// <param name="ibegint">First year of earnings.</param>
/// <param name="ibirtht">Year of birth.</param>
void WorkerDataGeneral::ibeginCheck( int ibegint, int ibirtht )
{
  // find first possible year of earnings
  const int year1 = max(YEAR37, ibirtht);
  if (ibegint < year1)
    throw PiaException(PIA_IDS_BEGIN1);
}

/// <summary>Sets first month of waiting period to the first month after
/// onset.</summary>
///
/// <remarks>This version sets the first month of the waiting period to the
/// first month after onset. It does that by calling
/// <see cref="DisabPeriod::waitperDateCal"/> and passing the resulting date
/// as an argument to the version of this function with two arguments.
/// </remarks>
///
/// <param name="index">Which disability period to use (0 for most recent,
/// 1 for next most recent).</param>
void WorkerDataGeneral::setWaitperDate( unsigned index )
{
  if (valdi > index) {
    DateMoyr waitpert = disabPeriod[index].waitperDateCal();
    if (waitpert != disabPeriod[index].getWaitperDate()) {
      setWaitperDate(index, waitpert);
    }
  }
}

/// <summary>Sets first month of waiting period to the specified date.
/// </summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_WAITPD1"/> if bad month; of type
/// <see cref="PIA_IDS_WAITPD2"/> if year is before 1940; of type
/// <see cref="PIA_IDS_WAITPD3"/> if year is after maximum allowed. (All only
/// in debug mode.)</exception>
///
/// <param name="index">Which disability period to use (0 for most recent,
/// 1 for next most recent).</param>
/// <param name="dateMoyr">The date to use.</param>
void WorkerDataGeneral::setWaitperDate( unsigned index,
 const DateMoyr& dateMoyr )
{
  if (valdi > index) {
#ifndef NDEBUG
     DisabPeriod::waitperDateCheck(dateMoyr);
#endif
#if defined(DEBUGCASE)
    if (isDebugPid(getIdNumber())) {
      ostringstream strm;
      strm << getIdString() << ", index " << index << ", dateMoyr "
        << DateFormatter::toString(dateMoyr) << ": Setting waitperDate";
      Trace::writeLine(strm.str());
    }
#endif
    disabPeriod[index].setWaitperDate(dateMoyr);
  }
}

/// <summary>Sets date of disability onset.</summary>
///
/// <param name="index">Which disability period to use (0 for most recent,
///  1 for next most recent).</param>
/// <param name="dateModyyr">The date to use.</param>
void WorkerDataGeneral::setOnsetDate( unsigned index,
 const boost::gregorian::date& dateModyyr )
{
#ifndef NDEBUG
  DisabPeriod::onsetDateCheck(dateModyyr);
#endif
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm << getIdString() << ", index " << index << ", dateModyyr "
      << DateFormatter::toString(dateModyyr) << ": Setting onsetDate";
    Trace::writeLine(strm.str());
  }
#endif
  disabPeriod[index].setOnsetDate(dateModyyr);
}

/// <summary>Sets date of disability cessation.</summary>
///
/// <param name="index">Which disability period to use (0 for most recent,
/// 1 for next most recent).</param>
/// <param name="dateMoyr">The date to use.</param>
void WorkerDataGeneral::setCessationDate( unsigned index,
 const DateMoyr& dateMoyr )
{
  if ((index == 1 && valdi > 1) || (index == 0 && needDiCess())) {
#ifndef NDEBUG
    DisabPeriod::cessationDateCheck(dateMoyr);
#endif
#if defined(DEBUGCASE)
    if (isDebugPid(getIdNumber())) {
      ostringstream strm;
      strm << getIdString() << ", index " << index << ", dateMoyr "
        << DateFormatter::toString(dateMoyr) << ": Setting cessationDate";
      Trace::writeLine(strm.str());
    }
#endif
    disabPeriod[index].setCessationDate(dateMoyr);
  }
  else {
#if defined(DEBUGCASE)
    if (isDebugPid(getIdNumber())) {
      ostringstream strm;
      strm << getIdString() << ", index " << index
        << ": Zeroing out cessationDate";
      Trace::writeLine(strm.str());
    }
#endif
    disabPeriod[index].deleteCessationDate();
  }
}

/// <summary>Sets pia at disability cessation.</summary>
///
/// <param name="index">Which disability period to use (0 for most recent,
/// 1 for next most recent).</param>
/// <param name="newPia">The pia to use.</param>
void WorkerDataGeneral::setCessationPia( unsigned index, float newPia )
{
  if ((index == 1 && valdi > 1) || (index == 0 && needDiCess())) {
#if defined(DEBUGCASE)
    if (isDebugPid(getIdNumber())) {
      ostringstream strm;
      strm.setf(ios::fixed, ios::floatfield);
      strm.precision(2);
      strm << getIdString() << ", index " << index << ", newPia "
        << newPia << ": Setting cessationPia";
      Trace::writeLine(strm.str());
    }
#endif
    disabPeriod[index].setCessationPia(newPia);
  }
}

/// <summary>Sets mfb at disability cessation.</summary>
///
/// <param name="index">Which disability period to use (0 for most
///     recent, 1 for next most recent).</param>
/// <param name="newMfb">The mfb to use.</param>
void WorkerDataGeneral::setCessationMfb( unsigned index, float newMfb )
{
  if ((index == 1 && valdi > 1) || (index == 0 && needDiCess())) {
#if defined(DEBUGCASE)
    if (isDebugPid(getIdNumber())) {
      ostringstream strm;
      strm.setf(ios::fixed, ios::floatfield);
      strm.precision(2);
      strm << getIdString() << ", index " <<index << ", newMfb "
        << newMfb << ": Setting cessationMfb";
      Trace::writeLine(strm.str());
    }
#endif
    disabPeriod[index].setCessationMfb(newMfb);
  }
}

/// <summary>Checks date of disability onset, using passed values.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_ONSET5"/> if disability is before birth; of type
/// <see cref="PIA_IDS_ONSET6"/> if disability is after entitlement in a
/// life case or after benefit date in a survivor case; of type
/// <see cref="PIA_IDS_ONSET7"/> if death of worker is before onset in a
/// survivor case.</exception>
///
/// <param name="onsetDatet">The date of onset.</param>
/// <param name="deathDatet">The date of death.</param>
/// <param name="valdit">Number of periods of disability.</param>
/// <param name="birthDatet">The date of birth.</param>
/// <param name="ioasdi">Type of benefit.</param>
/// <param name="entDatet">The date of entitlement.</param>
/// <param name="benDatet">The date of benefit.</param>
void WorkerDataGeneral::disCheck( const boost::gregorian::date& onsetDatet,
const boost::gregorian::date& deathDatet, int valdit,
const boost::gregorian::date& birthDatet, ben_type ioasdi,
const DateMoyr& entDatet, const DateMoyr& benDatet )
{
  // do not bother checking if there are no periods of disability
  if (valdit > 0) {
    // check for disability before birth
    disBirthEntCheck(onsetDatet, birthDatet, ioasdi, entDatet, benDatet);
    // check for disability after death
    disDeathCheck(onsetDatet, deathDatet, ioasdi);
  }
}

/// <summary>Checks date of disability onset against dates of birth,
/// entitlement, and benefit.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_ONSET5"/> if disability is before birth.
/// <see cref="PiaException"/> of type <see cref="PIA_IDS_ONSET6"/>
/// if disability is after entitlement.</exception>
///
/// <param name="onsetDatet">The date of onset.</param>
/// <param name="birthDatet">The date of birth.</param>
/// <param name="ioasdi">Type of benefit.</param>
/// <param name="entDatet">The date of entitlement.</param>
/// <param name="benDatet">The date of benefit.</param>
void WorkerDataGeneral::disBirthEntCheck(
const boost::gregorian::date& onsetDatet,
const boost::gregorian::date& birthDatet, ben_type ioasdi,
const DateMoyr& entDatet, const DateMoyr& benDatet )
{
  if (onsetDatet.is_not_a_date()) {
    throw PiaException(PIA_IDS_ONSET5);
  }
  if (birthDatet.is_not_a_date()) {
    throw PiaException(PIA_IDS_ONSET5);
  }
  if (onsetDatet < birthDatet) {
    // disability before birth
    throw PiaException(PIA_IDS_ONSET5);
  }
  if (ioasdi != NO_BEN) {
    if (ioasdi == SURVIVOR) {
      if (benDatet < onsetDatet)
        // disability after benefit date
        throw PiaException(PIA_IDS_ONSET6);
    }
    else {
      if (entDatet < onsetDatet)
        // disability after entitlement
        throw PiaException(PIA_IDS_ONSET6);
    }
  }
}

/// <summary>Checks date of prior disability onset, using passed values.
/// </summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_ONSET5"/> if disability is before birth; of type
/// <see cref="PIA_IDS_ONSET8"/> if prior disability is after current
/// disability.</exception>
///
/// <param name="onset1Datet">The date of prior disability onset.</param>
/// <param name="onsetDatet">The date of most recent disability onset.</param>
/// <param name="valdit">Number of periods of disability.</param>
/// <param name="birthDatet">The date of birth.</param>
void WorkerDataGeneral::dis1Check( const boost::gregorian::date& onset1Datet,
const boost::gregorian::date& onsetDatet, int valdit,
const boost::gregorian::date& birthDatet )
{
  if (valdit > 1) {
    if (onset1Datet < birthDatet)
      // disability before birth
      throw PiaException(PIA_IDS_ONSET5);
    if (onsetDatet < onset1Datet)
      // prior disability after current disability
      throw PiaException(PIA_IDS_ONSET8);
  }
}

/// <summary>Checks date of disability cessation, using passed values.
/// </summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DICESS4"/> if cesation is before
/// onset. <see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DICESS6"/> if entitlement is before
/// cessation. <see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DICESS7"/> if cessation is
/// after death in a survivor case.</exception>
///
/// <param name="cessationDatet">The date of disability cessation.</param>
/// <param name="onsetDatet">The date of disability onset.</param>
/// <param name="deathDatet">The date of death.</param>
/// <param name="entDatet">The date of entitlement.</param>
/// <param name="ioasdi">Type of benefit.</param>
void WorkerDataGeneral::cessationCheck2( const DateMoyr& cessationDatet,
const boost::gregorian::date& onsetDatet,
const boost::gregorian::date& deathDatet,
const DateMoyr& entDatet, ben_type ioasdi )
{
  // check for disability cessation before onset
  if (cessationDatet < onsetDatet)
    throw PiaException(PIA_IDS_DICESS4);
  if (ioasdi == SURVIVOR) {
    // check for disability cessation after death
    if (DateMoyr(deathDatet) < cessationDatet) {
      throw PiaException(PIA_IDS_DICESS7);
    }
  }
  else {
    // check for disability cessation after current entitlement
    if (entDatet < cessationDatet) {
      throw PiaException(PIA_IDS_DICESS6);
    }
  }
}

/// <summary>Checks date of prior disability cessation against date of prior
/// onset, using stored values.</summary>
///
/// <remarks>This version passes stored values to the static version with 3
/// arguments.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DICESS4"/> if disability cessation
/// before onset. <see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DICESS5"/> if prior disability
/// cessation after second entitlement.</exception>
void WorkerDataGeneral::cessation1Check() const
{
  if (joasdi == DISABILITY) {
    cessation1Check(disabPeriod[1].getCessationDate(),
      disabPeriod[1].getOnsetDate(), entDate);
  }
  else {
    cessation1Check(disabPeriod[1].getCessationDate(),
      disabPeriod[1].getOnsetDate(), disabPeriod[0].getEntDate());
  }
}

/// <summary>Checks date of prior disability cessation against date of prior
/// onset, using passed values.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DICESS4"/> if disability cessation
/// before onset. <see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DICESS5"/> if prior disability
/// cessation after second entitlement.</exception>
///
/// <param name="cessation1Datet">The date of prior disability cessation.
/// </param>
/// <param name="onset1Datet">The date of prior disability onset.</param>
/// <param name="entDatet">The date of current entitlement.</param>
void WorkerDataGeneral::cessation1Check( const DateMoyr& cessation1Datet,
const boost::gregorian::date& onset1Datet, const DateMoyr& entDatet )
{
  if (cessation1Datet < onset1Datet)
    // disability cessation before onset
    throw PiaException(PIA_IDS_DICESS4);
  if (entDatet < cessation1Datet)
    // prior disability cessation after second entitlement
    throw PiaException(PIA_IDS_DICESS5);
}

/// <summary>Sets date of entitlement to disability.</summary>
///
/// <param name="index">Which disability period to use (0 for most recent,
///     1 for next most recent).</param>
/// <param name="dateMoyr">The date to use.</param>
void WorkerDataGeneral::setPriorentDate( unsigned index,
 const DateMoyr& dateMoyr )
{
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm << getIdString() << ", index " << index << ", dateMoyr "
      << DateFormatter::toString(dateMoyr) << ": Setting priorentDate";
    Trace::writeLine(strm.str());
  }
#endif
  disabPeriod[index].setEntDate(dateMoyr);
}

/// <summary>Sets date of prior entitlement to disability.</summary>
///
/// <param name="index">Which disability period to use (0 for most recent,
/// 1 for next most recent).</param>
/// <param name="dateMoyr">The date to check and use.</param>
void WorkerDataGeneral::setPriorentDateCheck( unsigned index,
 const DateMoyr& dateMoyr )
{
#ifndef NDEBUG
  DisabPeriod::entDateCheck(dateMoyr);
#endif
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm << getIdString() << ", index " << index << ", dateMoyr "
      << DateFormatter::toString(dateMoyr) << ": Setting priorentDate";
    Trace::writeLine(strm.str());
  }
#endif
  disabPeriod[index].setEntDate(dateMoyr);
}

/// <summary>Checks date of prior entitlement to disability, using passed
/// values.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PRRENT1"/> if month of prior entitlement is out of
/// range; of type <see cref="PIA_IDS_PRRENT2"/> if year of prior
/// entitlement is before 1940; of type <see cref="PIA_IDS_PRRENT3"/>
/// if year of prior entitlement is after maximum allowed; of type
/// <see cref="PIA_IDS_PRRENT4"/> if prior entitlement is not before current
/// entitlement in an old-age case (prior entitlement should equal current
/// entitlement in a disability case, and there is no date of entitlement
/// for the worker in a survivor case); of type
/// <see cref="PIA_IDS_PRRENT5"/> if prior entitlement to disability is
/// before disability onset.</exception>
///
/// <param name="priorentDatet">The date of prior entitlement.</param>
/// <param name="onsetDatet">The date of current disability onset.</param>
/// <param name="entDatet">The date of current entitlement.</param>
/// <param name="ioasdit">The type of benefit.</param>
void WorkerDataGeneral::priorentCheck( const DateMoyr& priorentDatet,
const boost::gregorian::date& onsetDatet, const DateMoyr& entDatet,
ben_type ioasdit )
{
  if (ioasdit == OLD_AGE && !(priorentDatet < entDatet)) {
    throw PiaException(PIA_IDS_PRRENT4);
  }
  if (priorentDatet < onsetDatet)
    // prior entitlement to disability before disability onset
    throw PiaException(PIA_IDS_PRRENT5);
}

/// <summary>Checks date of second prior entitlement to disability, using
/// passed values.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PRRENT1"/> if month of prior entitlement is out of
/// range; of type <see cref="PIA_IDS_PRRENT2"/> if year of prior
/// entitlement is before 1940; of type <see cref="PIA_IDS_PRRENT3"/> if
/// year of prior entitlement is after maximum allowed; of type
/// <see cref="PIA_IDS_PRRENT4"/> if prior entitlement is not before current
/// entitlement; of type <see cref="PIA_IDS_PRRENT5"/> if prior entitlement
/// to disability is before disability onset.</exception>
///
/// <param name="priorent1t">The date of second prior entitlement.</param>
/// <param name="ionsett">The date of current disability onset.</param>
/// <param name="priorentt">The date of prior entitlement.</param>
void WorkerDataGeneral::priorent1Check( const DateMoyr& priorent1t,
const boost::gregorian::date& ionsett, const DateMoyr& priorentt )
{
  if (!(priorent1t < priorentt))
    throw PiaException(PIA_IDS_PRRENT4);
  if (priorent1t < ionsett)
    // prior entitlement to disability before disability onset
    throw PiaException(PIA_IDS_PRRENT5);
}

/// <summary>Sets date of benefit to the specified value.</summary>
///
/// <remarks>The specified date is used to set the date of benefit, if the
/// recalculation indicator has been set or this is a survivor benefit.
/// Otherwise, the benefit date is set to the date of initial entitlement, by
/// calling the version of this function with no arguments.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BENDATE2"/> if month is out of range.
/// <see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BENDATE3"/> if year is before 1940.
/// <see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BENDATE4"/> if year is after maximum allowed.
/// All are in debug mode only.</exception>
///
/// <param name="newBenefitDate">The date to use.</param>
void WorkerDataGeneral::setBenefitDate( const DateMoyr& newBenefitDate )
{
  if (getRecalc() || joasdi == SURVIVOR) {
#ifndef NDEBUG
    benefitDate1Check(newBenefitDate);
#endif
#if defined(DEBUGCASE)
    if (isDebugPid(getIdNumber())) {
      ostringstream strm;
      strm << getIdString() << ", newBenefitDate "
        << DateFormatter::toString(newBenefitDate)
        << ": Setting benefitDate";
      Trace::writeLine(strm.str());
    }
#endif
    benefitDate = newBenefitDate;
  }
  else {
    setBenefitDate();
  }
}

/// <summary>Sets date of benefit to the date of initial entitlement.
/// </summary>
///
/// <remarks>This version should be used if the recalculation indicator has
/// not been set and this is not a survivor benefit.</remarks>
void WorkerDataGeneral::setBenefitDate()
{
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm << getIdString() << ", entDate " << DateFormatter::toString(entDate)
      << ": Setting benefitDate to entDate";
    Trace::writeLine(strm.str());
  }
#endif
  benefitDate = entDate;
}

/// <summary>Zeroes out earnings and quarters of coverage before first year
/// and after last year of specified earnings.</summary>
///
/// <remarks>This version zeroes out quarters of coverage, but not earnings.
/// A child class should implement this function, calling this version and
/// then zeroing out earnings in the child class.</remarks>
///
/// <param name="ibegint">First year of specified earnings.</param>
/// <param name="iendt">Last year of specified earnings.</param>
void WorkerDataGeneral::zeroEarn( int ibegint, int iendt )
{
  qc.deleteContents(YEAR37, ibegint - 1);
  qc.deleteContents(iendt + 1, maxyear);
}

/// <summary>Sets date of entitlement.</summary>
///
/// <param name="newEntDate">The date to use.</param>
void WorkerDataGeneral::setEntDate( const DateMoyr& newEntDate )
{
#ifndef NDEBUG
  entCheck(newEntDate);
#endif
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm << getIdString() << ", newEntDate "
      << DateFormatter::toString(newEntDate) << ": Setting entDate";
    Trace::writeLine(strm.str());
  }
#endif
  entDate = newEntDate;
}

/// <summary>Initializes some Statement data (no disability and no date of
/// death).</summary>
void WorkerDataGeneral::setPebsData()
{
  valdi = 0;
  deathDate = boost::gregorian::date(boost::date_time::not_a_date_time);
  disabPeriod[0].deleteContents();
}

/// <summary>Checks quarters of coverage to 1977 for consistency, assuming
/// both qcs for 1937-1977 and qcs for 1951-1977 have been set to reasonable
/// values.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QC51TD"/> if number of quarters of
/// coverage, 1951-77, is out of range.</exception>
void WorkerDataGeneral::qctdCheck2()
{
  // limit number of qcs 1937-1977 if first year of earnings is after 1950
  if (ibegin > 1950) {
    qc51tdCheck(qctottd);
    qctot51td = qctottd;
  }
  if (iend < 1951) {
    qctot51td = 0;
  }
  if (ibegin > 1977) {
    qctottd = qctot51td = 0;
  }
}

/// <summary>Sets summary quarters of coverage, 1937 to 1977.</summary>
///
/// <param name="qct">New summary quarters of coverage.</param>
void WorkerDataGeneral::setQctd( int qct )
{
#ifndef NDEBUG
  qctdCheck(qct);
#endif
  qctottd = (unsigned char)qct;
}

/// <summary>Sets summary quarters of coverage, 1951 to 1977.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QC51TD"/> if number of quarters of
/// coverage, 1951-77, is out of range (debug mode only).</exception>
///
/// <param name="qct">New summary quarters of coverage.</param>
void WorkerDataGeneral::setQc51td( int qct )
{
#ifndef NDEBUG
  qc51tdCheck(qct);
#endif
  qctot51td = (unsigned char)qct;
}

/// <summary>Sets type of benefit.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_JOASDI"/> if type of benefit
/// is out of range (debug mode only).</exception>
///
/// <param name="newJoasdi">New type of benefit.</param>
void WorkerDataGeneral::setJoasdi( int newJoasdi )
{
#ifndef NDEBUG
  joasdiCheck(newJoasdi);
#endif
  joasdi = static_cast< unsigned char >(newJoasdi);
}

/// <summary>Sets the sex of the worker.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_SEX"/> if sex is not MALE or FEMALE
/// (only in debug mode).</exception>
///
/// <param name="newSex">New sex indicator.</param>
void WorkerDataGeneral::setSex( Sex::sex_type newSex )
{
#ifndef NDEBUG
  Sex::check(newSex);
#endif
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm << getIdString() << ", newSex " << static_cast<int>(newSex)
      << ": Setting sex";
    Trace::writeLine(strm.str());
  }
#endif
  theBits[SEX_BIT] = (newSex > 0);
}

/// <summary>Sets date of birth.</summary>
///
/// <param name="newBirthDate">The date to use.</param>
void WorkerDataGeneral::setBirthDate(
const boost::gregorian::date& newBirthDate )
{
#ifndef NDEBUG
  birth2Check(newBirthDate);
#endif
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm << getIdString() << ", newBirthDate "
      << DateFormatter::toString(newBirthDate) << ": Setting birthDate";
    Trace::writeLine(strm.str());
  }
#endif
  birthDate = newBirthDate;
}

/// <summary>Sets first year of earnings, including any projected earnings.
///   Also sets first year of HI earnings (ignored if indicator for
///   HI earnings is not set before attempting to use HI earnings).</summary>
///
/// <param name="newIbegin">New first year of earnings.</param>
void WorkerDataGeneral::setIbegin( int newIbegin )
{
#ifndef NDEBUG
  ibegin2Check(newIbegin);
#endif
  ibegin = (unsigned short)newIbegin;
  //if (projback == 0)
  //   earnpebs.setFirstYear(newIbegin);
  // HI earnings start in same year, but at least 1983
  //earnhi.setFirstYear(max(1983, ibegin));
}

/// <summary>Checks first year of earnings in a Statement case.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BEGIN3"/> if first year of earnings is
/// before 1937. <see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BEGIN4"/> if first year of earnings
/// is in starting year of projections or later.</exception>
///
/// <param name="ibegint">Year to check.</param>
/// <param name="istartt">Starting year of projections.</param>
void WorkerDataGeneral::ibegin1Check( int ibegint, int istartt )
{
  if (ibegint < YEAR37)
    throw PiaException(PIA_IDS_BEGIN3);
  if (ibegint > istartt - 1)
    throw PiaException(PIA_IDS_BEGIN4);
}

/// <summary>Checks first year of earnings in a non-Statement case.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BEGIN3"/> if first year of earnings
/// is before 1937. <see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BEGIN5"/> if first year of
/// earnings is after maximum year allowed.</exception>
///
/// <param name="ibegint">Year to check.</param>
void WorkerDataGeneral::ibegin2Check( int ibegint )
{
  if (ibegint < YEAR37)
    throw PiaException(PIA_IDS_BEGIN3);
  if (ibegint > maxyear)
    throw PiaException(PIA_IDS_BEGIN5);
}

/// <summary>Checks last year of earnings.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_END1"/> if last year of earnings is
/// before 1937. <see cref="PiaException"/> of type
/// <see cref="PIA_IDS_END2"/> if last year of earnings
/// is after maximum year allowed.</exception>
///
/// <param name="iendt">Year to check.</param>
void WorkerDataGeneral::iendCheck( int iendt )
{
  if (iendt < YEAR37)
    throw PiaException(PIA_IDS_END1);
  if (iendt > maxyear)
    throw PiaException(PIA_IDS_END2);
}

/// <summary>Sets last year of earnings, including any projected earnings.
/// </summary>
///
/// <param name="newIend">New last year of earnings.</param>
void WorkerDataGeneral::setIend( int newIend )
{
#ifndef NDEBUG
  iendCheck(newIend);
#endif
  iend = (unsigned short)newIend;
}

/// <summary>Sets number of periods of disability.</summary>
///
/// <param name="newValdi">New number of periods of disability.</param>
void WorkerDataGeneral::setValdi( int newValdi )
{
#ifndef NDEBUG
  if (joasdi == DISABILITY) {
    valdi2Check(newValdi);
  }
  else {
    valdi1Check(newValdi);
  }
#endif
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm << getIdString() << ", newValdi " <<newValdi << ": Setting valdi";
    Trace::writeLine(strm.str());
  }
#endif
  valdi = static_cast< unsigned char >(newValdi);
}

/// <summary>Sets date of death.</summary>
///
/// <param name="newDeathDate">The date to use.</param>
void WorkerDataGeneral::setDeathDate(
const boost::gregorian::date& newDeathDate )
{
#ifndef NDEBUG
  deathCheck2(newDeathDate);
#endif
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm << getIdString() << ", newDeathDate "
      << DateFormatter::toString(newDeathDate) << ": Setting deathDate";
    Trace::writeLine(strm.str());
  }
#endif
  deathDate = newDeathDate;
}

/// <summary>Returns true if benefit increase assumptions are needed.
/// </summary>
///
/// <returns>True if benefit increase assumptions are needed.</returns>
///
/// <param name="year">First year of projection period.</param>
bool WorkerDataGeneral::need_bi( int year ) const
{
  // no need for assumptions if no benefit calculated
  if (getJoasdi() == NO_BEN)
    return(false);
  // check for faulty starting date
  if (year < YEAR79)
    return(false);
  return(!(benefitDate < DateMoyr(boost::date_time::Dec, year)));
}

/// <summary>Returns true if average wage increase assumptions are needed.
/// </summary>
///
/// <returns>True if average wage increase assumptions are needed.</returns>
///
/// <param name="year">First year of projection period.</param>
/// <param name="lastTwoYears">True if need to look at last 2 years of
///  historical period.</param>
bool WorkerDataGeneral::need_aw( int year, bool lastTwoYears ) const
{
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm << getIdString() << ", year " << year << ", lastTwoYears "
      << (lastTwoYears ? "true" : "false")
      << ": At top of WorkerDataGeneral::need_aw";
    Trace::writeLine(strm.str());
  }
#endif
  // no need for assumptions if no benefit calculated
  if (getJoasdi() == NO_BEN)
    return(false);
  // check for faulty starting date
  if (year < YEAR79)
    return(false);
  if (lastTwoYears) {
    year -= 2;
  }
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm << getIdString() << ", benefitDate.getYear() "
      << (int)benefitDate.getYear() << ", year " << year
      << ": At bottom of WorkerDataGeneral::need_aw";
    Trace::writeLine(strm.str());
  }
#endif
  return(static_cast<int>(benefitDate.getYear()) > year);
}

/// <summary>Returns true if wage base assumptions are needed.</summary>
///
/// <returns>True if wage base assumptions are needed.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_NEEDBASES"/> if year is out of range
/// (only in debug mode).</exception>
///
/// <param name="year">First year of projection period.</param>
bool WorkerDataGeneral::need_bases( int year ) const
{
#ifndef NDEBUG
  if (year < YEAR37)
    throw PiaException(PIA_IDS_NEEDBASES);
#endif
  return(static_cast<int>(benefitDate.getYear()) > year);
}

/// <summary>Checks that date of benefit is in range, using stored value.
/// </summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BENDATE2"/> if month is out of range.
/// <see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BENDATE3"/> if year is before 1940.
/// <see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BENDATE4"/> if year is after maximum allowed.
/// </exception>
///
/// <remarks>This version passes the stored benefit date to the static version
/// with one argument.</remarks>
void WorkerDataGeneral::benefitDate1Check() const
{
  benefitDate1Check(benefitDate);
}

/// <summary>Checks that date of benefit is in range, using passed value.
/// </summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BENDATE2"/> if month is out of range.
/// <see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BENDATE3"/> if year is before 1940.
/// <see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BENDATE4"/> if year is after maximum allowed.
/// </exception>
///
/// <param name="datetemp">The date to check.</param>
void WorkerDataGeneral::benefitDate1Check( const DateMoyr& datetemp )
{
  try {
    datetemp.monthCheck();
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_BENDATE2);
  }
  if (datetemp.getYear() < 1940)
    throw PiaException(PIA_IDS_BENDATE3);
  if (static_cast<int>(datetemp.getYear()) > maxyear)
    throw PiaException(PIA_IDS_BENDATE4);
}

/// <summary>Checks date of birth in a Statement case, using passed values.
/// </summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BIRTH3"/> if month of birth is out of range; of type
/// <see cref="PIA_IDS_BIRTH4"/> if day of birth is out of range; of type
/// <see cref="PIA_IDS_BIRTH2"/> if year of birth is before 1800; of type
/// <see cref="PIA_IDS_BIRTH1"/> if year of birth is in current year or
/// later.</exception>
///
/// <param name="istartt">Starting year of projections.</param>
/// <param name="datetemp">The date to check.</param>
void WorkerDataGeneral::birth1Check( const boost::gregorian::date& datetemp,
 int istartt )
{
  if (datetemp.is_not_a_date()) {
    throw PiaException(PIA_IDS_BIRTH3);
  }
  if (static_cast<unsigned int>(datetemp.year()) < Date::lowyear) {
    throw PiaException(PIA_IDS_BIRTH2);
  }
  if (static_cast<int>(datetemp.year()) > istartt - 1) {
    throw PiaException(PIA_IDS_BIRTH1);
  }
}

/// <summary>Checks date of birth in a non-Statement case, using passed
/// values.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BIRTH3"/> if month of birth is out of range; of type
/// <see cref="PIA_IDS_BIRTH4"/> if day of birth is out of range; of type
/// <see cref="PIA_IDS_BIRTH2"/> if year of birth is before 1800; of type
/// <see cref="PIA_IDS_BIRTH5"/> if year of birth is after maximum allowed.
/// </exception>
///
/// <param name="datetemp">The date to check.</param>
void WorkerDataGeneral::birth2Check( const boost::gregorian::date& datetemp )
{
  if (datetemp.is_not_a_date()) {
    throw PiaException(PIA_IDS_BIRTH3);
  }
  if (static_cast<unsigned int>(datetemp.year()) < Date::lowyear) {
    throw PiaException(PIA_IDS_BIRTH2);
  }
  if (static_cast<int>(datetemp.year()) > maxyear) {
    throw PiaException(PIA_IDS_BIRTH5);
  }
}

/// <summary>Checks date of death (checks in survivor case for death in 1937
/// or later).</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DEATH3"/> if death before 1937 in a survivor case.
/// </exception>
///
/// <param name="dateTemp">The date to check.</param>
/// <param name="ioasdi">Type of benefit.</param>
void WorkerDataGeneral::deathCheck1( const boost::gregorian::date& dateTemp,
ben_type ioasdi )
{
  if (ioasdi == SURVIVOR) {
    if (static_cast<int>(dateTemp.year()) < YEAR37) {
      // death before 1937
      throw PiaException(PIA_IDS_DEATH3);
    }
  }
  deathCheck2(dateTemp);
}

/// <summary>Checks date of death for basic correctness.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DEATH1"/> if month of death is out
/// of range. <see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DEATH2"/> if year of death is after
/// maximum allowed.</exception>
///
/// <param name="datetemp">The date to check.</param>
void WorkerDataGeneral::deathCheck2( const boost::gregorian::date& datetemp )
{
  if (datetemp.is_not_a_date()) {
    throw PiaException(PIA_IDS_DEATH1);
  }
  if (static_cast<int>(datetemp.year()) > maxyear) {
    throw PiaException(PIA_IDS_DEATH2);
  }
}

/// <summary>Checks that current date of entitlement is within bounds, using
/// passed values.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_ENT1"/> if month of entitlement is out of range; of
/// type <see cref="PIA_IDS_ENT2"/> if year of entitlement is before 1940;
/// of type <see cref="PIA_IDS_ENT3"/> if year of entitlement is after
/// maximum allowed.</exception>
///
/// <param name="datetemp">The date to check.</param>
void WorkerDataGeneral::entCheck( const DateMoyr& datetemp )
{
  try {
    datetemp.monthCheck();
  } catch (PiaException e) {
    throw PiaException(PIA_IDS_ENT1);
  }
  if (datetemp.getYear() < 1940)
    throw PiaException(PIA_IDS_ENT2);
  if (static_cast<int>(datetemp.getYear()) > maxyear)
    throw PiaException(PIA_IDS_ENT3);
}

/// <summary>Checks public pension amount.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PUBPEN"/> if public pension amount
/// is out of range.</exception>
///
/// <param name="pubpent">Pension to check.</param>
void WorkerDataGeneral::pubpenCheck( float pubpent )
{
  try {
    WageBaseGeneral::check(static_cast<double>(pubpent));
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_PUBPEN);
  }
}

/// <summary>Checks public pension entitlement date.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_NONPENDATE1"/> or <see cref="PIA_IDS_NONPENDATE2"/>
/// if public pension entitlement date is out of range (before date of
/// birth or after date of death).</exception>
///
/// <param name="pubpenDatet">Pension to check.</param>
/// <param name="birthDatet">Birth date.</param>
/// <param name="deathDatet">Death date (could be not_a_date).</param>
void WorkerDataGeneral::pubpenDateCheck( const DateMoyr& pubpenDatet,
   const boost::gregorian::date& birthDatet,
   const boost::gregorian::date& deathDatet )
{
  if (!birthDatet.is_not_a_date()) {
    DateMoyr tempDate(birthDatet);
    if (pubpenDatet < birthDatet) {
      throw PiaException(PIA_IDS_NONPENDATE1);
    }
  }
  if (!deathDatet.is_not_a_date()) {
    DateMoyr tempDate(deathDatet);
    if (pubpenDatet > tempDate) {
      throw PiaException(PIA_IDS_NONPENDATE2);
    }
  }
}

/// <summary>Returns primary beneficiary code.</summary>
///
/// <returns>Primary beneficiary code.
/// <table>
///   0   was never a primary beneficiary.
///   1   is a primary beneficiary.
///   2   is a deceased primary beneficiary.
/// </table></returns>
///
/// <param name="ioasdi">Type of benefit.</param>
int WorkerDataGeneral::isPrimary( ben_type ioasdi ) const
{
  if (ioasdi == SURVIVOR) {
    // assume was old-age if over age 62 or disabled
    return ((deathDate.year() > 62 + birthDate.year()) || valdi > 0) ? 2 : 0;
  }
  return (ioasdi == OLD_AGE || ioasdi == DISABILITY) ? 1 : 0;
}

/// <summary>Returns true if quarters of coverage by year from regular
/// earnings are needed, false otherwise.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCLUMPYEAR"/> if <see cref="qclumpyear"/>
/// is out of range (only in debug mode).</exception>
///
/// <returns>True if quarters of coverage by year from regular
///   earnings are needed, false otherwise.</returns>
bool WorkerDataGeneral::needQcsByYear() const
{
#ifndef NDEBUG
  if (qclumpyear < YEAR37 || qclumpyear > 1977)
    throw PiaException(PIA_IDS_QCLUMPYEAR);
#endif
  if (getTotalize() || joasdi == PEBS_CALC || qclumpyear < 1977 ||
    getIndms() || getIndrr() || ibegin < 1950)
    return true;
  // check for disability case with onset within 10 years of qclumpyear
  if (joasdi == DISABILITY)
    return (disabPeriod[0].getOnsetDate() < boost::gregorian::date(
    static_cast<unsigned short>(11 + qclumpyear), 1u, 1u));
  // check for survivor case with death within 13 quarters of qclumpyear
  if (joasdi == SURVIVOR)
    return (deathDate < boost::gregorian::date(
    static_cast<unsigned short>(4 + qclumpyear), 4u, 1u));
  // all other cases
  return false;
}

/// <summary>Returns true if ok to enable earnings entry dialog box.</summary>
///
/// <returns>True if ok to enable earnings entry dialog box.</returns>
bool WorkerDataGeneral::getEarnEnable() const
{
  // do not allow earnings entry until years are consistent
  if (getIndearn()) {
    if (birthDate.is_not_a_date()) {
      return false;
    }
    try {
      ibeginCheck();
    } catch (PiaException&) {
      return false;
    }
    return true;
  }
  // if no regular earnings, must have railroad earnings
  return getIndrr();
}

/// <summary>Sets earnings indicator.</summary>
///
/// <param name="newIndearn">New earnings indicator.</param>
void WorkerDataGeneral::setIndearn( bool newIndearn )
{
#if defined(DEBUGCASE)
  if (isDebugPid(getIdNumber())) {
    ostringstream strm;
    strm << getIdString() << ", indearn " << (theBits[INDEARN_BIT] ? 1 : 0)
      << ", newIndearn " << (newIndearn ? 1 : 0) << ": Setting indearn";
    Trace::writeLine(strm.str());
  }
#endif
  theBits[INDEARN_BIT] = newIndearn;
}

/// <summary>Returns earliest entitlement date on this record.</summary>
///
/// <returns>Earliest entitlement date on this record.</returns>
///
/// <remarks>If there are multiple periods of disability, take the first
/// disability entitlement date. Otherwise, for old-age or survivor with a
/// prior disability, use the prior disability entitlement. Finally, for
/// disability entitlement with only one period of disability, or old-age or
/// survivor with no disability, use current entitlement.
///
/// Note that for a survivor with no prior disability, this returns a date of
/// zero. This function should probably not be used in that case.</remarks>
const DateMoyr& WorkerDataGeneral::getEarliestEntDate() const
{
  // if there are multiple periods of disability, take the first disability
  // entitlement date
  if (valdi > 1) {
    return disabPeriod[valdi - 1].getEntDate();
  }
  // for old-age or survivor with a prior disability, use disability
  // entitlement
  else if ((joasdi == OLD_AGE || joasdi == SURVIVOR) && valdi == 1) {
    return disabPeriod[0].getEntDate();
  }
  // for disability entitlement with only one period of disability, or
  // old-age or survivor with no disability, use current entitlement
  else {
    return getEntDate();
  }
}
