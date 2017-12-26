// Functions for the <see cref="LawChangeCOLACHANGE"/> class to manage
// parameters for a proposal to change the cost-of-living benefit increases.
//
// $Id: LawChangeCOLACHANGE.cpp 1.15 2011/08/01 10:06:10EDT 044579 Development 277133(2017/09/13 06:57:37EDT) $

#include <sstream>
#include "LawChangeCOLACHANGE.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"
#include "StringParser.h"

using namespace std;

/// <summary>Constructor.</summary>
///
/// <remarks>It sets the amount by which to adjust the COLA and the PIA
/// percentile to use when indicator == 2 to zero.</remarks>
LawChangeCOLACHANGE::LawChangeCOLACHANGE() :
LawChange("ad hoc COLA change"), colaAdj(0.0f), piaPercentile(0)
{ }

/// <summary>Reads the one line required for this law-change.</summary>
///
/// <remarks>It reads the first and last years, and either the percentage
/// increase or decrease in COLA (for type 1), or the pia percentile
/// (for type 2).</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_LAWCHG6"/> if there are not enough entries on the first
/// line for first year, last year, phase-in type, and a percentage increase
/// or decrease in COLA (for type 1), or the pia percentile (for type 2).
/// </exception>
///
/// <param name="infile">Open input stream.</param>
void LawChangeCOLACHANGE::read( std::istream& infile )
{
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    if (getInd() == 1) {
      if (strvec.size() < 4)
        throw PiaException(PIA_IDS_LAWCHG6);
      setColaAdj(atof(strvec[3].c_str()));
    }
    if (getInd() == 2) {
      if (strvec.size() < 4)
        throw PiaException(PIA_IDS_LAWCHG6);
      setPiaPercentile(atoi(strvec[3].c_str()));
    }
  }
}

/// <summary>Writes a line with this law-change.</summary>
///
/// <remarks>The line has the first and last years, and either the percentage
/// increase or decrease in COLA (for type 1), or the pia percentile
/// (for type 2).</remarks>
///
/// <param name="out">Open output stream.</param>
void LawChangeCOLACHANGE::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    out.precision(4);
    if (getInd() == 1) {
      out << " " << getColaAdj();
    }
    if (getInd() == 2) {
      out << " " << getPiaPercentile();
    }
    out << endl;
  }
}

/// <summary>Prepares output strings for this change from present law.
/// </summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeCOLACHANGE::prepareStrings(
std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    ostringstream strm;
    if (getInd() == 1) {
      strm.precision(2);
      strm << "Change in COLA of " << getColaAdj();
    }
    if (getInd() == 2) {
      strm << "Capping the COLA based on the " << getPiaPercentile()
        << "th percentile";
    }
    strm << " begins in " << getStartYear();
    outputString.push_back(strm.str());
  }
}
