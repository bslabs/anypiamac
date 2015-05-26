// Declarations for the <see cref="LawChangeCHILDCARECREDIT"/> class to manage
// parameters for a proposal to provide child care credits.
//
// $Id: LawChangeCHILDCARECREDIT.h 1.13 2011/07/29 16:27:33EDT 044579 Development  $

#pragma once

#include "LawChange.h"

/// <summary>Parameters for a proposal to provide child care credits.
/// </summary>
///
/// <remarks>There are 2 types of child care credit proposals handled by this
/// class. The first type provides an earnings credit for anybody that has an
/// eligible child in care. To be eligible, the child must be under a
/// specified age (see <see cref="getMaxAge"/>). The amount of the credit is
/// equal to a specified percentage (see <see cref="getFqRatio"/>) of the wage
/// base in the year of the credit. The number of years that the person is
/// eligible for the credit is specifed (see <see cref="getMaxYears"/>), and
/// the creditable years are determined such that the person's PIA will be
/// maximized (i.e., not just taking the first years in which the person is
/// elgible for the credit.) Although the proposal starts in a specified year,
/// the historical earnings (before that start year) used for the person's PIA
/// calculation may or may not have credits applied. This is specified by
/// <see cref="getChildCareYearsBeforeEffYear"/>.
///
/// The second type of child care credit proposal handled by this class also
/// provides earnings credits to those who have an eligible child in care.
/// But these credits are based on the person earnings history up to the
/// point of the potential credit. The person uses the his/her top years of
/// earnings. The number of these years is specified (see
/// <see cref="getNumTopEarnYears"/>). After finding this group of top years
/// of earnings, some extreme years (the years with the highest and lowest of
/// earnings) are dropped out of this group. The number of extreme years is
/// specified (see <see cref="getNumDropExtremeEarnYears"/>). For example,
/// dropping 2 extreme years means to drop the 2 highest and 2 lowest years
/// of earnings. The remaining years of earnings in the selected group are
/// averaged together and a specified percentage of that average (see
/// <see cref="getAvgEarnPct"/>) is used as the credit amount. The other
/// parameters are the same as the first version of the child care credit
/// proposal, except that <see cref="getFqRatio"/> is not used.
///
/// One additional note is that for both versions of the proposal, the credit
/// given is not added on to any earnings that the person already has in that
/// year. It is the total amount of earnings the person is credited in that
/// year. For example, if a person has $10,000 of earnings in the year, and
/// the credit amount is calculated to be $15,000, then the new amount of
/// earnings for that person in that year is $15,000 (i.e., $5,000 on top of
/// his/her regular earnings for that year).</remarks>
class LawChangeCHILDCARECREDIT : public LawChange
{
private:
  /// <summary>Ratio of average earnings credited for a child care year.
  /// </summary>
  double fqRatio;
  /// <summary>Maximum age of child to get credit for a child care year.
  /// </summary>
  int maxAge;
  /// <summary>Maximum number of child care years.</summary>
  int maxYears;
  /// <summary>The percentage to be multiplied by the person's average
  /// earnings to calculate the credit amount.</summary>
  double avgEarnPct;
  /// <summary>Number of highest earnings years to look at for the
  /// calculation of the credit amount.</summary>
  int numTopEarnYears;
  /// <summary>Number of years of highest earnings to drop out of the
  /// calculation of the credit amount.</summary>
  ///
  /// <remarks>Drop the "extreme" years. So if numTopEarnYears is 5, and
  /// numDropExtremeEarnYears is 1, then we'll look at the highest 5 years of
  /// earnings, and drop 1 extreme year at each end (i.e. the highest and
  /// lowest earnings amounts) leaving the 3 middle years.</remarks>
  int numDropExtremeEarnYears;
  /// <summary>Whether or not the person can get credited for years of
  /// child-care before the proposal went into effect.</summary>
  bool childCareYearsBeforeEffYear;
public:
  LawChangeCHILDCARECREDIT();
  /// <summary>Returns the percentage to be multiplied by the person's
  /// average earnings to calculate the credit amount.</summary>
  ///
  /// <returns>The percentage to be multiplied by the person's average
  /// earnings to calculate the credit amount.</returns>
  double getAvgEarnPct() const { return avgEarnPct; }
  /// <summary>Returns true if the person can be credited for child care
  /// years before the first effective year of the proposal, false otherwise.
  /// </summary>
  ///
  /// <returns>True if the person can be credited for child care years before
  /// the first effective year of the proposal, false otherwise.</returns>
  bool getChildCareYearsBeforeEffYear() const
  { return childCareYearsBeforeEffYear; }
  /// <summary>Returns ratio of average earnings credited for a child care
  /// year.</summary>
  ///
  /// <returns>Ratio of average earnings credited for a child care year.
  /// </returns>
  double getFqRatio() const { return fqRatio; }
  /// <summary>Returns maximum age of child to get credited for a child care
  /// year.</summary>
  ///
  /// <returns>Maximum age of child to get credited for a child care year.
  /// </returns>
  int getMaxAge() const { return maxAge; }
  /// <summary>Returns maximum number of child care years.</summary>
  ///
  /// <returns>Maximum number of child care years.</returns>
  int getMaxYears() const { return maxYears; }
  /// <summary>Returns the number of years of highest earnings to drop out
  /// of the calculation of the credit amount.</summary>
  ///
  /// <returns>The number of years of highest earnings to drop out of the
  /// calculation of the credit amount.</returns>
  int getNumDropExtremeEarnYears() const { return numDropExtremeEarnYears; }
  /// <summary>Returns the number of highest earnings years to look at for
  /// the calculation of the credit amount.</summary>
  ///
  /// <returns>The number of highest earnings years to look at for the
  /// calculation of the credit amount.</returns>
  int getNumTopEarnYears() const { return numTopEarnYears; }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets the percentage to be multiplied by the person's average
  /// earnings to calculate the credit amount.</summary>
  ///
  /// <param name="newAvgEarnPct">The percentage to be used.</param>
  void setAvgEarnPct( double newAvgEarnPct ) { avgEarnPct = newAvgEarnPct; }
  /// <summary>Sets if the person can be credited for child care years
  /// before the first effective year of the proposal.</summary>
  ///
  /// <param name="newChildCareYearsBeforeEffYear">1 if the person can be
  /// credited for child care years before the first effective year of the
  /// proposal; 0 otherwise.</param>
  void setChildCareYearsBeforeEffYear( int newChildCareYearsBeforeEffYear )
  { childCareYearsBeforeEffYear = (newChildCareYearsBeforeEffYear == 1); }
  /// <summary>Sets ratio of average earnings credited for a child care
  /// year.</summary>
  ///
  /// <param name="newFqRatio">New ratio of average earnings credited for a
  /// child care year.</param>
  void setFqRatio( double newFqRatio ) { fqRatio = newFqRatio; }
  /// <summary>Sets maximum age of child to get credit for a child care
  /// year.</summary>
  ///
  /// <param name="newMaxAge">New maximum age of child.</param>
  void setMaxAge( int newMaxAge ) { maxAge = newMaxAge; }
  /// <summary>Sets maximum number of child care years.</summary>
  ///
  /// <param name="newMaxYears">New maximum number of child care years.
  /// </param>
  void setMaxYears( int newMaxYears ) { maxYears = newMaxYears; }
  /// <summary>Sets the number of years of highest earnings to drop out of
  /// the calculation of the credit amount.</summary>
  ///
  /// <param name="newNumDropExtremeEarnYears">Number of years.</param>
  void setNumDropExtremeEarnYears( int newNumDropExtremeEarnYears )
  { numDropExtremeEarnYears = newNumDropExtremeEarnYears; }
  /// <summary>Sets the number of highest earnings years to look at for the
  /// calculation of the credit amount.</summary>
  ///
  /// <param name="newNumTopEarnYears">Number of years.</param>
  void setNumTopEarnYears( int newNumTopEarnYears )
  { numTopEarnYears = newNumTopEarnYears; }
  void write( std::ostream& out ) const;
};
