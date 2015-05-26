// Declarations for the <see cref="PiaData"/> class to set up data to
// calculate and print out a PIA.

// $Id: piadata.h 1.78 2014/01/05 07:55:53EST 277133 Development  $

#pragma once

#include "WorkerDataGeneral.h"
#include "age.h"
#include "qtryear.h"
#include "comppd.h"
#include "dinscode.h"
#include "inscode.h"
#include "frzyrs.h"
#include "datemoyr.h"
#include "qcamt.h"
#include "awinc.h"
#include "avgwg.h"
#include "BenefitAmount.h"
#include "BitAnnual.h"
#include "boost/date_time/gregorian/greg_date.hpp"

/// <summary>Manages all of the worker-specific amounts calculated in
/// preparation for and during the calculation of a Social Security benefit.
/// </summary>
///
/// <remarks>Manages all of the worker-specific amounts calculated in
/// preparation for and during the calculation of a Social Security benefit.
/// </remarks>
class PiaData
{
public:
  /// <summary>Special minimum-drc indicator.</summary>
  enum arf_app_type {
    /// <summary>No special minimum, or no delayed retirement credits.
    /// </summary>
    NO_SPEC_MIN_DRC,
    /// <summary>Highest PIA is special minimum, delayed retirement credits
    /// are involved, and benefit equals special minimum PIA.</summary>
    SPEC_MIN_BEN,
    /// <summary>Highest PIA is special minimum, delayed retirement
    /// credits are involved, and benefit equals support PIA
    /// increased by increment factor.</summary>
    SUPPORT_BEN
  };
  /// <summary>Types of earnings (without or with totalization).</summary>
  enum earn_total_type {
    /// <summary>Without considering totalization.</summary>
    EARN_NO_TOTALIZATION,
    /// <summary>After considering attributed earnings for totalization.
    /// </summary>
    EARN_WITH_TOTALIZATION,
    /// <summary>Number of types of earnings (without or with totalization).
    /// </summary>
    NUM_EARN_TOTALIZATION
  };
  /// <summary><see cref="Age"/> at benefit date of worker.</summary>
  Age ageBen;
  /// <summary><see cref="Age"/> at entitlement of worker.</summary>
  Age ageEnt;
  /// <summary>Benefit actually payable, before dollar rounding.</summary>
  ///
  /// <remarks>See <see cref="PiaData::roundedBenefit"/> for the benefit after
  /// dollar rounding.</remarks>
  BenefitAmount unroundedBenefit;
  /// <summary>Indicators for use of child care credits.</summary>
  ///
  /// <remarks>True (1) if using child care credit that year, false (0)
  /// otherwise.</remarks>
  BitAnnual childCareOrder;
  /// <summary>The maximum amount of credit for each child care year,
  /// and the year in which it should be credited.</summary>
  std::vector< std::pair<double, int> > childCareMaxCredit;
  /// <summary>Computation period for new-start calculations.</summary>
  CompPeriod compPeriodNew;
  /// <summary>Computation period for non-freeze new-start calculations.</summary>
  CompPeriod compPeriodNewNonFreeze;
  /// <summary>Computation period for old-start calculation.</summary>
  CompPeriod compPeriodOld;
  /// <summary>Disability insured status code.</summary>
  DisInsCode disInsCode;
  /// <summary>Disability insured status code for non-freeze.</summary>
  DisInsCode disInsNonFreezeCode;
  /// <summary>Earliest possible retirement age.</summary>
  Age earlyRetAge;
  /// <summary>Military service wage credits, 1951 to date.</summary>
  DoubleAnnual earnMilServ;
  /// <summary>Worker HI earnings, 1937 to maxyear.</summary>
  AverageWage earnHi;
  /// <summary>Worker HI earnings limited to wage base, 1937 to maxyear.
  /// </summary>
  AverageWage earnHiLimited;
  /// <summary>Worker OASDI earnings, 1937 to maxyear, including
  /// railroad and military service.</summary>
  AverageWage earnOasdi;
  /// <summary>Worker OASDI earnings limited to wage base, 1937 to
  /// maxyear, including railroad and military service.</summary>
  AverageWage earnOasdiLimited;
  /// <summary>Worker OASDI earnings attributed for totalization, 1937
  /// to maxyear.</summary>
  AverageWage earnTotalized;
  /// <summary>Worker OASDI earnings attributed for totalization,
  /// limited to wage base, 1937 to maxyear.</summary>
  AverageWage earnTotalizedLimited;
  /// <summary>Fully insured status code.</summary>
  InsCode finsCode;
  /// <summary>Fully insured non-freeze status code.</summary>
  InsCode finsNonFreezeCode;
  /// <summary>Full retirement age.</summary>
  Age fullRetAge;
  /// <summary>Years wholly within a freeze period.</summary>
  FreezeYears freezeYears;
  /// <summary>Highest applicable MFB.</summary>
  BenefitAmount highMfb;
  /// <summary>Highest applicable PIA.</summary>
  BenefitAmount highPia;
  /// <summary>Years partially within a freeze period.</summary>
  FreezeYears partialFreezeYears;
  /// <summary>Quarters of coverage, including railroad and military service.
  /// </summary>
  QcArray qcov;
  /// <summary>Military service quarters of coverage, 1937 to 1956.</summary>
  QcArray qcovMilServ;
  /// <summary>Annual relative earnings position, 1937 to maxyear.</summary>
  DoubleAnnual relEarnPosition;
  /// <summary>Benefit actually payable, after dollar rounding.</summary>
  ///
  /// <remarks>See <see cref="unroundedBenefit"/> for the benefit before
  /// dollar rounding.</remarks>
  BenefitAmount roundedBenefit;
  /// <summary>Support PIA when highest PIA is special minimum and
  /// delayed retirement credits are involved.</summary>
  BenefitAmount supportPia;
private:
   /// <summary>True if entitlement after 1990 amendments.</summary>
   bool amend90;
   /// <summary>Reduction or increment factor.</summary>
   double arf;
   /// <summary>Special minimum-drc indicator.</summary>
   arf_app_type arfApp;
   /// <summary>Number of child care years used in PIA computation.</summary>
   int childCareYearsTotal;
   /// <summary>Number of quarters of coverage required for
   ///  deemed fully insured status.</summary>
   int deemedQcReq;
   /// <summary>Total number of quarters of coverage earned for deemed fully
   ///  insured status.</summary>
   int deemedQctot;
   /// <summary>Indicator for calculation of quarters of coverage for
   ///   early years.</summary>
   ///
   /// <remarks>Possible values are:
   /// <table>
   ///  true    Calculate total by adding earnings qcs, railroad qcs, and military service qcs (as in Anypia).
   ///  false   Use annual totals as read in (as for microsimulation data).
   /// </table></remarks>
   bool doEarlyQcs;
   /// <summary>Military service wage credits, 1937-50.</summary>
   double earn3750ms;
   /// <summary>First year of earnings after 1950, calculated 2 different
   /// ways.</summary>
   ///
   /// <remarks>Index 0 is without considering totalization. Index 1 is
   /// after considering attributed earnings for totalization.</remarks>
   int earn50[NUM_EARN_TOTALIZATION];
   /// <summary>Total earnings prior to 1951, calculated 2 different ways.
   /// </summary>
   ///
   /// <remarks>Index 0 is actual. Index 1 is attributed for totalization.
   /// </remarks>
   double earnTotal50[NUM_EARN_TOTALIZATION];
  /// <summary>Year prior to year of retirement or disability,
  /// or exact year of death (last year of earnings considered).</summary>
  int earnYear;
  /// <summary>Worker's month and year of eligibility, before considering
  /// prior disability (date of worker's attainment of age 62, for old-age
  /// and disability, or date of worker's death, if earlier, for survivor).
  /// </summary>
  DateMoyr eligDate;
  /// <summary>Fully insured status code, as used in OACT printouts.
  /// </summary>
  char finsCode2;
  /// <summary>Fully insured non-freeze status code, as used in OACT printouts.
  /// </summary>
  char finsNonFreezeCode2;
  /// <summary>The date of attainment of fully insured status.</summary>
  DateMoyr fullInsDate;
  /// <summary>The date of attainment of full retirement age.</summary>
  DateMoyr fullRetDate;
  /// <summary>Applicable PIA method number (see
  /// <see cref="PiaMethod::pia_type"/>).</summary>
  int iappn;
  /// <summary>Support PIA method number (see
  /// <see cref="PiaMethod::pia_type"/>).</summary>
  int iapps;
  /// <summary>First year of earnings after considering railroad and
  /// military service.</summary>
  int ibeginAll;
  /// <summary>First year of attributed earnings in totalization case.
  /// </summary>
  int ibeginTotal;
  /// <summary>Year of eligibility after considering prior
  /// disability onset.</summary>
  int ielgyr;
  /// <summary>Year of eligibility for non-freeze computations.</summary>
  int ielgyrnf;
  /// <summary>Last year of earnings after considering railroad and
  /// military service.</summary>
  int iendAll;
  /// <summary>Last year of attributed earnings in totalization case.
  /// </summary>
  int iendTotal;
  /// <summary>Day before birth of worker.</summary>
  boost::gregorian::date kbirth;
  /// <summary>Mumber of months of early or delayed retirement.</summary>
  int monthsArdri;
  /// <summary>Indicator for earnings over maximum.</summary>
  ///
  /// <remarks>Possible values:
  /// <table>
  ///  false   no earnings over <see cref="MAXEARN"/>.
  ///  true    some earnings over <see cref="MAXEARN"/>.
  /// </table></remarks>
  bool overMax;
  /// <summary>Code for type of pia (see <see cref="Pifc::theCode"/>).
  /// </summary>
  char pifc;
  /// <summary>Military service quarters of coverage, 1937-50.</summary>
  int qc3750ms;
  /// <summary>Simplified quarters of coverage, 1937-50.</summary>
  int qc3750simp;
  /// <summary>Quarters of coverage in prior 3 years.</summary>
  int qcCurrent;
  /// <summary>Quarters of coverage in prior 3 years for non-freeze.</summary>
  int qcCurrentNonFreeze;
  /// <summary>Beginning quarter and year of period of required
  /// quarters of coverage for disability insured status.</summary>
  QtrYear qcDisDate1;
  /// <summary>Ending quarter and year of period of required
  /// quarters of coverage for disability insured status.</summary>
  QtrYear qcDisDate2;
  /// <summary>Beginning quarter and year of period of required quarters of
  /// coverage for disability insured status, for period after disability
  /// cessation.</summary>
  QtrYear qcDisDate3;
  /// <summary>Ending quarter and year of period of required quarters of
  /// coverage for disability insured status, for period after disability
  /// cessation.</summary>
  QtrYear qcDisDate4;
  /// <summary>Beginning quarter and year of period of required quarters of
  /// coverage for disability insured status, for period before prior
  /// disability.</summary>
  QtrYear qcDisDate5;
  /// <summary>Ending quarter and year of period of required quarters of
  /// coverage for disability insured status, for period before prior
  /// disability.</summary>
  QtrYear qcDisDate6;
  /// <summary>Beginning quarter and year of period of required
  /// quarters of coverage for disability insured status for
  /// non-freeze.</summary>
  QtrYear qcDisDateNonFreeze1;
  /// <summary>Ending quarter and year of period of required
  /// quarters of coverage for disability insured status for
  /// non-freeze.</summary>
  QtrYear qcDisDateNonFreeze2;
  /// <summary>Number of quarters over which quarters of
  /// coverage must be earned for disability insured status.</summary>
  int qcDisQtr;
  /// <summary>Number of quarters over which quarters of coverage
  /// must be earned for disability insured status, based on elapsed
  /// quarters after disability cessation (included in qcDisQtr).</summary>
  int qcDisQtr2;
  /// <summary>Number of quarters over which quarters of
  /// coverage must be earned for disability insured status.</summary>
  int qcDisQtrNonFreeze;
  /// <summary>Number of quarters of coverage required for
  /// disability insured status.</summary>
  int qcDisReq;
  /// <summary>Number of quarters of coverage required for
  /// disability insured status.</summary>
  int qcDisReqNonFreeze;
  /// <summary>Number of years over which quarters of coverage
  /// must be earned for disability insured status for non-freeze.</summary>
  int qcDisYears;
  /// <summary>Number of years over which quarters of coverage
  /// must be earned for disability insured status for non-freeze.</summary>
  int qcDisYearsNonFreeze;
  /// <summary>Number of quarters of coverage required for fully insured
  /// status.</summary>
  int qcReq;
  /// <summary>Number of quarters of coverage required for fully insured
  /// non-freeze status.</summary>
  int qcReqNonFreeze;
  /// <summary>Number of quarters of coverage required for permanently
  /// insured status.</summary>
  int qcReqPerm;
  /// <summary>Number of quarters of coverage required for permanently
  /// insured non-freeze status.</summary>
  int qcReqPermNonFreeze;
  /// <summary>Total number of quarters of coverage earned.</summary>
  int qcTotal;
  /// <summary>Total number of quarters of coverage earned for non-freeze.</summary>
  int qcTotalNonFreeze;
  /// <summary>Number of quarters of coverage earned from 1937 to 1950.
  /// </summary>
  int qcTotal50;
  /// <summary>Number of quarters of coverage earned after 1950.</summary>
  int qcTotal51;
  /// <summary>Number of quarters of coverage earned after 1950 for non-freeze.</summary>
  int qcTotal51NonFreeze;
  /// <summary>Number of quarters of coverage earned for
  /// disability insured status.</summary>
  int qcTotalDis;
  /// <summary>Number of quarters of coverage earned for
  /// disability insured status for non-freeze computation.</summary>
  int qcTotalDisNonFreeze;
  /// <summary>Number of quarters of coverage earned for
  /// a totalization benefit computation.</summary>
  int qcTotalRel;
  /// <summary>Average relative earnings position.</summary>
  double relEarnPositionAverage;
  /// <summary>Starting point of annual qc's, if using sum of qc's
  /// through 1950.</summary>
  static const QtrYear qtr151;
  /// <summary>Starting point of annual qc's, if using sum of qc's
  /// through 1977.</summary>
  static const QtrYear qtr178;
  /// <summary>The earliest possible retirement date.</summary>
  static const DateMoyr jan1937;
public:
  PiaData();
  ~PiaData();
  PiaData& operator=( const PiaData& newData );
  void deleteContents();
  void earn50Cal( const WorkerDataGeneral& workerData );
  void earnHiCal( const WorkerDataGeneral& workerData );
  void earnProjection( const WorkerDataGeneral& workerData );
  void earnTotal50Cal0();
  void earnTotal50Cal1();
  void earnYearCal( const WorkerDataGeneral& workerData,
    WorkerDataGeneral::ben_type ioasdi);
  void freezeYearsCal( const WorkerDataGeneral& workerData,
    WorkerDataGeneral::ben_type ioasdi, const Age& fullRetAgeDI );
  /// <summary>Returns indicator for entitlement after 1990 amendments.
  /// </summary>
  ///
  /// <returns>Indicator for entitlement after 1990 amendments.</returns>
  bool getAmend90() const { return(amend90); }
  /// <summary>Returns actuarial reduction factor or delayed retirement
  /// credit.</summary>
  ///
  /// <returns>Actuarial reduction factor or delayed retirement
  /// credit.</returns>
  double getArf() const { return(arf); }
  /// <summary>Returns special minimum-delayed retirement credit
  /// indicator.</summary>
  ///
  /// <returns>Special minimum-delayed retirement credit indicator.</returns>
  arf_app_type getArfApp() const { return(arfApp); }
  /// <summary>Returns day before worker's birth.</summary>
  ///
  /// <returns>Day before worker's birth.</returns>
  const boost::gregorian::date& getBirthDateMinus1() const
  { return(kbirth); }
  double getChildCareMaxCredit( int yearOfCredit ) const;
  /// <summary>Returns total number of child care years.</summary>
  ///
  /// <returns>Total number of child care years.</returns>
  int getChildCareYearsTotal() const { return childCareYearsTotal; }
  /// <summary>Returns number of quarters required for deemed fully insured
  /// status.</summary>
  ///
  /// <returns>Number of quarters required for deemed fully insured status.
  /// </returns>
  int getDeemedQcReq() const { return(deemedQcReq); }
  /// <summary>Returns military service earnings, 1937-50.</summary>
  ///
  /// <returns>Military service earnings, 1937-50.</returns>
  double getEarn3750ms() const { return(earn3750ms); }
  /// <summary>Returns first year of earnings after 1950.</summary>
  ///
  /// <returns>First year of earnings after 1950.</returns>
  ///
  /// <param name="type">Type of earnings.</param>
  int getEarn50( earn_total_type type ) const { return(earn50[(int)type]); }
  /// <summary>Returns total earnings prior to 1951.</summary>
  ///
  /// <returns>Total earnings prior to 1951.</returns>
  ///
  /// <param name="type">Type of earnings.</param>
  double getEarnTotal50( earn_total_type type ) const
  { return(earnTotal50[(int)type]); }
  /// <summary>Returns last year of earnings considered.</summary>
  ///
  /// <returns>Last year of earnings considered.</returns>
  int getEarnYear() const { return(earnYear); }
  /// <summary>Returns worker's month and year of eligibility before
  /// considering prior disability onset.</summary>
  ///
  /// <returns>Worker's month and year of eligibility before
  /// considering prior disability onset.</returns>
  const DateMoyr& getEligDate() const { return(eligDate); }
  /// <summary>Returns worker's year of eligibility after considering
  /// prior disability onset.</summary>
  ///
  /// <returns>Worker's year of eligibility after considering
  /// prior disability onset.</returns>
  int getEligYear() const { return(ielgyr); }
  /// <summary>Returns worker's year of eligibility for non-freeze
  /// computations.</summary>
  ///
  /// <returns>Worker's year of eligibility for non-freeze
  /// computations.</returns>
  int getEligYearNonFreeze() const { return(ielgyrnf); }
  /// <summary>Returns fully insured code as used in oact printouts.
  /// </summary>
  ///
  /// <returns>Fully insured code as used in oact printouts.</returns>
  char getFinsCode2() const { return(finsCode2); }
  /// <summary>Returns fully insured non-freeze code as used in oact printouts.
  /// </summary>
  ///
  /// <returns>Fully insured non-freeze code as used in oact printouts.</returns>
  char getFinsNonFreezeCode2() const { return(finsNonFreezeCode2); }
  int getFirstQcYear( const WorkerDataGeneral& workerData ) const;
  /// <summary>Returns date fully insured status is attained.</summary>
  ///
  /// <returns>The date fully insured status is attained.</returns>
  const DateMoyr& getFullInsDate() const { return(fullInsDate); }
  /// <summary>Returns date full retirement age is attained.</summary>
  ///
  /// <returns>The date full retirement age is attained.</returns>
  const DateMoyr& getFullRetDate() const { return(fullRetDate); }
  /// <summary>Returns number of applicable pia calculation method.</summary>
  ///
  /// <returns>Number of applicable pia calculation method.</returns>
  int getIappn () const { return(iappn); }
  /// <summary>Returns support PIA method number.</summary>
  ///
  /// <returns>Support PIA method number.</returns>
  int getIapps () const { return(iapps); }
  /// <summary>Returns first year of earnings after considering railroad
  /// and military service.</summary>
  ///
  /// <returns>First year of earnings after considering railroad and
  /// military service.</returns>
  int getIbeginAll() const { return ibeginAll; }
  /// <summary>Returns first year of attributed earnings in totalization
  /// case.</summary>
  ///
  /// <returns>First year of attributed earnings in totalization case.
  /// </returns>
  int getIbeginTotal() const { return(ibeginTotal); }
  /// <summary>Returns last year of earnings after considering railroad and
  /// military service.</summary>
  ///
  /// <returns>Last year of earnings after considering railroad and
  /// military service.</returns>
  int getIendAll() const { return iendAll; }
  /// <summary>Returns last year of attributed earnings in totalization case.
  /// </summary>
  ///
  /// <returns>Last year of attributed earnings in totalization case.
  /// </returns>
  int getIendTotal() const { return(iendTotal); }
  int getLastQcYear( const WorkerDataGeneral& workerData ) const;
  /// <summary>Returns number of months of increment or reduction.</summary>
  ///
  /// <returns>Number of months of increment or reduction.</returns>
  int getMonthsArdri() const { return(monthsArdri); }
  /// <summary>Returns indicator for earnings over maximum.</summary>
  ///
  /// <returns>True if any earnings over maximum, false otherwise.</returns>
  bool getOverMax() const { return(overMax); }
  /// <summary>Returns code for type of pia.</summary>
  ///
  /// <returns>Code for type of pia.</returns>
  char getPifc() const { return(pifc); }
  float getPubpenApp( const WorkerDataGeneral& workerData ) const;
  /// <summary>Returns military service quarters of coverage, 1937-50.
  /// </summary>
  ///
  /// <returns>Military service quarters of coverage, 1937-50.</returns>
  int getQc3750ms() const { return(qc3750ms); }
  /// <summary>Returns simplified quarters of coverage, 1937-50.</summary>
  ///
  /// <returns>Simplified quarters of coverage, 1937-50.</returns>
  int getQc3750simp() const { return(qc3750simp); }
  /// <summary>Returns number of quarters earned in prior 3 years.</summary>
  ///
  /// <returns>Number of quarters earned in prior 3 years.</returns>
  int getQcCurrent() const { return(qcCurrent); }
  /// <summary>Returns number of quarters earned in prior 3 years for non-freeze.</summary>
  ///
  /// <returns>Number of quarters earned in prior 3 years for non-freeze.</returns>
  int getQcCurrentNonFreeze() const { return(qcCurrentNonFreeze); }
  /// <summary>Returns number of quarters over which quarters of coverage
  /// must be earned for disability insured status.</summary>
  ///
  /// <returns>Number of quarters over which quarters of coverage must
  /// be earned for disability insured status.</returns>
  int getQcDisQtr() const { return(qcDisQtr); }
  /// <summary>Returns number of quarters over which quarters of coverage
  /// must be earned for disability insured status, based on elapsed quarters
  /// after disability cessation.</summary>
  ///
  /// <returns>Number of quarters over which quarters of coverage must
  /// be earned for disability insured status, based on elapsed quarters
  /// after disability cessation.</returns>
  int getQcDisQtr2() const { return(qcDisQtr2); }
  /// <summary>Returns number of quarters over which quarters of coverage
  /// must be earned for disability insured status for non-freeze.</summary>
  ///
  /// <returns>Number of quarters over which quarters of coverage must
  /// be earned for disability insured status for non-freeze.</returns>
  int getQcDisQtrNonFreeze() const { return(qcDisQtrNonFreeze); }
  /// <summary>Returns number of quarters required for disability insured
  /// status.</summary>
  ///
  /// <returns>Number of quarters required for disability insured status.
  /// </returns>
  int getQcDisReq() const { return(qcDisReq); }
  /// <summary>Returns number of quarters required for disability insured
  /// status for non-freeze.</summary>
  ///
  /// <returns>Number of quarters required for disability insured status
  /// for non-freeze.</returns>
  int getQcDisReqNonFreeze() const { return(qcDisReqNonFreeze); }
  /// <summary>Returns beginning quarter and year of period of required
  /// quarters of coverage for disability insured status.</summary>
  ///
  /// <returns>Beginning quarter and year of period of required
  /// quarters of coverage for disability insured status.</returns>
  const QtrYear& getQcDisDate1() const { return qcDisDate1; }
  /// <summary>Returns ending quarter and year of period of required
  /// quarters of coverage for disability insured status.</summary>
  ///
  /// <returns>Ending quarter and year of period of required quarters
  /// of coverage for disability insured status.</returns>
  const QtrYear& getQcDisDate2() const { return qcDisDate2; }
  /// <summary>Returns beginning quarter and year of period of required
  /// quarters of coverage for disability insured status, for period after
  /// disability cessation.</summary>
  ///
  /// <returns>Beginning quarter and year of period of required quarters of
  /// coverage for disability insured status, for period after disability
  /// cessation.</returns>
  const QtrYear& getQcDisDate3() const { return qcDisDate3; }
  /// <summary>Returns ending quarter and year of period of required quarters
  // of coverage for disability insured status, for period after disability
  /// cessation.</summary>
  ///
  /// <returns>Ending quarter and year of period of required quarters of
  /// coverage for disability insured status, for period after disability
  /// cessation.</returns>
  const QtrYear& getQcDisDate4() const { return qcDisDate4; }
  /// <summary>Returns beginning quarter and year of period of required
  /// quarters of coverage for disability insured status, for period before
  /// prior disability.</summary>
  ///
  /// <returns>Beginning quarter and year of period of required quarters of
  /// coverage for disability insured status, for period before prior
  /// disability.</returns>
  const QtrYear& getQcDisDate5() const { return qcDisDate5; }
  /// <summary>Returns ending quarter and year of period of required quarters
  /// of coverage for disability insured status, for period before prior
  /// disability.</summary>
  ///
  /// <returns>Ending quarter and year of period of required quarters of
  /// coverage for disability insured status, for period before prior
  /// disability.</returns>
  const QtrYear& getQcDisDate6() const { return qcDisDate6; }
  /// <summary>Returns beginning quarter and year of period of required
  /// quarters of coverage for disability insured status for non-freeze.</summary>
  ///
  /// <returns>Beginning quarter and year of period of required
  /// quarters of coverage for disability insured status for non-freeze.</returns>
  const QtrYear& getQcDisDateNonFreeze1() const { return qcDisDateNonFreeze1; }
  /// <summary>Returns ending quarter and year of period of required
  /// quarters of coverage for disability insured status for non-freeze.</summary>
  ///
  /// <returns>Ending quarter and year of period of required quarters
  /// of coverage for disability insured status for non-freeze.</returns>
  const QtrYear& getQcDisDateNonFreeze2() const { return qcDisDateNonFreeze2; }
  /// <summary>Returns number of years over which quarters of coverage must
  /// be earned for disability insured status.</summary>
  ///
  /// <returns>Number of years over which quarters of coverage must be
  /// earned for disability insured status.</returns>
  int getQcDisYears() const { return(qcDisYears); }
  /// <summary>Returns number of years over which quarters of coverage must
  /// be earned for disability insured status for non-freeze.</summary>
  ///
  /// <returns>Number of years over which quarters of coverage must be
  /// earned for disability insured status for non-freeze.</returns>
  int getQcDisYearsNonFreeze() const { return(qcDisYearsNonFreeze); }
  /// <summary>Returns number of quarters required for fully insured status.
  /// </summary>
  ///
  /// <returns>Number of quarters required for fully insured status.
  /// </returns>
  int getQcReq() const { return(qcReq); }
  /// <summary>Returns number of quarters required for fully insured non-freeze status.
  /// </summary>
  ///
  /// <returns>Number of quarters required for fully insured non-freeze status.
  /// </returns>
  int getQcReqNonFreeze() const { return(qcReqNonFreeze); }
  /// <summary>Returns number of quarters required for permanently insured
  /// status.</summary>
  ///
  /// <returns>Number of quarters required for permanently insured status.
  /// </returns>
  int getQcReqPerm() const { return(qcReqPerm); }
  /// <summary>Returns number of quarters required for permanently insured
  /// non-freeze status.</summary>
  ///
  /// <returns>Number of quarters required for permanently insured non-freeze status.
  /// </returns>
  int getQcReqPermNonFreeze() const { return(qcReqPermNonFreeze); }
  /// <summary>Returns total number of quarters earned for fully insured
  /// status.</summary>
  ///
  /// <returns>Total number of quarters earned for fully insured status.
  /// </returns>
  int getQcTotal() const { return(qcTotal); }
  /// <summary>Returns total number of quarters earned for fully insured
  /// non-freeze status.</summary>
  ///
  /// <returns>Total number of quarters earned for fully insured non-freeze status.
  /// </returns>
  int getQcTotalNonFreeze() const { return(qcTotalNonFreeze); }
  /// <summary>Returns total number of quarters earned from 1937 to 1950.
  /// </summary>
  ///
  /// <returns>Total number of quarters earned from 1937 to 1950.</returns>
  int getQcTotal50() const { return(qcTotal50); }
  /// <summary>Returns total number of quarters earned after 1950.</summary>
  ///
  /// <returns>Total number of quarters earned after 1950.</returns>
  int getQcTotal51() const { return(qcTotal51); }
  /// <summary>Returns total number of quarters earned after 1950 for non-freeze.</summary>
  ///
  /// <returns>Total number of quarters earned after 1950 for non-freeze.</returns>
  int getQcTotal51NonFreeze() const { return(qcTotal51NonFreeze); }
  /// <summary>Returns total number of quarters earned for disability
  /// insured status.</summary>
  ///
  /// <returns>Total number of quarters earned for disability insured
  /// status.</returns>
  int getQcTotalDis() const { return(qcTotalDis); }
  /// <summary>Returns total number of quarters earned for disability
  /// insured status for non-freeze.</summary>
  ///
  /// <returns>Total number of quarters earned for disability insured
  /// status for non-freeze.</returns>
  int getQcTotalDisNonFreeze() const { return(qcTotalDisNonFreeze); }
  /// <summary>Returns total number of quarters earned for totalization
  /// benefit computation.</summary>
  ///
  /// <returns>Total number of quarters earned for disability insured
  /// status.</returns>
  int getQcTotalRel() const { return(qcTotalRel); }
  /// <summary>Returns average relative earnings position.</summary>
  ///
  /// <returns>Average relative earnings position.</returns>
  double getRelEarnPositionAverage() const { return(relEarnPositionAverage); }
  void initialize();
  void qcCal( WorkerDataGeneral& workerData, const Qcamt& qcamt );
  void qcDisCal( const WorkerDataGeneral& workerData );
  void qcDisReqCal( const WorkerDataGeneral& workerData,
    const DateMoyr& date, int trial, bool dospecial, bool dofreeze );
  void qcDisReq2Cal( const WorkerDataGeneral& workerData,
    const DateMoyr& date );
  void qcDiSpec( int qcdiqtrt, int period );
  void qcDisNonFreezeCal( const WorkerDataGeneral& workerData );
  void qcDisReqNonFreezeCal( const WorkerDataGeneral& workerData,
    const DateMoyr& date, int trial, bool dospecial, bool dofreeze );
  void qcDiSpecNonFreeze( int qcdiqtrt, int period );
  void qcTotalCal( const WorkerDataGeneral& workerData,
    const QtrYear& qtrYear );
  void qcTotalNonFreezeCal( const WorkerDataGeneral& workerData,
    const QtrYear& qtrYear );
  /// <summary>Sets indicator for entitlement after 1990 amendments.</summary>
  ///
  /// <param name="newAmend90">New indicator.</param>
  void setAmend90( bool newAmend90 ) { amend90 = newAmend90; }
  /// <summary>Sets actuarial reduction factor or delayed retirement credit.
  /// </summary>
  ///
  /// <param name="newArf">New actuarial reduction factor or delayed
  /// retirement credit.</param>
  void setArf( double newArf ) { arf = newArf; }
  /// <summary>Sets special minimum-delayed retirement credit indicator.
  /// </summary>
  ///
  /// <param name="newArfApp">New indicator.</param>
  void setArfApp( arf_app_type newArfApp ) { arfApp = newArfApp; }
  void setBirthDateMinus1( const boost::gregorian::date& birthDate );
  /// <summary>Sets total number of child care years.</summary>
  ///
  /// <param name="totalYears">New total number of child care years.</param>
  void setChildCareYearsTotal( int totalYears )
  { childCareYearsTotal = totalYears; }
  /// <summary>Sets number of quarters of coverage required for deemed
  /// insured status.</summary>
  ///
  /// <param name="newDeemedQcReq">New required quarters for deemed insured
  /// status.</param>
  void setDeemedQcReq( int newDeemedQcReq ) { deemedQcReq = newDeemedQcReq; }
  /// <summary>Sets indicator for calculation of quarters of coverage for
  /// early years.</summary>
  ///
  /// <param name="newDoEarlyQcs">New indicator.
  ///
  ///     true  - Calculate total by adding earnings qcs, railroad qcs, and
  ///             military service qcs (as in Anypia).
  ///     false - Use annual totals as read in (as for microsimulation
  ///             data).</param>
  void setDoEarlyQcs( bool newDoEarlyQcs ) { doEarlyQcs = newDoEarlyQcs; }
  /// <summary>Sets military service earnings, 1937-50.</summary>
  ///
  /// <param name="newEarn3750ms">New value of military service earnings,
  /// 1937-50.</param>
  void setEarn3750ms( double newEarn3750ms ) { earn3750ms = newEarn3750ms; }
  /// <summary>Sets month and year of eligibility before considering
  /// disability.</summary>
  ///
  /// <param name="newEligDate">New date of eligibility.</param>
  void setEligDate( const DateMoyr& newEligDate ) { eligDate = newEligDate; }
  /// <summary>Sets eligibility year after considering prior disability
  /// onset.</summary>
  ///
  /// <param name="newEligYear">New eligibility year.</param>
  void setEligYear( int newEligYear ) { ielgyr = newEligYear; }
  /// <summary>Sets eligibility year for non-freeze computations.</summary>
  ///
  /// <param name="newEligYear">New eligibility year.</param>
  void setEligYearNonFreeze( int newEligYear ) { ielgyrnf = newEligYear; }
  /// <summary>Sets fully insured status code as used in oact printouts.
  /// </summary>
  ///
  /// <param name="newFinsCode2">New insured status code.</param>
  void setFinsCode2( char newFinsCode2 ) { finsCode2 = newFinsCode2; }
  /// <summary>Sets fully insured non-freeze status code as used in oact printouts.
  /// </summary>
  ///
  /// <param name="newFinsNonFreezeCode2">New insured non-freeze status code.</param>
  void setFinsNonFreezeCode2( char newFinsNonFreezeCode2 ) 
  { finsNonFreezeCode2 = newFinsNonFreezeCode2; }
  /// <summary>Sets date fully insured status is attained.</summary>
  ///
  /// <param name="newFullInsDate">New date fully insured status is
  /// attained.</param>
  void setFullInsDate( const DateMoyr& newFullInsDate )
  { fullInsDate = newFullInsDate; }
  void setFullRetDate();
  /// <summary>Sets number of applicable pia calculation method.</summary>
  ///
  /// <param name="newIappn">New number of applicable pia method.</param>
  void setIappn( int newIappn ) { iappn = newIappn; }
  /// <summary>Sets support pia method number.</summary>
  ///
  /// <param name="newIapps">New support pia method number.</param>
  void setIapps( int newIapps ) { iapps = newIapps; }
  void setIbeginTotal( const WorkerDataGeneral& workerData );
  void setIendTotal( const WorkerDataGeneral& workerData );
  /// <summary>Sets number of months of increment or reduction.</summary>
  ///
  /// <param name="newMonthsArdri">New number of months.</param>
  void setMonthsArdri( int newMonthsArdri ) { monthsArdri = newMonthsArdri; }
  /// <summary>Sets indicator for earnings over maximum.</summary>
  ///
  /// <param name="newOverMax">New indicator.</param>
  void setOverMax( bool newOverMax ) { overMax = newOverMax; }
  /// <summary>Sets code for type of pia.</summary>
  ///
  /// <param name="newPifc">New pia factor code.</param>
  void setPifc( char newPifc ) { pifc = newPifc; }
  void setQc3( WorkerDataGeneral& workerData );
  /// <summary>Sets military service quarters of coverage, 1937-50.</summary>
  ///
  /// <param name="newQc3750ms">New value of military service qcs, 1937-50.
  /// </param>
  void setQc3750ms( int newQc3750ms ) { qc3750ms = newQc3750ms; }
  /// <summary>Sets simplified quarters of coverage, 1937-50.</summary>
  ///
  /// <param name="newQc3750simp">New value of simplified qcs, 1937-50.
  /// </param>
  void setQc3750simp( int newQc3750simp ) { qc3750simp = newQc3750simp; }
  /// <summary>Sets quarters of coverage earned in prior 3 years.</summary>
  ///
  /// <param name="newQcCurrent">New quarters of coverage earned in prior 3
  /// years.</param>
  void setQcCurrent( int newQcCurrent ) { qcCurrent = newQcCurrent; }
  /// <summary>Sets quarters of coverage earned in prior 3 years for non-freeze.</summary>
  ///
  /// <param name="newQcCurrentNonFreeze">New quarters of coverage earned in prior 3
  /// years for non-freeze.</param>
  void setQcCurrentNonFreeze( int newQcCurrentNonFreeze ) 
  { qcCurrentNonFreeze = newQcCurrentNonFreeze; }
  /// <summary>Sets number of quarters required for fully insured status.
  /// </summary>
  ///
  /// <param name="newQcReq">New required number of quarters.</param>
  void setQcReq( int newQcReq ) { qcReq = newQcReq; }
  /// <summary>Sets number of quarters required for fully insured non-freeze status.
  /// </summary>
  ///
  /// <param name="newQcReqNonFreeze">New required number of quarters.</param>
  void setQcReqNonFreeze( int newQcReqNonFreeze ) 
  { qcReqNonFreeze = newQcReqNonFreeze; }
  /// <summary>Sets number of quarters required for permanently insured
  /// status.</summary>
  ///
  /// <param name="newQcReqPerm">New required number of quarters.</param>
  void setQcReqPerm( int newQcReqPerm ) { qcReqPerm = newQcReqPerm; }
  /// <summary>Sets number of quarters required for permanently insured
  /// non-freeze status.</summary>
  ///
  /// <param name="newQcReqPermNonFreeze">New required number of quarters.</param>
  void setQcReqPermNonFreeze( int newQcReqPermNonFreeze ) 
  { qcReqPermNonFreeze = newQcReqPermNonFreeze; }
  /// <summary>Sets total quarters of coverage.</summary>
  ///
  /// <param name="newQcTotal">New total quarters of coverage.</param>
  void setQcTotal( int newQcTotal ) { qcTotal = newQcTotal; }
  /// <summary>Sets total quarters of coverage for non-freeze.</summary>
  ///
  /// <param name="newQcTotalNonFreeze">New total quarters of coverage for non-freeze.</param>
  void setQcTotalNonFreeze( int newQcTotalNonFreeze ) 
  { qcTotalNonFreeze = newQcTotalNonFreeze; }
  /// <summary>Sets quarters of coverage prior to 1951.</summary>
  ///
  /// <param name="newQcTotal50">New quarters of coverage prior to 1951.
  /// </param>
  void setQcTotal50( int newQcTotal50 ) { qcTotal50 = newQcTotal50; }
  /// <summary>Sets total quarters of coverage earned for a totalization
  /// benefit computation.</summary>
  ///
  /// <param name="newQcTotalRel">New total quarters of coverage earned for a
  /// totalization benefit computation.</param>
  void setQcTotalRel( int newQcTotalRel ) { qcTotalRel = newQcTotalRel; }
  /// <summary>Sets average relative earnings position.</summary>
  ///
  /// <param name="newRelEarnPosition">New average relative earnings
  /// position.</param>
  void setRelEarnPositionAverage( double newRelEarnPosition )
  { relEarnPositionAverage = newRelEarnPosition; }
  void updateMilServ( const WorkerDataGeneral& workerData );
};
