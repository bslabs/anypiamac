// Functions for the <see cref="Qc"/> class to manage one annual quarters
// of coverage number.
//
// $Id: qc.cpp 1.28 2017/05/12 10:21:38EDT 277133 Development  $

#include <algorithm>
#include <cmath>  // for floor
#include "qc.h"
#include "oactcnst.h"
#include "PiaException.h"
#include "Resource.h"

using namespace std;

/// <summary>The maximum number of qcs in one year (4).</summary>
const unsigned int Qc::MAXQCSINYEAR;

/// <summary>Checks annual quarters of coverage for one year.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCANN"/> if number of quarters of coverage is greater
/// than <see cref="Qc::MAXQCSINYEAR"/>.</exception>
///
/// <param name="newNumber">Value to check.</param>
void Qc::check( unsigned newNumber )
{
  if (newNumber > MAXQCSINYEAR)
    throw PiaException(PIA_IDS_QCANN);
}

/// <summary>Returns number of quarters of coverage for one year, applying
/// annual test.</summary>
///
/// <returns>Number of quarters of coverage for one year. The number cannot
/// exceed <see cref="Qc::MAXQCSINYEAR"/>.</returns>
///
/// <param name="earnings">The earnings in the year.</param>
/// <param name="qcAmount">Amount required for one quarter of coverage.
/// </param>
unsigned Qc::qcCal( double earnings, double qcAmount )
{
  return(min(MAXQCSINYEAR,
    static_cast<unsigned>(floor(earnings / qcAmount))));
}
