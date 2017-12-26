// Functions for the <see cref="LawChangeWAGEBASECHG"/> class to manage
// parameters for a proposal to change the OASDI and/or old-law wage bases.

// $Id: LawChangeWAGEBASECHG.cpp 1.10 2011/08/01 11:38:36EDT 044579 Development 277133(2017/09/13 06:56:59EDT) $

#include <sstream>
#include "LawChangeWAGEBASECHG.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"
#include "StringParser.h"

using namespace std;

/// <summary>Constructor.</summary>
LawChangeWAGEBASECHG::LawChangeWAGEBASECHG() :
LawChange("ad hoc wage base change"), baseOasdi(YEAR37, getEndYearLC()),
base77(YEAR37, getEndYearLC())
{ }

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_LAWCHG6"/> if there are not enough lines of data.
/// </exception>
///
/// <param name="infile">Open input stream.</param>
void LawChangeWAGEBASECHG::read( std::istream& infile )
{
  string str;
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    // read wage bases if 1 or 3
    if (getInd() %2 == 1) {
      getline(infile, str);
      if (infile.eof() || infile.fail())
        throw PiaException(PIA_IDS_LAWCHG6);
      strvec = parseString(str);
      // parse bases
      int i3 = 0;
      for (int i1 = getStartYear(); i1 <= getEndYear(); i1++) {
        double dtemp = atof(strvec[i3++].c_str());
        setBaseOasdi(i1, dtemp);
      }
    }
    // read 1977-law wage bases if 2 or 3
    if (getInd() >= 2) {
      getline(infile, str);
      if (infile.eof() || infile.fail())
        throw PiaException(PIA_IDS_LAWCHG6);
      strvec = parseString(str);
      // parse remaining bases
      int i3 = 0;
      for (int i1 = getStartYear(); i1 <= getEndYear(); i1++) {
        double dtemp = atof(strvec[i3++].c_str());
        setBase77(i1, dtemp);
      }
    }
  }
}

/// <summary>Writes a line with first and last years, and percentage.
/// </summary>
///
/// <param name="out">Open output stream.</param>
void LawChangeWAGEBASECHG::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    out << endl;
    out.precision(2);
    // write oasdi bases
    if (getInd() % 2 == 1) {
      for (int i1 = getStartYear(); i1 <= getEndYear(); i1++) {
        out << " " << getBaseOasdi(i1);
      }
      out << endl;
    }
    // write old-law bases
    if (getInd() >= 2) {
      for (int i1 = getStartYear(); i1 <= getEndYear(); i1++) {
        out << " " << getBase77(i1);
      }
      out << endl;
    }
  }
}

/// <summary>Prepares output strings for this change from present law.
/// </summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeWAGEBASECHG::prepareStrings(
std::vector< std::string >& outputString )
{
  string s;
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    if (getInd() == 1 || getInd() == 3) {
      ostringstream strm;
      strm << "Ad hoc OASDI wage bases from " << getStartYear() << " to "
        << getEndYear();
      outputString.push_back(strm.str());
    }
    if (getInd() == 2 || getInd() == 3) {
      ostringstream strm;
      strm << "Ad hoc old-law wage bases from " << getStartYear() << " to "
        << getEndYear();
      outputString.push_back(strm.str());
    }
  }
}
