// Declarations for the <see cref="BitAnnual"/> class to manage an array of
// annual bits.
//
// $Id: BitAnnual.h 1.23 2011/08/11 15:21:19EDT 044579 Development  $

#pragma once

#include "boost/serialization/access.hpp"
#include "BitAnnualBase.h"
#include "Resource.h"
#include "PiaException.h"

/// <summary>Manages an array of annual bits.</summary>
///
/// <remarks>When adding functions to this class, or changing the
/// implementation of existing functions, be careful not to use square brackets
/// ( [] ) to access <see cref="theData"/> unless you're really sure of what
/// you are doing. This is because there are different implementations of this
/// class depending on whether you are using NT or Linux. Instead, use the
/// <see cref="setBit"/> and <see cref="getBit"/> functions.</remarks>
class BitAnnual
{
  // <summary>Allows serialization of this class.</summary>
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
  { ar & theData & baseYear & lastYear; }
protected:
  /// <summary>Bit array.</summary>
  BitAnnualData theData;
  /// <summary>Base year of data.</summary>
  int baseYear;
  /// <summary>Last year of data.</summary>
  int lastYear;
public:
  BitAnnual( int newBaseYear, int newLastYear );
  BitAnnual( const BitAnnual& bitAnnual );
  virtual ~BitAnnual();
  bool any() const;
  void assign( const BitAnnual& bitAnnual, int firstYeart, int lastYeart );
  void deleteContents();
  /// <summary>Returns base year.</summary>
  //
  /// <returns>Base year.</returns>
  int getBaseYear() const { return(baseYear); }
  bool getBit( int year ) const;
  /// <summary>Returns last year.</summary>
  //
  /// <returns>Last year.</returns>
  int getLastYear() const { return(lastYear); }
  void setBit( int year, int bit );
};

#include "BitAnnualInlines.h"
