// Functions for the <see cref="LawChangeAGE65COMP"/> class to manage
// parameters for a proposal to increase the number of elapsed years in a pia
// calculation.
//
// $Id: LawChangeAGE65COMP.cpp 1.8 2011/07/29 16:27:32EDT 044579 Development  $

#include <sstream>
#include "LawChangeAGE65COMP.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"

using namespace std;

/// <summary>Constructor.</summary>
LawChangeAGE65COMP::LawChangeAGE65COMP() :
LawChange("increase number of elapsed years"), step(0)
{ }

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <param name="infile">Open input stream.</param>
void LawChangeAGE65COMP::read( std::istream& infile )
{
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    if (strvec.size() < 4)
      throw PiaException(PIA_IDS_LAWCHG6);
    setStep(atoi(strvec[3].c_str()));
  }
}

/// <summary>Writes a line with first and last years, and percentage.
/// </summary>
///
/// <param name="out">Open output stream.</param>
void LawChangeAGE65COMP::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    out << " " << getStep() << endl;
  }
}

/// <summary>Prepares output strings for this change from present law.
/// </summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeAGE65COMP::prepareStrings(
std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    ostringstream strm;
    strm << "Elapsed years increase by " << getInd();
    outputString.push_back(strm.str());
    strm.str("");
    strm << "Increase of 1 elapsed year every " << getStep() << " years";
    outputString.push_back(strm.str());
  }
}
