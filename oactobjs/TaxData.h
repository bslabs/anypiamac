// Declarations for the <see cref="TaxData"/> class to manage taxes paid by a
// worker.
//
// $Id: TaxData.h 1.12 2011/08/09 16:51:47EDT 044579 Development  $

#pragma once

#include <vector>
#include "dbleann.h"
#include "TaxRates.h"

/// <summary>Manages the Social Security payroll and self-employed taxes paid
/// by a worker.</summary>
///
/// <remarks>An instance of this class is used in the Anypia program.</remarks>
class TaxData
{
public:
  /// <summary>Dimension limits.</summary>
  enum YEAR_GROUP {
    YEARS1937_50 = 0,  // Years 1937 to 1950.
    YEARS1937PLUS,     // All years starting with 1937.
    NUMYEARPERIODS     // Number of year periods.
  };
  /// <summary>OASI taxes paid by year.</summary>
  DoubleAnnual taxesOasi;
  /// <summary>DI taxes paid by year.</summary>
  DoubleAnnual taxesDi;
  /// <summary>OASDI taxes paid by year.</summary>
  DoubleAnnual taxesOasdi;
  /// <summary>HI taxes paid by year.</summary>
  DoubleAnnual taxesHi;
  /// <summary>OASDHI taxes paid by year.</summary>
  DoubleAnnual taxesOasdhi;
private:
  /// <summary>Total taxes over all years in 2 time periods.</summary>
  ///
  /// <remarks>There are 5 types of taxes (see TaxRates::trustfundType) in 2
  /// time periods (see YEAR_GROUP).</remarks>
  std::vector< double > taxTotal[TaxRates::NUMTRUSTFUNDTYPES];
  /// <summary>Maximum year allowed.</summary>
  int maxyear;
public:
  TaxData( int lastYear );
  /// <summary>Destroys this instance.</summary>
  ~TaxData() { }
  /// <summary>Returns total taxes paid for a trust fund type and a period
  /// of years.</summary>
  ///
  /// <returns>Total taxes paid for a trust fund type and a period  of years.
  /// </returns>
  ///
  /// <param name="trustfund">Type of taxes.</param>
  /// <param name="yearGroup">Group of years to use (years 1937 to 1950 or all
  /// years starting with 1937).</param>
  double getTaxesTotal( TaxRates::trustfundType trustfund,
    YEAR_GROUP yearGroup ) const
  { return(taxTotal[trustfund][yearGroup]); }
  void oasdhiTaxCal();
  void totalTaxCal();
};
