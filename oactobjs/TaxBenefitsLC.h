// Declarations for the <see cref="TaxBenefitsLC"/> class to manage tax on
// benefits functions with law changes.

// $Id: TaxBenefitsLC.h 1.12 2011/08/09 16:51:48EDT 044579 Development  $

#pragma once

#include "TaxBenefits.h"
class LawChangeArray;

/// <summary>Contains the functions required to calculate the tax on a Social
/// Security benefit with law changes.</summary>
///
/// <remarks>Contains the functions required to calculate the tax on a Social
/// Security benefit with law changes.</remarks>
class TaxBenefitsLC : public TaxBenefits
{
private:
  /// <summary>Law-change parameters.</summary>
  const LawChangeArray& lawChange;
  /// <summary>Declining thresholds.</summary>
  double thresholds[2][11];
public:
  TaxBenefitsLC( const LawChangeArray& newLawChange );
  ~TaxBenefitsLC();
  double deductionsCal( unsigned startAge, double aggContribs,
    double taxRecovered, int year ) const;
  virtual double getProportion( int year ) const;
  double getThreshold( bool married, int year ) const;
  static int paymentsCal( unsigned age );
  double taxOasdhi( int year, double magi, double benefits,
    bool married ) const;
  void thresholdsCal();
private:
  TaxBenefitsLC operator=( TaxBenefitsLC newTaxBenefitsLC );
};
