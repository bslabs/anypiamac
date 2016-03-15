// Functions for the <see cref="RtGeneral"/> class - parent of classes to
// manage one retirement test projection.

// $Id: rtgenrl.cpp 1.48 2016/01/11 08:51:04EST 277133 Development  $

#include "rtgenrl.h"
#include "Resource.h"
#include "PiaException.h"
#include "BaseYearNonFile.h"
#include "piaparms.h"

using namespace std;

// <summary>The age 62 monthly exempt amounts, 1951-2016.</summary>
const double Rtea62m::theAmts[] = {
   50.0,    50.0,   75.0,   75.0,   80.0,  // 1951-1955
   80.0,    80.0,   80.0,  100.0,  100.0,  // 1956-1960
   100.0,  100.0,  100.0,  100.0,  100.0,  // 1961-1965
   125.0,  125.0,  140.0,  140.0,  140.0,  // 1966-1970
   140.0,  140.0,  175.0,  200.0,  210.0,  // 1971-1975
   230.0,  250.0,  270.0,  290.0,  310.0,  // 1976-1980
   340.0,  370.0,  410.0,  430.0,  450.0,  // 1981-1985
   480.0,  500.0,  510.0,  540.0,  570.0,  // 1986-1990
   590.0,  620.0,  640.0,  670.0,  680.0,  // 1991-1995
   690.0,  720.0,  760.0,  800.0,  840.0,  // 1996-2000
   890.0,  940.0,  960.0,  970.0, 1000.0,  // 2001-2005
  1040.0, 1080.0, 1130.0, 1180.0, 1180.0,  // 2006-2010
  1180.0, 1220.0, 1260.0, 1290.0, 1310.0,  // 2011-2015
  1310.0                                   // 2016
};
// <summary>The age 65 monthly exempt amounts, 1951-2016.</summary>
const double Rtea65m::theAmts[] = {
    50.0,   50.0,   75.0,   75.0,   80.0,  // 1951-1955
    80.0,   80.0,   80.0,  100.0,  100.0,  // 1956-1960
   100.0,  100.0,  100.0,  100.0,  100.0,  // 1961-1965
   125.0,  125.0,  140.0,  140.0,  140.0,  // 1966-1970
   140.0,  140.0,  175.0,  200.0,  210.0,  // 1971-1975
   230.0,  250.0, 333.33333, 375.0, 416.66666,  // 1976-1980
   458.33333, 500.0, 550.0, 580.0,  610.0,  // 1981-1985
   650.0,  680.0,  700.0,  740.0,  780.0,  // 1986-1990
   810.0,  850.0,  880.0,  930.0,  940.0,  // 1991-1995
  1041.66666, 1125.0, 1208.33333, 1291.66666, 1416.66666,  // 1996-2000
  2083.33333, 2500.0, 2560.0, 2590.0, 2650.0,  // 2001-2005
  2770.0, 2870.0, 3010.0, 3140.0, 3140.0,  // 2006-2010
  3140.0, 3240.0, 3340.0, 3450.0, 3490.0,  // 2011-2015
  3490.0                                   // 2016
};

// <summary>Exempt amounts, 1961-1972.</summary>
const double RteaHigh::rteaHigh[YEAR72 - YEAR61 + 1] = {
  1500.0, 1700.0, 1700.0, 1700.0, 1700.0, 2700.0,
  2700.0, 2880.0, 2880.0, 2880.0, 2880.0, 2880.0
};

/// <summary>Initializes historical values for monthly test under full
/// retirement age, with first year as 1937.</summary>
///
/// <remarks>Calls <see cref="initialize"/>.</remarks>
///
/// <param name="newLastYear">Last year of projection.</param>
Rtea62m::Rtea62m( int newLastYear ) : RtGeneral(newLastYear)
{
  initialize();
}

/// <summary>Initializes historical values for monthly test under full
/// retirement age, with first year as specified.</summary>
///
/// <remarks>Calls <see cref="initialize"/>.</remarks>
///
/// <param name="newBaseYear">First year of data.</param>
/// <param name="newLastYear">Last year of projection.</param>
Rtea62m::Rtea62m( int newBaseYear, int newLastYear ) :
RtGeneral(newBaseYear, newLastYear)
{
   initialize();
}

/// <summary>Initializes historical data.</summary>
void Rtea62m::initialize()
{
  const int yr1 = max(getBaseYear(), YEAR51);
  const int yr2 = min(getLastYear(), BaseYearNonFile::YEAR);
  for (int yr = yr1; yr <= yr2; yr++) {
    theData[yr - getBaseYear()] = theAmts[yr - YEAR51];
  }
}

/// <summary>Projects retirement test exempt amounts for ages under full
/// retirement age.</summary>
///
/// <param name="firstYear">First year of projection.</param>
/// <param name="averageWages">Average wage series.</param>
/// <param name="cpi">Benefit increase series.</param>
void Rtea62m::project( int firstYear, const DoubleAnnual& averageWages,
const DoubleAnnual& cpi )
{
  int count = 0;  // count of missed increases
  for (int yr = firstYear; yr <= getLastYear(); yr++) {
    const int index = yr - getBaseYear();
    if (cpi[yr - 1] < 0.05) {
      theData[index] = theData[index - 1];
      count++;
    }
    else {
      if (yr >= 1995) {
        theData[index] = project1(theData[1994 - getBaseYear()],
          averageWages[yr - 2], averageWages[1992], theData[index - 1]);
      }
      else {
        theData[index] = project1(theData[index - 1 - count],
          averageWages[yr - 2], averageWages[yr - 3 - count],
          theData[index - 1]);
      }
      count = 0;
    }
  }
}

/// <summary>Returns x in 1-for-x rule for earnings offset.</summary>
///
/// <returns>x in 1-for-x rule for earnings offset.</returns>
///
/// <param name="year">Year for which fraction is desired.</param>
int Rtea62m::getFraction( int year )
{
  if (year < 1961)
    return(1);
  return(2);
}

/// <summary>Initializes historical values for monthly test over full
/// retirement age, with first year as 1937.</summary>
///
/// <remarks>Calls <see cref="initialize"/>.</remarks>
///
/// <param name="newLastYear">Last year of projection.</param>
Rtea65m::Rtea65m( int newLastYear ) : RtGeneral(newLastYear)
{
  initialize();
}

/// <summary>Initializes historical values for monthly test over full
/// retirement age, with first year as specified.</summary>
///
/// <remarks>Calls <see cref="initialize"/>.</remarks>
///
/// <param name="newBaseYear">First year of data.</param>
/// <param name="newLastYear">Last year of projection.</param>
Rtea65m::Rtea65m( int newBaseYear, int newLastYear ) :
RtGeneral(newBaseYear, newLastYear)
{
  initialize();
}

/// <summary>Initializes historical data.</summary>
void Rtea65m::initialize()
{
  const int yr1 = max(getBaseYear(), YEAR51);
  const int yr2 = min(getLastYear(), BaseYearNonFile::YEAR);
  for (int yr = yr1; yr <= yr2; yr++) {
    theData[yr - getBaseYear()] = theAmts[yr - YEAR51];
  }
}

/// <summary>Projects retirement test exempt amounts for ages 65-69.</summary>
///
/// <param name="firstYear">First year of projection.</param>
/// <param name="averageWages">Average wage series.</param>
/// <param name="cpi">Benefit increase series.</param>
void Rtea65m::project( int firstYear, const DoubleAnnual& averageWages,
const DoubleAnnual& cpi )
{
  int count = 0;  // count of missed increases
  for (int yr = firstYear; yr <= getLastYear(); yr++) {
    const int index = yr - getBaseYear();
    if (cpi[yr - 1] < 0.05) {
      theData[index] = theData[index - 1];
      count++;
    }
    else {
      if (yr < 1995) {
        theData[index] = project1(theData[index - 1 - count],
          averageWages[yr - 2], averageWages[yr - 3 - count],
          theData[index - 1]);
      }
      else if (yr == 1995) {
        theData[index] = project1(theData[1994 - getBaseYear()],
          averageWages[yr - 2], averageWages[1992], theData[index - 1]);
      }
      else if (yr >= 1996 && yr <= 2002) {
        theData[index] = theAmts[yr - YEAR51];
      }
      else if (yr > 2002) {
        theData[index] = project1(theData[2002 - getBaseYear()],
          averageWages[yr - 2], averageWages[2000], theData[index - 1]);
      }
      count = 0;
    }
  }
}

/// <summary>Returns x in 1-for-x rule for earnings offset.</summary>
///
/// <returns>x in 1-for-x rule for earnings offset.</returns>
///
/// <param name="year">Year for which fraction is desired.</param>
int Rtea65m::getFraction( int year )
{
  if (year < 1961)
    return(1);
  if (year < 1990)
    return(2);
  return(3);
}

/// <summary>Initializes historical values for annual test at second tier.
/// </summary>
///
/// <remarks>Calls <see cref="initialize"/>.</remarks>
///
/// <remarks>First and last years are 1961 and 1972.</remarks>
RteaHigh::RteaHigh() : RtGeneral(YEAR61, YEAR72)
{
  initialize();
}

/// <summary>Initializes historical data.</summary>
void RteaHigh::initialize()
{
  for (int yr = getBaseYear(); yr <= getLastYear(); yr++) {
    theData[yr - getBaseYear()] = rteaHigh[yr - YEAR61];
  }
}

/// <summary>Returns x in 1-for-x rule for earnings offset.</summary>
///
/// <returns>Always returns 1.</returns>
int RteaHigh::getFraction()
{
  return(1);
}

/// <summary>Initializes arrays starting in 1937.</summary>
///
/// <param name="newLastYear">Last year of projection.</param>
RtGeneral::RtGeneral( int newLastYear ) : DoubleAnnual(YEAR37, newLastYear)
{ }

/// <summary>Initializes arrays starting in specified year.</summary>
///
/// <param name="newBaseYear">First year of data.</param>
/// <param name="newLastYear">Last year of projection.</param>
RtGeneral::RtGeneral( int newBaseYear, int newLastYear ) :
DoubleAnnual(newBaseYear, newLastYear)
{ }

/// <summary>Destructor.</summary>
RtGeneral::~RtGeneral()
{ }

/// <summary>Calculates a monthly retirement test exempt amount using a base
/// amount and a ratio of average wages.</summary>
///
/// <returns>The new monthly retirement test exempt amount.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_RTGENRL"/> if denominator is out of range
/// (only in debug mode).</exception>
///
/// <param name="baseAmount">Value of exempt amount in base year.</param>
/// <param name="numerator">Numerator used in projection.</param>
/// <param name="denominator">Denominator used in projection.</param>
/// <param name="previous">Value of exempt amount in previous year.</param>
double RtGeneral::project1( double baseAmount, double numerator,
double denominator, double previous )
{
#ifndef NDEBUG
  if (denominator < 1.0)
    throw PiaException(PIA_IDS_RTGENRL);
#endif
  // exempt10 is exempt amount in multiples of $10
  const int exempt10 = static_cast<int>(
    (((numerator / denominator) * baseAmount) + 4.99) / 10.0);
  // do not allow decrease in exempt amount
  return(max(static_cast<double>(exempt10) * 10.0, previous));
}

/// <summary>Converts monthly retirement test exempt amounts to annual
/// amounts.</summary>
///
/// <param name="rteam">Monthly retirement test exempt amounts.</param>
void RtGeneral::convertMonthToAnnual( const RtGeneral& rteam )
{
  for (int yr = getBaseYear(); yr <= getLastYear(); yr++) {
    theData[yr - getBaseYear()] = rteam[yr] * 12.0;
  }
}

/// <summary>Returns maximum age for which earnings test applies.</summary>
///
/// <returns>Maximum age for which earnings test applies.</returns>
///
/// <param name="year">Year for which age is desired.</param>
Age RtGeneral::getMaxAgePL( int year )
{
  if (year < 1955)
    return PiaParams::age75;
  if (year < 1983)
    return PiaParams::age72;
  if (year < 2000)
    return PiaParams::age70;
  return PiaParams::maxDibAgePL(year);
}

/// <summary>Initializes first year of data to 1937.</summary>
///
/// <param name="newLastYear">Last year of projection.</param>
Rtea62::Rtea62( int newLastYear ) : RtGeneral(newLastYear)
{  }

/// <summary>Initializes first year of data to specified value.</summary>
///
/// <param name="newBaseYear">First year of data.</param>
/// <param name="newLastYear">Last year of projection.</param>
Rtea62::Rtea62( int newBaseYear, int newLastYear ) :
RtGeneral(newBaseYear, newLastYear)
{  }

/// <summary>Initializes first year of data to 1937.</summary>
///
/// <param name="newLastYear">Last year of projection.</param>
Rtea65::Rtea65( int newLastYear ) : RtGeneral(newLastYear)
{  }

/// <summary>Initializes first year of data to specified value.</summary>
///
/// <param name="newBaseYear">First year of data.</param>
/// <param name="newLastYear">Last year of projection.</param>
Rtea65::Rtea65( int newBaseYear, int newLastYear ) :
RtGeneral(newBaseYear, newLastYear)
{  }
