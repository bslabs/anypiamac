// Functions for the <see cref="LawChangeNEWFORMULA"/> class to manage
// parameters for a proposal to change the pia formula bend points and/or
// percentages.
//
// $Id: LawChangeNEWFORMULA.cpp 1.15 2017/11/13 08:00:21EST 277133 Development  $

#include <sstream>
#include <iomanip>
#include "LawChangeNEWFORMULA.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"
#include "StringParser.h"

using namespace std;

/// <summary>Constructor.</summary>
///
/// <remarks>Allocates memory for each element of the bend point and
/// percentage arrays.</remarks>
LawChangeNEWFORMULA::LawChangeNEWFORMULA() :
LawChange("new benefit formula")
{
  for (int i = 0; i < 4; i++) {
    bendpAlt[i] = new DoubleAnnual(YEAR37, getEndYearLC());
  }
  for (int i = 0; i < 5; i++) {
    percpAlt[i] = new DoubleAnnual(YEAR37, getEndYearLC());
  }
}

/// <summary>Destructor.</summary>
///
/// <remarks>Deletes allocated memory.</remarks>
LawChangeNEWFORMULA::~LawChangeNEWFORMULA()
{
  for (int i = 0; i < 4; i++) {
    delete bendpAlt[i];
  }
  for (int i = 0; i < 5; i++) {
    delete percpAlt[i];
  }
}

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_LAWCHG6"/> if there are not enough lines of data.
/// </exception>
///
/// <param name="infile">Open input stream.</param>
void LawChangeNEWFORMULA::read( std::istream& infile )
{
  string str;  // input string
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    getline(infile, str);
    if (infile.eof() || infile.fail()) {
      throw PiaException(PIA_IDS_LAWCHG6);
    }
    strvec = parseString(str);
    if (strvec.size() < 1) {
      throw PiaException(PIA_IDS_LAWCHG6);
    }
    setNumBp(atoi(strvec[0].c_str()));
    for (int i2 = getStartYear(); i2 <= getEndYear(); i2++) {
      getline(infile, str);
      if (infile.eof() || infile.fail()) {
        throw PiaException(PIA_IDS_LAWCHG6);
      }
      strvec = parseString(str);
      // set first percentage
      if (strvec.size() < 2 * numBp + 1) {
        throw PiaException(PIA_IDS_LAWCHG6);
      }
      // set percentages
      for (int i1 = 0; i1 < numBp + 1; i1++) {
        double dtemp = atof(strvec[i1].c_str());
        setAltPercPia(i2, i1, dtemp);
      }
      // set bend points
      for (int i1 = 1; i1 < numBp + 1; i1++) {
        double dtemp = atof(strvec[i1 + numBp].c_str());
        setAltBendPia(i2, i1, dtemp);
      }
    }
  }
}

/// <summary>Writes a line with first and last years, and percentage.</summary>
///
/// <param name="out">Open output stream.</param>
void LawChangeNEWFORMULA::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    out << endl;
    out << getNumBp() << endl;
    for (int year = getStartYear(); year <= getEndYear(); year++) {
      out.precision(2);
      for (int i1 = 0; i1 <= getNumBp(); i1++) {
        out << setw(4) << getAltPercPia(year, i1) << " ";
      }
      out.precision(0);
      for (int i1 = 1; i1 <= getNumBp(); i1++) {
        out << setw(7) << getAltBendPia(year, i1) << " ";
      }
      out << endl;
    }
  }
}

/// <summary>Prepares output strings for this change from present law.
/// </summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeNEWFORMULA::prepareStrings(
std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    const int yr = getStartYear();
    ostringstream strm;
    strm.precision(2);
    strm << "PIA formula is " << setprecision(2)
      << (getAltPercPia(yr, 0) * 100.0) << "% of first $"
      << setprecision(0) << getAltBendPia(yr, 1) << " of AIME +";
    outputString.push_back(strm.str());
    for (int i = 1; i <= numBp; i++) {
      if (i == numBp) {
        strm.str("");
        strm << setprecision(2) << (getAltPercPia(yr, i) * 100.0)
          << "% of remainder for those eligible in " << yr;
        outputString.push_back(strm.str());
      } else {
        strm.str("");
        strm << setprecision(2) << (getAltPercPia(yr, i) * 100.0)
          << "% of AIME up to $" << setprecision(0) << getAltBendPia(yr, i + 1)
          << " +";
        outputString.push_back(strm.str());
      }
    }
  }
}
