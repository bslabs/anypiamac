// Functions for <see cref="Psa"/> class to manage PSA accounts.

// $Id: Psa.cpp 1.53 2012/07/30 16:26:52EDT 044579 Development  $

#include <sstream>
#include <fstream>
#include <iomanip>
#include "Psa.h"
#include "LawChangeArray.h"
#include "WageBase.h"
#include "BaseYearNonFile.h"
#include "LawChangePSAACCT.h"
#include "piaparms.h"

using namespace std;

// <summary>Contribution rates when using rates based on age, as opposed
// to using the same set rates for everybody.</summary>
const double Psa::contribPerc[NUM_INVESTMENTS][MAXCONTRIBAGES + 1] = {
  // BONDS
  { 0.45, 0.50, 0.60, 0.80 },
  // STOCKS
  { 0.55, 0.50, 0.40, 0.20 }
};

// <summary>The age splits for contribution rates, when using rates based
// on age.</summary>
const int Psa::contribAge[MAXCONTRIBAGES] = { 40, 50, 60 };

/// <summary>Constructor.</summary>
///
/// <remarks>Creates the annuity factor and contribution rate arrays.</remarks>
///
/// <param name="newLawChange">Law change data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newAlt">Trustees Report alternative (1, 2, or 3).</param>
Psa::Psa( const LawChangeArray& newLawChange, const PiaParams& newPiaParams,
int newAlt ) :
annuityType(ANNUITY_SL), intRate(0.0), percToSpouse(0.0),
unisexInd(UNISEX_SL_AND_JS), lawChange(newLawChange), piaParams(newPiaParams)
{
  TRyear = BaseYearNonFile::TR_YEAR;
  for (int sex = 0; sex < PSA_NUM_SEX; sex++) {
    for (int ageWorker = 0; ageWorker < NUM_ANNUITY_AGES; ageWorker++) {
      annFacSL[sex][ageWorker] = new DoubleAnnual(TRyear, YEAR2100);
      for (int ageSpouse = 0; ageSpouse < NUM_ANNUITY_AGES; ageSpouse++) {
        annFacJS[sex][ageWorker][ageSpouse] =
          new DoubleAnnual(TRyear, YEAR2100);
      }
    }
  }
  for (int i = 0; i < MAXCONTRIBRATES; i++) {
    contribRates[i] = new DoubleAnnual(YEAR37, piaParams.getMaxyear());
    contribRatesBpAmounts[i] =
      new DoubleAnnual(YEAR37, piaParams.getMaxyear());
  }
  setParameters(newAlt);
}

/// <summary>Destructor.</summary>
///
/// <remarks>Deletes the annuity factor and contribution rate arrays.</remarks>
Psa::~Psa()
{
  for (int sex = 0; sex < PSA_NUM_SEX; sex++) {
    for (int ageWorker = 0; ageWorker < NUM_ANNUITY_AGES; ageWorker++) {
      delete annFacSL[sex][ageWorker];
      for (int ageSpouse = 0; ageSpouse < NUM_ANNUITY_AGES; ageSpouse++) {
        delete annFacJS[sex][ageWorker][ageSpouse];
      }
    }
  }
  for (int i = 0; i < MAXCONTRIBRATES; i++) {
    delete contribRates[i];
    delete contribRatesBpAmounts[i];
  }
}

/// <summary>Returns a year converted to a limited year.</summary>
///
/// <returns>A year converted to a limited year.</returns>
///
/// <remarks>The limited year is a minimum of current Trustees Report year and
/// a maximum of <see cref="YEAR2100"/>.</remarks>
///
/// <param name="year">The year for which a limited year is desired.</param>
int Psa::getLimitedYear( int year ) const
{
  const int temp = std::max(year, TRyear);
  return std::min(temp, YEAR2100);
}

/// <summary>Returns an age converted to an age index.</summary>
///
/// <returns>An age converted to an age index.</returns>
///
/// <remarks>The age index is 0 for age 16, 1 for age 17, ..., 59 for ages
/// 75 and above.</remarks>
///
/// <param name="theAge">The age to convert to an age index.</param>
int Psa::getAgeIndex( int theAge )
{
  return std::min(std::max(theAge - FIRST_AGE, 0), NUM_ANNUITY_AGES - 1);
}

/// <summary>Sets psa parameters from the lawchange file.</summary>
///
/// <param name="newAlt">Trustees Report alternative (1, 2, or 3).</param>
void Psa::setParameters( int newAlt )
{
  const LawChangePSAACCT& lawChangePSAACCT = *lawChange.lawChangePSAACCT;
  // Open the straight life annuity factors input file and store the data.
  intRate = lawChangePSAACCT.getAnnIntRate();
  string temp;
  ostringstream strm;
  strm << "annFacSL" << newAlt << "_" << setfill('0') << setw(4)
    << (int)(100.0 * intRate) << ".txt";
  pathname = strm.str();
  ifstream in(pathname.c_str(), ios::in);
  if (!in.is_open())
    throw PiaException(PIA_IDS_GENFILE3);
  for (int sex = 0; sex < PSA_NUM_SEX; sex++) {
    in >> temp;  // ignore <sex> tag
    for (int year = TRyear; year <= YEAR2100; year++) {
      in >> temp;  // ignore <year> tag
      for (int ageWorker = 0; ageWorker < NUM_ANNUITY_AGES; ageWorker++) {
        in >> (*annFacSL[sex][ageWorker])[year];
      }
      in >> temp;  // ignore </year> tag
    }
    in >> temp;  // ignore </sex> tag
  }
  in.close();
  annuityType = lawChangePSAACCT.getAnnuityType();
  // If using joint and 2/3 survivor annuities, load that data also.
  if (annuityType == ANNUITY_JS) {
    strm.str("");
    strm << "annFacJS" << newAlt << "_" << setfill('0') << setw(4)
      << (int)(100.0 * intRate) << ".txt";
    pathname = temp;
    in.open(pathname.c_str(), ios::in);
    if (!in.is_open())
      throw PiaException(PIA_IDS_GENFILE3);
    for (int sex = 0; sex < PSA_NUM_SEX; sex++) {
      in >> temp;  // ignore <sex> tag
      // Since the JS file is so big and slow to be produced, we only
      // have data for every 10th year. Interpolate for in-between years.
      int year;
      for (year = TRyear; year < YEAR2100; year += 10) {
        in >> temp;  // ignore <year> tag
        for (int ageW = 0; ageW < NUM_ANNUITY_AGES; ageW++) {
          for (int ageS = 0; ageS < NUM_ANNUITY_AGES; ageS++) {
            in >> (*annFacJS[sex][ageW][ageS])[year];
          }
        }
        in >> temp;  // ignore </year> tag
        if (year >= TRyear + 10) {
          interpolateFactors(year - 10, year, (PsaSexType)sex);
        }
      }
      // Now read the year 2100. Subtract 10 from the last value of year
      // to set it equal to the last year read in up till now.
      year -= 10;
      in >> temp;  // ignore <2100> tag
      for (int ageW = 0; ageW < NUM_ANNUITY_AGES; ageW++) {
        for (int ageS = 0; ageS < NUM_ANNUITY_AGES; ageS++) {
          in >> (*annFacJS[sex][ageW][ageS])[YEAR2100];
        }
      }
      in >> temp;  // ignore </2100> tag
      interpolateFactors(year, YEAR2100, (PsaSexType)sex);
      in >> temp;  // ignore </sex> tag
    }
    in.close();
  }
  // Store the contribution rates for each year, along with the bend points.
  for (int i = 0; i <= lawChangePSAACCT.getNumContribBps(); i++) {
    int yearNum = 0;
    double currentContribRate = 0.0;
    double currentContribRateBpAmount = 0.0;
    double currentContribRateBpPerc = 0.0;
    int nextCutoffYear = lawChangePSAACCT.getContribYears(yearNum);
    for (int year = contribRates[i]->getBaseYear();
      year <= contribRates[i]->getLastYear(); year++) {
      if (year == nextCutoffYear) {
        currentContribRate = lawChangePSAACCT.getContribRates(yearNum, i);
        currentContribRateBpPerc =
          lawChangePSAACCT.getContribBps(yearNum, i);
        yearNum++;
        nextCutoffYear = lawChangePSAACCT.getContribYears(yearNum);
      }
      (*contribRates[i])[year] = currentContribRate;
      if (i != lawChangePSAACCT.getNumContribBps()) {
        currentContribRateBpAmount = currentContribRateBpPerc *
          piaParams.getBaseOasdi(year);
        (*contribRatesBpAmounts[i + 1])[year] = currentContribRateBpAmount;
      }
    }
  }
  percToSpouse = lawChangePSAACCT.getPercToSpouse();
  unisexInd = lawChangePSAACCT.getUnisexInd();
  for (int i = 0; i < NUM_INVESTMENTS; i++) {
    InvestmentType type = static_cast<InvestmentType>(i);
    adminFee[type] = lawChangePSAACCT.getAdminFee(type);
    stdDev[type] = lawChangePSAACCT.getInvestStdDev(type) / 100.0;
    mean[type] = lawChangePSAACCT.getInvestMean(type) / 100.0;
    if (lawChangePSAACCT.getContribDist() == 1) {
      investPerc[type] = lawChangePSAACCT.getInvestPerc(type);
    }
    else {
      investPerc[type] = 0.0;
    }
    mngmtFee[type] = lawChangePSAACCT.getMngmtFee(type);
  }
}

/// <summary>Interpolates the JS annuity factors since the input file only has
/// data for every 10th year.</summary>
///
/// <remarks>The years in between year1 and year2 are the ones with missing
/// data that needs to be filled in by this function.</remarks>
///
/// <param name="year1">Earlier year with data.</param>
/// <param name="year2">Latter year with data.</param>
/// <param name="sex">The sex of the person with the annuity.</param>
void Psa::interpolateFactors( int year1, int year2, PsaSexType sex )
{
  // Number of sections to break up the range into for interpolating.
  const int numSections = year2 - year1;
  for (int ageWorker = 0; ageWorker < NUM_ANNUITY_AGES; ageWorker++) {
    for (int ageSpouse = 0; ageSpouse < NUM_ANNUITY_AGES; ageSpouse++) {
      const double addVal = ( (*annFacJS[sex][ageWorker][ageSpouse])[year2] -
        (*annFacJS[sex][ageWorker][ageSpouse])[year1] ) / numSections;
      for (int year = year1 + 1; year < year2; year++) {
        (*annFacJS[sex][ageWorker][ageSpouse])[year] =
          (*annFacJS[sex][ageWorker][ageSpouse])[year - 1] + addVal;
      }
    }
  }
}

/// <summary>Calculates contribution to psa for one year, first limiting the
/// earnings to the maximum taxable for the year of contribution.</summary>
///
/// <returns>Contribution to psa for one year.</returns>
///
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="benYear">Year of contribution.</param>
/// <param name="earnings">The earnings on which contribution is made.</param>
double Psa::contribCal( int eligYear, int benYear, double earnings ) const
{
  const LawChangePSAACCT& lawChangePSAACCT = *lawChange.lawChangePSAACCT;
  if (lawChangePSAACCT.isEffective(eligYear, benYear)) {
    // First limit the earnings to the maximum taxable.
    earnings = min(earnings, piaParams.getBaseOasdi(benYear));
    // Then calculate (and return) the contribution amount.
    double contrib = 0.0;
    for (int bpNum = 0; bpNum < lawChangePSAACCT.getNumContribBps();
      bpNum++) {
      const double bpDiff = (*contribRatesBpAmounts[bpNum + 1])[benYear] -
        (*contribRatesBpAmounts[bpNum])[benYear];
      if (earnings <= bpDiff)
        return contrib + earnings * (*contribRates[bpNum])[benYear];
      contrib += bpDiff * (*contribRates[bpNum])[benYear];
      earnings -= bpDiff;
    }
    return contrib + earnings *
      (*contribRates[lawChangePSAACCT.getNumContribBps()])[benYear];
  }
  else {
    return 0.0;
  }
}

/// <summary>Calculate an initial psa (monthly) annuity.</summary>
///
/// <returns>Initial psa (monthly) annuity.</returns>
///
/// <param name="amount">Amount in psa account.</param>
/// <param name="year">Year for which annuity is being calculated.</param>
/// <param name="sex">The sex of the person.</param>
/// <param name="workerAge">The age in years of the person getting the annuity.
/// </param>
/// <param name="spouseAge">The age in years of the person's spouse
/// (0 if not married).</param>
double Psa::annuityCal( double amount, int year, Sex::sex_type sex,
 int workerAge, int spouseAge ) const
{
  const int yearInd = getLimitedYear(year);
  const int workerAgeInd = getAgeIndex(workerAge);
  const int spouseAgeInd = getAgeIndex(spouseAge);
  const bool isSLorUnmarried = (annuityType == ANNUITY_SL || spouseAge == 0);
  const PsaSexType sexInd = getSexType(sex, isSLorUnmarried);
  // Find the correct annuity factor
  const double annFac = (isSLorUnmarried) ?
    (*annFacSL[sexInd][workerAgeInd])[yearInd] :
    (*annFacJS[sexInd][workerAgeInd][spouseAgeInd])[yearInd];
  // Return the amount that person will get each month.
  return amount / annFac;
}

/// <summary>Calculate the ammount that a person would get if he sold back
/// the specified (monthly) annuity.</summary>
///
/// <returns>Amount that the person's annuity is worth.</returns>
///
/// <param name="annuity">Amount of the (monthly) annuity.</param>
/// <param name="year">Year for which annuity is being evaluated.</param>
/// <param name="sex">The sex of the person.</param>
/// <param name="workerAge">The age in years of the person getting the annuity.
/// </param>
/// <param name="spouseAge">The age in years of the person's spouse
/// (0 if not married).</param>
double Psa::annuityRevCal( double annuity, int year, Sex::sex_type sex,
int workerAge, int spouseAge ) const
{
  const int yearInd = getLimitedYear(year);
  const int workerAgeInd = getAgeIndex(workerAge);
  const int spouseAgeInd = getAgeIndex(spouseAge);
  const bool isSLorUnmarried = (annuityType == ANNUITY_SL || spouseAge == 0);
  const PsaSexType sexInd = getSexType(sex, isSLorUnmarried);
  // Find the correct annuity factor
  const double annFac = (isSLorUnmarried) ?
    (*annFacSL[sexInd][workerAgeInd])[yearInd] :
    (*annFacJS[sexInd][workerAgeInd][spouseAgeInd])[yearInd];
  // Return the amount that annuity is worth.
  return annuity * annFac;
}

/// <summary>Returns percentage of contributions that go into the
/// investment.</summary>
///
/// <returns>Percentage of contributions that go into the investment.</returns>
///
/// <param name="type">Type of investment.</param>
/// <param name="age">The age of the contributor in years.</param>
/// <param name="eligYear">Year of eligibility.</param>
/// <param name="benYear">Year of contribution.</param>
double Psa::getInvestPerc( InvestmentType type, int age, int eligYear,
 int benYear ) const
{
  const LawChangePSAACCT& lawChangePSAACCT = *lawChange.lawChangePSAACCT;
  if (lawChangePSAACCT.isEffective(eligYear, benYear)) {
    if (lawChangePSAACCT.getContribDist() == 1)
      // constant percentage for all people for all years
      return investPerc[type];
    else {
      // rate depends on age
      for (int i = 0; i < MAXCONTRIBAGES; i++) {
        if (age < contribAge[i])
          return contribPerc[type][i];
      }
      return contribPerc[type][MAXCONTRIBAGES];
    }
  }
  else {
    return 0.0;
  }
}

/// <summary>Returns the sex code to use for annuities for a specified person.
/// </summary>
///
/// <returns>The sex code to use for annuities for a specified person.
/// </returns>
///
/// <param name="sex">The sex of the person.</param>
/// <param name="isSLorUnmarried">True if an SL annuity or if person is
/// unmarried, false otherwise.</param>
Psa::PsaSexType Psa::getSexType( Sex::sex_type sex, bool isSLorUnmarried ) const
{
  switch (unisexInd) {
  case (UNISEX_SL_AND_JS):
    return PSA_UNISEX;
  case (UNISEX_SL_NOT_JS):
    return isSLorUnmarried ? PSA_UNISEX : (PsaSexType)sex;
  case (UNISEX_JS_NOT_SL):
    return !isSLorUnmarried ? PSA_UNISEX : (PsaSexType)sex;
  case (NO_UNISEX):
    return (PsaSexType)sex;
  default:
    throw PiaException("Psa::getSexType: Invalid argument");
  }
}
