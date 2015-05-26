// Functions for the <see cref="PiaParamsPL"/> class of pia calculation
// parameters using present law only.

// $Id: piaparmsPL.cpp 1.44 2011/08/05 16:30:20EDT 044579 Development  $

#include <cfloat>  // for FLT_MAX
#include "piaparmsPL.h"
#include "WageBase.h"
#include "oactcnst.h"
#include "AwbiDataNonFile.h"

using namespace std;

/// <summary>Initializes arrays with a specified starting year.</summary>
///
/// <param name="newIstart">Year after last known benefit increase.</param>
/// <param name="newMaxyear">Maximum projected year.</param>
/// <param name="awbiData">Structure with historical amounts.</param>
PiaParamsPL::PiaParamsPL( int newIstart, int newMaxyear,
  AwbiData& awbiData ) :
PiaParams(newIstart, newMaxyear), baseOasdi(newMaxyear), base77(newMaxyear)
{
  setData(awbiData);
  initdata();
}

/// <summary>Initializes arrays without a specified starting year.</summary>
///
/// <remarks>This version without a specified starting year uses
/// <see cref="BaseYearNonFile::YEAR"/>, and constructs an
/// <see cref="AwbiDataNonFile"/> using that year.</remarks>
///
/// <param name="newMaxyear">Maximum projected year.</param>
PiaParamsPL::PiaParamsPL( int newMaxyear ) :
PiaParams(newMaxyear), baseOasdi(newMaxyear), base77(newMaxyear)
{
  AwbiDataNonFile awbiData(newMaxyear);
  setData(awbiData);
  initdata();
}

/// <summary>Destroys arrays.</summary>
PiaParamsPL::~PiaParamsPL( )
{ }

/// <summary>Returns the amount of the cap on the COLA for the given year.
/// </summary>
///
/// <returns>Maximum float value.</returns>
double PiaParamsPL::getColaCap( int ) const
{
  // No cap on the COLA, it can be as big as possible.
  return FLT_MAX;
}

/// <summary>Returns true if a cola cap is required at any time.</summary>
///
/// <returns>Always returns false.</returns>
bool PiaParamsPL::needColaCap() const
{
  return false;
}

/// <summary>Returns true if a cola cap is required for a specified year.
/// </summary>
///
/// <returns>Always returns false.</returns>
bool PiaParamsPL::needColaCap( int ) const
{
  return false;
}

/// <summary>Sets the amount of the cap on the COLA for the given year.
/// </summary>
void PiaParamsPL::setColaCap( double, int )
{ }

/// <summary>Returns the amount of the cap on the COLA for the MFB
/// for the given year.</summary>
///
/// <returns>Maximum float value.</returns>
double PiaParamsPL::getMfbColaCap( int ) const
{
  // No cap on the COLA, it can be as big as possible.
  return FLT_MAX;
}

/// <summary>Sets the amount of the cap on the COLA for the MFB
/// for the given year.</summary>
void PiaParamsPL::setMfbColaCap( double, int )
{ }

/// <summary>Returns series of average wages used for indexing pia formula
/// bend points.</summary>
///
/// <returns>Series of average wages used for indexing pia formula bend
/// points.</returns>
const AverageWage& PiaParamsPL::getFqBppia() const
{
  return(fq);
}

/// <summary>Returns series of average wages used for indexing an earnings
/// record.</summary>
///
/// <returns>Series of average wages used for indexing an earnings record.
/// </returns>
const AverageWage& PiaParamsPL::getFqIndex() const
{
  return(fq);
}

/// <summary>Returns maximum amount of earnings in a childcare dropout year.
/// </summary>
///
/// <returns>0 for present law.</returns>
double PiaParamsPL::getChildcareDropoutAmount( int, int ) const
{
  return getChildcareDropoutAmountPL();
}

/// <summary>Returns maximum number of childcare dropout years.</summary>
///
/// <returns>3 for present law.</returns>
int PiaParamsPL::getMaxChildcareDropoutYears( int, int ) const
{
  return getMaxChildcareDropoutYearsPL();
}

/// <summary>Returns maximum number of usable years for special minimum.
/// </summary>
///
/// <remarks>This version (with no arguments) returns the maximum number that
/// could ever be for any year of eligibility and year of benefit.</remarks>
///
/// <returns>Maximum number of usable years for special minimum.</returns>
int PiaParamsPL::specMinMaxYears() const
{
  return specMinMaxYearsPL();
}

/// <summary>Returns maximum number of usable years for special minimum for
/// a specified year of eligibility and year of benefit.</summary>
///
/// <returns>Maximum number of usable years for special minimum.</returns>
int PiaParamsPL::specMinMaxYears( int, int ) const
{
  return specMinMaxYearsPL();
}

/// <summary>Returns amount per year of coverage in special minimum.
/// </summary>
///
/// <returns>Amount per year of coverage in special minimum.</returns>
///
/// <param name="date">The date of desired amount.</param>
double PiaParamsPL::specMinAmountCal( const DateMoyr& date ) const
{
  return specMinAmountCalPL(date);
}

/// <summary>Returns maximum DI beneficiary age at end of year.</summary>
///
/// <returns> Maximum DI beneficiary age at end of year, based on present
/// law.</returns>
///
/// <param name="year">Year of benefit.</param>
Age PiaParamsPL::maxDibAge( int year ) const
{
   return maxDibAgePL(year);
}

/// <summary>Sets earliest possible retirement age for old-age benie or aged
/// spouse.</summary>
///
/// <returns>Earliest possible retirement age, based on present law.
/// </returns>
///
/// <param name="sex">The sex of number holder
/// (<see cref="Sex::sex_type::MALE"/> for male,
/// <see cref="Sex::sex_type::FEMALE"/> for female).</param>
/// <param name="kbirth">The date of birth, adjusted to previous day.</param>
Age PiaParamsPL::earlyAgeOabCal( Sex::sex_type sex,
const boost::gregorian::date& kbirth ) const
{
  return(earlyAgeOabCalPL(sex, kbirth));
}

/// <summary>Returns factor for wife/husband benefit before age reduction.
/// </summary>
///
/// <returns>Factor for wife/husband benefit before age reduction.</returns>
double PiaParamsPL::factorAgedSpouseCal( int, int ) const
{
  return factorAgedSpouseCalPL();
}

/// <summary>Returns full retirement age.</summary>
///
/// <returns>Full retirement age based on present law.</returns>
///
/// <param name="eligYear">Year of eligibility (age 62 for oab or
/// wife/husband, 60 for widow(er)).</param>
Age PiaParamsPL::fullRetAgeCal( int eligYear ) const
{
  return fullRetAgeCalPL(eligYear);
}

/// <summary>Returns factor for widow/widower benefit before age reduction.
/// </summary>
///
/// <returns>Factor for widow/widower benefit before age reduction.</returns>
///
/// <remarks>Returns <see cref="factorAgedWidCalPL"/>.</remarks>
///
/// <param name="monthsArdri">Months of reduction.</param>
/// <param name="age">The age of widow/widower.</param>
/// <param name="benefitDate">The date of benefit.</param>
double PiaParamsPL::factorAgedWidCal( int monthsArdri, const Age& age,
  int, const DateMoyr& benefitDate ) const
{
  return factorAgedWidCalPL(monthsArdri, age, benefitDate);
}

/// <summary>Returns factor for disabled widow/widower benefit before age
/// reduction.</summary>
///
/// <returns>Factor for disabled widow/widower benefit before age reduction.
/// </returns>
///
/// <remarks>Returns <see cref="factorDisWidCalPL"/>.</remarks>
///
/// <param name="benefitDate">The date of benefit.</param>
double PiaParamsPL::factorDisWidCal( int, const DateMoyr& benefitDate ) const
{
  return factorDisWidCalPL(benefitDate);
}

/// <summary>Returns reduction factor for wife/husband.</summary>
///
/// <returns>Reduction factor for wife/husband.</returns>
///
/// <param name="monthsArdri">Number of months of reduction.</param>
double PiaParamsPL::factorArAgedSpouseCal( int monthsArdri ) const
{
  return factorArAgedSpouseCalPL(monthsArdri);
}

/// <summary>Calculates reduction factor for OAB.</summary>
///
/// <returns>Reduction factor for OAB.</returns>
///
/// <param name="monthsArdri">Number of months of reduction.</param>
double PiaParamsPL::factorArCal( int monthsArdri ) const
{
  return factorArCalPL(monthsArdri);
}

/// <summary>Returns full retirement age for benefit calculations involving
/// disability freeze.</summary>
///
/// <returns>Full retirement age for benefit calculations involving
/// disability freeze.</returns>
///
/// <param name="eligYear">Year of eligibility (age 62 for oab or
/// wife/husband, 60 for widow(er)).</param>
/// <param name="currentYear">Current year.</param>
Age PiaParamsPL::fullRetAgeCalDI( int eligYear, int currentYear ) const
{
  return fullRetAgeCalDIPL(eligYear, currentYear);
}

/// <summary>Projects amounts dependent on benefit increases.</summary>
void PiaParamsPL::projectCpiinc()
{
  resizeSpecMin();
  projectSpecMin(YEAR79, maxyear);
}

/// <summary>Updates wage bases for average wage changes.</summary>
void PiaParamsPL::updateBases()
{
  const int firstYeart = getIstart() + 1;
  // project present-law OASDI bases
  baseOasdi.project(fq, getCpiincArray(), firstYeart,
    baseOasdi.getLastYear());
  // project HI bases
  baseHi.project(fq, getCpiincArray(), firstYeart, baseHi.getLastYear());
  // project old-law OASDI bases
  base77.project(fq, getCpiincArray(), firstYeart, base77.getLastYear());
}


/// <param name="userbase">User-entered bases.</param>
/// <param name="userbase77">User-entered old-law bases.</param>
/// <param name="ibasch">Indicator for base changes.</param>
/// <param name="firstYeart">First year to update.</param>
/// <param name="lastYeart">Last year to update.</param>
void PiaParamsPL::updateBases( const DoubleAnnual& userbase,
const DoubleAnnual& userbase77, BaseChangeType::change_type ibasch,
int firstYeart, int lastYeart )
{
  if (ibasch == BaseChangeType::ADHOC) {
    // set ad hoc OASDI bases
    baseOasdi.assign(userbase, firstYeart, lastYeart);
    // project OASDI bases
    baseOasdi.project(fq, getCpiincArray(), lastYeart + 1,
      baseOasdi.getLastYear());
    // set ad hoc old-law bases
    base77.assign(userbase77, firstYeart, lastYeart);
    // project old-law OASDI bases
    base77.project(fq, getCpiincArray(), lastYeart + 1,
      base77.getLastYear());
    // HI bases are always set automatically
    baseHi.project(fq, getCpiincArray(), getIstart() + 1,
      baseHi.getLastYear());
  }
  else {
    updateBases();
  }
}

/// <summary>Returns maximum age of child for childcare dropout year.
/// </summary>
///
/// <returns>3.</returns>
int PiaParamsPL::getMaxChildcareAge() const
{
  return getMaxChildcareAgePL();
}

/// <summary>Sets data from historical average wages and benefit increase
/// file.</summary>
///
/// <param name="awbiData">Structure with historical average wages and
/// benefit increases.</param>
void PiaParamsPL::setData( AwbiData& awbiData )
{
  PiaParams::setData(awbiData);
  const int year = awbiData.getLastYear();
  baseOasdi.assign(awbiData.baseOasdi, YEAR37, year);
  base77.assign(awbiData.base77, YEAR37, year);
}

/// <summary>Returns the number of years of marriage needed to be eligible
/// for a divorced spouse benefit.</summary>
///
/// <param name="dateMoyr">The date (year and month) when checking if
/// eligible for divorced spouse benefit.</param>
///
/// <returns>Number of years of marriage needed to be eligible
/// for a divorced spouse benefit.</returns>
int PiaParamsPL::getMarrLengthForDivBen( const DateMoyr& dateMoyr ) const
{
  return getMarrLengthForDivBenPL(dateMoyr);
}

/// <summary>Returns widow(er) benefit reduced for RIB-LIM.</summary>
///
/// <returns>Reduced widow(er) benefit.</returns>
///
/// <remarks>This should only be called in cases where the deceased worker
/// was an OAB before death.</remarks>
///
/// <param name="widowBen">Widow(er) benefit reduced for widow(er)'s age,
/// but not for RIB-LIM.</param>
/// <param name="oabPia">PIA upon which widow(er) benefit is based. Note that
/// this is not a re-indexed widow(er) PIA, since the OAB must have survived
/// to age 62.</param>
/// <param name="oabBen">Benefit that deceased OAB was receiving.</param>
/// <param name="benefitDate">The date of benefit.</param>
double PiaParamsPL::ribLimCal( double widowBen, double oabPia, double oabBen,
const DateMoyr& benefitDate ) const
{
  return ribLimCalPL(widowBen, oabPia, oabBen, benefitDate);
}
