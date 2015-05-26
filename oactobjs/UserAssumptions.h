// Declarations for the <see cref="UserAssumptions"/> class to manage
// user-specified assumptions.
//
// $Id: UserAssumptions.h 1.19 2011/08/11 08:46:58EDT 044579 Development  $

#pragma once

#include "cachup.h"
#include "biproj.h"
#include "dbleann.h"
#include "intann.h"
#include "AssumptionType.h"
#include "BaseChangeType.h"

/// <summary>Manages all the assumptions about future average wage increases,
/// wage bases, benefit increases, and catch-up benefit increases that a user
/// might specify.</summary>
///
/// <remarks>An instance of this class is used in the Anypia program.
/// </remarks>
class UserAssumptions
{
public:
  /// <summary>Projected increases in annual average earnings.</summary>
  DoubleAnnual awincproj;
  /// <summary>Wage base under present law, 1937 to maxyear, as read in.
  /// </summary>
  DoubleAnnual baseOasdi;
  /// <summary>Old-law wage bases (ignoring 1979-81 ad hoc increases), 1937
  /// to maxyear, as read in.</summary>
  DoubleAnnual base77;
  /// <summary>Projected benefit increases.</summary>
  DoubleAnnual biproj;
  /// <summary>Catch-up benefit increases.</summary>
  Catchup catchup;
  /// <summary>Title of user's average wages.</summary>
  std::string titleAw;
  /// <summary>Title of user's benefit increases</summary>
  std::string titleBi;
private:
  /// <summary>Benefit increase assumption.</summary>
  AssumptionType::assum_type ialtbi;
  /// <summary>Answer to catch-up benefit increase prompt.</summary>
  char anscch;
  /// <summary>Average wage increase assumption.</summary>
  AssumptionType::assum_type ialtaw;
  /// <summary>Ad hoc base change (1=none, 2=change).</summary>
  BaseChangeType::change_type ibasch;
  /// <summary>Last benefit increase assumption read in.</summary>
  AssumptionType::assum_type jaltbi;
  /// <summary>Last average wage increase assumption read in.</summary>
  AssumptionType::assum_type jaltaw;
  /// <summary>Last ad hoc base change read in.</summary>
  BaseChangeType::change_type jbasch;
  /// <summary>Last year of projection.</summary>
  int lastYear;
  /// <summary>Current year (year after last known benefit increase).
  /// </summary>
  static int istart;
  /// <summary>Maximum year allowed.</summary>
  static int maxyear;
public:
  UserAssumptions( int newLastYear );
  void deleteContents();
  /// <summary>Returns catch-up benefit increase indicator.</summary>
  ///
  /// <returns>Catch-up benefit increase indicator.</returns>
  char getAnscch() const { return(anscch); }
  /// <summary>Returns average wage increase assumption.</summary>
  ///
  /// <returns>Average wage increase assumption.</returns>
  AssumptionType::assum_type getIaltaw() const { return(ialtaw); }
  /// <summary>Returns benefit increase assumption.</summary>
  ///
  /// <returns>Benefit increase assumption.</returns>
  AssumptionType::assum_type getIaltbi() const { return(ialtbi); }
  /// <summary>Returns wage base projection indicator.</summary>
  ///
  /// <returns>Wage base projection indicator.</returns>
  BaseChangeType::change_type getIbasch() const { return(ibasch); }
  /// <summary>Returns last average wage increase assumption read in.
  /// </summary>
  ///
  /// <returns>Last average wage increase assumption read in.</returns>
  AssumptionType::assum_type getJaltaw() const { return(jaltaw); }
  /// <summary>Returns last benefit increase assumption read in.</summary>
  ///
  /// <returns>Last benefit increase assumption read in.</returns>
  AssumptionType::assum_type getJaltbi() const { return(jaltbi); }
  /// <summary>Returns last wage base projection indicator read in.</summary>
  ///
  /// <returns>Last wage base projection indicator read in.</returns>
  BaseChangeType::change_type getJbasch() const { return(jbasch); }
  void pebsasmCheck() const;
  void resetTriggers();
  void setAnscch( char ansccht );
  void setIaltaw( int ialtawt );
  void setIaltbi( int ialtbit );
  void setIbasch( int ibascht );
  void setIstart( int istartt );
  /// <summary>Sets the previous average wage increase assumption.</summary>
  ///
  /// <param name="jaltawt">New value of previous average wage increase
  /// assumption.</param>
  void setJaltaw( AssumptionType::assum_type jaltawt ) { jaltaw = jaltawt; }
  /// <summary>Sets the previous benefit increase assumption.</summary>
  ///
  /// <param name="jaltbit">New value of previous benefit increase
  /// assumption.</param>
  void setJaltbi( AssumptionType::assum_type jaltbit ) { jaltbi = jaltbit; }
  /// <summary>Sets the previous wage base assumption.</summary>
  ///
  /// <param name="jbascht">New value of previous wage base assumption.
  /// </param>
  void setJbasch( BaseChangeType::change_type jbascht ) { jbasch = jbascht; }
  void setLastYear( int year );
  /// <summary>Sets average wage change assumption title.</summary>
  ///
  /// <param name="newTitleAw">New title to use.</param>
  void setTitleAw( const std::string& newTitleAw ) { titleAw = newTitleAw; }
  /// <summary>Sets benefit increase assumption title.</summary>
  ///
  /// <param name="newTitleBi">New title to use.</param>
  void setTitleBi( const std::string& newTitleBi ) { titleBi = newTitleBi; }
  /// <summary>Initializes previous assumptions indicators.</summary>
  void zeroJalt()
  { jaltbi = jaltaw = AssumptionType::NONE; jbasch = BaseChangeType::NONE; }
  static void anscchCheck( char ansccht );
  /// <summary>Returns current year (year after last known benefit
  /// increase).</summary>
  ///
  /// <returns>Current year (year after last known benefit increase).</returns>
  static int getIstart() { return(istart); }
  static void istartCheck( int istartt );
};
