// Declarations for the <see cref="LawChangeWAGEBASECHG"/> class to manage
// parameters for a proposal to change the OASDI and/or old-law wage bases.

// $Id: LawChangeWAGEBASECHG.h 1.7 2011/08/01 11:38:35EDT 044579 Development  $

#pragma once

#include "LawChange.h"
#include "dbleann.h"

/// <summary>Parameters for a proposal to change the OASDI and/or old-law
/// wage bases.</summary>
///
/// <remarks>There are two additional data fields compared to the base class.
///
/// If the indicator is greater than zero, it has the following meanings:
/// <table>
///   1 = ad hoc OASDI wage bases.
///   2 = ad hoc old-law wage bases.
///   3 = ad hoc OASDI and old-law wage bases.
/// </table></remarks>
class LawChangeWAGEBASECHG : public LawChange
{
private:
  /// <summary>Ad hoc OASDI wage bases.</summary>
  DoubleAnnual baseOasdi;
  /// <summary>Ad hoc 1977-law wage bases.</summary>
  DoubleAnnual base77;
public:
  LawChangeWAGEBASECHG();
  /// <summary>Returns ad hoc OASDI wage base for specified year.</summary>
  ///
  /// <returns>Ad hoc wage base for specified year.</returns>
  ///
  /// <param name="year">Year of wage base.</param>
  double getBaseOasdi( int year ) const { return baseOasdi[year]; }
  /// <summary>Returns ad hoc 1977-law wage base for specified year.</summary>
  ///
  /// <returns>Ad hoc 1977-law wage base for specified year.</returns>
  ///
  /// <param name="year">Year of wage base.</param>
  double getBase77( int year ) const { return base77[year]; }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets ad hoc OASDI wage base.</summary>
  ///
  /// <param name="year">Year for which to set base.</param>
  /// <param name="newBaseOasdi">New value of base.</param>
  void setBaseOasdi( int year, double newBaseOasdi )
  { baseOasdi[year] = newBaseOasdi; }
  /// <summary>Sets ad hoc 1977-law wage base.</summary>
  ///
  /// <param name="year">Year for which to set base.</param>
  /// <param name="newBase77">New value of base.</param>
  void setBase77( int year, double newBase77 ) { base77[year] = newBase77; }
  void write( std::ostream& out ) const;
};
