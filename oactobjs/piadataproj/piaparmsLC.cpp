// Functions for the <see cref="PiaParamsLC"/> class to handle changed
// law PIA parameters.

// $Id: piaparmsLC.cpp 1.84 2017/10/12 13:06:34EDT 277133 Development  $

#include <cfloat>  // for FLT_MAX
#include "piaparmsLC.h"
#include "bppiaout.h"
#include "oactcnst.h"
#include "BenefitAmount.h"
#include "AwbiDataNonFile.h"
#include "LawChangeArray.h"
#include "LawChangeBPFRACWAGE.h"
#include "LawChangeNEWFORMULA.h"
#include "LawChangeDECLINEPERC.h"
#include "LawChangeBPSPECRATE.h"
#include "LawChangeBPMINCONST.h"
#include "LawChangeMARRLENGTH.h"
#include "LawChangeNEWSPECMIN.h"
#include "LawChangeNRACHANGE.h"
#include "LawChangeWIFEFACTOR.h"
#include "LawChangeWIDFACTOR.h"
#include "LawChangeCHILDCAREDROPOUT.h"
#include "LawChangeCHILDCARECREDIT.h"
#include "LawChangeWAGEBASECHG.h"
#include "LawChangeCOLACHANGE.h"

using namespace std;

/// <summary>Initializes arrays with a specified starting year.</summary>
///
/// <param name="newIstart">Year after last known benefit increase.</param>
/// <param name="newMaxyear">Maximum projected year.</param>
/// <param name="awbiData">Structure with historical amounts.</param>
/// <param name="newLawChange">Law change parameters.</param>
PiaParamsLC::PiaParamsLC( int newIstart, int newMaxyear, AwbiData& awbiData,
const LawChangeArray& newLawChange ) :
PiaParams(newIstart, newMaxyear),
baseOasdi(newLawChange, maxyear), base77(newLawChange, maxyear),
altCpiinc(YEAR51, newMaxyear), colaCaps(YEAR51, newMaxyear),
fqbppia(YEAR37, newMaxyear), fqindex(YEAR37, newMaxyear),
lawChange(newLawChange), mfbColaCaps(YEAR51, newMaxyear),
specMinPiaExtra(), specMinMfbExtra()
{
  setData(awbiData);
  initdata();
  setAltCpiinc();
  setFqIndex();
  setFqBppia();
}

/// <summary>Initializes arrays without a specified starting year.</summary>
///
/// <remarks>This version without a specified starting year uses
/// <see cref="BaseYearNonFile::YEAR"/>, and constructs an
/// <see cref="AwbiDataNonFile"/> using that year.</remarks>
///
/// <param name="newMaxyear">Maximum projected year.</param>
/// <param name="newLawChange">Law change parameters.</param>
PiaParamsLC::PiaParamsLC( int newMaxyear,
const LawChangeArray& newLawChange ) :
PiaParams(newMaxyear), baseOasdi(newLawChange, maxyear),
base77(newLawChange, maxyear), altCpiinc(YEAR51, newMaxyear),
colaCaps(YEAR51, newMaxyear), fqbppia(YEAR37, newMaxyear),
fqindex(YEAR37, newMaxyear), lawChange(newLawChange), mfbColaCaps(YEAR51,
newMaxyear), specMinPiaExtra(), specMinMfbExtra()
{
  AwbiDataNonFile awbiData(newMaxyear);
  setData(awbiData);
  initdata();
  setAltCpiinc();
  setFqIndex();
  setFqBppia();
}

/// <summary>Destroys arrays.</summary>
PiaParamsLC::~PiaParamsLC( )
{ }

/// <summary>Initializes some data.</summary>
///
/// <remarks>For various types of law changes that are being phased in
/// immediately (i.e. <see cref="LawChange::getPhaseType"/> == 1), then need
/// to do a recalculation for benefits the first year it is effective. In those
/// cases, we set the indicator for a recalculation in that year.</remarks>
void PiaParamsLC::initdata()
{
  PiaParams::initdata();
  LawChangeCHILDCARECREDIT *lawChangeCHILDCARECREDIT =
    lawChange.lawChangeCHILDCARECREDIT;
  if (lawChangeCHILDCARECREDIT->getInd() > 0 &&
    lawChangeCHILDCARECREDIT->getPhaseType() == 1) {
    recalcInd.setBit(lawChangeCHILDCARECREDIT->getStartYear(), true);
  }
  LawChangeNEWSPECMIN *lawChangeNEWSPECMIN = lawChange.lawChangeNEWSPECMIN;
  if (lawChangeNEWSPECMIN->getInd() > 0 &&
    lawChangeNEWSPECMIN->getPhaseType() == 1) {
    recalcInd.setBit(lawChangeNEWSPECMIN->getStartYear(), true);
  }
}

/// <summary>Sets benefit increases when changed from present law.</summary>
///
/// <remarks>This is called in the constructor (see
/// <see cref="PiaParamsLC::PiaParamsLC"/>) and in
/// <see cref="projectCpiinc"/>.</remarks>
void PiaParamsLC::setAltCpiinc()
{
  LawChangeCOLACHANGE *lawChangeCOLACHANGE = lawChange.lawChangeCOLACHANGE;
  if (lawChangeCOLACHANGE->getInd() == 1) {
    const int lastYear = min(altCpiinc.getLastYear(),
      lawChangeCOLACHANGE->getEndYear());
    const int firstYear = lawChangeCOLACHANGE->getStartYear();
    // use present-law colas before first specified year
    altCpiinc.assign(cpiinc, YEAR51, firstYear - 1);
    // use changed colas during specified period
    for (int i = firstYear; i <= lastYear; i++) {
      altCpiinc[i] = cpiinc[i] + lawChangeCOLACHANGE->getColaAdj();
    }
    // use present-law colas after last specified year
    altCpiinc.assign(cpiinc, lastYear + 1, altCpiinc.getLastYear());
  }
}

/// <summary>Sets series of average wages or other series used for indexing
/// pia formula bend points.</summary>
///
/// <remarks>This is called in the constructor (see
/// <see cref="PiaParamsLC::PiaParamsLC"/>).</remarks>
void PiaParamsLC::setFqBppia()
{
  // increase at fraction of wage rate
  LawChangeBPFRACWAGE* lawChangeBPFRACWAGE = lawChange.lawChangeBPFRACWAGE;
  if (lawChangeBPFRACWAGE->getInd() > 0) {
    const int lastYear = min(fqbppia.getLastYear(),
      lawChangeBPFRACWAGE->getEndYear() - 2);
    const int firstYear = lawChangeBPFRACWAGE->getStartYear() - 2;
    fqbppia.assign(getFqArray(), 1977, firstYear - 1);
    double temp = getFq(firstYear - 1);
    // project by percent of average wage increase
    for (int year = firstYear; year <= lastYear; year++) {
      const double factor =
        1.0 + lawChangeBPFRACWAGE->getProportionBendPoints() *
        getFqinc(year) / 100.0;
      temp *= factor;
      fqbppia[year] = temp;
    }
    // continue projecting at wage rate beyond ending year
    for (int year = lastYear + 1; year <= fqbppia.getLastYear(); year++) {
      const double factor = getFq(year) / getFq(year-1);
      temp *= factor;
      fqbppia[year] = temp;
    }
    return;
  }
  // increase at cpi rate
  LawChange *lawChangeBPCPI = lawChange.lawChange[LawChange::BPCPI];
  if (lawChangeBPCPI->getInd() > 0) {
    const int lastYear = min(fqbppia.getLastYear(),
      lawChangeBPCPI->getEndYear() - 2);
    const int firstYear = lawChangeBPCPI->getStartYear() - 2;
    fqbppia.assign(getFqArray(), 1977, firstYear - 1);
    double temp = getFq(firstYear - 1);
    // project by cpi increase
    for (int year = firstYear - 2; year <= lastYear - 2; year++) {
      const double factor = 1.0 + getCpiinc(year) / 100.0;
      temp *= factor;
      fqbppia[year] = temp;
    }
    // continue projecting at wage rate beyond ending year
    for (int year = lastYear + 1; year <= fqbppia.getLastYear(); year++) {
      const double factor = getFq(year) / getFq(year-1);
      temp *= factor;
      fqbppia[year] = temp;
    }
    return;
  }
  // project off of alternate bend points
  LawChangeNEWFORMULA* lawChangeNEWFORMULA = lawChange.lawChangeNEWFORMULA;
  if (lawChangeNEWFORMULA->getInd() > 0) {
    const int firstYear = lawChangeNEWFORMULA->getStartYear() - 2;
    fqbppia.assign(getFqArray(), 1977, fqbppia.getLastYear());
    // remainder of bend points are set in projectfq
    return;
  }
  LawChangeBPSPECRATE* lawChangeBPSPECRATE = lawChange.lawChangeBPSPECRATE;
  if (lawChangeBPSPECRATE->getInd() > 0) {
    const int lastYear = min(fqbppia.getLastYear(),
      lawChangeBPSPECRATE->getEndYear() - 2);
    const int firstYear = lawChangeBPSPECRATE->getStartYear() - 2;
    fqbppia.assign(getFqArray(), 1977, firstYear - 1);
    double temp = getFq(firstYear - 1);
    // project by ad hoc bend point increases
    for (int year = firstYear; year <= lastYear; year++) {
      const double factor =
        1.0 + lawChangeBPSPECRATE->getBendPiaInc(year) / 100.0;
      temp *= factor;
      fqbppia[year] = temp;
    }
    // continue projecting at wage rate beyond ending year
    for (int year = lastYear + 1; year <= fqbppia.getLastYear(); year++) {
      double factor = getFq(year) / getFq(year-1);
      temp *= factor;
      fqbppia[year] = temp;
    }
    return;
  }
  LawChangeBPMINCONST *lawChangeBPMINCONST = lawChange.lawChangeBPMINCONST;
  if (lawChangeBPMINCONST->getInd() > 0) {
    const int lastYear = min(fqbppia.getLastYear(),
      lawChangeBPMINCONST->getEndYear() - 2);
    const int firstYear = lawChangeBPMINCONST->getStartYear() - 2;
    fqbppia.assign(getFqArray(), 1977, firstYear - 1);
    double temp = getFq(firstYear - 1);
    // project by average wage increase minus a percentage
    for (int year = firstYear; year <= lastYear; year++) {
      const double factor = 1.0 + (getFqinc(year) -
        lawChangeBPMINCONST->getConstantPerc()) / 100.0;
      temp *= factor;
      fqbppia[year] = temp;
    }
    // continue projecting at wage rate beyond ending year
    for (int year = lastYear + 1; year <= fqbppia.getLastYear(); year++) {
      const double factor = getFq(year) / getFq(year-1);
      temp *= factor;
      fqbppia[year] = temp;
    }
    return;
  }
}

/// <summary>Returns benefit amount increased by cola.</summary>
///
/// <returns>Benefit amount increased by cola, without considering any
/// catch-up benefit increases.</returns>
///
/// <remarks>This version does not consider any catch-up benefit increases.
/// </remarks>
///
/// <param name="pia">Amount to be increased.</param>
/// <param name="year">Year of increase.</param>
double PiaParamsLC::applyCola( double pia, int year ) const
{
  LawChangeCOLACHANGE *lawChangeCOLACHANGE = lawChange.lawChangeCOLACHANGE;
  if (lawChangeCOLACHANGE->getInd() == 2 &&
    lawChangeCOLACHANGE->isEffective(year, year)) {
    return min(PiaParams::applyCola(pia, year), pia + colaCaps[year]);
  }
  return PiaParams::applyCola(pia, year);
}

/// <summary>Returns benefit amount increased by cola.</summary>
///
/// <returns>Benefit amount increased by cola, considering any catch-up
/// benefit increases.</returns>
///
/// <remarks>This version considers any catch-up benefit increases.</remarks>
///
/// <param name="pia">Amount to be increased.</param>
/// <param name="year">Year of increase.</param>
/// <param name="eligYear">Year of eligibility for catch-up benefit
/// increases.</param>
double PiaParamsLC::applyCola( double pia, int year, int eligYear ) const
{
  LawChangeCOLACHANGE *lawChangeCOLACHANGE = lawChange.lawChangeCOLACHANGE;
  if (lawChangeCOLACHANGE->getInd() == 2 &&
    lawChangeCOLACHANGE->isEffective(year, year)) {
    return min(PiaParams::applyCola(pia, year, eligYear),
      pia + colaCaps[year]);
  }
  return PiaParams::applyCola(pia, year, eligYear);
}

/// <summary>Returns maximum family benefit amount increased by cola, with
/// check that result is at least 150 percent of pia.</summary>
///
/// <remarks>This version does not consider any catch-up benefit increases.
/// </remarks>
///
/// <returns>Increased maximum family benefit amount.</returns>
///
/// <param name="mfb">Amount to be increased.</param>
/// <param name="year">Year of increase.</param>
/// <param name="pia">Pia to check against.</param>
double PiaParamsLC::applyColaMfb( double mfb, int year, double pia ) const
{
  LawChangeCOLACHANGE *lawChangeCOLACHANGE = lawChange.lawChangeCOLACHANGE;
  if (lawChangeCOLACHANGE->getInd() == 2 &&
    lawChangeCOLACHANGE->isEffective(year, year)) {
    return min(PiaParams::applyColaMfb(mfb, year, pia),
      mfb + mfbColaCaps[year]);
  }
  return PiaParams::applyColaMfb(mfb, year, pia);
}

/// <summary>Returns maximum family benefit amount increased by cola, with
/// check that result is at least 150 percent of pia.</summary>
///
/// <remarks>This version considers any catch-up benefit increases.</remarks>
///
/// <returns>Increased maximum family benefit amount.</returns>
///
/// <param name="mfb">Amount to be increased.</param>
/// <param name="year">Year of increase.</param>
/// <param name="pia">Pia to check against.</param>
/// <param name="eligYear">Year of eligibility for catch-up benefit
/// increases.</param>
double PiaParamsLC::applyColaMfb( double mfb, int year, int eligYear,
double pia ) const
{
  LawChangeCOLACHANGE *lawChangeCOLACHANGE = lawChange.lawChangeCOLACHANGE;
  if (lawChangeCOLACHANGE->getInd() == 2 &&
    lawChangeCOLACHANGE->isEffective(year, year)) {
    return min(PiaParams::applyColaMfb(mfb, year, eligYear, pia),
      mfb + mfbColaCaps[year]);
  }
  return PiaParams::applyColaMfb(mfb, year, eligYear, pia);
}

/// <summary>Returns the amount of the cap on the COLA for the given year.
/// </summary>
///
/// <returns>The amount of the cap on the COLA for the given year (returns
///   maximum float if no cap).</returns>
///
/// <param name="year">Year being capped.</param>
double PiaParamsLC::getColaCap( int year ) const
{
  LawChangeCOLACHANGE *lawChangeCOLACHANGE = lawChange.lawChangeCOLACHANGE;
  if (lawChangeCOLACHANGE->getInd() == 2)
    return(colaCaps[year]);
  // No cap on the COLA, it can be as big as a regular COLA increase.
  return FLT_MAX;
}

/// <summary>Returns true if a cola cap is required at any time.</summary>
///
/// <returns>True if a cola cap is required, based on changed law.</returns>
///
/// <remarks>Looks at <see cref="LawChange::lawChangeType::COLACHANGE"/>.
/// </remarks>
bool PiaParamsLC::needColaCap() const
{
  LawChangeCOLACHANGE *lawChangeCOLACHANGE = lawChange.lawChangeCOLACHANGE;
  return (lawChangeCOLACHANGE->getInd() == 2);
}

/// <summary>Returns true if a cola cap is required for a specified year.
/// </summary>
///
/// <returns>True if a cola cap is required, based on changed law.</returns>
///
/// <remarks>Looks at <see cref="LawChange::lawChangeType::COLACHANGE"/>.
/// </remarks>
///
/// <param name="year">Year being capped.</param>
bool PiaParamsLC::needColaCap( int year ) const
{
  LawChangeCOLACHANGE *lawChangeCOLACHANGE = lawChange.lawChangeCOLACHANGE;
  return (lawChangeCOLACHANGE->isEffective(year, year) &&
    lawChangeCOLACHANGE->getInd() == 2);
}

/// <summary>Sets the amount of the cap on the COLA for the given year.
/// </summary>
///
/// <remarks>Sets the cap in the stored arrays of cola caps for pias and
/// mfbs.</remarks>
///
/// <param name="cap">Amount of cap for pia.</param>
/// <param name="year">Year being capped.</param>
void PiaParamsLC::setColaCap( double cap, int year )
{
  if (needColaCap(year)) {
    colaCaps[year] = cap;
    mfbColaCaps[year] = BenefitAmount::round(1.75 * cap, year);
  }
}

/// <summary>Returns the amount of the cap on the COLA for the MFB for the
/// given year.</summary>
///
/// <returns>The amount of the cap on the COLA for the MFB for the given year
/// (returns maximum float if no cap).</returns>
///
/// <param name="year">Year being capped.</param>
double PiaParamsLC::getMfbColaCap( int year ) const
{
  LawChangeCOLACHANGE *lawChangeCOLACHANGE = lawChange.lawChangeCOLACHANGE;
  if (lawChangeCOLACHANGE->getInd() == 2)
    return(mfbColaCaps[year]);
  // No cap on the COLA, it can be as big as a regular COLA increase.
  return FLT_MAX;
}

/// <summary>Sets the amount of the cap on the COLA for the MFB
///   for the given year.</summary>
///
/// <param name="cap">Amount of cap.</param>
/// <param name="year">Year being capped.</param>
void PiaParamsLC::setMfbColaCap( double cap, int year )
{
  LawChangeCOLACHANGE *lawChangeCOLACHANGE = lawChange.lawChangeCOLACHANGE;
  if (lawChangeCOLACHANGE->getInd() == 2)
    mfbColaCaps[year] = cap;
}

/// <summary>Returns one benefit increase.</summary>
///
/// <returns>One benefit increase.</returns>
///
/// <param name="year">The year for which a benefit increase is desired.
/// </param>
double PiaParamsLC::getCpiinc( int year ) const
{
  LawChangeCOLACHANGE *lawChangeCOLACHANGE = lawChange.lawChangeCOLACHANGE;
  if (lawChangeCOLACHANGE->getInd() == 1)
    return(altCpiinc[year]);
  // return regular benefit increases if no changes in law.
  return(cpiinc[year]);
}

/// <summary>Returns the benefit increases.</summary>
///
/// <returns>The benefit increases.</returns>
const DoubleAnnual& PiaParamsLC::getCpiincArray() const
{
  LawChangeCOLACHANGE *lawChangeCOLACHANGE = lawChange.lawChangeCOLACHANGE;
  if (lawChangeCOLACHANGE->getInd() == 1)
    return(altCpiinc);
  // return regular benefit increases if no changes in law.
  return(cpiinc);
}

/// <summary>Returns series of average wages or other series used for
/// indexing pia formula bend points.</summary>
///
/// <returns>Series of average wages or other series used for indexing pia
/// formula bend points.</returns>
const AverageWage& PiaParamsLC::getFqBppia() const
{
  if (lawChange.lawChange[LawChange::BPFRACWAGE]->getInd() > 0 ||
    lawChange.lawChange[LawChange::BPCPI]->getInd() > 0 ||
    lawChange.lawChange[LawChange::NEWFORMULA]->getInd() > 0 ||
    lawChange.lawChange[LawChange::BPSPECRATE]->getInd() > 0 ||
    lawChange.lawChange[LawChange::BPMINCONST]->getInd() > 0) {
    return(fqbppia);
  }
  // return average wage series if no changes
  return(fq);
}

/// <summary>Calculate series of average wages or other series used for
///   indexing an earnings record.</summary>
void PiaParamsLC::setFqIndex()
{
  LawChange *lawChangeEARNINDCPI =
    lawChange.lawChange[LawChange::EARNINDCPI];
  if (lawChangeEARNINDCPI->getInd() > 0) {
    const int yr2 = min(fqindex.getLastYear(),
      lawChangeEARNINDCPI->getEndYear() - 2);
    const int yr3 = lawChangeEARNINDCPI->getStartYear() - 2;
    fqindex.assign(getFqArray(), YEAR37, yr3 - 1);
    double temp = getFq(yr3 - 1);
    // project by cpi increase
    for (int yr = yr3; yr <= yr2; yr++) {
      const double factor = 1.0 + getCpiinc(yr) / 100.0;
      temp *= factor;
      fqindex[yr] = temp;
    }
    // continue projecting at wage rate beyond ending year
    for (int yr = yr2 + 1; yr <= fqindex.getLastYear(); yr++) {
      const double factor = getFq(yr) / getFq(yr-1);
      temp *= factor;
      fqindex[yr] = temp;
    }
  }
}

/// <summary>Returns series of average wages or other series used for
/// indexing an earnings record.</summary>
///
/// <returns>Series of average wages or other series used for indexing an
/// earnings record.</returns>
const AverageWage& PiaParamsLC::getFqIndex() const
{
  LawChange *lawChangeEARNINDCPI =
    lawChange.lawChange[LawChange::EARNINDCPI];
  if (lawChangeEARNINDCPI->getInd() > 0)
    return(fqindex);
  // return average wage series if no changes
  return(fq);
}

/// <summary>Projects average annual wage and amounts dependent on it.
/// </summary>
void PiaParamsLC::projectFq()
{
  PiaParams::projectFq();
  LawChangeNEWFORMULA *lawChangeNEWFORMULA = lawChange.lawChangeNEWFORMULA;
  if (lawChangeNEWFORMULA->getInd() > 0) {
    double altBp[4];
    const int yr2 =
      min(fqbppia.getLastYear(), lawChangeNEWFORMULA->getEndYear());
    const int yr3 = lawChangeNEWFORMULA->getStartYear();
    for (int yr = yr3; yr <= yr2; yr++) {
      for (int i = 0; i < lawChangeNEWFORMULA->getNumBp(); i++) {
        altBp[i] = lawChangeNEWFORMULA->getAltBendPia(yr, i + 1);
      }
      bpPiaOut.setSingleYearData(yr, lawChangeNEWFORMULA->getNumBp(), altBp);
    }
    // project bend points beyond specified ones
    bpPiaOut.setIndexedData(getFqArray(), yr2 + 1, yr2, lawChangeNEWFORMULA->getNumBp());
  }
}

/// <summary>Projects benefit formula percentages.</summary>
void PiaParamsLC::projectPerc()
{
  PiaParams::projectPerc();
  LawChangeNEWFORMULA *lawChangeNEWFORMULA = lawChange.lawChangeNEWFORMULA;
  LawChangeDECLINEPERC *lawChangeDECLINEPERC =
    lawChange.lawChangeDECLINEPERC;
  if (lawChangeNEWFORMULA->getInd() > 0 ||
    lawChangeDECLINEPERC->getInd() > 0) {
    int lastYear;  // last year of changing percentages
    int firstYear;  // first year of changing percentages
    double percTemp[5];
    int numBp = 2;
    if (lawChangeNEWFORMULA->getInd() > 0) {
      numBp = lawChangeNEWFORMULA->getNumBp();
      lastYear = min(maxyear, lawChangeNEWFORMULA->getEndYear());
      firstYear = lawChangeNEWFORMULA->getStartYear();
      for (int yr = firstYear; yr <= lastYear; yr++) {
        for (int i = 0; i <= numBp; i++){
          percTemp[i] = lawChangeNEWFORMULA->getAltPercPia(yr, i);
        }
        percPiaOut.setData(yr, numBp + 1, percTemp);
      }
      // save last percentages
      for (int i = 0; i <= numBp; i++){
        percTemp[i] = lawChangeNEWFORMULA->getAltPercPia(lastYear, i);
      }
    } else {
      lastYear = min(maxyear, lawChangeDECLINEPERC->getEndYear());
      firstYear = lawChangeDECLINEPERC->getStartYear();
      for (int yr = firstYear; yr <= lastYear; yr++) {
        percTemp[0] = lawChangeDECLINEPERC->getAltPercPia(yr, 0);
        percTemp[1] = lawChangeDECLINEPERC->getAltPercPia(yr, 1);
        percTemp[2] = lawChangeDECLINEPERC->getAltPercPia(yr, 2);
        percPiaOut.setData(yr, 3, percTemp);
      }
      // save last percentages
      percTemp[0] = lawChangeDECLINEPERC->getAltPercPia(lastYear, 0);
      percTemp[1] = lawChangeDECLINEPERC->getAltPercPia(lastYear, 1);
      percTemp[2] = lawChangeDECLINEPERC->getAltPercPia(lastYear, 2);
    }
    // project percentages beyond specified ones
    for (int yr = lastYear + 1; yr <= maxyear; yr++) {
      percPiaOut.setData(yr, numBp + 1, percTemp);
    }
  }
}

/// <summary>Returns amount per year of coverage in special minimum.
/// </summary>
///
/// <returns>Amount per year of coverage in special minimum.</returns>
///
/// <param name="date">The date of desired amount.</param>
double PiaParamsLC::specMinAmountCal( const DateMoyr& date ) const
{
  LawChangeNEWSPECMIN *lawChangeNEWSPECMIN = lawChange.lawChangeNEWSPECMIN;
  // change dollar amount, if applicable
  const int ind = lawChangeNEWSPECMIN->getInd();
  if ((ind == 1 || ind == 3) && static_cast<int>(date.getYear()) >=
    lawChangeNEWSPECMIN->getStartYear()) {
    return(lawChangeNEWSPECMIN->getAmount());
  }
  else {
    return(specMinAmountCalPL(date));
  }
}

/// <summary>Projects amounts dependent on benefit increases.</summary>
void PiaParamsLC::projectCpiinc()
{
  setAltCpiinc();
  resizeSpecMin();
  LawChangeNEWSPECMIN* lawChangeNEWSPECMIN = lawChange.lawChangeNEWSPECMIN;
  const int ind = lawChangeNEWSPECMIN->getInd();
  if (ind == 1 || ind == 3) {
    projectSpecMin(YEAR79, lawChangeNEWSPECMIN->getStartYear() - 1);
    projectSpecMin(lawChangeNEWSPECMIN->getStartYear(), maxyear);
  }
  else {
    projectSpecMin(YEAR79, maxyear);
  }
}

/// <summary>Returns maximum number of usable years for special minimum that
/// could ever be for any year of eligibility and year of benefit.</summary>
///
/// <returns>Maximum number of usable years for special minimum.</returns>
int PiaParamsLC::specMinMaxYears() const
{
  LawChangeNEWSPECMIN *lawChangeNEWSPECMIN = lawChange.lawChangeNEWSPECMIN;
  // change maximum years, if applicable
  if ((lawChangeNEWSPECMIN->getInd() % 4) >= 2) {
    return max(specMinMaxYearsPL(), lawChangeNEWSPECMIN->getYears() - 10);
  }
  else {
    return specMinMaxYearsPL();
  }
}

/// <summary>Returns maximum number of usable years for special minimum for
/// a specified year of eligibility and year of benefit.</summary>
///
/// <returns>Maximum number of usable years for special minimum.</returns>
///
/// <param name="eligYear">Year of eligibility for which maximum number of
/// years is desired.</param>
/// <param name="benYear">Year of benefit for which maximum number of years
/// is desired.</param>
int PiaParamsLC::specMinMaxYears( int eligYear, int benYear ) const
{
  LawChangeNEWSPECMIN *lawChangeNEWSPECMIN = lawChange.lawChangeNEWSPECMIN;
  // change maximum years, if applicable
  if (lawChangeNEWSPECMIN->isEffective(eligYear, benYear) &&
    (lawChangeNEWSPECMIN->getInd() % 4) >= 2) {
    return(lawChangeNEWSPECMIN->getYears() - 10);
  }
  else {
    return specMinMaxYearsPL();
  }
}

/// <summary>Returns full retirement age.</summary>
///
/// <returns>Full retirement age.</returns>
///
/// <param name="eligYear">Year of eligibility (age 62 for oab or
/// wife/husband, 60 for widow(er)).</param>
Age PiaParamsLC::fullRetAgeCal( int eligYear ) const
{
  LawChangeNRACHANGE *lawChangeNRACHANGE = lawChange.lawChangeNRACHANGE;
  if (lawChangeNRACHANGE->getInd() > 0) {
    // eliminate increase beyond age 65 for alternative 1
    if (lawChangeNRACHANGE->getInd() == 1)
      return age65;
    // eliminate plateau between 66 and 67 for alternative 2 or 3
    if (lawChangeNRACHANGE->getInd() > 1) {
      if (eligYear < 2000)  // age 65
        return age65;
      if (eligYear > 1999 && eligYear < 2005)
        // age 65 and 2 months to age 65 and 10 months
        return Age(65, 2 * (eligYear - 1999));
      if (eligYear > 2004 && eligYear < 2006)  // age 66
        return age66;
      if (eligYear > 2005 && eligYear < 2011)
        // age 66 and 2 months to age 66 and 10 months
        return Age(66, 2 * (eligYear - 2005));
      // if alternative 2, stop at age 67
      if (lawChangeNRACHANGE->getInd() == 2) {
        // age 67
        return age67;
      }
      // if alternative 3, continue on forever
      else {
        // calculate increase in nra over age 67, in months
        int months = (eligYear - 2011) / 2;
        // convert to years and months
        const int years = 67 + months / 12;
        months %= 12;
        return Age(years, months);
      }
    }
    // impossible parameter
    else {
      return age199;
    }
  }
  else {
    return(PiaParams::fullRetAgeCalPL(eligYear));
  }
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
Age PiaParamsLC::fullRetAgeCalDI( int eligYear, int currentYear ) const
{
  LawChangeNRACHANGE *lawChangeNRACHANGE = lawChange.lawChangeNRACHANGE;
  // assume that any change does not take effect for benefit calculations
  // until 2006
  if (lawChangeNRACHANGE->getInd() > 0 && currentYear > 2005) {
    return fullRetAgeCal(eligYear);
  }
  else {
    return PiaParams::fullRetAgeCalDIPL(eligYear, currentYear);
  }
}

/// <summary>Returns maximum DI beneficiary age at end of year.</summary>
///
/// <returns>Maximum DI beneficiary age at end of year.</returns>
///
/// <param name="year">Year of benefit.</param>
Age PiaParamsLC::maxDibAge( int year ) const
{
  LawChangeNRACHANGE *lawChangeNRACHANGE = lawChange.lawChangeNRACHANGE;
  if (lawChangeNRACHANGE->getInd() > 0) {
    // eliminate increase beyond age 65
    if (lawChangeNRACHANGE->getInd() == 1)
      return age65;
    // eliminate plateau between 66 and 67
    if (lawChangeNRACHANGE->getInd() > 1) {
      if (year < 2003)  // age 65
        return age65;
      if (year > 2002 && year < 2008)
        // age 65 and 2 months to age 65 and 10 months
        return Age(65, 2 * (year - 2002));
      if (year > 2007 && year < 2010)  // age 66
         return age66;
      if (year > 2009 && year < 2015)
        // age 66 and 2 months to age 66 and 10 months
        return Age(66, 2 * (year - 2009));
      // if alternative 2, stop at age 67
      if (lawChangeNRACHANGE->getInd() == 2) {
        // age 67
        return age67;
      }
      // if alternative 3, continue on forever
      else {
        // calculate years, taking 24 years to increment by one full year,
        // plus an extra year (explained in maxDibAge.xls)
        const int years = 67 + (year - 2015) / 25;
        // calculate year from which to determine months
        const int year25 = 2016 + (years - 67) * 25;
        // calculate months
        const int months = max(0, (year - year25) / 2);
        return Age(years, months);
      }
    }
    // impossible parameter
    else {
      return age199;
    }
  }
  else {
    return(PiaParams::maxDibAgePL(year));
  }
}

/// <summary>Returns factor for wife/husband benefit before age reduction.
/// </summary>
///
/// <returns>Factor for wife/husband benefit before age reduction.</returns>
///
/// <param name="eligYear">Spouse's year of eligibility.</param>
/// <param name="benYear">Year of benefit.</param>
double PiaParamsLC::factorAgedSpouseCal( int eligYear, int benYear ) const
{
  LawChangeWIFEFACTOR *lawChangeWIFEFACTOR = lawChange.lawChangeWIFEFACTOR;
  if (lawChangeWIFEFACTOR->isEffective(eligYear, benYear)) {
    if (lawChangeWIFEFACTOR->getInd() == 1) {
      // effective immediately
      return(LawChangeWIFEFACTOR::newSpouseFactor);
    }
    else {
      if (eligYear < lawChangeWIFEFACTOR->getStartYear()) {
        return PiaParams::factorAgedSpouseCalPL();
      }
      else {
        // phase in over 17 years
        const int numYears =
          (eligYear - lawChangeWIFEFACTOR->getStartYear() + 1);
        return(max(LawChangeWIFEFACTOR::newSpouseFactor,
          PiaParams::factorAgedSpouseCalPL() -
          numYears * LawChangeWIFEFACTOR::amtPerYear));
      }
    }
  }
  else {
    return PiaParams::factorAgedSpouseCalPL();
  }
}

/// <summary>Returns factor for widow/widower benefit before age reduction.
/// </summary>
///
/// <returns>Factor for widow/widower benefit before age reduction. This
/// could be an increased factor (up to 112.5 percent), if
/// <see cref="LawChange::lawChangeType::WIDFACTOR"/> is effective.</returns>
///
/// <param name="monthsArdri">Months of reduction.</param>
/// <param name="age">The age of widow/widower.</param>
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="benefitDate">The date of benefit.</param>
double PiaParamsLC::factorAgedWidCal( int monthsArdri, const Age& age,
int eligYear, const DateMoyr& benefitDate ) const
{
  LawChangeWIDFACTOR *lawChangeWIDFACTOR = lawChange.lawChangeWIDFACTOR;
  // if proposal to increase widow factor is effective, calculate new factor
  if (lawChangeWIDFACTOR->getInd() < 3 &&
    lawChangeWIDFACTOR->isEffective(eligYear, benefitDate.getYear())) {
    if (lawChangeWIDFACTOR->getInd() == 1) {
      // effective immediately
      return(LawChangeWIDFACTOR::newAgedWidFactor);
    }
    else {
      if (eligYear < lawChangeWIDFACTOR->getStartYear()) {
        return PiaParams::factorAgedWidCalPL(monthsArdri, age,
          benefitDate);
      }
      else {
        // phase in over 40 years
        const double factorPL =
          PiaParams::factorAgedWidCalPL(monthsArdri, age, benefitDate);
        const double excessFactor =
          LawChangeWIDFACTOR::newAgedWidFactor - factorPL;
        const int numYears =
          eligYear - lawChangeWIDFACTOR->getStartYear() + 1;
        const double ratio =
          (double)numYears / (double)LawChangeWIDFACTOR::numYears;
        return(min(LawChangeWIDFACTOR::newAgedWidFactor,
          factorPL + ratio * excessFactor));
      }
    }
  }
  // otherwise return present-law factor
  else {
    return PiaParams::factorAgedWidCalPL(monthsArdri, age, benefitDate);
  }
}

#pragma warning(disable:4100)
/// <summary>Returns factor for disabled widow/widower benefit before age
/// reduction.</summary>
///
/// <returns>Factor for disabled widow/widower benefit before age reduction.
/// </returns>
///
/// <remarks>Returns <see cref="PiaParams::factorDisWidCalPL"/>. There are no
/// law changes built in for disabled widow/widower benefits yet, so the
/// year of eligibility is currently unused.</remarks>
///
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="benefitDate">The date of benefit.</param>
double PiaParamsLC::factorDisWidCal( int eligYear,
const DateMoyr& benefitDate ) const
{
  return factorDisWidCalPL(benefitDate);
}
#pragma warning(default:4100)

/// <summary>Updates wage bases.</summary>
///
/// <remarks>This version of the function (with no arguments) handles wage
/// base changes as provided in the stored
/// <see cref="LawChangeWAGEBASECHG"/>.</remarks>
void PiaParamsLC::updateBases()
{
  int startyear;  // starting year of automatic projection
  LawChangeWAGEBASECHG *lawChangeWAGEBASECHG =
    lawChange.lawChangeWAGEBASECHG;
  // Handle regular wage bases
  const int ind = lawChangeWAGEBASECHG->getInd();
  const int firstYear = lawChangeWAGEBASECHG->getStartYear();
  const int lastYear = lawChangeWAGEBASECHG->getEndYear();
  if (ind == 1 || ind == 3) {
    for (int year = firstYear; year <= lastYear; year++) {
      baseOasdi[year] = lawChangeWAGEBASECHG->getBaseOasdi(year);
    }
    startyear = lawChangeWAGEBASECHG->getEndYear() + 1;
  }
  else {
    startyear = getIstart() + 1;
  }
  baseOasdi.project(fq, getCpiincArray(), startyear,
    baseOasdi.getLastYear());
  // Handle 1977-law wage bases
  if (ind == 2 || ind == 3) {
    for (int year = firstYear; year <= lastYear; year++) {
      base77[year] = lawChangeWAGEBASECHG->getBase77(year);
    }
    startyear = lawChangeWAGEBASECHG->getEndYear() + 1;
  }
  else {
    startyear = getIstart() + 1;
  }
  base77.project(fq, getCpiincArray(), startyear, base77.getLastYear());
  // HI bases are always set automatically
  baseHi.project(fq, getCpiincArray(), getIstart() + 1,
    baseHi.getLastYear());
}

/// <summary>Updates wage bases.</summary>
///
/// <remarks>This version of the function (with specified arguments) handles
/// ad hoc wage bases as provided in the arguments.</remarks>
///
/// <param name="userbase">User-entered bases.</param>
/// <param name="userbase77">User-entered old-law bases.</param>
/// <param name="ibasch">Indicator for base changes.</param>
/// <param name="firstYeart">First year to update.</param>
/// <param name="lastYeart">Last year to update.</param>
void PiaParamsLC::updateBases( const DoubleAnnual& userbase,
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

/// <summary>Calculates reduction factor for OAB.</summary>
///
/// <returns>Reduction factor for OAB.</returns>
///
/// <remarks>If there are fewer months of reduction than are already accounted
/// for under present law, or if there is no change in full retirement age,
/// then it simply returns <see cref="PiaParams::factorArCalPL"/>.</remarks>
///
/// <param name="monthsArdri">Number of months of reduction.</param>
double PiaParamsLC::factorArCal( int monthsArdri ) const
{
  LawChangeNRACHANGE *lawChangeNRACHANGE = lawChange.lawChangeNRACHANGE;
  if (lawChangeNRACHANGE->getInd() > 0 && monthsArdri > maxMonthsAr62_67) {
    const int maxMonthsArLC = LawChangeNRACHANGE::ageArChange - age62;
    if (monthsArdri <= maxMonthsArLC) {
      const int excessMonths = monthsArdri - maxMonthsAr62_67;
      // use 3/8 percent per month for years 6 and 7
      return (1.0 -
        static_cast< double >(maxMonthsAr62_65) * arMonthlyFactorOab62_65 -
        static_cast< double >(maxMonthsAr65_67) * arMonthlyFactor65_67 -
        static_cast<double>(excessMonths) *
        LawChangeNRACHANGE::arMonthlyFactor67_69);
    }
    else {
      const int excessMonths = monthsArdri - maxMonthsArLC;
      const int maxMonthsAr67_69 = maxMonthsArLC - maxMonthsAr62_67;
      // use 1/3 percent per month for years 8 and later
      return (1.0 -
        static_cast< double >(maxMonthsAr62_65) * arMonthlyFactorOab62_65 -
        static_cast< double >(maxMonthsAr65_67) * arMonthlyFactor65_67 -
        static_cast< double >(maxMonthsAr67_69) *
        LawChangeNRACHANGE::arMonthlyFactor67_69 -
        static_cast<double>(excessMonths) *
        LawChangeNRACHANGE::arMonthlyFactor69Plus);
    }
  }
  else {
    return PiaParams::factorArCalPL(monthsArdri);
  }
}

/// <summary>Calculates reduction factor for wife/husband.</summary>
///
/// <returns>Reduction factor for wife/husband, based on changed law.
/// </returns>
///
/// <remarks>If there are fewer months of reduction than are already accounted
/// for under present law, or if there is no change in full retirement age,
/// then it simply returns <see cref="PiaParams::factorArAgedSpouseCalPL"/>.
/// </remarks>
///
/// <param name="monthsArdri">Number of months of reduction.</param>
double PiaParamsLC::factorArAgedSpouseCal( int monthsArdri ) const
{
  LawChangeNRACHANGE *lawChangeNRACHANGE = lawChange.lawChangeNRACHANGE;
  if (lawChangeNRACHANGE->getInd() > 0 && monthsArdri > maxMonthsAr62_67) {
    const int maxMonthsArLC = LawChangeNRACHANGE::ageArChange - age62;
    if (monthsArdri <= maxMonthsArLC) {
      const int excessMonths = monthsArdri - maxMonthsAr62_67;
      // use 3/8 percent per month for years 6 and 7
      return (1.0 -
        static_cast< double >(maxMonthsAr62_65) * arMonthlyFactorSpouse62_65 -
        static_cast< double >(maxMonthsAr65_67) * arMonthlyFactor65_67 -
        static_cast<double>(excessMonths) *
        LawChangeNRACHANGE::arMonthlyFactor67_69);
    }
    else {
      const int excessMonths = monthsArdri - maxMonthsArLC;
      const int maxMonthsAr67_69 = maxMonthsArLC - maxMonthsAr62_67;
      // use 1/3 percent per month for years 8 and later
      return (1.0 -
        static_cast< double >(maxMonthsAr62_65) * arMonthlyFactorSpouse62_65 -
        static_cast< double >(maxMonthsAr65_67) * arMonthlyFactor65_67 -
        static_cast< double >(maxMonthsAr67_69) *
        LawChangeNRACHANGE::arMonthlyFactor67_69 -
        static_cast<double>(excessMonths) *
        LawChangeNRACHANGE::arMonthlyFactor69Plus);
    }
  }
  else {
    return PiaParams::factorArAgedSpouseCalPL(monthsArdri);
  }
}

/// <summary>Returns maximum amount of earnings in a childcare dropout year.
/// </summary>
///
/// <returns>Maximum amount of earnings in a childcare dropout year.
/// </returns>
///
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="benYear">Year for which amount is desired.</param>
double PiaParamsLC::getChildcareDropoutAmount( int eligYear,
int benYear ) const
{
  LawChangeCHILDCAREDROPOUT *lawChangeCHILDCAREDROPOUT =
    lawChange.lawChangeCHILDCAREDROPOUT;
  if (lawChangeCHILDCAREDROPOUT->isEffective(eligYear, benYear)) {
    return lawChangeCHILDCAREDROPOUT->getFqRatio() * fq[benYear];
  }
  return getChildcareDropoutAmountPL();
}

/// <summary>Returns maximum number of childcare dropout years.</summary>
///
/// <returns>Maximum number of childcare dropout years, based on changed law.
/// </returns>
///
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="benYear">Year for which number is desired.</param>
int PiaParamsLC::getMaxChildcareDropoutYears( int eligYear,
  int benYear ) const
{
  LawChangeCHILDCAREDROPOUT *lawChangeCHILDCAREDROPOUT =
    lawChange.lawChangeCHILDCAREDROPOUT;
  if (lawChangeCHILDCAREDROPOUT->isEffective(eligYear, benYear)) {
    return lawChangeCHILDCAREDROPOUT->getMaxYears();
  }
  return getMaxChildcareDropoutYearsPL();
}

/// <summary>Returns earliest possible retirement age for old-age benie or
/// aged spouse.</summary>
///
/// <returns>Earliest possible retirement age for old-age benie or aged
/// spouse, based on changed law.</returns>
///
/// <param name="sex">The sex of number holder.</param>
/// <param name="kbirth">The date of birth, adjusted to previous day.</param>
Age PiaParamsLC::earlyAgeOabCal( Sex::sex_type sex,
const boost::gregorian::date& kbirth ) const
{
  return(earlyAgeOabCalPL(sex, kbirth));
}

/// <summary>Returns maximum age of child for childcare dropout year or
///   childcare credit.</summary>
///
/// <returns>Maximum age of child for childcare dropout year.</returns>
int PiaParamsLC::getMaxChildcareAge() const
{
  LawChangeCHILDCAREDROPOUT *lawChangeCHILDCAREDROPOUT =
    lawChange.lawChangeCHILDCAREDROPOUT;
  if (lawChangeCHILDCAREDROPOUT->getInd() > 0) {
    return lawChangeCHILDCAREDROPOUT->getMaxAge();
  }
  LawChangeCHILDCARECREDIT *lawChangeCHILDCARECREDIT =
    lawChange.lawChangeCHILDCARECREDIT;
  if (lawChangeCHILDCARECREDIT->getInd() > 0) {
    return lawChangeCHILDCARECREDIT->getMaxAge();
  }
  return PiaParams::getMaxChildcareAgePL();
}

/// <summary>Resizes special minimum arrays.</summary>
void PiaParamsLC::resizeSpecMin()
{
  PiaParams::resizeSpecMin();
  int n = specMinMaxYears();
  // for changed law allocate space for initial amounts
  specMinPiaExtra.resize(n);
  specMinMfbExtra.resize(n);
}

/// <summary>Projects special minimum amounts.</summary>
///
/// <param name="baseYear">First year of special minimum amounts
/// (1979 for present law).</param>
/// <param name="lastYear">Last year of projection.</param>
void PiaParamsLC::projectSpecMin( int baseYear, int lastYear )
{
  const double amountPerYear = specMinAmountCal(DateMoyr(1, baseYear));
  for (int numYears = 0; numYears < specMinMaxYears(); numYears++) {
    double pia = (numYears + 1) * amountPerYear;
    double mfb = BenefitAmount::round(1.5 * pia, baseYear);
    DoubleAnnual& daPia = *specMinPia[numYears];
    DoubleAnnual& daMfb = *specMinMfb[numYears];
    // for present law, save initial amounts in 1978 slot
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
    // for changed law (after 2001) save initial amounts in extra vectors
    else {
      specMinPiaExtra[numYears] = pia;
      specMinMfbExtra[numYears] = mfb;
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

/// <summary>Returns special minimum pia as of a specified month.</summary>
///
/// <returns>Special minimum pia, based on changed law.</returns>
///
/// <param name="dateMoyr">Month and year for which pia is desired
/// (January 1979 or later).</param>
/// <param name="yoc">Years of coverage in excess of 10 (1-20).</param>
double PiaParamsLC::getSpecMinPia( const DateMoyr& dateMoyr, int yoc ) const
{
  LawChangeNEWSPECMIN *lawChangeNEWSPECMIN = lawChange.lawChangeNEWSPECMIN;
  unsigned year = dateMoyr.getYear();
  const unsigned month = dateMoyr.getMonth();
  if (static_cast<int>(month) < getMonthBeninc(year)) {
    // in 2001, apply extra 0.1 percent in August
    if (year == amend01.getYear() && month >= amend01.getMonth()) {
      return((yoc > 0) ? specMinPia2001[yoc-1] : 0.0);
    }
    // if this is the year where the amounts changed, use the special amounts
    else if ((lawChangeNEWSPECMIN->getInd() == 1 ||
      lawChangeNEWSPECMIN->getInd() == 3) &&
      static_cast<int>(year) == lawChangeNEWSPECMIN->getStartYear()) {
      return((yoc > 0) ? specMinPiaExtra[yoc-1] : 0.0);
    }
    else {
      year--;
    }
  }
  // if did not return above, use regular special minimum table
  return((yoc > 0) ? (*specMinPia[yoc-1])[year] : 0.);
}

/// <summary>Returns special minimum mfb as of a specified month.</summary>
///
/// <returns>Special minimum mfb, based on changed law.</returns>
///
/// <param name="dateMoyr">Month and year for which mfb is desired
/// (January 1979 or later).</param>
/// <param name="yoc">Years of coverage in excess of 10 (1 to 20).</param>
double PiaParamsLC::getSpecMinMfb( const DateMoyr& dateMoyr, int yoc ) const
{
  LawChangeNEWSPECMIN *lawChangeNEWSPECMIN = lawChange.lawChangeNEWSPECMIN;
  unsigned year = dateMoyr.getYear();
  const unsigned month = dateMoyr.getMonth();
  if (static_cast<int>(month) < getMonthBeninc(year)) {
    // in 2001, apply extra 0.1 percent in August
    if (year == amend01.getYear() && month >= amend01.getMonth()) {
      return((yoc > 0) ? specMinMfb2001[yoc-1] : 0.0);
    }
    // if this is the year where the amounts changed, use the special amounts
    else if ((lawChangeNEWSPECMIN->getInd() == 1 ||
      lawChangeNEWSPECMIN->getInd() == 3) &&
      static_cast<int>(year) == lawChangeNEWSPECMIN->getStartYear()) {
      return((yoc > 0) ? specMinMfbExtra[yoc-1] : 0.0);
    }
    else {
      year--;
    }
  }
  // if did not return above, use regular special minimum table
  return((yoc > 0) ? (*specMinMfb[yoc-1])[year] : 0.0);
}

/// <summary>Sets data from historical average wages and benefit increase
/// file.</summary>
///
/// <param name="awbiData">Structure with historical average wages and
///  benefit increases.</param>
void PiaParamsLC::setData( AwbiData& awbiData )
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
/// <returns>Number of years of marriage needed to be eligible for a divorced
/// spouse benefit.</returns>
int PiaParamsLC::getMarrLengthForDivBen( const DateMoyr& dateMoyr ) const
{
  const int year = dateMoyr.getYear();
  LawChangeMARRLENGTH *lawChangeMARRLENGTH = lawChange.lawChangeMARRLENGTH;
  if (lawChangeMARRLENGTH->isEffective(year, year)) {
    return lawChangeMARRLENGTH->getMarrLengthForDivBen();
  }
  else {
    return getMarrLengthForDivBenPL(dateMoyr);
  }
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
/// <param name="oabPia">PIA upon which widow(er) benefit is based. Note
/// that this is not a re-indexed widow(er) PIA, since the OAB must have
/// survived to age 62.</param>
/// <param name="oabBen">Benefit that deceased OAB was receiving.</param>
/// <param name="benefitDate">The date of benefit.</param>
double PiaParamsLC::ribLimCal( double widowBen, double oabPia, double oabBen,
const DateMoyr& benefitDate ) const
{
  LawChangeWIDFACTOR *lawChangeWIDFACTOR = lawChange.lawChangeWIDFACTOR;
  // if law change is effective, do not apply RIB-LIM
  if (lawChangeWIDFACTOR->getInd() == 3 &&
    lawChangeWIDFACTOR->isEffective(benefitDate.getYear(),
    benefitDate.getYear())) {
    return widowBen;
  }
  // otherwise apply present-law RIB-LIM
  else {
    return ribLimCalPL(widowBen, oabPia, oabBen, benefitDate);
  }
}
