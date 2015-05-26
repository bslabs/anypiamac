// Declarations for the <see cref="QcArray"/> class to handle an array
// of annual number of quarters of coverage.

// $Id: QcArray.h 1.27 2011/08/09 14:58:42EDT 044579 Development  $

#pragma once

#include <vector>
#include "boost/serialization/access.hpp"
#include "qc.h"
#include "QcPacked.h"
#include "qtryear.h"

/// <summary>Manages an array of annual quarters of coverage.</summary>
///
/// <remarks>The quarters of coverage are stored in <see cref="QcPacked"/>
/// structures to save space.</remarks>
///
/// <seealso cref="QcPacked"/>
class QcArray
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
  { ar & firstYear & lastYear & theData; }
private:
  /// <summary>First year of data.</summary>
  unsigned short firstYear;
  /// <summary>Last year of data.</summary>
  unsigned short lastYear;
  /// <summary>Quarters of coverage data.</summary>
  std::vector< QcPacked > theData;
public:
  QcArray( int firstyr, int lastyr );
  QcArray( const QcArray& qcArray );
  /// <summary>Deletes allocated memory.</summary>
  ~QcArray() { }
  unsigned accumulate( int firstyr, int lastyr, unsigned startValue ) const;
  unsigned accumulate( const QtrYear& qtrYear1, const QtrYear& qtrYear2,
    unsigned startValue ) const;
  void assign( unsigned value, int firstYeart, int lastYeart );
  void assign( const QcArray &qcArray, int firstYeart, int lastYeart );
  /// <summary>Sets values to zero.</summary>
  ///
  /// <remarks>The version with no arguments sets all values to zero. The
  /// version with 2 arguments sets values to zero in the specified
  /// range.</remarks>
  void deleteContents() { deleteContents(firstYear, lastYear); }
  void deleteContents( int firstyr, int lastyr );
  unsigned get( int year ) const;
  /// <summary>Returns the last year of data.</summary>
  ///
  /// <returns>The last year of data.</returns>
  int getLastYear() const { return(lastYear); }
  void set( int year, unsigned value );
  std::string toString( int firstyr, int lastyr ) const;
};
