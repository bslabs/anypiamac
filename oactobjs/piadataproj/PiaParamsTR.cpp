// Functions for the <see cref="PiaParamsTR"/> class of pia calculation
// parameters using present law only, with projected values set to Trustees
// Report assumptions.

// $Id: PiaParamsTR.cpp 1.6 2011/08/05 16:30:20EDT 044579 Development  $

#include "PiaParamsTR.h"
#include "AssumptionsNonFile.h"

using namespace std;

/// <summary>Constructor.</summary>
///
/// <param name="newMaxyear">Maximum projected year.</param>
/// <param name="newAltNum">The Trustees Report alternative.</param>
PiaParamsTR::PiaParamsTR( int newMaxyear,
AssumptionType::assum_type newAltNum ) :
PiaParamsPL(newMaxyear), altNum(newAltNum)
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
PiaParamsTR::~PiaParamsTR()
{ }
