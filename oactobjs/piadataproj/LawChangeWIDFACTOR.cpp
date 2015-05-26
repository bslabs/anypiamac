// Functions for the <see cref="LawChangeWIDFACTOR"/> class to manage
// parameters for a proposal to change the factor for an aged widow(er)
// benefit.

// $Id: LawChangeWIDFACTOR.cpp 1.10 2011/08/01 11:38:36EDT 044579 Development  $

#include <sstream>
#include "LawChangeWIDFACTOR.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"

using namespace std;

// <summary>The ultimate changed factor for an aged widow(er) benefit (1.125).
// </summary>
const double LawChangeWIDFACTOR::newAgedWidFactor = 1.125;
// <summary>The number of years over which the factor for an aged widow(er)
// benefit is phased in (40).</summary>
const int LawChangeWIDFACTOR::numYears = 40;

/// <summary>Constructor.</summary>
LawChangeWIDFACTOR::LawChangeWIDFACTOR() :
LawChange("modify the aged widow(er) benefit calculation")
{ }

/// <summary>Prepares output strings for this change from present law.
/// </summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeWIDFACTOR::prepareStrings(
std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    ostringstream strm;
    switch (getInd()) {
    case 1:
      strm << "Widow(er) factor changed immediately to 112.5% from ";
      break;
    case 2:
      strm << "Widow(er) factor changed gradually to 112.5% from ";
      break;
    case 3:
      strm << "Widow(er)'s RIM-LIM eliminated from ";
      break;
    default:
      strm << "Unknown WIDFACTOR change from ";
    }
    strm << getStartYear() << " to " << getEndYear();
    outputString.push_back(strm.str());
  }
}
