// Functions for the <see cref="PiaMethod"/> class - parent of the various
// methods required for calculation of a Social Security benefit.

// $Id: PiaMethod.cpp 1.67 2013/10/29 13:12:47EDT 277133 Development  $

#include <cmath>  // for floor and ceil
#include <numeric>
#include <algorithm>
#include <utility>  // for pair
#include "PiaMethod.h"
#include "oactcnst.h"
#include "UserAssumptions.h"
#include "PiaException.h"
#include "piaparms.h"
#include "DebugCase.h"
#if defined(DEBUGCASE)
#include <sstream>
#include "DateFormatter.h"
#include "Trace.h"
#endif

using namespace std;

/// <summary>Initializes this instance.</summary>
///
/// <param name="newWorkerData">Worker data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
/// <param name="newTitle">Title of this method.</param>
/// <param name="newMethod">Number of this method.</param>
PiaMethod::PiaMethod( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams,
int newMaxyear, const std::string& newTitle, pia_type newMethod ) :
ame(0.0), ameTotal(0.0), cap(0.0), ftearn(0.0), iapp(NOT_APPLICABLE),
indCap(NO_CAP), ioasdi(WorkerDataGeneral::NO_BEN), tableNum(NO_TABLE),
piasub(0.0), methodType(newMethod), mfbsub(0.0), pebsAssumptions(false),
piaEligTotal(), piaTotal(), piaWindfall(), methodTitle(newTitle),
workerData(newWorkerData), piaData(newPiaData), piaParams(newPiaParams),
dirty(false), iorder(YEAR37, newMaxyear), mfbElig(YEAR37, newMaxyear),
mfbEnt(), piaElig(YEAR37, newMaxyear), piaEnt(), earnMultiplied(YEAR37,
newMaxyear), earnIndexed(YEAR37, newMaxyear)
{
  fill_n(yearCpi, (int)MAXIYCPI, 0);
}

/// <summary>Destructor.</summary>
PiaMethod::~PiaMethod()
{ }

/// <summary>Initializes computed values to zero.</summary>
void PiaMethod::initialize()
{
  if (dirty) {
    fill_n(yearCpi, (int)MAXIYCPI, 0);
    iapp = NOT_APPLICABLE;
    indCap = NO_CAP;
    earnIndexed.fill(0.0);
    iorder.fill(0);
    piaEnt.set(0.0);
    ame = cap = 0.0;
    mfbEnt.set(0.0);
    piaElig.fill(0.0);
    mfbElig.fill(0.0);
    dirty = false;
  }
}

/// <summary>Saves pia before application of windfall elimination provision.
/// </summary>
void PiaMethod::setPiaWindfall()
{
  piaWindfall.set(piaElig[yearCpi[FIRST_YEAR]]);
}

/// <summary>Calculates number of years of coverage for special minimum
///   or windfall elimination provision.</summary>
///
/// <remarks>Calculates number of years of coverage for special minimum if
/// method is <see cref="PiaMethod::pia_type::SPEC_MIN"/>; otherwise calculates
/// number of years of coverage for windfall elimination provision. Calls
/// <see cref="specMinYearsCal"/> to do the calculation.</remarks>
///
/// <returns>Number of years of coverage.</returns>
int PiaMethod::specMinYearsTotalCal()
{
  const double pre1951 = piaData.getEarnTotal50(workerData.getTotalize() ?
    PiaData::EARN_WITH_TOTALIZATION : PiaData::EARN_NO_TOTALIZATION);
  const int year = min(workerData.getIend(), piaData.getEarnYear());
  if (methodType == SPEC_MIN) {
    return(specMinYearsCal(pre1951, piaData.earnOasdi, year,
      piaParams.getYocAmountSpecMinArray(), iorder));
  }
  else {
    return(specMinYearsCal(pre1951, piaData.earnOasdi, year,
      piaParams.getYocAmountWindfallArray()));
  }
}

/// <summary>Calculates number of years of coverage, using passed argument
/// for amounts required for coverage.</summary>
///
/// <returns>Number of years of coverage.</returns>
///
/// <remarks>This version does not save indicators for years used.</remarks>
///
/// <param name="pre1951">Pre-1951 earnings.</param>
/// <param name="earnings">Annual earnings, starting in 1951.</param>
/// <param name="year">Last year of earnings considered.</param>
/// <param name="yocAmount">Amount required for year of coverage.</param>
int PiaMethod::specMinYearsCal( double pre1951, const DoubleAnnual& earnings,
int year, const DoubleAnnual& yocAmount )
{
  int totyears = min(14, static_cast<int>(floor(pre1951 / 900.0)));
  // count years after 1950
  for (int year1 = YEAR51; year1 <= year; year1++) {
    if (earnings[year1] > yocAmount[year1] - 0.009) {
      totyears++;
    }
  }
  return(totyears);
}

/// <summary>Calculates number of years of coverage, using passed argument
/// for amounts required for coverage.</summary>
///
/// <returns>Number of years of coverage.</returns>
///
/// <remarks>This version saves indicators for years used in the iordert
/// argument.</remarks>
///
/// <param name="pre1951">Pre-1951 earnings.</param>
/// <param name="earnings">Annual earnings, starting in 1951.</param>
/// <param name="year">Last year of earnings considered.</param>
/// <param name="yocAmount">Amount required for year of coverage.</param>
/// <param name="iordert">1 if year of coverage, 0 otherwise, for years
/// after 1950; total pre-1951 years of coverage for 1950.</param>
int PiaMethod::specMinYearsCal( double pre1951, const DoubleAnnual& earnings,
int year, const DoubleAnnual& yocAmount, IntAnnual& iordert )
{
  int totyears = min(14, static_cast<int>(floor(pre1951 / 900.0)));
  iordert[YEAR50] = totyears;
  // count years after 1950
  for (int year1 = YEAR51; year1 <= year; year1++) {
    if (earnings[year1] > yocAmount[year1] - 0.009) {
      iordert[year1] = 1;
      totyears++;
    }
  }
  return(totyears);
}

/// <summary>Saves pia before application of totalization provision.</summary>
void PiaMethod::setPiaTotal()
{
  piaTotal = piaEnt;
}

/// <summary>Saves pia at eligibility before application of totalization
/// provision.</summary>
void PiaMethod::setPiaEligTotal()
{
  piaEligTotal.set(piaElig[yearCpi[FIRST_YEAR]]);
}

/// <summary>Saves aime before application of totalization provision.
/// </summary>
void PiaMethod::setAmeTotal()
{
  ameTotal = getAme();
}

/// <summary>Calculates portion of pia at eligibility in each section of
/// mfb formula.</summary>
///
/// <param name="piasub">Pia to portion.</param>
/// <param name="piapart">Parts of the pia.</param>
/// <param name="bendMfbTemp">Bend points to use.</param>
void PiaMethod::setPortionPiaElig( double piasub, double piapart[],
const BendMfb& bendMfbTemp )
{
  // calculate portion of PIA in each interval
  piapart[0] = min(piasub, bendMfbTemp[1]);
  piapart[1] = max(0.0, min(piasub - bendMfbTemp[1],
    bendMfbTemp[2] - bendMfbTemp[1]));
  piapart[2] = max(0.0, min(piasub - bendMfbTemp[2],
    bendMfbTemp[3] - bendMfbTemp[2]));
  piapart[3] = max(piasub - bendMfbTemp[3], 0.0);
}

/// <summary>Sets years of benefit increases.</summary>
void PiaMethod::setYearCpi()
{
  int year = workerData.getBenefitDate().getYear();
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", BenefitDate().getYear() " << year
      << ": Setting yearCpi[YEAR_BEN] in PiaMethod::setYearCpi";
    Trace::writeLine(strm.str());
  }
#endif
  yearCpi[YEAR_BEN] =
    (static_cast<int>(workerData.getBenefitDate().getMonth()) <
    piaParams.getMonthBeninc(year)) ? year - 1 : year;
  DateMoyr dateMoyr = (ioasdi == WorkerDataGeneral::SURVIVOR) ?
    DateMoyr(workerData.getDeathDate()) : workerData.getEntDate();
  year = dateMoyr.getYear();
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", ioasdi "
      << static_cast<int>(ioasdi);
    if (ioasdi == WorkerDataGeneral::SURVIVOR) {
      strm << ", DeathDate().getYear() " << year;
    }
    else {
      strm << ", EntDate().getYear() " << year;
    }
    strm << ": Setting yearCpi[YEAR_ENT] in PiaMethod::setYearCpi";
    Trace::writeLine(strm.str());
  }
#endif
  yearCpi[YEAR_ENT] = (static_cast<int>(dateMoyr.getMonth()) <
    piaParams.getMonthBeninc(year)) ? year - 1 : year;
}

/// <summary>Applies benefit increases to 1977 Amendments PIA or MFB.
/// </summary>
///
/// <returns>PIA or MFB increased by benefit increases.</returns>
///
/// <remarks>The first benefit increase applied is for year year1; it is
/// applied to pia77 at index (year1 - 1). The values are calculated and
/// saved in pia77 sequentially.</remarks>
///
/// <param name="pia77">PIA or MFB to be increased.</param>
/// <param name="year1">First year of benefit increases.</param>
/// <param name="dateMoyr2">The date up to which to apply benefit increases.
/// </param>
/// <param name="catchupYear">Year of eligibility for catch-up benefit
///  increases.</param>
double PiaMethod::applyColasElig( DoubleAnnual& pia77, int year1,
 const DateMoyr& dateMoyr2, int catchupYear ) const
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", year1 " << year1
      << ", dateMoyr2 " << DateFormatter::toString(dateMoyr2)
      << ", catchupYear " << catchupYear
      << ": At top of PiaMethod::applyColas";
    Trace::writeLine(strm.str());
  }
#endif
  // do not apply any increases prior to 1979
  if (year1 < YEAR79) {
    return(pia77[year1]);
  }
  const int year2 = dateMoyr2.getYear();
  // if year of benefit equals year of eligibility, and benefit date is
  // prior to benefit increase month, there are no CPI increases applied
  const int year3 = (static_cast< int >(dateMoyr2.getMonth()) <
    piaParams.getMonthBeninc(year2)) ? year2 - 1 : year2;
  for (int year = year1; year <= year3; year++) {
    // check for special 1999 increase (with extra 0.1 percent)
    if (piaParams.isApplicableCola99(year, dateMoyr2)) {
      // apply 1999 special increase
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        Trace::writeLine(workerData.getIdString() +
          ": Calling piaParams.applyCola99");
      }
#endif
      pia77[year] = piaParams.applyCola99(pia77[year-1]);
    }
    else {
#if defined(DEBUGCASE)
      if (isDebugPid(workerData.getIdNumber())) {
        ostringstream strm;
        strm << workerData.getIdString() << ", cola for year " << year
          << ": Calling piaParams.applyCola";
        Trace::writeLine(strm.str());
      }
#endif
      pia77[year] = piaParams.applyCola(pia77[year-1], year, catchupYear);
    }
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm.setf(ios::fixed, ios::floatfield);
      strm.precision(2);
      strm << workerData.getIdString() << ", amount with cola "
        << pia77[year]
        << ": After calling piaParams.applyCola or applyCola99";
      Trace::writeLine(strm.str());
    }
#endif
  }
  return(pia77[year3]);
}

/// <summary>Orders earnings and sets indicator for the highest years.
/// </summary>
///
/// <remarks>The number of years selected could be less than the desired
/// number, if there are not that many years of earnings.</remarks>
///
/// <param name="first">First year of earnings.</param>
/// <param name="last">Last year of earnings.</param>
/// <param name="number">Number of years to be selected.</param>
void PiaMethod::orderEarnings( int first, int last, int number )
{
  vector< pair< double, int > > wearn;  // earnings array
  const int numtosort = last - first + 1;
  if (numtosort > 0) {
    for (int i1 = 0; i1 < numtosort; i1++) {  // initialize array
      wearn.push_back(make_pair(earnIndexed[first + i1], first + i1));
    }
    sort(wearn.begin(),wearn.end());
    const int i2 = max(numtosort - number, 0);
    for (int i1 = i2; i1 < numtosort; i1++) {
      iorder[wearn[i1].second] = 1;
    }
  }
}

/// <summary>Computes total earnings and AME or AIME.</summary>
///
/// <remarks> Highest earning years must already be selected.  If there are
/// not enough selected years, years of zero earnings will be added.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_METHOD"/> if there is an inconsistency in the number of
/// years desired and the ordered earnings (only in debug mode).</exception>
///
/// <param name="first">First year of earnings.</param>
/// <param name="last">Last year of earnings.</param>
/// <param name="number">Number of years to be selected.</param>
void PiaMethod::totalEarnCal( int first, int last, int number )
{
  double tearn = 0.0;  // sum of high n years of earnings
  int count = 0;  // number of years used in sum
  for (int i1 = first; i1 <= last; i1++) {
    if (iorder[i1] == 1) {
      tearn += earnIndexed[i1];
      count++;
    }
  }
#if !defined(NDEBUG)
  if (count <= 0 || count > number)
    throw PiaException(PIA_IDS_METHOD);
#endif
  setftearn(tearn);
  setAme(floor(tearn / (static_cast<double>(number) * 12.0)));
}

/// <summary>Calculates pro rata totalization benefit.</summary>
void PiaMethod::prorate()
{
  setPiaEligTotal();
  setPiaTotal();
  const double factor = piaData.getQcTotalRel() /
    static_cast<double>(4 * piaData.compPeriodNew.getN());
  const double piat = BenefitAmount::round(factor * piaEnt.get(),
    yearCpi[YEAR_BEN]);
  piaEnt.set(piat);
  const int numYears = yearCpi[YEAR_BEN] - yearCpi[FIRST_YEAR];
  piaElig[yearCpi[FIRST_YEAR]] = piaParams.deconvertPia(
    piaData.getEligYear(), numYears, piaEnt.get(),
    workerData.getBenefitDate());
}

/// <summary>Calculates an MFB at eligibility under the 1977 law.</summary>
///
/// <returns>MFB at eligibility.</returns>
///
/// <param name="portionPiaElig">Portion of mfb in each formula interval.
/// </param>
/// <param name="percMfbTemp">The four mfb formula percentages.</param>
/// <param name="year">Year to use for rounding.</param>
double PiaMethod::mfbCal( const double portionPiaElig[],
const PercMfb& percMfbTemp, int year )
{
  //  multiply percent by PIA portion and add
  const double temp = inner_product(percMfbTemp.begin(), percMfbTemp.end(),
    portionPiaElig, 0.0);
  return BenefitAmount::round(temp, year);
}

/// <summary>Applies DI maximum provision, including COLAs to date of
/// benefit.</summary>
///
/// <returns>MFB from DI maximum provision, including COLAs to date of
/// benefit.</returns>
///
/// <param name="amedimax">Ame for DI maximum calculation.</param>
/// <param name="piamax">Pia at date of benefit.</param>
double PiaMethod::diMax( double amedimax, double piamax )
{
  double capt;  // percentage used in calculation
  dis_mfb_cap_type icapt;  // indicator for type of cap
  const int year = piaData.getEligYear() - 1;  // year of initial benefit
  mfbElig[year] = dimaxElig(amedimax, piaElig[year], year, capt, icapt);
  setCap(capt);
  setIndCap(icapt);
  return(max(applyColas(mfbElig, piaData.getEligYear(),
    workerData.getBenefitDate()), piamax));
}

/// <summary>Applies DI maximum provision for non-freeze, including COLAs to date of
/// benefit.</summary>
///
/// <returns>MFB from DI maximum provision for non-freeze, including COLAs to date of
/// benefit.</returns>
///
/// <param name="amedimax">Ame for DI maximum calculation.</param>
/// <param name="piamax">Pia at date of benefit.</param>
double PiaMethod::diMaxNonFreeze( double amedimax, double piamax )
{
  double capt;  // percentage used in calculation
  dis_mfb_cap_type icapt;  // indicator for type of cap
  const int year = piaData.getEligYearNonFreeze() - 1;  // year of initial benefit
  mfbElig[year] = dimaxElig(amedimax, piaElig[year], year, capt, icapt);
  setCap(capt);
  setIndCap(icapt);
  return(max(applyColas(mfbElig, piaData.getEligYearNonFreeze(),
    workerData.getBenefitDate()), piamax));
}

/// <summary>Applies DI maximum provision as of year of eligibility.</summary>
///
/// <returns>MFB at eligibility.</returns>
///
/// <param name="amedimax">Aime for DI maximum calculation.</param>
/// <param name="piaelig">Pia as of year of eligibility.</param>
/// <param name="year">Year prior to year of eligibility.</param>
/// <param name="capt">Percentage used in calculation (1.5, 0.85, or 1.0).
/// </param>
/// <param name="icapt">Type of calculation (returned).</param>
double PiaMethod::dimaxElig( double amedimax, double piaelig,
int year, double& capt, dis_mfb_cap_type& icapt )
{
  double mfbelig = BenefitAmount::round(0.85 * amedimax, year);
  double mfbelt = BenefitAmount::round(1.5 * piaelig, year);
  if (mfbelig > mfbelt) {
    // cap is 150% of PIA
    capt = 1.5;
    icapt = PIA_1_5;
    mfbelig = mfbelt;
  }
  else if (mfbelig < piaelig) {
    // MFB is at floor of 100% of PIA
    capt = 1.0;
    icapt = PIA_1_0;
    mfbelig = piaelig;
  }
  else {
    // cap is 85% of AIME
    capt = 0.85;
    icapt = AIME_85;
  }
  return(mfbelig);
}

/// <summary>Returns windfall indicator.</summary>
///
/// <returns><see cref="WindfallElimType::NOWINDFALLELIM"/>. This
/// could be overridden in a child class.</returns>
PiaMethod::WindfallElimType PiaMethod::getWindfall() const
{
  return(NOWINDFALLELIM);
}

/// <summary>Returns number of years of coverage for windfall elimination
/// provision.</summary>
///
/// <returns>0. This could be overridden in a child class.</returns>
int PiaMethod::getYearsTotal() const
{
  return(0);
}

/// <summary>Determines if windfall elimination provision applies.</summary>
///
/// <remarks>The windfall elimination provision applies to old-age and
/// disability benefits, not survivor benefits. It also applies only if the
/// year of eligibility is 1986 or later, and if there is a noncovered
/// pension with an entitlement date after 1985.</remarks>
///
/// <returns>True if windfall elimination provision applies, false otherwise.
/// </returns>
bool PiaMethod::wep_app() const
{
  if (piaData.getEligYear() > 1985 && ioasdi != WorkerDataGeneral::SURVIVOR
    && (workerData.getBenefitDate() >= workerData.getPubpenDate())) {
    return(piaData.getPubpenApp(workerData) > 0.0f &&
      (workerData.getPubpenDate().getYear() == 0 ||
      workerData.getPubpenDate().getYear() > 1985));
  }
  else
    return(false);
}
