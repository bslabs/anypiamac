// Declarations for the <see cref="PiaMethod"/> class - parent of the various
// methods required for calculation of a Social Security benefit.

// $Id: PiaMethod.h 1.44 2017/10/12 12:48:33EDT 277133 Development  $

#pragma once

#include "piadata.h"
#include "bendpia.h"
#include "bendmfb.h"
#include "PortionAime.h"
#include "BenefitAmount.h"
class PiaParams;

/// <summary>Parent of the various methods required for calculation of a
/// Social Security benefit.</summary>
///
/// <remarks>Child classes are <see cref="DibGuar"/>, <see cref="FrozMin"/>,
/// <see cref="OldPia"/>, and <see cref="WageIndGeneral"/>, and their
/// children.</remarks>
///
/// <seealso cref="DibGuar"/>
/// <seealso cref="FrozMin"/>
/// <seealso cref="OldPia"/>
/// <seealso cref="WageIndGeneral"/>
class PiaMethod
{
public:
  /// <summary>Type of windfall elimination (WEP).</summary>
  enum WindfallElimType {
    /// <summary>WEP not applicable (no noncovered pension, or eligible
    /// before 1986).</summary>
    NOWINDFALLELIM,
    /// <summary>WEP applicable, but have 30 years of coverage.</summary>
    HAS30YEARS,
    /// <summary>PIA reduced by one-half of noncovered pension.</summary>
    ONEHALFPENSION,
    /// <summary>First benefit formula factor reduced.</summary>
    REDUCEDPERC
  };
  /// <summary>Years of benefit increases applied.</summary>
  enum cpi_year_type {
    /// <summary>First year.</summary>
    FIRST_YEAR,
    /// <summary>First year starting with year of eligibility.</summary>
    YEAR_ELIG,
    /// <summary>Last year ending with year of entitlement.</summary>
    YEAR_ENT,
    /// <summary>Last year ending with year of benefit.</summary>
    YEAR_BEN,
    /// <summary>Last year for table changes.</summary>
    YEAR_TABLE,
    /// <summary>Number of start and end years of CPI increases.</summary>
    MAXIYCPI
  };
  /// <summary>Type of calculation method.</summary>
  enum pia_type {
    OLD_START,  // Old-start method.
    PIA_TABLE,  // Pia table method.
    WAGE_IND,  // Wage-indexed method.
    TRANS_GUAR,  // Transitional guarantee method.
    SPEC_MIN,  // Special minimum method.
    REIND_WID,  // Re-indexed widow(er) method.
    FROZ_MIN,  // Frozen minimum method.
    CHILD_CARE,  // Child care dropout years method.
    DIB_GUAR,  // Disability pia guarantee method.
    WAGE_IND_NON_FREEZE, // Wage-indexed non-freeze method.
    NO_PIA_TYPE  // No method.
  };
  /// <summary>Type of method applicability.</summary>
  enum app_type {
    /// <summary>Method not applicable.</summary>
    NOT_APPLICABLE,
    /// <summary>Method is applicable, but is not the highest or the
    /// support PIA.</summary>
    APPLICABLE,
    /// <summary>Method has highest PIA.</summary>
    HIGH_PIA,
    /// <summary>Method has support PIA.</summary>
    SUPPORT_PIA
  };
  /// <summary>Disability MFB cap indicator.</summary>
  enum dis_mfb_cap_type {
    NO_CAP,   // No disability MFB cap applies.
    PIA_1_5,  // 1.5 * PIA.
    AIME_85,  // 0.85 * AIME.
    PIA_1_0   // 1.0 * PIA.
  };
  /// <summary>Old-law PIA table indicator.</summary>
  enum table_type {
    NO_TABLE = -1,  // PIA table does not apply.
    PL_1952 = 0,    // 1952 Act.
    PL_1954,        // 1954 Act.
    PL_1958,        // 1958 Act.
    PL_1965,        // 1965 Act.
    PL_1967,        // 1967 Act.
    PL_1969,        // 1969 Act.
    PL_1971,        // 1971 Act.
    PL_1972,        // 1972 Act.
    PL_1973,        // 1973 Act.
    PL_1977,        // 1977 Act.
    NUM_TABLES      // Number of PIA tables.
  };
private:
  /// <summary>Average (indexed) monthly earnings.</summary>
  double ame;
  /// <summary>Average (indexed) monthly earnings before application of
  /// totalization provision, if applicable.</summary>
  double ameTotal;
  /// <summary>Percentage cap for MFB (for DI cases).</summary>
  double cap;
  /// <summary>Sum of high n years of earnings.</summary>
  double ftearn;
  /// <summary>PIA method applicability.</summary>
  app_type iapp;
  /// <summary>Disability MFB cap indicator.</summary>
  dis_mfb_cap_type indCap;
  /// <summary>Type of benefit, set even if Statement case.</summary>
  WorkerDataGeneral::ben_type ioasdi;
  /// <summary>Number of applicable PIA table for each method.</summary>
  table_type tableNum;
  /// <summary>Calculation method.</summary>
  pia_type methodType;
  /// <summary>True if old Pebs assumptions (with 1-percent real wage gain).
  /// </summary>
  bool pebsAssumptions;
  /// <summary>PIA at eligibility before application of totalization
  /// provision, if applicable (0 otherwise).</summary>
  BenefitAmount piaEligTotal;
  /// <summary>PIA before application of totalization provision, if
  /// applicable (0 otherwise).</summary>
  BenefitAmount piaTotal;
  /// <summary>PIA at eligibility before application of windfall provision,
  /// if applicable (0 otherwise).</summary>
  BenefitAmount piaWindfall;
  /// <summary>Title of this method.</summary>
  const std::string methodTitle;
  /// <summary>Indicator for values having been set (via setAt).</summary>
  bool dirty;
protected:
  /// <summary>Temporary pia.</summary>
  double piasub;
  /// <summary>Temporary mfb.</summary>
  double mfbsub;
  /// <summary>Pointer to worker data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pointer to pia calculation data.</summary>
  const PiaData& piaData;
  /// <summary>Pointer to pia calculation parameters.</summary>
  const PiaParams& piaParams;
  /// <summary>Years of benefit increases applied.</summary>
  int yearCpi[MAXIYCPI];
public:
  /// <summary>Indicators for use of earnings.</summary>
  ///
  /// <remarks>The value for each year has the following meaning:
  /// <table>
  ///   1   earnings are among highest n.
  ///   0   otherwise.
  /// </table></remarks>
  IntAnnual iorder;
  /// <summary>MFB at eligibility and succeeding MFB's.</summary>
  DoubleAnnual mfbElig;
  /// <summary>MFB at entitlement.</summary>
  BenefitAmount mfbEnt;
  /// <summary>PIA at eligibility and succeeding PIA's.</summary>
  DoubleAnnual piaElig;
  /// <summary>PIA at entitlement.</summary>
  BenefitAmount piaEnt;
  /// <summary>The worker's earnings multiplied by average wages of base year.
  /// </summary>
  DoubleAnnual earnMultiplied;
  /// <summary>The worker's earnings after indexing.</summary>
  DoubleAnnual earnIndexed;
public:
  PiaMethod( const WorkerDataGeneral& newWorkerData,
    const PiaData& newPiaData, const PiaParams& newPiaParams,
    int newMaxyear, const std::string& newTitle, pia_type newMethod );
  virtual ~PiaMethod();
  /// <summary>Applies benefit increases to 1977 Amendments PIA or MFB.
  /// </summary>
  ///
  /// <returns>PIA or MFB increased by benefit increases.</returns>
  ///
  /// <remarks>Calls applyColas, with a fourth argument equal to the second
  /// argument.</remarks>
  ///
  /// <param name="pia77">PIA or MFB to be increased.</param>
  /// <param name="year1">First year of benefit increases.</param>
  /// <param name="dateMoyr2">The date up to which to apply benefit
  /// increases.</param>
  double applyColas( DoubleAnnual& pia77, int year1,
    const DateMoyr& dateMoyr2 ) const
  { return applyColasElig(pia77, year1, dateMoyr2, year1); }
  double applyColasElig( DoubleAnnual& pia77, int year1,
    const DateMoyr& dateMoyr2, int catchupYear ) const;
  /// <summary>Calculates pia and mfb for this method.</summary>
  virtual void calculate() = 0;
  double diMax( double amedimax, double piamax );
  double diMaxNonFreeze( double amedimax, double piamax );
  /// <summary>Returns ame or aime.</summary>
  ///
  /// <returns>Ame or aime.</returns>
  double getAme() const { return(ame); }
  /// <summary>Returns average (indexed) monthly earnings before
  /// application of totalization provision, if applicable.</summary>
  ///
  /// <returns>Average (indexed) monthly earnings before application
  /// of totalization provision, if applicable.</returns>
  double getAmeTotal() const { return(ameTotal); }
  /// <summary>Returns indicator for use of pia calculation method.</summary>
  ///
  /// <returns>Indicator for use of pia calculation method.</returns>
  app_type getApplicable() const { return(iapp); }
  /// <summary>Returns percentage cap for MFB (for DI cases).</summary>
  ///
  /// <returns>Percentage cap for MFB (for DI cases).</returns>
  double getCap() const { return(cap); }
  /// <summary>Returns year that benefit increases were applied.</summary>
  ///
  /// <returns>Year that benefit increases were applied.</returns>
  ///
  /// <param name="index">The type of year that benefit increases were
  /// applied for.</param>
  int getCpiYear( cpi_year_type index ) const { return yearCpi[index]; }
  /// <summary>Returns sum of high n years of earnings.</summary>
  ///
  /// <returns>Sum of high n years of earnings.</returns>
  double getftearn() const { return(ftearn); }
  /// <summary>Returns disability MFB cap indicator.</summary>
  ///
  /// <returns>Disability MFB cap indicator.</returns>
  dis_mfb_cap_type getIndCap() const { return(indCap); }
  /// <summary>Returns type of benefit.</summary>
  ///
  /// <returns>Type of benefit.</returns>
  WorkerDataGeneral::ben_type getIoasdi() const { return(ioasdi); }
  /// <summary>Returns the calculation method.</summary>
  ///
  /// <returns>The calculation method.</returns>
  pia_type getMethod() const { return(methodType); }
  /// <summary>Returns PIA at eligibility before application of
  /// totalization provision, if applicable (0 otherwise).</summary>
  ///
  /// <returns>PIA at eligibility before application of totalization
  /// provision, if applicable (0 otherwise).</returns>
  BenefitAmount getPiaEligTotal() const { return(piaEligTotal); }
  /// <summary>Returns PIA before application of totalization provision,
  /// if applicable (0 otherwise).</summary>
  ///
  /// <returns>PIA before application of totalization provision,
  /// if applicable (0 otherwise).</returns>
  BenefitAmount getPiaTotal() const { return(piaTotal); }
  /// <summary>Returns PIA at eligibility before application of windfall
  /// provision, if applicable (0 otherwise).</summary>
  ///
  /// <returns>PIA at eligibility before application of windfall
  /// provision, if applicable (0 otherwise).</returns>
  BenefitAmount getPiaWindfall() const { return(piaWindfall); }
  /// <summary>Returns number of applicable PIA table.</summary>
  ///
  /// <returns>Number of applicable PIA table.</returns>
  table_type getTableNum() const { return(tableNum); }
  /// <summary>Returns title of this method.</summary>
  ///
  /// <returns>Title of this method.</returns>
  const std::string& getTitle() const { return(methodTitle); }
  virtual PiaMethod::WindfallElimType getWindfall() const;
  virtual int getYearsTotal() const;
  virtual void initialize();
  /// <summary>Returns true if this method is applicable.</summary>
  ///
  /// <returns>True if this method is applicable.</returns>
  virtual bool isApplicable() = 0;
  /// <summary>Returns old Statement assumptions indicator.</summary>
  ///
  /// <returns>True if old Statement assumptions (with 1-percent real wage
  /// gain).</returns>
  bool isPebsAssumptions() const { return pebsAssumptions; }
  double mfbCal( const double portionPiaElig[], const PercMfb& percMfbTemp,
    int year );
  void orderEarnings( int first, int last, int number );
  void prorate();
  /// <summary>Sets ame or aime.</summary>
  ///
  /// <param name="newAme">New ame.</param>
  void setAme( double newAme ) { ame = newAme; }
  void setAmeTotal();
  /// <summary>Sets indicator for use of pia calculation method.</summary>
  ///
  /// <param name="newIapp">Indicator for use of method.</param>
  void setApplicable( app_type newIapp ) { iapp = newIapp; }
  /// <summary>Sets percentage cap for MFB (for DI cases).</summary>
  ///
  /// <param name="newCap">New cap.</param>
  void setCap( double newCap ) { cap = newCap; }
  /// <summary>Sets indicator for non-zero values.</summary>
  void setDirty() { dirty = true; }
  /// <summary>Sets sum of high n years of earnings,</summary>
  ///
  /// <param name="newftearn">New sum of high n years of earnings.</param>
  void setftearn( double newftearn ) { ftearn = newftearn; }
  /// <summary>Sets disability MFB cap indicator.</summary>
  ///
  /// <param name="newIndCap">New disability MFB cap indicator.</param>
  void setIndCap( dis_mfb_cap_type newIndCap ) { indCap = newIndCap; }
  /// <summary>Sets the type of benefit.</summary>
  ///
  /// <param name="ioasdit">New type of benefit.</param>
  void setIoasdi( WorkerDataGeneral::ben_type ioasdit ) { ioasdi = ioasdit; }
  /// <summary>Sets Statement assumptions indicator.</summary>
  ///
  /// <param name="newPebsAssumptions">True if old Statement assumptions
  /// (with 1-percent real wage gain).</param>
  void setPebsAssumptions( bool newPebsAssumptions )
  { pebsAssumptions = newPebsAssumptions; }
  void setPiaEligTotal();
  void setPiaTotal();
  void setPiaWindfall();
  /// <summary>Sets number of applicable PIA table.</summary>
  ///
  /// <param name="newTableNum">Number of applicable PIA table.</param>
  void setTableNum( table_type newTableNum ) { tableNum = newTableNum; }
  void setYearCpi();
  int specMinYearsTotalCal();
  void totalEarnCal( int first, int last, int number );
  bool wep_app() const;
  static double dimaxElig( double amedimax, double piaelig,
    int year, double& capt, dis_mfb_cap_type& icapt );
  static void setPortionPiaElig( double piasub, double piapart[],
    const BendMfb& bendMfbTemp );
  static int specMinYearsCal( double pre1951, const DoubleAnnual& earnings,
    int year, const DoubleAnnual& yocAmount );
  static int specMinYearsCal( double pre1951, const DoubleAnnual& earnings,
    int year, const DoubleAnnual& yocAmount, IntAnnual& iordert );
private:
  PiaMethod& operator=( PiaMethod& newPiaMethod );
};
