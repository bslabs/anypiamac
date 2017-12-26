// Functions for the <see cref="DibGuar"/> class to manage disability
// guarantee pia calculations.
//
// $Id: DibGuar.cpp 1.15 2017/09/18 10:37:23EDT 277133 Development  $

#include "DibGuar.h"
#include "DebugCase.h"
#if defined(DEBUGCASE)
#include "Trace.h"
#endif

using namespace std;

/// <summary>Initializes a disability guarantee pia method.</summary>
///
/// <remarks>Initializes all amounts to zero, then calls
/// <see cref="PiaMethod::initialize"/>.</remarks>
///
/// <param name="newWorkerData">Worker data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
DibGuar::DibGuar( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear ) :
PiaMethod(newWorkerData, newPiaData, newPiaParams, newMaxyear,
"Disability Guarantee", DIB_GUAR), colasApply(false), whichPeriod(0),
rawPia(0.0), rawMfb(0.0), cessationMfbOrig(0.0), cessationMfbConv(0.0),
entDeathDate()
{
  initialize();
}

/// <summary>Destructor.</summary>
DibGuar::~DibGuar()
{ }

/// <summary>Determines applicability of method using stored values.</summary>
///
/// <remarks>Returns true if eligibility is after 1978, and this is a
/// disability case with more than 1 period of disability, or if this is an
/// old-age or survivor case with any period of disability.
///
/// This version with no arguments calls the static version with 3 arguments.
/// </remarks>
///
/// <returns>True if method is applicable, false otherwise.</returns>
bool DibGuar::isApplicable()
{
  if (isApplicable(workerData, piaData, getIoasdi())) {
    setApplicable(APPLICABLE);
    return(true);
  }
  else {
    return(false);
  }
}

/// <summary>Determines applicability of method using passed values.</summary>
///
/// <remarks>Returns true if eligibility is after 1978, and this is a
/// disability case with more than 1 period of disability, or if this is an
/// old-age or survivor case with any period of disability.</remarks>
///
/// <returns>True if method is applicable, false otherwise.</returns>
///
/// <param name="workerData">Worker data.</param>
/// <param name="piaData">Pia calculation data.</param>
/// <param name="ioasdi">Type of benefit.</param>
bool DibGuar::isApplicable( const WorkerDataGeneral& workerData,
const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi )
{
  return ((piaData.getEligYear() > 1978) &&
    ((workerData.getValdi() > 1) || (workerData.getValdi() > 0 &&
    (ioasdi == WorkerDataGeneral::OLD_AGE ||
    ioasdi == WorkerDataGeneral::SURVIVOR))));
}

/// <summary>Computes disability guarantee PIA.</summary>
///
/// <remarks>Gets the pia at cessation from the prior disability period. For
/// old-age or survivor, that is the most recent period. For disability,
/// that is the prior period of disability.
///
/// Apply colas from the prior cessation date to the new entitlement date
/// only if the cessation was within 12 months of the new entitlement.
/// </remarks>
void DibGuar::calculate()
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Starting DibGuar::calculate");
  }
#endif
  // use prior disability in disability case, else most recent disability
  whichPeriod = (getIoasdi() == WorkerDataGeneral::DISABILITY) ? 1 : 0;
  const DisabPeriod& disabPeriod = workerData.disabPeriod[whichPeriod];
  entDeathDate = entDeathDateCal();
  // apply cpi increases beginning with year of entitlement
  // if there was more than a 12 month gap in benefits. Otherwise
  // apply colas starting with prior year of cessation.
  const int months = disabPeriod.getCessationDate().getMonths(entDeathDate);
  colasApply = (months < 13);
  yearCpi[YEAR_ELIG] = yearCpi1Cal();
  yearCpi[FIRST_YEAR] = yearCpi[YEAR_ELIG] - 1;
  piaElig[yearCpi[FIRST_YEAR]] = disabPeriod.getCessationPia();
  setYearCpi();
  piaEnt.set(applyColasElig(piaElig, yearCpi[YEAR_ELIG],
    workerData.getBenefitDate(), piaData.getEligYear()));
  diMaxApplies = (disabPeriod.getOnsetDate().year() > 1978 &&
    workerData.getEarliestEntDate() >= PiaParams::amend80);
  eligYear = disabPeriod.getOnsetDate().year();
  convertedMfbType = convertedMfbTypeCal();
  cessationMfbOrig = workerData.disabPeriod[whichPeriod].getCessationMfb();
  cessationMfbConv = cessationMfbCal(disabPeriod);
  mfbElig[yearCpi[FIRST_YEAR]] = cessationMfbConv;
  mfbEnt.set(applyColasElig(mfbElig, yearCpi[YEAR_ELIG],
    workerData.getBenefitDate(), piaData.getEligYear()));
  setDirty();
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Returning from DibGuar::calculate");
  }
#endif
}

/// <summary>Returns first year that benefit increases are applied.</summary>
///
/// <returns>First year that benefit increases are applied.</returns>
///
/// <remarks>This should be called after colasApply has been calculated. If
/// colasApply is true, returns year of prior disability cessation. Otherwise
/// returns year of entitlement in life case, year of death in survivor
/// case.</remarks>
int DibGuar::yearCpi1Cal() const
{
  const DateMoyr dateMoyr = dateCpiCal();
  const int year = dateMoyr.getYear();
  return (static_cast< int >(dateMoyr.getMonth()) <=
    piaParams.getMonthBeninc(year)) ? year : year + 1;
}

/// <summary>Returns the date as of which the initial pia is
///  calculated.</summary>
///
/// <returns>The date as of which the initial pia is calculated.</returns>
///
/// <remarks>Cpi increases are applied after this date.</remarks>
DateMoyr DibGuar::dateCpiCal() const
{
  return (colasApply) ?
    workerData.disabPeriod[whichPeriod].getCessationDate() : entDeathDate;
}

/// <summary>Returns date of entitlement in a life case, or date of death
/// in a survivor case.</summary>
///
/// <returns>The date of entitlement in a life case, or date of death in a
/// survivor case.</returns>
DateMoyr DibGuar::entDeathDateCal() const
{
  // do not use date of entitlement in a survivor case
  return (getIoasdi() == WorkerDataGeneral::SURVIVOR) ?
    DateMoyr(workerData.getDeathDate()) : workerData.getEntDate();
}

/// <summary>Calculates the applicable mfb at cessation date.</summary>
///
/// <returns>The applicable mfb at cessation date.</returns>
///
/// <remarks>For a disability benefit, or any benefit prior to January 1996,
/// the mfb is simply the one at the date of cessation of the prior disability
/// benefit. For old-age or survivors in 1996 or later, if the prior mfb was
/// based on the Disability Maximum, the pia at cessation must be deconverted
/// back to eligibility, then that is used to compute a raw mfb, and that is
/// converted forward to the cessation date.
///
/// Also sets rawPia and rawMfb, if the deconversion is necessary.</remarks>
///
/// <param name="disabPeriod">The period of disability being considered.
/// </param>
double DibGuar::cessationMfbCal( const DisabPeriod& disabPeriod )
{
  switch (convertedMfbType) {
  case POST1995_NOCHANGE:
    return cessationMfbOrig;
  case POST1995_DECONVERTED:
  {
    const int year2 = disabPeriod.getCessationDate().getYear();
    const int numYears = year2 - eligYear;
    rawPia = piaParams.deconvertPia(eligYear, numYears,
      disabPeriod.getCessationPia(), disabPeriod.getCessationDate());
    bendMfb.project(eligYear, piaParams.getFqArray());
    setPortionPiaElig(rawPia, portionPiaElig, bendMfb);
    rawMfb = mfbCal(portionPiaElig, percMfb, eligYear - 1);
    mfbElig[eligYear - 1] = rawMfb;
    return applyColas(mfbElig, eligYear,
      disabPeriod.getCessationDate().getMonthBefore());
  }
  case PRE1996_PRE1979_LAST12:
  case PRE1996_POST1978_LAST12:
  case PRE1996_NOTLAST12:
    bendMfb.project(piaData.getEligYear(), piaParams.getFqArray());
    setPortionPiaElig(disabPeriod.getCessationPia(), portionPiaElig,
      bendMfb);
    return mfbCal(portionPiaElig, percMfb, piaData.getEligYear() - 1);
  default:
    throw PiaException("DibGuar::cessationMfbCal: Illegal convertedMfbType");
  }
}

/// <summary>Returns the type of mfb at cessation, after conversion.</summary>
///
/// <returns>The type of mfb at cessation, after conversion.</returns>
///
/// <remarks>The type of mfb first depends on the date of entitlement to the
/// subsequent benefit. If it is January 1996 or later, then the type is
/// POST1995_NOCHANGE if the new benefit is a disability benefit, or if the
/// Disability Maximum does not apply. The type is POST1995_DECONVERTED if it
/// is an old-age or survivor benefit and the Disability Maximum applies.
///
/// If the date of entitlement to the subsequent benefit is before January
/// 1996, then the type depends on if the prior DIB entitlement continued in
/// the 12 months preceding the new entitlement. If so, the type is
/// PRE1996_PRE1979_LAST12 if eligibility to the prior DIB benefit was before
/// 1979, or PRE1996_POST1978_LAST12 if eligibility to the prior DIB benefit
/// was after 1978. Otherwise, the type is PRE1996_NOTLAST12.</remarks>
DibGuar::ConvertedMfbType DibGuar::convertedMfbTypeCal() const
{
  if (entDeathDate > DateMoyr(12, 1995)) {
    if (diMaxApplies && getIoasdi() != WorkerDataGeneral::DISABILITY) {
      // Subsequent entitlement in January 1996 or later, new benefit is
      // OASI, and Disability Maximum applies (requires deconversion to prior
      // eligibility year).
      return POST1995_DECONVERTED;
    }
    else {
      // Subsequent entitlement in January 1996 or later, new benefit is DI,
      // or the Disability Maximum does not apply (no conversion required).
      return POST1995_NOCHANGE;
    }
  }
  else {
    if (colasApply) {
      if (eligYear < 1979) {
        // Subsequent entitlement before January 1996, prior DIB continued in
        // the 12 months preceding the new entitlement, prior DIB eligibility
        // was before 1979.
        return PRE1996_PRE1979_LAST12;
      }
      else {
        // Subsequent entitlement before January 1996, prior DIB continued
        // in the 12 months preceding the new entitlement, prior DIB
        // eligibility was after 1978.
        return PRE1996_POST1978_LAST12;
      }
    }
    else {
      // Subsequent entitlement before January 1996, prior DIB did not
      // continue in the 12 months preceding the new entitlement.
      return PRE1996_NOTLAST12;
    }
  }
}
