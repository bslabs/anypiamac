// Declarations for <see cref="Psa"/> class to manage PSA accounts.

// $Id: Psa.h 1.36 2010/11/23 10:21:01EST 044579 Development  $

#pragma once

#include <string>
#include <algorithm>
#include "dbleann.h"
#include "oactcnst.h"
#include "Sex.h"
class LawChangeArray;
class PiaParams;

/// <summary>Manages the (theoretical) PSA accounts.</summary>
///
/// <remarks>Manages the (theoretical) PSA accounts.</remarks>
class Psa
{
public:
  /// <summary>Types of investments.</summary>
  ///
  /// <remarks>If any more investment types are added, the initialization of
  /// <see cref="Psa::contribPerc"/> in <see cref="Psa.cpp"/> must be updated!
  /// </remarks>
  enum InvestmentType {
    BONDS,  // Bonds.
    STOCKS,  // Stocks.
    NUM_INVESTMENTS  // Number of types of investments.
  };
  /// <summary>Constants for dimensioning.</summary>
  enum DimLimits {
    /// <summary>The maximum number of bend points allowed in the contribution
    ///  rate.</summary>
    MAXCONTRIBRATES = 3,
    /// <summary>The maximum number of bend points allowed in the contribution
    /// rate, when they vary by age.</summary>
    MAXCONTRIBAGES = 3,
    /// <summary>The lowest age considered for annuities.</summary>
    FIRST_AGE = 16,
    /// <summary>The number of ages for which annuities can
    ///  start (16 to 75).</summary>
    NUM_ANNUITY_AGES = 60
  };
  /// <summary>The sex types for PSA annuities.</summary>
  enum PsaSexType {
    PSA_MALE,    // Male.
    PSA_FEMALE,  // Female.
    PSA_UNISEX,  // Unisex.
    PSA_NUM_SEX  // Number of sex types considered.
  };
  /// <summary>Codes for using unisex annuity factors.</summary>
  enum UnisexType {
    /// <summary>Unisex for SL and JS annuities.</summary>
    UNISEX_SL_AND_JS,
    /// <summary>Unisex for SL annuitites, sex-specific for JS annuities.
    /// </summary>
    UNISEX_SL_NOT_JS,
    /// <summary>SL annuitites are sex-specific, unisex for JS annuities.
    /// </summary>
    UNISEX_JS_NOT_SL,
    /// <summary>SL and JS annuitites are sex-specific.</summary>
    NO_UNISEX
  };
  /// <summary>Type of annuity.</summary>
  enum AnnuityType {
    ANNUITY_SL,  // Always straight life.
    ANNUITY_JS   // Using joint and 2/3 survivor for married people.
  };
  /// <summary>The age splits for contribution rates, when using rates
  /// based on age.</summary>
  static const int contribAge[MAXCONTRIBAGES];
  /// <summary>Contribution rates when using rates based on age, as
  /// opposed to using the same set rates for everybody.</summary>
  static const double contribPerc[NUM_INVESTMENTS][MAXCONTRIBAGES + 1];
private:
   /// <summary>File information.</summary>
   std::string pathname;
   /// <summary>Annual administrative fee, stored in dollars.</summary>
   double adminFee[NUM_INVESTMENTS];
   /// <summary>Joint and 2/3 survivor annuity factors.</summary>
   ///
   /// <remarks>Factors are for 3 sexes (male, female, and unisex), for the
   ///  years TRyear through 2100, and 60 worker ages (16-75), and 60
   ///  spouse ages (16-75). Using these factors gives a monthly
   ///  annuity.</remarks>
   DoubleAnnual* annFacJS[PSA_NUM_SEX][NUM_ANNUITY_AGES][NUM_ANNUITY_AGES];
   /// <summary>Straight life annuity factors.</summary>
   ///
   /// <remarks>Factors are for 3 sexes (male, female, and unisex), for the
   ///  years TRyear through 2100, and 60 ages (16-75). Using these factors
   ///  gives a monthly annuity.</remarks>
   DoubleAnnual* annFacSL[PSA_NUM_SEX][NUM_ANNUITY_AGES];
   /// <summary>Type of annuity.</summary>
   AnnuityType annuityType;
   /// <summary>Interest rate for calculating annuities.</summary>
   double intRate;
   /// <summary>Percentage of contribution going to investments (used
   ///  if LawChange::PSAACCT is 1).</summary>
   double investPerc[NUM_INVESTMENTS];
   /// <summary>Mean of the normal distribution for investments.</summary>
   double mean[NUM_INVESTMENTS];
   /// <summary>Annual management fee, stored as a multiplier (i.e. 0.01 means
   /// that 1 percent of the fund is paid as a management fee).</summary>
   double mngmtFee[NUM_INVESTMENTS];
   /// <summary>Percentage of contribution going to spouse, if any.</summary>
   double percToSpouse;
   /// <summary>Standard deviation of the normal distribution for
   /// investments.</summary>
   double stdDev[NUM_INVESTMENTS];
   /// <summary>Current Trustees Report year.</summary>
   int TRyear;
   /// <summary>Indicator for using unisex annuity factors.</summary>
   UnisexType unisexInd;
private:
   /// <summary>Contribution rates by year.</summary>
   DoubleAnnual* contribRates[MAXCONTRIBRATES];
   /// <summary>Contribution rates bend point amounts (in dollars).</summary>
   DoubleAnnual* contribRatesBpAmounts[MAXCONTRIBRATES];
   /// <summary>Law change data.</summary>
   const LawChangeArray& lawChange;
   /// <summary>Benefit calculation parameters.</summary>
   const PiaParams& piaParams;
protected:
   PsaSexType getSexType( Sex::sex_type sex, bool isSLorUnmarried ) const;
   int getLimitedYear( int year ) const;
   void interpolateFactors( int year1, int year2, PsaSexType sex );
   static int getAgeIndex( int theAge );
public:
  Psa( const LawChangeArray& newLawChange, const PiaParams& newPiaParams,
    int newAlt );
   ~Psa();
  double annuityCal( double amount, int year, Sex::sex_type sex,
    int workerAge, int spouseAge ) const;
  double annuityRevCal( double annuity, int year, Sex::sex_type sex,
    int workerAge, int spouseAge ) const;
   double contribCal( int eligYear, int benYear, double earnings ) const;
   /// <summary>Returns the annual administrative fee.</summary>
   ///
   /// <returns>The annual administrative fee.</returns>
   ///
   /// <param name="type">Type of investment.</param>
   double getAdminFee( InvestmentType type ) const { return adminFee[type]; }
   /// <summary>Returns one psa contribution rate for a specified year.
   /// </summary>
   ///
   /// <returns>One psa contribution rate for a specified year.</returns>
   ///
   /// <param name="benYear">The year.</param>
   /// <param name="bpNum">Contribution rate bend point number.</param>
   double getContribRate( int benYear, int bpNum ) const
      { return (*contribRates[bpNum])[benYear]; }
   double getInvestPerc( InvestmentType type, int age, int eligYear,
      int benYear ) const;
   /// <summary>Returns the mean of the normal distribution for bonds.</summary>
   ///
   /// <returns>The mean of the normal distribution for bonds.</returns>
   ///
   /// <param name="type">Type of investment.</param>
   double getMean( InvestmentType type ) const { return mean[type]; }
   /// <summary>Returns the annual management fee.</summary>
   ///
   /// <returns>The annual management fee.</returns>
   ///
   /// <param name="type">Type of investment.</param>
   double getMngmtFee( InvestmentType type ) const { return mngmtFee[type]; }
   /// <summary>Returns percentage of contribution going to spouse,
   ///  if any.</summary>
   ///
   /// <returns>Percentage of contribution going to spouse, if any.</returns>
   double getPercToSpouse() const { return percToSpouse; }
   /// <summary>Returns standard deviation of the normal distribution for
   /// specified type.</summary>
   ///
   /// <returns>Standard deviation of the normal distribution for specified
   /// type.</returns>
   ///
   /// <param name="type">Type of investment.</param>
   double getStdDev( InvestmentType type ) const { return stdDev[type]; }
   void setParameters( int newAlt );
private:
   Psa operator=( Psa newPsa );
};
