// Functions for the <see cref="SpecMin"/> class to manage special minimum pia
// calculations.
//
// $Id: SpecMin.cpp 1.48 2011/10/11 15:23:43EDT 044579 Development  $

#include "SpecMin.h"
#include "piaparms.h"
#include "oactcnst.h"
#include "DebugCase.h"
#if defined(DEBUGCASE)
#include <sstream>
#include "Trace.h"
#endif

using namespace std;

/// <summary>Initializes this instance.</summary>
///
/// <remarks>Initializes the member variables to 0, then calls
/// <see cref="initialize"/>. </remarks>
///
/// <param name="newWorkerData">Worker basic data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newMaxyear">Maximum year of projection.</param>
SpecMin::SpecMin( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams, int newMaxyear ) :
OldPia(newWorkerData, newPiaData, newPiaParams, newMaxyear,
"Special Minimum", SPEC_MIN),
specMinYearsExcess(0), specMinAmount(0.), yearsTotal(0)
{
  initialize();
}

/// <summary>Destructor.</summary>
SpecMin::~SpecMin()
{ }

/// <summary>Initializes computed values to zero.</summary>
void SpecMin::initialize()
{
  PiaMethod::initialize();
  specMinYearsExcess = yearsTotal = 0;
  specMinAmount = 0.0;
}

/// <summary>Determines applicability of method, using stored values.
/// </summary>
///
/// <remarks>To be applicable, the benefit date must be at least
/// <see cref="PiaParams::amend722"/> (January 1973), and it cannot be a
/// totalization benefit.
///
/// This version calls the static version with 1 argument.</remarks>
///
/// <returns>True if method is applicable.</returns>
bool SpecMin::isApplicable()
{
  if (isApplicable(workerData)) {
    setApplicable(APPLICABLE);
    return(true);
  }
  else {
    return(false);
  }
}

/// <summary>Determines applicability of method, using passed values.
/// </summary>
///
/// <remarks>To be applicable, the benefit date must be at least
/// <see cref="PiaParams::amend722"/> (January 1973), and it cannot be a
/// totalization benefit.</remarks>
///
/// <returns>True if method is applicable.</returns>
///
/// <param name="workerData">Worker basic data.</param>
bool SpecMin::isApplicable( const WorkerDataGeneral& workerData )
{
  return (!(workerData.getBenefitDate() < PiaParams::amend722) &&
    !workerData.getTotalize());
}

/// <summary>Computes special minimum PIA.</summary>
/// <remarks>Finds years of coverage by calling
/// <see cref="PiaMethod::specMinYearsTotalCal"/>. Finds the amount per year
/// of coverage by calling <see cref="PiaParams::specMinAmountCal"/>. Limits
/// the usable years of coverage by calling
/// <see cref="specMinYearsExcessCal"/>.</remarks>
void SpecMin::calculate()
{
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    Trace::writeLine(workerData.getIdString() +
      ": Starting SpecMin::calculate");
  }
#endif
  // find years of coverage
  yearsTotal = specMinYearsTotalCal();
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm << workerData.getIdString() << ", yearsTotal = " << yearsTotal
      << ": After specMinYearsTotalCal in SpecMin::calculate";
    Trace::writeLine(strm.str());
  }
#endif
  const DateMoyr& benefitDate = workerData.getBenefitDate();
  setSpecMinAmount(piaParams.specMinAmountCal(benefitDate));
  // limit usable years of coverage
  setSpecMinYearsExcess(specMinYearsExcessCal());
  const int i1 = benefitDate.getYear();
  yearCpi[YEAR_BEN] = yearCpi[YEAR_ENT] =
    (static_cast<int>(benefitDate.getMonth()) >=
    piaParams.getMonthBeninc(i1)) ? i1 : i1 - 1;
  if (!(benefitDate < PiaParams::amend772)) {
    // benefit increases start in 1979
    yearCpi[FIRST_YEAR] = 1978;
    yearCpi[YEAR_ELIG] = YEAR79;
    const int excessYears = getSpecMinYearsExcess();
    piaElig[yearCpi[FIRST_YEAR]] =
      piaParams.getSpecMinPia(PiaParams::amend772, excessYears);
    // find January 1979 MFB
    mfbElig[yearCpi[FIRST_YEAR]] =
      piaParams.getSpecMinMfb(PiaParams::amend772, excessYears);
    // apply benefit increases through year before benefit year
    for (int i2 = yearCpi[1]; i2 < i1; i2++) {
      piaElig[i2] = piaParams.getSpecMinPia(DateMoyr(12, i2), excessYears);
      mfbElig[i2] = piaParams.getSpecMinMfb(DateMoyr(12, i2), excessYears);
    }
    piaElig[i1] = piaParams.getSpecMinPia(benefitDate, excessYears);
    mfbElig[i1] = piaParams.getSpecMinMfb(benefitDate, excessYears);
    // apply benefit increase in benefit year, if applicable
    mfbEnt.set(mfbElig[i1]);
    piaEnt.set(piaElig[i1]);
  }
  else {
    yearCpi[FIRST_YEAR] = 1975;
    // PIA equals dollar amount times years of coverage
    piaElig[yearCpi[FIRST_YEAR]] =
      static_cast<double>(getSpecMinYearsExcess()) * getSpecMinAmount();
    // find old-law MFB from PIA table for pre-1977 special minimum
    piaEnt.set(piaElig[yearCpi[FIRST_YEAR]]);
    setAme(mfbOldCal(false));
  }
  setDirty();
#if defined(DEBUGCASE)
  if (isDebugPid(workerData.getIdNumber())) {
    ostringstream strm;
    strm.setf(ios::fixed, ios::floatfield);
    strm.precision(2);
    strm << workerData.getIdString() << ", pia = " << piaEnt.get()
      << ": Returning from SpecMin::calculate";
    Trace::writeLine(strm.str());
  }
#endif
}

/// <summary>Limits usable years of coverage.</summary>
///
/// <returns>Usable years of coverage (in excess of 10, up to 20).</returns>
int SpecMin::specMinYearsExcessCal() const
{
  return(min(max(yearsTotal - 10, 0), piaParams.specMinMaxYearsPL()));
}
