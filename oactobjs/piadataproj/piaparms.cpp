// Functions for the <see cref="PiaParams"/> class - parent of classes to
// manage the Social Security parameters required to calculate a Social
// Security benefit.

// $Id: piaparms.cpp 1.129 2017/10/12 13:06:33EDT 277133 Development  $

#include <utility>  // for rel_ops
#include "piaparms.h"
#include "wbgenrl.h"
#include "baseyear.h"
#include "bpmfbout.h"
#include "oactcnst.h"
#include "Resource.h"
#include "PiaException.h"
#include "WageBase.h"
#include "BenefitAmount.h"
#include "awbidat.h"
#include "BaseYearNonFile.h"

using namespace std;
#if !defined(__SGI_STL_INTERNAL_RELOPS)
using namespace std::rel_ops;
#endif

/// <summary>The date of 1950 amendments.</summary>
const DateMoyr PiaParams::amend50 = DateMoyr(9, 1950);
/// <summary>The date of 1952 amendments.</summary>
const DateMoyr PiaParams::amend52 = DateMoyr(9, 1952);
/// <summary>The date of 1954 amendments.</summary>
const DateMoyr PiaParams::amend54 = DateMoyr(9, 1954);
/// <summary>The date of 1956 amendments.</summary>
const DateMoyr PiaParams::amend56 = DateMoyr(11, 1956);
/// <summary>First birth date for which age 62 is effective for females.
/// </summary>
const boost::gregorian::date PiaParams::amend561 =
  boost::gregorian::date(1894, 10, 31);
/// <summary>Second birth date for which age 62 is effective for females.
/// </summary>
const boost::gregorian::date PiaParams::amend562 =
  boost::gregorian::date(1891, 11, 30);
/// <summary>The date of 1958 amendments.</summary>
const DateMoyr PiaParams::amend58 = DateMoyr(1, 1959);
/// <summary>The date of 1961 amendments.</summary>
const DateMoyr PiaParams::amend61 = DateMoyr(8, 1961);
/// <summary>First birth date for which age 62 is effective for males.
/// </summary>
const boost::gregorian::date PiaParams::amend611 =
  boost::gregorian::date(1899, 7, 31);
/// <summary>Second birth date for which age 62 is effective for males.
/// </summary>
const boost::gregorian::date PiaParams::amend612 =
  boost::gregorian::date(1896, 8, 31);
/// <summary>The date of 1965 amendments for benefit increase.</summary>
const DateMoyr PiaParams::amend651 = DateMoyr(1, 1965);
/// <summary>The date of 1965 amendments for change to age 60.</summary>
const DateMoyr PiaParams::amend652 = DateMoyr(9, 1965);
/// <summary>The date of 1965 amendments for old start.</summary>
const DateMoyr PiaParams::amend653 = DateMoyr(1, 1966);
/// <summary>The date of 1967 amendments for old start.</summary>
const DateMoyr PiaParams::amend671 = DateMoyr(1, 1968);
/// <summary>The date of 1967 amendments for benefit increase.</summary>
const DateMoyr PiaParams::amend672 = DateMoyr(2, 1968);
/// <summary>The date of 1969 amendments.</summary>
const DateMoyr PiaParams::amend69 = DateMoyr(1, 1970);
/// <summary>The date of 1970 amendments.</summary>
const DateMoyr PiaParams::amend70 = DateMoyr(1, 1971);
/// <summary>The date of 1972 amendments for benefit increase.</summary>
const DateMoyr PiaParams::amend721 = DateMoyr(9, 1972);
/// <summary>The date of 1972 amendments for special minimum.</summary>
const DateMoyr PiaParams::amend722 = DateMoyr(1, 1973);
/// <summary>The date of 1974 amendments for partial increase.</summary>
const DateMoyr PiaParams::amend741 = DateMoyr(3, 1974);
/// <summary>The date of 1974 amendments for full increase.</summary>
const DateMoyr PiaParams::amend742 = DateMoyr(6, 1974);
/// <summary>The date of 1977 amendments for frozen pia table.</summary>
const DateMoyr PiaParams::amend771 = DateMoyr(12, 1978);
/// <summary>The date of 1977 amendments for start of wage indexing.</summary>
const DateMoyr PiaParams::amend772 = DateMoyr(1, 1979);
/// <summary>The date of 1980 amendments.</summary>
const DateMoyr PiaParams::amend80 = DateMoyr(7, 1980);
/// <summary>The date of birth for age 62 and 1 month in 1981 amendments.
/// </summary>
const boost::gregorian::date PiaParams::amend811 =
  boost::gregorian::date(1919, 8, 31);
/// <summary>The date of 1981 amendments (month, day, and year).</summary>
const boost::gregorian::date PiaParams::amend812 =
  boost::gregorian::date(1981, 9, 1);
/// <summary>The date of 1981 amendments (month and year).</summary>
const DateMoyr PiaParams::amend813 = DateMoyr(9, 1981);
/// <summary>The date of 1982 amendments.</summary>
const DateMoyr PiaParams::amend82 = DateMoyr(6, 1982);
/// <summary>The date of 1983 amendments.</summary>
const DateMoyr PiaParams::amend83 = DateMoyr(1, 1984);
/// <summary>The date of 1988 amendments.</summary>
const DateMoyr PiaParams::amend88 = DateMoyr(1, 1989);
/// <summary>The date of 1990 amendments.</summary>
const DateMoyr PiaParams::amend90 = DateMoyr(6, 1992);
/// <summary>The date of special 0.1 percent benefit increase.</summary>
const DateMoyr PiaParams::amend01 = DateMoyr(7, 2001);
/// <summary>Amount per old start increment year.</summary>
const double PiaParams::amtperincyr = 1650.0;
/// <summary><see cref="Age"/> 16.</summary>
const Age PiaParams::age16 = Age(16, 0);
/// <summary><see cref="Age"/> 18.</summary>
const Age PiaParams::age18 = Age(18, 0);
/// <summary><see cref="Age"/> 19.</summary>
const Age PiaParams::age19 = Age(19, 0);
/// <summary><see cref="Age"/> 22.</summary>
const Age PiaParams::age22 = Age(22, 0);
/// <summary><see cref="Age"/> 50.</summary>
const Age PiaParams::age50 = Age(50, 0);
/// <summary><see cref="Age"/> 60.</summary>
const Age PiaParams::age60 = Age(60, 0);
/// <summary><see cref="Age"/> 62.</summary>
const Age PiaParams::age62 = Age(62, 0);
/// <summary><see cref="Age"/> 62 and 1 month.</summary>
const Age PiaParams::age621 = Age(62, 1);
/// <summary><see cref="Age"/> 65.</summary>
const Age PiaParams::age65 = Age(65, 0);
/// <summary><see cref="Age"/> 65 and 2 months.</summary>
const Age PiaParams::age65_2 = Age(65, 2);
/// <summary><see cref="Age"/> 65 and 4 months.</summary>
const Age PiaParams::age65_4 = Age(65, 4);
/// <summary><see cref="Age"/> 65 and 6 months.</summary>
const Age PiaParams::age65_6 = Age(65, 6);
/// <summary><see cref="Age"/> 65 and 8 months.</summary>
const Age PiaParams::age65_8 = Age(65, 8);
/// <summary><see cref="Age"/> 65 and 10 months.</summary>
const Age PiaParams::age65_10 = Age(65, 10);
/// <summary><see cref="Age"/> 66.</summary>
const Age PiaParams::age66 = Age(66, 0);
/// <summary><see cref="Age"/> 66 and 2 months.</summary>
const Age PiaParams::age66_2 = Age(66, 2);
/// <summary><see cref="Age"/> 66 and 4 months.</summary>
const Age PiaParams::age66_4 = Age(66, 4);
/// <summary><see cref="Age"/> 66 and 6 months.</summary>
const Age PiaParams::age66_6 = Age(66, 6);
/// <summary><see cref="Age"/> 66 and 8 months.</summary>
const Age PiaParams::age66_8 = Age(66, 8);
/// <summary><see cref="Age"/> 66 and 10 months.</summary>
const Age PiaParams::age66_10 = Age(66, 10);
/// <summary><see cref="Age"/> 67.</summary>
const Age PiaParams::age67 = Age(67, 0);
/// <summary><see cref="Age"/> 70.</summary>
const Age PiaParams::age70 = Age(70, 0);
/// <summary><see cref="Age"/> 72.</summary>
const Age PiaParams::age72 = Age(72, 0);
/// <summary><see cref="Age"/> 75.</summary>
const Age PiaParams::age75 = Age(75, 0);
/// <summary><see cref="Age"/> 199 (impossible age).</summary>
const Age PiaParams::age199 = Age(199, 0);
/// <summary>Temporary benefit increase, March-May 1974.</summary>
const double PiaParams::beninc74 = 7.0;
/// <summary>Factor for calculating life benefits.</summary>
const double PiaParams::factor50 = 0.50;
/// <summary>Factor for calculating survivor benefits.</summary>
const double PiaParams::factor75 = 0.75;
/// <summary>Factor for calculating widow(er) benefits.</summary>
const double PiaParams::factor825 = 0.825;
/// <summary>Factor for calculating mfbs.</summary>
const double PiaParams::factor150 = 1.50;
/// <summary>Factor for calculating mfbs.</summary>
const double PiaParams::factor175 = 1.75;
/// <summary>Bend points for old-start PIB formula.</summary>
const int PiaParams::bendOS[] = { 0, 50, 250 };
/// <summary>Old-start PIB formula percentages.</summary>
const double PiaParams::percOS[] = { .40, .10 };
/// <summary>Percentage of old-law wage base required for year of
/// coverage for special minimum.</summary>
const double PiaParams::specMinPerc[] = { 0.25, 0.15 };
/// <summary>First quarter for which a quarter of coverage could be earned.
/// </summary>
const QtrYear PiaParams::year37qtr1 = QtrYear(0, YEAR37);
/// <summary>Starting point of deemed insured.</summary>
const QtrYear PiaParams::qtr184 = QtrYear(0, 1984);
/// <summary>Maximum number of months of actuarial reduction between ages 62
/// and 65.</summary>
const int PiaParams::maxMonthsAr62_65 = age65 - age62;
/// <summary>Maximum number of months of actuarial reduction between ages 62
/// and 67.</summary>
const int PiaParams::maxMonthsAr62_67 = age67 - age62;
/// <summary>Maximum number of months of actuarial reduction between ages 65
/// and 67.</summary>
const int PiaParams::maxMonthsAr65_67 = age67 - age65;
/// <summary>Monthly actuarial reduction factor between ages 62 and 65.
/// </summary>
const double PiaParams::arMonthlyFactorOab62_65 = 5.0 / 900.0;
/// <summary>Monthly actuarial reduction factor for aged spouses between
/// ages 62 and 65.</summary>
const double PiaParams::arMonthlyFactorSpouse62_65 = 25.0 / 3600.0;
/// <summary>Monthly actuarial reduction factor between ages 65 and 67.
/// </summary>
const double PiaParams::arMonthlyFactor65_67 = 5.0 / 1200.0;
/// <summary>Total actuarial reduction factor for aged widow(er)s.</summary>
const double PiaParams::arFactorWid285 = 0.285;

/// <summary>Constructor sets the starting year of projection.</summary>
///
/// <remarks>Initialize arrays and sets the starting year of projection.
/// </remarks>
///
/// <param name="newIstart">Year after last known benefit increase.</param>
/// <param name="newMaxyear">Maximum projected year.</param>
PiaParams::PiaParams( int newIstart, int newMaxyear ) :
qcamt(newMaxyear), bpMfbOut(newMaxyear), bpPiaOut(newMaxyear),
percPiaOut(newMaxyear), baseHi(newMaxyear), monthBeninc(YEAR51, newMaxyear),
noChange(YEAR37, newMaxyear), percSpecMin(YEAR37, newMaxyear),
yocAmountSpecMin(YEAR37, newMaxyear), yocAmountWindfall(YEAR37, newMaxyear),
cpiinc(YEAR51, newMaxyear), catchup(newIstart),
fq(YEAR37, newMaxyear), fqinc(newMaxyear), istart(newIstart),
maxyear(newMaxyear), specMinPia(), specMinPia2001(), specMinMfb(),
specMinMfb2001(), recalcInd(YEAR37, newMaxyear), titleAw(), titleBi()
{ }

/// <summary>Constructor sets the starting year as
/// <see cref="BaseYearNonFile::YEAR"/>.</summary>
///
/// <remarks>Initialize arrays and sets the starting year of projection.
/// </remarks>
///
/// <param name="newMaxyear">Maximum projected year.</param>
PiaParams::PiaParams( int newMaxyear ) :
qcamt(newMaxyear), bpMfbOut(newMaxyear), bpPiaOut(newMaxyear),
percPiaOut(newMaxyear), baseHi(newMaxyear), monthBeninc(YEAR51, newMaxyear),
noChange(YEAR37, newMaxyear), percSpecMin(YEAR37, newMaxyear),
yocAmountSpecMin(YEAR37, newMaxyear), yocAmountWindfall(YEAR37, newMaxyear),
cpiinc(YEAR51, newMaxyear), catchup(BaseYearNonFile::YEAR),
fq(YEAR37, newMaxyear), fqinc(newMaxyear), istart(BaseYearNonFile::YEAR),
maxyear(newMaxyear), specMinPia(), specMinPia2001(), specMinMfb(),
specMinMfb2001(), recalcInd(YEAR37, newMaxyear), titleAw(), titleBi()
{ }

/// <summary>Destroys arrays.</summary>
PiaParams::~PiaParams( )
{
  const size_t size1 = specMinPia.size();
  if (size1 > 0) {
    for (size_t i = 0; i < size1; i++) {
      delete specMinPia[i];
      delete specMinMfb[i];
    }
  }
}

/// <summary>Initializes some data.</summary>
void PiaParams::initdata()
{
  for (int year = YEAR37; year < 1991; year++)
    percSpecMin[year] = specMinPerc[0];
  for (int year = 1991; year <= maxyear; year++)
    percSpecMin[year] = specMinPerc[1];
  monthBeninc[1952] = 9;
  monthBeninc[1954] = 9;
  monthBeninc[1956] = 11;
  monthBeninc[1959] = 1;
  monthBeninc[1961] = 8;
  monthBeninc[1965] = 1;
  monthBeninc[1968] = 2;
  monthBeninc[1970] = 1;
  monthBeninc[1971] = 1;
  monthBeninc[1972] = 9;
  for (int year = 1974; year <= 1982; year++)
    monthBeninc[year] = 6;
  for (int year = 1983; year <= maxyear; year++)
    monthBeninc[year] = 12;
  // These years need recalculations because of an increase in the
  // minimum PIA that could make a person's new PIA be greater than
  // it would have been had the person just gotten the regular COLA
  recalcInd.setBit(1961, true);  // minimum PIA: $40
  recalcInd.setBit(1965, true);  // minimum PIA: $44
  recalcInd.setBit(1968, true);  // minimum PIA: $55
  recalcInd.setBit(1970, true);  // minimum PIA: $64
  // These 3 years need recalculations because of changes to the way
  // special mins were calculated.
  recalcInd.setBit(1973, true);
  recalcInd.setBit(1974, true);
  recalcInd.setBit(YEAR79, true);
  // This year needs a recalc because of a special provision giving an
  // increase in benefits of 1/10 of 1 percent starting in July 2001.
  recalcInd.setBit(2001, true);
  // If law-change file is read in after initialization, then this function
  // must be called again.
  projectPerc();
}

/// <summary>Sets the first projected year, including for members.</summary>
///
/// <remarks>The first projected year is set in the constructor, but may be
/// reset by this function. This is only necessary if the user wants to change
/// the base year for computations, which is rare (such as when a pension
/// actuary wants to get a benefit with the law as it stood at a prior
/// date).</remarks>
///
/// <param name="newIstart">New first projected year.</param>
void PiaParams::setIstart( int newIstart )
{
  BaseYear::yearCheck(newIstart);
  istart = newIstart;
}

/// <summary>Sets data from historical average wages and benefit increase
///   file.</summary>
///
/// <param name="awbiData">Structure with historical average wages and benefit
/// increases.</param>
void PiaParams::setData( AwbiData& awbiData )
{
  const int year = awbiData.getLastYear();
  cpiinc.assign(awbiData.cpiinc, YEAR37, year - 1);
  baseHi.assign(awbiData.baseHi, YEAR37, year);
  fq.assign(awbiData.fq, YEAR37, year - 2);
}

/// <summary>Applies catch-up benefit increases.</summary>
///
/// <returns>Increased benefit amount.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BICCHP"/> if either year is out of
/// range (debug mode only).</exception>
///
/// <param name="bcatch">Amount to be increased.</param>
/// <param name="year">Year of catch-up benefit increase.</param>
/// <param name="eligYear">Year of eligibility for catch-up benefit increases.
/// </param>
double PiaParams::benincCatchUp( double bcatch, int year, int eligYear ) const
{
#ifndef NDEBUG
  if (year < YEAR37 || eligYear < 1900) {
    throw PiaException(PIA_IDS_BICCHP);
  }
#endif
  if (catchup.exist(eligYear, year)) {
    const double rv = bcatch * (catchup.get(eligYear, year) / 100.0 + 1.0);
    return BenefitAmount::round(rv, year);
  }
  else {
    return bcatch;
  }
}

/// <summary>Sets indicators for no change allowed.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_SETNOCH"/> if year is out of range
/// (debug mode only).</exception>
///
/// <param name="firstYear">First year change is allowed.</param>
void PiaParams::setNoChange( int firstYear )
{
#ifndef NDEBUG
  if (firstYear < YEAR37) {
    throw PiaException(PIA_IDS_SETNOCH);
  }
#endif
  noChange.assign(1, YEAR37, firstYear);
  noChange.assign(0, firstYear, maxyear);
}

/// <summary>Sets historical average wage increases.</summary>
void PiaParams::setHistFqinc()
{
  fqinc.project(fq, YEAR37 + 1, istart - 2);
}

/// <summary>Finds last AMW in old-law table.</summary>
///
/// <returns>Last AMW in old-law table.</returns>
///
/// <param name="year">Year for which last AMW is desired.</param>
long PiaParams::lastAmw( int year ) const
{
  const long baseTemp = static_cast<long>(getBaseOasdi(year));
  // check to see if wage base is exact multiple of $60
  // ($22,900 in 1979 and $25,900 in 1980 are not)
  return((baseTemp % 60 > 0) ? (baseTemp / 60 + 1) * 5 : baseTemp / 12);
}

/// <summary>Returns full retirement age based on present law.</summary>
///
/// <returns>Full retirement age based on present law.</returns>
///
/// <param name="eligYear">Year of eligibility (year of attainment of age 62
/// for oab or wife/husband, year of attainment of age 60 for widow(er)).
/// </param>
Age PiaParams::fullRetAgeCalPL( int eligYear )
{
  if (eligYear < 2000)  // age 65
    return age65;
  if (eligYear > 1999 && eligYear < 2005) {
    switch (eligYear - 1999)
    {
    case 1:
      // age 65 and 2 months
      return age65_2;
    case 2:
      // age 65 and 4 months
      return age65_4;
    case 3:
      // age 65 and 6 months
      return age65_6;
    case 4:
      // age 65 and 8 months
      return age65_8;
    case 5:
      // age 65 and 10 months
      return age65_10;
    default:
      // impossible
      throw PiaException("Impossible age in PiaParams::fullRetAgeCalPL");
    }
  }
  if (eligYear > 2004 && eligYear < 2017)  // age 66
    return age66;
  if (eligYear > 2016 && eligYear < 2022) {
    switch (eligYear - 2016)
    {
    case 1:
      // age 66 and 2 months
      return age66_2;
    case 2:
      // age 66 and 4 months
      return age66_4;
    case 3:
      // age 66 and 6 months
      return age66_6;
    case 4:
      // age 66 and 8 months
      return age66_8;
    case 5:
      // age 66 and 10 months
      return age66_10;
    default:
      // impossible
      throw PiaException("Impossible age in PiaParams::fullRetAgeCalPL");
    }
  }
  // age 67
  return age67;
}

/// <summary>Returns full retirement age for benefit calculations involving
/// disability freeze.</summary>
///
/// <returns>Full retirement age for benefit calculations involving
/// disability freeze. In this class, it is based on present law, but a
/// subclass could change it.</returns>
///
/// <param name="eligYear">Year of eligibility (year of attainment of age 62
/// for oab or wife/husband, year of attainment of age 60 for widow(er)).
/// </param>
/// <param name="currentYear">Current year.</param>
Age PiaParams::fullRetAgeCalDIPL( int eligYear, int currentYear )
{
  return (currentYear < 2000) ? age65 : fullRetAgeCalPL(eligYear);
}

/// <summary>Returns amount of monthly delayed retirement credit.</summary>
///
/// <returns>Amount of monthly delayed retirement credit.</returns>
///
/// <param name="eligYear">Year of eligibility.</param>
double PiaParams::retCredit( int eligYear )
{
  if (eligYear < YEAR79)
    return(1.0 / 1200.0);
  if (eligYear >= YEAR79 && eligYear < 1987)
    return(1.0 / 400.0);
  if (eligYear > 1986 && eligYear < 2005) {
    return(static_cast<double>((eligYear - 1985) / 2) / 2400.0 +
      1.0 / 400.0);
  }
  return(2.0 / 300.0);
}

/// <summary>Returns earliest possible retirement age for old-age benie or
/// aged spouse, based on present law.</summary>
///
/// <returns>Earliest possible retirement age for old-age benie or aged
/// spouse, based on present law.</returns>
///
/// <param name="sex">The sex of number holder
/// (<see cref="Sex::sex_type::MALE"/> for male,
/// <see cref="Sex::sex_type::FEMALE"/> for female).</param>
/// <param name="kbirth">The date of birth, adjusted to previous day.</param>
Age PiaParams::earlyAgeOabCalPL( Sex::sex_type sex,
 const boost::gregorian::date& kbirth )
{
  if (sex == Sex::MALE) {
    // change to age 62
    if (amend611 < kbirth) {
      return (amend811 < kbirth && kbirth.day() != 1) ? age621 : age62;
    }
    else if (kbirth <= amend611 && amend612 < kbirth) {
      return (amend61 - kbirth);
    }
    else {
      return age65;
    }
  }
  else {
    // change to age 62
    if (amend561 < kbirth) {
      return (amend811 < kbirth && kbirth.day() != 1) ? age621 : age62;
    }
    else if (kbirth <= amend561 && amend562 < kbirth) {
      return (amend56 - kbirth);
    }
    else {
      return age65;
    }
  }
}

/// <summary>Returns earliest possible aged widow(er)'s retirement age.
/// </summary>
///
/// <returns>Earliest possible aged widow(er)'s retirement age.</returns>
///
/// <param name="benefitDate">Benefit date.</param>
Age PiaParams::earlyAgeWidCal( const DateMoyr& benefitDate )
{
  if (benefitDate < amend56) {
    return age65;
  }
  else if (benefitDate < amend652) {
    return age62;
  }
  else {
    return age60;
  }
}

/// <summary>Returns earliest possible disabled widow(er)'s retirement age.
/// </summary>
///
/// <returns>Earliest possible disabled widow(er)'s retirement age.</returns>
///
/// <param name="benefitDate">Benefit date.</param>
Age PiaParams::earlyAgeDisWidCal( const DateMoyr& benefitDate )
{
  // Not allowed until 1967
  if (benefitDate < amend672) {
    return age199;
  }
  else {
    return age50;
  }
}

/// <summary>Returns maximum DI beneficiary age at end of year, based on
/// present law.</summary>
///
/// <returns>Maximum DI beneficiary age at end of year, based on present law.
/// </returns>
///
/// <param name="year">Year of benefit.</param>
Age PiaParams::maxDibAgePL( int year )
{
  if (year < 2003)  // age 65
    return age65;
  if (year > 2002 && year < 2008) {
    switch (year - 2002)
    {
    case 1:
      // age 65 and 2 months
      return age65_2;
    case 2:
      // age 65 and 4 months
      return age65_4;
    case 3:
      // age 65 and 6 months
      return age65_6;
    case 4:
      // age 65 and 8 months
      return age65_8;
    case 5:
      // age 65 and 10 months
      return age65_10;
    default:
      // impossible
      throw PiaException("Impossible age in PiaParams::maxDibAgePL");
    }
  }
  if (year > 2007 && year < 2021)  // age 66
    return age66;
  if (year > 2020 && year < 2026) {
    switch (year - 2020)
    {
    case 1:
      // age 66 and 2 months
      return age66_2;
    case 2:
      // age 66 and 4 months
      return age66_4;
    case 3:
      // age 66 and 6 months
      return age66_6;
    case 4:
      // age 66 and 8 months
      return age66_8;
    case 5:
      // age 66 and 10 months
      return age66_10;
    default:
      // impossible
      throw PiaException("Impossible age in PiaParams::maxDibAgePL");
    }
  }
  // age 67
  return age67;
}

/// <summary>Updates amounts required for special minimum year of coverage.
/// </summary>
void PiaParams::updateYocAmountSpecMin()
{
  // set percentage of old-law base required for year of coverage
  double factor = getSpecMinPerc(PRE1991);
  for (int year = YEAR51; year <= yocAmountWindfall.getLastYear(); year++) {
    yocAmountWindfall[year] = factor * getBase77(year);
  }
  for (int year = YEAR51; year <= yocAmountSpecMin.getLastYear(); year++) {
    factor = getPercSpecMin(year);
    yocAmountSpecMin[year] = factor * getBase77(year);
  }
}

/// <summary>Updates average wages, using specified amounts.</summary>
///
/// <remarks>After filling in the average wages, it projects the percentage
/// increases in average wages in <see cref="fqinc"/>. Then it calls
/// <see cref="projectFq"/> to project the amounts dependent on average
/// wages.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_UPDATEFQ"/> if average wage is zero
/// (only in debug mode).</exception>
///
/// <param name="userfq">Specified average wages.</param>
/// <param name="firstYeart">First year of specified average wages.</param>
/// <param name="lastYeart">Last year of specified average wages.</param>
void PiaParams::updateFq( const DoubleAnnual& userfq, int firstYeart,
int lastYeart )
{
  fq.assign(userfq, firstYeart, lastYeart);
  for (int year = firstYeart; year <= lastYeart; year++) {
#if !defined(NDEBUG)
    if (fq[year-1] < 0.001)
      throw PiaException(PIA_IDS_UPDATEFQ);
#endif
    fqinc[year] = (fq[year] / fq[year-1] - 1.0) * 100.0;
  }
  projectFq();
}

/// <summary>Updates increases in average wages, using specified percentages.
/// </summary>
///
/// <remarks>After filling in the percentage increases in average wages, it
/// projects the amount of average wages. Then it calls
/// <see cref="projectFq"/> to project the amounts dependent on average
/// wages.</remarks>
///
/// <param name="userFqinc">Specified average wage percentage increases.
/// </param>
/// <param name="userTitle">Specified title of assumptions.</param>
/// <param name="firstYeart">First year of specified increases.</param>
/// <param name="lastYeart">Last year of specified increases.</param>
void PiaParams::updateFqinc( const DoubleAnnual& userFqinc,
const std::string& userTitle, int firstYeart, int lastYeart )
{
  // last year of specified assumptions
  const int lastyear = min(lastYeart, getMaxyear());
  if (lastyear < getMaxyear()) {
    fqinc.assign(0.0, lastyear + 1, getMaxyear());
  }
  fqinc.assign(userFqinc, firstYeart, lastyear);
  titleAw = userTitle;
  fq.project(fqinc, getIstart() - 1);
  projectFq();
}

/// <summary>Projects amounts dependent on the average annual wage.</summary>
void PiaParams::projectFq()
{
  qcamt.project(fq, 1978);
  bpMfbOut.setData(getFqArray());
  bpPiaOut.set1979Data(getFqBppia());
}

/// <summary>Projects benefit formula percentages.</summary>
void PiaParams::projectPerc()
{
  double percTemp[3];
  percTemp[0] = PercPia::PERC[0];
  percTemp[1] = PercPia::PERC[1];
  percTemp[2] = PercPia::PERC[2];
  for (int year = YEAR79; year <= maxyear; year++) {
    percPiaOut.setData(year, 3, percTemp);
  }
}

/// <summary>Resizes special minimum arrays.</summary>
///
/// <remarks>This is called in <see cref="PiaParamsPL::projectCpiinc"/>.
/// </remarks>
void PiaParams::resizeSpecMin()
{
  const size_t size1 = specMinPia.size();
  if (size1 > 0) {
    for (size_t i = 0; i < size1; i++) {
      delete specMinPia[i];
      delete specMinMfb[i];
    }
  }
  const size_t size2 = specMinMaxYears();
  specMinPia.resize(size2);
  specMinMfb.resize(size2);
  for (size_t i = 0; i < size2; i++) {
    specMinPia[i] = new DoubleAnnual(YEAR37, maxyear);
    specMinMfb[i] = new DoubleAnnual(YEAR37, maxyear);
  }
  specMinPia2001.resize(size2);
  specMinMfb2001.resize(size2);
}

/// <summary>Projects special minimum amounts.</summary>
///
/// <param name="baseYear">First year of special minimum amounts (1979 for
///  present law).</param>
/// <param name="lastYear">Last year of projection.</param>
void PiaParams::projectSpecMin( int baseYear, int lastYear )
{
  const double amountPerYear = specMinAmountCal(DateMoyr(1, baseYear));
  for (int numYears = 0; numYears < specMinMaxYears(); numYears++) {
    double pia = (numYears + 1) * amountPerYear;
    double mfb = BenefitAmount::round(factor150 * pia, baseYear);
    DoubleAnnual& daPia = *specMinPia[numYears];
    DoubleAnnual& daMfb = *specMinPia[numYears];
    if (baseYear == YEAR79) {
      daPia[1978] = pia;
      daMfb[1978] = mfb;
      // handle years up through 2000
      for (int year = baseYear; year <= min(2000, lastYear); year++) {
        pia = applyCola(pia, year);
        daPia[year] = pia;
        mfb = applyColaMfb(mfb, year, pia);
        daMfb[year] = mfb;
      }
      // handle 2001 (special August and regular December numbers)
      if (lastYear > 2000) {
        // recalculate values for December 1999, adding 0.1 percent
        double pia1999 = applyCola99(daPia[1998]);
        double mfb1999 = applyColaMfb99(daMfb[1998], pia1999);
        // recalculate values for December 2000, adding 0.1 percent
        // these become effective August 2001
        pia1999 = applyCola(pia1999, 2000);
        specMinPia2001[numYears] = pia1999;
        mfb1999 = applyColaMfb(mfb1999, 2000, pia);
        specMinMfb2001[numYears] = mfb1999;
        // calculate values for December 2001
        pia = applyCola(pia1999, 2001);
        daPia[2001] = pia;
        mfb = applyColaMfb(mfb1999, 2001, pia);
        daMfb[2001] = mfb;
      }
    }
    // handle years after 2001
    for (int year = max(2002, baseYear); year <= lastYear; year++) {
      pia = applyCola(pia, year, YEAR79);
      daPia[year] = pia;
      mfb = applyColaMfb(mfb, year, YEAR79, pia);
      daMfb[year] = mfb;
    }
  }
}

/// <summary>Updates benefit increases.</summary>
///
/// <remarks>This version updates with catch-up increases.</remarks>
///
/// <param name="userBiproj">User-entered benefit increases.</param>
/// <param name="userCatchup">User-entered catch-up benefit increases.</param>
/// <param name="userTitle">User-entered title of benefit increases.</param>
/// <param name="firstYeart">First year of user-specified benefit increases.
/// </param>
/// <param name="lastYeart">Last year of user-specified benefit increases.
/// </param>
void PiaParams::updateCpiinc( const DoubleAnnual& userBiproj,
const Catchup& userCatchup, const std::string& userTitle, int firstYeart,
int lastYeart )
{
  int lastyear;  // last year of specified assumptions
  if (lastYeart < getMaxyear()) {
    lastyear = lastYeart;
    cpiinc.assign(0.0f, lastyear + 1, getMaxyear());
  }
  else
    lastyear = getMaxyear();
  cpiinc.assign(userBiproj, firstYeart, lastyear);
  catchup = userCatchup;
  titleBi = userTitle;
  projectCpiinc();
}

/// <summary>Updates benefit increases.</summary>
///
/// <remarks>This version updates benefit increases without catch-up
/// increases.</remarks>
///
/// <param name="userBiproj">User-entered benefit increases.</param>
/// <param name="userTitle">User-entered title of benefit increases.</param>
/// <param name="firstYeart">First year of user-specified benefit increases.
/// </param>
/// <param name="lastYeart">Last year of user-specified benefit increases.
/// </param>
void PiaParams::updateCpiinc( const DoubleAnnual& userBiproj,
const std::string& userTitle, int firstYeart, int lastYeart )
{
  int lastyear;  // last year of specified assumptions
  if (lastYeart < getMaxyear()) {
    lastyear = lastYeart;
    cpiinc.assign(0.0, lastyear + 1, getMaxyear());
  }
  else
    lastyear = getMaxyear();
  cpiinc.assign(userBiproj, firstYeart, lastyear);
  catchup.deleteContents();
  titleBi = userTitle;
  projectCpiinc();
}

/// <summary>Returns benefit amount decreased by cola.</summary>
///
/// <remarks>This version does not consider any catch-up benefit decreases.
/// </remarks>
///
/// <returns>Benefit amount decreased by cola.</returns>
///
/// <param name="pia">Amount to be decreased.</param>
/// <param name="year">Year of decrease.</param>
double PiaParams::unApplyCola( double pia, int year ) const
{
  return BenefitAmount::unApplyCola(pia, getCpiinc(year), year);
}

/// <summary>Returns benefit amount decreased by cola.</summary>
///
/// <remarks>This version considers catch-up benefit decreases.</remarks>
///
/// <returns>Benefit amount decreased by cola.</returns>
///
/// <param name="pia">Amount to be decreased.</param>
/// <param name="year">Year of decrease.</param>
/// <param name="eligYear">Year of eligibility for catch-up benefit decreases.
/// </param>
double PiaParams::unApplyCola( double pia, int year, int eligYear ) const
{
  const double rv = BenefitAmount::unApplyCola(pia, getCpiinc(year), year);
  return(unbiCatchUp(year, rv, eligYear));
}

/// <summary>Returns benefit amount decreased by special 1999 cola
/// (includes extra 0.1 percent decrease.)</summary>
///
/// <returns>Benefit amount decreased by special 1999 cola (includes
/// extra 0.1 percent decrease.)</returns>
///
/// <param name="pia">Amount to be decreased.</param>
double PiaParams::unApplyCola99( double pia ) const
{
  return BenefitAmount::unApplyCola(pia, getCpiinc(YEAR1999) + 0.1,
    YEAR1999);
}

/// <summary>Deconverts PIA back to year of eligibility.</summary>
///
/// <returns>Deconverted PIA.</returns>
///
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="number">Number of years for which benefit increases are
///  applied.</param>
/// <param name="pia77">PIA or MFB to be increased.</param>
/// <param name="benDate">The date of benefit.</param>
double PiaParams::deconvertPia( int eligYear, int number, double pia77,
const DateMoyr& benDate ) const
{
  double rv = pia77;
  // return if there are no CPI increases applied
  if (number <= 0)
    return rv;
  // do not handle year of eligibility prior to 1979
  if (eligYear < YEAR79)
    return rv;
  // divide out increases, starting with last one
  for (int year = eligYear + number - 1; year >= eligYear; year--) {
    // check for special 1999 increase (with extra 0.1 percent)
    if (PiaParams::isApplicableCola99(year, benDate)) {
      rv = unApplyCola99(rv);
    }
    else {
      const double temp =
        BenefitAmount::unApplyCola(rv, getCpiinc(year), year);
      rv = unbiCatchUp(year, temp, eligYear);
    }
  }
  return(rv);
}

/// <summary>Divides out catch-up benefit increases.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_UNBICCHP"/> if either year is out of
/// range (debug mode only).</exception>
///
/// <returns>Amount with catch-up benefit increase removed.</returns>
///
/// <param name="year">Year of catch-up benefit increase.</param>
/// <param name="bcatch">Amount to be unincreased.</param>
/// <param name="eligYear">Year of eligibility.</param>
double PiaParams::unbiCatchUp( int year, double bcatch, int eligYear ) const
{
#ifndef NDEBUG
  if (year < YEAR37 || eligYear < YEAR37)
    throw PiaException(PIA_IDS_UNBICCHP);
#endif
  if (catchup.exist(eligYear, year)) {
    const double rv = bcatch / (catchup.get(eligYear, year) / 100.0 + 1.0);
    return BenefitAmount::unround(rv, year);
  }
  else
    return bcatch;
}

/// <summary>Calculates number of months of actuarial reduction for OAB.
/// </summary>
///
/// <remarks>Assumes age is no more than the full retirement age. Returns the
/// number of months between those two ages.</remarks>
///
/// <returns>Number of months of actuarial reduction for OAB.</returns>
///
/// <param name="age">The age for which reduction is being calculated.</param>
/// <param name="fullRetAge">Full retirement age.</param>
int PiaParams::monthsArCal( const Age& age, const Age& fullRetAge )
{
  return (fullRetAge - age);
}

/// <summary>Calculates reduction factor for OAB or DIB.</summary>
///
/// <returns>Reduction factor for OAB or DIB.</returns>
///
/// <remarks>The reduction is 5/9 of 1 percent for each of the first 36 months
/// of reduction, and 5/12 of 1 percent for each additional month. Assumes a
/// maximum of 60 months of reduction.</remarks>
///
/// <param name="monthsArdri">Number of months of reduction.</param>
double PiaParams::factorArCalPL( int monthsArdri )
{
  if (monthsArdri <= maxMonthsAr62_65) {
    return (1.0 -
      (static_cast< double >(monthsArdri) * arMonthlyFactorOab62_65));
  }
  else {
    const int excessMonths = monthsArdri - maxMonthsAr62_65;
    return (1.0 -
      static_cast< double >(maxMonthsAr62_65) * arMonthlyFactorOab62_65 -
      static_cast< double >(excessMonths) * arMonthlyFactor65_67);
  }
}

/// <summary>Calculates reduction factor for wife/husband. Assumes a maximum
/// of 60 months of reduction.</summary>
///
/// <returns>Reduction factor for wife/husband.</returns>
///
/// <param name="monthsArdri">Number of months of reduction.</param>
double PiaParams::factorArAgedSpouseCalPL( int monthsArdri )
{
  // do not allow any delayed retirement credit
  if (monthsArdri < 0) {
    return 0.0;
  }
  else {
    if (monthsArdri <= maxMonthsAr62_65) {
      return (1.0 -
        static_cast<double>(monthsArdri) * arMonthlyFactorSpouse62_65);
    }
    else {
      const int excessMonths = monthsArdri - maxMonthsAr62_65;
      return (1.0 -
        static_cast<double>(maxMonthsAr62_65) * arMonthlyFactorSpouse62_65 -
        static_cast<double>(excessMonths) * arMonthlyFactor65_67);
    }
  }
}

/// <summary>Calculates number of months of delayed retirement credit for
/// OAB.</summary>
///
/// <remarks>Delayed retirement credit can be earned for months beginning in
/// January 1971, but only if the worker has attained full retirement age and
/// is fully insured.  Credits stop at age 72 or 70.</remarks>
///
/// <returns>Number of months of delayed retirement credit for OAB.</returns>
///
/// <param name="fullRetDate">The date of attainment of full retirement age.
/// </param>
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="dobadj">Adjusted date of birth.</param>
/// <param name="entDate">The date of entitlement.</param>
/// <param name="benefitDate">The date of benefit.</param>
/// <param name="fullInsDate">The date fully insured status is attained.
/// </param>
int PiaParams::monthsDriCal( const DateMoyr& fullRetDate, int eligYear,
const boost::gregorian::date& dobadj, const DateMoyr& entDate,
const DateMoyr& benefitDate, const DateMoyr& fullInsDate )
{
  int i2;  // index of month after last month to which delayed
    // retirement credit can apply
  int i4;  // index of month of age 72 or 70
  DateMoyr tempDate;  // temporary date
  // set index of month of attainment of full retirement age, or month of
  // attainment of fully insured status, if later.
  const int i3 = max(fullRetDate.index(), fullInsDate.index());
  // set index of first month to which delayed retirement credit can
  // apply (Jan 1971 = 0)
  const int i1 = max(i3, 0);
  // decrease maximum age for DRI from 72 to 70 in Jan 1984
  if (eligYear <= 1975) {
    tempDate = DateMoyr(dobadj) + age72;
    i4 = tempDate.index();
    const int i8 = amend83.index();
    if (eligYear >= 1974 && i4 > i8)
      i4 = i8;
  }
  else {
    tempDate = DateMoyr(dobadj) + age70;
    i4 = tempDate.index();
  }
  // set index of month of entitlement
  const int i5 = entDate.index();
  // set index of January of year of entitlement, or month of
  // attainment of full retirement age, if later
  tempDate = DateMoyr(boost::date_time::Jan, entDate.getYear());
  const int i6 = max(i3, tempDate.index());
  // set index of month of benefit
  const int i7 = benefitDate.index();
  // if already age 70 (or 72), use DRCs through age 70 (or 72)
  if (i4 <= i5)
    i2 = i4;
  else {
    // if calculating benefit after age 70 (or 72), or in year after
    // entitlement, use DRCs through entitlement
    if (i4 <= i7 || benefitDate.getYear() > entDate.getYear())
      i2 = i5;
    // otherwise use DRCs through end of year before entitlement
    else
      i2 = i6;
  }
  // no DRC until January 1973
  if (entDate < amend722)
    i2 = 0;
  return(max(i2 - i1, 0));
}

/// <summary>Calculates delayed retirement increment factor for OAB.</summary>
///
/// <returns>Delayed retirement increment factor for OAB.</returns>
///
/// <param name="monthsArdri">Number of months of increment.</param>
/// <param name="eligYear">Year of eligibility.</param>
double PiaParams::factorDriCal( int monthsArdri, int eligYear )
{
  return(1.0 + static_cast<double>(monthsArdri) * retCredit(eligYear));
}

/// <summary>Calculates number of months of actuarial reduction for disabled
/// widow(er).</summary>
///
/// <returns>Number of months of actuarial reduction for disabled widow(er).
/// </returns>
///
/// <remarks>Before 1984, includes all months from entitlement date to age 65.
/// Starting in 1984, includes months only from age 60 to full retirement
/// age.</remarks>
///
/// <param name="age">The age for which reduction is being calculated.</param>
/// <param name="benefitDate">The date of benefit.</param>
/// <param name="fullRetAge">Full retirement age of widow(er).</param>
int PiaParams::monthsArDisWidCal( const Age& age,
const DateMoyr& benefitDate, const Age& fullRetAge )
{
  return (benefitDate < amend83) ? (age65 - age) : (fullRetAge - age60);
}

/// <summary>Calculates reduction factor for disabled widow(er).</summary>
///
/// <returns>Reduction factor for disabled widow(er).</returns>
///
/// <param name="monthsArdri">Number of months of reduction. This includes
/// months to full retirement age. These are ignored starting in 1984, when
/// all disabled widow(er)s get 71.5 percent of PIA. But before 1984, we must
/// use the number of months prior to age 60 to calculate the correct
/// reduction. So we subtract 60 (the number of months from 60 to 65) from
/// monthsArdri before calculating the reduction factor.</param>
/// <param name="entDate">The date of entitlement.</param>
double PiaParams::factorArDisWidCal( int monthsArdri,
  const DateMoyr& entDate )
{
  if (entDate < amend722)
    return(0.69167 -
    (static_cast<double>(monthsArdri - 60) * 43.0 / 19800.0));
  if (entDate < amend83)
    return(0.715 - (static_cast<double>(monthsArdri - 60) * 43.0 / 24000.0));
  return(0.715);
}

/// <summary>Calculates number of months of actuarial reduction for aged
/// widow(er).</summary>
///
/// <returns>Mumber of months of actuarial reduction for aged widow(er).
/// Returns 0 if entitlement is at or after full retirement age.</returns>
///
/// <param name="age">The age for which reduction is being calculated.</param>
/// <param name="benefitDate">The date of benefit.</param>
/// <param name="fullRetAge">Full retirement age of widow(er).</param>
int PiaParams::monthsArWidCal( const Age& age, const DateMoyr& benefitDate,
const Age& fullRetAge )
{
  if (benefitDate < amend61) {
    return(0);
  }
  if (benefitDate < amend722) {
    return (age < age62) ? (age62 - age) : 0;
  }
  // handle 1973 and later
  return (age < fullRetAge) ? (fullRetAge - age) : 0;
}

/// <summary>Calculates number of months of actuarial reduction for aged
/// wife/husband.</summary>
///
/// <returns>Number of months of actuarial reduction for aged wife/husband.
/// Returns 0 if entitlement is at or after full retirement age.</returns>
///
/// <param name="age">The age for which reduction is being calculated.</param>
/// <param name="benefitDate">The date of benefit.</param>
/// <param name="fullRetAge">Full retirement age of wife/husband.</param>
int PiaParams::monthsArAgedSpouseCal( const Age& age,
const DateMoyr& benefitDate, const Age& fullRetAge )
{
  if (benefitDate < amend56) {
    return(0);
  }
  return (age < fullRetAge) ? (fullRetAge - age) : 0;
}

/// <summary>Calculates reduction factor for aged widow(er).</summary>
///
/// <returns>Reduction factor for aged widow(er).</returns>
///
/// <param name="monthsArdri">Number of months of reduction.</param>
/// <param name="age">The age for which reduction is being calculated.</param>
/// <param name="benefitDate">The date of benefit.</param>
/// <param name="fullRetAge">Full retirement age of widow(er).</param>
double PiaParams::factorArWidCal( int monthsArdri, const Age& age,
const DateMoyr& benefitDate, const Age& fullRetAge ) const
{
  if (benefitDate < amend722) {
    return(1.0);
  }
  // handle 1973 and later
  if (!(age < fullRetAge)) {
    return(1.0);
  }
  else {
    // set maximum possible number of months of reduction
    const int maxMonths = fullRetAge - age60;
    const double ratio =
      static_cast<double>(monthsArdri) / static_cast<double>(maxMonths);
    return (1.0 - ratio * arFactorWid285);
  }
}

/// <summary>Returns factor for widow/widower benefit before age reduction.
/// </summary>
///
/// <returns>Factor for widow/widower benefit before age reduction.</returns>
///
/// <param name="monthsArdri">Months of reduction.</param>
/// <param name="age">The age of widow/widower.</param>
/// <param name="benefitDate">The date of benefit.</param>
double PiaParams::factorAgedWidCalPL( int monthsArdri, const Age& age,
const DateMoyr& benefitDate )
{
  if (benefitDate < amend61) {
    return(factor75);
  }
  if (benefitDate < amend722) {
    if (age.getYears() >= 62)
      return(factor825);
    else {
      return((factor825 - (static_cast<double>(monthsArdri) * 5.0 / 900.0)));
    }
  }
  // handle 1973 and later
  return(1.0);
}

/// <summary>Returns factor for disabled widow/widower benefit before age
/// reduction.</summary>
///
/// <returns>Factor for disabled widow/widower benefit before age reduction.
/// </returns>
///
/// <remarks>For benefits prior to 1973, returns 0.825. For 1973 and later,
/// returns 1.0, even though all disabled widow/widower benefits are 71.5
/// percent of PIA. We must do it this way because the unreduced benefit is
/// used when applying the maximum family benefit. After the MFB, we reduce
/// the benefit for age (0.715 factor).</remarks>
///
/// <param name="benefitDate">The date of benefit.</param>
double PiaParams::factorDisWidCalPL( const DateMoyr& benefitDate )
{
  if (benefitDate < amend722) {
    return(factor825);
  }
  // handle 1973 and later
  return(1.0);
}

/// <summary>Calculates reduction factor for young survivor.</summary>
///
/// <returns>0.75 for present law.</returns>
double PiaParams::factorYngSurvCal() const
{
  return(factor75);
}

/// <summary>Calculates reduction factor for young life benie.</summary>
///
/// <returns>0.50 for present law.</returns>
double PiaParams::factorYngLifeCal() const
{
  return(factor50);
}

/// <summary>Calculates number of months of reduction for disabled worker.
/// </summary>
///
/// <returns>The number of months from oabEntDate to oabCessDate.</returns>
///
/// <remarks>Disabled workers do not have any reduction unless they received
/// an old-age benefit prior to their disability benefit.</remarks>
///
/// <param name="oabEntDate">The date of entitlement to an old-age benefit
/// prior to the disabled worker benefit. Should be the default
/// <see cref="DateMoyr"/> (0 month and 0 year) if none.</param>
/// <param name="oabCessDate">The date of cessation of an old-age benefit
/// prior to the disabled worker benefit. Should be the default
/// <see cref="DateMoyr"/> (0 month and 0 year) if none.</param>
int PiaParams::monthsArDICal( const DateMoyr& oabEntDate,
const DateMoyr& oabCessDate )
{
  const Age tempAge(oabCessDate - oabEntDate);
  return(tempAge.toMonths());
}

/// <summary>Returns oldest age of child for which parent can get young
/// wife/husband or mother/father benefits.</summary>
///
/// <returns>Infinite age (199) if isDisabled is true. Otherwise, returns 18
/// if benefitDate is before the 1981 amendments, 16 if at that date or
/// later.</returns>
///
/// <param name="benefitDate">Benefit date.</param>
/// <param name="isDisabled">True if child is disabled, false otherwise.
/// </param>
Age PiaParams::childAgeForMFCal( const DateMoyr& benefitDate,
  bool isDisabled )
{
  if (isDisabled)
    return age199;
  return (benefitDate < amend813) ? age18 : age16;
}

/// <summary>Returns amount of lump sum benefit.</summary>
///
/// <returns>Simply $255.</returns>
double PiaParams::lumpSum()
{
  return(255.0);
}

/// <summary>Returns maximum child beneficiary age.</summary>
///
/// <returns>Maximum child beneficiary age.</returns>
///
/// <param name="benefitDate">The date of benefit.</param>
/// <param name="isDisabled">True if child is disabled, false otherwise.
/// </param>
/// <param name="isStudent">True if student, false otherwise.</param>
Age PiaParams::maxChildAge( const DateMoyr& benefitDate, bool isDisabled,
  bool isStudent )
{
  if (benefitDate < amend652)  // age 18
    return age18;
  if (!isDisabled) {
    if (isStudent) {
      return (benefitDate < amend813) ? age22 : age19;
    }
    else {
      return age18;
    }
  }
  else {
    return age199;
  }
}

/// <summary>Returns benefit amount increased by cola.</summary>
///
/// <remarks>This version does not consider any catch-up benefit increases.
/// </remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_APPCOLA1"/> if year is out of
/// range (debug mode only).</exception>
///
/// <returns>Benefit amount increased by cola, without considering any
/// catch-up benefit increases.</returns>
///
/// <param name="pia">Amount to be increased.</param>
/// <param name="year">Year of increase.</param>
double PiaParams::applyCola( double pia, int year ) const
{
#ifndef NDEBUG
  if (year < YEAR37) {
    throw PiaException(PIA_IDS_APPCOLA1);
  }
#endif
  return BenefitAmount::applyCola(pia, getCpiinc(year), year);
}

/// <summary>Returns benefit amount increased by cola.</summary>
///
/// <remarks>This version considers any catch-up benefit increases.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_APPCOLA1"/> if year is out of range (debug mode only).
/// </exception>
///
/// <returns>Benefit amount increased by cola, considering any catch-up
/// benefit increases.</returns>
///
/// <param name="pia">Amount to be increased.</param>
/// <param name="year">Year of increase.</param>
/// <param name="eligYear">Year of eligibility for catch-up benefit
/// increases.</param>
double PiaParams::applyCola( double pia, int year, int eligYear ) const
{
#ifndef NDEBUG
  if (year < YEAR37 || eligYear < 1900) {
    throw PiaException(PIA_IDS_APPCOLA2);
  }
#endif
  const double rv = BenefitAmount::applyCola(pia, getCpiinc(year), year);
  return(benincCatchUp(rv, year, eligYear));
}

/// <summary>Returns benefit amount increased by special 1999 cola (includes
/// extra 0.1 percent increase).</summary>
///
/// <returns>Benefit amount increased by special 1999 cola (includes extra
/// 0.1 percent increase).</returns>
///
/// <param name="pia">Amount to be increased.</param>
double PiaParams::applyCola99( double pia ) const
{
  return BenefitAmount::applyCola(pia, getCpiinc(YEAR1999) + 0.1, YEAR1999);
}

/// <summary>Returns maximum family benefit amount increased by cola, with
/// check that result is at least 150 percent of pia.</summary>
///
/// <remarks>This version does not consider any catch-up benefit increases.
/// </remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_APPCOLA3"/> if year is out of range (debug mode only).
/// </exception>
///
/// <returns>Maximum family benefit amount increased by cola.</returns>
///
/// <param name="mfb">Amount to be increased.</param>
/// <param name="year">Year of increase.</param>
/// <param name="pia">Pia to check against.</param>
double PiaParams::applyColaMfb( double mfb, int year, double pia ) const
{
#ifndef NDEBUG
  if (year < YEAR37) {
    throw PiaException(PIA_IDS_APPCOLA3);
  }
#endif
  // set temporary return value
  const double rv = BenefitAmount::applyCola(mfb, getCpiinc(year), year);
   // check to see that mfb is at least 150 percent of pia
  const double mfbt = BenefitAmount::round(factor150 * pia, year);
  return max(rv, mfbt);
}

/// <summary>Returns maximum family benefit amount increased by cola, with
/// check that result is at least 150 percent of pia.</summary>
///
/// <remarks>This version considers any catch-up benefit increases.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_APPCOLA3"/> if year is out of range (debug mode only).
/// </exception>
///
/// <returns>Maximum family benefit amount increased by cola.</returns>
///
/// <param name="mfb">Amount to be increased.</param>
/// <param name="year">Year of increase.</param>
/// <param name="pia">Pia to check against.</param>
/// <param name="eligYear">Year of eligibility for catch-up benefit
/// increases.</param>
double PiaParams::applyColaMfb( double mfb, int year, int eligYear,
double pia ) const
{
#ifndef NDEBUG
  if (year < YEAR37 || eligYear < 1900) {
    throw PiaException(PIA_IDS_APPCOLA4);
  }
#endif
  const double rv = applyCola(mfb, year, eligYear);
  // check to see that mfb is at least 150 percent of pia
  const double mfbt = BenefitAmount::round(factor150 * pia, year);
  return max(mfbt, rv);
}

/// <summary>Returns maximum family benefit amount increased by special 1999
/// cola (includes extra 0.1 percent increase), with check that result is at
/// least 150 percent of pia.</summary>
///
/// <returns>Maximum family benefit amount increased by cola.</returns>
///
/// <param name="mfb">Amount to be increased.</param>
/// <param name="pia">Pia to check against.</param>
double PiaParams::applyColaMfb99( double mfb, double pia ) const
{
  // set temporary return value
  const double rv =
    BenefitAmount::applyCola(mfb, getCpiinc(YEAR1999) + 0.1, YEAR1999);
  // check to see that mfb is at least 150 percent of pia
  const double mfbt = BenefitAmount::round(factor150 * pia, YEAR1999);
  return max(rv, mfbt);
}

/// <summary>Returns amount per year of coverage in special minimum.</summary>
///
/// <returns>Amount per year of coverage in special minimum.</returns>
///
/// <param name="date">The date of desired amount.</param>
double PiaParams::specMinAmountCalPL( const DateMoyr& date )
{
  // set amount per year depending on date of entitlement
  if (date < amend722)
    return 0.00;
  // from Jan 1973 to Feb 1974, was $8.50 per year of coverage
  if (date < amend741)
    return(8.50);
  // from March 1974 to Dec 1978, was $9.00 per year of coverage
  if (date < amend772)
    return(9.00);
  // for January 1979 and later, use $11.50 per year of coverage
  return(11.50);
}

/// <summary>Returns special minimum pia as of a specified month.</summary>
///
/// <returns>Special minimum pia.</returns>
///
/// <param name="dateMoyr">Month and year for which pia is desired
/// (January 1979 or later).</param>
/// <param name="yoc">Years of coverage in excess of 10 (1-20).</param>
double PiaParams::getSpecMinPia( const DateMoyr& dateMoyr, int yoc ) const
{
  unsigned year = dateMoyr.getYear();
  const unsigned month = dateMoyr.getMonth();
  if (static_cast<int>(month) < getMonthBeninc(year)) {
    // in 2001, apply extra 0.1 percent in August
    if (year == amend01.getYear() && month >= amend01.getMonth()) {
      return((yoc > 0) ? specMinPia2001[yoc-1] : 0.0);
    }
    else {
      year--;
    }
  }
  // if did not return above, use regular special minimum table
  return((yoc > 0) ? (*specMinPia[yoc-1])[year] : 0.0);
}

/// <summary>Returns special minimum mfb as of a specified month.</summary>
///
/// <returns>Special minimum mfb.</returns>
///
/// <param name="dateMoyr">Month and year for which mfb is desired
/// (January 1979 or later).</param>
/// <param name="yoc">Years of coverage in excess of 10 (1-20).</param>
double PiaParams::getSpecMinMfb( const DateMoyr& dateMoyr, int yoc ) const
{
   unsigned year = dateMoyr.getYear();
   if (static_cast<int>(dateMoyr.getMonth()) < getMonthBeninc(year)) {
      // in 2001, apply extra 0.1 percent in August
      if (year == 2001 && dateMoyr.getMonth() >= 8) {
         return((yoc > 0) ? specMinMfb2001[yoc-1] : 0.0);
      }
      else {
         year--;
      }
   }
   // if did not return above, use regular special minimum table
   return((yoc > 0) ? (*specMinMfb[yoc-1])[year] : 0.0);
}

/// <summary>Returns number of quarters of coverage required for deemed
/// insured status.</summary>
///
/// <returns>Number of quarters of coverage required for deemed insured
/// status. Returns -1 if ineligible because of age (born January 2, 1929 or
/// later).</returns>
///
/// <remarks>Note that this is a static function.</remarks>
///
/// <param name="year">Adjusted year of birth.</param>
int PiaParams::deemedQcReqCal( int year )
{
  if (year < 1924)
    return(6);
  if (year < 1925)
    return(8);
  if (year < 1926)
    return(12);
  if (year < 1927)
    return(16);
  if (year < 1929)
    return(20);
  // if ineligible, because of age, use -1
  return(-1);
}

/// <summary>Returns widow(er) benefit reduced for RIB-LIM.</summary>
///
/// <returns>Reduced widow(er) benefit (with rounding applied).</returns>
///
/// <remarks>This should only be called in cases where the deceased worker
/// was an OAB before death. Note that this is a static function.
///
/// Before 1973, the widow(er) benefit was 82.5 percent of PIA. Starting in
/// 1973, there was an age-related factor applied to the PIA, resulting in
/// benefits that could be more or less than 82.5 percent of PIA. As part of
/// that change, widow(er) benefits were limited to what the deceased worker
/// was receiving as an OAB. If he/she was not receiving an OAB, there was no
/// limit. He/she could have been receiving a benefit as low as 80 percent of
/// PIA. But if he/she was receiving less than 82.5 percent of PIA, the
/// widow(er) can still get 82.5 percent of PIA, so as not to get less than
/// she/he could have gotten before the law changed.</remarks>
///
/// <param name="widowBen">Widow(er) benefit reduced for widow(er)'s age,
/// but not for RIB-LIM.</param>
/// <param name="oabPia">PIA upon which widow(er) benefit is based.
/// Note that this is not a re-indexed widow(er) PIA, since the OAB
/// must have survived to age 62.</param>
/// <param name="oabBen">Benefit that deceased OAB was receiving, increased
/// by colas up to benefitDate (with rounding applied).</param>
/// <param name="benefitDate">The date of benefit.</param>
double PiaParams::ribLimCalPL( double widowBen, double oabPia, double oabBen,
const DateMoyr& benefitDate )
{
  // before 1973, no RIB-LIM (widow benefit was only 82.5 percent)
  if (benefitDate < amend722) {
    return widowBen;
  }
  // find 82.5 percent of pia
  const double pia825 =
   BenefitAmount::round(factor825 * oabPia, benefitDate.getYear());
  // limit widow benefit to greater of 82.5 percent of pia, or benefit OAB
  // was receiving
  const double maxWidowBen = max(pia825, oabBen);
  return min(widowBen, maxWidowBen);
}

/// <summary>Returns the earliest birthdate for someone to be over NRA at
/// the end of a year.</summary>
///
/// <returns>The earliest birthdate for someone to be over NRA at
/// the end of a year.</returns>
///
/// <param name="year">Year to compute a birthdate for.</param>
boost::gregorian::date PiaParams::birthDateFRA( int year )
{
  if ((unsigned)year < Date::lowyear) {
    throw PiaException("PiaParams::birthDateFRA: year too low");
  }
  if (year < 2003) {
    return boost::gregorian::date((unsigned short)(year-64),1,1);
  }
  if (year > 2008 && year < 2021) {
    return boost::gregorian::date((unsigned short)(year-65),1,1);
  }
  if (year > 2026) {
    return boost::gregorian::date((unsigned short)(year-66),1,1);
  }
  switch (year) {
    case 2003:
      return boost::gregorian::date(1938,11,1);
    break;
    case 2004:
      return boost::gregorian::date(1939,9,1);
    break;
    case 2005:
      return boost::gregorian::date(1940,7,1);
    break;
    case 2006:
      return boost::gregorian::date(1941,5,1);
    break;
    case 2007:
      return boost::gregorian::date(1942,3,1);
    break;
    case 2008:
      return boost::gregorian::date(1943,1,1);
    break;
    case 2021:
      return boost::gregorian::date(1955,11,1);
    break;
    case 2022:
      return boost::gregorian::date(1956,9,1);
    break;
    case 2023:
      return boost::gregorian::date(1957,7,1);
    break;
    case 2024:
      return boost::gregorian::date(1958,5,1);
    break;
    case 2025:
      return boost::gregorian::date(1959,3,1);
    break;
    case 2026:
      return boost::gregorian::date(1960,1,1);
    break;
  }
  return boost::gregorian::date((unsigned short)(year-64),1,1);
}

  /// <summary>Sets benefit formula percents.</summary>
  ///
  /// <param name="eligYear">Year of eligibility.</param>
  /// <param name="percPia">Benefit formula percents.</param>
  void PiaParams::percPiaCal( int eligYear, PercPia& percPia ) const
  { 
    int numPercs = percPiaOut.getNumPercs(eligYear);
    percPia.setNumPerc(numPercs);
    for (int i = 0; i < numPercs; i++) {
      percPia[i] = percPiaOut.getPercPia(eligYear, i + 1);
    }
  }
