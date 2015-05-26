// Declarations for the <see cref="LawChangeNEWSPECMIN"/> class to manage
// parameters for a proposal to change the special minimum pia calculation.
//
// $Id: LawChangeNEWSPECMIN.h 1.10 2011/08/01 10:06:08EDT 044579 Development  $

#pragma once

#include "LawChange.h"

/// <summary>Parameters for a proposal to change the special minimum pia
/// calculation.</summary>
///
/// <remarks>There are three additional data field compared to the base class,
/// for changes to the dollar amount (see <see cref="getAmount"/>), the
/// percentage of the old-law wage base required for a year of coverage (see
/// <see cref="getPercent"/>), and the maximum number of years considered (see
/// <see cref="getYears"/>).
///
/// If the indicator is greater than zero, it has the following meanings:
/// <table>
///   1   new dollar amount.
///   2   new maximum years of coverage.
///   3   new dollar amount and maximum years of coverage.
///   4   new year of coverage percent.
///   5   new year of coverage percent and dollar amount.
///   6   new year of coverage percent and maximum years of coverage.
///   7   new year of coverage percent, dollar amount, and maximum years of coverage.
/// </table></remarks>
class LawChangeNEWSPECMIN : public LawChange
{
private:
  /// <summary>Alternative special minimum dollar amount.</summary>
  double theAmt;
  /// <summary>Alternative percent of old-law wage base required for a year
  /// of coverage in special minimum method.</summary>
  double percent;
  /// <summary>Alternative maximum number of years of coverage considered
  /// in special minimum calculation (30 in present law).</summary>
  int maxYearsOfCoverage;
public:
  LawChangeNEWSPECMIN();
  /// <summary>Returns alternative special minimum dollar amount.</summary>
  ///
  /// <returns>Alternative special minimum dollar amount.</returns>
  double getAmount() const { return(theAmt); }
  /// <summary>Returns alternate special minimum percentage of wage base
  /// required for quarter of coverage.</summary>
  ///
  /// <returns>Alternate special minimum percentage of wage base
  /// required for quarter of coverage.</returns>
  double getPercent() const { return percent; }
  /// <summary>Returns alternative maximum number of years of coverage
  /// considered in special minimum calculation (30 in PL).</summary>
  ///
  /// <returns>Alternative maximum number of years of coverage
  /// considered in special minimum calculation (30 in PL).</returns>
  int getYears() const { return(maxYearsOfCoverage); }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets alternative special minimum dollar amount.</summary>
  ///
  /// <param name="newAmount">New alternative special minimum dollar
  /// amount.</param>
  void setAmount( double newAmount ) { theAmt = newAmount; }
  /// <summary>Sets alternate special minimum percentage of wage base
  /// required for quarter of coverage.</summary>
  ///
  /// <param name="newPercent">New alternative special minimum percentage of
  /// wage base.</param>
  void setPercent( double newPercent ) { percent = newPercent; }
  /// <summary>Sets alternative maximum number of years of coverage
  /// considered in special minimum calculation (30 in PL).</summary>
  ///
  /// <param name="newYears">New alternative special minimum maximum years of
  /// coverage.</param>
  void setYears( int newYears ) { maxYearsOfCoverage = newYears; }
  void write( std::ostream& out ) const;
};
