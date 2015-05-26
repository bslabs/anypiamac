// Declarations for the <see cref="LawChangePSAACCT"/> class to manage
// parameters for a proposal to introduce individual accounts.

// $Id: LawChangePSAACCT.h 1.9 2011/08/01 10:06:10EDT 044579 Development  $

#pragma once

#include "LawChange.h"
#include "Psa.h"

/// <summary>Parameters for a proposal to introduce individual accounts.
/// </summary>
///
/// <remarks>There are many additional data fields compared to the base class.
/// If the indicator is set (greater than 0), then there are individual
/// accounts with contributions of a certain percent of covered wages, where
/// the indicator equals the number of different contribution rates over
/// time.</remarks>
class LawChangePSAACCT : public LawChange
{
public:
  /// <summary>Array sizes.</summary>
  enum DimLimits {
    MAXCONTRIBRATEYEARS = 100 // Maximum number of PSA contribution rates.
  };
private:
  /// <summary>Indicator for whether or not a person starts getting an
  /// annuity from his PSA upon beginning DI benefits.</summary>
  bool annuityToDib;
  /// <summary>Percentage of contribution going to spouse.</summary>
  double percToSpouse;
  /// <summary>Annual PSA administrative fee, stored in dollars.</summary>
  double adminFee[Psa::NUM_INVESTMENTS];
  /// <summary>The interest rate used for the PSA annuities.</summary>
  double annIntRate;
  /// <summary>The indicator on whether or not the PSA annuity will
  /// offset the regular OASDI benefits.</summary>
  bool annuityOffset;
  /// <summary>Type of annuities bought from PSA (0 if always straight
  /// life, 1 if using joint and 2/3 survivor for married people).</summary>
  Psa::AnnuityType annuityType;
  /// <summary>Indicator for whether PSA buys an annuity.</summary>
  ///
  /// <remarks>True indicates that the PSA will be used to buy an annuity,
  /// thus the amount in the PSA is zeroed out after the purchase. False
  /// indicates that the an annuity is paid from the PSA, so the PSA is
  /// diminished each year by the amount of monthly annuity payments over
  /// the course of the year.</remarks>
  bool buysAnnuity;
  /// <summary>Bend points for the PSA contribution rate, as a percent
  /// of the wage base.</summary>
  double contribBps[MAXCONTRIBRATEYEARS][Psa::MAXCONTRIBRATES];
  /// <summary>Indicator defining how the PSA contributions should be
  /// distributed.</summary>
  ///
  /// <remarks>It has the following possible values:
  /// <table>
  ///   0   Use built-in split that varies by age.
  ///   1   Constant (user-specified) split, stored in psaInvestPerc.
  /// </table></remarks>
  int contribDist;
  /// <summary>PSA contribution rates.</summary>
  double contribRates[MAXCONTRIBRATEYEARS][Psa::MAXCONTRIBRATES];
  /// <summary>Starting age for PSA contributions.</summary>
  int contribStartAge;
  /// <summary>Starting years correspodning to the PSA contribution rates.
  /// </summary>
  int contribYears[MAXCONTRIBRATEYEARS];
  /// <summary>Mean to be used for the return rate on each investment.
  /// </summary>
  double investMean[Psa::NUM_INVESTMENTS];
  /// <summary>Proportion of PSA contributions going to each investment.
  /// </summary>
  double investPerc[Psa::NUM_INVESTMENTS];
  /// <summary>Standard deviation to be used for the return rate on each
  /// investment.</summary>
  double investStdDev[Psa::NUM_INVESTMENTS];
  /// <summary>True if person gets entire PSA as a lump sum payment if attains
  /// FRA prior to becoming eligible for an annuity from the PSA, false
  /// otherwise.</summary>
  bool lumpSumAtFra;
  /// <summary>Percent of PSA paid as a lump sum payment upon conversion to
  /// annuity.</summary>
  double lumpSumPerc;
  /// <summary>Annual PSA management fee, stored as a multiplier
  /// (i.e. .01 means that 1% of the fund goes to fees).</summary>
  double mngmtFee[Psa::NUM_INVESTMENTS];
  /// <summary>Number of bend points in the contribution formula for
  /// PSA's. Currently, can be 0 or 1.</summary>
  int numContribBps;
  /// <summary>Indicator for method by which PSA annuities get taxed.
  /// </summary>
  ///
  /// <remarks>It has the following possible values:
  /// <table>
  ///   0   Not taxed at all.
  ///   1   Taxed like Social Security benefits.
  ///   2   Taxed like regular income.
  /// </table></remarks>
  int taxAnnuityInd;
  /// <summary>Indicator regarding the transfer of the PSA to a spouse
  /// upon the death of the person.</summary>
  ///
  /// <remarks>It has the following possible values:
  /// <table>
  ///   0   No transfer.
  ///   1   Transfer to current (at time of death) spouse, if any.
  ///   2   Transfer to most recent spouse (current or divorced).
  /// </table></remarks>
  int toSpouseOnDeathInd;
  /// <summary>True if the PSA investment accounts get re-balanced each
  /// year so as to preserve the same percentage split used for the
  /// contributions, false otherwise.</summary>
  bool rebalanceAccounts;
  /// <summary>Indicator regarding the use of unisex annuity factors.
  /// </summary>
  Psa::UnisexType unisexInd;
public:
  LawChangePSAACCT();
  /// <summary>Returns the administrative fee for the specified investment.
  /// </summary>
  ///
  /// <returns>The administrative fee for the specified investment.</returns>
  ///
  /// <param name="type">Type of investment.</param>
  double getAdminFee( Psa::InvestmentType type ) const
  { return adminFee[type]; }
  /// <summary>Returns the interst rate for the PSA annuities.</summary>
  ///
  /// <returns>The interest rate for the PSA annuities.</returns>
  double getAnnIntRate() const { return annIntRate; }
  /// <summary>Returns true if the PSA annuity offsets the regular OASDI
  /// benefits, false otherwise.</summary>
  ///
  /// <returns>True if the PSA annuity offsets the regular OASDI benefits,
  /// false otherwise.</returns>
  bool getAnnuityOffset() const { return annuityOffset; }
  /// <summary>Returns true if a person converts his PSA into an annuity
  /// upon beginning DI benefits.</summary>
  ///
  /// <returns>True if a person converts his PSA into an annuity upon
  /// beginning DI benefits.</returns>
  bool getAnnuityToDib() const { return annuityToDib; }
  /// <summary>Returns the type of annuity (0 for always straight life, 1
  /// for joint and 2/3 survivor if married).</summary>
  ///
  /// <returns>The annuity type.</returns>
  Psa::AnnuityType getAnnuityType() const { return annuityType; }
  /// <summary>Returns true if the PSA is used to buy an annuity, false if
  /// it is just diminished by the amount of the annuity payments as the
  /// payments are made.</summary>
  ///
  /// <returns>True if the PSA is used to buy an annuity, false if it is
  /// just diminished by the amount of the annuity payments as the payments
  /// are made.</returns>
  bool getBuysAnnuity() const { return buysAnnuity; }
  /// <summary>Returns the percent of the wage base to be used as a bend
  /// point for the contribution rate to the PSA.</summary>
  ///
  /// <returns>The percent of the wage base to be used as a bend point for
  /// the contribution rate to the PSA.</returns>
  ///
  /// <param name="yearNum">Year number.</param>
  /// <param name="bpNum">Bend point number.</param>
  double getContribBps( int yearNum, int bpNum ) const
  { return contribBps[yearNum][bpNum]; }
  /// <summary>Returns the type of distribution to use for PSA contributions.
  /// </summary>
  ///
  /// <returns>The type of distribution to use for PSA contributions.
  /// <table>
  ///   0   Use built-in split that varies by age.
  ///   1   Constant (user-specified) split.
  /// </table></returns>
  int getContribDist() const { return contribDist; }
  /// <summary>Returns specified PSA contribution rate.</summary>
  ///
  /// <returns>Specified PSA contribution rate.</returns>
  ///
  /// <param name="yearNum">Year number.</param>
  /// <param name="rateNum">Rate number.</param>
  double getContribRates( int yearNum, int rateNum ) const
  { return contribRates[yearNum][rateNum]; }
  /// <summary>Returns the starting age for PSA contributions.</summary>
  ///
  /// <returns>The starting age for PSA contributions.</returns>
  int getContribStartAge() const { return contribStartAge; }
  /// <summary>Returns PSA contribution year index.</summary>
  ///
  /// <returns>PSA contribution year index.</returns>
  ///
  /// <param name="yearNum">Year number.</param>
  int getContribYears( int yearNum ) const { return contribYears[yearNum]; }
  /// <summary>Returns mean to be used for the return rate on stocks.
  /// </summary>
  ///
  /// <returns>Mean to be used for the return rate on stocks.</returns>
  ///
  /// <param name="type">Type of investment.</param>
  double getInvestMean( Psa::InvestmentType type ) const
  { return investMean[type]; }
  /// <summary>Returns proportion of PSA contributions going to specified
  /// investment.</summary>
  ///
  /// <returns>Proportion of PSA contributions going to specified investment.
  /// </returns>
  ///
  /// <param name="type">Type of investment.</param>
  double getInvestPerc( Psa::InvestmentType type ) const
  { return investPerc[type]; }
  /// <summary>Returns standard deviation to be used for the return rate on
  /// specified investment.</summary>
  ///
  /// <returns>Standard deviation to be used for the return rate on specified
  /// investment.</returns>
  ///
  /// <param name="type">Type of investment.</param>
  double getInvestStdDev( Psa::InvestmentType type ) const
  { return investStdDev[type]; }
  /// <summary>Returns true if the entire PSA is paid as a lump sum to a
  /// person who attains FRA and is not eligible for an annuity, false
  /// otherwise.</summary>
  ///
  /// <returns>True if the entire PSA is paid as a lump sum to a person who
  /// attains FRA and is not eligible for an annuity, false otherwise.
  /// </returns>
  bool getLumpSumAtFra() const { return lumpSumAtFra; }
  /// <summary>Returns the percent of the PSA that is paid out as a lump sum
  /// upon conversion to annuity.</summary>
  ///
  /// <returns>The percent of the PSA that is paid out as a lump sum upon
  /// conversion to annuity.</returns>
  double getLumpSumPerc() const { return lumpSumPerc; }
  /// <summary>Returns the management fee for the specified investment.
  /// </summary>
  ///
  /// <returns>The management fee for the specified investment.</returns>
  ///
  /// <param name="type">Type of investment.</param>
  double getMngmtFee( Psa::InvestmentType type ) const
  { return mngmtFee[type]; }
  /// <summary>Returns the number of bend point in the contribution rate for
  /// the PSA's.</summary>
  ///
  /// <returns>The number of bend point in the contribution rate for the
  /// PSA's.</returns>
  int getNumContribBps() const { return numContribBps; }
  /// <summary>Returns percentage of contribution going to spouse, if any.
  /// </summary>
  ///
  /// <returns>Percentage of contribution going to spouse, if any.</returns>
  double getPercToSpouse() const { return percToSpouse; }
  /// <summary>Returns true if the PSA investment accounts need to be
  /// re-balanced each year, false otherwise.</summary>
  ///
  /// <returns>True if the PSA investment accounts need to be re-balanced
  /// each year, false otherwise.</returns>
  bool getRebalanceAccounts() const { return rebalanceAccounts; }
  /// <summary>Returns the indicator on how PSA annuities are taxed.</summary>
  ///
  /// <returns>The indicator on how PSA annuities are taxed.
  /// <table>
  ///   0   Not taxed at all.
  ///   1   Taxed like Social Security benefits.
  ///   2   Taxed like regular income.
  /// </table></returns>
  int getTaxAnnuityInd() const { return taxAnnuityInd; }
  /// <summary>Returns the indicator regarding the transfer of the PSA to a
  /// spouse upon death of the person.</summary>
  ///
  /// <returns>The indicator regarding the transfer of the PSA to a spouse
  /// upon death of the person.
  /// <table>
  ///   0   No transfer.
  ///   1   Transfer to current (at time of death) spouse, if any.
  ///   2   Transfer to most recent spouse (current or divorced).
  /// </table></returns>
  int getToSpouseOnDeathInd() const { return toSpouseOnDeathInd; }
  /// <summary>Returns the unisex indicator.</summary>
  ///
  /// <returns>The unisex indicaor.</returns>
  Psa::UnisexType getUnisexInd() const { return unisexInd; }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets the administrative fee for the specified investment.
  /// </summary>
  ///
  /// <param name="type">Type of investment.</param>
  /// <param name="newFee">New administrative fee.</param>
  void setAdminFee( Psa::InvestmentType type, double newFee )
  { adminFee[type] = newFee; }
  /// <summary>Sets the interest rate for the PSA annuities.</summary>
  ///
  /// <param name="newIntRate">The new interest rate to use for the PSA
  /// annuities.</param>
  void setAnnIntRate( double newIntRate ) { annIntRate = newIntRate; }
  /// <summary>Sets the indicator for offsetting OASDI benefits with the PSA
  /// annuity.</summary>
  ///
  /// <param name="newOffset">The new indicator.</param>
  void setAnnuityOffset( int newOffset )
  { annuityOffset = (newOffset > 0) ? true : false; }
  /// <summary>Sets the indicator for whether or not a person starts getting
  /// an annuity from his PSA upon beginning DI benefits.</summary>
  ///
  /// <param name="newAnnuityToDib">New indicator as to whether or not a
  /// person converts his PSA into an annuity upon getting DI benefits.
  /// </param>
  void setAnnuityToDib( int newAnnuityToDib )
  { annuityToDib = (newAnnuityToDib > 0) ? true : false; }
  /// <summary>Sets the type of annuity (0 for always straight life, 1 for
  /// joint and 2/3 survivor if married).</summary>
  ///
  /// <param name="newAnnuityType">New type of annuity.</param>
  void setAnnuityType( Psa::AnnuityType newAnnuityType )
  { annuityType = newAnnuityType; }
  /// <summary>Sets the indicator for how the PSA is treated when the person
  /// starts getting an annuity payment.</summary>
  ///
  /// <param name="newBuysAnnuity">Indicator how to treat the PSA once the
  /// annuity starts.</param>
  void setBuysAnnuity( int newBuysAnnuity )
  { buysAnnuity = (newBuysAnnuity > 0) ? true : false; }
  /// <summary>Sets the percent of the wage base used a s a bend point for
  /// the contribution rate into the PSA's.</summary>
  ///
  /// <param name="yearNum">Year number.</param>
  /// <param name="bpNum">Bend point number.</param>
  /// <param name="newBp">New bend point.</param>
  void setContribBps( int yearNum, int bpNum, double newBp )
  { contribBps[yearNum][bpNum] = newBp; }
  /// <summary>Sets the type of distribution to use for PSA contributions.
  /// </summary>
  ///
  /// <param name="newContribDist">The new type of distribution to use.
  /// </param>
  void setContribDist( int newContribDist ) { contribDist = newContribDist; }
  /// <summary>Sets psa contribution rate.</summary>
  ///
  /// <param name="yearNum">Year number.</param>
  /// <param name="rateNum">Rate number.</param>
  /// <param name="newContribRate">New PSA contribution rate.</param>
  void setContribRates( int yearNum, int rateNum, double newContribRate )
  { contribRates[yearNum][rateNum] = newContribRate; }
  /// <summary>Sets the starting age for PSA contributions.</summary>
  ///
  /// <param name="newStartAge">New starting age for PSA contributions.
  /// </param>
  void setContribStartAge( int newStartAge )
  { contribStartAge = newStartAge; }
  /// <summary>Sets PSA contribution year index.</summary>
  ///
  /// <param name="yearNum">Year number.</param>
  /// <param name="newYear">New PSA contribution year index.</param>
  void setContribYears( int yearNum, int newYear )
  { contribYears[yearNum] = newYear; }
  /// <summary>Sets mean to be used for the return of specified investment.
  /// </summary>
  ///
  /// <param name="theType">Type of investment.</param>
  /// <param name="newMean">New mean.</param>
  void setInvestMean( Psa::InvestmentType theType, double newMean )
  { investMean[theType] = newMean; }
  /// <summary>Sets proportion of psa contributions going to specified
  /// investment.</summary>
  ///
  /// <param name="theType">Type of investment.</param>
  /// <param name="newInvestPerc">New proportion of PSA contributions
  /// going to specified investment.</param>
  void setInvestPerc( Psa::InvestmentType theType, double newInvestPerc )
  { investPerc[theType] = newInvestPerc; }
  /// <summary>Sets standard deviation to be used for the return rate on
  /// specified investment.</summary>
  ///
  /// <param name="theType">Type of investment.</param>
  /// <param name="newStdDev">New standard deviation.</param>
  void setInvestStdDev( Psa::InvestmentType theType, double newStdDev )
  { investStdDev[theType] = newStdDev; }
  /// <summary>Sets the indicator for whether or not a person receives the
  /// entire PSA as a lump sum if they attain FRA and are not eligible for an
  /// annuity.</summary>
  ///
  /// <param name="newLumpSumAtFra">New indicator for receiving the entire
  /// PSA as a lump sum.</param>
  void setLumpSumAtFra( int newLumpSumAtFra )
  { lumpSumAtFra = (newLumpSumAtFra > 0) ? true : false; }
  /// <summary>Sets the new percent of the PSA that is paid out as a lump
  /// sum upon conversion to annuity.</summary>
  ///
  /// <param name="newLumpSumPerc">New percent of the PSA that is paid out
  /// as a lump sum.</param>
  void setLumpSumPerc( double newLumpSumPerc )
  { lumpSumPerc = newLumpSumPerc; }
  /// <summary>Sets the management fee for the specified investment.</summary>
  ///
  /// <param name="type">Type of investment.</param>
  /// <param name="newFee">New management fee.</param>
  void setMngmtFee( Psa::InvestmentType type, double newFee )
  { mngmtFee[type] = newFee; }
  /// <summary>Sets the number of bend point in the contribution rate into
  /// the PSA's.</summary>
  ///
  /// <param name="newNumBps">new number of bend points.</param>
  void setNumContribBps( int newNumBps ) { numContribBps = newNumBps; }
  /// <summary>Sets percentage of contribution going to spouse, if any.
  /// </summary>
  ///
  /// <param name="newPercToSpouse">New percentage going to spouse.</param>
  void setPercToSpouse( double newPercToSpouse )
  { percToSpouse = newPercToSpouse; }
  /// <summary>Sets the rebalanceAccounts indicator.</summary>
  ///
  /// <param name="newRebalanceAccounts">New rebalanceAccounts.</param>
  void setRebalanceAccounts( int newRebalanceAccounts )
  { rebalanceAccounts = (newRebalanceAccounts > 0) ? true : false; }
  /// <summary>Sets the indicator on how PSA annuities are taxed.</summary>
  ///
  /// <param name="newInd">New indicator on how PSA annuities are taxed.
  /// </param>
  void setTaxAnnuityInd( int newInd ) { taxAnnuityInd = newInd; }
  /// <summary>Sets the indicator regarding the transfer of the PSA to a
  /// spouse upon death of the person.</summary>
  ///
  /// <param name="newToSpouseOnDeathInd">New indicator regarding the transfer
  /// of the PSA to a spouse upon the person's death.
  ///
  ///    0 - No transfer.
  ///    1 - Transfer to current (at time of death) spouse, if any.
  ///    2 - Transfer to most recent spouse (current or divorced).</param>
  void setToSpouseOnDeathInd( int newToSpouseOnDeathInd )
  { toSpouseOnDeathInd = newToSpouseOnDeathInd; }
  /// <summary>Sets unisex indicator.</summary>
  ///
  /// <param name="newUnisexInd">New indicator for unisex annuity factors.
  /// </param>
  void setUnisexInd( Psa::UnisexType newUnisexInd )
  { unisexInd = newUnisexInd; }
  void write( std::ostream& out ) const;
};
