// Functions for the <see cref="LawChangeCHILDCARECREDIT"/> class to manage
// parameters for a proposal to provide child care credits.
//
// $Id: LawChangeCHILDCARECREDIT.cpp 1.12 2011/07/29 16:27:32EDT 044579 Development  $

#include <sstream>
#include "LawChangeCHILDCARECREDIT.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"
#include "StringParser.h"

using namespace std;

/// <summary>Constructor.</summary>
LawChangeCHILDCARECREDIT::LawChangeCHILDCARECREDIT() :
LawChange("childcare credit years"), fqRatio(0.0), maxAge(0), maxYears(0),
avgEarnPct(0.0), numTopEarnYears(0), numDropExtremeEarnYears(0),
childCareYearsBeforeEffYear(false)
{ }

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <remarks>There are 2 types of child care credit proposals. The first
/// is based on a set percentage of the current year's wage base. And the
/// second is based on the person's earnings history.</remarks>
///
/// <param name="infile">Open input stream.</param>
void LawChangeCHILDCARECREDIT::read( std::istream& infile )
{
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    if (getInd() <= 2) {
      if (strvec.size() < 7)
        throw PiaException(PIA_IDS_LAWCHG6);
      setFqRatio(atof(strvec[3].c_str()));
      setMaxAge(atoi(strvec[4].c_str()));
      setMaxYears(atoi(strvec[5].c_str()));
      setChildCareYearsBeforeEffYear(atoi(strvec[6].c_str()));
    }
    else {
      if (strvec.size() < 9)
        throw PiaException(PIA_IDS_LAWCHG6);
      setAvgEarnPct(atof(strvec[3].c_str()));
      setMaxAge(atoi(strvec[4].c_str()));
      setMaxYears(atoi(strvec[5].c_str()));
      setNumTopEarnYears(atoi(strvec[6].c_str()));
      setNumDropExtremeEarnYears(atoi(strvec[7].c_str()));
      setChildCareYearsBeforeEffYear(atoi(strvec[8].c_str()));
    }
  }
}

/// <summary>Writes a line with first and last years, and percentage.
/// </summary>
///
/// <param name="out">Open output stream.</param>
void LawChangeCHILDCARECREDIT::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    out << endl;
    out.precision(4);
    if (getInd() <= 2) {
      out << " " << getFqRatio() << endl;
      out << " " << getMaxAge() << endl;
      out << " " << getMaxYears() << endl;
      out << " " << (getChildCareYearsBeforeEffYear() ? "1" : "0") << endl;
    }
    else {
      out << " " << getAvgEarnPct() << endl;
      out << " " << getMaxAge() << endl;
      out << " " << getMaxYears() << endl;
      out << " " << getNumTopEarnYears() << endl;
      out << " " << getNumDropExtremeEarnYears() << endl;
      out << " " << (getChildCareYearsBeforeEffYear() ? "1" : "0") << endl;
    }
  }
}

/// <summary>Prepares output strings for this change from present law.
/// </summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeCHILDCARECREDIT::prepareStrings(
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
    strm << "Childcare credit years from " << getStartYear() << " to "
      << getEndYear();
    outputString.push_back(strm.str());
    strm.str("");
    strm << "for children less than or equal to age " <<getMaxAge();
    outputString.push_back(strm.str());
    strm.str("");
    if (getInd() <= 2) {
      strm << "Credit equals " << getFqRatio()
        << " times average wage, for a maximum of " << getMaxYears()
        << " credit years";
      outputString.push_back(strm.str());
    }
    else {
      strm << "Credit equals " << getAvgEarnPct()
        << " times the person's average earnings of the top "
        << getNumTopEarnYears() << " years excluding "
        << getNumDropExtremeEarnYears()
        << " extreme year(s) of earnings at each end of these top earnings"
        << ", for a maximum of " << getMaxYears() << " credit years";
      outputString.push_back(strm.str());
    }
    strm.str("");
    strm << "Child-care years "
      << (getChildCareYearsBeforeEffYear() ? "can" : "can not")
      << " be credited for years before first effective year";
    outputString.push_back(strm.str());
    strm.str("");
    strm << "Quarters of coverage "
      << ((getInd() == 1 || getInd() == 3) ? "are" : "are not")
      << " credited for child-care years";
    outputString.push_back(strm.str());
  }
}
