// Declaration of the <see cref="AwbiData"/> class to manage historical Social
// Security parameters.
//
// $Id: awbidat.h 1.18 2011/07/28 10:16:26EDT 044579 Development  $

#pragma once

#include "dbleann.h"

/// <summary>Manages the historical Social Security parameters.</summary>
///
/// <remarks>Manages the historical Social Security parameters:
/// historical benefit increases, average wages, OASDI wage bases, old-law
/// wage bases, and HI wage bases. This class cannot be instantiated; use one
/// of its subclasses: <see cref="AwbiDataFile"/> or
/// <see cref="AwbiDataNonFile"/>.</remarks>
class AwbiData
{
public:
  /// <summary>Year when HI wage base first differs from OASDI base.</summary>
  static const int HI_BASE_YEAR = 1991;
  /// <summary>Historical benefit increases.</summary>
  DoubleAnnual cpiinc;
  /// <summary>Historical average wages.</summary>
  DoubleAnnual fq;
  /// <summary>Historical OASDI wage bases.</summary>
  DoubleAnnual baseOasdi;
  /// <summary>Historical old-law wage bases.</summary>
  DoubleAnnual base77;
  /// <summary>Historical HI wage bases.</summary>
  DoubleAnnual baseHi;
private:
  /// <summary>Last year of historical wage bases to read or write.</summary>
  int lastYear;
  /// <summary>Maximum projected year.</summary>
  int maxyear;
public:
  AwbiData( int newLastYear, int newMaxyear );
  explicit AwbiData( int newMaxyear );
  virtual ~AwbiData();
  /// <summary>Gets last year of historical wage bases.</summary>
  ///
  /// <returns>Last year of historical wage bases.</returns>
  int getLastYear() const { return(lastYear); }
  virtual void read();
  /// <summary>Set data values.</summary>
  virtual void setData() = 0;
  /// <summary>Sets last year of historical wage bases.</summary>
  ///
  /// <param name="newLastYear">Last year of historical wage bases.</param>
  void setLastYear ( int newLastYear ) { lastYear = newLastYear; }
  virtual void write();
};
