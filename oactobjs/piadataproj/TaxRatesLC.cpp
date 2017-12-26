// Functions for the <see cref="TaxRatesLC"/> class to manage the Social
// Security payroll and self-employed tax rates, with changes from present law.

// $Id: TaxRatesLC.cpp 1.14 2017/08/29 09:01:26EDT 277133 Development  $

#include <algorithm>
#include "TaxRatesLC.h"
#include "LawChangeArray.h"
#include "LawChangeTAXRATECHG.h"

using namespace std;

/// <summary>Initializes arrays.</summary>
///
/// <remarks>Calls <see cref="initialize"/>.</remarks>
///
/// <param name="lastYear">Maximum projected year.</param>
/// <param name="newLawChange">Law change data.</param>
TaxRatesLC::TaxRatesLC( int lastYear, const LawChangeArray& newLawChange ) :
TaxRates(lastYear), lawChange(newLawChange)
{
  initialize();
}

/// <summary>Destroys this instance.</summary>
TaxRatesLC::~TaxRatesLC()
{ }

/// <summary>Sets all tax rates, using historical rates unless overridden by
/// rates in law change data.</summary>
/// <remarks>Whether or not tax rates are changed, the tax credits are not
/// currently allowed to be specified for future years; therefore, calls
/// <see cref="TaxRates::setHistCredits"/>, specifying that all years are
/// being set. Finally calls <see cref="TaxRates::setGrossRates"/>.</remarks>
void TaxRatesLC::initialize()
{
  int i;  // loop counter
  LawChangeTAXRATECHG *lawChangeTAXRATECHG = lawChange.lawChangeTAXRATECHG;
  if (lawChangeTAXRATECHG->getInd() > 0 &&
    lawChangeTAXRATECHG->getNumTaxrates() > 0) {
    setHistRates(lawChangeTAXRATECHG->getYearTaxrate(0));
    // handle intermediate tax rates
    for (i = 0; i < lawChangeTAXRATECHG->getNumTaxrates() - 1; i++) {
      for (int year = lawChangeTAXRATECHG->getYearTaxrate(i);
      year < min(lawChangeTAXRATECHG->getYearTaxrate(i+1), maxyear);
      year++) {
        setNetRates(year, EMPLOYEERATE,
          lawChangeTAXRATECHG->getTaxrate(i, EMPLOYEERATE, OASITF),
          lawChangeTAXRATECHG->getTaxrate(i, EMPLOYEERATE, DITF),
          lawChangeTAXRATECHG->getTaxrate(i, EMPLOYEERATE, OASDITF));
        setNetRates(year, SELFEMPRATE,
          lawChangeTAXRATECHG->getTaxrate(i, SELFEMPRATE, OASITF),
          lawChangeTAXRATECHG->getTaxrate(i, SELFEMPRATE, DITF),
          lawChangeTAXRATECHG->getTaxrate(i, SELFEMPRATE, OASDITF));
      }
    }
    // handle final tax rate
    i = lawChangeTAXRATECHG->getNumTaxrates() - 1;
    for (int year = lawChangeTAXRATECHG->getYearTaxrate(i); year <= maxyear;
    year++) {
      setNetRates(year, EMPLOYEERATE,
        lawChangeTAXRATECHG->getTaxrate(i, EMPLOYEERATE, OASITF),
        lawChangeTAXRATECHG->getTaxrate(i, EMPLOYEERATE, DITF),
        lawChangeTAXRATECHG->getTaxrate(i, EMPLOYEERATE, OASDITF));
      setNetRates(year, SELFEMPRATE,
        lawChangeTAXRATECHG->getTaxrate(i, SELFEMPRATE, OASITF),
        lawChangeTAXRATECHG->getTaxrate(i, SELFEMPRATE, DITF),
        lawChangeTAXRATECHG->getTaxrate(i, SELFEMPRATE, OASDITF));
    }
  }
  else {
    setHistRates(maxyear);
  }
  // not currently allowing any change in credits
  setHistCredits(maxyear);
  setGrossRates();
}
