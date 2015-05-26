// Declarations for the <see cref="MilServDatesVec"/> class to manage an
// array of military service dates records.

// $Id: MilServDatesVec.h 1.16 2011/08/01 11:38:35EDT 044579 Development  $

#pragma once

#include "msdates.h"

/// <summary>Manages all the military service dates records for one ssn.
/// </summary>
///
/// <remarks>An instance of this class is created in <see cref="WorkerData"/>.
/// </remarks>
class MilServDatesVec
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
  { ar & msdates & msCount; }
public:
  /// <summary>The number of <see cref="MilServDates"/> records allowed.
  /// </summary>
  static const unsigned MAX_MILSERVDATES = 15;
  /// <summary>An array of <see cref="MilServDates"/> records.</summary>
  ///
  /// <remarks>There can be up to <see cref="MAX_MILSERVDATES"/> records.
  /// </remarks>
  MilServDates msdates[MAX_MILSERVDATES];
private:
  /// <summary>How many <see cref="MilServDates"/> records are being used.
  /// </summary>
  unsigned msCount;
public:
  MilServDatesVec();
  ~MilServDatesVec();
  void check() const;
  void deleteContents();
  double getEarn3750() const;
  double getEarn( unsigned year ) const;
  /// <summary>Returns first year with any military service, 0 if none.
  /// </summary>
  ///
  /// <returns>First year with any military service, 0 if none.</returns>
  int getFirstYear() const
  { return (msCount == 0) ? 0 : msdates[0].startDate.getYear(); }
  /// <summary>Returns last year with any military service, 0 if none.
  /// </summary>
  ///
  /// <returns>Last year with any military service, 0 if none.</returns>
  int getLastYear() const
  { return (msCount == 0) ? 0 : msdates[msCount - 1].endDate.getYear(); }
  /// <summary>Returns the number of records being used.</summary>
  ///
  /// <returns>The number of records being used.</returns>
  unsigned getMSCount() const { return(msCount); }
  unsigned getQc3750() const;
  unsigned getQcov( unsigned year ) const;
  void setMSCount( unsigned newMSCount );
  static void msCountCheck( unsigned newMSCount );
};
