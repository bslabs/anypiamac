// Functions for the <see cref="TaxBenefits"/> class to manage tax on
// benefits functions.

// $Id: TaxBenefits.cpp 1.12 2011/08/09 16:51:47EDT 044579 Development  $

#include <algorithm>
#include "TaxBenefits.h"

using namespace std;

const double TaxBenefits::SINGLEOASDITHRESHOLD = 25000.0;
const double TaxBenefits::MARRIEDOASDITHRESHOLD = 32000.0;
const double TaxBenefits::SINGLEOASDHITHRESHOLD = 34000.0;
const double TaxBenefits::MARRIEDOASDHITHRESHOLD = 44000.0;
const double TaxBenefits::SINGLEOASDHITAX = 0.5 *
  (TaxBenefits::SINGLEOASDHITHRESHOLD - TaxBenefits::SINGLEOASDITHRESHOLD);
const double TaxBenefits::MARRIEDOASDHITAX = 0.5 *
  (TaxBenefits::MARRIEDOASDHITHRESHOLD - TaxBenefits::MARRIEDOASDITHRESHOLD);
const double TaxBenefits::OasdiRate = 0.5;
const double TaxBenefits::OasdhiRate = 0.85;

/// <summary>Initializes an instance.</summary>
TaxBenefits::TaxBenefits()
{ }

/// <summary>Destructor.</summary>
TaxBenefits::~TaxBenefits()
{ }

/// <summary>Returns taxable benefits for OASDI trust funds.</summary>
///
/// <returns>Taxable benefits for OASDI trust funds.</returns>
///
/// <param name="year">Year for which benefits are being taxed.</param>
/// <param name="magi">Modified adjusted gross income (excluding benefits).
/// </param>
/// <param name="benefits">Social Security benefits (including spouse's
///  benefits, if married). Should be reduced by deduction for
///  contributions, if applicable.</param>
/// <param name="married">True if married filing jointly.</param>
double TaxBenefits::taxOasdi( int year, double magi, double benefits,
bool married ) const
{
  if (benefits <= 0.0) {
    return(0.0);
  }
  const double threshold = getThreshold(married, year);
  const double magiPlusBenefits = magi + getProportion(year) * benefits;
  if (magiPlusBenefits > threshold) {
    return min(OasdiRate * (magiPlusBenefits - threshold),
      OasdiRate * benefits);
  }
  else {
    return(0.0);
  }
}

/// <summary>Returns taxable benefits for OASDHI trust funds.</summary>
///
/// <returns>Taxable benefits for OASDHI trust funds.</returns>
///
/// <param name="year">Year for which benefits are being taxed.</param>
/// <param name="magi">Modified adjusted gross income (excluding benefits).
/// </param>
/// <param name="benefits">Social Security benefits (including spouse's
/// benefits, if married, reduced by deduction for contributions).</param>
/// <param name="married">True if married filing jointly.</param>
double TaxBenefits::taxOasdhi( int year, double magi, double benefits,
bool married ) const
{
  if (year < HiYear)
    return taxOasdi(year, magi, benefits, married);
  if (benefits <= 0.0)
    return(0.0);
  const double threshold =
    married ? MARRIEDOASDHITHRESHOLD : SINGLEOASDHITHRESHOLD;
  const double magiPlusHalf = magi + getProportion(year) * benefits;
  if (magiPlusHalf < threshold)
    return taxOasdi(year, magi, benefits, married);
  const double part1 = married ? MARRIEDOASDHITAX : SINGLEOASDHITAX;
  const double part2 = min(part1, OasdiRate * benefits);
  const double part3 = OasdhiRate * (magiPlusHalf - threshold);
  return min(OasdhiRate * benefits, part2 + part3);
}

/// <summary>Returns appropriate OASDI threshold.</summary>
///
/// <returns>Appropriate OASDI threshold.</returns>
///
/// <param name="married">True if married filing jointly.</param>
double TaxBenefits::getThreshold( bool married, int ) const
{
  return(married ? MARRIEDOASDITHRESHOLD : SINGLEOASDITHRESHOLD);
}

/// <summary>Returns proportion of benefits that are taxable for OASDI.
/// </summary>
///
/// <returns>0.5 for present law.</returns>
double TaxBenefits::getProportion( int ) const
{
  return 0.5;
}

/// <summary>Returns the amount of deductions allowed under the
///   Ball taxation plan.</summary>
///
/// <returns>Zero for present law.</returns>
double TaxBenefits::deductionsCal( unsigned , double , double , int ) const
{
  return 0.0;
}
