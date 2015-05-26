// Declarations for the <see cref="RailRoadData"/> class to manage edited
// railroad record.

// $Id: railroad.h 1.28 2011/10/11 15:24:37EDT 044579 Development  $

#pragma once

#include "boost/serialization/access.hpp"
#include "dbleann.h"
#include "QcArray.h"

/// <summary>Manages the railroad data for one worker.</summary>
///
/// <remarks>Each record contains railroad quarters of coverage for 1937-50
/// (total) and for each year from 1951 to 1977. It also contains railroad
/// earnings for 1937-50 (total) and for each year from 1951 to the last year.
/// </remarks>
class RailRoadData
{
  /// <summary>Friend class to allow serialization.</summary>
  friend class boost::serialization::access;
  // <summary>Archives the data.</summary>
  //
  // <remarks>When the class Archive corresponds to an output archive, the
  // & operator is defined similar to <<. Likewise, when the class Archive is
  // a type of input archive, the & operator is defined similar to >>.
  // </remarks>
  //
  // <param name="ar">The archive to use.</param>
  // <param name="version">The version of the archive to use.</param>
  template< class Archive >
  void serialize( Archive& ar, const unsigned int version )
  { ar & qc3750 & earn3750 & firstYear & lastYear & qcArray & earningsArray; }
private:
  /// <summary>Total railroad quarters of coverage, 1937 to 1950.</summary>
  int qc3750;
  /// <summary>Total railroad earnings, 1937 to 1950.</summary>
  double earn3750;
  /// <summary>First year of railroad earnings.</summary>
  int firstYear;
  /// <summary>Last year of railroad earnings.</summary>
  int lastYear;
public:
  /// <summary>Annual railroad quarters of coverage, from 1951 to 1977.
  /// </summary>
  QcArray qcArray;
  /// <summary>Annual railroad earnings, starting in 1951.</summary>
  DoubleAnnual earningsArray;
public:
  explicit RailRoadData( int year );
  ~RailRoadData();
  RailRoadData& operator=( const RailRoadData& newData );
  void deleteContents();
  std::string get3750String( int width, int decimals ) const;
  std::string getDecadeEarningsString( int decade, int width,
    int decimals ) const;
  int getDecades() const;
  /// <summary>Returns total earnings 1937 to 1950.</summary>
  ///
  /// <returns>Total earnings 1937 to 1950.</returns>
  double getEarn3750() const { return(earn3750); }
  /// <summary>Returns first year of earnings.</summary>
  ///
  /// <returns>First year of earnings.</returns>
  int getFirstYear() const { return firstYear; }
  /// <summary>Returns last year of earnings.</summary>
  ///
  /// <returns>Last year of earnings.</returns>
  int getLastYear() const { return lastYear; }
  /// <summary>Returns total quarters of coverage 1937 to 1950.</summary>
  ///
  /// <returns>Total quarters of coverage 1937 to 1950.</returns>
  int getQc3750() const { return(qc3750); }
  std::string getQcString() const;
  std::string getYearsString() const;
  bool isNonzero() const;
  void parse3750( const std::string& line, int width );
  void parseEarnings( int decade, const std::string& line, int width );
  void parseQcs( const std::string& line );
  void parseYears( const std::string& line );
  /// <summary>Sets total earnings 1937 to 1950.</summary>
  ///
  /// <param name="newEarn3750">New total earnings.</param>
  void setEarn3750( double newEarn3750 ) { earn3750 = newEarn3750; }
  /// <summary>Sets first year of earnings.</summary>
  ///
  /// <param name="newFirstYear">New first year of earnings.</param>
  void setFirstYear( int newFirstYear )
  { railroadYearCheck(newFirstYear); firstYear = newFirstYear; }
  /// <summary>Sets last year of earnings.</summary>
  ///
  /// <param name="newLastYear">New last year of earnings.</param>
  void setLastYear( int newLastYear )
  { railroadYearCheck(newLastYear); lastYear = newLastYear; }
  /// <summary>Sets total quarters of coverage 1937 to 1950.</summary>
  ///
  /// <param name="newQc3750">New total quarters of coverage.</param>
  void setQc3750( int newQc3750 ) { qc3750 = newQc3750; }
  static void railroadYearCheck( int year);
};
