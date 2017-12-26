// Functions for the <see cref="LawChangeDECLINEPERC"/> class to manage
// parameters for a proposal to decrease the benefit formula percentages by
// various rates.
//
// $Id: LawChangeDECLINEPERC.cpp 1.14 2017/09/18 10:37:18EDT 277133 Development  $

#include <sstream>
#include <iomanip>
#include <algorithm>
#include "LawChangeDECLINEPERC.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"
#include "StringParser.h"
#include "PercPia.h"

using namespace std;

/// <summary>Constructor.</summary>
///
/// <remarks>Allocates memory for the benefit formula percentages.</remarks>
LawChangeDECLINEPERC::LawChangeDECLINEPERC() :
LawChange("declining benefit formula percents")
{
  for (int i = 0; i < 4; i++) {
    percpa[i] = new DoubleAnnual(YEAR37, getEndYearLC());
  }
}

/// <summary>Destructor.</summary>
///
/// <remarks>Releases memory.</remarks>
LawChangeDECLINEPERC::~LawChangeDECLINEPERC()
{
  for (int i = 0; i < 4; i++) {
    delete percpa[i];
  }
}

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_LAWCHG6"/> if there are not 6 entries on the first line
/// (for first year, last year, phase-in type, and 3 factors). If the
/// law-change indicator is greater than 1, same type of exception if there are
/// not 4 entries on each succeeding line (for year and 3 factors).</exception>
///
/// <param name="infile">Open input stream.</param>
void LawChangeDECLINEPERC::read( std::istream& infile )
{
  string str;  // input string
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    int yearNum = 0;
    if (strvec.size() < 6)
      throw PiaException(PIA_IDS_LAWCHG6);
    for (int i1 = 0; i1 < 3; i1++) {
      setFactor(yearNum, i1, atof(strvec[i1+3].c_str()));
    }
    setYear(yearNum, getStartYear());
    // read the remaining lines
    if (getInd() > 1) {
      for (yearNum = 1; yearNum < getInd(); yearNum++) {
        getline(infile, str);
        if (infile.eof() || infile.fail())
          throw PiaException(PIA_IDS_LAWCHG6);
        strvec = parseString(str);
        if (strvec.size() < 4)
          throw PiaException(PIA_IDS_LAWCHG6);
        setYear(yearNum, atoi(strvec[0].c_str()));
        for (int i1 = 0; i1 < 3; i1++) {
          setFactor(yearNum, i1, atof(strvec[i1 + 1].c_str()));
        }
      }
    }
    // set all the percentages
    percPiaCal();
  }
}

/// <summary>Writes a line with first and last years, and percentage.</summary>
///
/// <param name="out">Open output stream.</param>
void LawChangeDECLINEPERC::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    int yearNum = 0;
    out.precision(2);
    for (int i1 = 0; i1 < 3; i1++) {
      out << " " << setw(4) << getFactor(yearNum, i1);
    }
    out << endl;
    // write the remaining lines
    if (getInd() > 1) {
      for (yearNum = 1; yearNum < getInd(); yearNum++) {
        out << getYear(yearNum) << " ";
        for (int i1 = 0; i1 < 3; i1++) {
          out << " " << setw(4) << getFactor(yearNum, i1);
        }
        out << endl;
      }
    }
  }
}

/// <summary>Prepares output strings for this change from present law.</summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeDECLINEPERC::prepareStrings(
std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    for (int i2 = 0; i2 < getInd(); i2++) {
      ostringstream strm;
      strm.precision(2);
      strm << "Percentages decline by " << getFactor(i2, 0) << ", "
        << getFactor(i2, 1) << ", and " << getFactor(i2, 2)
        << " starting in " << getYear(i2);
      outputString.push_back(strm.str());
    }
    ostringstream strm;
    strm << "Last year of decline is " << getEndYear();
    outputString.push_back(strm.str());
  }
}

/// <summary>Sets pia formula percentages.</summary>
///
/// <remarks>Initializes the pia formula percentages in the year prior to the
/// effective year with the present-law percentages, then applies the rate of
/// decline for each percentage for each year in the future.</remarks>
void LawChangeDECLINEPERC::percPiaCal()
{
  if (getInd() > 0) {
    for (int i = 0; i < 3; i++) {
      // initialize first percentage
      (*percpa[i])[getYear(0) - 1] = PercPia::PERC[i];
      // handle each interval of declining percentages
      for (int yearNum = 0; yearNum < getInd(); yearNum++) {
        const double factor = 1. - getFactor(yearNum, i) / 100.0;
        int endYr;  // ending year for this interval
        if (yearNum == getInd() - 1)
          endYr = min(percpa[i]->getLastYear(), getEndYear());
        else
          endYr = min(percpa[i]->getLastYear(),
            getYear(yearNum + 1) - 1);
        for (int year = getYear(yearNum); year <= endYr; year++) {
          (*percpa[i])[year] = (*percpa[i])[year - 1] * factor;
        }
      }
      // extend to last possible year
      if (getEndYear() < percpa[i]->getLastYear()) {
        const double ultimatePerc = (*percpa[i])[getEndYear()];
        for (int year = getEndYear() + 1;
          year <= percpa[i]->getLastYear(); year++) {
          (*percpa[i])[year] = ultimatePerc;
        }
      }
    }
  }
}
