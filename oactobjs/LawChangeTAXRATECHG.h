// Declarations for the <see cref="LawChangeTAXRATECHG"/> class to manage
// parameters for a proposal to change the OASDHI tax rates.

// $Id: LawChangeTAXRATECHG.h 1.10 2011/08/01 10:06:08EDT 044579 Development  $

#pragma once

#include "LawChange.h"
#include "TaxRates.h"

/// <summary>Parameters for a proposal to change the OASDI tax rates.</summary>
///
/// <remarks>There are five additional data fields compared to the base class.
/// The indicator can have the following values:
/// <table>
///   1   the changed tax rates continue to have the self-employed rate equal to twice the employee rate.
///   2   the changed tax rates include specified self-employed rates.
/// </table></remarks>
class LawChangeTAXRATECHG : public LawChange
{
public:
  /// <summary>Array sizes.</summary>
  enum DimLimits {
    MAXTAXRATES = 10 // Maximum number of new tax rate intervals.
  };
private:
  /// <summary>Number of tax rates available.</summary>
  int numTaxrates;
  /// <summary>Years changed tax rates are effective.</summary>
  int yearTaxrate[MAXTAXRATES];
  /// <summary>Di tax rates (employee-employer and self-employed, for up to
  /// <see cref="MAXTAXRATES"/> intervals).</summary>
  double diTaxrate[TaxRates::NUMTAXRATETYPES][MAXTAXRATES];
  /// <summary>Hi tax rates (employee-employer and self-employed, for up to
  /// <see cref="MAXTAXRATES"/> intervals).</summary>
  double hiTaxrate[TaxRates::NUMTAXRATETYPES][MAXTAXRATES];
  /// <summary>Oasi tax rates (employee-employer and self-employed, for up to
  /// <see cref="MAXTAXRATES"/> intervals).</summary>
  double oasiTaxrate[TaxRates::NUMTAXRATETYPES][MAXTAXRATES];
public:
  LawChangeTAXRATECHG();
  /// <summary>Returns number of tax rates available.</summary>
  ///
  /// <returns>Number of tax rates available.</returns>
  int getNumTaxrates() const { return numTaxrates; }
  double getTaxrate( int yearNum, TaxRates::taxrateType eeer,
    TaxRates::trustfundType trustfund ) const;
  /// <summary>Returns year that changed tax rate is effective.</summary>
  ///
  /// <returns>Year that changed tax rate is effective.</returns>
  ///
  /// <param name="yearNum">Number of tax rate interval (0 to
  /// <see cref="getNumTaxrates"/> - 1).</param>
  int getYearTaxrate( int yearNum ) const{ return yearTaxrate[yearNum]; }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets number of tax rates available.</summary>
  ///
  /// <param name="numRates">New number of tax rates.</param>
  void setNumTaxrates( int numRates ) { numTaxrates = numRates; }
  void setSelfEmpTaxrate( int yearNum );
  void setTaxrate( int yearNum, TaxRates::taxrateType eeer,
    TaxRates::trustfundType trustfund, double newRate );
  /// <summary>Sets year that changed tax rate is effective.</summary>
  ///
  /// <param name="yearNum">Number of tax rate interval (0 to
  /// <see cref="getNumTaxrates"/> - 1).</param>
  /// <param name="year">New year of changed tax rate.</param>
  void setYearTaxrate( int yearNum, int year )
  { yearTaxrate[yearNum] = year; }
  void write( std::ostream& out ) const;
};
