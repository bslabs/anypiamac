// Functions for the <see cref="LawChangeBPMINCONST"/> class to manage
// parameters for a proposal to increase the bend points in the benefit
// formula by the growth in wages minus a specified rate.
//
// $Id: LawChangeBPMINCONST.cpp 1.10 2011/07/29 16:27:32EDT 044579 Development  $

#include <sstream>
#include "LawChangeBPMINCONST.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"

using namespace std;

/// <summary>Constructor.</summary>
LawChangeBPMINCONST::LawChangeBPMINCONST() :
LawChange("bend points increasing at wage rate minus a constant"), cp(0.0)
{ }

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <param name="infile">Open input stream.</param>
void LawChangeBPMINCONST::read( std::istream& infile )
{
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    if (strvec.size() < 4)
      throw PiaException(PIA_IDS_LAWCHG6);
    setConstantPerc(atof(strvec[3].c_str()));
  }
}

/// <summary>Writes a line with first and last years, and percentage.</summary>
///
/// <param name="out">Open output stream.</param>
void LawChangeBPMINCONST::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    out.precision(2);
    out << " " << getConstantPerc() << endl;
  }
}

/// <summary>Prepares output strings for this change from present law.</summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeBPMINCONST::prepareStrings(
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
    strm << getConstantPerc()
      << " percent subtracted from wage increase factor";
    outputString.push_back(strm.str());
    strm.str("");
    strm << "for increase in bend points from " << getStartYear() << " to "
      << getEndYear();
    outputString.push_back(strm.str());
  }
}
