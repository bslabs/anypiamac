// Declarations for the <see cref="PiaParams"/> class - parent of classes to
// manage the Social Security parameters required to calculate a Social
// Security benefit.

// $Id: piaparms.h 1.105 2017/10/12 12:48:33EDT 277133 Development  $

#pragma once

#include <vector>
#include "datemoyr.h"
#include "dbleann.h"
#include "intann.h"
#include "age.h"
#include "qcamt.h"
#include "avgwg.h"
#include "bpmfbout.h"
#include "bppiaout.h"
#include "PercPiaOut.h"
#include "BitAnnual.h"
#include "qc.h"
#include "WageBase.h"
#include "Sex.h"
#include "cachup.h"
#include "awinc.h"
#include "qtryear.h"
#include "BendPoints.h"
#include "BaseChangeType.h"
#include "boost/date_time/gregorian/greg_date.hpp"
class AwbiData;

/// <summary>Abstract parent of classes to manage the Social Security
/// parameters required to calculate a Social Security benefit.</summary>
///
/// <remarks>You should instantiate a sub-class, or a sub-sub-class.
/// <see cref="PiaParamsLC"/> is used in the microsimulation work;
/// <see cref="PiaParamsAny"/> is used in the Anypia program;
/// <see cref="PiaParamsPL"/> is also available.</remarks>
///
/// <seealso cref="PiaParamsLC"/>
/// <seealso cref="PiaParamsAny"/>
/// <seealso cref="PiaParamsPL"/>
class PiaParams
{
public:
  /// <summary>The year range for the special minimum percentage.</summary>
  enum SPECMINPERCTYPE {
    PRE1991 = 0,         // Special minimum before 1991.
    POST1990,            // Special minimum after 1990.
    NUMSPECMINPERCTYPES  // Number of year ranges for the spec min percentage.
  };
  /// <summary>The date of 1950 amendments.</summary>
  static const DateMoyr amend50;
  /// <summary>The date of 1952 amendments.</summary>
  static const DateMoyr amend52;
  /// <summary>The date of 1954 amendments.</summary>
  static const DateMoyr amend54;
  /// <summary>The date of 1956 amendments.</summary>
  static const DateMoyr amend56;
  /// <summary>First birth date for which age 62 is effective for females.
  /// </summary>
  static const boost::gregorian::date amend561;
  /// <summary>Second birth date for which age 62 is effective for females.
  /// </summary>
  static const boost::gregorian::date amend562;
  /// <summary>The date of 1958 amendments.</summary>
  static const DateMoyr amend58;
  /// <summary>The date of 1961 amendments.</summary>
  static const DateMoyr amend61;
  /// <summary>First birth date for which age 62 is effective for males.
  /// </summary>
  static const boost::gregorian::date amend611;
  /// <summary>Second birth date for which age 62 is effective for males.
  /// </summary>
  static const boost::gregorian::date amend612;
  /// <summary>The date of 1965 amendments for benefit increase.</summary>
  static const DateMoyr amend651;
  /// <summary>The date of 1965 amendments for change to age 60.</summary>
  static const DateMoyr amend652;
  /// <summary>The date of 1965 amendments for old start.</summary>
  static const DateMoyr amend653;
  /// <summary>The date of 1967 amendments for old start.</summary>
  static const DateMoyr amend671;
  /// <summary>The date of 1967 amendments for benefit increase.</summary>
  static const DateMoyr amend672;
  /// <summary>The date of 1969 amendments.</summary>
  static const DateMoyr amend69;
  /// <summary>The date of 1970 amendments.</summary>
  static const DateMoyr amend70;
  /// <summary>The date of 1972 amendments for benefit increase.</summary>
  static const DateMoyr amend721;
  /// <summary>The date of 1972 amendments for special minimum.</summary>
  static const DateMoyr amend722;
  /// <summary>The date of 1974 amendments for partial increase.</summary>
  static const DateMoyr amend741;
  /// <summary>The date of 1974 amendments for full increase.</summary>
  static const DateMoyr amend742;
  /// <summary>The date of 1977 amendments for frozen pia table.</summary>
  static const DateMoyr amend771;
  /// <summary>The date of 1977 amendments for start of wage indexing.
  /// </summary>
  static const DateMoyr amend772;
  /// <summary>The date of 1980 amendments.</summary>
  static const DateMoyr amend80;
  /// <summary>The date of birth for age 62 and 1 month in 1981 amendments.
  /// </summary>
  static const boost::gregorian::date amend811;
  /// <summary>The date of 1981 amendments (month, day, and year).</summary>
  static const boost::gregorian::date amend812;
  /// <summary>The date of 1981 amendments (month and year).</summary>
  static const DateMoyr amend813;
  /// <summary>The date of 1982 amendments.</summary>
  static const DateMoyr amend82;
  /// <summary>The date of 1983 amendments.</summary>
  static const DateMoyr amend83;
  /// <summary>The date of 1988 amendments.</summary>
  static const DateMoyr amend88;
  /// <summary>The date of 1990 amendments.</summary>
  static const DateMoyr amend90;
  /// <summary>The date of special 0.1 percent benefit increase.</summary>
  static const DateMoyr amend01;
  /// <summary>Amount per old start increment year.</summary>
  static const double amtperincyr;
  /// <summary><see cref="Age"/> 16.</summary>
  static const Age age16;
  /// <summary><see cref="Age"/> 18.</summary>
  static const Age age18;
  /// <summary><see cref="Age"/> 19.</summary>
  static const Age age19;
  /// <summary><see cref="Age"/> 22.</summary>
  static const Age age22;
  /// <summary><see cref="Age"/> 50.</summary>
  static const Age age50;
  /// <summary><see cref="Age"/> 60.</summary>
  static const Age age60;
  /// <summary><see cref="Age"/> 62.</summary>
  static const Age age62;
  /// <summary><see cref="Age"/> 62 and 1 month.</summary>
  static const Age age621;
  /// <summary><see cref="Age"/> 65.</summary>
  static const Age age65;
  /// <summary><see cref="Age"/> 65 and 2 months.</summary>
  static const Age age65_2;
  /// <summary><see cref="Age"/> 65 and 4 months.</summary>
  static const Age age65_4;
  /// <summary><see cref="Age"/> 65 and 6 months.</summary>
  static const Age age65_6;
  /// <summary><see cref="Age"/> 65 and 8 months.</summary>
  static const Age age65_8;
  /// <summary><see cref="Age"/> 65 and 10 months.</summary>
  static const Age age65_10;
  /// <summary><see cref="Age"/> 66.</summary>
  static const Age age66;
  /// <summary><see cref="Age"/> 66 and 2 months.</summary>
  static const Age age66_2;
  /// <summary><see cref="Age"/> 66 and 4 months.</summary>
  static const Age age66_4;
  /// <summary><see cref="Age"/> 66 and 6 months.</summary>
  static const Age age66_6;
  /// <summary><see cref="Age"/> 66 and 8 months.</summary>
  static const Age age66_8;
  /// <summary><see cref="Age"/> 66 and 10 months.</summary>
  static const Age age66_10;
  /// <summary><see cref="Age"/> 67.</summary>
  static const Age age67;
  /// <summary><see cref="Age"/> 70.</summary>
  static const Age age70;
  /// <summary><see cref="Age"/> 72.</summary>
  static const Age age72;
  /// <summary><see cref="Age"/> 75.</summary>
  static const Age age75;
  /// <summary><see cref="Age"/> 199 (impossible age).</summary>
  static const Age age199;
  /// <summary>Temporary benefit increase, March-May 1974.</summary>
  static const double beninc74;
  /// <summary>Factor for calculating life benefits.</summary>
  static const double factor50;
  /// <summary>Factor for calculating survivor benefits.</summary>
  static const double factor75;
  /// <summary>Factor for calculating widow(er) benefits.</summary>
  static const double factor825;
  /// <summary>Factor for calculating mfbs.</summary>
  static const double factor150;
  /// <summary>Factor for calculating mfbs.</summary>
  static const double factor175;
  /// <summary>First quarter for which a quarter of coverage could be earned.
  /// </summary>
  static const QtrYear year37qtr1;
  /// <summary>Starting point of deemed insured.</summary>
  static const QtrYear qtr184;
  /// <summary>Year of extra 0.1 percent benefit increase.</summary>
  static const int YEAR1999 = 1999;
  /// <summary>Maximum number of months of actuarial reduction between ages 62
  /// and 65.</summary>
  static const int maxMonthsAr62_65;
  /// <summary>Maximum number of months of actuarial reduction between ages 62
  /// and 67.</summary>
  static const int maxMonthsAr62_67;
  /// <summary>Maximum number of months of actuarial reduction between ages 65
  /// and 67.</summary>
  static const int maxMonthsAr65_67;
  /// <summary>Monthly actuarial reduction factor for retired workers between
  /// ages 62 and 65.</summary>
  static const double arMonthlyFactorOab62_65;
  /// <summary>Monthly actuarial reduction factor for aged spouses between
  /// ages 62 and 65.</summary>
  static const double arMonthlyFactorSpouse62_65;
  /// <summary>Monthly actuarial reduction factor between ages 65 and 67.
  /// </summary>
  static const double arMonthlyFactor65_67;
  /// <summary>Total actuarial reduction factor for aged widow(er)s.</summary>
  static const double arFactorWid285;
  /// <summary>Array of amounts required for quarter of coverage.</summary>
  ///
  /// <remarks>The years go from 1937 to <see cref="maxyear"/>.</remarks>
  Qcamt qcamt;
  /// <summary>Table of mfb formula bend points.</summary>
  BpMfbOut bpMfbOut;
  /// <summary>Table of pia formula bend points.</summary>
  BpPiaOut bpPiaOut;
  /// <summary>Table of pia formula percentages.</summary>
  PercPiaOut percPiaOut;
protected:
  /// <summary>HI wage bases.</summary>
  WageBaseHI baseHi;
  /// <summary>Array of month of benefit increase.</summary>
  ///
  /// <remarks>The years go from 1951 to <see cref="maxyear"/>.</remarks>
  IntAnnual monthBeninc;
  /// <summary>Array of indicators for no change allowed.</summary>
  ///
  /// <remarks>The years go from 1937 to <see cref="maxyear"/>.</remarks>
  IntAnnual noChange;
  /// <summary>Percentage of old-law wage base required for year of coverage.
  /// </summary>
  ///
  /// <remarks>The years go from 1937 to <see cref="maxyear"/>.</remarks>
  DoubleAnnual percSpecMin;
  /// <summary>Bend points for old-start PIB formula.</summary>
  static const int bendOS[3];
  /// <summary>Old-start PIB formula percentages.</summary>
  static const double percOS[2];
  /// <summary>Percentage of old-law wage base required for year of coverage
  /// for special minimum.</summary>
  ///
  /// <remarks>There are two values in this array. The first is for before
  /// 1991. The second is for after 1990.</remarks>
  static const double specMinPerc[NUMSPECMINPERCTYPES];
  /// <summary>Amount required for year of coverage for special minimum.
  /// </summary>
  DoubleAnnual yocAmountSpecMin;
  /// <summary>Amount required for year of coverage for windfall elimination
  /// provision.</summary>
  DoubleAnnual yocAmountWindfall;
  /// <summary>Array of benefit increases.</summary>
  ///
  /// <remarks>The years go from 1951 to <see cref="maxyear"/>.</remarks>
  DoubleAnnual cpiinc;
  /// <summary>Catch-up benefit increases.</summary>
  Catchup catchup;
  /// <summary>Array of annual average earnings.</summary>
  ///
  /// <remarks>The years go from 1937 to <see cref="maxyear"/>.</remarks>
  AverageWage fq;
  /// <summary>Array of percentage increases in annual average earnings.
  /// </summary>
  ///
  /// <remarks>The years go from 1937 to <see cref="maxyear"/>.</remarks>
  Awinc fqinc;
  /// <summary>Current year (year after last known benefit increase).</summary>
  int istart;
  /// <summary>Maximum year allowed.</summary>
  int maxyear;
  /// <summary>Special minimum pias.</summary>
  std::vector< DoubleAnnual* > specMinPia;
  /// <summary>Special minimum pias for August-November 2001.</summary>
  std::vector< double > specMinPia2001;
  /// <summary>Special minimum mfbs.</summary>
  std::vector< DoubleAnnual* > specMinMfb;
  /// <summary>Special minimum mfbs for August-November 2001.</summary>
  std::vector< double > specMinMfb2001;
  /// <summary>Indicator for which year(s) the PIA should be recalculated
  /// due to a newly effective law change.</summary>
  BitAnnual recalcInd;
  /// <summary>Title of average wage assumptions.</summary>
  std::string titleAw;
  /// <summary>Title of benefit increase assumptions.</summary>
  std::string titleBi;
public:
  PiaParams( int newIstart, int newMaxyear );
  explicit PiaParams( int newMaxyear );
  virtual ~PiaParams();
  virtual double applyCola( double pia, int year ) const;
  virtual double applyCola( double pia, int year, int eligYear ) const;
  double applyCola99( double pia ) const;
  virtual double applyColaMfb( double mfb, int year, double pia ) const;
  virtual double applyColaMfb( double mfb, int year, int eligYear,
    double pia ) const;
  double applyColaMfb99( double mfb, double pia ) const;
  double benincCatchUp( double bcatch, int year, int eligYear ) const;
  /// <summary>Returns true if should recalculate PIAs this year because of
  /// a newly effective law change, else false.</summary>
  ///
  /// <returns>True if should recalculate PIAs this year because of a newly
  /// effective law change, else false.</returns>
  ///
  /// <param name="year">Year to check if a recalc is needed.</param>
  bool doRecalc( int year ) const { return recalcInd.getBit(year); }
  double deconvertPia( int eligYear, int number, double pia77,
    const DateMoyr& benDate) const;
  /// <summary>Returns earliest possible retirement age for oab or aged
  /// spouse.</summary>
  ///
  /// <returns>Earliest possible retirement age for oab or aged spouse.
  /// </returns>
  virtual Age earlyAgeOabCal( Sex::sex_type,
    const boost::gregorian::date& ) const = 0;
  /// <summary>Returns factor for widow(er) benefit before age reduction.
  /// </summary>
  ///
  /// <returns>Factor for widow(er) benefit before age reduction.</returns>
  virtual double factorAgedWidCal( int, const Age&, int,
    const DateMoyr& ) const = 0;
  /// <summary>Returns factor for wife/husband benefit before age reduction.
  /// </summary>
  ///
  /// <returns>Factor for wife/husband benefit before age reduction.</returns>
  virtual double factorAgedSpouseCal( int, int ) const = 0;
  /// <summary>Returns reduction factor for wife/husband.</summary>
  ///
  /// <returns>Reduction factor for wife/husband.</returns>
  virtual double factorArAgedSpouseCal( int ) const = 0;
  /// <summary>Returns reduction factor for OAB.</summary>
  ///
  /// <returns>Reduction factor for OAB.</returns>
  virtual double factorArCal( int ) const = 0;
  virtual double factorArWidCal( int monthsArdri, const Age& age,
    const DateMoyr& benefitDate, const Age& fullRetAge ) const;
  /// <summary>Returns factor for disabled widow(er) benefit before age
  /// reduction.</summary>
  ///
  /// <returns>Factor for disabled widow(er) benefit before age reduction.
  /// </returns>
  virtual double factorDisWidCal( int, const DateMoyr& ) const = 0;
  virtual double factorYngLifeCal() const;
  virtual double factorYngSurvCal() const;
  /// <summary>Returns full retirement age.</summary>
  ///
  /// <returns>Full retirement age.</returns>
  virtual Age fullRetAgeCal( int ) const = 0;
  /// <summary>Returns full retirement age for benefit calculations involving
  /// disability freeze.</summary>
  ///
  /// <returns>Full retirement age for benefit calculations involving
  /// disability freeze.</returns>
  virtual Age fullRetAgeCalDI( int, int ) const = 0;
  /// <summary>Returns title of average wage assumptions.</summary>
  ///
  /// <returns>Title of average wage assumptions.</returns>
  const std::string& getAverageWage() const { return titleAw; }
  /// <summary>Returns the array of OASDI wage bases.</summary>
  ///
  /// <returns>The array of OASDI wage bases.</returns>
  virtual const WageBase& getBaseOasdiArray() const = 0;
  /// <summary>Returns one OASDI wage base.</summary>
  ///
  /// <returns>One OASDI wage base.</returns>
  virtual double getBaseOasdi( int ) const = 0;
  /// <summary>Returns the array of old-law wage bases.</summary>
  ///
  /// <returns>The array of old-law wage bases.</returns>
  virtual const WageBaseOldLaw& getBase77Array() const = 0;
  /// <summary>Returns one old-law wage base.</summary>
  ///
  /// <returns>One old-law wage base.</returns>
  virtual double getBase77( int ) const = 0;
  /// <summary>Returns the array of HI wage bases.</summary>
  ///
  /// <returns>The array of HI wage bases.</returns>
  const WageBaseHI& getBaseHiArray() const { return baseHi; }
  /// <summary>Returns one HI wage base.</summary>
  ///
  /// <returns>One HI wage base.</returns>
  ///
  /// <param name="year">Year for which base is desired.</param>
  double getBaseHi( int year ) const { return baseHi[year]; }
  /// <summary>Returns title of benefit increase assumptions.</summary>
  ///
  /// <returns>Title of benefit increase assumptions.</returns>
  const std::string& getBenefitInc() const { return titleBi; }
  /// <summary>Returns catch-up benefit increases.</summary>
  ///
  /// <returns>Catch-up benefit increases.</returns>
  const Catchup& getCatchup() const { return catchup; }
  /// <summary>Returns maximum amount of earnings in a childcare dropout year.
  /// </summary>
  ///
  /// <returns>Maximum amount of earnings in a childcare dropout year.
  /// </returns>
  virtual double getChildcareDropoutAmount( int, int ) const = 0;
  /// <summary>Returns the amount of the cap on the COLA for the given year.
  /// </summary>
  ///
  /// <returns>The amount of the cap on the COLA for the given year.</returns>
  virtual double getColaCap( int ) const = 0;
  /// <summary>Returns one benefit increase.</summary>
  ///
  /// <returns>One benefit increase.</returns>
  ///
  /// <param name="year">Year for which benefit increase is desired.</param>
  virtual double getCpiinc( int year ) const { return(cpiinc[year]); }
  /// <summary>Returns the array of benefit increases.</summary>
  ///
  /// <returns>The array of benefit increases.</returns>
  virtual const DoubleAnnual& getCpiincArray() const { return(cpiinc); }
  /// <summary>Returns array of average wages for indexing.</summary>
  ///
  /// <returns>Array of average wages for indexing.</returns>
  const AverageWage& getFqArray() const { return(fq); }
  /// <summary>Returns one average wage for indexing.</summary>
  ///
  /// <returns>One average wage for indexing.</returns>
  ///
  /// <param name="year">Year for which base is desired.</param>
  double getFq( int year ) const { return(fq[year]); }
  /// <summary>Returns series used for indexing pia formula bend points.
  /// </summary>
  ///
  /// <returns>Series used for indexing pia formula bend points.</returns>
  virtual const AverageWage& getFqBppia() const = 0;
  /// <summary>Returns one increase in average wage for indexing.</summary>
  ///
  /// <returns>Increase in average wage for indexing.</returns>
  ///
  /// <param name="year">Year for which average wage increase is desired.
  /// </param>
  double getFqinc( int year ) const { return(fqinc[year]); }
  /// <summary>Returns array of increases in average wage for indexing.
  /// </summary>
  ///
  /// <returns>Array of increases in average wage for indexing.</returns>
  const Awinc& getFqincArray() const { return(fqinc); }
  /// <summary>Returns series used for indexing an earnings record.</summary>
  ///
  /// <returns>Series used for indexing an earnings record.</returns>
  virtual const AverageWage& getFqIndex() const = 0;
  /// <summary>Returns current year.</summary>
  ///
  /// <returns>Current year.</returns>
  int getIstart() const { return(istart); }
  /// <summary>Returns the number of years of marraige needed to be eligible
  /// for a divroced spouse benefit.</summary>
  ///
  /// <returns>Number of years of marriage needed to be eligible for a
  /// divorced spouse benefit.</returns>
  virtual int getMarrLengthForDivBen( const DateMoyr& ) const = 0;
  /// <summary>Returns maximum age of child for childcare dropout year.
  /// </summary>
  ///
  /// <returns>Maximum age of child for childcare dropout year.</returns>
  virtual int getMaxChildcareAge() const = 0;
  /// <summary>Returns maximum number of childcare dropout years.</summary>
  ///
  /// <returns>Maximum number of childcare dropout years.</returns>
  virtual int getMaxChildcareDropoutYears( int, int ) const = 0;
  /// <summary>Returns maximum possible year.</summary>
  ///
  /// <returns>Maximum possible year.</returns>
  int getMaxyear() const { return(maxyear); }
  /// <summary>Returns the amount of the cap on the COLA for the MFB.
  /// </summary>
  ///
  /// <returns>The amount of the cap on the COLA for the MFB.</returns>
  virtual double getMfbColaCap( int ) const = 0;
  /// <summary>Returns array of months of benefit increase.</summary>
  ///
  /// <returns>Array of months of benefit increase.</returns>
  const IntAnnual& getMonthBenincArray() const { return(monthBeninc); }
  /// <summary>Returns one month of benefit increase.</summary>
  ///
  /// <returns>Month of benefit increase for specified year.</returns>
  ///
  /// <param name="year"> Year of desired month of benefit increase.</param>
  int getMonthBeninc( int year ) const { return(monthBeninc[year]); }
  /// <summary>Returns array of percents of old-law base required for year
  /// of coverage.</summary>
  ///
  /// <returns>Array of percents of old-law base required for year of
  /// coverage.</returns>
  const DoubleAnnual& getPercSpecMinArray() const { return(percSpecMin); }
  /// <summary>Returns percent of old-law base required for year of
  /// coverage.</summary>
  ///
  /// <returns>Percent of old-law base required for year of coverage.
  /// </returns>
  ///
  /// <param name="year">The year for which the percent is required.</param>
  double getPercSpecMin( int year ) const { return(percSpecMin[year]); }
  virtual double getSpecMinPia( const DateMoyr& dateMoyr, int yoc ) const;
  virtual double getSpecMinMfb( const DateMoyr& dateMoyr, int yoc ) const;
  /// <summary>Returns array of amounts required for year of coverage for
  /// special minimum.</summary>
  ///
  /// <returns>Array of amounts required for year of coverage for special
  /// minimum.</returns>
  const DoubleAnnual& getYocAmountSpecMinArray() const
  { return(yocAmountSpecMin); }
  /// <summary>Returns amount required for year of coverage for special
  /// minimum.</summary>
  ///
  /// <returns>Amount required for year of coverage for special minimum.
  /// </returns>
  ///
  /// <param name="year">Year for which amount is required.</param>
  double getYocAmountSpecMin( int year ) const
  { return(yocAmountSpecMin[year]); }
  /// <summary>Returns array of amounts required for year of coverage for
  /// windfall elimination.</summary>
  ///
  /// <returns>Array of amounts required for year of coverage for windfall
  /// elimination.</returns>
  const DoubleAnnual& getYocAmountWindfallArray() const
  { return(yocAmountWindfall); }
  virtual void initdata();
  /// <summary>Checks for applicability of special 1999 increase (with
  /// extra 0.1 percent).</summary>
  ///
  /// <remarks>The year of benefit increase must be 1999, 2001 must be a
  /// historical year, and the benefit is for August 2001 or later.</remarks>
  ///
  /// <returns>True if special 1999 increase should be used.</returns>
  ///
  /// <param name="year">Year of benefit increase.</param>
  /// <param name="dateMoyr">The date up to which to apply benefit increases.
  /// </param>
  bool isApplicableCola99( int year, const DateMoyr& dateMoyr ) const
  { return ((year == 1999) && (istart > (int)amend01.getYear()) &&
    !(dateMoyr < amend01)); };
  long lastAmw( int year ) const;
  /// <summary>Returns maximum DI beneficiary age at end of year.</summary>
  ///
  /// <returns>Maximum DI beneficiary age at end of year.</returns>
  virtual Age maxDibAge( int ) const = 0;
  /// <summary>Returns true if a cola cap is required at any time.</summary>
  ///
  /// <returns>True if a cola cap is required.</returns>
  virtual bool needColaCap() const = 0;
  /// <summary>Returns true if a cola cap is required for a specified year.
  /// </summary>
  ///
  /// <returns>True if a cola cap is required.</returns>
  ///
  /// <param name="year">Year being capped.</param>
  virtual bool needColaCap( int year ) const = 0;
  void percPiaCal( int eligYear, PercPia& percPia ) const;
  /// <summary>Projects amounts dependent on benefit increases.</summary>
  virtual void projectCpiinc() = 0;
  virtual void projectFq();
  virtual void projectPerc();
  virtual void projectSpecMin( int baseYear, int lastYear );
  /// <summary>Returns number of quarters of coverage for one year,
  /// applying annual test.</summary>
  ///
  /// <returns>Number of quarters of coverage for one year.</returns>
  ///
  /// <param name="earnings">Annual earnings.</param>
  /// <param name="year">Year of earnings.</param>
  int qcCal( double earnings, int year ) const
  { return Qc::qcCal(earnings, qcamt[year]); }
  virtual void resizeSpecMin();
  /// <summary>Returns widow(er) benefit reduced for RIB-LIM.</summary>
  ///
  /// <returns>Reduced widow(er) benefit.</returns>
  virtual double ribLimCal( double, double, double,
    const DateMoyr& ) const = 0;
  /// <summary>Sets the amount of the cap on the COLA for the given year.
  /// </summary>
  virtual void setColaCap( double, int ) = 0;
  virtual void setData( AwbiData& awbiData );
  void setHistFqinc();
  void setIstart( int newIstart );
  /// <summary>Sets the amount of the cap on the COLA for the MFB
  /// for the given year.</summary>
  virtual void setMfbColaCap( double, int ) = 0;
  void setNoChange( int firstYear );
  /// <summary>Returns maximum number of usable years for special minimum that
  /// could ever be for any year of eligibility and year of benefit.</summary>
  ///
  /// <returns>Maximum number of usable years for special minimum.</returns>
  virtual int specMinMaxYears() const = 0;
  /// <summary>Returns maximum number of usable years for special minimum for
  /// a specified year of eligibility and year of benefit.</summary>
  ///
  /// <returns>Maximum number of usable years for special minimum.</returns>
  virtual int specMinMaxYears( int, int ) const = 0;
  /// <summary>Sets amount per year of coverage in special minimum.</summary>
  ///
  /// <returns>Amount per year of coverage in special minimum.</returns>
  virtual double specMinAmountCal( const DateMoyr& ) const = 0;
  double unApplyCola( double pia, int year ) const;
  double unApplyCola( double pia, int year, int eligYear ) const;
  double unApplyCola99( double pia ) const;
  double unbiCatchUp( int year, double bcatch, int eligYear ) const;
  /// <summary>Updates wage bases for average wage changes.</summary>
  virtual void updateBases() = 0;
  /// <summary>Updates wage bases for user entries and/or for average wage
  /// changes.</summary>
  virtual void updateBases( const DoubleAnnual&, const DoubleAnnual&,
    BaseChangeType::change_type, int, int ) = 0;
  void updateCpiinc( const DoubleAnnual& userBiproj,
    const std::string& userTitle, int firstYeart, int lastYeart );
  void updateCpiinc( const DoubleAnnual& userBiproj,
    const Catchup& userCatchup, const std::string& userTitle,
    int firstYeart, int lastYeart );
  void updateFq( const DoubleAnnual& userfq, int firstYeart, int lastYeart );
  void updateFqinc( const DoubleAnnual& userFqinc,
    const std::string& userTitle, int firstYeart, int lastYeart );
  void updateYocAmountSpecMin();
  static boost::gregorian::date birthDateFRA( int year );
  static Age childAgeForMFCal( const DateMoyr& benefitDate,
    bool isDisabled );
  static int deemedQcReqCal( int year );
  static Age earlyAgeDisWidCal( const DateMoyr& benefitDate );
  static Age earlyAgeOabCalPL( Sex::sex_type sex,
    const boost::gregorian::date& kbirth );
  static Age earlyAgeWidCal( const DateMoyr& benefitDate );
  /// <summary>Returns factor for wife/husband benefit before age
  /// reduction.</summary>
  ///
  /// <returns>0.5 for present law.</returns>
  static double factorAgedSpouseCalPL() { return(0.5); }
  static double factorAgedWidCalPL( int monthsArdri, const Age& age,
    const DateMoyr& benefitDate );
  static double factorArAgedSpouseCalPL( int monthsArdri );
  static double factorArCalPL( int monthsArdri );
  static double factorArDisWidCal( int monthsArdri,
    const DateMoyr& entDate );
  static double factorDisWidCalPL( const DateMoyr& benefitDate );
  static double factorDriCal( int monthsArdri, int eligYear );
  static Age fullRetAgeCalDIPL( int eligYear, int currentYear );
  static Age fullRetAgeCalPL( int eligYear );
  /// <summary>Returns a bend point for old-start PIB formula.</summary>
  ///
  /// <returns>A bend point for old-start PIB formula.</returns>
  ///
  /// <param name="number">Number of bend point (0-2).</param>
  static int getBendOS( int number ) { return(bendOS[number]); }
  /// <summary>Returns maximum amount of earnings in a childcare dropout
  /// year.</summary>
  ///
  /// <returns>0 for present law.</returns>
  static double getChildcareDropoutAmountPL() { return 0.0; }
  /// <summary>Returns the number of years of marriage needed to be eligible
  /// for a divorced spouse benefit.</summary>
  ///
  /// <returns>The number of years of marriage needed to be eligible for a
  /// divorced spouse benefit.</returns>
  ///
  /// <param name="dateMoyr">The date (year and month) when checking
  /// if eligible for a divorced spouse benefit.</param>
  static int getMarrLengthForDivBenPL( const DateMoyr& dateMoyr )
  { return (dateMoyr < amend772) ? 20 : 10; }
  /// <summary>Returns maximum age of child for childcare dropout year.
  /// </summary>
  ///
  /// <returns>3 for present law.</returns>
  static int getMaxChildcareAgePL() { return 3; }
  /// <summary>Returns maximum number of childcare dropout years.</summary>
  ///
  /// <returns>3 for present law.</returns>
  static int getMaxChildcareDropoutYearsPL() { return 3; }
  /// <summary>Returns old-start PIB formula percentage.</summary>
  ///
  /// <returns>Old-start PIB formula percentage.</returns>
  ///
  /// <param name="number">Number of percentage (0-1).</param>
  static double getPercOS( int number ) { return(percOS[number]); }
  /// <summary>Returns percentage of old-law base required for year of
  /// coverage.</summary>
  ///
  /// <returns>Percentage of old-law base required for year of coverage.
  /// </returns>
  ///
  /// <param name="number">Type of year of coverage.</param>
  static double getSpecMinPerc( SPECMINPERCTYPE number )
  { return(specMinPerc[number]); }
  static double lumpSum();
  static Age maxChildAge( const DateMoyr& benefitDate, bool isDisabled,
    bool isStudent );
  static Age maxDibAgePL( int year );
  static int monthsArCal( const Age& age, const Age& fullRetAge );
  static int monthsArDICal( const DateMoyr& oabEntDate,
    const DateMoyr& oabCessDate );
  static int monthsArDisWidCal( const Age& age,
    const DateMoyr& benefitDate, const Age& fullRetAge );
  static int monthsArWidCal( const Age& age, const DateMoyr& benefitDate,
    const Age& fullRetAge );
  static int monthsArAgedSpouseCal( const Age& age,
    const DateMoyr& benefitDate, const Age& fullRetAge );
  static int monthsDriCal( const DateMoyr& fullRetDate, int eligYear,
    const boost::gregorian::date& dobadj, const DateMoyr& entDate,
    const DateMoyr& benefitDate, const DateMoyr& fullInsDate );
  static double retCredit( int eligYear );
  static double ribLimCalPL( double widowBen, double oabPia, double oabBen,
    const DateMoyr& benefitDate );
  static double specMinAmountCalPL( const DateMoyr& date );
  /// <summary>Returns maximum number of usable years for special minimum.
  /// </summary>
  ///
  /// <returns>20 for present law.</returns>
  static int specMinMaxYearsPL() { return(20); }
};
