// Functions for the <see cref="LawChangeMARRLENGTH"/> class to manage
// parameters for a proposal to change the length of marriage requirement for
// divorced spouse benefits.
//
// $Id: LawChangeMARRLENGTH.cpp 1.8 2011/08/01 10:06:08EDT 044579 Development  $

#include <sstream>
#include <iomanip>
#include "LawChangeMARRLENGTH.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"

using namespace std;

/// <summary>Constructor.</summary>
LawChangeMARRLENGTH::LawChangeMARRLENGTH() :
LawChange("change number of years of marriage required"),
marrLengthForDivBen(0)
{ }

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <param name="infile">Open input stream.</param>
void LawChangeMARRLENGTH::read( std::istream& infile )
{
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    if (strvec.size() < 4)
      throw PiaException(PIA_IDS_LAWCHG6);
    setMarrLengthForDivBen(atoi(strvec[3].c_str()));
  }
}

/// <summary>Writes a line with first and last years, and step size.</summary>
///
/// <param name="out">Open output stream.</param>
void LawChangeMARRLENGTH::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    out << " " << setw(3) << getMarrLengthForDivBen() << endl;
  }
}

/// <summary>Prepares output strings for this change from present law.</summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeMARRLENGTH::prepareStrings(
std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    outputString.push_back(
      "Change the number of years of marriage needed for a divorced");
    ostringstream strm;
    strm << "spouse benefit to " << getMarrLengthForDivBen()
      << " starting in " << getStartYear();
    outputString.push_back(strm.str());
  }
}
