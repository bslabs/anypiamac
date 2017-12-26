// Functions for the <see cref="AnnualArrays"/> class of static functions
// using annual arrays.
//
// $Id: AnnualArrays.cpp 1.5 2017/08/29 09:03:51EDT 277133 Development  $

#include <sstream>
#include <algorithm>
#include "AnnualArrays.h"
#include "dbleann.h"
#include "intann.h"

using namespace std;

/// <summary>Divides the first <see cref="DoubleAnnual"/> by the second
/// <see cref="DoubleAnnual"/>.</summary>
///
/// <param name="data1">Array of doubles to be divided.</param>
/// <param name="data2">Array of doubles to divide by.</param>
/// <param name="firstYear">First year to divide.</param>
/// <param name="lastYear">Last year to divide.</param>
void AnnualArrays::divide( DoubleAnnual& data1, const DoubleAnnual& data2,
int firstYear, int lastYear )
{
  int year1 = max(data1.getBaseYear(), data2.getBaseYear());
  year1 = max(year1, firstYear);
  int year2 = min(data1.getLastYear(), data2.getLastYear());
  year2 = min(year2, lastYear);
  for (int year = year1; year <= year2; year++) {
    if (data2[year] == 0.0)
      data1[year] = 0.0;
    else
      data1[year] /= data2[year];
  }
}

/// <summary>Returns the <see cref="DoubleAnnual"/> converted from an
/// <see cref="IntAnnual"/>.</summary>
///
/// <returns>The <see cref="DoubleAnnual"/> converted from an
/// <see cref="IntAnnual"/>.</returns>
///
/// <param name="intAnnual">Array of integers to convert.</param>
DoubleAnnual AnnualArrays::convert( const IntAnnual& intAnnual )
{
  const int year1 = intAnnual.getBaseYear();
  const int year2 = intAnnual.getLastYear();
  DoubleAnnual doubleAnnual(year1, year2);
  for (int year = year1; year <= year2; year++) {
    doubleAnnual[year] = static_cast<double>(intAnnual[year]);
  }
  return doubleAnnual;
}

/// <summary>Adds the second <see cref="DoubleAnnual"/> to the first
/// <see cref="DoubleAnnual"/>.</summary>
///
/// <param name="data1">Array of doubles to be added to.</param>
/// <param name="data2">Array of doubles to add to data1.</param>
/// <param name="firstYear">First year to add.</param>
/// <param name="lastYear">Last year to add.</param>
void AnnualArrays::add( DoubleAnnual& data1, const DoubleAnnual& data2,
int firstYear, int lastYear )
{
  for (int year = firstYear; year <= lastYear; year++) {
    data1[year] += data2[year];
  }
}

/// <summary>Subtracts the second <see cref="DoubleAnnual"/> from the first
/// <see cref="DoubleAnnual"/>.</summary>
///
/// <param name="data1">Array of doubles to be subtracted from.</param>
/// <param name="data2">Array of doubles to subtract from data1.</param>
/// <param name="firstYear">First year to subtract.</param>
/// <param name="lastYear">Last year to subtract.</param>
void AnnualArrays::subtract( DoubleAnnual& data1, const DoubleAnnual& data2,
int firstYear, int lastYear )
{
  for (int year = firstYear; year <= lastYear; year++) {
    data1[year] -= data2[year];
  }
}

/// <summary>Multiplies the second <see cref="DoubleAnnual"/> into the first
/// <see cref="DoubleAnnual"/>.</summary>
///
/// <param name="data1">Array of doubles to be multiplied into.</param>
/// <param name="data2">Array of doubles to multiply into data1.</param>
/// <param name="firstYear">First year of data to multiply.</param>
/// <param name="lastYear">Last year of data to multiply.</param>
void AnnualArrays::multiply( DoubleAnnual& data1, const DoubleAnnual& data2,
int firstYear, int lastYear )
{
  int year1 = max(data1.getBaseYear(), data2.getBaseYear());
  year1 = max(year1, firstYear);
  int year2 = min(data1.getLastYear(), data2.getLastYear());
  year2 = min(year2, lastYear);
  for (int year = year1; year <= year2; year++) {
    data1[year] *= data2[year];
  }
}

/// <summary>Multiplies the <see cref="DoubleAnnual"/> by a scalar.</summary>
///
/// <param name="data1">Array of doubles to be multiplied into.</param>
/// <param name="factor">The scalar multiplier. It is applied to all specified
/// years of data.</param>
/// <param name="firstYear">First year of data to multiply.</param>
/// <param name="lastYear">Last year of data to multiply.</param>
void AnnualArrays::multiply( DoubleAnnual& data1, double factor,
int firstYear, int lastYear )
{
  const int year1 = max(data1.getBaseYear(), firstYear);
  const int year2 = min(data1.getLastYear(), lastYear);
  for (int i = year1; i <= year2; i++) {
    data1[i] *= factor;
  }
}
