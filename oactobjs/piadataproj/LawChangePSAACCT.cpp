// Functions for the <see cref="LawChangePSAACCT"/> class to manage
// parameters for a proposal to introduce individual accounts.

// $Id: LawChangePSAACCT.cpp 1.11 2011/08/01 10:06:08EDT 044579 Development 277133(2017/09/13 06:57:10EDT) $

#include <sstream>
#include <iomanip>
#include "LawChangePSAACCT.h"
#include "PiaException.h"
#include "Resource.h"
#include "oactcnst.h"
#include "StringParser.h"

using namespace std;

/// <summary>Constructor.</summary>
LawChangePSAACCT::LawChangePSAACCT() :
LawChange("add PSA accounts"),
annuityToDib(false), percToSpouse(0.0), annIntRate(0.0),
annuityOffset(false), annuityType(Psa::ANNUITY_SL), buysAnnuity(false),
contribDist(0), contribStartAge(0), lumpSumAtFra(false), lumpSumPerc(0.0),
numContribBps(0), taxAnnuityInd(0), toSpouseOnDeathInd(0),
rebalanceAccounts(false), unisexInd(Psa::UNISEX_SL_AND_JS)
{
  fill_n(adminFee, (int)Psa::NUM_INVESTMENTS, 0.0);
  fill_n(investMean, (int)Psa::NUM_INVESTMENTS, 0.0);
  fill_n(investStdDev, (int)Psa::NUM_INVESTMENTS, 0.0);
  fill_n(investPerc, (int)Psa::NUM_INVESTMENTS, 0.0);
  fill_n(mngmtFee, (int)Psa::NUM_INVESTMENTS, 0.0);
  fill_n(contribYears, (int)MAXCONTRIBRATEYEARS, 0);
  for (int i = 0; i < MAXCONTRIBRATEYEARS; i++) {
    for (int j = 0; j < Psa::MAXCONTRIBRATES; j++) {
      contribBps[i][j] = 0.0;
      contribRates[i][j] = 0.0;
    }
  }
}

/// <summary>Reads all the lines required for this law-change.</summary>
///
/// <param name="infile">Open input stream.</param>
void LawChangePSAACCT::read( std::istream& infile )
{
  string str;  // input string
  if (getInd() > 0) {
    vector< string > strvec = getFirstLine(infile);
    if (strvec.size() != 18)
      throw PiaException(PIA_IDS_LAWCHG6);
    int i3 = 3;
    setPercToSpouse(atof(strvec[i3++].c_str()));
    setUnisexInd((Psa::UnisexType)atoi(strvec[i3++].c_str()));
    setAnnIntRate(atof(strvec[i3++].c_str()));
    setContribDist(atoi(strvec[i3++].c_str()));
    setTaxAnnuityInd(atoi(strvec[i3++].c_str()));
    setAnnuityToDib(atoi(strvec[i3++].c_str()));
    setToSpouseOnDeathInd(atoi(strvec[i3++].c_str()));
    setAnnuityOffset(atoi(strvec[i3++].c_str()));
    setContribStartAge(atoi(strvec[i3++].c_str()));
    setLumpSumPerc(atof(strvec[i3++].c_str()));
    setNumContribBps(atoi(strvec[i3++].c_str()));
    setLumpSumAtFra(atoi(strvec[i3++].c_str()));
    setBuysAnnuity(atoi(strvec[i3++].c_str()));
    setAnnuityType((Psa::AnnuityType)atoi(strvec[i3++].c_str()));
    setRebalanceAccounts(atoi(strvec[i3++].c_str()));
    getline(infile, str);
    if (infile.eof() || infile.fail())
      throw PiaException(PIA_IDS_LAWCHG6);
    strvec = parseString(str);
    if (static_cast<int>(strvec.size()) != Psa::NUM_INVESTMENTS *
      (4 + (getContribDist() == 1 ? 1 : 0)))
      throw PiaException(PIA_IDS_LAWCHG6);
    i3 = 0;
    for (int i2 = 0; i2 < Psa::NUM_INVESTMENTS; i2++) {
      const Psa::InvestmentType type = static_cast<Psa::InvestmentType>(i2);
      setInvestMean(type, atof(strvec[i3++].c_str()));
      setInvestStdDev(type, atof(strvec[i3++].c_str()));
      setAdminFee(type, atof(strvec[i3++].c_str()));
      setMngmtFee(type, atof(strvec[i3++].c_str()));
      // read proportions of contributions going to the investment
      if (getContribDist() == 1) {
        setInvestPerc(type, atof(strvec[i3++].c_str()));
      }
    }
    // get contribution rates
    for (int i2 = 0; i2 < getInd(); i2++) {
      getline(infile, str);
      if (infile.eof() || infile.fail())
        throw PiaException(PIA_IDS_LAWCHG6);
      strvec = parseString(str);
      if (strvec.size() < static_cast<unsigned>(2 + 2 * getNumContribBps()))
        throw PiaException(PIA_IDS_LAWCHG6);
      setContribYears(i2, atoi(strvec[0].c_str()));
      setContribRates(i2, 0, atof(strvec[1].c_str()));
      for (int i4 = 0; i4 < getNumContribBps(); i4++) {
        setContribBps(i2, i4, atof(strvec[2+2*i4].c_str()));
        setContribRates(i2, 1+i4, atof(strvec[3+2*i4].c_str()));
      }
    }
  }
}

/// <summary>Writes all the lines required for this law-change.</summary>
///
/// <param name="out">Open output stream.</param>
void LawChangePSAACCT::write( std::ostream& out ) const
{
  if (getInd() > 0) {
    writeFirstLine(out);
    out.precision(3);
    out << " " << setw(5) << getPercToSpouse() << " " << getUnisexInd()
      << " " << setw(5) << getAnnIntRate() << " " << getContribDist()
      << " " << getTaxAnnuityInd() << " "
      << (getAnnuityToDib() ? " 1 " : " 0 ") << " "
      << getToSpouseOnDeathInd() << " "
      << (getAnnuityOffset() ? " 1 " : " 0 ") << " " << getContribStartAge()
      << " " << setw(5) << getLumpSumPerc()<< " " << getNumContribBps()
      << " " << (getLumpSumAtFra() ? " 1 " : " 0 ") << " "
      << (getBuysAnnuity() ? " 1 " : " 0 ") << " " << getAnnuityType()
      << " " << (getRebalanceAccounts() ? " 1 " : " 0 ");
    for (int i = 0; i < Psa::NUM_INVESTMENTS; i++) {
      const Psa::InvestmentType type = static_cast<Psa::InvestmentType>(i);
      out << " " << setw(5) << getInvestMean(type) << " " << setw(5)
        << getInvestStdDev(type) << " " << setw(5) << getAdminFee(type)
        << " " << setw(5) << getMngmtFee(type);
      // write proportions of contributions going to each investment
      if (getContribDist() == 1) {
        out << " " << setw(5) << getInvestPerc(type);
      }
    }
    for (int i = 0; i < getInd(); i++) {
      out << " " << getContribYears(i)
        << " " << setw(5) << getContribRates(i, 0);
      for (int j = 0; j < getNumContribBps(); j++) {
        out << " " << setw(5) << getContribBps(i, j+1)
          << " " << setw(5) << getContribRates(i, j+1);
      }
    }
    out << endl;
  }
}

/// <summary>Prepares output strings for this change from present law.
/// </summary>
///
/// <param name="outputString">Table of output strings.</param>
void LawChangePSAACCT::prepareStrings(
  std::vector< std::string >& outputString )
{
  if (getInd() > 0) {
    outputString.push_back(getTitle());
    if (getPhaseType() == 0)
      outputString.push_back("Effective for new eligibles");
    else
      outputString.push_back("Effective immediately for all persons");
    ostringstream strm;
    strm.precision(3);
    strm << "Amount of contribution (as multiplier) going to spouse: "
      << getPercToSpouse();
    outputString.push_back(strm.str());
    switch (getUnisexInd()) {
    case 0:
      outputString.push_back("Using unisex annuity factors for SL and JS.");
      break;
    case 1:
      outputString.push_back(
        "Using unisex annuity factors for SL, and sex-specific for JS.");
      break;
    case 2:
      outputString.push_back(
        "Using sex-specific annuity factors for SL, and unisex for JS.");
      break;
    case 3:
      outputString.push_back(
        "Using sex-specific annuity factors for SL and JS.");
      break;
    default:
      outputString.push_back("");
    }
    strm.str("");
    strm << "Interest rate used for calculating annuity factors: "
      << setprecision(2) << getAnnIntRate() << "%";
    outputString.push_back(strm.str());
    for (int i = 0; i < Psa::NUM_INVESTMENTS; i++) {
      const Psa::InvestmentType type = static_cast<Psa::InvestmentType>(i);
      strm.str("");
      strm.precision(4);
      strm << "Mean and standard deviation of return rate of investment "
        << i << ": " << getInvestMean(type) << " and "
        << getInvestStdDev(type);
      outputString.push_back(strm.str());
      strm.str("");
      strm << "Administrative and management fees for investment " << i
        << ": " << getAdminFee(type) << " and " << getMngmtFee(type);
      outputString.push_back(strm.str());
      if (getContribDist() == 1) {
        strm.str("");
        strm << "Percentage of PSA contribution going to investment " << i
          << ": " << setprecision(3) << getInvestPerc(type);
        outputString.push_back(strm.str());
      }
    }
    if (getContribDist() == 0) {
      outputString.push_back("Split between investments varies by age");
    }
    switch (getTaxAnnuityInd()) {
    case 0:
      outputString.push_back("No tax on PSA annuities");
      break;
    case 1:
      outputString.push_back(
        "Tax PSA annuities like Social Security benefits");
      break;
    default:
      outputString.push_back("Tax PSA annuities like regular income.");
    }
    strm.str("");
    strm << "DIBs " << (getAnnuityToDib() ? "are" : "are not")
      << " eligible for annuities from the PSAs.";
    outputString.push_back(strm.str());
    strm.str("");
    strm << "Upon death of the PSA holder, the account ";
    switch (getToSpouseOnDeathInd()) {
    case 0:
      strm << "does not get transferred to anyone";
      break;
    case 1:
      strm << "transfers to the current spouse, if any";
      break;
    default:
      strm << "transfers to the most recent live spouse (current or divorced)";
    }
    outputString.push_back(strm.str());
    strm.str("");
    strm << "PSA annuity calculations are ";
    switch (getAnnuityType()) {
    case 0:
      strm << "always straight life, for both single and married people.";
      break;
    default:
      strm << "straight life for single people, joint and 2/3 survivor "
        << "for married people.";
    }
    outputString.push_back(strm.str());
    strm.str("");
    strm << "PSA annuities " << (getAnnuityOffset() ? "are" : "are not")
      << " used to offset the regular OASDI benefits.";
    outputString.push_back(strm.str());
    strm.str("");
    strm << "Contributions to the PSA start at age " << getContribStartAge();
    outputString.push_back(strm.str());
    strm.str("");
    strm << "Upon conversion of the PSA to an annuity, the person receives "
      << "a lump sum payment of " << setprecision(3) << getLumpSumPerc()
      << " percent of the PSA.";
    outputString.push_back(strm.str());
    strm.str("");
    strm << "The contribution rate calculation has " << getNumContribBps()
      << " bend point(s) in it.";
    outputString.push_back(strm.str());
    strm.str("");
    strm << "If a person attains FRA and is not eligible for an annuity"
      << " then the PSA " << (getLumpSumAtFra() ? "is" : "is not")
      << " paid out in full as a lump sum payment.";
    outputString.push_back(strm.str());
    strm.str("");
    strm << "PSA is "
      << (getBuysAnnuity() ? "used to buy an" : "diminished by the")
      << " annuity.";
    outputString.push_back(strm.str());
    strm.str("");
    strm << "Investment accounts "
      << (getRebalanceAccounts() ? "are" : "are not")
      << " re-balanced each year";
    outputString.push_back(strm.str());
    outputString.push_back(
      "PSA contribution rates and bend points (by starting year):");
    strm.precision(4);
    for (int i = 0; i < getInd(); i++) {
      strm.str("");
      strm << "  " << getContribYears(i) << " " << getContribRates(i, 0);
      for (int j = 0; j < getNumContribBps(); j++) {
        strm << " " << getContribBps(i, j) << " " << getContribRates(i, j+1);
      }
      outputString.push_back(strm.str());
    }
  }
}
