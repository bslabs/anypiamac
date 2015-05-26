// Declarations for the <see cref="Pebs"/> class to handle Social Security
// Statement data.

// $Id: pebs.h 1.17 2011/08/01 11:38:37EDT 044579 Development  $

#pragma once

#include <vector>
#include "age.h"

/// <summary>Manages most of the data required to prepare and print out the
/// results of a Statement calculation.</summary>
///
/// <remarks>Manages most of the data required to prepare and print out the
/// results of a Statement calculation.</remarks>
class Pebs
{
public:
  /// <summary>Type of Statement calculation.</summary>
  enum pebes_type {
    PEBS_OAB_DELAYED,  // Delayed retirement.
    PEBS_OAB_FULL,  // Full retirement age.
    PEBS_OAB_EARLY,  // Early retirement.
    PEBS_SURV,  // Survivor benefit.
    PEBS_DISAB,  // Disability benefit.
    MAXPEBS  // Number of Statement calculations.
  };
  /// <summary>Current age.</summary>
  Age ageNow;
  /// <summary><see cref="Age"/> 70 and 0 months.</summary>
  static const Age age70;
  /// <summary><see cref="Age"/> of planned early retirement, after adjusting
  /// for current age and earliest possible age (62).</summary>
  Age agePlan1;
  /// <summary><see cref="Age"/> for worker's full retirement.</summary>
  Age fullRetAge;
  /// <summary><see cref="Age"/> for full retirement calculation (could be
  /// between full retirement and 70).</summary>
  Age oab1;
  /// <summary><see cref="Age"/> for late retirement calculation (could be 70
  /// or later).</summary>
  Age oab2;
private:
  /// <summary>Benefit actually payable in Statement calculation.</summary>
  std::vector< int > benefitPebs;
  /// <summary>Planned retirement age as entered.</summary>
  int agePlan;
  /// <summary>Planned retirement age (adjusted to 62 if none
  /// entered), or current age, if greater.</summary>
  int agePlan2;
  /// <summary>MFB for Statement calculation.</summary>
  std::vector< int > mfbPebs;
  /// <summary>Month of processing (1-12).</summary>
  int monthnow;
  /// <summary>PIA for PEBS calculation.</summary>
  std::vector< int > piaPebs;
  /// <summary>Quarters of coverage earned through current year.</summary>
  int qcTotal;
  /// <summary>Quarters of coverage required for disability
  /// insured status for Statement calculation.</summary>
  int qcDisReq;
  /// <summary>Quarters of coverage earned for disability
  /// insured status for Statement calculation.</summary>
  int qcDisTotal;
  /// <summary>Number of years over which quarters of coverage must be
  /// earned for disability insured status for Statement calculation.</summary>
  int qcDisYears;
  /// <summary>Quarters of coverage required for fully
  /// insured status for Statement calculation.</summary>
  std::vector< int > qcReq;
  /// <summary>Number of oab calculations to do.</summary>
  ///
  /// <remarks>It has the following possible values:
  /// <table>
  ///  1   age 70 or later only.
  ///  2   add full retirement age or later.
  ///  3   add early retirement.
  /// </table></remarks>
  int pebsOab;
  /// <summary>Indicator for dib calculation.</summary>
  ///
  /// <remarks>It has the following possible values:
  /// <table>
  ///  1   do dib calculation.
  ///  0   do not do it.
  /// </table></remarks>
  int pebsDib;
public:
  Pebs();
  void deleteContents();
  /// <summary>Returns planned age of retirement.</summary>
  ///
  /// <returns>Planned age of retirement.</returns>
  int getAgePlan() const { return(agePlan); }
  /// <summary>Returns planned retirement age.</summary>
  ///
  /// <returns>Planned retirement age.</returns>
  int getAgePlan2() const { return(agePlan2); }
  /// <summary>Returns benefit actually payable in Statement calculation.
  /// </summary>
  ///
  /// <returns>Benefit actually payable in Statement calculation.</returns>
  ///
  /// <param name="type">Type of calculation (0-4).</param>
  int getBenefitPebs( pebes_type type ) const { return(benefitPebs[type]); }
  /// <summary>Returns maximum family benefit in Statement calculation.
  /// </summary>
  ///
  /// <returns>Maximum family benefit in Statement calculation.</returns>
  ///
  /// <param name="type">Type of calculation (0-4).</param>
  int getMfbPebs( pebes_type type ) const { return(mfbPebs[type]); }
  /// <summary>Returns month of processing.</summary>
  ///
  /// <returns>Month of processing.</returns>
  int getMonthnow() const { return(monthnow); }
  /// <summary>Returns number of dib computations to do.</summary>
  ///
  /// <returns>Number of dib computations to do.</returns>
  int getPebsDib() const { return(pebsDib); }
  /// <summary>Returns number of oab computations to do.</summary>
  ///
  /// <returns>Number of oab computations to do.</returns>
  int getPebsOab() const { return(pebsOab); }
  /// <summary>Returns PIA in Statement calculation.</summary>
  ///
  /// <returns>PIA in Statement calculation.</returns>
  ///
  /// <param name="type">Type of calculation (0-4).</param>
  int getPiaPebs( pebes_type type ) const { return(piaPebs[type]); }
  /// <summary>Returns quarters of coverage required for disability insured
  /// status for Statement calculation.</summary>
  ///
  /// <returns>Quarters of coverage required for disability insured
  /// status for Statement calculation.</returns>
  int getQcDisReq() const { return(qcDisReq); }
  /// <summary>Returns quarters of coverage earned for disability insured
  /// status for Statement calculation.</summary>
  ///
  /// <returns>Quarters of coverage earned for disability insured
  /// status for Statement calculation.</returns>
  int getQcDisTotal() const { return(qcDisTotal); }
  /// <summary>Returns number of years over which quarters of coverage must
  /// be earned for disability insured status for Statement calculation.
  /// </summary>
  ///
  /// <returns>Number of years over which quarters of coverage must be earned
  /// for disability insured status for Statement calculation.</returns>
  int getQcDisYears() const { return(qcDisYears); }
  /// <summary>Returns required quarters of coverage in Statement calculation.
  /// </summary>
  ///
  /// <returns>Required quarters of coverage in Statement calculation.
  /// </returns>
  ///
  /// <param name="type">Type of calculation (0-4).</param>
  int getQcReq( pebes_type type ) const { return(qcReq[type]); }
  /// <summary>Returns quarters of coverage earned through current year.
  /// </summary>
  ///
  /// <returns>Quarters of coverage earned through current year.</returns>
  int getQcTotal() const { return(qcTotal); }
  void monthnowCheck() const;
  void setAgePlan( int newAgePlan );
  /// <summary>Sets planned retirement age.</summary>
  ///
  /// <param name="newAgePlan2">New planned retirement age.</param>
  void setAgePlan2( int newAgePlan2 ) { agePlan2 = newAgePlan2; }
  /// <summary>Sets benefit actually payable in Statement calculation.
  /// </summary>
  ///
  /// <param name="type">Type of calculation (0-4).</param>
  /// <param name="newBenefit">New benefit.</param>
  void setBenefitPebs( pebes_type type, int newBenefit )
  { benefitPebs[type] = newBenefit; }
  /// <summary>Sets maximum family benefit in Statement calculation.</summary>
  ///
  /// <param name="type">Type of calculation (0-4).</param>
  /// <param name="newMfb">New maximum family benefit.</param>
  void setMfbPebs( pebes_type type, int newMfb ) { mfbPebs[type] = newMfb; }
  void setMonthnow( int newMonthnow );
  void setMonthnow();
  /// <summary>Sets number of dib computations to do.</summary>
  ///
  /// <param name="newPebsDib">New number of dib computations.</param>
  void setPebsDib( int newPebsDib ) { pebsDib = newPebsDib; }
  /// <summary>Sets number of oab computations to do.</summary>
  ///
  /// <param name="newPebsOab">New number of oab computations.</param>
  void setPebsOab( int newPebsOab ) { pebsOab = newPebsOab; }
  /// <summary>Sets PIA in Statement calculation.</summary>
  ///
  /// <param name="type">Type of calculation (0-4).</param>
  /// <param name="newPia">New PIA.</param>
  void setPiaPebs( pebes_type type, int newPia ) { piaPebs[type] = newPia; }
  /// <summary>Sets number of DI quarters of coverage required.</summary>
  ///
  /// <param name="newQcDisReq">New value for number of DI quarters of
  /// coverage required.</param>
  void setQcDisReq( int newQcDisReq ) { qcDisReq = newQcDisReq; }
  /// <summary>Sets number of DI quarters of coverage earned.</summary>
  ///
  /// <param name="newQcDisTotal">New value for number of DI quarters
  /// of coverage earned.</param>
  void setQcDisTotal( int newQcDisTotal ) { qcDisTotal = newQcDisTotal; }
  /// <summary>Sets number of years allowed to earn DI quarters of
  /// coverage.</summary>
  ///
  /// <param name="newQcDisYears">New value for years allowed to earn DI
  /// quarters of coverage.</param>
  void setQcDisYears( int newQcDisYears ) { qcDisYears = newQcDisYears; }
  /// <summary>Sets required quarters of coverage in Statement calculation.
  /// </summary>
  ///
  /// <param name="type">Type of calculation (0-4).</param>
  /// <param name="newQcReq">New required quarters of coverage.</param>
  void setQcReq( pebes_type type, int newQcReq ) { qcReq[type] = newQcReq; }
  /// <summary>Sets number of quarters of coverage earned through current
  /// year.</summary>
  ///
  /// <param name="newQcTotal">New number of quarters of coverage.</param>
  void setQcTotal( int newQcTotal ) { qcTotal = newQcTotal; }
  static void agePlanCheck( int agePlant );
  static void monthnowCheck( int monthnowt );
};
