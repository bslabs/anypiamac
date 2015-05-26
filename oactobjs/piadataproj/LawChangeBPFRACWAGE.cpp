// Functions for the <see cref="LawChangeBPFRACWAGE"/> class to manage
// parameters for a proposal to increase the bend points in the benefit
// formula by a fraction of average wage growth.
//
// $Id: LawChangeBPFRACWAGE.cpp 1.10 2011/08/01 10:06:09EDT 044579 Development  $

#include <sstream>
#include "LawChangeBPFRACWAGE.h"
#include "PiaException.h"
#include "Resource.h"

using namespace std;

/// <summary>Constructor.</summary>
LawChangeBPFRACWAGE::LawChangeBPFRACWAGE() :
LawChange("bend points increasing at fraction of wage rate"), pbp(0.0)
{ }

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <param name="infile">Open input stream.</param>
void LawChangeBPFRACWAGE::read( std::istream& infile )
{
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    if (strvec.size() < 4)
      throw PiaException(PIA_IDS_LAWCHG6);
    setProportionBendPoints(atof(strvec[3].c_str()));
  }
}

/// <summary>Writes a line with first and last years, and percentage.</summary>
///
/// <param name="out">Open output stream.</param>
void LawChangeBPFRACWAGE::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    out.precision(2);
    out << getProportionBendPoints() << endl;
  }
}

/// <summary>Prepares output strings for this change from present law.</summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeBPFRACWAGE::prepareStrings(
std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    ostringstream strm;
    strm.precision(2);
    strm << "Bend points are " << getProportionBendPoints()
      << " wage-indexed from " << getStartYear() << " to " << getEndYear();
    outputString.push_back(strm.str());
  }
}
