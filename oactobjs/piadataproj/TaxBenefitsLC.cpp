// Functions for the <see cref="TaxBenefitsLC"/> class to manage tax on
// benefits functions with law changes.

// $Id: TaxBenefitsLC.cpp 1.19 2011/08/09 16:51:47EDT 044579 Development  $

#include <algorithm>
#include "TaxBenefitsLC.h"
#include "LawChangeArray.h"
#include "LawChangeTAXBENCHG.h"

using namespace std;

/// <summary>Initializes an instance.</summary>
///
/// <param name="newLawChange">Law change data.</param>
TaxBenefitsLC::TaxBenefitsLC( const LawChangeArray& newLawChange ) :
TaxBenefits(), lawChange(newLawChange)
{
  thresholdsCal();
}

/// <summary>Destructor.</summary>
TaxBenefitsLC::~TaxBenefitsLC()
{ }

/// <summary>Returns taxable benefits for OASDHI trust funds (same as
/// taxable for OASDI for proposal to tax as for private pension plans).
/// </summary>
///
/// <returns>Taxable benefits for OASDHI trust funds.</returns>
///
/// <param name="year">Year for which benefits are being taxed.</param>
/// <param name="magi">Modified adjusted gross income (excluding benefits).
/// </param>
/// <param name="benefits">Social Security benefits (including spouse's
///  benefits, if married, reduced by deduction for contributions).</param>
/// <param name="married">True if married filing jointly.</param>
double TaxBenefitsLC::taxOasdhi( int year, double magi, double benefits,
bool married ) const
{
  LawChangeTAXBENCHG *lawChangeTAXBENCHG = lawChange.lawChangeTAXBENCHG;
  // assume effective for all
  if (lawChangeTAXBENCHG->isEffective(year, year)) {
    return(TaxBenefits::taxOasdi(year, magi, benefits, married));
  }
  else {
    return(TaxBenefits::taxOasdhi(year, magi, benefits, married));
  }
}

/// <summary>Returns the amount of deductions allowed under the Ball
/// taxation plan.</summary>
///
/// <returns>Amount of deductions allowed under the Ball taxation plan.
/// </returns>
///
/// <param name="startAge">Starting age of deductions.</param>
/// <param name="aggContribs">Aggregate contribution (no interest) over
///  person's lifetime into OASDI trust funds through taxes.</param>
/// <param name="taxRecovered">Tax recovered through deductions.</param>
/// <param name="year">Year for which benefits are being taxed.</param>
double TaxBenefitsLC::deductionsCal( unsigned startAge, double aggContribs,
double taxRecovered, int year ) const
{
  LawChangeTAXBENCHG *lawChangeTAXBENCHG = lawChange.lawChangeTAXBENCHG;
  if (lawChangeTAXBENCHG->isEffective(year, year) &&
    (lawChangeTAXBENCHG->getInd() == 2 ||
    lawChangeTAXBENCHG->getInd() == 3)) {
    double deduction = 0.0;
    // Figure out deduction
    if (taxRecovered < aggContribs) {
      const double payments = static_cast<double>(paymentsCal(startAge));
      deduction = (aggContribs / payments) * 12.0;
      // limit deduction to amount of outstanding contributions
      deduction = min(deduction, aggContribs - taxRecovered);
    }
    return deduction;
  }
  else {
    return TaxBenefits::deductionsCal(startAge, aggContribs, taxRecovered,
      year);
  }
}

/// <summary>Returns number of payments over which contributions are spread
/// for Ball taxation plan.</summary>
///
/// <returns>Number of payments over which contributions are spread for Ball
/// taxation plan.</returns>
///
/// <param name="age">The age of taxpayer.</param>
int TaxBenefitsLC::paymentsCal( unsigned age )
{
  if (age <= 55) {
    return 360;
  }
  else if (age <= 60) {
    return 310;
  }
  else if (age <= 65) {
    return 260;
  }
  else if (age <= 70) {
    return 210;
  }
  else {
    return 160;
  }
}

/// <summary>Fills in declining threshold amounts.</summary>
void TaxBenefitsLC::thresholdsCal()
{
  thresholds[0][0] = SINGLEOASDITHRESHOLD;
  thresholds[1][0] = MARRIEDOASDITHRESHOLD;
  for (int j = 0; j < 2; j++) {
    for (int i = 1; i < 11; i++) {
      thresholds[j][i] =
        (10.0 - static_cast<double>(i)) * thresholds[j][0] / 10.0;
    }
  }
}

/// <summary>Returns appropriate threshold.</summary>
///
/// <returns>Appropriate threshold.</returns>
///
/// <param name="married">True if married filing jointly.</param>
/// <param name="year">Year for which benefits are being taxed.</param>
double TaxBenefitsLC::getThreshold( bool married, int year ) const
{
  LawChangeTAXBENCHG *lawChangeTAXBENCHG = lawChange.lawChangeTAXBENCHG;
  // assume effective for all
  if (lawChangeTAXBENCHG->isEffective(year, year) &&
    (lawChangeTAXBENCHG->getInd() == 1 ||
    lawChangeTAXBENCHG->getInd() == 3)) {
    const int index = min(max(
      year - lawChangeTAXBENCHG->getStartYear() + 1, 0), 10);
    return thresholds[married ? 1 : 0][index];
  }
  else {
    return TaxBenefits::getThreshold(married, year);
  }
}

/// <summary>Returns proportion of benefits that are taxable for OASDI.
/// </summary>
///
/// <returns>Proportion of benefits that are taxable for OASDI.</returns>
///
/// <param name="year">Year for which benefits are being taxed.</param>
double TaxBenefitsLC::getProportion( int year ) const
{
  LawChangeTAXBENCHG *lawChangeTAXBENCHG = lawChange.lawChangeTAXBENCHG;
  // assume effective for all
  if (lawChangeTAXBENCHG->isEffective(year, year) &&
    (lawChangeTAXBENCHG->getInd() == 2 ||
    lawChangeTAXBENCHG->getInd() == 3)) {
    return 1.0;
  }
  else {
    return TaxBenefits::getProportion(year);
  }
}
