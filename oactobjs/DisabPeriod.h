// Declarations for the <see cref="DisabPeriod"/> class to manage a period
// of disability.

// $Id: DisabPeriod.h 1.12 2011/07/29 15:26:17EDT 044579 Development  $

#pragma once

#include <string>
#include "datemoyr.h"
#include "boost/serialization/access.hpp"
#include "boost/date_time/gregorian/greg_date.hpp"

/// <summary>Manages a period of disability.</summary>
///
/// <remarks>It has slots for the date of onset, first month of the waiting
/// period, date of entitlement, date of cessation, and PIA and MFB as of the
/// month before cessation. (The month of cessation is the month after the
/// last month for which benefits were received.) If the disability is still
/// open, as in a disability case, the date of cessation, and PIA and MFB just
/// before cessation, are zero.
///
/// An array of two periods of disability are created in
/// <see cref="WorkerDataGeneral"/>. The one at index 0 is the most recent,
/// while the one at index 1 is the next most recent. The values in those
/// structures are ignored if the indicator for the number of periods of
/// disability is zero. In a disability case, the most recent period of
/// disability is the current one.</remarks>
class DisabPeriod
{
  /// <summary>Friend class to allow serialization.</summary>
  friend class boost::serialization::access;
  // <summary>Archives the data.</summary>
  //
  // <remarks>When the class Archive corresponds to an output archive, the
  // & operator is defined similar to <<. Likewise, when the class Archive is
  // a type of input archive, the & operator is defined
  // similar to >>.</remarks>
  //
  // <param name="ar">The archive to use.</param>
  // <param name="version">The version of the archive to use.</param>
  template< class Archive >
  void serialize( Archive& ar, const unsigned int version )
  { ar & onsetDate & cessationDate & entDate & waitperDate;
    ar & cessationPia & cessationMfb; }
private:
  /// <summary>Month, day, and year of disability onset.</summary>
  boost::gregorian::date onsetDate;
  /// <summary>Month and year of disability cessation.</summary>
  ///
  /// <remarks>This will be 0 for a current period of disability.</remarks>
  DateMoyr cessationDate;
  /// <summary>Month and year of DIB entitlement.</summary>
  DateMoyr entDate;
  /// <summary>Month and year of beginning of waiting period.</summary>
  DateMoyr waitperDate;
  /// <summary>The PIA just before cessation.</summary>
  ///
  /// <remarks>This will be 0 for a current period of disability.</remarks>
  float cessationPia;
  /// <summary>The MFB just before cessation.</summary>
  ///
  /// <remarks>This will be 0 for a current period of disability.</remarks>
  float cessationMfb;
public:
  DisabPeriod();
  /// <summary>Deletes the cessation date.</summary>
  ///
  /// <remarks>Also deletes the pia and mfb at cessation.</remarks>
  void deleteCessationDate()
  { cessationDate.deleteContents(); cessationPia = cessationMfb = 0.0f; }
  void deleteContents();
  /// <summary>Deletes the entitlement date.</summary>
  void deleteEntDate() { entDate.deleteContents(); }
  /// <summary>Deletes the onset date.</summary>
  void deleteOnsetDate()
  { onsetDate = boost::gregorian::date(boost::date_time::not_a_date_time); }
  /// <summary>Deletes the first month of the waiting period.</summary>
  void deleteWaitperDate() { waitperDate.deleteContents(); }
  /// <summary>Returns date of disability cessation.</summary>
  ///
  /// <returns>The date of disability cessation.</returns>
  const DateMoyr& getCessationDate() const { return(cessationDate); }
  /// <summary>Returns the mfb in the month before cessation.</summary>
  ///
  /// <returns>The mfb at cessation.</returns>
  float getCessationMfb() const { return(cessationMfb); }
  /// <summary>Returns the pia in the month before cessation.</summary>
  ///
  /// <returns>The pia at cessation.</returns>
  float getCessationPia() const { return(cessationPia); }
  /// <summary>Returns date of entitlement to disability.</summary>
  ///
  /// <returns>The date of entitlement to disability.</returns>
  const DateMoyr& getEntDate() const { return(entDate); }
  /// <summary>Returns date of disability onset.</summary>
  ///
  /// <returns>The date of disability onset.</returns>
  const boost::gregorian::date& getOnsetDate() const { return(onsetDate); }
  /// <summary>Returns first month of waiting period.</summary>
  ///
  /// <returns>First month of waiting period.</returns>
  const DateMoyr& getWaitperDate() const { return(waitperDate); }
  void parseString( const std::string& line );
  /// <summary>Sets the cessation date.</summary>
  ///
  /// <param name="dateMoyr">The new cessation date.</param>
  void setCessationDate( const DateMoyr& dateMoyr )
  { cessationDate = dateMoyr; }
  /// <summary>Sets the mfb at cessation.</summary>
  ///
  /// <param name="newMfb">The new mfb at cessation.</param>
  void setCessationMfb( float newMfb ) { cessationMfb = newMfb; }
  /// <summary>Sets the pia at cessation.</summary>
  ///
  /// <param name="newPia">The new pia at cessation.</param>
  void setCessationPia( float newPia ) { cessationPia = newPia; }
  /// <summary>Sets the entitlement date.</summary>
  ///
  /// <param name="dateMoyr">The new entitlement date.</param>
  void setEntDate( const DateMoyr& dateMoyr ) { entDate = dateMoyr; }
  /// <summary>Sets the onset date.</summary>
  ///
  /// <param name="dateModyyr">The new onset date.</param>
  void setOnsetDate( const boost::gregorian::date& dateModyyr )
  { onsetDate = dateModyyr; }
  /// <summary>Sets the first month of the waiting period.</summary>
  ///
  /// <param name="dateMoyr">The new first month of the waiting period.</param>
  void setWaitperDate( const DateMoyr& dateMoyr ) { waitperDate = dateMoyr; }
  std::string toString() const;
  std::string toString( const std::string& format ) const;
  DateMoyr waitperDateCal() const;
  static void cessationDateCheck( const DateMoyr& dateMoyr );
  static void entDateCheck( const DateMoyr& dateMoyr );
  static void onsetDateCheck( const boost::gregorian::date& dateModyyr );
  static void waitperDateCheck( const DateMoyr& dateMoyr );
};
