// Declarations for the <see cref="EarnProject"/> class to handle an array of
// earnings, with forward and backward projections and specified levels of
// earnings.

// $Id: EarnProject.h 1.22 2011/07/29 15:26:15EDT 044579 Development  $

#pragma once

#include "avgwg.h"
#include "intann.h"
#include "oactcnst.h"
class WorkerDataGeneral;

/// <summary>Manages an array of earnings, with forward and backward
/// projections and specified levels of earnings.</summary>
///
/// <remarks>An instance of this class is used in the Anypia program.
/// </remarks>
class EarnProject
{
public:
  /// <summary>Type of earnings.</summary>
  enum earn_type {
    ENTERED,         // Entered earnings.
    MAXIMUM,         // Maximum earnings.
    HIGH,            // High earnings.
    AVERAGE,         // Average earnings.
    LOW,             // Low earnings.
    OLDLAW_MAXIMUM,  // Old-law maximum.
    CHILDCARE_YEAR,  // Childcare year.
    NUM_TYPES        // Number of earnings types.
  };
  /// <summary>Type of earnings projection.</summary>
  ///
  /// <remarks>There are two instances of this enum in this class, one for a
  /// backward projection and one for a forward projection.</remarks>
  enum earn_proj_type {
    NO_PROJ,       // No projection.
    AVGWAGE_PROJ,  // Projection related to average wage increase.
    CONSTANT_PROJ  // Projection by a constant percentage.
  };
  /// <summary>Worker earnings, 1937 to maxyear, as read in.</summary>
  AverageWage earnpebs;
  /// <summary>Lowest acceptable projection percentage.</summary>
  static const double PERC_LOW;
  /// <summary>Highest acceptable projection percentage.</summary>
  static const double PERC_HIGH;
private:
  /// <summary>Type of earnings (see <see cref="earn_type"/>).</summary>
  IntAnnual earntype;
  /// <summary>First year of earnings before projection.</summary>
  int firstYear;
  /// <summary>Last year of earnings before projection.</summary>
  int lastYear;
  /// <summary>Indicator for backward projection.</summary>
  earn_proj_type projback;
  /// <summary>Percentage used in backward projection.</summary>
  ///
  /// <remarks>If <see cref="projback"/> is
  /// <see cref="earn_proj_type::AVGWAGE_PROJ"/>, it is the increment to
  /// average wage increase. If <see cref="projback"/> is
  /// <see cref="earn_proj_type::CONSTANT_PROJ"/>, it is the constant
  /// percentage increase.</remarks>
  double percback;
  /// <summary>Indicator for forward projection.</summary>
  earn_proj_type projfwrd;
  /// <summary>Percentage used in forward projection.</summary>
  ///
  /// <remarks>If <see cref="projfwrd"/> is
  /// <see cref="earn_proj_type::AVGWAGE_PROJ"/>, it is the increment to
  /// average wage increase. If <see cref="projfwrd"/> is
  /// <see cref="earn_proj_type::CONSTANT_PROJ"/>, it is the constant
  /// percentage increase.</remarks>
  double percfwrd;
  /// <summary>Maximum year of projection.</summary>
  static int maxyear;
  /// <summary>Earnings selection titles.</summary>
  static const char *earnmal[NUM_TYPES];
public:
  explicit EarnProject( int newMaxYear );
  virtual ~EarnProject();
  /// <summary>Returns true if any earnings are entered.</summary>
  ///
  /// <returns>True if any earnings are entered.</returns>
  bool anyEntered() const
  { for (int yr1 = firstYear; yr1 <= lastYear; yr1++) {
      if (earntype[yr1] == ENTERED) return true; }
    return false; }
  virtual void deleteContents();
  /// <summary>Returns array of indicators for type of earnings.</summary>
  ///
  /// <returns>Array of indicators for type of earnings. The possible values
  /// for the entries are in <see cref="earn_type"/>.</returns>
  const IntAnnual& getEarntype() const { return(earntype); }
  /// <summary>Returns first year of earnings.</summary>
  ///
  /// <returns>First year of earnings.</returns>
  int getFirstYear() const { return firstYear; }
  /// <summary>Returns last year of earnings.</summary>
  ///
  /// <returns>Last year of earnings.</returns>
  int getLastYear() const { return lastYear; }
  /// <summary>Returns percentage used in backwards projection.</summary>
  ///
  /// <returns>Percentage used in backwards projection.</returns>
  double getPercback() const { return(percback); }
  /// <summary>Returns percentage used in forwards projection.</summary>
  ///
  /// <returns>Percentage used in forwards projection.</returns>
  double getPercfwrd() const { return(percfwrd); }
  /// <summary>Returns backward projection indicator.</summary>
  ///
  /// <returns>Backward projection indicator.</returns>
  earn_proj_type getProjback() const { return(projback); }
  /// <summary>Returns forward projection indicator.</summary>
  ///
  /// <returns>Forward projection indicator.</returns>
  earn_proj_type getProjfwrd() const { return(projfwrd); }
  bool need_aw( int year, int iendt ) const;
  bool project( WorkerDataGeneral& workerData, const Awinc& fqinc ) const;
  /// <summary>Sets earnings for specified year.</summary>
  ///
  /// <param name="year">Year of earnings.</param>
  /// <param name="newEarnpebs">Amount of earnings.</param>
  void setEarnpebs( int year, double newEarnpebs )
  {
  #ifndef NDEBUG
    earnpebsCheck(newEarnpebs);
  #endif
    earnpebs[year] = newEarnpebs;
  }
  /// <summary>Sets type of earnings for specified year.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_EARNTYPEYR"/> if year is out of
  /// range (only in debug mode); of type <see cref="PIA_IDS_EARNTYPE"/> if
  /// type of earnings is out of range (only in debug mode).</exception>
  ///
  /// <param name="year">Year of earnings.</param>
  /// <param name="newEarntype">Type of earnings. The possible values
  /// are in <see cref="earn_type"/>.</param>
  void setEarntype( int year, int newEarntype )
  {
  #ifndef NDEBUG
    if (year < YEAR37)
      throw PiaException(PIA_IDS_EARNTYPEYR);
    earntypeCheck(newEarntype);
  #endif
    earntype[year] = newEarntype;
  }
  /// <summary>Sets first year of specified earnings (earnings could be
  /// projected backward before this year).</summary>
  ///
  /// <param name="newFirstYear">New first year of earnings before
  /// projection.</param>
  void setFirstYear( int newFirstYear )
  {
  #ifndef NDEBUG
    ibegin2Check(newFirstYear);
  #endif
    firstYear = newFirstYear;
  }
  /// <summary>Sets last year of specified earnings (earnings could be
  /// projected forward after this year).</summary>
  ///
  /// <param name="newLastYear">New last year of earnings before projection.
  /// </param>
  void setLastYear( int newLastYear )
  {
  #ifndef NDEBUG
    iendCheck(newLastYear);
  #endif
    lastYear = newLastYear;
  }
  void setPebsData( int year, int ibegint );
  void setPebsEarn( int year );
  /// <summary>Sets backward projection percentage.</summary>
  ///
  /// <param name="newPercback">New backward projection percentage.</param>
  void setPercback( double newPercback )
  {
  #ifndef NDEBUG
    percbackCheck(newPercback);
  #endif
    percback = newPercback;
  }
  /// <summary>Sets percentage in forward projection.</summary>
  ///
  /// <param name="newPercfwrd">New forward projection percentage.</param>
  void setPercfwrd( double newPercfwrd )
  {
  #ifndef NDEBUG
    percfwrdCheck(newPercfwrd);
  #endif
    percfwrd = newPercfwrd;
  }
  /// <summary>Sets backward projection indicator.</summary>
  ///
  /// <param name="newProjback">New backward projection indicator. The
  /// possible values are in <see cref="earn_proj_type"/>.</param>
  void setProjback( int newProjback )
  {
  #ifndef NDEBUG
    projbackCheck1(newProjback);
  #endif
    projback = (earn_proj_type)newProjback;
  }
  /// <summary>Sets forward projection indicator.</summary>
  ///
  /// <param name="newProjfwrd">New forward projection indicator. The
  /// possible values are in <see cref="earn_proj_type"/>.</param>
  void setProjfwrd( int newProjfwrd )
  {
  #ifndef NDEBUG
    projfwrdCheck1(newProjfwrd);
  #endif
    projfwrd = (earn_proj_type)newProjfwrd;
  }
  std::string toString( int year ) const;
  /// <summary>Checks for first and last year of earnings within bounds.
  /// </summary>
  ///
  /// <remarks>Calls the static version with 4 arguments, adding firstYear and
  /// lastYear as arguments.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_END1"/> if first year of earnings in forward
  /// projection period is before last year of earnings in backward
  /// projection period; of type <see cref="PIA_IDS_END3"/> if first year of
  /// earnings in forward projection period is after last year of earnings in
  /// forward projection period; of type <see cref="PIA_IDS_END4"/> if the
  /// total number of years of earnings is greater than the maximum allowed
  /// (<see cref="MAXYRS"/>); of type <see cref="PIA_IDS_END1"/> if last year
  /// of earnings is before 1937; of type <see cref="PIA_IDS_END2"/> if last
  /// year of earnings is after maximum year allowed.</exception>
  ///
  /// <param name="ibegint">First year of earnings in backward
  /// projection period.</param>
  /// <param name="iendt">Last year of earnings in forward projection
  /// period.</param>
  void yearsCheck( int ibegint, int iendt ) const
  { yearsCheck(ibegint, firstYear, iendt, lastYear); }
  /// <summary>Zeroes out earnings before firstYear and after lastYear.
  /// </summary>
  void zeroEarn()
  { earnpebs.assign(0.0, YEAR37, firstYear - 1);
    earnpebs.assign(0.0, lastYear + 1, maxyear); }
  static void earnpebsCheck( double earnpebst );
  /// <summary>Checks type of earnings.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_EARNTYPE"/> if type of earnings is out of range.
  /// </exception>
  ///
  /// <param name="earntypet">Type of earnings to check. The
  /// possible values are in <see cref="earn_type"/>.</param>
  static void earntypeCheck( int earntypet )
  { if (earntypet < static_cast<int>(ENTERED) ||
     earntypet > static_cast<int>(LOW))
     throw PiaException(PIA_IDS_EARNTYPE); }
  /// <summary>Checks for first year of earnings within bounds, using
  /// passed values, with a backward projection.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_BEGIN2"/> if last year of earnings is before first
  /// year of earnings in backward projection period.</exception>
  ///
  /// <param name="ibegint">First year of earnings in backward projection
  /// period.</param>
  /// <param name="ibegin1t">Last year of earnings in backward projection
  /// period.</param>
  static void ibeginCheck( int ibegint, int ibegin1t )
  { if (ibegin1t < ibegint) throw PiaException(PIA_IDS_BEGIN2); }
  /// <summary>Checks for first year of earnings in bounds.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_BEGIN3"/> if first year of earnings is before 1937;
  /// of type <see cref="PIA_IDS_BEGIN5"/> if first year of
  /// earnings is after maximum year allowed.</exception>
  ///
  /// <param name="ibegint">Year to check.</param>
  static void ibegin2Check( int ibegint )
  { if (ibegint < YEAR37) throw PiaException(PIA_IDS_BEGIN3);
    if (ibegint > maxyear) throw PiaException(PIA_IDS_BEGIN5); }
  /// <summary>Checks for last year of earnings within bounds.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_END1"/> if last year of earnings is before 1937; of
  /// type <see cref="PIA_IDS_END2"/> if last year of earnings is after
  /// maximum year allowed.</exception>
  ///
  /// <param name="iendt">Last year of earnings.</param>
  static void iendCheck( int iendt )
  { if (iendt < YEAR37) throw PiaException(PIA_IDS_END1);
    if (iendt > maxyear) throw PiaException(PIA_IDS_END2); }
  /// <summary>Checks backward projection percentage.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_PERCBACK"/> if backward projection
  /// percentage is out of range.</exception>
  ///
  /// <param name="percbackt">Percentage to check.</param>
  static void percbackCheck( double percbackt )
  { if (percbackt < PERC_LOW || percbackt > PERC_HIGH)
      throw PiaException(PIA_IDS_PERCBACK); }
  /// <summary>Checks forward projection percentage.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_PERCFWRD"/> if backward projection percentage is out
  /// of range.</exception>
  ///
  /// <param name="percfwrdt">Percentage to check.</param>
  static void percfwrdCheck( double percfwrdt )
  { if (percfwrdt < PERC_LOW || percfwrdt > PERC_HIGH)
      throw PiaException(PIA_IDS_PERCFWRD); }
  /// <summary>Checks backward projection indicator in general case.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_PROJBACK1"/> if backward projection indicator is not
  /// in the <see cref="earn_proj_type"/> range.</exception>
  ///
  /// <param name="projbackt">Projection indicator to check. The
  /// possible values are in <see cref="earn_proj_type"/>.</param>
  static void projbackCheck1( int projbackt )
  { if (projbackt != (int)NO_PROJ && projbackt != (int)AVGWAGE_PROJ &&
      projbackt != (int)CONSTANT_PROJ)
      throw PiaException(PIA_IDS_PROJBACK1); }
  /// <summary>Checks backward projection indicator in backward projection
  /// case.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_PROJBACK2"/> if backward projection indicator is not
  /// an <see cref="earn_proj_type"/> other than
  /// <see cref="earn_proj_type::NO_PROJ"/>.</exception>
  ///
  /// <param name="projbackt">Projection indicator to check. The
  /// possible values are in <see cref="earn_proj_type"/>.</param>
  static void projbackCheck2( int projbackt )
  { if (projbackt != (int)NO_PROJ && projbackt != (int)AVGWAGE_PROJ)
      throw PiaException(PIA_IDS_PROJBACK2); }
  /// <summary>Checks forward projection indicator in general case.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_PROJFWRD1"/> if forward projection indicator is not
  /// in the <see cref="earn_proj_type"/> range.</exception>
  ///
  /// <param name="projfwrdt">Projection indicator to check. The
  /// possible values are in <see cref="earn_proj_type"/>.</param>
  static void projfwrdCheck1( int projfwrdt )
  { if (projfwrdt != (int)NO_PROJ && projfwrdt != (int)AVGWAGE_PROJ &&
      projfwrdt != (int)CONSTANT_PROJ)
      throw PiaException(PIA_IDS_PROJFWRD1); }
  /// <summary>Checks forward projection indicator in forward projection
  /// case.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_PROJFWRD2"/> if forward projection indicator is not
  /// an <see cref="earn_proj_type"/> other than
  /// <see cref="earn_proj_type::NO_PROJ"/>.</exception>
  ///
  /// <param name="projfwrdt">Projection indicator to check. The
  /// possible values are in <see cref="earn_proj_type"/>.</param>
  static void projfwrdCheck2( int projfwrdt )
  { if (projfwrdt != (int)NO_PROJ && projfwrdt != (int)AVGWAGE_PROJ)
      throw PiaException(PIA_IDS_PROJFWRD1); }
  /// <summary>Checks for first and last year of earnings within bounds.
  /// </summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_END1"/> if first year of earnings in forward
  /// projection period is before last year of earnings in backward
  /// projection period; of type <see cref="PIA_IDS_END3"/> if first year of
  /// earnings in forward projection period is after last year of earnings in
  /// forward projection period; of type <see cref="PIA_IDS_END4"/> if the
  /// total number of years of earnings is greater than the maximum allowed
  /// (<see cref="MAXYRS"/>); of type <see cref="PIA_IDS_END1"/> if last year
  /// of earnings is before 1937; of type <see cref="PIA_IDS_END2"/> if last
  /// year of earnings is after maximum year allowed.</exception>
  ///
  /// <param name="ibegint">First year of earnings in backward
  ///  projection period.</param>
  /// <param name="iendt">Last year of earnings in forward projection
  ///  period.</param>
  /// <param name="ibegin1t">Last year of earnings in backward projection
  ///  period.</param>
  /// <param name="iend1t">First year of earnings in forward projection
  ///  period.</param>
  static void yearsCheck( int ibegint, int ibegin1t, int iendt, int iend1t )
  { if (iend1t < ibegin1t) throw PiaException(PIA_IDS_END1);
    if (iend1t > iendt) throw PiaException(PIA_IDS_END3);
    if (iendt > ibegint + MAXYRS - 1) throw PiaException(PIA_IDS_END4); }
};
