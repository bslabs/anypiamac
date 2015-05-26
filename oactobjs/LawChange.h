// Declarations for the <see cref="LawChange"/> class to manage law-change
// parameters.
//
// $Id: LawChange.h 1.12 2011/07/29 16:27:31EDT 044579 Development  $

#pragma once

#include <vector>
#include <string>
#include <iosfwd>

/// <summary>The basic class that manages all of the extra data and functions
/// required for one change in law concerning benefit calculations.</summary>
///
/// <remarks>This class has enough data for simple law-changes. If a
/// particular law-change requires additional data, it should subclass from
/// this class and add the additional data.</remarks>
class LawChange
{
public:
  /// <summary>Short names of changes in law.</summary>
  enum lawChangeType {
    PRE1977LAW,   /// Pre-1977 law benefits.
    BPFRACWAGE,   /// Bend points increasing at fraction of wage rate.
    BPCPI,        /// Bend points increasing at CPI rate.
    EARNINDCPI,   /// Earnings indexed to CPI rate.
    NEWFORMULA,   /// New benefit formula.
    BPSPECRATE,   /// Bend points increasing at specified rates.
    BPMINCONST,   /// Bend points increasing at wage rate minus a constant.
    AGE65COMP,    /// Change to age-65 computation point.
    DECLINEPERC,  /// Declining benefit formula percents.
    NOCPIELIG,    /// No benefit increase in year of eligibility.
    NOREINDWID,   /// No re-indexed widow guarantee.
    WINDFALL,     /// No windfall elimination provision.
    NEWSPECMIN,   /// New special minimum (years and/or amount).
    NOOLDSTART,   /// No old-start method.
    DIDROP5,      /// 5-year DI dropout.
    TRANSGUAR1,   /// Use earnings after age 62 for transitional guarantee.
    MARRLENGTH,   /// Number of years married for divorced spouse benefit.
    TRANSGUAR3,   /// Use average of type-16 and aime for transitional guar.
    ALLEARN,      /// Use all earnings in AIME computation.
    DROPOUTCHG,   /// Reduce number of dropout years.
    RETTESTCHG,   /// Retirement test ad hoc change.
    SANFORD1,     /// Original Sanford proposal (3% to retirement).
    NOPIATABLE,   /// No PIA table method.
    NOTRANSGUAR,  /// No transitional guarantee method.
    TAXRATECHG,   /// Tax rate change.
    SANFORD2,     /// Second Sanford notch prop (3.25% to age 62; red yrs).
    NRACHANGE,    /// Change full retirement age.
    CHILDCARECREDIT,  /// Child care credit years.
    RETROWAGEIND,  /// Retroactive wage indexing.
    TRANSGUAR4,   /// Corman-Myers COLA limitation (notch proposal).
    WIFEFACTOR,   /// Reduce aged wife factor to .33.
    WIDFACTOR,    /// Modify the aged widow(er) benefit calculation.
    TAXBENCHG,    /// Change in tax on benefits provision.
    PSAACCT,      /// Add PSA accounts.
    WAGEBASECHG,  /// Ad hoc wage base change.
    STATELOCAL,   /// Cover State and local employees.
    FEDERAL,      /// Cover Federal employees.
    CHILDCAREDROPOUT,  /// Child care dropout years.
    COLACHANGE,   /// Change in the COLA increase.
    NODIBGUAR,    /// No DIB guarantee.
    MAXLCH        /// Number of different law changes.
  };
private:
  /// <summary>Earliest starting year for changes.</summary>
  static int startYearLC;
  /// <summary>Latest year for changes.</summary>
  static int endYearLC;
  /// <summary>Last year of proposal to change law.</summary>
  int endYear;
  /// <summary>Indicators for change from present law.</summary>
  int ind;
  /// <summary>Type of phase-in for this law-change.</summary>
  ///
  /// <remarks>It has the following possible values:
  /// <table>
  ///   0   phased in by year of eligiblity,
  ///   1   effective immediately.
  /// </table></remarks>
  int phaseType;
  /// <summary>First year of proposal to change law.</summary>
  int startYear;
  /// <summary>Title of this law-change.</summary>
  std::string titleLC;
public:
  LawChange();
  LawChange( const std::string& newTitle );
  virtual ~LawChange();
  /// <summary>Returns ending year for this law-change.</summary>
  ///
  /// <returns>Ending year for this law-change.</returns>
  int getEndYear() const { return endYear; }
  std::vector< std::string > getFirstLine( std::istream& infile );
  /// <summary>Returns indicator for this law-change.</summary>
  ///
  /// <returns>Indicator for this law-change.</returns>
  int getInd() const { return ind; }
  /// <summary>Returns starting year for this law-change.</summary>
  ///
  /// <returns>Starting year for this law-change.</returns>
  int getStartYear() const { return startYear; }
  /// <summary>Returns indicator for type of phase-in.</summary>
  ///
  /// <returns>Indicator for type of phase-in.
  /// <table>
  ///   0   phased in by year of eligiblity,
  ///   1   effective immediately.
  /// </table></returns>
  int getPhaseType() const { return phaseType; }
  /// <summary>Returns title of this law change.</summary>
  ///
  /// <returns>Title of this law change.</returns>
  std::string getTitle() const { return titleLC; }
  bool isEffective( int eligYear, int benYear ) const;
  virtual void prepareStrings( std::vector< std::string >& outputString );
  virtual void read( std::istream& infile );
  /// <summary>Sets ending year for this law-change.</summary>
  ///
  /// <param name="newEndYear">New ending year for this law-change.</param>
  void setEndYear( int newEndYear ) { endYear = newEndYear; }
  /// <summary>Sets change indicator for this law-change.</summary>
  ///
  /// <param name="newInd">New change indicator for this law-change.</param>
  void setInd( int newInd ) { ind = newInd; }
  /// <summary>Sets starting year for this law-change.</summary>
  ///
  /// <param name="newStartYear">New starting year for this law-change.
  /// </param>
  void setStartYear( int newStartYear) { startYear = newStartYear; }
  /// <summary>Sets indicator for type of phase-in.</summary>
  ///
  /// <param name="newPhaseType">New indicator for type of phase-in.</param>
  void setPhaseType( int newPhaseType ) { phaseType = newPhaseType; }
  virtual void write( std::ostream& out ) const;
  void writeFirstLine( std::ostream& out ) const;
  static void bendPointCheck( double bendPoint );
  static void bpincCheck( double bpinc );
  static void constantPercCheck( double constantPerc );
  /// <summary>Returns latest possible ending year for a law-change.</summary>
  ///
  /// <returns>Latest possible starting year for a law-change.</returns>
  static int getEndYearLC() { return endYearLC; }
  /// <summary>Returns earliest possible ending year for a law-change.
  /// </summary>
  ///
  /// <returns>Earliest possible starting year for a law-change.</returns>
  static int getStartYearLC() { return endYearLC; }
  static void percCheck ( double perc );
  static void proportionBendPointsCheck  ( double proportionBendPoints );
  /// <summary>Sets latest possible ending year for a law-change.</summary>
  ///
  /// <param name="newEndYearLC">Latest possible starting year for a
  /// law-change.</param>
  static void setEndYearLC( int newEndYearLC ) { endYearLC = newEndYearLC; }
  /// <summary>Sets earliest possible starting year for a law-change.
  /// </summary>
  ///
  /// <param name="newStartYearLC">Earliest possible starting year for a
  /// law-change.</param>
  static void setStartYearLC( int newStartYearLC )
  { startYearLC = newStartYearLC; }
  static void startCheck( int start );
};
