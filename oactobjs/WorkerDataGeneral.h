// Declarations for the <see cref="WorkerDataGeneral"/> class - parent of
// classes to manage worker data.

// $Id: WorkerDataGeneral.h 1.57 2011/08/11 13:55:33EDT 044579 Development  $

#pragma once

#include <bitset>
#include "boost/serialization/access.hpp"
#include "boost/date_time/gregorian/greg_date.hpp"
#include "datemoyr.h"
#include "cachup.h"
#include "biproj.h"
#include "qc.h"
#include "BitAnnual.h"
#include "QcArray.h"
#include "Sex.h"
#include "DisabPeriod.h"

/// <summary>The abstract parent of all classes that manage the basic data
/// required to calculate a Social Security benefit.</summary>
///
/// <remarks>The child class in this library is <see cref="WorkerData"/>.
/// </remarks>
///
/// <seealso cref="WorkerData"/>
class WorkerDataGeneral
{
  /// <summary>Friend class to allow serialization.</summary>
  friend class boost::serialization::access;
  // <summary>Archives the data.</summary>
  //
  // <remarks>When the class Archive corresponds to an output archive, the
  // & operator is defined similar to <<. Likewise, when the class Archive is
  // a type of input archive, the & operator is defined similar to >>.
  // </remarks>
  //
  // <param name="ar">The archive to use.</param>
  // <param name="version">The version of the archive to use.</param>
  template< class Archive >
  void serialize( Archive& ar, const unsigned int version )
  { ar & childCareYears & qc & disabPeriod & qctot51td & qctottd & joasdi;
    ar & valdi & ibegin & iend & theBits & birthDate & benefitDate;
    ar & entDate & deathDate; }
public:
  /// <summary>Type of benefit calculation.</summary>
  enum ben_type {
    NO_BEN,  // No benefit calculation.
    OLD_AGE,  // Old-age benefit calculation.
    SURVIVOR,  // Survivor benefit calculation.
    DISABILITY,  // Disability benefit calculation.
    PEBS_CALC  // Statement benefit calculation.
  };
  /// <summary>Dimension limits.</summary>
  enum DimLimits {
    /// <summary>Number of periods of disability.</summary>
    NUM_DISAB_PERIODS = 2
  };
  /// <summary>Members of the bitset.</summary>
  enum BitsetMembers {
    SEX_BIT = 0,    /// The sex of worker (see <see cref="Sex::sex_type"/>).
    RECALC_BIT,     /// Recalculation code.
    TOTALIZE_BIT,   /// Totalization code.
    DEEMED_BIT,     /// Indicator for deemed insured status possible.
    BLIND_BIT,      /// Indicator for disability insured status allowed based
                    /// on fully insured status only because of blindness.
    MQGE_BIT,       /// Medicare earnings code.
    RESERVIST_BIT,  /// Indicator for military reservist pension.
    INDEARN_BIT,    /// Indicator for regular earnings available.
    INDRR_BIT,      /// Indicator for railroad qcs and earnings available.
    QCSBYYEAR_BIT,  /// Indicator for QCs by single year prior to 1978.
    DEADOAB_BIT     /// Indicator for deceased OAB in survivor case.
  };
  /// <summary>Child care years, 1937 to 2100.</summary>
  BitAnnual childCareYears;
  /// <summary>Quarters of coverage, 1937 to
  /// <see cref="WorkerDataGeneral::maxyear"/>.</summary>
  QcArray qc;
  /// <summary>Two periods of disability.</summary>
  DisabPeriod disabPeriod[NUM_DISAB_PERIODS];
private:
  /// <summary>Number of quarters of coverage earned from 1951 through 1977.
  /// </summary>
  unsigned char qctot51td;
  /// <summary>Number of quarters of coverage earned from 1937 through 1977.
  /// </summary>
  unsigned char qctottd;
  /// <summary>Type of benefit calculation.</summary>
  ///
  /// <remarks>It has the values listed in <see cref="ben_type"/>.</remarks>
  unsigned char joasdi;
  /// <summary>Number of periods of disability (0-2).</summary>
  unsigned char valdi;
  /// <summary>First year of earnings after backward projection.</summary>
  unsigned short ibegin;
  /// <summary>Last year of earnings after forward projection.</summary>
  unsigned short iend;
  /// <summary>A set of bools, used for various purposes.</summary>
  ///
  /// <remarks>The fields have the following meanings:
  /// <table>
  ///  0   The sex of worker (see <see cref="Sex::sex_type"/>).
  ///  1   Recalculation code (false for initial benefit, true for recalculation).
  ///  2   Totalization code (false for no totalization, true if totalization applies).
  ///  3   Indicator for deemed insured status possible (false for no deemed insured status possible, true if deemed insured status possible).
  ///  4   Indicator for disability insured status allowed based on fully insured status only because of blindness (false for no blindness, true if blindness).
  ///  5   Medicare earnings code (false for no Medicare earnings, true if Medicare earnings).
  ///  6   Indicator for military reservist pension (false if not a military reservist pension, true if a military reservist pension).
  ///  7   Indicator for regular earnings available.
  ///  8   Indicator for railroad qcs and earnings available.
  ///  9   Indicator for QCs by single year prior to 1978.
  /// 10   Indicator for deceased OAB in survivor case. (True if deceased worker was an OAB before death. This is used in application of the RIB-LIM.)
  /// </table></remarks>
  std::bitset<32> theBits;
  /// <summary>Month, day, and year of birth.</summary>
  boost::gregorian::date birthDate;
  /// <summary>Month and year of benefit date.</summary>
  DateMoyr benefitDate;
  /// <summary>Month and year of entitlement.</summary>
  DateMoyr entDate;
  /// <summary>Month, day, and year of death.</summary>
  boost::gregorian::date deathDate;
protected:
  /// <summary>Maximum year allowed.</summary>
  static int maxyear;
  /// <summary>Year through which quarters of coverage are specified as a
  /// lump sum.</summary>
  static int qclumpyear;
public:
  WorkerDataGeneral();
  WorkerDataGeneral( const WorkerDataGeneral& workerData );
  virtual ~WorkerDataGeneral();
  /// <summary>Returns true if any self-employed taxes.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>True if any self-employed taxes.</returns>
  virtual bool anySelfEmp() const = 0;
  void benefitDate1Check() const;
  /// <summary>Checks that date of benefit is at or after entitlement, using
  /// stored values.</summary>
  ///
  /// <remarks>This version passes the stored values to the static version
  /// with 2 arguments.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_BENDATE1"/> if benefit date is before entitlement
  /// date.</exception>
  void benefitDate2Check() const { benefitDate2Check(entDate, benefitDate); }
  /// <summary>Checks date of birth in a Statement case, using stored values.
  /// </summary>
  ///
  /// <remarks>This version passes stored values to the static version with 2
  /// arguments.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_BIRTH3"/> if month of birth is out of range; of type
  /// <see cref="PIA_IDS_BIRTH4"/> if day of birth is out of range; of type
  /// <see cref="PIA_IDS_BIRTH2"/> if year of birth is before 1800; of type
  /// <see cref="PIA_IDS_BIRTH1"/> if year of birth is in current year or
  /// later.</exception>
  ///
  /// <param name="istartt">Starting year of projections.</param>
  void birth1Check( int istartt ) const { birth1Check(birthDate, istartt); }
  /// <summary>Checks date of birth in a non-Statement case, using stored
  /// values.</summary>
  ///
  /// <remarks>This version passes stored values to the static version with 1
  /// argument.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_BIRTH3"/> if month of birth is out of range; of type
  /// <see cref="PIA_IDS_BIRTH4"/> if day of birth is out of range; of type
  /// <see cref="PIA_IDS_BIRTH2"/> if year of birth is before 1800; of type
  /// <see cref="PIA_IDS_BIRTH5"/> if year of birth is after maximum allowed.
  /// </exception>
  void birth2Check() const { birth2Check(birthDate); }
  /// <summary>Checks date of disability cessation, using stored values.
  /// </summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_DICESS4"/> if cesation is before onset; of type
  /// <see cref="PIA_IDS_DICESS6"/> if entitlement is before cessation; of
  /// type <see cref="PIA_IDS_DICESS7"/> if cessation is after death in a
  /// survivor case.</exception>
  void cessationCheck() const
  { cessationCheck2(disabPeriod[0].getCessationDate(),
    disabPeriod[0].getOnsetDate(), deathDate, entDate,
    static_cast<ben_type>(joasdi)); }
  void cessation1Check() const;
  /// <summary>Checks that date of death is after birth, using stored values.
  /// </summary>
  ///
  /// <remarks>This version passes stored values to the static version with 2
  /// arguments.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_DEATH5"/> if death of worker is before birth.
  /// </exception>
  void deathCheck() const { deathCheck(deathDate, birthDate); }
  virtual void deleteContents();
  /// <summary>Deletes date of death.</summary>
  void deleteDeathDate()
  { deathDate = boost::gregorian::date(boost::date_time::not_a_date_time); }
  /// <summary>Deletes date of entitlement.</summary>
  void deleteEntDate() { entDate = DateMoyr(0,0); }
  /// <summary>Checks date of disability onset, using stored values.</summary>
  ///
  /// <remarks>This version passes stored values to the static version with 7
  /// arguments.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_ONSET5"/> if disability is before birth; of type
  /// <see cref="PIA_IDS_ONSET6"/> if disability is after entitlement in a
  /// life case or after benefit date in a survivor case; of type
  /// <see cref="PIA_IDS_ONSET7"/> if death of worker is before onset in a
  /// survivor case.</exception>
  void disCheck() const
  { disCheck(disabPeriod[0].getOnsetDate(), deathDate, valdi, birthDate,
    static_cast<ben_type>(joasdi), entDate, benefitDate); }
  /// <summary>Checks date of prior disability onset, using stored values.
  /// </summary>
  ///
  /// <remarks>This version passes stored values to the static version with 4
  /// arguments.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_ONSET5"/> if disability is before birth; of type
  /// <see cref="PIA_IDS_ONSET8"/> if prior disability is after current
  /// disability.</exception>
  void dis1Check() const
  { dis1Check(disabPeriod[1].getOnsetDate(), disabPeriod[0].getOnsetDate(),
    valdi, birthDate); }
  /// <summary>Checks for no earnings of any type, using stored values.
  /// </summary>
  ///
  /// <remarks> This version passes stored valuesto the static version with 3
  /// arguments.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_BEGIN6"/> if there are no regular, railroad, or
  /// military service earnings.</exception>
  void earningsCheck() const
  { earningsCheck(getIndearn(), getIndrr(), getIndms()); }
  /// <summary>Checks that current date of entitlement is within bounds, using
  /// stored values.</summary>
  ///
  /// <remarks> This version passes stored values to the static version with 1
  /// argument.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_ENT1"/> if month of entitlement is out of range; of
  /// type <see cref="PIA_IDS_ENT2"/> if year of entitlement is before 1940;
  /// of type <see cref="PIA_IDS_ENT3"/> if year of entitlement is after
  /// maximum allowed.</exception>
  void entCheck() const { entCheck(entDate); }
  /// <summary>Returns first year of Medicare earnings, if any.</summary>
  ///
  /// <returns>First year of Medicare earnings, if any.</returns>
  int firstEarnHiYear() const { return (1983 < ibegin) ? ibegin : 1983; }
  /// <summary>Returns date of benefit.</summary>
  ///
  /// <returns>The date of benefit.</returns>
  const DateMoyr& getBenefitDate() const { return(benefitDate); }
  /// <summary>Returns date of birth of worker.</summary>
  ///
  /// <returns>The date of birth of worker.</returns>
  const boost::gregorian::date& getBirthDate() const { return(birthDate); }
  /// <summary>Returns the blind indicator.</summary>
  ///
  /// <returns>True if blind, false otherwise.</returns>
  bool getBlindind() const { return(theBits[BLIND_BIT]); }
  /// <summary>Returns date of death.</summary>
  ///
  /// <returns>The date of death.</returns>
  const boost::gregorian::date& getDeathDate() const { return(deathDate); }
  /// <summary>Returns the deceased OAB indicator.</summary>
  ///
  /// <returns>True if the deceased worker was an OAB before death.</returns>
  bool getDeceasedOAB() const { return(theBits[DEADOAB_BIT]); }
  /// <summary>Returns the deemed insured indicator.</summary>
  ///
  /// <returns>True if the deemed insured status provision applies, false
  ///  otherwise.</returns>
  bool getDeemedind() const { return(theBits[DEEMED_BIT]); }
  const DateMoyr& getEarliestEntDate() const;
  /// <summary>Returns Medicare earnings for specified year.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Medicare earnings for specified year.</returns>
  virtual double getEarnHi( int ) const = 0;
  /// <summary>Returns Medicare earnings for specified year if there are no
  /// OASDI earnings; otherwise return zero.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Medicare earnings for specified year if there are no OASDI
  /// earnings; otherwise return zero.</returns>
  virtual double getEarnHiOnly( int ) const = 0;
  /// <summary>Returns military service earnings for specified year.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Military service earnings for specified year.</returns>
  virtual double getEarnMS( int ) const = 0;
  /// <summary>Returns total military service earnings, 1937-50.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Total military service earnings, 1937-50.</returns>
  virtual double getEarnMS3750() const = 0;
  /// <summary>Returns OASDI plus Medicare earnings for specified year.
  /// </summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>OASDI plus Medicare earnings for specified year.</returns>
  virtual double getEarnOasdhi( int ) const = 0;
  /// <summary>Returns OASDI-covered earnings for the specified year.
  /// </summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>OASDI-covered earnings for the specified year.</returns>
  virtual double getEarnOasdi( int ) const = 0;
  /// <summary>Returns railroad earnings for specified year.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Railroad earnings for specified year.</returns>
  virtual double getEarnRR( int ) const = 0;
  /// <summary>Returns total railroad earnings, 1937-50.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Total railroad earnings, 1937-50.</returns>
  virtual double getEarnRR3750() const = 0;
  bool getEarnEnable() const;
  /// <summary>Returns date of entitlement.</summary>
  ///
  /// <returns>The date of entitlement.</returns>
  const DateMoyr& getEntDate() const { return(entDate); }
  /// <summary>Returns first year of military service (0 if none).</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>First year of military service (0 if none).</returns>
  virtual int getFirstYearMS() const = 0;
  /// <summary>Returns first year of railroad (0 if none).</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>First year of railroad (0 if none).</returns>
  virtual int getFirstYearRR() const = 0;
  /// <summary>Returns first year of earnings after any projection.</summary>
  ///
  /// <returns>First year of earnings after any projection.</returns>
  int getIbegin() const { return(ibegin); }
  /// <summary>Returns Social Security number as a number.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Social Security number as a number.</returns>
  virtual unsigned long getIdNumber() const = 0;
  /// <summary>Returns Social Security number as a string.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Social Security number as a string.</returns>
  virtual std::string getIdString() const = 0;
  /// <summary>Returns last year of earnings after any projection.</summary>
  ///
  /// <returns>Last year of earnings after any projection.</returns>
  int getIend() const { return(iend); }
  /// <summary>Returns earnings indicator.</summary>
  ///
  /// <returns>False if no regular earnings, true if some regular earnings.
  /// </returns>
  bool getIndearn() const { return(theBits[INDEARN_BIT]); }
  /// <summary>Returns military service indicator.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>True if any military service, false otherwise.</returns>
  virtual bool getIndms() const = 0;
  /// <summary>Returns railroad indicator.</summary>
  ///
  /// <returns>False if no railroad earnings, true if some railroad earnings.
  /// </returns>
  bool getIndrr() const { return(theBits[INDRR_BIT]); }
  /// <summary>Returns type of benefit calculation, including possibly
  /// Statement.</summary>
  ///
  /// <returns>Type of benefit calculation.
  /// <table>
  ///  NO_BEN       No benefit calculation.
  ///  OLD_AGE      Old-age benefit calculation.
  ///  SURVIVOR     Survivor benefit calculation.
  ///  DISABILITY   Disability benefit calculation.
  ///  PEBS_CALC    Statement benefit calculation.
  /// </table></returns>
  WorkerDataGeneral::ben_type getJoasdi() const
  { return(static_cast<ben_type>(joasdi)); }
  /// <summary>Returns last year of military service (0 if none).</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Last year of military service (0 if none).</returns>
  virtual int getLastYearMS() const = 0;
  /// <summary>Returns last year of military service (0 if none).</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Last year of military service (0 if none).</returns>
  virtual int getLastYearRR() const = 0;
  /// <summary>Returns Medicare-qualified government employment indicator.
  /// </summary>
  ///
  /// <returns>True if any Medicare-qualified government employment.</returns>
  bool getMqge() const { return(theBits[MQGE_BIT]); }
  /// <summary>Returns one line of number holder's address.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>One line of number holder's address.</returns>
  virtual std::string getNhaddr( int ) const = 0;
  /// <summary>Returns number holder's name.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Number holder's name.</returns>
  virtual std::string getNhname() const = 0;
  /// <summary>Returns date of cessation of oab entitlement prior to most
  /// recent DIB entitlement.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>The date of cessation of oab entitlement prior to most recent
  /// DIB entitlement.</returns>
  virtual DateMoyr getOabCessDate() const = 0;
  /// <summary>Returns date of entitlement to oab prior to most recent DIB
  /// entitlement.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>The date of entitlement to oab prior to most recent DIB
  /// entitlement.</returns>
  virtual DateMoyr getOabEntDate() const = 0;
  /// <summary>Returns monthly noncovered pension.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Monthly noncovered pension.</returns>
  virtual float getPubpen() const = 0;
  /// <summary>Returns date of entitlement for noncovered pension.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>The date of noncovered pension entitlement.</returns>
  virtual DateMoyr getPubpenDate() const = 0;
  /// <summary>Returns monthly noncovered pension after December 1994,
  /// after removing reservist pension.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Monthly noncovered pension after December 1994,
  /// after removing reservist pension.</returns>
  virtual float getPubpenReservist() const = 0;
  /// <summary>Returns military service quarters of coverage for specified
  /// year.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Military service quarters of coverage for specified year.
  /// </returns>
  virtual unsigned getQcovMS( int ) const = 0;
  /// <summary>Returns total military service quarters of coverage, 1937-50.
  /// </summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Total military service quarters of coverage, 1937-50.</returns>
  virtual unsigned getQcovMS3750() const = 0;
  /// <summary>Returns railroad quarters of coverage for specified year.
  /// </summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Railroad quarters of coverage for specified year.</returns>
  virtual unsigned getQcovRR( int ) const = 0;
  /// <summary>Returns total railroad quarters of coverage, 1937-50.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Total railroad quarters of coverage, 1937-50.</returns>
  virtual unsigned getQcovRR3750() const = 0;
  /// <summary>Returns total railroad quarters of coverage, 1951-77.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Total railroad quarters of coverage, 1951-77.</returns>
  virtual unsigned getQcovRR5177() const = 0;
  /// <summary>Returns quarters of coverage from 1951 to 1977.</summary>
  ///
  /// <returns>Quarters of coverage from 1951 to 1977.</returns>
  int getQctot51td() const { return(qctot51td); }
  /// <summary>Returns quarters of coverage from 1937 to 1977.</summary>
  ///
  /// <returns>Quarters of coverage from 1937 to 1977.</returns>
  int getQctottd() const { return(qctottd); }
  /// <summary>Returns recalculation code.</summary>
  ///
  /// <returns>True if benefit is being recalculated after initial
  /// entitlement, false otherwise.</returns>
  bool getRecalc() const { return(theBits[RECALC_BIT]); }
  /// <summary>Returns reservist indicator.</summary>
  ///
  /// <returns>False if no reservist, true if pubpen is based at least
  /// partially on reservist pension.</returns>
  bool getReservist() const { return(theBits[RESERVIST_BIT]); }
  /// <summary>Returns the sex of the worker.</summary>
  ///
  /// <returns>MALE (0) if male, FEMALE (1) if female.</returns>
  Sex::sex_type getSex() const
  { return static_cast<Sex::sex_type>(theBits[SEX_BIT]); }
  /// <summary>Returns type of taxes for specified year.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  ///
  /// <returns>Type of taxes for specified year.</returns>
  virtual bool getTaxType( int ) const = 0;
  /// <summary>Returns totalization indicator.</summary>
  ///
  /// <returns>False if no totalization, true if totalization applies.
  /// </returns>
  bool getTotalize() const { return(theBits[TOTALIZE_BIT]); }
  /// <summary>Returns number of periods of disability.</summary>
  ///
  /// <returns>Could have 0, 1, or 2 periods of disability.</returns>
  int getValdi() const { return(valdi); }
  /// <summary>Returns indicator for QCs prior to 1978 by single year.
  /// </summary>
  ///
  /// <returns>True if has QCs prior to 1978 by single year, false otherwise.
  /// </returns>
  bool hasQcsByYear() const { return theBits[QCSBYYEAR_BIT]; }
  /// <summary>Checks for first year of earnings within bounds, using stored
  /// values.</summary>
  ///
  /// <remarks>This version passes stored values to the static version with 2
  /// arguments.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_BEGIN1"/> if first year of earnings is out of range.
  /// </exception>
  void ibeginCheck() const
  { ibeginCheck(ibegin, static_cast<int>(birthDate.year())); }
  /// <summary>Returns true if alive at end of year, false otherwise.
  /// </summary>
  ///
  /// <returns>True if alive at end of year, false otherwise.</returns>
  ///
  /// <param name="year"> Year being checked.</param>
  bool isalive( int year ) const
  { if (deathDate.is_not_a_date())
      return (static_cast<int>(birthDate.year()) <= year);
    else return((static_cast<int>(birthDate.year()) <= year) &&
      (year < deathDate.year())); }
  /// <summary>Returns true if dead at end of year, false otherwise.</summary>
  ///
  /// <returns>True if dead at end of year, false otherwise.</returns>
  ///
  /// <param name="year">Year being checked.</param>
  bool isdead( int year ) const
  { if (deathDate.is_not_a_date()) return false;
    else return(year >= deathDate.year()); }
  /// <summary>Returns true if dead at end of month, false otherwise.
  /// </summary>
  ///
  /// <returns>True if dead at end of month, false otherwise.</returns>
  ///
  /// <param name="date">Month and year being checked.</param>
  bool isdead( const DateMoyr& date ) const
  { return(!deathDate.is_not_a_date() && !(date < DateMoyr(deathDate))); }
  /// <summary>Return true if person is <see cref="Sex::sex_type::FEMALE"/>,
  /// false otherwise.</summary>
  ///
  /// <returns>True if person is <see cref="Sex::sex_type::FEMALE"/>, false
  /// otherwise.</returns>
  bool isFemale() const { return theBits[SEX_BIT] == Sex::FEMALE; }
  /// <summary>Return true if person is <see cref="Sex::sex_type::MALE"/>,
  /// false otherwise.</summary>
  ///
  /// <returns>True if person is <see cref="Sex::sex_type::MALE"/>, false
  /// otherwise.</returns>
  bool isMale() const { return theBits[SEX_BIT] == Sex::MALE; }
  int isPrimary( ben_type ioasdi ) const;
  /// <summary>Checks type of benefit, using stored values.</summary>
  ///
  /// <remarks>This version passes stored values to the static version with 1
  /// argument.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_JOASDI"/> if type of benefit is out of range.
  /// </exception>
  void joasdiCheck() const { joasdiCheck(joasdi); }
  /// <summary>Returns last year of annual QCs.</summary>
  ///
  /// <returns>Last year of annual QCs (lesser of last year of earnings and
  /// 1977).</returns>
  int lastQcyr() const { return(iend < 1977 ? iend : 1977); }
  bool need_aw( int year, bool lastTwoYears ) const;
  bool need_bases( int year ) const;
  bool need_bi( int year ) const;
  /// <summary>Returns true if date of cessation to disability is needed,
  /// using stored values.</summary>
  ///
  /// <remarks>This version passes stored values to the static version with 2
  /// arguments.</remarks>
  ///
  /// <returns>True if date of cessation to disability is needed.</returns>
  bool needDiCess() const
  { return(needDiCess(valdi, static_cast<ben_type>(joasdi))); }
  /// <summary>Checks date of prior entitlement to disability, using stored
  /// values.</summary>
  ///
  /// <remarks>This version passes stored values to the static version with 4
  /// arguments.</remarks>
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
  void priorentCheck() const
  { priorentCheck(disabPeriod[0].getEntDate(), disabPeriod[0].getOnsetDate(),
      entDate, static_cast< ben_type >(joasdi)); }
  /// <summary>Checks date of second prior entitlement to disability, using
  /// stored values.</summary>
  ///
  /// <remarks>This version passes stored valuesto the static version with 3
  /// arguments.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_PRRENT1"/> if month of prior entitlement is out of
  /// range; of type <see cref="PIA_IDS_PRRENT2"/> if year of prior
  /// entitlement is before 1940; of type <see cref="PIA_IDS_PRRENT3"/> if
  /// year of prior entitlement is after maximum allowed; of type
  /// <see cref="PIA_IDS_PRRENT4"/> if prior entitlement is not before current
  /// entitlement; of type <see cref="PIA_IDS_PRRENT5"/> if prior entitlement
  /// to disability is before disability onset.</exception>
  void priorent1Check() const
  { priorent1Check(disabPeriod[1].getEntDate(),
      disabPeriod[1].getOnsetDate(), disabPeriod[0].getEntDate()); }
  bool needQcsByYear() const;
  /// <summary>Checks for correct numbers of QC's, using stored values.
  /// </summary>
  ///
  /// <remarks>This version passes stored valuesto the static version with 2
  /// arguments.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_QC"/> if number of quarters of coverage, 1951-77, is
  /// greater than number of quarters of coverage, 1937-77.</exception>
  void qcCheck() const { qcCheck(qctottd, qctot51td); }
  void qctdCheck2();
  void setBenefitDate( const DateMoyr& newBenefitDate );
  void setBenefitDate();
  void setBirthDate( const boost::gregorian::date& newBirthDate );
  /// <summary>Sets blind indicator.</summary>
  ///
  /// <param name="newBlindind">New blind indicator.</param>
  void setBlindind( bool newBlindind ) { theBits[BLIND_BIT] = newBlindind; }
  void setCessationDate( unsigned index, const DateMoyr& dateMoyr );
  void setCessationMfb( unsigned index, float newMfb );
  void setCessationPia( unsigned index, float newPia );
  void setDeathDate( const boost::gregorian::date& newDeathDate );
  /// <summary>Sets deceased OAB indicator.</summary>
  ///
  /// <param name="newDeceasedOAB">New deceased OAB indicator.</param>
  void setDeceasedOAB( bool newDeceasedOAB )
  { theBits[DEADOAB_BIT] = newDeceasedOAB; }
  /// <summary>Sets deemed insured indicator.</summary>
  ///
  /// <param name="newDeemedind">New deemed insured indicator.</param>
  void setDeemedind( bool newDeemedind )
  { theBits[DEEMED_BIT] = newDeemedind; }
  /// <summary>Sets Medicare earnings for the specified year.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  virtual void setEarnHi( int, double ) = 0;
  /// <summary>Sets OASDI-covered earnings.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  virtual void setEarnOasdi( int, double ) = 0;
  void setEntDate( const DateMoyr& newEntDate );
  void setIbegin( int newIbegin );
  void setIend( int newIend );
  void setIndearn( bool newIndearn );
  /// <summary>Sets railroad indicator.</summary>
  ///
  /// <param name="newIndrr">New railroad indicator.</param>
  void setIndrr( bool newIndrr ) { theBits[INDRR_BIT] = newIndrr; }
  void setJoasdi( int newJoasdi );
  /// <summary>Sets mqge indicator.</summary>
  ///
  /// <param name="newMqge">New mqge indicator.</param>
  void setMqge( bool newMqge ) { theBits[MQGE_BIT] = newMqge; }
  /// <summary>Sets one line of address.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  virtual void setNhaddr ( int, const std::string& ) = 0;
  /// <summary>Sets number holder's name.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  virtual void setNhname ( const std::string& ) = 0;
  /// <summary>Sets date of cessation of oab prior to most recent dib.
  /// </summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  virtual void setOabCessDate( const DateMoyr& ) = 0;
  /// <summary>Sets date of oab entitlement prior to most recent dib.
  /// </summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  virtual void setOabEntDate( const DateMoyr& ) = 0;
  void setOnsetDate( unsigned index, const boost::gregorian::date& dateModyyr );
  void setPebsData();
  /// <summary>Sets last year of Statement earnings when computation year
  /// changes, or after a Statement calculation has been done.</summary>
  ///
  /// <param name="year">First year of projection period.</param>
  void setPebsEarn( int year ) { iend = (unsigned short)(1 + year); }
  void setPriorentDate( unsigned index, const DateMoyr& dateMoyr );
  void setPriorentDateCheck( unsigned index, const DateMoyr& dateMoyr );
  /// <summary>Sets amount of monthly noncovered pension.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  virtual void setPubpen( float ) = 0;
  /// <summary>Sets date of monthly noncovered pension entitlement.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  virtual void setPubpenDate( const DateMoyr& ) = 0;
  /// <summary>Sets amount of monthly noncovered pension after December 1994,
  /// after removing military reservist pension.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  virtual void setPubpenReservist( float ) = 0;
  void setQc51td( int qct );
  /// <summary>Sets indicator for QCs by year prior to 1978.</summary>
  ///
  /// <param name="newQcsByYear">Indicator for QCs by year prior to 1978.
  /// </param>
  void setQcsByYear( bool newQcsByYear )
  { theBits[QCSBYYEAR_BIT] = newQcsByYear; }
  void setQctd( int qct );
  /// <summary>Sets recalculation indicator.</summary>
  ///
  /// <param name="newRecalc">New recalculation indicator.</param>
  void setRecalc( bool newRecalc ) { theBits[RECALC_BIT] = newRecalc; }
  /// <summary>Sets reservist indicator.</summary>
  ///
  /// <param name="newReservist">New reservist indicator.</param>
  void setReservist( bool newReservist )
  { theBits[RESERVIST_BIT] = newReservist; }
  void setSex( Sex::sex_type newSex );
  /// <summary>Sets type of taxes for the specified year.</summary>
  ///
  /// <remarks>Needs to be implemented by a child class.</remarks>
  virtual void setTaxType( int, int ) = 0;
  /// <summary>Sets totalization indicator.</summary>
  ///
  /// <param name="newTotalize">New totalization indicator.</param>
  void setTotalize( bool newTotalize )
  { theBits[TOTALIZE_BIT] = newTotalize; }
  void setValdi( int newValdi );
  void setWaitperDate( unsigned index );
  void setWaitperDate( unsigned index, const DateMoyr& dateMoyr );
  /// <summary>Checks number of periods of disability, using stored value.
  /// </summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_VALDI1"/> if number of periods of disability is not
  /// 0, 1, or 2 in a nondisability case; of type <see cref="PIA_IDS_VALDI2"/>
  /// if number of periods of disability is not 1 or 2 in a disability case.
  /// </exception>
  void valdiCheck() const
  { if (joasdi == DISABILITY) valdi2Check(valdi);
    else valdi1Check(valdi); }
  /// <summary>Checks first month of waiting period, using stored values.
  /// </summary>
  ///
  /// <remarks>This version passes stored valuesto the static version with 3
  /// arguments.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_WAITPD1"/> if first month of waiting period is out of
  /// range; of type <see cref="PIA_IDS_WAITPD2"/> if year of first month of
  /// waiting period is before 1940; of type <see cref="PIA_IDS_WAITPD3"/> if
  /// year of first month of waiting period is after maximum allowed; of type
  /// <see cref="PIA_IDS_WAITPD4"/> if waiting period starts before onset; of
  /// type <see cref="PIA_IDS_WAITPD5"/> if entitlement is before waiting
  /// period.</exception>
  void waitpdCheck() const
  { waitpdCheck(disabPeriod[0].getWaitperDate(),
      disabPeriod[0].getOnsetDate(), entDate); }
  /// <summary>Checks first month of prior waiting period, using stored
  /// values.</summary>
  ///
  /// <remarks>This version passes stored values to the static version with 3
  /// arguments.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_WAITPD4"/> if waiting period starts before onset; of
  /// type <see cref="PIA_IDS_WAITPD6"/> if cessation is before waiting
  /// period.</exception>
  void waitpd1Check() const
  { waitpd1Check(disabPeriod[1].getWaitperDate(),
      disabPeriod[1].getOnsetDate(), disabPeriod[1].getCessationDate()); }
  virtual void zeroEarn( int ibegint, int iendt );
  /// <summary>Zeroes out HI earnings before first year
  /// and after last year of specified earnings.</summary>
  virtual void zeroEarnHi( int, int ) { };
  /// <summary>Zeroes out OASDI earnings before first year
  ///  and after last year of specified earnings.</summary>
  virtual void zeroEarnOasdi( int, int ) { };
  /// <summary>Zeroes out the type of benefit.</summary>
  void zeroOasdi() { joasdi = NO_BEN; }
  static void benefitDate1Check( const DateMoyr& datetemp );
  /// <summary>Checks that date of benefit is at or after entitlement, using
  /// passed values.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_BENDATE1"/> if benefit date is before entitlement
  /// date.</exception>
  ///
  /// <param name="entDatet">The date of entitlement.</param>
  /// <param name="benefitDatet">The date of benefit.</param>
  static void benefitDate2Check( const DateMoyr& entDatet,
    const DateMoyr& benefitDatet )
  { if (benefitDatet < entDatet) throw PiaException(PIA_IDS_BENDATE1); }
  static void birth1Check( const boost::gregorian::date& datetemp, int istartt );
  static void birth2Check( const boost::gregorian::date& datetemp );
  static void cessationCheck2( const DateMoyr& cessationDatet,
    const boost::gregorian::date& onsetDatet,
    const boost::gregorian::date& deathDatet,
    const DateMoyr& entDatet, ben_type ioasdi );
  /// <summary>Checks date of prior disability cessation against date of
  /// prior onset.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_DICESS4"/> if disability cessation before onset.
  /// </exception>
  ///
  /// <param name="cessation1Datet">The date of prior disability
  /// cessation.</param>
  /// <param name="onset1Datet">The date of prior disability onset.</param>
  static void cessation1OnsetCheck( const DateMoyr& cessation1Datet,
    const boost::gregorian::date& onset1Datet )
  { if (cessation1Datet < onset1Datet) throw PiaException(PIA_IDS_DICESS4); }
  static void cessation1Check( const DateMoyr& cessation1Datet,
    const boost::gregorian::date& onset1Datet, const DateMoyr& entDatet );
  /// <summary>Checks that date of death is after birth, using passed values.
  /// </summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_DEATH5"/> if death of worker is before birth.
  /// </exception>
  ///
  /// <param name="deathDatet">The date of death.</param>
  /// <param name="birthDatet">The date of birth.</param>
  static void deathCheck( const boost::gregorian::date& deathDatet,
    const boost::gregorian::date& birthDatet )
  { if (deathDatet < birthDatet) throw PiaException(PIA_IDS_DEATH5); }
  static void deathCheck1( const boost::gregorian::date& dateTemp,
    ben_type ioasdi );
  static void deathCheck2( const boost::gregorian::date& datetemp );
  /// <summary>Checks that date of death is before entitlement, using passed
  /// values.</summary>
  ///
  /// <remarks> Assumes that this is a survivor case. Should be called for
  /// each survivor's date of entitlement.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_DEATH4"/> if entitlement of survivor is before death
  /// of worker.</exception>
  ///
  /// <param name="deathDatet">The date of death.</param>
  /// <param name="entDatet">The date of survivor's entitlement.</param>
  static void deathCheck3( const boost::gregorian::date& deathDatet,
    const DateMoyr& entDatet )
  { if (entDatet < DateMoyr(deathDatet))
      throw PiaException(PIA_IDS_DEATH4); }
  static void disBirthEntCheck( const boost::gregorian::date& onsetDatet,
    const boost::gregorian::date& birthDatet, ben_type ioasdi,
    const DateMoyr& entDatet, const DateMoyr& benDatet );
  static void disCheck( const boost::gregorian::date& onsetDatet,
    const boost::gregorian::date& deathDatet, int valdit,
    const boost::gregorian::date& birthDatet, ben_type ioasdi,
    const DateMoyr& entDatet, const DateMoyr& benDatet );
  /// <summary>Checks date of disability onset against date of death.
  /// </summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_ONSET7"/> if death before onset in a survivor case.
  /// </exception>
  ///
  /// <param name="onsetDatet">The date of onset.</param>
  /// <param name="deathDatet">The date of death.</param>
  /// <param name="ioasdi">The type of benefit.</param>
  static void disDeathCheck( const boost::gregorian::date& onsetDatet,
    const boost::gregorian::date& deathDatet, ben_type ioasdi )
  { if (ioasdi == SURVIVOR && deathDatet < onsetDatet)
      throw PiaException(PIA_IDS_ONSET7); }
  static void dis1Check( const boost::gregorian::date& onset1Datet,
    const boost::gregorian::date& onsetDatet, int valdit,
    const boost::gregorian::date& birthDatet );
  /// <summary>Checks for no earnings of any type, using passed values.
  /// </summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_BEGIN6"/> if there are no regular, railroad, or
  /// military service earnings.</exception>
  ///
  /// <param name="indEarn">True if there are any regular OASDI earnings.
  /// </param>
  /// <param name="indRailroad">True if there are any railroad earnings.
  /// </param>
  /// <param name="indMS">True if there are any military service wage credits.
  /// </param>
  static void earningsCheck( bool indEarn, bool indRailroad, bool indMS )
  { if (!indEarn && !indRailroad && !indMS)
      throw PiaException(PIA_IDS_BEGIN6); }
  static void entCheck( const DateMoyr& datetemp );
  /// <summary>Returns maximum year allowed.</summary>
  ///
  /// <remarks>Note that this is a static function.</remarks>
  ///
  /// <returns>Maximum year allowed (default 2100).</returns>
  static int getMaxyear() { return(maxyear); }
  /// <summary>Returns last year of period for which quarters of coverage
  /// are entered as a lump sum.</summary>
  ///
  /// <returns>Last year of period for which quarters of coverage are
  /// entered as a lump sum (default 1977).</returns>
  static int getQcLumpYear() { return(qclumpyear); }
  static void ibeginCheck( int ibegint, int ibirtht );
  static void ibegin1Check( int ibegint, int istartt );
  static void ibegin2Check( int ibegint );
  static void iendCheck( int iendt );
  /// <summary>Checks type of benefit, using passed values.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_JOASDI"/> if type of benefit is out of range.
  /// </exception>
  ///
  /// <param name="joasdit">Type of benefit to check.</param>
  static void joasdiCheck( int joasdit )
  { if (joasdit < static_cast<int>(NO_BEN) ||
      joasdit > static_cast<int>(PEBS_CALC))
      throw PiaException(PIA_IDS_JOASDI); }
  /// <summary>Returns true if date of cessation to disability is needed,
  /// using passed values.</summary>
  ///
  /// <returns>True if date of cessation to disability is needed.</returns>
  ///
  /// <param name="valdit">Number of periods of disability.</param>
  /// <param name="ioasdi">Type of benefit.</param>
  static bool needDiCess( int valdit, ben_type ioasdi )
  { return(valdit > 0 && (ioasdi == OLD_AGE || ioasdi == SURVIVOR ||
      ioasdi == NO_BEN)); }
  static void priorentCheck ( const DateMoyr& priorentDatet,
    const boost::gregorian::date& onsetDatet, const DateMoyr& entDatet,
    ben_type ioasdit );
  static void priorent1Check( const DateMoyr& priorent1t,
    const boost::gregorian::date& ionsett, const DateMoyr& priorentt );
  static void pubpenCheck( float pubpent );
  static void pubpenDateCheck( const DateMoyr& pubpenDatet,
    const boost::gregorian::date& birthDatet,
    const boost::gregorian::date& deathDatet );
  /// <summary>Checks for correct numbers of QC's, using passed values.
  /// </summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_QC"/> if number of quarters of coverage, 1951-77, is
  /// greater than number of quarters of coverage, 1937-77.</exception>
  ///
  /// <param name="qctottdt">QCs from 1937 to 1977.</param>
  /// <param name="qctot51tdt">QCs from 1951 to 1977.</param>
  static void qcCheck( int qctottdt, int qctot51tdt )
  { if (qctot51tdt > qctottdt) throw PiaException(PIA_IDS_QC); }
  /// <summary>Checks summary quarters of coverage, 1951 to 1977.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_QC51TD"/> if number of quarters of coverage, 1951-77,
  /// is out of range.</exception>
  ///
  /// <param name="qct">Quarters of coverage to check.</param>
  static void qc51tdCheck( int qct )
  { if ( qct < 0 || qct > 108) throw PiaException(PIA_IDS_QC51TD); }
  /// <summary>Checks summary quarters of coverage, 1937 to 1977.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_QCTD"/> if number of quarters of coverage, 1937-77,
  /// is out of range.</exception>
  ///
  /// <param name="qct">Quarters of coverage to check.</param>
  static void qctdCheck( int qct )
  { if ( qct < 0 || qct > 164) throw PiaException(PIA_IDS_QCTD); }
  /// <summary>Sets maximum year allowed.</summary>
  ///
  /// <remarks>Note that this is a static function.</remarks>
  ///
  /// <param name="newMaxyear">New maximum year allowed.</param>
  static void setMaxyear( int newMaxyear ) { maxyear = newMaxyear; }
  /// <summary>Sets last year of period for which quarters of coverage are
  /// entered as a lump sum.</summary>
  ///
  /// <param name="newQcLumpYear">New last year of period for which
  /// quarters of coverage are entered as a lump sum.</param>
  static void setQcLumpYear( int newQcLumpYear )
  { qclumpyear = newQcLumpYear; }
  /// <summary>Checks type of taxes.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_TAXTYPE"/> if type of taxes is not 0 or 1.
  /// </exception>
  ///
  /// <param name="taxTypet">Type of taxes to check.</param>
  static void taxTypeCheck( int taxTypet )
  { if (taxTypet > 1 || taxTypet < 0)
      throw PiaException(PIA_IDS_TAXTYPE); }
  /// <summary>Checks number of periods of disability in nondisability case.
  /// </summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_VALDI1"/> if number of periods of disability is not
  /// 0, 1, or 2.</exception>
  ///
  /// <param name="valdit">Number of periods to check.</param>
  static void valdi1Check( int valdit )
  { if (valdit < 0 || valdit > NUM_DISAB_PERIODS)
      throw PiaException(PIA_IDS_VALDI1); }
  /// <summary>Checks number of periods of disability in disability case.
  /// </summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_VALDI2"/> if number of periods of disability is not
  /// 1 or 2.</exception>
  ///
  /// <param name="valdit">Number of periods of disability to check.</param>
  static void valdi2Check( int valdit )
  { if (valdit < 1 || valdit > NUM_DISAB_PERIODS)
      throw PiaException(PIA_IDS_VALDI2); }
  /// <summary>Checks first month of waiting period, using passed values.
  /// </summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_WAITPD1"/> if first month of waiting period is out of
  /// range; of type <see cref="PIA_IDS_WAITPD2"/> if year of first month of
  /// waiting period is before 1940; of type <see cref="PIA_IDS_WAITPD3"/> if
  /// year of first month of waiting period is after maximum allowed; of type
  /// <see cref="PIA_IDS_WAITPD4"/> if waiting period starts before onset; of
  /// type <see cref="PIA_IDS_WAITPD5"/> if entitlement is before waiting
  /// period.</exception>
  ///
  /// <param name="waitperDatet">First month of waiting period.</param>
  /// <param name="onsetDatet">The date of onset.</param>
  /// <param name="entDatet">The date of entitlement.</param>
  static void waitpdCheck( const DateMoyr& waitperDatet,
    const boost::gregorian::date& onsetDatet, const DateMoyr& entDatet )
  { if (waitperDatet < onsetDatet) throw PiaException(PIA_IDS_WAITPD4);
    if (entDatet < waitperDatet) throw PiaException(PIA_IDS_WAITPD5); }
  /// <summary>Checks first month of prior waiting period, using passed
  /// values.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_WAITPD4"/> if waiting period starts before onset; of
  /// type <see cref="PIA_IDS_WAITPD6"/> if cessation is before waiting
  /// period.</exception>
  ///
  /// <param name="waitper1Datet">First month of prior waiting period.</param>
  /// <param name="onset1Datet">The date of prior onset.</param>
  /// <param name="cessation1Datet">The date of prior cessation.</param>
  static void waitpd1Check( const DateMoyr& waitper1Datet,
    const boost::gregorian::date& onset1Datet,
    const DateMoyr& cessation1Datet )
  { if (waitper1Datet < onset1Datet)
      throw PiaException(PIA_IDS_WAITPD4);
    if (cessation1Datet < waitper1Datet)
      throw PiaException(PIA_IDS_WAITPD6); }
};
