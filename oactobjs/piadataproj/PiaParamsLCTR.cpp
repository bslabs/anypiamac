// Functions for the <see cref="PiaParamsLCTR"/> class to manage the changed
// law Social Security parameters, already projected based on a Trustees Report
// assumption.
//
// $Id: PiaParamsLCTR.cpp 1.4 2011/08/05 16:30:20EDT 044579 Development  $

#include "PiaParamsLCTR.h"
#include "AssumptionsNonFile.h"

using namespace std;

/// <summary>Constructor.</summary>
///
/// <param name="newMaxyear">Maximum projected year.</param>
/// <param name="altNum">The Trustees Report alternative.</param>
/// <param name="newLawChange">Law change parameters.</param>
PiaParamsLCTR::PiaParamsLCTR( int newMaxyear,
AssumptionType::assum_type altNum, const LawChangeArray& newLawChange ) :
PiaParamsLC(newMaxyear, newLawChange)
{
  AssumptionsNonFile assumptions(newMaxyear);
  setHistFqinc();
  assumptions.benefitIncMenu(altNum);
  updateCpiinc(assumptions.getBiProj().theData,
    assumptions.getCatchupDoc().catchup,
    assumptions.getBenefitInc(altNum), assumptions.getIstart(),
    assumptions.getBiProj().theData.getLastYear());
  assumptions.averageWageMenu(altNum);
  updateFqinc(assumptions.getAwincProj().awinc,
    assumptions.getAverageWage(altNum), assumptions.getIstart() - 1,
    assumptions.getAwincProj().awinc.getLastYear());
  projectFq();
  projectCpiinc();
  updateBases();
  updateYocAmountSpecMin();
}

/// <summary>Destructor.</summary>
PiaParamsLCTR::~PiaParamsLCTR()
{ }
