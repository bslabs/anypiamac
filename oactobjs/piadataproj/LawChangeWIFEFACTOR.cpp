// Functions for the <see cref="LawChangeWIFEFACTOR"/> class to manage
// parameters for a proposal to change the factor for an aged spouse benefit.

// $Id: LawChangeWIFEFACTOR.cpp 1.10 2011/08/01 11:38:37EDT 044579 Development  $

#include <sstream>
#include "LawChangeWIFEFACTOR.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"

using namespace std;

// <summary>The ultimate changed factor for an aged spouse benefit (0.33).
// </summary>
const double LawChangeWIFEFACTOR::newSpouseFactor = 0.33;
// <summary>The annual amount of reduction in the factor for an aged spouse
// benefit (0.01).</summary>
const double LawChangeWIFEFACTOR::amtPerYear = 0.01;

/// <summary>Constructor.</summary>
LawChangeWIFEFACTOR::LawChangeWIFEFACTOR() :
LawChange("reduce aged wife factor to .33")
{ }

/// <summary>Prepares output strings for this change from present law.
/// </summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeWIFEFACTOR::prepareStrings(
std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    ostringstream strm;
    strm << (getInd() == 1 ?
      "Factor changed immediately to 33% from " :
      "Factor changed gradually to 33% from ")
      << getStartYear() << " to " << getEndYear();
    outputString.push_back(strm.str());
  }
}
