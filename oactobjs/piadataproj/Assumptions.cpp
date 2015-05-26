// Functions for the <see cref="Assumptions"/> class - parent of classes
// that manage the average wage and benefit increase assumptions.
//
// $Id: Assumptions.cpp 1.23 2011/07/28 08:55:09EDT 044579 Development  $

#include "Assumptions.h"
#include "UserAssumptions.h"
#include "BaseYearNonFile.h"

using namespace std;

/// <summary>Initializes years, where the year after last known benefit
/// increase is specified.</summary>
///
/// <param name="newIstart">Year after last known benefit increase.</param>
/// <param name="newMaxyear">Maximum projected year</param>
Assumptions::Assumptions( int newIstart, int newMaxyear ) :
istart(newIstart), maxyear(newMaxyear)
{ }

/// <summary>Initializes years, where the year after last known benefit
/// increase is taken from <see cref="BaseYearNonFile"/>.</summary>
//
/// <param name="newMaxyear">Maximum projected year</param>
Assumptions::Assumptions( int newMaxyear ) :
istart(BaseYearNonFile::YEAR), maxyear(newMaxyear)
{ }

/// <summary>Destructor.</summary>
Assumptions::~Assumptions()
{ }

/// <summary>Returns an average wage increase assumption title.</summary>
///
/// <returns>An average wage increase assumption title.</returns>
///
/// <param name="altNum">Number of assumption (1 to
/// <see cref="AssumptionType::assum_type::PEBS_ASSUM"/>).</param>
const std::string& Assumptions::getAverageWage( int altNum )
{
  setAwincProjTitle(altNum);
  return getAwincProj().getTitle();
}

/// <summary>Returns a benefit increase assumption title.</summary>
///
/// <returns>A benefit increase assumption title.</returns>
///
/// <param name="altNum">Number of assumption (1 to
/// <see cref="AssumptionType::assum_type::PEBS_ASSUM"/>).</param>
const std::string& Assumptions::getBenefitInc( int altNum )
{
  setBiProjTitle(altNum);
  return getBiProj().getTitle();
}
