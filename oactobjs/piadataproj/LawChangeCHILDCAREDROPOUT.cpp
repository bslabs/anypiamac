// Functions for the <see cref="LawChangeCHILDCAREDROPOUT"/> class to
// manage parameters for a proposal to provide child care dropout years.
//
// $Id: LawChangeCHILDCAREDROPOUT.cpp 1.11 2011/07/29 16:27:31EDT 044579 Development 277133(2017/09/13 06:57:37EDT) $

#include <sstream>
#include "LawChangeCHILDCAREDROPOUT.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"
#include "StringParser.h"

using namespace std;

/// <summary>Constructor.</summary>
LawChangeCHILDCAREDROPOUT::LawChangeCHILDCAREDROPOUT() :
LawChange("childcare dropout years"), fqRatio(0.0), maxAge(0), maxYears(0)
{ }

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <param name="infile">Open input stream.</param>
void LawChangeCHILDCAREDROPOUT::read( std::istream& infile )
{
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    if (strvec.size() < 6)
      throw PiaException(PIA_IDS_LAWCHG6);
    setFqRatio(atof(strvec[3].c_str()));
    setMaxAge(atoi(strvec[4].c_str()));
    setMaxYears(atoi(strvec[5].c_str()));
  }
}

/// <summary>Writes a line with first and last years, and percentage.
/// </summary>
///
/// <param name="out">Open output stream.</param>
void LawChangeCHILDCAREDROPOUT::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    out.precision(4);
    out << " " << getFqRatio() << endl;
    out << " " << getMaxAge() << endl;
    out << " " << getMaxYears() << endl;
    out << endl;
  }
}

/// <summary>Prepares output strings for this change from present law.
/// </summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeCHILDCAREDROPOUT::prepareStrings(
std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    ostringstream strm;
    strm.precision(4);
    strm << "Childcare dropout years from " << getStartYear() << " to "
      << getEndYear() << " for children less than or equal to age "
      << getMaxAge();
    outputString.push_back(strm.str());
    strm.str("");
    strm << "Earnings must be less than " << getFqRatio()
      << " times average wage, for maximum of " << getMaxYears()
      << " dropout years";
    outputString.push_back(strm.str());
  }
}
