// Declarations for the <see cref="MilServDates"/> class to manage one
// military service dates record.

// $Id: msdates.h 1.27 2011/08/01 11:38:36EDT 044579 Development  $

#pragma once

#include "boost/serialization/access.hpp"
#include "datemoyr.h"

/// <summary>Manages one period of military service dates.</summary>
///
/// <remarks>An array of these is stored in <see cref="MilServDatesVec"/>.
/// </remarks>
class MilServDates
{
  /// <summary>Friend class to allow serialization.</summary>
  friend class boost::serialization::access;
  // <summary>Archives the data.</summary>
  //
  // <remarks>When the class Archive corresponds to an output archive, the
  // & operator is defined similar to <<. Likewise, when the class Archive is
  // a type of input archive, the & operator is defined similar
  // to >>.</remarks>
  //
  // <param name="ar">The archive to use.</param>
  // <param name="version">The version of the archive to use.</param>
  template< class Archive >
  void serialize( Archive& ar, const unsigned int version )
  { ar & startDate & endDate & count1 & count2 & theType; }
public:
  /// <summary>Starting date of military service.</summary>
  DateMoyr startDate;
  /// <summary>Ending date of military service.</summary>
  DateMoyr endDate;
  /// <summary>First possible date of military service wage credits.</summary>
  static const DateMoyr firstMilServDate;
  /// <summary>Last possible date of military service wage credits.</summary>
  static const DateMoyr lastMilServDate;
  /// <summary>Last possible date of pre-1951 military service wage credits.
  /// </summary>
  static const DateMoyr dec1950;
  /// <summary>Amount of credit per month for military service wage credits
  /// through 1956.</summary>
  static const double amtPerMonth37_56;
  /// <summary>Amount of credit per quarter for military service wage credits
  /// from 1957 through 1977.</summary>
  static const double amtPerQtr57_77;
  /// <summary>Amount of credit per year for military service wage credits
  /// from 1978 through 2001.</summary>
  static const double amtPerYear78_01;
private:
  /// <summary>Record number for this ssn.</summary>
  int count1;
  /// <summary>Total record count for this ssn.</summary>
  int count2;
  /// <summary>Record type.</summary>
  ///
  /// <remarks>It has the following possible values:
  /// <table>
  ///  F   found in folder.
  ///  N   not proven.
  ///  P   proven.
  /// </table></remarks>
  char theType;
public:
  MilServDates();
  ~MilServDates();
  void check() const;
  void deleteContents();
  /// <summary>Checks an ending date for validity.</summary>
  ///
  /// <remarks>Passes the stored value to the static version with 1 argument.
  /// </remarks>
  void endDateCheck() const { endDateCheck(endDate); }
  /// <summary>Returns this record number.</summary>
  ///
  /// <returns>This record number.</returns>
  int getCount1() const { return(count1); }
  /// <summary>Returns the number of records in this set.</summary>
  ///
  /// <returns>The number of records in this set.</returns>
  int getCount2() const { return(count2); }
  std::string getDatesString() const;
  double getEarn  ( unsigned year ) const;
  double getEarn3750() const;
  int getQc3750() const;
  int getQcov( unsigned year ) const;
  /// <summary>Returns the type of record.</summary>
  ///
  /// <returns>The type of record:
  /// <table>
  ///  F   found in folder.
  ///  N   not proven.
  ///  P   proven.
  /// </table></returns>
  char getType() const { return(theType); }
  void parseDates( const std::string& line );
  void parseLine( const std::string& line );
  /// <summary>Sets this record number.</summary>
  ///
  /// <param name="newCount1">New record number.</param>
  void setCount1( int newCount1 ) { count1 = newCount1; }
  /// <summary>Sets the number of records in this set.</summary>
  ///
  /// <param name="newCount2">New number of records in this set.</param>
  void setCount2( int newCount2 ) { count2 = newCount2; }
  /// <summary>Sets the type of record.</summary>
  ///
  /// <param name="newType">New type of record.</param>
  void setType( char newType ) { theType = newType; }
  /// <summary>Checks a starting date for validity.</summary>
  ///
  /// <remarks>Passes the stored value to the static version with 1 argument.
  /// </remarks>
  void startDateCheck() const { startDateCheck(startDate); }
  std::string toString() const;
  std::string toString( const std::string& format ) const;
  static void endDateCheck( const DateMoyr& date );
  static void startDateCheck( const DateMoyr& date );
};
