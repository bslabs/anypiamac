// Functions for the <see cref="TaxData"/> class to manage taxes paid by a
// worker.
//
// $Id: TaxData.cpp 1.9 2011/08/09 16:51:48EDT 044579 Development  $

#include "TaxData.h"
#include "wrkrdata.h"
#include "oactcnst.h"

using namespace std;

/// <summary>Initializes arrays.</summary>
///
/// <param name="lastYear">Maximum projected year.</param>
TaxData::TaxData( int lastYear ) : taxesOasi(YEAR37, lastYear),
taxesDi(YEAR37, lastYear), taxesOasdi(YEAR37, lastYear),
taxesHi(YEAR37, lastYear), taxesOasdhi(YEAR37, lastYear), maxyear(lastYear)
{
  // allocate memory for total tax array
  for (int i = 0; i < TaxRates::NUMTRUSTFUNDTYPES; i++) {
    taxTotal[i].resize(NUMYEARPERIODS);
  }
}

/// <summary>Calculates total taxes paid over a lifetime.</summary>
///
/// <remarks>Calculates total taxes paid over a lifetime. Assumes that OASDI
/// and OASDHI taxes have already been calculated via a call to
/// <see cref="TaxData::oasdhiTaxCal"/>.</remarks>
void TaxData::totalTaxCal()
{
  // find total OASI taxes
  taxTotal[TaxRates::OASITF][YEARS1937_50] =
    taxesOasi.accumulate(YEAR37, YEAR50, 0.0);
  taxTotal[TaxRates::OASITF][YEARS1937PLUS] = taxesOasi.accumulate(YEAR51,
    maxyear, taxTotal[TaxRates::OASITF][YEARS1937_50]);
  // find total DI taxes
  taxTotal[TaxRates::DITF][YEARS1937_50] =
    taxesDi.accumulate(YEAR37, YEAR50, 0.0);
  taxTotal[TaxRates::DITF][YEARS1937PLUS] = taxesDi.accumulate(YEAR51,
    maxyear, taxTotal[TaxRates::DITF][YEARS1937_50]);
  // find total OASDI taxes
  taxTotal[TaxRates::OASDITF][YEARS1937_50] =
    taxesOasdi.accumulate(YEAR37, YEAR50, 0.0);
  taxTotal[TaxRates::OASDITF][YEARS1937PLUS] = taxesOasdi.accumulate(YEAR51,
    maxyear, taxTotal[TaxRates::OASDITF][YEARS1937_50]);
  // find total HI taxes
  taxTotal[TaxRates::HITF][YEARS1937_50] =
    taxesHi.accumulate(YEAR37, YEAR50, 0.0);
  taxTotal[TaxRates::HITF][YEARS1937PLUS] = taxesHi.accumulate(YEAR51,
    maxyear, taxTotal[TaxRates::HITF][YEARS1937_50]);
  // find total OASDHI taxes
  taxTotal[TaxRates::OASDHITF][YEARS1937_50] =
    taxesOasdhi.accumulate(YEAR37, YEAR50, 0.0);
  taxTotal[TaxRates::OASDHITF][YEARS1937PLUS] = taxesOasdhi.accumulate(
    YEAR51, maxyear, taxTotal[TaxRates::OASDHITF][YEARS1937_50]);
}

/// <summary>Calculates OASDI and OASDHI taxes paid over a lifetime.</summary>
///
/// <remarks>Calculates OASDI and OASDHI taxes paid over a lifetime. Assumes
/// that OASI, DI, and HI taxes have already been calculated.</remarks>
void TaxData::oasdhiTaxCal()
{
  // find OASDI and OASDHI taxes
  for (int yr = YEAR37; yr <= maxyear; yr++) {
    taxesOasdi[yr] = taxesOasi[yr] + taxesDi[yr];
    taxesOasdhi[yr] = taxesOasdi[yr] + taxesHi[yr];
  }
}
