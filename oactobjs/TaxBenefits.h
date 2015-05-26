// Declarations for the <see cref="TaxBenefits"/> class to manage tax on
// benefits functions.

// $Id: TaxBenefits.h 1.12 2011/08/09 16:51:47EDT 044579 Development  $

#pragma once

/// <summary>Contains the functions required to calculate the tax on a Social
/// Security benefit.</summary>
///
/// <remarks>Contains the functions required to calculate the tax on a Social
/// Security benefit.</remarks>
class TaxBenefits
{
public:
  /// <summary>Starting year of tax on Social Security benefits.</summary>
  static const int START_YEAR = 1984;
  /// <summary>OASDI threshold for single taxpayers.</summary>
  static const double SINGLEOASDITHRESHOLD;
  /// <summary>OASDI threshold for married taxpayers.</summary>
  static const double MARRIEDOASDITHRESHOLD;
  /// <summary>OASDHI threshold for single taxpayers.</summary>
  static const double SINGLEOASDHITHRESHOLD;
  /// <summary>OASDHI threshold for married taxpayers.</summary>
  static const double MARRIEDOASDHITHRESHOLD;
  /// <summary>OASDHI tax for single taxpayers in first tier.</summary>
  static const double SINGLEOASDHITAX;
  /// <summary>OASDHI tax for married taxpayers in first tier.</summary>
  static const double MARRIEDOASDHITAX;
  /// <summary>The OASDI tax rate.</summary>
  static const double OasdiRate;
  /// <summary>The OASDHI tax rate.</summary>
  static const double OasdhiRate;
  /// <summary>The year that the OASDHI tax became effective.</summary>
  static const int HiYear = 1994;
public:
  TaxBenefits();
  virtual ~TaxBenefits();
  virtual double deductionsCal( unsigned, double, double, int ) const;
  virtual double getProportion( int ) const;
  virtual double getThreshold( bool married, int ) const;
  double taxOasdi( int year, double magi, double benefits,
    bool married ) const;
  virtual double taxOasdhi( int year, double magi, double benefits,
    bool married ) const;
};
