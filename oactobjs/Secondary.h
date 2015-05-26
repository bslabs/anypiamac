// Declarations for the <see cref="Secondary"/> class containing the
// information about a secondary benefit.
//
// $Id: Secondary.h 1.18 2011/08/09 15:55:30EDT 044579 Development  $

#pragma once

#include <vector>
#include "bic.h"
#include "datemoyr.h"
#include "Pifc.h"

/// <summary>Contains the information about a secondary benefit.</summary>
///
/// <remarks>This is used as a vector element in
/// <see cref="SecondaryArray"/>.</remarks>
///
/// <seealso cref="SecondaryArray"/>
class Secondary
{
public:
  /// <summary>Type of beneficiary code.</summary>
  Bic bic;
  /// <summary>Type of pia calculation method.</summary>
  Pifc pifc;
  /// <summary>The date of entitlement.</summary>
  DateMoyr entDate;
private:
  /// <summary>Full-rate benefit (before actuarial reduction or reduction for
  /// family maximum).</summary>
  double fullBenefit;
  /// <summary>Benefit after reduction for family maximum but before
  /// actuarial reduction.</summary>
  double benefit;
  /// <summary>Benefit after actuarial reduction and reduction for
  /// family maximum.</summary>
  double reducedBenefit;
  /// <summary>Benefit after all reductions and rounding.</summary>
  double roundedBenefit;
  /// <summary>Benefit factor.</summary>
  double benefitFactor;
  /// <summary>Months of actuarial reduction or delayed retirement credit.
  /// </summary>
  int monthsArdri;
  /// <summary>Reduction or increment factor.</summary>
  double arf;
public:
  Secondary();
  void check();
  void deleteContents();
  /// <summary>Returns true if this benefit is affected by family maximum.
  /// </summary>
  ///
  /// <returns>True if this benefit is affected by family maximum.</returns>
  bool eligibleForMax() const { return bic.eligibleForMax(); }
  /// <summary>Returns reduction or increment factor.</summary>
  ///
  /// <returns>Reduction or increment factor.</returns>
  double getArf() const { return arf; }
  /// <summary>Returns benefit after reduction for family maximum but
  /// before actuarial reduction.</summary>
  ///
  /// <returns>Benefit after reduction for family maximum but
  /// before actuarial reduction.</returns>
  double getBenefit() const { return benefit; }
  /// <summary>Returns benefit factor.</summary>
  ///
  /// <returns>Benefit factor.</returns>
  double getBenefitFactor() const { return benefitFactor; }
  /// <summary>Returns full-rate benefit (before actuarial reduction or
  /// reduction for family maximum).</summary>
  ///
  /// <returns>Full-rate benefit (before actuarial reduction or
  /// reduction for family maximum).</returns>
  double getFullBenefit() const { return fullBenefit; }
  /// <summary>Returns months of actuarial reduction or increment.</summary>
  ///
  /// <returns>Months of actuarial reduction or increment.</returns>
  int getMonthsArdri() const { return monthsArdri; }
  /// <summary>Returns benefit after actuarial reduction and reduction
  /// for family maximum.</summary>
  ///
  /// <returns>Benefit after actuarial reduction and reduction
  /// for family maximum.</returns>
  double getReducedBenefit() const { return reducedBenefit; }
  /// <summary>Returns benefit after all reductions and rounding.</summary>
  ///
  /// <returns>Benefit after all reductions and rounding.</returns>
  double getRoundedBenefit() const { return roundedBenefit; }
  /// <summary>Returns true if widow (aged or disabled); false otherwise.
  /// </summary>
  ///
  /// <returns>True if widow (aged or disabled); false otherwise.</returns>
  bool isWidow() const { return bic.isWidow(); }
  /// <summary>Sets reduction or increment factor.</summary>
  ///
  /// <param name="newArf">New reduction or increment factor.</param>
  void setArf( double newArf ) { arf = newArf; }
  /// <summary>Sets benefit after reduction for family maximum but
  /// before actuarial reduction.</summary>
  ///
  /// <param name="newBenefit">New benefit.</param>
  void setBenefit( double newBenefit ) { benefit = newBenefit; }
  /// <summary>Sets benefit factor.</summary>
  ///
  /// <param name="newBenefitFactor">New benefit factor.</param>
  void setBenefitFactor( double newBenefitFactor )
  { benefitFactor = newBenefitFactor; }
  /// <summary>Sets full-rate benefit (before actuarial reduction or
  /// reduction for family maximum).</summary>
  ///
  /// <param name="newFullBenefit">New full benefit.</param>
  void setFullBenefit( double newFullBenefit )
  { fullBenefit = newFullBenefit; }
  /// <summary>Sets months of actuarial reduction or increment.</summary>
  ///
  /// <param name="newMonthsArdri">New months of actuarial reduction or
  /// increment.</param>
  void setMonthsArdri( int newMonthsArdri )
  { monthsArdri = newMonthsArdri; }
  /// <summary>Sets benefit after actuarial reduction and reduction
  /// for family maximum.</summary>
  ///
  /// <param name="newReducedBenefit">New reduced benefit.</param>
  void setReducedBenefit( double newReducedBenefit )
  { reducedBenefit = newReducedBenefit; }
  /// <summary>Sets benefit after all reductions and rounding.</summary>
  ///
  /// <param name="newRoundedBenefit">New rounded benefit.</param>
  void setRoundedBenefit( double newRoundedBenefit )
  { roundedBenefit = newRoundedBenefit; }
};
