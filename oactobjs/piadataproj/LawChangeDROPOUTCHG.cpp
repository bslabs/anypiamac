// Functions for the <see cref="LawChangeDROPOUTCHG"/> class to manage
// parameters for a proposal to reduce the number of dropout years in a pia
// calculation.
//
// $Id: LawChangeDROPOUTCHG.cpp 1.8 2011/08/01 10:06:09EDT 044579 Development  $

//#include "PragmaWarningDisable4786.h"
#include <sstream>
#include <iomanip>
#include "LawChangeDROPOUTCHG.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"

using namespace std;

/// <summary>Constructor.</summary>
LawChangeDROPOUTCHG::LawChangeDROPOUTCHG() :
LawChange("reduce number of dropout years"), step(0)
{ }

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <param name="infile">Open input stream.</param>
void LawChangeDROPOUTCHG::read( std::istream& infile )
{
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    if (strvec.size() < 4)
      throw PiaException(PIA_IDS_LAWCHG6);
    setStep(atoi(strvec[3].c_str()));
  }
}

/// <summary>Writes a line with first and last years, and step size.</summary>
///
/// <param name="out">Open output stream.</param>
void LawChangeDROPOUTCHG::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    out << " " << setw(3) << getStep() << endl;
  }
}

/// <summary>Prepares output strings for this change from present law.</summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeDROPOUTCHG::prepareStrings(
std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    ostringstream strm;
    strm << "Dropout years decrease by " << getInd();
    outputString.push_back(strm.str());
    strm.str("");
    strm << "Decrease of 1 dropout year every " << getStep() << " years";
    outputString.push_back(strm.str());
  }
}
