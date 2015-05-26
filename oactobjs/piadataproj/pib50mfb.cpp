// Functions for the <see cref="Pib50Mfb"/> class to handle 1950 pib-mfb
// conversion table.
//
// $Id: pib50mfb.cpp 1.10 2011/08/08 08:45:12EDT 044579 Development  $

#include "pibtable.h"
#include "Resource.h"
#include "PiaException.h"

// <summary>The 1950 mfb's.</summary>
const double Pib50Mfb::mfb50[] = {
  40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00,
  40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00,
  40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00,
  40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00,
  40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00, 40.00,
  40.00, 40.16, 40.32, 40.48, 40.64, 40.80, 40.96, 41.12, 41.28, 41.44,
  41.60, 41.76, 41.92, 42.08, 42.24, 42.40, 42.56, 42.72, 42.88, 43.04,
  43.20, 43.36, 43.52, 43.68, 43.84, 44.00, 44.16, 44.32, 44.48, 44.64,
  44.80, 44.96, 45.12, 45.28, 45.44, 45.60, 45.76, 45.92, 46.08, 46.24,
  46.40, 46.56, 46.72, 46.88, 47.04, 47.20, 47.36, 47.52, 47.68, 47.84,
  48.00, 48.16, 48.32, 48.48, 48.64, 48.80, 48.96, 49.12, 49.28, 49.44,
  49.60, 49.76, 49.92, 50.08, 50.24, 50.40, 50.56, 50.72, 50.88, 51.04,
  51.20, 51.36, 51.52, 51.68, 51.84, 52.00, 52.16, 52.32, 52.48, 52.64,
  52.80, 52.96, 53.12, 53.28, 53.44, 53.60, 53.76, 53.92, 54.08, 54.24,
  54.40, 54.56, 54.72, 54.88, 55.04, 55.20, 55.36, 55.52, 55.68, 55.84,
  56.00, 56.16, 56.32, 56.48, 56.64, 56.80, 56.96, 57.12, 57.28, 57.44,
  57.60, 57.76, 57.92, 58.08, 58.24, 58.40, 58.56, 58.72, 58.88, 59.04,
  59.20, 59.36, 59.52, 59.68, 59.84, 60.00, 60.16, 60.32, 60.48, 60.64,
  60.80, 60.96, 61.12, 61.28, 61.44, 61.60, 61.76, 61.92, 62.08, 62.24,
  62.40, 62.56, 62.72, 62.88, 63.04, 63.20, 63.36, 63.52, 63.68, 63.84,
  64.00, 64.16, 64.32, 64.48, 64.64, 64.80, 64.96, 65.12, 65.28, 65.44,
  65.60, 65.76, 65.92, 66.08, 66.24, 66.40, 66.56, 66.72, 66.88, 67.04,
  67.20, 67.36, 67.52, 67.68, 67.84, 68.00, 68.16, 68.32, 68.48, 68.64,
  68.80, 68.96, 69.12, 69.28, 69.44, 69.60, 69.76, 69.92, 70.08, 70.24,
  70.40, 70.56, 70.72, 70.88, 71.04, 71.20, 71.36, 71.52, 71.68, 71.84,
  72.00, 72.16, 72.32, 72.48, 72.64, 72.80, 72.96, 73.12, 73.28, 73.44,
  73.60, 73.76, 73.92, 74.08, 74.24, 74.40, 74.56, 74.72, 74.88, 75.04,
  75.20, 75.36, 75.52, 75.68, 75.84, 76.00, 76.16, 76.32, 76.48, 76.64,
  76.80, 76.96, 77.12, 77.28, 77.44, 77.60, 77.76, 77.92, 78.08, 78.24,
  78.40, 78.56, 78.72, 78.88, 79.04, 79.20, 79.36, 79.52, 79.68, 79.84,
  80.00, 80.54, 81.06, 81.60, 82.14, 82.66, 83.20, 83.74, 84.26, 84.80,
  85.34, 85.86, 86.40, 86.94, 87.46, 88.00, 88.53, 89.06, 89.58, 90.12,
  90.65, 91.18, 91.70, 92.23, 92.76, 93.30, 93.82, 94.35, 94.88, 95.42,
  95.94, 96.48, 97.02, 97.54, 98.08, 98.62, 99.14, 99.68, 100.22,
  100.74, 101.28, 101.82, 102.36, 102.90, 103.43, 103.97, 104.51,
  105.05, 105.58, 106.12, 106.66, 107.20, 107.73, 108.26, 108.79,
  109.32, 109.86, 110.38, 110.92, 111.45, 111.98, 112.51, 113.04,
  113.58, 114.11, 114.65, 115.18, 115.72, 116.26, 116.79, 117.33,
  117.86, 118.40, 118.93, 119.46, 119.98, 120.51, 121.04, 121.57,
  122.10, 122.62, 123.15, 123.68, 124.22, 124.75, 125.29, 125.82,
  126.36, 126.90, 127.43, 127.97, 128.50, 129.04, 129.58, 130.11,
  130.65, 131.18, 131.72, 132.26, 132.79, 133.33, 133.86, 134.40,
  134.93, 135.46, 135.98, 136.51, 137.04, 137.57, 138.10, 138.62,
  139.15, 139.68, 140.22, 140.75, 141.29, 141.82, 142.36, 142.90,
  143.43, 143.97, 144.50, 145.04, 145.58, 146.10, 146.64, 147.18,
  147.70, 148.24, 148.78, 149.30, 149.84, 150.00, 150.00, 150.00,
  150.00, 150.00, 150.00, 150.00, 150.00, 150.00, 150.00, 150.00,
  150.00, 150.00, 150.00, 150.00, 150.00, 150.00, 150.00, 150.00,
  150.00, 150.00, 150.00, 150.00, 150.00, 150.00, 150.00, 150.00,
  150.00, 150.00, 150.00, 150.00, 150.00, 150.00, 150.00, 150.00,
  150.00, 150.00, 150.00, 150.00, 150.00, 150.00, 150.00, 150.00,
  150.00, 150.00, 150.00, 150.00, 150.00, 150.00, 150.00, 150.00,
  150.00, 150.00, 150.00
};

/// <summary>Returns 1950 conversion table mfb.</summary>
///
/// <returns>1950 conversion table mfb.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PIB50MFB"/> if index is out of range
/// (only in debug mode).</exception>
///
/// <param name="index">Number of mfb desired.</param>
double Pib50Mfb::getAt( int index )
{
#ifndef NDEBUG
  if (index < 0 || index > 485)
    throw PiaException(PIA_IDS_PIB50MFB);
#endif
  return(mfb50[index]);
}
