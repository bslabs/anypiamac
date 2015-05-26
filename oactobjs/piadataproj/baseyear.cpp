// Functions for the <see cref="BaseYear"/> class - parent of classes that
// manage the computation year for a pia program.
//
// $Id: baseyear.cpp 1.18 2011/07/28 11:50:15EDT 044579 Development  $

#include "baseyear.h"
#include "Resource.h"
#include "PiaException.h"
#include "oactcnst.h"

using namespace std;

/// <summary>Initializes base year to zero.</summary>
BaseYear::BaseYear() : theYear(0)
{ }

/// <summary>Destroys base year info.</summary>
BaseYear::~BaseYear()
{ }

/// <summary>Checks base year.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BASEYR1"/> if base year is out of range (1979-2025).
/// When the last short-range year is updated, the string tables for anypia
/// need to be changed to reflect the new maximum year.</exception>
///
/// <param name="newYear">New value of base year.</param>
void BaseYear::yearCheck( int newYear )
{
  if (newYear < YEAR79 || newYear > LASTYRSR)
    throw PiaException(PIA_IDS_BASEYR1);
}
