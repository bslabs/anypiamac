// Functions for the <see cref="LawChangeTAXBENCHG"/> class to manage
// parameters for a proposal to change the taxation of benefits.

// $Id: LawChangeTAXBENCHG.cpp 1.8 2011/08/01 10:06:10EDT 044579 Development  $

#include <sstream>
#include "LawChangeTAXBENCHG.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"

using namespace std;

/// <summary>Constructor.</summary>
LawChangeTAXBENCHG::LawChangeTAXBENCHG() :
LawChange("change in tax on benefits provision")
{ }

/// <summary>Prepares output strings for this change from present law.</summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeTAXBENCHG::prepareStrings(
std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    if (getInd() == 1 || getInd() == 3) {
      ostringstream strm;
      strm << "Thresholds phased down to zero over 10 years beginning "
        << getStartYear();
      outputString.push_back(strm.str());
    }
    if (getInd() == 2 || getInd() == 3) {
      ostringstream strm;
      strm << "Tax benefits like private pension plans beginning "
        << getStartYear();
      outputString.push_back(strm.str());
    }
  }
}
