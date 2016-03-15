// Functions for the <see cref="TaxRates"/> class - parent of classes to
// manage OASDHI tax rates.

// $Id: TaxRates.cpp 1.19 2016/03/09 07:53:21EST 277133 Development  $

#include "TaxRates.h"
#include "wrkrdata.h"

using namespace std;

/// <summary>Historical OASI employee/employer net rates, 1937-2019.
/// </summary>
const double TaxRates::oasieeerrate[] = {
              0.01,    0.01,    0.01,    0.01,  // 1937-40
     0.01,    0.01,    0.01,    0.01,    0.01,  // 1941-45
     0.01,    0.01,    0.01,    0.01,   0.015,  // 1941-50
    0.015,   0.015,   0.015,    0.02,    0.02,  // 1951-55
     0.02,    0.02,    0.02,  0.0225,  0.0275,  // 1956-60
   0.0275, 0.02875, 0.03375, 0.03375, 0.03375,  // 1961-65
    0.035,  0.0355, 0.03325, 0.03725,  0.0365,  // 1966-70
   0.0405,  0.0405,   0.043, 0.04375, 0.04375,  // 1971-75
  0.04375, 0.04375, 0.04275,  0.0433,  0.0452,  // 1976-80
    0.047, 0.04575, 0.04775,0.049263158,0.052,  // 1981-85
    0.052,   0.052,  0.0553,  0.0553,   0.056,  // 1986-90
    0.056,   0.056,   0.056,  0.0526,  0.0526,  // 1991-95
   0.0526,  0.0535,  0.0535,  0.0535,   0.053,  // 1996-2000
    0.053,   0.053,   0.053,   0.053,   0.053,  // 2001-05
    0.053,   0.053,   0.053,   0.053,   0.053,  // 2006-10
0.03590323,0.03590323,0.053,   0.053,   0.053,  // 2011-15
  0.05015, 0.05015, 0.05015,   0.053            // 2016-19
};
/// <summary>Historical DI employee/employer net rates, 1937-2019.</summary>
const double TaxRates::dieeerrate[] = {
              0.00,    0.00,    0.00,    0.00,  // 1937-40
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1941-45
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1946-50
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1951-55
     0.00,  0.0025,  0.0025,  0.0025,  0.0025,  // 1956-60
   0.0025,  0.0025,  0.0025,  0.0025,  0.0025,  // 1961-65
   0.0035,  0.0035, 0.00475, 0.00475,  0.0055,  // 1966-70
   0.0055,  0.0055,  0.0055, 0.00575, 0.00575,  // 1971-75
  0.00575, 0.00575, 0.00775,  0.0075,  0.0056,  // 1976-80
   0.0065, 0.00825, 0.00625,0.004736842,0.005,  // 1981-85
    0.005,   0.005,  0.0053,  0.0053,   0.006,  // 1986-90
    0.006,   0.006,   0.006,  0.0094,  0.0094,  // 1991-95
   0.0094,  0.0085,  0.0085,  0.0085,   0.009,  // 1996-2000
    0.009,   0.009,   0.009,   0.009,   0.009,  // 2001-05
    0.009,   0.009,   0.009,   0.009,   0.009,  // 2006-10
0.00609677,0.00609677,0.009,   0.009,   0.009,  // 2011-15
  0.01185, 0.01185, 0.01185,   0.009            // 2016-19
};
/// <summary>Historical HI employee/employer net rates, 1937-2019.</summary>
const double TaxRates::hieeerrate[] = {
              0.00,    0.00,    0.00,    0.00,  // 1937-40
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1941-45
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1946-50
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1951-55
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1956-60
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1961-65
   0.0035,   0.005,   0.006,   0.006,   0.006,  // 1966-70
    0.006,   0.006,    0.01,   0.009,   0.009,  // 1971-75
    0.009,   0.009,    0.01,  0.0105,  0.0105,  // 1976-80
    0.013,   0.013,   0.013,   0.013,  0.0135,  // 1981-85
   0.0145,  0.0145,  0.0145,  0.0145,  0.0145,  // 1986-90
   0.0145,  0.0145,  0.0145,  0.0145,  0.0145,  // 1991-95
   0.0145,  0.0145,  0.0145,  0.0145,  0.0145,  // 1996-2000
   0.0145,  0.0145,  0.0145,  0.0145,  0.0145,  // 2001-05
   0.0145,  0.0145,  0.0145,  0.0145,  0.0145,  // 2006-10
   0.0145,  0.0145,  0.0145,  0.0145,  0.0145,  // 2011-15
   0.0145,  0.0145,  0.0145,  0.0145            // 2016-19
};
/// <summary>Historical OASI self-employed net rates, 1937-2019.</summary>
const double TaxRates::oasiserate[] = {
              0.00,    0.00,    0.00,    0.00,  // 1937-40
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1941-45
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1941-50
   0.0225,  0.0225,  0.0225,    0.03,    0.03,  // 1951-55
     0.03,    0.03,    0.03, 0.03375, 0.04125,  // 1956-60
  0.04125, 0.04325, 0.05025, 0.05025, 0.05025,  // 1961-65
  0.05275, 0.05375,0.050875,0.055875, 0.05475,  // 1966-70
  0.06075, 0.06075, 0.06205, 0.06185, 0.06185,  // 1971-75
  0.06185, 0.06185,  0.0601,  0.0601,0.062725,  // 1976-80
  0.07025,0.068125,0.071125,  0.0839,   0.087,  // 1981-85
   0.0895,  0.0895,  0.0959,  0.0959,  0.1120,  // 1986-90
   0.1120,  0.1120,  0.1120,  0.1052,  0.1052,  // 1991-95
   0.1052,  0.1070,  0.1070,  0.1070,  0.1060,  // 1996-2000
   0.1060,  0.1060,  0.1060,  0.1060,  0.1060,  // 2000-05
   0.1060,  0.1060,  0.1060,  0.1060,  0.1060,  // 2006-10
0.08890323,0.08890323,0.1060, 0.1060,  0.1060,  // 2011-15
   0.1003,  0.1003,  0.1003,  0.1060            // 2016-19
};
/// <summary>Historical DI self-employed net rates, 1937-2019.</summary>
const double TaxRates::diserate[] = {
              0.00,    0.00,    0.00,    0.00,  // 1937-40
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1941-45
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1941-50
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1951-55
     0.00, 0.00375, 0.00375, 0.00375, 0.00375,  // 1956-60
  0.00375, 0.00375, 0.00375, 0.00375, 0.00375,  // 1961-65
  0.00525, 0.00525,0.007125,0.007125, 0.00825,  // 1966-70
  0.00825, 0.00825, 0.00795, 0.00815, 0.00815,  // 1971-75
  0.00815, 0.00815,  0.0109,  0.0104,0.007775,  // 1976-80
  0.00975,0.012375,0.009375,  0.0081,  0.0084,  // 1981-85
   0.0086,  0.0086,  0.0092,  0.0092,  0.0120,  // 1986-90
   0.0120,  0.0120,  0.0120,  0.0188,  0.0188,  // 1991-95
   0.0188,  0.0170,  0.0170,  0.0170,  0.0180,  // 1996-2000
   0.0180,  0.0180,  0.0180,  0.0180,  0.0180,  // 2001-05
   0.0180,  0.0180,  0.0180,  0.0180,  0.0180,  // 2006-10
0.01509677,0.01509677,0.0180, 0.0180,  0.0180,  // 2011-15
   0.0237,  0.0237,  0.0237,  0.0180            // 2016-19
};
/// <summary>Historical HI self-employed net rates, 1937-2019.</summary>
const double TaxRates::hiserate[] = {
              0.00,    0.00,    0.00,    0.00,  // 1937-40
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1941-45
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1946-50
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1951-55
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1956-60
     0.00,    0.00,    0.00,    0.00,    0.00,  // 1961-65
   0.0035,   0.005,   0.006,   0.006,   0.006,  // 1966-70
    0.006,   0.006,    0.01,   0.009,   0.009,  // 1971-75
    0.009,   0.009,    0.01,  0.0105,  0.0105,  // 1976-80
    0.013,   0.013,   0.013,   0.021,  0.0226,  // 1981-85
   0.0249,  0.0249,  0.0251,  0.0251,   0.029,  // 1986-90
    0.029,   0.029,   0.029,   0.029,   0.029,  // 1991-95
    0.029,   0.029,   0.029,   0.029,   0.029,  // 1996-2000
    0.029,   0.029,   0.029,   0.029,   0.029,  // 2001-05
    0.029,   0.029,   0.029,   0.029,   0.029,  // 2006-10
    0.029,   0.029,   0.029,   0.029,   0.029,  // 2011-15
    0.029,   0.029,   0.029,   0.029            // 2016-19
};
/// <summary>Historical OASI employee/employer credit, 1937-2019.</summary>
const double TaxRates::oasieeercredit[] = {
               0.0,     0.0,     0.0,     0.0,  // 1937-40
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1941-45
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1946-50
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1951-55
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1956-60
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1961-65
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1966-70
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1971-75
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1976-80
      0.0,     0.0,     0.0, 0.002736842, 0.0,  // 1981-85
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1986-90
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1991-95
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1996-2000
      0.0,     0.0,     0.0,     0.0,     0.0,  // 2001-05
      0.0,     0.0,     0.0,     0.0,     0.0,  // 2006-10
0.01709677,0.01709677,  0.0,     0.0,     0.0,  // 2011-15
      0.0,     0.0,     0.0,     0.0            // 2016-19
};
/// <summary>Historical DI employee/employer credit, 1937-2019.</summary>
const double TaxRates::dieeercredit[] = {
               0.0,     0.0,     0.0,     0.0,  // 1937-40
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1941-45
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1946-50
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1951-55
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1956-60
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1961-65
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1966-70
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1971-75
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1976-80
      0.0,     0.0,     0.0, 0.000263158, 0.0,  // 1981-85
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1986-90
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1991-95
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1996-2000
      0.0,     0.0,     0.0,     0.0,     0.0,  // 2001-05
      0.0,     0.0,     0.0,     0.0,     0.0,  // 2006-10
0.00290323,0.00290323,  0.0,     0.0,     0.0,  // 2011-15
      0.0,     0.0,     0.0,     0.0            // 2016-19
};
/// <summary>Historical HI employee/employer credit, 1937-2019.</summary>
const double TaxRates::hieeercredit[] = {
               0.0,     0.0,     0.0,     0.0,  // 1937-40
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1941-45
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1946-50
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1951-55
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1956-60
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1961-65
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1966-70
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1971-75
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1976-80
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1981-85
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1986-90
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1991-95
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1996-2000
      0.0,     0.0,     0.0,     0.0,     0.0,  // 2001-05
      0.0,     0.0,     0.0,     0.0,     0.0,  // 2006-10
      0.0,     0.0,     0.0,     0.0,     0.0,  // 2011-15
      0.0,     0.0,     0.0,     0.0            // 2016-19
};
/// <summary>Historical OASI self-employed credits, 1937-2019.</summary>
const double TaxRates::oasisecredit[] = {
               0.0,     0.0,     0.0,     0.0,  // 1937-40
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1941-45
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1946-50
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1951-55
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1956-60
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1961-65
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1966-70
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1971-75
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1976-80
      0.0,     0.0,     0.0,  0.0201,   0.017,  // 1981-85
   0.0145,  0.0145,  0.0147,  0.0147,     0.0,  // 1986-90
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1991-95
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1996-2000
      0.0,     0.0,     0.0,     0.0,     0.0,  // 2001-05
      0.0,     0.0,     0.0,     0.0,     0.0,  // 2006-10
0.01709677,0.01709677,  0.0,     0.0,     0.0,  // 2011-15
      0.0,     0.0,     0.0,     0.0            // 2016-19
};
/// <summary>Historical DI self-employed credits, 1937-2013.</summary>
const double TaxRates::disecredit[] = {
               0.0,     0.0,     0.0,     0.0,  // 1937-40
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1941-45
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1946-50
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1951-55
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1956-60
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1961-65
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1966-70
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1971-75
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1976-80
      0.0,     0.0,     0.0,  0.0019,  0.0016,  // 1981-85
   0.0014,  0.0014,  0.0014,  0.0014,     0.0,  // 1986-90
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1991-95
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1996-2000
      0.0,     0.0,     0.0,     0.0,     0.0,  // 2001-05
      0.0,     0.0,     0.0,     0.0,     0.0,  // 2006-10
0.00290323,0.00290323,  0.0,     0.0,     0.0,  // 2011-15
      0.0,     0.0,     0.0,     0.0            // 2016-19
};
/// <summary>Historical HI self-employed credits, 1937-2019.</summary>
const double TaxRates::hisecredit[] = {
               0.0,     0.0,     0.0,     0.0,  // 1937-40
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1941-45
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1946-50
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1951-55
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1956-60
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1961-65
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1966-70
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1971-75
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1976-80
      0.0,     0.0,     0.0,   0.005,  0.0044,  // 1981-85
   0.0041,  0.0041,  0.0039,  0.0039,     0.0,  // 1986-90
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1991-95
      0.0,     0.0,     0.0,     0.0,     0.0,  // 1996-2000
      0.0,     0.0,     0.0,     0.0,     0.0,  // 2001-05
      0.0,     0.0,     0.0,     0.0,     0.0,  // 2006-10
      0.0,     0.0,     0.0,     0.0,     0.0,  // 2011-15
      0.0,     0.0,     0.0,     0.0            // 2016-19
};

/// <summary>Initializes arrays.</summary>
///
/// <param name="lastYear">Maximum projected year.</param>
TaxRates::TaxRates( int lastYear ) : maxyear(lastYear)
{
  // allocate memory for net tax rate arrays
  for (int i1 = 0; i1 < NUMTAXRATETYPES; i1++) {
    for (int i = 0; i < NUMTRUSTFUNDTYPES; i++) {
      taxrate[i1][i] = new DoubleAnnual(YEAR37, maxyear);
    }
  }
  // allocate memory for tax credit arrays
  for (int i1 = 0; i1 < NUMTAXRATETYPES; i1++) {
    for (int i = 0; i < NUMTRUSTFUNDTYPES; i++) {
      taxcredit[i1][i] = new DoubleAnnual(YEAR37, maxyear);
    }
  }
  // allocate memory for gross tax rate arrays
  for (int i1 = 0; i1 < NUMTAXRATETYPES; i1++) {
    for (int i = 0; i < NUMTRUSTFUNDTYPES; i++) {
      taxrategross[i1][i] = new DoubleAnnual(YEAR37, maxyear);
    }
  }
}

/// <summary>Destroys this instance.</summary>
///
/// <remarks>Deletes memory for tax rate and credit arrays.</remarks>
TaxRates::~TaxRates()
{
  // delete memory for net tax rate arrays
  for (int i1 = 0; i1 < NUMTAXRATETYPES; i1++) {
    for (int i = 0; i < NUMTRUSTFUNDTYPES; i++) {
      delete taxrate[i1][i];
    }
  }
  // delete memory for tax credit arrays
  for (int i1 = 0; i1 < NUMTAXRATETYPES; i1++) {
    for (int i = 0; i < NUMTRUSTFUNDTYPES; i++) {
      delete taxcredit[i1][i];
    }
  }
  // delete memory for gross tax rate arrays
  for (int i1 = 0; i1 < NUMTAXRATETYPES; i1++) {
    for (int i = 0; i < NUMTRUSTFUNDTYPES; i++) {
      delete taxrategross[i1][i];
    }
  }
}

/// <summary>Sets historical tax rates.</summary>
///
/// <remarks>This is called from <see cref="TaxRatesLC::initialize"/> and
/// <see cref="TaxRatesPL::initialize"/>.</remarks>
///
/// <param name="lastYear">Last year in historical period (could be last
///  projected year in present law case).</param>
void TaxRates::setHistRates( int lastYear )
{
  const int year2 = min(lastYear, LAST_TAX_RATE_YEAR);
  for (int year = YEAR37; year <= year2; year++) {
    const int index = year - YEAR37;
    setNetRates(year, EMPLOYEERATE, oasieeerrate[index], dieeerrate[index],
      hieeerrate[index]);
    setNetRates(year, SELFEMPRATE, oasiserate[index], diserate[index],
      hiserate[index]);
  }
  const int index = LAST_TAX_RATE_YEAR - YEAR37;
  for (int year = LAST_TAX_RATE_YEAR + 1; year <= lastYear; year++) {
    setNetRates(year, EMPLOYEERATE, oasieeerrate[index], dieeerrate[index],
      hieeerrate[index]);
    setNetRates(year, SELFEMPRATE, oasiserate[index], diserate[index],
      hiserate[index]);
  }
}

/// <summary>Sets historical tax credits.</summary>
///
/// <remarks>This is called from <see cref="TaxRatesLC::initialize"/> and
/// <see cref="TaxRatesPL::initialize"/>.</remarks>
///
/// <param name="lastYear">Last year in historical period (could be last
/// projected year in present law case).</param>
void TaxRates::setHistCredits( int lastYear )
{
  const int year2 = min(lastYear, LAST_TAX_RATE_YEAR);
  for (int year = YEAR37; year <= year2; year++) {
    const int index = year - YEAR37;
    setCredits(year, EMPLOYEERATE, oasieeercredit[index],
      dieeercredit[index], hieeercredit[index]);
    setCredits(year, SELFEMPRATE, oasisecredit[index],
      disecredit[index], hisecredit[index]);
  }
  const int index = LAST_TAX_RATE_YEAR - YEAR37;
  for (int year = LAST_TAX_RATE_YEAR + 1; year <= lastYear; year++) {
    setCredits(year, EMPLOYEERATE, oasieeercredit[index],
      dieeercredit[index], hieeercredit[index]);
    setCredits(year, SELFEMPRATE, oasisecredit[index],
      disecredit[index], hisecredit[index]);
  }
}

/// <summary>Sets net tax rates for one year for employee or employer for
/// all trust funds.</summary>
///
/// <param name="year">Year for which rates are being set.</param>
/// <param name="taxType">Tax type. 0: employee, 1: employer.</param>
/// <param name="oasiRate">OASI tax rate.</param>
/// <param name="diRate">DI tax rate.</param>
/// <param name="hiRate">HI tax rate.</param>
void TaxRates::setNetRates( int year, taxrateType taxType, double oasiRate,
double diRate, double hiRate )
{
  (*taxrate[taxType][OASITF])[year] = oasiRate;
  (*taxrate[taxType][DITF])[year] = diRate;
  (*taxrate[taxType][OASDITF])[year] = oasiRate + diRate;
  (*taxrate[taxType][HITF])[year] = hiRate;
  (*taxrate[taxType][OASDHITF])[year] =
    (*taxrate[taxType][OASDITF])[year] + hiRate;
}

/// <summary>Sets tax credits for one year for employee or employer for
/// all trust funds.</summary>
/// <remarks>This is called from <see cref="setHistCredits"/> and
/// <see cref="TaxRatesLC::initialize"/></remarks>
/// <param name="year">Year for which credits are being set.</param>
/// <param name="taxType">Tax type.</param>
/// <param name="oasiCredit">OASI tax rate.</param>
/// <param name="diCredit">DI tax rate.</param>
/// <param name="hiCredit">HI tax rate.</param>
void TaxRates::setCredits(int year, taxrateType taxType, double oasiCredit,
  double diCredit, double hiCredit)
{
  (*taxcredit[taxType][OASITF])[year] = oasiCredit;
  (*taxcredit[taxType][DITF])[year] = diCredit;
  (*taxcredit[taxType][OASDITF])[year] = oasiCredit + diCredit;
  (*taxcredit[taxType][HITF])[year] = hiCredit;
  (*taxcredit[taxType][OASDHITF])[year] =
    (*taxcredit[taxType][OASDITF])[year] + hiCredit;
}

/// <summary>Sets gross tax rates for all years for employee and employer,
/// for all trust funds.</summary>
///
/// <param name="year">Year for which rates are being set.</param>
/// <param name="taxType">Tax type.
///     0: employee, 1: employer.</param>
/// <param name="oasiRate">OASI tax rate.</param>
/// <param name="diRate">DI tax rate.</param>
/// <param name="hiRate">HI tax rate.</param>
void TaxRates::setGrossRates()
{
  for (int year = YEAR37; year <= maxyear; year++) {
    for (int taxType = EMPLOYEERATE; taxType < NUMTAXRATETYPES; taxType++) {
      (*taxrategross[taxType][OASITF])[year] =
        (*taxrate[taxType][OASITF])[year] +
        (*taxcredit[taxType][OASITF])[year];
      (*taxrategross[taxType][DITF])[year] =
        (*taxrate[taxType][DITF])[year] +
        (*taxcredit[taxType][DITF])[year];
      (*taxrategross[taxType][OASDITF])[year] =
        (*taxrate[taxType][OASDITF])[year] +
        (*taxcredit[taxType][OASDITF])[year];
      (*taxrategross[taxType][HITF])[year] =
        (*taxrate[taxType][HITF])[year] +
        (*taxcredit[taxType][HITF])[year];
      (*taxrategross[taxType][OASDHITF])[year] =
        (*taxrate[taxType][OASDITF])[year] +
        (*taxcredit[taxType][OASDHITF])[year];
    }
  }
}

/// <summary>Calculates taxes paid over a lifetime, net of any tax credit.
/// </summary>
///
/// <param name="earnOasdi">Array of earnings limited to wage base.</param>
/// <param name="taxType">Array of tax types. Each entry is true for employee
/// or false for employer.</param>
/// <param name="taxes">Array to hold calculated taxes.</param>
/// <param name="fundType">Trust fund type (0: oasi, 1: di, 2: oasdi, 3: hi,
/// 4: oasdi).</param>
void TaxRates::taxCalAllYears( const DoubleAnnual& earnOasdi,
const BitAnnual& taxType, DoubleAnnual& taxes, trustfundType fundType ) const
{
  const int year2 = min(maxyear, earnOasdi.getLastYear());
  // check tax type
  for (int year = YEAR37; year <= year2; year++) {
    WorkerData::taxTypeCheck(static_cast<int>(taxType.getBit(year)));
  }
  // calculate taxes
  for (int year = YEAR37; year <= year2; year++) {
    taxes[year] = taxCalOneYearNet(year, earnOasdi[year],
      (taxType.getBit(year) ? SELFEMPRATE : EMPLOYEERATE), fundType);
  }
}

/// <summary>Returns taxes paid for one year for one trust fund, net of any
/// tax credit.</summary>
///
/// <returns>Taxes paid for one year for one trust fund.</returns>
///
/// <param name="year">Year of earnings.</param>
/// <param name="earnings">The earnings limited to wage base.</param>
/// <param name="taxType">Tax type.</param>
/// <param name="fundType">Trust fund type.</param>
double TaxRates::taxCalOneYearNet( int year, double earnings,
  taxrateType taxType, trustfundType fundType ) const
{
  return ((*taxrate[taxType][fundType])[year] * earnings);
}

/// <summary>Returns taxes paid for one year for one trust fund, including any
/// tax credit.</summary>
///
/// <returns>Taxes paid for one year for one trust fund.</returns>
///
/// <param name="year">Year of earnings.</param>
/// <param name="earnings">The earnings limited to wage base.</param>
/// <param name="taxType">Tax type.</param>
/// <param name="fundType">Trust fund type.</param>
double TaxRates::taxCalOneYearGross( int year, double earnings,
  taxrateType taxType, trustfundType fundType ) const
{
  return ((*taxrategross[taxType][fundType])[year] * earnings);
}
