// Functions for the <see cref="SgaGeneral"/> class - parent of classes to
// handle historical and projected SGA (Substantial Gainful Activity) amounts,
// stored in arrays.

// $Id: SgaGeneral.cpp 1.14 2017/08/29 09:01:26EDT 277133 Development  $

#include <algorithm>
#include <cmath>  // for floor
#include "SgaGeneral.h"

using namespace std;

/// <summary>Constructor.</summary>
///
/// <remarks>The SGA amounts start in 1961 and go through newLastYear.
/// </remarks>
///
/// <param name="newLastYear">New last year.</param>
/// <param name="newBaseSgaYear">New year for base SGA amount for projections.
/// </param>
SgaGeneral::SgaGeneral( int newLastYear, int newBaseSgaYear ) :
DoubleAnnual(1961, newLastYear), baseSgaYear(newBaseSgaYear),
baseAverageWageYear(newBaseSgaYear - 2)
{ }

/// <summary>Destructor.</summary>
SgaGeneral::~SgaGeneral()
{ }

/// <summary>Converts monthly SGA amounts to annual amounts, starting with
/// the first projected year.</summary>
///
/// <remarks>Historical amounts are initialized in the constructor of any
/// derived class (because, for some years, the annual amount is not simply 12
/// times the monthly amount).</remarks>
///
/// <param name="sgaM">Monthly SGA amounts.</param>
void SgaGeneral::convertMonthToAnnual( const SgaGeneral& sgaM )
{
  for (int yr = FIRST_PROJ_YEAR; yr <= getLastYear(); yr++) {
    theData[yr - getBaseYear()] = sgaM[yr] * 12.0;
  }
}

/// <summary>Projects the SGA monthly amounts.</summary>
///
/// <param name="averageWages">Average wages to use for projection.</param>
void SgaGeneral::project( const DoubleAnnual& averageWages )
{
  for (int yr = FIRST_PROJ_YEAR; yr <= getLastYear(); yr++) {
    const int index = yr - getBaseYear();
    theData[index] = project1(theData[baseSgaYear - getBaseYear()],
      averageWages[yr - 2], averageWages[baseAverageWageYear],
      theData[index - 1]);
  }
}
/// <summary>Calculates a monthly SGA amount using a base amount and
///   a ratio of average wages.</summary>
///
/// <returns>Increased monthly SGA amount.</returns>
///
/// <param name="baseAmount">Value of SGA amount in base year.</param>
/// <param name="numerator">Numerator used in projection.</param>
/// <param name="denominator">Denominator used in projection.</param>
/// <param name="previous">Value of SGA amount in previous year.</param>
double SgaGeneral::project1( double baseAmount, double numerator,
double denominator, double previous )
{
  // Round to the nearest 10 dollars.
  const double temp =
    floor(((numerator / denominator) * baseAmount) / 10.0 + 0.50);
  // Make sure it's not less than last year's amount.
  return max(10.0 * temp, previous);
}
