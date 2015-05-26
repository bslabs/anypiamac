// Functions for the <see cref="LawChangeNRACHANGE"/> class to manage
// parameters for a proposal to change the full retirement age.

// $Id: LawChangeNRACHANGE.cpp 1.10 2012/09/24 16:36:02EDT 044579 Development  $

#include "LawChangeNRACHANGE.h"

using namespace std;

// <summary>The age at which the actuarial reduction factor changes.</summary>
const Age LawChangeNRACHANGE::ageArChange= Age(69, 0);
// <summary>Monthly actuarial reduction factor between ages 67 and 69.
// </summary>
const double LawChangeNRACHANGE::arMonthlyFactor67_69 = 3.0 / 800.0;
// <summary>Monthly actuarial reduction factor over age 69.</summary>
const double LawChangeNRACHANGE::arMonthlyFactor69Plus = 1.0 / 300.0;

/// <summary>Constructor.</summary>
LawChangeNRACHANGE::LawChangeNRACHANGE() :
LawChange("change full retirement age")
{ }

/// <summary>Prepares output strings for this change from present law.
/// </summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeNRACHANGE::prepareStrings(
std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    switch (getInd()) {
    case 1:
      outputString.push_back("Hold full retirement age at 65");
      break;
    case 2:
      outputString.push_back("Eliminate the hiatus from 66 to 67");
      break;
    default:
      outputString.push_back("Eliminate the hiatus from 66 to 67, "
        "increase thereafter by life expectancy");
    }
  }
}
