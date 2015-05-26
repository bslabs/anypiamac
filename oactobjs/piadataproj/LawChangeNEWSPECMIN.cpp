// Functions for the <see cref="LawChangeNEWSPECMIN"/> class to manage
// parameters for a proposal to change the special minimum pia calculation.
//
// $Id: LawChangeNEWSPECMIN.cpp 1.10 2011/08/01 10:06:09EDT 044579 Development  $

//#include "PragmaWarningDisable4786.h"
#include <sstream>
#include <iomanip>
#include "LawChangeNEWSPECMIN.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"
#include "StringParser.h"

using namespace std;

/// <summary>Constructor.</summary>
///
/// <remarks>Initializes all numbers to zero.</remarks>
LawChangeNEWSPECMIN::LawChangeNEWSPECMIN() :
LawChange("new special minimum (years and/or amount)"),
theAmt(0.0), percent(0.0), maxYearsOfCoverage(0)
{ }

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <param name="infile">Open input stream.</param>
void LawChangeNEWSPECMIN::read( std::istream& infile )
{
  string str;  // input string
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    // if 1, 3, 5, or 7, read alternative amount
    if (getInd() % 2 == 1) {
      getline(infile, str);
      if (infile.eof() || infile.fail())
        throw PiaException(PIA_IDS_LAWCHG6);
      strvec = parseString(str);
      if (strvec.size() < 1)
        throw PiaException(PIA_IDS_LAWCHG6);
      setAmount(atof(strvec[0].c_str()));
    }
    // if 2, 3, 6, or 7, read alternative maximum years
    if (getInd() % 4 >= 2) {
      getline(infile, str);
      if (infile.eof() || infile.fail())
        throw PiaException(PIA_IDS_LAWCHG6);
      strvec = parseString(str);
      if (strvec.size() < 1)
        throw PiaException(PIA_IDS_LAWCHG6);
      setYears(atoi(strvec[0].c_str()));
    }
    // if 4, 5, 6, or 7, read alternative percent of wage base
    if (getInd() > 3) {
      getline(infile, str);
      if (infile.eof() || infile.fail())
        throw PiaException(PIA_IDS_LAWCHG6);
      strvec = parseString(str);
      if (strvec.size() < 1)
        throw PiaException(PIA_IDS_LAWCHG6);
      setPercent(atof(strvec[0].c_str()));
    }
  }
}

/// <summary>Writes a line with first and last years, and step size.</summary>
///
/// <param name="out">Open output stream.</param>
void LawChangeNEWSPECMIN::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    out << endl;
    out.precision(2);
    // if 1, 3, 5, or 7, write alternative amount
    if (getInd() % 2 == 1) {
      out << setw(4) << getAmount() << endl;
    }
    // if 2, 3, 6, or 7, write alternative maximum years
    if (getInd() % 4 >= 2) {
      out << " " << getYears() << endl;
    }
    // if 4, 5, 6, or 7, write alternative percent of wage base
    if (getInd() > 3) {
      out << " " << setw(4) << getPercent() << endl;
    }
  }
}

/// <summary>Prepares output strings for this change from present law.</summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeNEWSPECMIN::prepareStrings(
std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    if (getInd() % 2 == 1) {
      outputString.push_back("New dollar amount per year of coverage");
    }
    if (getInd() % 4 > 1) {
      outputString.push_back("New maximum years of coverage");
    }
    if (getInd() % 8 > 3) {
      outputString.push_back("New years of coverage percent");
    }
  }
}
