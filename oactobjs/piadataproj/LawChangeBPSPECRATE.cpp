// Functions for the <see cref="LawChangeBPSPECRATE"/> class to manage
// parameters for a proposal to increase the bend points in the benefit
// formula by a specified growth rate.
//
// $Id: LawChangeBPSPECRATE.cpp 1.16 2011/07/29 16:27:33EDT 044579 Development  $

#include <sstream>
#include <iomanip>
#include "LawChangeBPSPECRATE.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"

using namespace std;

/// <summary>Constructor.</summary>
LawChangeBPSPECRATE::LawChangeBPSPECRATE() :
LawChange("bend points increasing at specified rates"),
bpinc(YEAR79, getEndYearLC())
{ }

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_LAWCHG6"/> if there are not enough entries on the first
/// line for first year, last year, phase-in type, and a bend point increase
/// factor for each year.</exception>
///
/// <param name="infile">Open input stream.</param>
void LawChangeBPSPECRATE::read( std::istream& infile )
{
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    if ((int)strvec.size() < getEndYear() - getStartYear() + 4)
      throw PiaException(PIA_IDS_LAWCHG6);
    int i3 = 3;
    for (int i1 = getStartYear(); i1 <= getEndYear(); i1++, i3++) {
      double ftemp = atof(strvec[i3].c_str());
      setBendPiaInc(i1, ftemp);
    }
  }
}

/// <summary>Writes a line with first and last years, and percentage.</summary>
///
/// <param name="out">Open output stream.</param>
void LawChangeBPSPECRATE::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    out.precision(2);
    out << " ";
    for (int yr = getStartYear(); yr <= getEndYear(); yr++) {
      out << getBendPiaInc(yr) << " ";
    }
    out << endl;
  }
}

/// <summary>Prepares output strings for this change from present law.</summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeBPSPECRATE::prepareStrings(
std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    ostringstream strm;
    strm << "Bend points are indexed from " << getStartYear() << " to "
      << getEndYear() << " by:";
    outputString.push_back(strm.str());
    strm.str("");
    strm.precision(2);
    for (int yr = getStartYear(); yr <= getEndYear(); yr++) {
      strm << setw(7) << getBendPiaInc(yr);
    }
    outputString.push_back(strm.str());
  }
}
