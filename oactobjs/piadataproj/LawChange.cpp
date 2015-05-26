// Functions for the <see cref="LawChange"/> class to manage law-change
// parameters.
//
// $Id: LawChange.cpp 1.14 2011/07/29 16:27:32EDT 044579 Development  $

#include <istream>
#include <ostream>
#include "LawChange.h"
#include "PiaException.h"
#include "Resource.h"
#include "StringParser.h"

// Define static class members
int  LawChange::startYearLC;
int  LawChange::endYearLC;

using namespace std;

/// <summary>Constructor with an empty string as title.</summary>
LawChange::LawChange() : endYear(0), ind(0), phaseType(0), startYear(0),
titleLC()
{ }

/// <summary>Constructor with title.</summary>
///
/// <param name="newTitle">Title of this law-change.</param>
LawChange::LawChange( const std::string& newTitle ) : endYear(0), ind(0),
phaseType(0), startYear(0), titleLC(newTitle)
{ }

/// <summary>Destructor.</summary>
LawChange::~LawChange()
{ }

/// <summary>Returns true if this law change is in effect.</summary>
///
/// <returns>True if this law change is in effect.</returns>
///
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="benYear">Year of benefit.</param>
bool LawChange::isEffective( int eligYear, int benYear ) const
{
  const int year = (phaseType == 0) ? eligYear : benYear;
  return (ind > 0 && year >= startYear && year <= endYear);
}

/// <summary>Checks range of a law-change starting year.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_LAWCHG1"/> if starting year is out of range.
/// </exception>
///
/// <param name="start">Starting year to check.</param>
void LawChange::startCheck( int start )
{
  if (start < startYearLC || start > endYearLC)
    throw PiaException(PIA_IDS_LAWCHG1);
}

/// <summary>Checks range of a bendpoint value.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_LAWCHG4"/> if bend point is out of range.</exception>
///
/// <param name="bendPoint">Value to check.</param>
void LawChange::bendPointCheck( double bendPoint )
{
  if (bendPoint < 0.0 || bendPoint > 99999.0)
    throw PiaException(PIA_IDS_LAWCHG4);
}

/// <summary>Checks range of a wage increase proportion.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_LAWCHG2"/> if wage increase proportion is out of range.
/// </exception>
///
/// <param name="proportionBendPoints">Value to check.</param>
void LawChange::proportionBendPointsCheck( double proportionBendPoints )
{
  if (proportionBendPoints < 0.0 || proportionBendPoints > 1.0)
    throw PiaException(PIA_IDS_LAWCHG2);
}

/// <summary>Checks range of a benefit formula percentage.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_LAWCHG3"/> if benefit formula percentage is out of
/// range.</exception>
///
/// <param name="perc">Value to check.</param>
void LawChange::percCheck( double perc )
{
  if (perc < 0.0 || perc > 100.0)
    throw PiaException(PIA_IDS_LAWCHG3);
}

/// <summary>Checks range of a bend point increase percentage.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_LAWCHG3"/> if bend point increase percentage is out of
/// range.</exception>
///
/// <param name="bpinc">Value to check.</param>
void LawChange::bpincCheck( double bpinc )
{
  if (bpinc < 0.0 || bpinc > 100.0)
    throw PiaException(PIA_IDS_LAWCHG3);
}

/// <summary>Checks range of a constant decline percentage.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_LAWCHG5"/> if constant decline percentage is out of
/// range.</exception>
///
/// <param name="constantPerc">Value to check.</param>
void LawChange::constantPercCheck( double constantPerc )
{
  if (constantPerc < 0.0 || constantPerc > 9.99)
    throw PiaException(PIA_IDS_LAWCHG5);
}

/// <summary>Writes a line with first and last years.</summary>
///
/// <param name="out">Open output stream.</param>
void LawChange::writeFirstLine( std::ostream& out ) const
{
  out << getStartYear() << " " << getEndYear() << " " << getPhaseType();
}

/// <summary>Writes a line with first and last years.</summary>
///
/// <param name="out">Open output stream.</param>
void LawChange::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    out << endl;
  }
}

/// <summary>Reads a line and parses first and last years and type of
/// phase-in.</summary>
///
/// <returns>Vector of words contained in the line. The first 3 words are the
/// first and last years and the type of phase-in.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_LAWCHG6"/> if unable to parse input line.</exception>
///
/// <param name="infile">Open input stream.</param>
std::vector< std::string > LawChange::getFirstLine( std::istream& infile )
{
  string str;  // input line
  vector< string > retvec;  // return vector
  getline(infile, str);
  if (infile.eof() || infile.fail())
    throw PiaException(PIA_IDS_LAWCHG6);
  retvec = parseString(str);
  if (retvec.size() < 3)
    throw PiaException(PIA_IDS_LAWCHG6);
  setStartYear(atoi(retvec[0].c_str()));
  setEndYear(atoi(retvec[1].c_str()));
  setPhaseType(atoi(retvec[2].c_str()));
  return retvec;
}

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <param name="infile">Open input stream.</param>
void LawChange::read( std::istream& infile )
{
  if (getInd() > 0) {
    getFirstLine(infile);
  }
}

/// <summary>Prepares output strings for this change from present law.
/// </summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChange::prepareStrings( std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
  }
}
