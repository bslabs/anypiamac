// Functions for the <see cref="OldStart"/> class to manage old-start pia
// calculations.
//
// $Id: oldstart.cpp 1.71 2017/09/18 10:37:19EDT 277133 Development  $

#include <cmath>  // for floor
#include "OldStart.h"
#include "pibtable.h"
#include "oactcnst.h"
#include "PiaException.h"
#include "piaparms.h"
#include "WageBase.h"
#include "DebugCase.h"
#if defined(DEBUGCASE)
#include <sstream>
#include "Trace.h"
#endif

using namespace std;

// <summary>Factor for amount of increment per year (0.01).</summary>
const double OldStart::INC_PER_YEAR = 0.01;

/// <summary>Initializes an OldStart instance.</summary>
///
/// <param name="newWorkerData">Worker data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
/// <param name="newEntDate">The date of entitlement.</param>
OldStart::OldStart( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear,
const DateMoyr& newEntDate ) :
OldPia(newWorkerData, newPiaData, newPiaParams, newMaxyear,
"Old-Start Calculation", OLD_START), bendMfb(), percMfb(), ameOs(0),
divisorOs(0), entDate(newEntDate), incyrs(0), methodOs(OS1939), pib(0),
pibInc(0)
{
  fill_n(portionAme, 2, 0.0);
  fill_n(portionPiaElig, 4, 0.0);
}

/// <summary>Destructor.</summary>
OldStart::~OldStart()
{ }

/// <summary>Initializes computed values to zero.</summary>
void OldStart::initialize()
{
  PiaMethod::initialize();
  methodOs = OS1939;
  pib = pibInc = 0.0;
  bendMfb.deleteContents();
  fill_n(portionAme, 2, 0.0);
  fill_n(portionPiaElig, 4, 0.0);
}

/// <summary>Determines applicability of method using stored data.</summary>
///
/// <remarks>The version with no arguments calls the static version with 1
/// argument.</remarks>
///
/// <returns>True if method is applicable, false otherwise.</returns>
bool OldStart::isApplicable()
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", piaData.getQcTotal50() = "
      << piaData.getQcTotal50()
      << ", piaData.getBirthDateMinus1().getYear() = "
      << piaData.getBirthDateMinus1().year()
      << ": At top of OldStart::isApplicable";
    Trace::writeLine(strm.str());
  }
#endif
   if (isApplicable(piaData)) {
      setApplicable(APPLICABLE);
#if defined(DEBUGCASE)
   if (isDebugPid(workerData.getIdNumber())) {
      string s("");
      s += workerData.getIdString();
      s += ": Returning true from OldStart::isApplicable";
      Trace::writeLine(s);
   }
#endif
      return(true);
   }
   else {
#if defined(DEBUGCASE)
   if (isDebugPid(workerData.getIdNumber())) {
      string s("");
      s += workerData.getIdString();
      s += ": Returning false from OldStart::isApplicable";
      Trace::writeLine(s);
   }
#endif
      return(false);
   }
}

/// <summary>Determines applicability of method using passed data.</summary>
///
/// <returns>True if method is applicable, false otherwise.</returns>
///
/// <param name="piaData">Pia calculation data.</param>
bool OldStart::isApplicable( const PiaData& piaData )
{
  if (piaData.getQcTotal50() < 1)
    return(false);
  const int year = piaData.getBirthDateMinus1().year();
  return (year < 1929 || (year < 1951 && piaData.getQcTotal51() < 6));
}

/// <summary>Computes old start PIA.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_OLDSTART1"/> if date of entitlement is out of range
/// (only in debug mode); of type <see cref="PIA_IDS_OLDSTART2"/> if old start
/// method number is out of range (only in debug mode).</exception>
void OldStart::calculate()
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Starting OldStart::calculate");
  }
#endif
  setMethodOs(methodOsCal());
  const AverageWage& earnings = workerData.getTotalize() ?
    piaData.earnTotalizedLimited : piaData.earnOasdiLimited;
  imputeEarnings();
  // fill out remainder of wearn
  int yr2 = piaData.getEarnYear();
  // if 1977 old-start using Dec 1978 frozen PIA table, do not use
  // earnings in year of eligibility or later
  if (getMethodOs() == OS1977_79 && yr2 >= piaData.getEligYear())
    yr2 = piaData.getEligYear() - 1;
  for (int yr = 1951; yr <= yr2; yr++) {
    if (!piaData.freezeYears.isFreezeYear(yr))
      earnIndexed[yr] = earnings[yr];
  }
  // order the earnings and compute average monthly earnings
  orderEarnings(YEAR37, yr2, piaData.compPeriodOld.getN());
  totalEarnCal(YEAR37, yr2, piaData.compPeriodOld.getN());
  // save old-start AME
  setAmeOs(static_cast<int>(getAme()));
  // calculate old-start AME
  portionAme[0] =
    static_cast<double>(min(getAmeOs(), piaParams.getBendOS(1)));
  double temp =
    static_cast<double>(min(piaParams.getBendOS(2) - piaParams.getBendOS(1),
    getAmeOs() - piaParams.getBendOS(1)));
  portionAme[1] = max(temp, 0.);
  piasub = 0.0;
  for (int i1 = 0; i1 < 2; i1++)  // find PIB before incrementing
    piasub += piaParams.getPercOS(i1) * portionAme[i1];
  setPib(piasub);
  // calculate increment years
  setIncyrs(incrementCal());
  // increase PIB 1% for each increment year
  setPibInc();
  setDirty();
  // do 1939 Amendments
  if (getMethodOs() == OS1939 &&
    workerData.getBenefitDate() < PiaParams::amend50) {
    oldStart39Cal();
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm.precision(2);
      strm << workerData.getIdString() << ", pia = " << piaEnt.get()
        << ": Returning from OldStart::calculate, using oldStart39Cal";
      Trace::writeLine(strm.str());
    }
#endif
    return;
  }
  // use 1950 conversion table
  if (workerData.getBenefitDate() < PiaParams::amend58 && 
      (getMethodOs() == OS1939 || getMethodOs() == OS1950)) {
    yearCpi[FIRST_YEAR] = 1950;
    oldStart50Cal();
#if defined(DEBUGCASE)
    if (isDebugPid(workerData.getIdNumber())) {
      ostringstream strm;
      strm.precision(2);
      strm << workerData.getIdString() << ", pia = " << piaEnt.get()
        << ": Returning from OldStart::calculate, using oldStart50Cal";
      Trace::writeLine(strm.str());
    }
#endif
    return;
  }
  // find PIB in 1958 PIB-PIA conversion table.
  // The lowest possible value varies depending on the table used.
  int i3;
  if (workerData.getBenefitDate() < PiaParams::amend61) {
    i3 = 0;
  }
  else {
    if (workerData.getBenefitDate() < PiaParams::amend672) {
      i3 = 7;
    }
    else {
      i3 = (workerData.getBenefitDate() < PiaParams::amend69) ? 11 : 12;
    }
  }
  while (getPibInc() > Pib58Pib::getAt(i3) && i3 < 62)
    i3++;
  setAme(static_cast<double>(Pib58Ame::getAt(i3)));
  if (getMethodOs() == OS1977_79) {
    // if eligible in 1982 or later, extend down below minimum
    if ((i3 == 0) && (piaData.getEligYear() > 1981))
      setAme(floor(getPibInc() * 76.0 / 16.20 + 0.999));
    yearCpi[FIRST_YEAR] = piaData.getEligYear() - 1;
    yearCpi[YEAR_ELIG] = piaData.getEligYear();
    // find corresponding PIA in Dec 1978 PIA Table
    setTableNum(cpiBase(PiaParams::amend771, true, getAme(), false));
    piaElig[yearCpi[FIRST_YEAR]] = piasub;
    // apply windfall elimination provision
    if (wep_app()) {
      const double piaelt = piaElig[yearCpi[FIRST_YEAR]];
      const double halfpiaelt =
        BenefitAmount::round(0.5 * piaelt, piaData.getEligYear() - 1);
      setPiaWindfall();
      const double test = BenefitAmount::round(0.5 * workerData.getPubpen(),
        piaData.getEligYear() - 1);
      // reduction cannot be more than half of pension or half of pia
      piaElig[yearCpi[FIRST_YEAR]] = (halfpiaelt < test) ?
        halfpiaelt : piaelt - test;
      piaEnt.set(piaElig[yearCpi[FIRST_YEAR]]);
    }
    // apply CPI increases to PIA
    setYearCpi();
    piaEnt.set(applyColas(piaElig, piaData.getEligYear(),
      workerData.getBenefitDate()));
    bendMfb.project(piaData.getEligYear(),piaParams.getFqArray());
    // apply totalization
    if (workerData.getTotalize())
      prorate();
    // find AIME MFB from wage-indexed formula
    setPortionPiaElig(piaElig[yearCpi[FIRST_YEAR]], portionPiaElig, bendMfb);
    mfbElig[yearCpi[FIRST_YEAR]] =
      mfbCal(portionPiaElig, percMfb, piaData.getEligYear() - 1);
    // apply CPI increases to MFB
    mfbEnt.set(applyColas(mfbElig, piaData.getEligYear(),
      workerData.getBenefitDate()));
  }
  else {
    if (workerData.getBenefitDate() < PiaParams::amend742) {
      setTableNum(oldPiaCal());
    }
    else {
      // handle benefit date in amend742 or later
#if !defined(NDEBUG)
      if (entDate.getYear() < YEAR37)
        throw PiaException(PIA_IDS_OLDSTART1);
#endif
      int i4 = entDate.getYear();
      yearCpi[YEAR_ENT] = (i4 < YEAR51) ? i4 :
          (static_cast<int>(entDate.getMonth()) >=
          piaParams.getMonthBeninc(i4)) ? i4 : i4 - 1;
#if !defined(NDEBUG)
      if (workerData.getBenefitDate().getYear() < YEAR37)
        throw PiaException(PIA_IDS_OLDSTART2);
#endif
      i4 = workerData.getBenefitDate().getYear();
      yearCpi[YEAR_BEN] =
        (static_cast<int>(workerData.getBenefitDate().getMonth()) >=
        piaParams.getMonthBeninc(i4)) ? i4 : i4 - 1;
      yearCpi[FIRST_YEAR] = 1975;
      setTableNum(cpiBase(workerData.getBenefitDate(), false, getAme(),
        false));
    }
    piaEnt.set(piasub);
    mfbEnt.set(mfbsub);
  }
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm.precision(2);
    strm << workerData.getIdString() << ", pia = " << piaEnt.get()
      << ": Returning from OldStart::calculate";
    Trace::writeLine(strm.str());
  }
#endif
  return;
}

/// <summary>Calculates 1939 old-start PIA and MFB.</summary>
void OldStart::oldStart39Cal()
{
  // PIB is PIA, with $10 minimum
  piaEnt = max(10.0, getPibInc());
  // MFB is 80% of old-start AMW
  mfbsub = 0.8 * static_cast<double>(getAmeOs());
  // limit MFB to $85
  mfbsub = min(mfbsub, 85.0);
  // limit MFB to twice PIA
  mfbsub = min(mfbsub, 2.0 * piaEnt.get());
  // MFB is at least $20
  mfbEnt = max(mfbsub, 20.0);
}

/// <summary>Calculates 1950 old-start PIA and MFB.</summary>
void OldStart::oldStart50Cal()
{
  int i1 = 0;
  while (getPibInc() > Pib50Pib::getAt(i1)+.005 && i1 < 485)
    i1++;
  // PIAs in conversion table start at $20 and increase at $.10 per interval
  piaEnt.set(20.00 + static_cast<double>(i1) / 10.0);
  piaElig[yearCpi[FIRST_YEAR]] = piaEnt.get();
  mfbElig[yearCpi[FIRST_YEAR]] = Pib50Mfb::getAt(i1);
  if (workerData.getBenefitDate() < PiaParams::amend52) {
    mfbEnt.set(mfbElig[yearCpi[FIRST_YEAR]]);
  }
  else {
    // increase is greater of $5 or 12.5%
    const double test = piaParams.applyCola(piaEnt.get(), 1952);
    piasub = max(test, piaEnt.get() + 5.0);
    piaEnt.set(piasub);
    if (workerData.getBenefitDate() < PiaParams::amend54) {
      mfbEnt.set(Pib52Mfb::getAt(i1));
      int i2 = 0;
      while (piaEnt.get()-0.005 > Pib52Ame::getAt(i2)) {
        i2++;
      }
      setAme(static_cast<double>(45 + i2));
    }
    else {
      if (i1 < 329) {
        piasub = piaEnt.get() + 5.0;
        piaEnt.set(piasub);
      }
      else {
        piaEnt.set(Pib54Pia::getAt(i1 - 329));
      }
      mfbEnt.set(Pib54Mfb::getAt(i1));
      int i2 = 0;
      while (piaEnt.get()-0.005 > Pib54Ame::getAt(i2)) {
        i2++;
      }
      setAme(static_cast<double>(55 + i2));
    }
  }
}

/// <summary>Determines correct old-start method.</summary>
///
/// <returns>Correct old-start method number.</returns>
OldStart::OldStartType OldStart::methodOsCal()
{
  // if entitled prior to September 1950, use 1939 amendments
  if (entDate < PiaParams::amend50)
    return(OS1939);
  // if entitled from September 1950 to December 1958, use 1950 conversion
  // table
  if (entDate < PiaParams::amend58)
    return(OS1950);
  // if entitled from January 1959 to December 1965, use 1958 conversion
  // table
  if (entDate < PiaParams::amend653)
    return(OS1958);
  // if entitled after 1965, but eligible prior to 1966, use 1958
  // conversion table or 1990 amendments
  if (piaData.getEligYear() < 1966)
    return(piaData.getAmend90() ? OS1990 : OS1958);
  // otherwise, if entitled in 1966-67, use 1965 old-start
  if (entDate < PiaParams::amend671)
    return(OS1965);
  // if entitled after 1967, but died prior to 1968, use 1965
  // old-start or 1990 amendments
  if (piaData.getEligYear() < 1968 &&
    getIoasdi() == WorkerDataGeneral::SURVIVOR)
    return(piaData.getAmend90() ? OS1990 : OS1965);
  // separate into old-law and new-law
  if (piaData.getBirthDateMinus1().year() < 1916 ||
    piaData.getEligYear() < 1978) {
    // if born before 1916, use 1967 old-start or 1990 amendments
    if (piaData.getBirthDateMinus1().year() < 1916)
      return(piaData.getAmend90() ? OS1990 : OS1967);
    // if born in 1916 or later, use 1965 old-start or 1990 amendments
    else
      return(piaData.getAmend90() ? OS1990 : OS1965);
  }
  else {
    // if eligible in 1978, use old-start method 5
    return (piaData.getEligYear() == 1978) ? OS1977_78 : OS1977_79;
  }
}

/// <summary>Imputes yearly earnings prior to 1951.</summary>
void OldStart::imputeEarnings()
{
  const AverageWage& earnings = workerData.getTotalize() ?
    piaData.earnTotalizedLimited : piaData.earnOasdiLimited;
  if (getMethodOs() == OS1939 || getMethodOs() == OS1950 ||
    getMethodOs() ==  OS1958 || getMethodOs() == OS1965)
    // old methods use actual yearly earnings
    earnIndexed.assign(earnings, YEAR37, 1950);
  else {
    // zero out earnings prior to 1951
    earnIndexed.assign(0.0, YEAR37, 1950);
    int i2,i3;  // first and last years of imputed earnings
    if (getMethodOs() == OS1967) {
      // 1967 old-start allocates over 9 years, up to $3000 per year
      setDivisorOs(9);
      i2 = 1942;
      i3 = 1950;
    }
    else {
      // 1977 old-start allocates over variable number of years
      i3 = 1950;
      if (getIoasdi() == WorkerDataGeneral::SURVIVOR) {
        i3 = min(i3,
          static_cast<int>(workerData.getDeathDate().year()) - 1);
      }
      if (workerData.getValdi() == 1)
        i3 = min(i3, piaData.freezeYears.getYear1() - 1);
      if (workerData.getValdi() == 2)
        i3 = min(i3, piaData.freezeYears.getYear3() - 1);
      i3 = max(i3, YEAR37);
      const int year21 = static_cast<int>(
        piaData.getBirthDateMinus1().year()) + 21;
      i2 = min(i3, max(YEAR37, min(year21, 1950)));
      setDivisorOs(i3 - i2 + 1);
    }
    const double baset = piaParams.getBaseOasdi(YEAR37);
    const double earnTotal50 =
      piaData.getEarnTotal50(workerData.getTotalize() ?
      PiaData::EARN_WITH_TOTALIZATION : PiaData::EARN_NO_TOTALIZATION);
    const double divisorOs = static_cast<double>(getDivisorOs());
    if (earnTotal50 > baset * divisorOs) {
      if (workerData.getBirthDate().year() > YEAR37) {
        // assign earnings backwards in $3000 increments.
        // stop at birth year if necessary.
        const int i6 = static_cast<int>(floor(earnTotal50 + .001) / baset);
        const int birthyear = workerData.getBirthDate().year();
        if (i6 > i3 - birthyear + 1) {
          earnIndexed.assign(baset, birthyear, i3);
        }
        else {
           earnIndexed.assign(baset, i3 - i6 + 1, i3);
           earnIndexed[i3 - i6] = earnTotal50 - baset * i6;
        }
      }
      else {
        // if over $3000 allocated per year, allocate $3000 per year up to 14
        const int i5 = (earnTotal50 > 14.0 * baset) ? 14 :
          static_cast<int>(floor(earnTotal50 + .001) / baset);
        setDivisorOs(min(i5, i3 - 1936));
        i2 = max(YEAR37, i3 - getDivisorOs());
        earnIndexed.assign(baset, i2 + 1, i3);
        // if any earnings remain, put in next prior year
        earnIndexed[i2] = earnTotal50 - floor(earnTotal50 / baset) * baset;
      }
    }
    else {
      // if less than $3000 per year, fill out earnings array
      earnIndexed.assign(earnTotal50 / divisorOs, i2, i3);
    }
  }
}

/// <summary>Calculates number of increment years for old-start.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_OLDSTART2"/> if old start method number is out of
/// range.</exception>
///
/// <returns>Number of increment years.</returns>
int OldStart::incrementCal() const
{
  switch (getMethodOs())
  {
  case OS1939:
  case OS1950:
  case OS1958:
  case OS1965:
    // under 1939 amendments, 1950 old-start, or 1965 old-start, one
    // increment year for each year of at least $200 of earnings
    return incrementOldCal();
  case OS1967:
    // under 1967 old-start, 14 increment years
    return 14;
  case OS1977_78:
  case OS1977_79:
  case OS1990:
    // under 1977 old-start, one increment year for each $1650 of
    // cumulative earnings, with minimum 4 and maximum 14
    return min(14, max(4, (int)(piaData.getEarnTotal50(
      workerData.getTotalize() ? PiaData::EARN_WITH_TOTALIZATION :
      PiaData::EARN_NO_TOTALIZATION) / PiaParams::amtperincyr)));
  default:
    throw PiaException(PIA_IDS_OLDSTART2);
  }
}

/// <summary>Calculates number of increment years for old old-start.</summary>
///
/// <returns>Number of increment years.</returns>
int OldStart::incrementOldCal() const
{
  int temp = 0;  // number of increment years
  const AverageWage& earnings = workerData.getTotalize() ?
    piaData.earnTotalizedLimited : piaData.earnOasdiLimited;
  for (int yr = YEAR37; yr <= 1950; yr++) {
    if (earnings[yr] >= 200.0)
      temp++;
  }
  return(temp);
}
