// Functions of the <see cref="AwbiDataNonFile"/> class to manage historical
// Social Security parameters stored with the code.
//
// $Id: awbidtnf.cpp 1.51 2018/10/16 10:48:40EDT 277133 Development  $

#include <algorithm>
#include "AwbiDataNonFile.h"
#include "PiaException.h"
#include "Resource.h"
#include "BaseYearNonFile.h"

using namespace std;

// <summary>Benefit increases, 1951-2018.</summary>
const double AwbiDataNonFile::cpiincData[] = {
   0.0, 12.5,  0.0, 13.0,  0.0,  0.0,  0.0,  0.0,  7.0,  0.0,
   0.0,  0.0,  0.0,  0.0,  7.0,  0.0,  0.0, 13.0,  0.0, 15.0,
  10.0, 20.0,  0.0, 11.0,  8.0,  6.4,  5.9,  6.5,  9.9, 14.3,
  11.2,  7.4,  3.5,  3.5,  3.1,  1.3,  4.2,  4.0,  4.7,  5.4,
   3.7,  3.0,  2.6,  2.8,  2.6,  2.9,  2.1,  1.3,  2.4,  3.5,
   2.6,  1.4,  2.1,  2.7,  4.1,  3.3,  2.3,  5.8,  0.0,  0.0,
   3.6,  1.7,  1.5,  1.7,  0.0,  0.3,  2.0,  2.8
};

// <summary>Average wage index, 1937-2017.</summary>
const double AwbiDataNonFile::fqData[] = {
             1137.96,  1053.24,  1142.36,  1195.00,
   1276.04,  1454.28,  1713.52,  1936.32,  2021.40,
   1891.76,  2175.32,  2361.64,  2483.20,  2543.96,
   2799.16,  2973.32,  3139.44,  3155.64,  3301.44,
   3532.36,  3641.72,  3673.80,  3855.80,  4007.12,
   4086.76,  4291.40,  4396.64,  4576.32,  4658.72,
   4938.36,  5213.44,  5571.76,  5893.76,  6186.24,
   6497.08,  7133.80,  7580.16,  8030.76,  8630.92,
   9226.48,  9779.44, 10556.03, 11479.46, 12513.46,
  13773.10, 14531.34, 15239.24, 16135.07, 16822.51,
  17321.82, 18426.51, 19334.04, 20099.55, 21027.98,
  21811.60, 22935.42, 23132.67, 23753.53, 24705.66,
  25913.90, 27426.00, 28861.44, 30469.84, 32154.82,
  32921.92, 33252.09, 34064.95, 35648.55, 36952.94,
  38651.41, 40405.48, 41334.97, 40711.61, 41673.83,
  42979.61, 44321.67, 44888.16, 46481.52, 48098.63,
  48642.15, 50321.89
};

// <summary>OASDI taxable maximum, 1937-2019.</summary>
const double AwbiDataNonFile::baseOasdiData[] = {
              3000.00,   3000.00,   3000.00,   3000.00,
   3000.00,   3000.00,   3000.00,   3000.00,   3000.00,
   3000.00,   3000.00,   3000.00,   3000.00,   3000.00,
   3600.00,   3600.00,   3600.00,   3600.00,   4200.00,
   4200.00,   4200.00,   4200.00,   4800.00,   4800.00,
   4800.00,   4800.00,   4800.00,   4800.00,   4800.00,
   6600.00,   6600.00,   7800.00,   7800.00,   7800.00,
   7800.00,   9000.00,  10800.00,  13200.00,  14100.00,
  15300.00,  16500.00,  17700.00,  22900.00,  25900.00,
  29700.00,  32400.00,  35700.00,  37800.00,  39600.00,
  42000.00,  43800.00,  45000.00,  48000.00,  51300.00,
  53400.00,  55500.00,  57600.00,  60600.00,  61200.00,
  62700.00,  65400.00,  68400.00,  72600.00,  76200.00,
  80400.00,  84900.00,  87000.00,  87900.00,  90000.00,
  94200.00,  97500.00, 102000.00, 106800.00, 106800.00,
 106800.00, 110100.00, 113700.00, 117000.00, 118500.00,
 118500.00, 127200.00, 128400.00, 132900.00
};

// <summary>HI taxable maximum, 1991-2019.</summary>
const double AwbiDataNonFile::baseHiData[] = {
   125000.00, 130200.00, 135000.00,   MAXEARN,   MAXEARN,
     MAXEARN,   MAXEARN,   MAXEARN,   MAXEARN,   MAXEARN,
     MAXEARN,   MAXEARN,   MAXEARN,   MAXEARN,   MAXEARN,
     MAXEARN,   MAXEARN,   MAXEARN,   MAXEARN,   MAXEARN,
     MAXEARN,   MAXEARN,   MAXEARN,   MAXEARN,   MAXEARN,
     MAXEARN,   MAXEARN,   MAXEARN,   MAXEARN
};

// <summary>Old-law taxable maximum, 1937-2019.</summary>
const double AwbiDataNonFile::base77Data[] = {
              3000.00,   3000.00,   3000.00,   3000.00,
   3000.00,   3000.00,   3000.00,   3000.00,   3000.00,
   3000.00,   3000.00,   3000.00,   3000.00,   3000.00,
   3600.00,   3600.00,   3600.00,   3600.00,   4200.00,
   4200.00,   4200.00,   4200.00,   4800.00,   4800.00,
   4800.00,   4800.00,   4800.00,   4800.00,   4800.00,
   6600.00,   6600.00,   7800.00,   7800.00,   7800.00,
   7800.00,   9000.00,  10800.00,  13200.00,  14100.00,
  15300.00,  16500.00,  17700.00,  18900.00,  20400.00,
  22200.00,  24300.00,  26700.00,  28200.00,  29700.00,
  31500.00,  32700.00,  33600.00,  35700.00,  38100.00,
  39600.00,  41400.00,  42900.00,  45000.00,  45300.00,
  46500.00,  48600.00,  50700.00,  53700.00,  56700.00,
  59700.00,  63000.00,  64500.00,  65100.00,  66900.00,
  69900.00,  72600.00,  75900.00,  79200.00,  79200.00,
  79200.00,  81900.00,  84300.00,  87000.00,  88200.00,
  88200.00,  94500.00,  95400.00,  98700.00
};

/// <summary>Initializes historical parameters info, where the last year of
/// wage bases is specified.</summary>
///
/// <remarks>Calls <see cref="setData"/> after setting the last year of wage
/// bases and the maximum year of projection.</remarks>
///
/// <param name="newLastYear">Last year of wage bases.</param>
/// <param name="newMaxyear">Maximum projected year.</param>
AwbiDataNonFile::AwbiDataNonFile( int newLastYear, int newMaxyear ) :
AwbiData(newLastYear, newMaxyear)
{
  setData();
}

/// <summary>Initializes historical parameters info, where the last year of
/// wage bases is set to <see cref="BaseYearNonFile::YEAR"/>.</summary>
///
/// <remarks>Calls <see cref="setData"/> after setting the last year of wage
/// bases and the maximum year of projection.</remarks>
///
/// <param name="newMaxyear">Maximum projected year.</param>
AwbiDataNonFile::AwbiDataNonFile( int newMaxyear ) : AwbiData(newMaxyear)
{
  setData();
}

/// <summary>Sets values for historical parameters through the last year of
/// historical wage bases (to avoid reading from disk).</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_AWBIDAT1"/> if last year of historical data is greater
/// than <see cref="BaseYearNonFile::YEAR"/> (only in debug mode).</exception>
void AwbiDataNonFile::setData()
{
#ifndef NDEBUG
  if (getLastYear() > BaseYearNonFile::YEAR) {
    throw PiaException(PIA_IDS_AWBIDAT1);
  }
#endif
  for (int year = cpiinc.getBaseYear(); year <= getLastYear() - 1; year++) {
    cpiinc[year] = getCpiincData(year);
  }
  for (int year = fq.getBaseYear(); year <= getLastYear() - 2; year++) {
    fq[year] = getFqData(year);
  }
  const int year1 = min(getLastYear(), baseOasdi.getLastYear());
  for (int year = baseOasdi.getBaseYear(); year <= year1; year++) {
    baseOasdi[year] = getBaseOasdiData(year);
  }
  const int year2 = min(getLastYear(), baseHi.getLastYear());
  baseHi.assign(baseOasdi, baseHi.getBaseYear(), HI_BASE_YEAR - 1);
  for (int year = HI_BASE_YEAR; year <= year2; year++) {
    baseHi[year] = getBaseHiData(year);
  }
  const int year3 = min(getLastYear(), base77.getLastYear());
  for (int year = base77.getBaseYear(); year <= year3; year++) {
    base77[year] = getBase77Data(year);
  }
}
