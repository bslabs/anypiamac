// Functions for the <see cref="LawChangeALLEARN"/> class to manage
// parameters for a proposal to use all years of earnings in an AIME
// calculation.
//
// $Id: LawChangeALLEARN.cpp 1.8 2011/07/29 16:27:32EDT 044579 Development  $

#include <sstream>
#include "LawChangeALLEARN.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"

using namespace std;

/// <summary>Constructor.</summary>
LawChangeALLEARN::LawChangeALLEARN() :
LawChange("use all earnings in AIME computation"), step(0), yearPhasein(0)
{ }

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <param name="infile">Open input stream.</param>
void LawChangeALLEARN::read( std::istream& infile )
{
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    // read phase-in information
    if (getInd() == 2) {
      if (strvec.size() < 5)
        throw PiaException(PIA_IDS_LAWCHG6);
      setYear(atoi(strvec[3].c_str()));
      setStep(atoi(strvec[4].c_str()));
    }
  }
}

/// <summary>Writes a line with first and last years, and step size.</summary>
///
/// <param name="out">Open output stream.</param>
void LawChangeALLEARN::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    // write phase-in information
    if (getInd() == 2) {
      out << " " << getYear() << " " << getStep();
    }
    out << endl;
  }
}

/// <summary>Prepares output strings for this change from present law.
/// </summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeALLEARN::prepareStrings(
  std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    if (getInd() == 1) {
      ostringstream strm;
      strm << "Use all earnings beginning " << getStartYear();
      outputString.push_back(strm.str());
    }
    if (getInd() == 2) {
      ostringstream strm;
      strm << "Phase in use of all earnings by 1 year every " << getStep()
        << " years, beginning " << getYear();
      outputString.push_back(strm.str());
      strm.str("");
      strm << "Use all earnings beginning " << getStartYear();
      outputString.push_back(strm.str());
    }
  }
}
