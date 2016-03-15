// Declarations for the <see cref="TaxRates"/> class - parent of classes to
// manage OASDHI tax rates.

// $Id: TaxRates.h 1.18 2016/03/09 07:53:44EST 277133 Development  $

#pragma once

#include "BitAnnual.h"
#include "dbleann.h"
#include "oactcnst.h"

/// <summary>Abstract parent of classes that manage the Social Security
/// payroll and self-employed tax rates.</summary>
///
/// <remarks>The child classes are <see cref="TaxRatesLC"/> and
/// <see cref="TaxRatesPL"/>.</remarks>
///
/// <seealso cref="TaxRatesLC"/>
/// <seealso cref="TaxRatesPL"/>
class TaxRates
{
public:
  /// <summary>Type of tax rate (employee or self-employed).</summary>
  enum taxrateType {
    EMPLOYEERATE,    // Employee rate.
    SELFEMPRATE,     // Self-employed rate.
    NUMTAXRATETYPES  // Number of tax rate types.
  };
  /// <summary>Trust fund indicator.</summary>
  enum trustfundType {
    OASITF,  // Oasi trust fund.
    DITF,  // Di trust fund.
    OASDITF,  // Oasdi trust fund.
    HITF,  // Hi trust fund.
    OASDHITF,  // Oasdhi trust fund.
    NUMTRUSTFUNDTYPES  // Number of trust fund types.
  };
  /// <summary>Last year that tax rates change.</summary>
  static const int LAST_TAX_RATE_YEAR = 2019;
  /// <summary>Number of years of historical tax rates.</summary>
  static const int NUMTAXRATES = LAST_TAX_RATE_YEAR - YEAR37 + 1;
private:
  /// <summary>Historical OASI employee/employer net rates, 1937-2013.
  /// </summary>
  static const double oasieeerrate[NUMTAXRATES];
  /// <summary>Historical DI employee/employer net rates, 1937-2013.</summary>
  static const double dieeerrate[NUMTAXRATES];
  /// <summary>Historical HI employee/employer net rates, 1937-2013.</summary>
  static const double hieeerrate[NUMTAXRATES];
  /// <summary>Historical OASI self-employed net rates, 1937-2013.</summary>
  static const double oasiserate[NUMTAXRATES];
  /// <summary>Historical DI self-employed net rates, 1937-2013.</summary>
  static const double diserate[NUMTAXRATES];
  /// <summary>Historical HI self-employed net rates, 1937-2013.</summary>
  static const double hiserate[NUMTAXRATES];
  /// <summary>Historical OASI employee/employer credit, 1937-2013.</summary>
  static const double oasieeercredit[NUMTAXRATES];
  /// <summary>Historical DI employee/employer credit, 1937-2013.</summary>
  static const double dieeercredit[NUMTAXRATES];
  /// <summary>Historical HI employee/employer credit, 1937-2013.</summary>
  static const double hieeercredit[NUMTAXRATES];
  /// <summary>Historical OASI self-employed credits, 1937-2013.</summary>
  static const double oasisecredit[NUMTAXRATES];
  /// <summary>Historical DI self-employed credits, 1937-2013.</summary>
  static const double disecredit[NUMTAXRATES];
  /// <summary>Historical HI self-employed credits, 1937-2013.</summary>
  static const double hisecredit[NUMTAXRATES];
  /// <summary>Array of net tax rates.</summary>
  ///
  /// <remarks>Tax rates are by tax rate type (see <see cref="taxrateType"/>)
  /// and trust fund (see <see cref="trustfundType"/>).</remarks>
  DoubleAnnual* taxrate[NUMTAXRATETYPES][NUMTRUSTFUNDTYPES];
  /// <summary>Array of tax credits.</summary>
  ///
  /// <remarks>Tax credits are by credit type (see <see cref="taxrateType"/>)
  /// and trust fund (see <see cref="trustfundType"/>).</remarks>
  DoubleAnnual* taxcredit[NUMTAXRATETYPES][NUMTRUSTFUNDTYPES];
  /// <summary>Array of gross tax rates.</summary>
  ///
  /// <remarks>Tax rates are by tax rate type (see <see cref="taxrateType"/>)
  /// and trust fund (see <see cref="trustfundType"/>).</remarks>
  DoubleAnnual* taxrategross[NUMTAXRATETYPES][NUMTRUSTFUNDTYPES];
protected:
  /// <summary>Maximum year allowed.</summary>
  int maxyear;
  void setCredits( int year, taxrateType taxType, double oasiCredit,
    double diCredit, double hiCredit );
  void setGrossRates();
  void setNetRates( int year, taxrateType taxType, double oasiRate,
    double diRate, double hiRate );
public:
  TaxRates( int lastYear );
  virtual ~TaxRates();
  /// <summary>Returns an array of gross tax rates.</summary>
  ///
  /// <returns>An array of gross tax rates.</returns>
  ///
  /// <param name="empeeEmper">Specifies the FICA or SECA rate.</param>
  /// <param name="trustfund">Type of taxes.</param>
  const DoubleAnnual& getTaxRatesGross( taxrateType empeeEmper,
    trustfundType trustfund ) const
    { return(*taxrategross[empeeEmper][trustfund]); }
  /// <summary>Returns an array of tax rates, net of any tax credit.</summary>
  ///
  /// <returns>An array of net tax rates.</returns>
  ///
  /// <param name="empeeEmper">Specifies the FICA or SECA rate.</param>
  /// <param name="trustfund">Type of taxes.</param>
  const DoubleAnnual& getTaxRatesNet( taxrateType empeeEmper,
    trustfundType trustfund ) const
    { return(*taxrate[empeeEmper][trustfund]); }
  /// <summary>Initializes all tax rates.</summary>
  virtual void initialize() = 0;
  void setHistCredits( int lastYear );
  void setHistRates( int lastYear );
  void taxCalAllYears( const DoubleAnnual& earnOasdi,
    const BitAnnual& taxType, DoubleAnnual& taxes,
    trustfundType fundType ) const;
  double taxCalOneYearNet( int year, double earnings, taxrateType taxType,
    trustfundType fundType ) const;
  double taxCalOneYearGross( int year, double earnings, taxrateType taxType,
    trustfundType fundType ) const;
};
