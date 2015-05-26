// Declarations for the <see cref="QtrYear"/> class to handle a year and
// quarter.

// $Id: qtryear.h 1.30 2011/08/19 15:57:44EDT 044579 Development  $

#pragma once

#include <string>
#include "boost/serialization/access.hpp"
#include "boost/date_time/gregorian/greg_date.hpp"
class DateMoyr;

/// <summary>Manages a year and quarter stored as unsigned integers.</summary>
///
/// <remarks>The quarter and year are stored as unsigned shorts to save space.
/// The quarter is 0 to 3, and the year is 1 to 65000.</remarks>
class QtrYear
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
  { ar & quarterOfYear & yearPart; }
private:
  /// <summary>Quarter of year (0-3).</summary>
  unsigned short quarterOfYear;
  /// <summary>The year (4 digits).</summary>
  unsigned short yearPart;
public:
  /// <summary>Initializes the quarter and year to 0.</summary>
  QtrYear() : quarterOfYear(0u), yearPart(0u) { }
  /// <summary>Initializes the quarter and year from an array of size 2.
  /// </summary>
  ///
  /// <param name="newQtrYear">Quarter and year in an array.</param>
  QtrYear( unsigned newQtrYear[2] ) :
    quarterOfYear(static_cast<unsigned short>(newQtrYear[0])),
    yearPart(static_cast<unsigned short>(newQtrYear[1])) { }
  /// <summary>Initializes the quarter and year from two specified values.
  /// </summary>
  ///
  /// <param name="newQuarter">Quarter of year (0-3) to use.</param>
  /// <param name="newYear">Year to use.</param>
  QtrYear( unsigned newQuarter, unsigned newYear ) :
    quarterOfYear(static_cast<unsigned short>(newQuarter)),
    yearPart(static_cast<unsigned short>(newYear)) { }
  QtrYear( const DateMoyr& dateMoyr );
  QtrYear( const boost::gregorian::date& dateModyyr );
  /// <summary>Destructor.</summary>
  ///
  /// <remarks>Does nothing.</remarks>
  ~QtrYear() { }
  QtrYear& operator=( const QtrYear& qtrYear );
  QtrYear& operator=( const DateMoyr& date );
  QtrYear& operator=( const boost::gregorian::date& date );
  void add( unsigned addQuarter );
  void add( const QtrYear& addQtrYear );
  bool operator<( const QtrYear& qtrYear ) const;
  /// <summary>Compares this quarter and year to another one for equality.
  /// </summary>
  ///
  /// <returns>True if this quarter and year is equal to argument, false
  /// otherwise.</returns>
  ///
  /// <param name="qtrYear">The quarter and year to compare to.</param>
  bool operator==( const QtrYear& qtrYear ) const
    { return(yearPart == qtrYear.yearPart &&
      quarterOfYear == qtrYear.quarterOfYear); }
  /// <summary>Checks for valid quarter and year.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> if date is not
  /// ok.</exception>
  void check() const { qtrCheck(); yearCheck(); }
  /// <summary>Zeroes quarter and year.</summary>
  void deleteContents() { quarterOfYear = yearPart = 0u; }
  static int diff( const QtrYear& qtrYear1, const QtrYear& qtrYear2 );
  /// <summary>Returns quarter (0-3).</summary>
  ///
  /// <returns>Quarter of year (0-3).</returns>
  unsigned getQuarter() const { return(quarterOfYear); }
  /// <summary>Returns year.</summary>
  ///
  /// <returns>The year.</returns>
  unsigned getYear() const { return(yearPart); }
  void qtrCheck () const;
  /// <summary>Sets quarter.</summary>
  ///
  /// <param name="newQuarter">New quarter of year (0-3).</param>
  void setQuarter( unsigned newQuarter )
  { quarterOfYear = static_cast<unsigned short>(newQuarter); }
  /// <summary>Sets year.</summary>
  ///
  /// <param name="newYear">New year.</param>
  void setYear( unsigned newYear )
  { yearPart = static_cast<unsigned short>(newYear); }
  void subtract( unsigned subQuarter );
  void subtract( const QtrYear& subQtrYear );
  DateMoyr toDateMoyr() const;
  std::string toString() const;
  void yearCheck() const;
};
