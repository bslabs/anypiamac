// Functions for the <see cref="BenefitAmountArray"/> class of an
// array of Social Security benefit amounts.

// $Id: BenefitAmountArray.cpp 1.17 2011/07/28 15:19:54EDT 044579 Development  $

#include "BenefitAmountArray.h"

using namespace std;

/// <summary>Initializes the last year to 0 and the data array to zero size.
/// </summary>
///
/// <remarks>The first year is set to 1937, but the last year must be set via
/// the <see cref="initialize"/> function before this instance can be used.
/// </remarks>
BenefitAmountArray::BenefitAmountArray() : theData(), lastYear(0),
  dirty(false)
{ }

/// <summary>Initializes the last year to the specified value.</summary>
///
/// <remarks>Resizes the data array to cover the span of years from 1937 to
/// the specified year.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BENARRAY1"/> (in debug version) if year is out of
/// range.</exception>
///
/// <param name="year">New last year.</param>
BenefitAmountArray::BenefitAmountArray( int year ) :
theData(year - YEAR37 + 1), lastYear(year), dirty(false)
{
#ifndef NDEBUG
  if (year < YEAR37)
    throw PiaException(PIA_IDS_BENARRAY1);
#endif
}

/// <summary>Deletes allocated memory.</summary>
BenefitAmountArray::~BenefitAmountArray()
{ }

/// <summary>Initializes last year.</summary>
///
/// <remarks>Resizes the data array to cover the span of years
/// from 1937 to the specified year.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BENARRAY1"/> (in debug version) if year
/// is out of range.</exception>
///
/// <param name="newLastYear">New last year.</param>
void BenefitAmountArray::initialize( int newLastYear )
{
#ifndef NDEBUG
  if (newLastYear < YEAR37)
    throw PiaException(PIA_IDS_BENARRAY1);
#endif
  theData.resize(newLastYear - YEAR37 + 1);
  lastYear = newLastYear;
}

/// <summary>Sets all values to zero.</summary>
void BenefitAmountArray::deleteContents()
{
  if (dirty) {
    for (int i = YEAR37; i <= lastYear; i++)
      theData[i - YEAR37].set(0.0);
    dirty = false;
  }
}
