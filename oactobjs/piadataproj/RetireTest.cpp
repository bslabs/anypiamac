// Functions for the <see cref="RetireTest"/> class to manage retirement test
// calculations.

// $Id: RetireTest.cpp 1.21 2012/07/30 16:26:52EDT 044579 Development  $

#include "RetireTest.h"
#include "piaparms.h"

using namespace std;

/// <summary>Constructor with first and last years specified.</summary>
///
/// <remarks>Calls the constructors for <see cref="RetireTest::rtea62m"/>,
/// <see cref="RetireTest::rtea62"/>, <see cref="RetireTest::rtea65m"/>, and
/// <see cref="RetireTest::rtea65"/> with the specified first and last years,
/// and the default constructor for <see cref="RetireTest::rteaHigh"/>.
/// </remarks>
///
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newBaseYear">First year allowed.</param>
/// <param name="newLastYear">Last year allowed.</param>
RetireTest::RetireTest( const PiaParams& newPiaParams, int newBaseYear,
int newLastYear ) : rtea62m(newBaseYear, newLastYear),
rtea62(newBaseYear, newLastYear), rtea65m(newBaseYear, newLastYear),
rtea65(newBaseYear, newLastYear), rteaHigh(), piaParams(newPiaParams)
{ }

/// <summary>Constructor with last year specified.</summary>
///
/// <remarks>Calls the constructors for <see cref="RetireTest::rtea62m"/>,
/// <see cref="RetireTest::rtea62"/>, <see cref="RetireTest::rtea65m"/>, and
/// <see cref="RetireTest::rtea65"/> with the specified last year and an
/// implied first year of 1937, and the default constructor for
/// <see cref="RetireTest::rteaHigh"/>.</remarks>
///
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newLastYear">Last year allowed.</param>
RetireTest::RetireTest( const PiaParams& newPiaParams, int newLastYear ) :
rtea62m(newLastYear), rtea62(newLastYear), rtea65m(newLastYear),
rtea65(newLastYear), rteaHigh(), piaParams(newPiaParams)
{ }

/// <summary>Returns how many months of person's benefit will be withheld
/// according to the retirement test.</summary>
///
/// <remarks>Some benies (i.e. disabled workers and disabled widow(er)s) are
/// not affected by the retirement test and should not have this function
/// called for them.</remarks>
///
/// <returns>How many months of person's benefit will be withheld according
/// to the retirement test.</returns>
///
/// <param name="age">The age at end of year.</param>
/// <param name="earnings">The earnings in year.</param>
/// <param name="year">Year being considered.</param>
/// <param name="benefit">Monthly benefit before reduction.</param>
double RetireTest::earningsTestCal( const Age& age, double earnings,
int year, double benefit ) const
{
  // no months of benefits if no benefit
  if (benefit <= 0.0)
    return 0.0;
  double reduction = 0.0;  // amount of annual reduction in benefits
  if (age < RtGeneral::getMaxAgePL(year)) {
    if (year > 1977 && age < PiaParams::maxDibAgePL(year)) {
      if (earnings > rtea62[year]) {
        reduction = static_cast<int>((earnings - rtea62[year]) /
          Rtea62m::getFraction(year));
      }
    }
    else if (year > 1960 && year < 1973) {
      if (earnings > rteaHigh[year]) {
        reduction = static_cast<int>((rteaHigh[year] - rtea65[year]) /
          Rtea65m::getFraction(year));
        reduction += static_cast<int>((earnings - rteaHigh[year]) /
          RteaHigh::getFraction());
      }
      else if (earnings > rtea65[year]) {
        reduction = static_cast<int>((earnings - rtea65[year]) /
          Rtea65m::getFraction(year));
      }
    }
    else {
      if (earnings > rtea65[year]) {
        reduction = static_cast<int>((earnings - rtea65[year]) /
          Rtea65m::getFraction(year));
      }
    }
  }
  // calculate number of months of benefit lost
  return min(reduction / benefit, 12.0);
}

/// <summary>Projects all retirement test exempt amounts.</summary>
///
/// <param name="firstYear">First year of projection.</param>
void RetireTest::project( int firstYear )
{
  rtea62m.project(firstYear, piaParams.getFqArray(),
    piaParams.getCpiincArray());
  rtea62.convertMonthToAnnual(rtea62m);
  rtea65m.project(firstYear, piaParams.getFqArray(),
    piaParams.getCpiincArray());
  rtea65.convertMonthToAnnual(rtea65m);
}
