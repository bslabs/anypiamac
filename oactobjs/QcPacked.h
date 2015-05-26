// Declaration of the <see cref="QcPacked"/> class to hold 8 quarters of
// coverage amounts in one word.

// $Id: QcPacked.h 1.15 2011/08/09 14:58:42EDT 044579 Development  $

#pragma once

#include "boost/serialization/access.hpp"

/// <summary>Holds 8 annual quarters of coverage amounts in one word (4
/// bytes). Each value is in 4 bits, and can take values from 0 to 4.</summary>
///
/// <remarks>This class is used in <see cref="QcArray"/> to save space.
/// </remarks>
///
/// <seealso cref="QcArray"/>
class QcPacked
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
  { ar & qc0_1 & qc2_3 & qc4_5 & qc6_7; }
public:
  /// <summary>Number of elements in this class.</summary>
  static const unsigned NUM_QCS = 8;
private:
  /// <summary>First and second quarter of coverage amounts.</summary>
  unsigned char qc0_1;
  /// <summary>Third and fourth quarter of coverage amounts.</summary>
  unsigned char qc2_3;
  /// <summary>Fifth and sixth quarter of coverage amounts.</summary>
  unsigned char qc4_5;
  /// <summary>Seventh and eighth quarter of coverage amounts.</summary>
  unsigned char qc6_7;
public:
  /// <summary>Initialize all values to 0.</summary>
  QcPacked() : qc0_1(0), qc2_3(0), qc4_5(0), qc6_7(0) { }
  unsigned get( int index ) const;
  void set( int index, unsigned value );
};
