// Functions for the <see cref="LawChangeTAXRATECHG"/> class to manage
// parameters for a proposal to change the OASDHI tax rates.

// $Id: LawChangeTAXRATECHG.cpp 1.10 2011/08/01 10:06:07EDT 044579 Development  $

#include <sstream>
#include <iomanip>
#include "LawChangeTAXRATECHG.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"
#include "StringParser.h"

using namespace std;

/// <summary>Constructor.</summary>
LawChangeTAXRATECHG::LawChangeTAXRATECHG() :
LawChange("tax rate change"), numTaxrates(0)
{ }

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <param name="infile">Open input stream.</param>
void LawChangeTAXRATECHG::read( std::istream& infile )
{
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    if (strvec.size() < 4)
      throw PiaException(PIA_IDS_LAWCHG6);
    setNumTaxrates(atoi(strvec[3].c_str()));
    for (int i1 = 0; i1 < getNumTaxrates(); i1++) {
      string str;
      getline(infile, str);
      if (infile.eof() || infile.fail())
        throw PiaException(PIA_IDS_LAWCHG6);
      strvec = parseString(str);
      if (strvec.size() < 4)
        throw PiaException(PIA_IDS_LAWCHG6);
      setYearTaxrate(i1, atoi(strvec[0].c_str()));
      // parse employee-employer rates
      setTaxrate(i1, TaxRates::EMPLOYEERATE, TaxRates::OASITF,
        atof(strvec[1].c_str()));
      setTaxrate(i1, TaxRates::EMPLOYEERATE, TaxRates::DITF,
        atof(strvec[2].c_str()));
      setTaxrate(i1, TaxRates::EMPLOYEERATE, TaxRates::OASDITF,
        atof(strvec[3].c_str()));
      if (getInd() == 2) {
        // parse self-employed rates
        if (strvec.size() < 7)
          throw PiaException(PIA_IDS_LAWCHG6);
        setTaxrate(i1, TaxRates::SELFEMPRATE, TaxRates::OASITF,
          atof(strvec[4].c_str()));
        setTaxrate(i1, TaxRates::SELFEMPRATE, TaxRates::DITF,
          atof(strvec[5].c_str()));
        setTaxrate(i1, TaxRates::SELFEMPRATE, TaxRates::OASDITF,
          atof(strvec[6].c_str()));
      }
      else {
        // set self-employed rates equal to twice ee-er rates
        setSelfEmpTaxrate(i1);
      }
    }
  }
}

/// <summary>Writes a line with first and last years, and step size.</summary>
///
/// <param name="out">Open output stream.</param>
void LawChangeTAXRATECHG::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    out << " " << getNumTaxrates() << endl;
    out.precision(4);
    for (int i1 = 0; i1 < getNumTaxrates(); i1++) {
      out << getYearTaxrate(i1);
      // write employee-employer rates
      out << " " << setw(5) << getTaxrate(i1, TaxRates::EMPLOYEERATE,
        TaxRates::OASITF);
      out << " " << setw(5) << getTaxrate(i1, TaxRates::EMPLOYEERATE,
        TaxRates::DITF);
      out << " " << setw(5) << getTaxrate(i1, TaxRates::EMPLOYEERATE,
        TaxRates::OASDITF);
      if (getInd() == 2) {
        // write self-employed rates
        out << " " << setw(5) << getTaxrate(i1, TaxRates::SELFEMPRATE,
          TaxRates::OASITF);
        out << " " << setw(5) << getTaxrate(i1, TaxRates::SELFEMPRATE,
          TaxRates::DITF);
        out << " " << setw(5) << getTaxrate(i1, TaxRates::SELFEMPRATE,
          TaxRates::OASDITF);
      }
      out << endl;
    }
  }
}

/// <summary>Prepares output strings for this change from present law.</summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangeTAXRATECHG::prepareStrings(
std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    outputString.push_back((getInd() == 1) ?
      "OASI, DI, and HI ee-er rates change as follows:" :
      "OASI, DI, and HI rates for ee-er and self-emp change as follows:");
    for (int i2 = 0; i2 < getNumTaxrates(); i2++) {
      ostringstream strm;
      strm.precision(4);
      // write employee-employer rates
      strm << getYearTaxrate(i2)
        << "   " << setw(5)
        << getTaxrate(i2, TaxRates::EMPLOYEERATE, TaxRates::OASITF)
        << "   " << setw(5)
        << getTaxrate(i2, TaxRates::EMPLOYEERATE, TaxRates::DITF)
        << "   " << setw(5)
        << getTaxrate(i2, TaxRates::EMPLOYEERATE, TaxRates::OASDITF);
      if (getInd() == 2) {
        // write self-employed rates
        strm << "   " << setw(5)
          << getTaxrate(i2, TaxRates::SELFEMPRATE, TaxRates::OASITF)
          << "   " << setw(5)
          << getTaxrate(i2, TaxRates::SELFEMPRATE, TaxRates::DITF)
          << "   " << setw(5)
          << getTaxrate(i2, TaxRates::SELFEMPRATE, TaxRates::OASDITF);
      }
      outputString.push_back(strm.str());
    }
  }
}

/// <summary>Returns tax rate.</summary>
///
/// <returns>Tax rate for specified parameters.</returns>
///
/// <param name="yearNum">Number of tax rate interval
///  (0 to numTaxrates - 1).</param>
/// <param name="eeer">Type of tax rate.</param>
/// <param name="trustfund">Type of taxes.
///     0: OASI, 1: DI, 2: HI.</param>
double LawChangeTAXRATECHG::getTaxrate( int yearNum, TaxRates::taxrateType eeer,
TaxRates::trustfundType trustfund ) const
{
  switch (trustfund) {
  case 0:
    return oasiTaxrate[eeer][yearNum];
  case 1:
    return diTaxrate[eeer][yearNum];
  default:
    return hiTaxrate[eeer][yearNum];
  }
}

/// <summary>Sets tax rate.</summary>
///
/// <param name="yearNum">Number of tax rate interval
///  (0 to numTaxrates - 1).</param>
/// <param name="eeer">Type of tax rate.</param>
/// <param name="trustfund">Type of taxes.
///     0: OASI, 1: DI, 2: HI.</param>
/// <param name="newRate">New tax rate.</param>
void LawChangeTAXRATECHG::setTaxrate( int yearNum, TaxRates::taxrateType eeer,
 TaxRates::trustfundType trustfund, double newRate )
{
  switch (trustfund) {
  case 0:
    oasiTaxrate[eeer][yearNum] = newRate;
  case 1:
    diTaxrate[eeer][yearNum] = newRate;
  default:
    hiTaxrate[eeer][yearNum] = newRate;
  }
}

/// <summary>Set self-employed rates equal to twice ee-er rates.</summary>
///
/// <param name="yearNum">Number of tax rate interval
///  (0 to numTaxrates - 1).</param>
void LawChangeTAXRATECHG::setSelfEmpTaxrate( int yearNum )
{
  oasiTaxrate[1][yearNum] = 2. * oasiTaxrate[0][yearNum];
  diTaxrate[1][yearNum] = 2. * diTaxrate[0][yearNum];
  hiTaxrate[1][yearNum] = 2. * hiTaxrate[0][yearNum];
}
