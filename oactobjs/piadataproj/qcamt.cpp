// Functions for the <see cref="Qcamt"/> class to manage array of amounts
// required for quarter of coverage.
//
// $Id: qcamt.cpp 1.22 2011/08/09 14:59:55EDT 044579 Development  $

#include <string>
#include "qcamt.h"
#include "PiaException.h"
#include "oactcnst.h"

using namespace std;

// <summary>The year when automatic increases started for the quarter
// of coverage amount.</summary>
const int Qcamt::AUTO_YEAR;
// <summary>The quarter of coverage amount in 1978.</summary>
const double Qcamt::qcamt1978 = 250.0;
// <summary>The quarter of coverage amount prior to 1978.</summary>
const double Qcamt::qcamt1977 = 50.0;

/// <summary>Initializes with base year 1937.</summary>
///
/// <param name="newLastYear">New last year of data.</param>
Qcamt::Qcamt( int newLastYear ) : DoubleAnnual(YEAR37, newLastYear)
{
  assign(qcamt1977,getBaseYear(), AUTO_YEAR - 1);
  theData[AUTO_YEAR - getBaseYear()] = qcamt1978;
}

/// <summary>Initializes with the data starting in a specified year.</summary>
///
/// <param name="newLastYear">New last year of data.</param>
/// <param name="newBaseYear">New base year of data.</param>
Qcamt::Qcamt( int newBaseYear, int newLastYear ) :
DoubleAnnual(newBaseYear, newLastYear)
{
  assign(qcamt1977, getBaseYear(), AUTO_YEAR- 1);
  theData[AUTO_YEAR - getBaseYear()] = qcamt1978;
}

/// <summary>Destructor.</summary>
Qcamt::~Qcamt()
{ }

/// <summary>Projects quarter of coverage amounts.</summary>
///
/// <remarks>Projects beginning maximum of specified year or 1978, and
/// continues to last year of data.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCAMT"/> if average wages have not been set (only in
/// debug mode).</exception>
///
/// <param name="averageWages">Average wages to use in projection.</param>
/// <param name="firstYeart">First year of projection.</param>
void Qcamt::project( const DoubleAnnual& averageWages, int firstYeart )
{
  // do not start projecting before 1978
  const int year78 = max(AUTO_YEAR, firstYeart);
  for (int yr = year78; yr <= getLastYear(); yr++) {
#if !defined(NDEBUG)
    if (averageWages[AUTO_YEAR - 2] < 1.0)
      throw PiaException(PIA_IDS_QCAMT);
#endif
    const double factor = averageWages[yr - 2] / averageWages[AUTO_YEAR - 2];
    const int k = static_cast<int>
      (((factor * theData[AUTO_YEAR - getBaseYear()]) + 4.99) / 10.0);
    theData[yr - getBaseYear()] = static_cast<double>(k) * 10.0;
    if (theData[yr-getBaseYear()] < theData[yr-1-getBaseYear()]) {
      theData[yr-getBaseYear()] = theData[yr-1-getBaseYear()];
    }
  }
}
