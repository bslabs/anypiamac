// Functions for the <see cref="PiaPageOut"/> class - parent of classes to
// write out results of a non-Statement benefit calculation.

// $Id: PiaPageOut.cpp 1.39.1.4 2017/10/12 13:07:40EDT 277133 Development  $

#include "PiaPageOut.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "piaparms.h"
#include "WageInd.h"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/local_time/local_time.hpp"
#include "DateFormatter.h"

using namespace std;

// <summary>Years of Social Security Acts.</summary>
const char *PiaPageOut::tableLabel[10] = {
  "1952", "1954", "1958", "1965", "1967", "1969", "1971", "1972", "1973",
  "1977"
};
// <summary>Warning for totalization benefit (less than 6 qcs, or
//  insured).</summary>
const std::string PiaPageOut::warnTotalization = string(
  "Hypothetical totalization benefit!!!");

/// <summary>Initializes PiaPageOut.</summary>
PiaPageOut::PiaPageOut() : PageOut()
{ }

/// <summary>Destructor.</summary>
PiaPageOut::~PiaPageOut()
{ }

/// <summary>Prepares number of computation years in a string table.</summary>
///
/// <param name="workerData">Worker basic data.</param>
/// <param name="piaData">Pia calculation data.</param>
/// <param name="compPeriod">Computation period (old or new).</param>
void PiaPageOut::compYear( const WorkerDataGeneral& workerData,
const PiaData& piaData, const CompPeriod& compPeriod )
{
  if (workerData.getValdi() > 0) {
    if (piaData.freezeYears.hasYears1() || piaData.freezeYears.hasYears2()) {
      outputString.push_back("Years wholly within disability freeze:");
      strm.str("");
      strm << "   ";
      if (piaData.freezeYears.hasYears2()) {
        strm << PunctFacet::getYear(piaData.freezeYears.getYear3()) << " to "
          << PunctFacet::getYear(piaData.freezeYears.getYear4());
      }
      if (piaData.freezeYears.hasYears1() &&
        piaData.freezeYears.hasYears2()) {
        strm << " and ";
      }
      if (piaData.freezeYears.hasYears1()) {
        strm << PunctFacet::getYear(piaData.freezeYears.getYear1()) << " to "
        << PunctFacet::getYear(piaData.freezeYears.getYear2());
      }
      outputString.push_back(strm.str());
    }
    else {
      outputString.push_back("No years wholly within disability freeze");
    }
    outputString.push_back("");
    if (piaData.partialFreezeYears.hasYears1() ||
      piaData.partialFreezeYears.hasYears2()) {
      outputString.push_back("Years partially within disability freeze:");
      strm.str("");
      strm << "   ";
      if (piaData.partialFreezeYears.hasYears2()) {
        strm << PunctFacet::getYear(piaData.partialFreezeYears.getYear3())
          << " to "
          << PunctFacet::getYear(piaData.partialFreezeYears.getYear4());
      }
      if (piaData.partialFreezeYears.hasYears1() &&
        piaData.partialFreezeYears.hasYears2()) {
        strm << " and ";
      }
      if (piaData.partialFreezeYears.hasYears1()) {
        strm << PunctFacet::getYear(piaData.partialFreezeYears.getYear1())
          << " to "          
          << PunctFacet::getYear(piaData.partialFreezeYears.getYear2());
      }
      outputString.push_back(strm.str());
    }
    else {
      outputString.push_back("No years partially within disability freeze");
    }
    outputString.push_back("");
  }
  strm.str("");
  strm << "Number of di dropout years = " << compPeriod.getDiYears();
  outputString.push_back(strm.str());
  outputString.push_back("");
  strm.str("");
  strm << "Number of elapsed years = " << compPeriod.getNElapsed();
  outputString.push_back(strm.str());
  outputString.push_back("");
  strm.str("");
  strm << "Number of dropout years = " << compPeriod.getNDrop();
  outputString.push_back(strm.str());
  outputString.push_back("");
  strm.str("");
  strm << "Number of computation years = " << compPeriod.getNElapsed()
    << " - " << compPeriod.getNDrop() << " = " << compPeriod.getN();
  outputString.push_back(strm.str());
  outputString.push_back("");
}

/// <summary>Prepares benefit increases used in PIA calculation in a string
/// table.</summary>
///
/// <remarks>This version is passed an int which determines the starting and
/// ending years. The type of writeout is one of the following:
/// <table>
///     0   to go from first year to entitlement.
///     1   to go from eligibility to entitlement.
///     2   to go from entitlement to benefit date.
///     3   to go from first year to benefit date.
/// </table>
/// The indicator for PIA and MFB printout is one of the following:
/// <table>
///     0   to not write out PIA or MFB.
///     1   for PIA.
///     2   for MFB.
///     3   for PIA and MFB.
/// </table></remarks>
///
/// <param name="piaMethod">Pia calculation method.</param>
/// <param name="piaParams">Pia calculation parameters.</param>
/// <param name="eligYear">Year of eligibility for catch-up benefit increases.
/// </param>
/// <param name="type">Type of writeout.</param>
/// <param name="piamfb">Indicator for PIA and MFB printout.</param>
void PiaPageOut::biOut( const PiaMethod& piaMethod,
const PiaParams& piaParams, int eligYear, int type, int piamfb )
{
#ifndef NDEBUG
  if (eligYear < YEAR37)
    throw PiaException(PIA_IDS_WISTRING);
#endif
  const PiaMethod::cpi_year_type cpiYear1 = firstYearCal(type);
  const PiaMethod::cpi_year_type cpiYear2 = endYearCal(type);
  outputString.push_back("CPI increases applied:");
  const int cpiYear3 = (type == 2) ? piaMethod.getCpiYear(cpiYear1) + 1 :
    piaMethod.getCpiYear(cpiYear1);
#ifndef NDEBUG
  if (cpiYear3 < YEAR37)
    throw PiaException(PIA_IDS_WISTRING);
#endif
  for (int yr = cpiYear3; yr <= piaMethod.getCpiYear(cpiYear2); yr++) {
    strm.precision(1);
    if (piaParams.getMonthBeninc(yr) > 0) {
      DateMoyr datetemp(piaParams.getMonthBeninc(yr), yr);
      if (piaParams.getCatchup().exist(eligYear, yr)) {
        strm.str("");
        strm << "   " << setw(4) <<piaParams.getCatchup().get(eligYear, yr)
          << " % for " << DateFormatter::toString(datetemp) << ", catch-up";
        outputString.push_back(strm.str());
      }
      strm.str("");
      strm << "   " << setw(4) << piaParams.getCpiinc(yr) << " % for "
        << DateFormatter::toString(datetemp);
      if (piamfb != 0) {
        strm.precision(2);
        strm << ":  ";
        if (piamfb % 2 == 1) {
          strm << setw(8) << piaMethod.piaElig[yr];
        }
        if (piamfb > 1) {
          strm << "  " << setw(8) << piaMethod.mfbElig[yr];
        }
      }
      outputString.push_back(strm.str());
    }
  }
  outputString.push_back("");
}

/// <summary>Prepares benefit increases used in PIA calculation in a string
/// table.</summary>
///
/// <remarks>This version is passed the starting and ending years.
/// The indicator for PIA and MFB printout is one of the following:
/// <table>
///     0   to not write out PIA or MFB.
///     1   for PIA.
///     2   for MFB.
///     3   for PIA and MFB.
/// </table></remarks>
///
/// <param name="piaMethod">Pia calculation method.</param>
/// <param name="piaParams">Pia calculation parameters.</param>
/// <param name="eligYear">Year of eligibility for catch-up benefit increases.
/// </param>
/// <param name="firstYear">First year to write increases.</param>
/// <param name="lastYear">Last year to write increases.</param>
/// <param name="piamfb">Indicator for PIA and MFB printout.</param>
void PiaPageOut::biOut( const PiaMethod& piaMethod,
const PiaParams& piaParams, int eligYear, int firstYear, int lastYear,
int piamfb )
{
#ifndef NDEBUG
  if (eligYear < YEAR37)
    throw PiaException(PIA_IDS_WISTRING);
#endif
  outputString.push_back("CPI increases applied:");
  for (int yr = firstYear; yr <= lastYear; yr++) {
    strm.precision(1);
    if (piaParams.getMonthBeninc(yr) > 0) {
      DateMoyr datetemp(piaParams.getMonthBeninc(yr), yr);
      if (piaParams.getCatchup().exist(eligYear, yr)) {
        strm.str("");
        strm << "   " << setw(4) << piaParams.getCatchup().get(eligYear, yr)
          << " % for " << DateFormatter::toString(datetemp) << ", catch-up";
        outputString.push_back(strm.str());
      }
      strm.str("");
      strm << "   " << setw(4) << piaParams.getCpiinc(yr) << " % for "
        << DateFormatter::toString(datetemp);
      if (piamfb != 0) {
        strm.precision(2);
        strm << ":  ";
        if (piamfb % 2 == 1) {
          strm << setw(8) << piaMethod.piaElig[yr];
        }
        if (piamfb > 1) {
          strm << "  " << setw(8) << piaMethod.mfbElig[yr];
        }
      }
      outputString.push_back(strm.str());
    }
  }
  outputString.push_back("");
}

/// <summary>Calculates the first year type for printing a range of benefit
/// increases.</summary>
///
/// <remarks>The type of writeout is one of the following:
/// <table>
///     0   to go from first year to entitlement.
///     1   to go from eligibility to entitlement.
///     2   to go from entitlement to benefit date.
///     3   to go from first year to benefit date.
/// </table></remarks>
///
/// <returns>The ending year type.</returns>
///
/// <param name="type">Type of writeout.</param>
PiaMethod::cpi_year_type PiaPageOut::firstYearCal( int type )
{
  switch (type) {
  case 0:
    return PiaMethod::FIRST_YEAR;
  case 1:
    return PiaMethod::YEAR_ELIG;
  case 2:
    return PiaMethod::YEAR_ENT;
  case 3:
    return PiaMethod::FIRST_YEAR;
  default:
    throw PiaException("PiaPageOut::endYearCal: bad value for type");
  }
}

/// <summary>Calculates the ending year type for printing a range of benefit
/// increases.</summary>
///
/// <remarks>The type of writeout is one of the following:
/// <table>
///     0   to go from first year to entitlement.
///     1   to go from eligibility to entitlement.
///     2   to go from entitlement to benefit date.
///     3   to go from first year to benefit date.
/// </table></remarks>
///
/// <returns>The ending year type.</returns>
///
/// <param name="type">Type of writeout.</param>
PiaMethod::cpi_year_type PiaPageOut::endYearCal( int type )
{
  switch (type) {
  case 0:
  case 1:
    return PiaMethod::YEAR_ENT;
  case 2:
  case 3:
    return PiaMethod::YEAR_BEN;
  default:
    throw PiaException("PiaPageOut::endYearCal: bad value for type");
  }
}

/// <summary>Prepares table change automatic increases used in PIA
///   calculation in a string table.</summary>
///
/// <remarks>The indicator for PIA and MFB printout is one of the following:
/// <table>
///     0   to not write out PIA or MFB.
///     1   for PIA.
///     2   for MFB.
///     3   for PIA and MFB.
/// </table></remarks>
///
/// <param name="piaMethod">Pia calculation method.</param>
/// <param name="piaParams">Pia calculation parameters.</param>
/// <param name="eligYear">Year of eligibility for catch-up benefit increases.
/// </param>
/// <param name="firstYear">First year to write.</param>
/// <param name="lastYear">Last year to write.</param>
/// <param name="piamfb">Indicator for PIA and MFB printout.</param>
void PiaPageOut::newTableOut( const PiaMethod& piaMethod,
const PiaParams& piaParams, int eligYear, int firstYear, int lastYear,
int piamfb )
{
#ifndef NDEBUG
  if (eligYear < YEAR37)
    throw PiaException(PIA_IDS_WISTRING);
#endif
  outputString.push_back("Table maximum changes applied:");
  for (int yr = firstYear; yr <= lastYear; yr++) {
    if (piaParams.getMonthBeninc(yr) > 0) {
      DateMoyr datetemp(piaParams.getMonthBeninc(yr), yr);
      strm.str("");
      strm << "     Max AME of " << piaParams.lastAmw(yr) << " for "
        << DateFormatter::toString(datetemp);
      if (piamfb != 0) {
        strm.precision(2);
        strm << ":  ";
        if (piamfb % 2 == 1) {
          strm << setw(8) << piaMethod.piaElig[yr];
        }
        if (piamfb > 1) {
          strm << "  " << setw(8) << piaMethod.mfbElig[yr];
        }
      }
      outputString.push_back(strm.str());
    }
  }
  outputString.push_back("");
}

/// <summary>Prepares PIA and MFB at entitlement in a string table.</summary>
///
/// <param name="piaMethod">Pia calculation method.</param>
void PiaPageOut::piamfb1( const PiaMethod& piaMethod )
{
  const int yearEnt = piaMethod.getCpiYear(PiaMethod::YEAR_ENT);
  piaEntOut(piaMethod.piaElig[yearEnt]);
  outputString.push_back("");
  if (piaMethod.getIndCap() == PiaMethod::PIA_1_5 ||
    piaMethod.getIndCap() == PiaMethod::PIA_1_0) {
    // cap is 1.0 or 1.5 times PIA
    outputString.push_back("MFB at entitlement date:");
    disMfbOut(piaMethod.getCap(), piaMethod.piaElig[yearEnt],
      piaMethod.mfbElig[yearEnt]);
  }
  else {
    strm.precision(2);
    strm.str("");
    strm << "MFB at entitlement date = " << piaMethod.mfbElig[yearEnt];
    outputString.push_back(strm.str());
    outputString.push_back("");
  }
}

/// <summary>Prepares PIA and MFB at benefit date in a string table.</summary>
///
/// <param name="piaMethod">Pia calculation method.</param>
void PiaPageOut::piamfb2( const PiaMethod& piaMethod )
{
  strm.precision(2);
  strm.str("");
  strm << "PIA at benefit date = " << piaMethod.piaEnt.get();
  outputString.push_back(strm.str());
  outputString.push_back("");
  if (piaMethod.getIndCap() == PiaMethod::PIA_1_5 ||
    piaMethod.getIndCap() == PiaMethod::PIA_1_0) {
    // cap is 1.0 or 1.5 times PIA
    outputString.push_back("MFB at benefit date:");
    disMfbOut(piaMethod.getCap(), piaMethod.piaEnt.get(),
      piaMethod.mfbEnt.get());
  }
  else {
    strm.str("");
    strm << "MFB at benefit date = " << piaMethod.mfbEnt.get();
    outputString.push_back(strm.str());
    outputString.push_back("");
  }
}

/// <summary>Prepares PIA and MFB from June 1974 table in a string
///  table.</summary>
///
/// <param name="piaMethod">Pia calculation method.</param>
void PiaPageOut::piamfb3( const PiaMethod& piaMethod )
{
  const int firstYear = piaMethod.getCpiYear(PiaMethod::FIRST_YEAR) - 1;
  strm.str("");
  strm.precision(2);
  strm << "PIA from June 1974 table = " << piaMethod.piaElig[firstYear];
  outputString.push_back(strm.str());
  outputString.push_back("");
  if (piaMethod.getIndCap() == PiaMethod::PIA_1_5 ||
    piaMethod.getIndCap() == PiaMethod::PIA_1_0) {
    // cap is 1.0 or 1.5 times PIA
    outputString.push_back("MFB at this date:");
    strm.str("");
    strm << "   " << piaMethod.getCap() << " * "
      << piaMethod.piaElig[firstYear];
    outputString.push_back(strm.str());
    strm.str("");
    strm << "   (MFB cap on DI cases) = " << piaMethod.mfbElig[firstYear];
    outputString.push_back(strm.str());
    outputString.push_back("");
  }
  else {
    strm.str("");
    strm << "MFB from June 1974 table = " << piaMethod.mfbElig[firstYear];
    outputString.push_back(strm.str());
    outputString.push_back("");
  }
}

/// <summary>Prepares MFB calculation in a string table.</summary>
///
/// <remarks>This version handles the DI family maximum.</remarks>
///
/// <param name="piaMethod">Pia calculation method number.</param>
/// <param name="portionPiaElig">Portion of pia in each mfb formula interval.
/// </param>
/// <param name="percMfb">Mfb formula percentages.</param>
/// <param name="aime">Aime used in pia formula.</param>
void PiaPageOut::mfbOut( const PiaMethod& piaMethod,
const double portionPiaElig[], const PercMfb& percMfb, double aime )
{
  if (piaMethod.getIndCap() == PiaMethod::PIA_1_5 ||
    piaMethod.getIndCap() == PiaMethod::PIA_1_0) {
    // cap is 1.0 or 1.5 times PIA
    return;
  }
  outputString.push_back("MFB at eligibility:");
  if (piaMethod.getIndCap() == PiaMethod::AIME_85) {
    // cap is 85% of AIME
    disMfbOut(piaMethod.getCap(), aime,
      piaMethod.mfbElig[piaMethod.getCpiYear(PiaMethod::FIRST_YEAR)]);
  }
  else {
    mfbOut(piaMethod, portionPiaElig, percMfb);
  }
}

/// <summary>Prepares MFB calculation in a string table.</summary>
///
/// <remarks>This version does not handle the DI family maximum.</remarks>
///
/// <param name="piaMethod">Pia calculation method number.</param>
/// <param name="portionPiaElig">Portion of pia in each mfb formula interval.
/// </param>
/// <param name="percMfb">Mfb formula percentages.</param>
void PiaPageOut::mfbOut( const PiaMethod& piaMethod,
const double portionPiaElig[], const PercMfb& percMfb )
{
  strm.precision(2);
  // usual formula
  for (int i2 = 0; i2 < 3; i2++) {
    strm.str("");
    strm << "   " << percMfb[i2] << " * " << portionPiaElig[i2] << " +";
    outputString.push_back(strm.str());
  }
  const int year = piaMethod.getCpiYear(PiaMethod::FIRST_YEAR);
  strm.str("");
  strm << "   " << percMfb[3] << " * " << portionPiaElig[3] << " = "
    << piaMethod.mfbElig[year];
  outputString.push_back(strm.str());
  outputString.push_back("");
}

/// <summary>Prepares real-wage-gain adjustment in a string table.</summary>
///
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="istart">Current year.</param>
/// <param name="piaMethod">Pia calculation method.</param>
/// <param name="rwgpia">Pia before real-wage-gain adjustment.</param>
/// <param name="rwgmfb">Mfb before real-wage-gain adjustment.</param>
void PiaPageOut::printRealWageGain( int eligYear, int istart,
const PiaMethod& piaMethod, double rwgpia, double rwgmfb )
{
  outputString.push_back(
    "Real-wage-gain adjustment for PEBES calculation:");
  strm.str("");
  strm.precision(2);
  strm << "   Factor = "
    << (1.0 + 0.01 * static_cast<double>(eligYear - istart));
  outputString.push_back(strm.str());
  strm.str("");
  strm << "   PIA before adjustment = " << rwgpia;
  outputString.push_back(strm.str());
  if (piaMethod.getMethod() == PiaMethod::WAGE_IND) {
    strm.str("");
    strm << "   MFB before adjustment = " << rwgmfb;
    outputString.push_back(strm.str());
  }
  outputString.push_back("");
}

/// <summary>Prepares table heading for wage-indexed earnings in a string
///   table.</summary>
///
/// <param name="avgbsyr">Average wage in base year.</param>
void PiaPageOut::tableHead1( double avgbsyr )
{
  outputString.push_back(
    "                           earnings        indexed       high n");
  strm.str("");
  strm.precision(2);
  strm << "year      earnings      * " << setw(11) << avgbsyr
    << "      earnings       years";
  outputString.push_back(strm.str());
  outputString.push_back("");
}

/// <summary>Prepares table heading for non-indexed earnings in a string
///   table.</summary>
void PiaPageOut::tableHead2()
{
  outputString.push_back("                        High n");
  outputString.push_back("Year      Earnings       Years");
  outputString.push_back("");
}

/// <summary>Prepares base year in a string table.</summary>
///
/// <param name="year">Base year to put in a string table.</param>
void PiaPageOut::baseYearOut( int year )
{
  strm.str("");
  strm << "Base year for indexing = " << PunctFacet::getYear(year);
  outputString.push_back(strm.str());
  outputString.push_back("");
}

/// <summary>Prepares PIA at entitlement in a string table.</summary>
///
/// <param name="piaentt">Pia at entitlement.</param>
void PiaPageOut::piaEntOut( double piaentt )
{
  strm.precision(2);
  strm.str("");
  strm << "PIA at entitlement = " << piaentt;
  outputString.push_back(strm.str());
}

/// <summary>Prepares raw pro rata PIA in a string table.</summary>
///
/// <param name="rawpia">Raw pro rata PIA.</param>
void PiaPageOut::rawPiaOut( double rawpia )
{
  strm.str("");
  strm.precision(2);
  strm << "Raw Pro Rata PIA = " << rawpia;
  outputString.push_back(strm.str());
  outputString.push_back("");
}

/// <summary>Prepares totalization PIAs in a string table.</summary>
///
/// <param name="piaMethod">Pia calculation method.</param>
/// <param name="qcTotal">Total number of quarters of coverage.</param>
/// <param name="n">Number of computation years.</param>
void PiaPageOut::totPiaOut( const PiaMethod& piaMethod, int qcTotal, int n )
{
  strm.precision(2);
  strm.str("");
  strm << "Theoretical PIA = " << piaMethod.getPiaTotal().get();
  outputString.push_back(strm.str());
  outputString.push_back("");
  strm.str("");
  strm << "Pro Rata PIA = (" << (3 * qcTotal) << "/" << (12 * n) << ") * "
    << piaMethod.getPiaTotal().get() << " = " << piaMethod.piaEnt.get();
  outputString.push_back(strm.str());
  outputString.push_back("");
}

/// <summary>Prepares MFB cap on DI cases in a string table.</summary>
///
/// <remarks>This is called from <see cref="piamfb1"/>, <see cref="piamfb2"/>,
/// and <see cref="mfbOut"/>.</remarks>
///
/// <param name="capt">Percentage cap for DI MFB.</param>
/// <param name="amepia">Ame or pia.</param>
/// <param name="mfbt">DI MFB.</param>
void PiaPageOut::disMfbOut( double capt, double amepia, double mfbt )
{
  strm.str("");
  strm.precision(2);
  strm << "   " << capt << " * " << amepia;
  outputString.push_back(strm.str());
  strm.str("");
  strm << "   (MFB cap on DI cases) = " << mfbt;
  outputString.push_back(strm.str());
  outputString.push_back("");
}

/// <summary>Prepares applicable old start pia table in a string
///  table.</summary>
///
/// <param name="piaMethod">Pia calculation method.</param>
void PiaPageOut::osTableOut( const PiaMethod& piaMethod )
{
  strm.str("");
  strm << "Applicable table: " << tableLabel[piaMethod.getTableNum()]
    << " Act";
  outputString.push_back(strm.str());
  outputString.push_back("");
}

/// <summary>Prepares MFB formula bend points in a string table.</summary>
///
/// <param name="bendMfb">Mfb formula bend points.</param>
void PiaPageOut::mfbBpOut( const BendMfb& bendMfb )
{
  strm.str("");
  strm.precision(0);
  strm << "MFB formula bend points = " << bendMfb[1] << ", " << bendMfb[2]
    << ", and " << bendMfb[3];
  outputString.push_back(strm.str());
  outputString.push_back("");
}

/// <summary>Prepares PIA formula bend points in a string table.</summary>
///
/// <param name="bendPia">Pia formula bend points.</param>
void PiaPageOut::piaBpOut( const BendPia& bendPia )
{
  strm.str("");
  strm.precision(0);
  strm << "PIA formula bend points = " << bendPia[1] << " and " << bendPia[2];
  outputString.push_back(strm.str());
  outputString.push_back("");
}

/// <summary>Prepares wage-indexed pia at eligibility calculation in a
/// string table.</summary>
///
/// <param name="percPia">Pia formula percentages.</param>
/// <param name="portionAime">Parts of aime in each pia formula interval.
/// </param>
/// <param name="piaTemp">Pia at eligibility.</param>
void PiaPageOut::piaEligOut( const PercPia& percPia,
const PortionAime& portionAime, double piaTemp )
{
  outputString.push_back("PIA at eligibility = ");
  for (int i = 0; i < 2; i++) {
    strm.str("");
    strm << "   " << setprecision(2) << percPia[i] << " * "
      << setprecision(0) << portionAime[i] << " +";
      outputString.push_back(strm.str());
  }
  strm.str("");
  strm << "   " << setprecision(2) << percPia[2] << " * " << setprecision(0)
    << portionAime[2] << " = " << setprecision(2) << piaTemp;
  outputString.push_back(strm.str());
  outputString.push_back("");
}

/// <summary>Prints out name and SSN.</summary>
///
/// <param name="workerData">Basic worker data.</param>
void PiaPageOut::printNameSsn( const WorkerDataGeneral& workerData )
{
  string s;  // temporary string
  AfxFormatString1(s, PIA_IDS_NAMEOUT, workerData.getNhname());
  outputString.push_back(s);
  AfxFormatString1(s, PIA_IDS_SSNOUT, workerData.getIdString());
  outputString.push_back(s);
}

/// <summary>Prints out disability insured message.</summary>
///
/// <param name="piaData">Pia calculation data.</param>
void PiaPageOut::disInsOut( const PiaData& piaData )
{
  if (!(piaData.disInsCode.isDisabilityInsured()) && 
      !(piaData.disInsCode == DisInsCode::TOTALIZED)) {
    string s;  // temporary string
    AfxFormatString1(s, PIA_IDS_NDISINOUT, "");
    outputString.push_back(s);
  }
}

/// <summary>Prints out noncovered pension amount and starting date.</summary>
///
/// <param name="pubpen">Amount of public pension.</param>
/// <param name="pubpenDate">Starting date of public pension.</param>
void PiaPageOut::nonpenOut( double pubpen, const DateMoyr& pubpenDate )
{
  string s;  // temporary string
  strm.str("");
  strm.precision(2);
  strm << pubpen;
  AfxFormatString1(s, PIA_IDS_NONPENOUT, strm.str());
  outputString.push_back(s);
  AfxFormatString1(s, PIA_IDS_NONPENDATEOUT,
    DateFormatter::toString(pubpenDate));
  outputString.push_back(s);
}

/// <summary>Prints out uninsured message.</summary>
///
/// <param name="finsCode">Insured status code.</param>
/// <param name="workerData">Worker basic data.</param>
/// <param name="piaData">Benefit calculation data.</param>
void PiaPageOut::nonIns( char finsCode, const WorkerDataGeneral& workerData,
const PiaData& piaData )
{
  string s;  // temporary output string
  switch (finsCode)
  {
  case '2':
    // survivor case, currently insured only
    AfxFormatString1(s, PIA_IDS_CURINSOUT, "");
    outputString.push_back(s);
    return;
  case '4':
    // life case, not fully insured
    AfxFormatString1(s, PIA_IDS_NINSUROUT, "");
    outputString.push_back(s);
    return;
  case '5':
    // not currently or fully insured (survivor case)
    AfxFormatString1(s, PIA_IDS_NINSTOUT, "");
    outputString.push_back(s);
    return;
  case '6':
    // totalization, with less than 6 qcs
    AfxFormatString1(s, PIA_IDS_NTOTALOUT, "");
    outputString.push_back(s);
    outputString.push_back(warnTotalization);
    return;
  case '7':
    // totalization, but fully insured
    if (!(piaData.disInsCode == DisInsCode::TOTALIZED &&
      workerData.getJoasdi() == WorkerDataGeneral::DISABILITY)) {
      AfxFormatString1(s, PIA_IDS_INSUREOUT, "");
      outputString.push_back(s);
      outputString.push_back(warnTotalization);
    }
    return;
  default:
    return;
  }
}

/// <summary>Prepares header strings for output.</summary>
void PiaPageOut::prepareHeader()
{
  boost::posix_time::time_facet* output_facet =
    new boost::posix_time::time_facet();
  output_facet->format("%a %b %d %H:%M:%S %Y");
  stringstream ss;
  ss.imbue(locale(locale::classic(), output_facet));
  boost::posix_time::ptime pt =
    boost::posix_time::second_clock::local_time();
  ss << pt;
  ss << string(43, ' ') << "Page " << pageNum;
  outputString.push_back(ss.str());
  for (int i = 0; i < 2; i++) {
    if (pageTitle[i].length() > 0) {
      strm.str("");
      const unsigned spacecount = (pageTitle[i].length() < 74u) ?
        (74u - pageTitle[i].length()) / 2u : 0u;
      strm << string(spacecount, ' ') << pageTitle[i];
      outputString.push_back(strm.str());
    }
  }
  outputString.push_back("");
  outputString.push_back("");
}

/// <summary>Print out indexed earnings table.</summary>
///
/// <param name="wageInd">Wage-indexed calculation method.</param>
/// <param name="piaData">Pia calculation summary data.</param>
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="earnings">Unindexed earnings.</param>
/// <param name="firstYear">First year of earnings.</param>
void PiaPageOut::printIndexedEarningsTable( const WageIndGeneral& wageInd,
const PiaData& piaData, int eligYear, const AverageWage& earnings,
int firstYear )
{
  tableHead1(wageInd.getIndexYearAvgWage());
  for (int yr = firstYear; yr <= piaData.getEarnYear(); yr++) {
    strm.str("");
    strm.precision(2);
    strm << PunctFacet::getYear(yr) << setw(14) << earnings[yr];
    if (yr < eligYear - 1) {
      strm << setw(19) << wageInd.earnMultiplied[yr];
    }
    else {
      strm << "                   ";
    }
    strm << setw(14) << wageInd.earnIndexed[yr];
    if (wageInd.iorder[yr] > 0) {
      strm << setw(14) << wageInd.earnIndexed[yr];
    }
    outputString.push_back(strm.str());
  }
  outputString.push_back("");
}

/// <summary>Prepares windfall elimination provision calculation.</summary>
///
/// <param name="wageInd">Wage-indexed calculation method.</param>
/// <param name="pubpen">Amount of public pension.</param>
/// <param name="pubpenDate">Starting date of public pension.</param>
void PiaPageOut::pubPenOut( const WageIndGeneral& wageInd, double pubpen,
  const DateMoyr& pubpenDate )
{
  nonpenOut(pubpen, pubpenDate);
  outputString.push_back("");
  strm.str("");
  strm << wageInd.getYearsTotal()
    << " years of coverage for windfall provision";
  outputString.push_back(strm.str());
  if (wageInd.getWindfall() == PiaMethod::HAS30YEARS) {
    outputString.push_back("");
    outputString.push_back("Special minimum savings clause applies");
    outputString.push_back("");
    return;
  }
  outputString.push_back("");
  outputString.push_back("PIA after windfall:");
  const int year = wageInd.getCpiYear(PiaMethod::FIRST_YEAR);
  if (wageInd.getWindfall() == PiaMethod::REDUCEDPERC) {
    for (int i = 0; i < 2; i++) {
      strm.str("");
      strm << "   " << setprecision(2) << wageInd.percWind[i] << " * "
        << setprecision(0) << wageInd.portionAime[i] << " +";
      outputString.push_back(strm.str());
    }
    strm.str("");
    strm << "   " << setprecision(2) << wageInd.percWind[2] << " * "
      << setprecision(0) << wageInd.portionAime[2] << " = "
      << setprecision(2) << wageInd.piaElig[year];
    outputString.push_back(strm.str());
    outputString.push_back("");
  }
  else {
    strm.str("");
    strm.precision(2);
    strm << "   " << wageInd.getPiaWindfall().toString() << " - 0.5*"
      << pubpen << " = " << wageInd.piaElig[year];
    outputString.push_back(strm.str());
    outputString.push_back("");
  }
}
