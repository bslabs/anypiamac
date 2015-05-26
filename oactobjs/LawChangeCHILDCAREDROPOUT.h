// Declarations for the <see cref="LawChangeCHILDCAREDROPOUT"/> class to
// manage parameters for a proposal to provide child care dropout years.
//
// $Id: LawChangeCHILDCAREDROPOUT.h 1.9 2011/07/29 16:27:31EDT 044579 Development  $

#pragma once

#include "LawChange.h"

/// <summary>Parameters for a proposal to provide child care dropout years.
/// </summary>
///
/// <remarks>There are three additional data fields compared to the base
/// class.</remarks>
class LawChangeCHILDCAREDROPOUT : public LawChange
{
private:
  /// <summary>Ratio of average earnings limit for a child care dropout year.
  /// </summary>
  double fqRatio;
  /// <summary>Maximum age of child to get a child care dropout year.
  /// </summary>
  int maxAge;
  /// <summary>Maximum number of child care dropout years.</summary>
  int maxYears;
public:
  LawChangeCHILDCAREDROPOUT();
  /// <summary>Returns ratio of average earnings limit for a child care
  /// dropout year.</summary>
  ///
  /// <returns>Ratio of average earnings limit for a child care dropout
  /// year.</returns>
  double getFqRatio() const { return fqRatio; }
  /// <summary>Returns maximum age of child to get a child care dropout
  /// year.</summary>
  ///
  /// <returns>Maximum age of child to get a child care dropout year.</returns>
  int getMaxAge() const { return maxAge; }
  /// <summary>Returns maximum number of child care dropout years.</summary>
  ///
  /// <returns>Maximum number of child care dropout years.</returns>
  int getMaxYears() const { return maxYears; }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets ratio of average earnings limit for a child care
  /// dropout year.</summary>
  ///
  /// <param name="newFqRatio">New ratio of average earnings limit for a
  /// child care dropout year.</param>
  void setFqRatio( double newFqRatio ) { fqRatio = newFqRatio; }
  /// <summary>Sets maximum age of child to get a child care dropout year.
  /// </summary>
  ///
  /// <param name="newMaxAge">New maximum age of child.</param>
  void setMaxAge( int newMaxAge ) { maxAge = newMaxAge; }
  /// <summary>Sets maximum number of child care dropout years.</summary>
  ///
  /// <param name="newMaxYears">New maximum number of child care dropout
  /// years.</param>
  void setMaxYears( int newMaxYears ) { maxYears = newMaxYears; }
  void write( std::ostream& out ) const;
};
