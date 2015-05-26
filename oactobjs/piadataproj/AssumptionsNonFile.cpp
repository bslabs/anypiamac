// Functions for the <see cref="AssumptionsNonFile"/> class that contains
// the average wage and benefit increase parameters (it does not read from
// disk).
//
// $Id: AssumptionsNonFile.cpp 1.21 2011/07/28 08:55:08EDT 044579 Development  $

#include "AssumptionsNonFile.h"
#include "oactcnst.h"
#include "BaseYearNonFile.h"

using namespace std;

/// <summary>Initializes years, where the year after last known benefit
/// increase is specified.</summary>
///
/// <param name="newIstart">Year after last known benefit increase.</param>
/// <param name="newMaxyear">Maximum projected year</param>
AssumptionsNonFile::AssumptionsNonFile( int newIstart, int newMaxyear ) :
Assumptions(newIstart, newMaxyear), awincProj(1978, newMaxyear),
biProj(YEAR79, newMaxyear), catchupDoc(newIstart)
{
  setIstart(newIstart);
}

/// <summary>Initializes years, where the year after last known benefit
/// increase is taken from <see cref="BaseYearNonFile"/>.</summary>
//
/// <param name="newMaxyear">Maximum projected year</param>
AssumptionsNonFile::AssumptionsNonFile( int newMaxyear ) :
Assumptions(newMaxyear), awincProj(1978, newMaxyear),
biProj(YEAR79, newMaxyear), catchupDoc(BaseYearNonFile::YEAR)
{
  setIstart(BaseYearNonFile::YEAR);
}

/// <summary>Destructor.</summary>
AssumptionsNonFile::~AssumptionsNonFile()
{ }

/// <summary>Reads stored benefit increases from a static array.</summary>
///
/// <param name="altNum">Desired alternative (1-4).</param>
void AssumptionsNonFile::benefitIncMenu( int altNum )
{
  // set benefit increases
  biProj.setData(altNum);
  biProj.setTitle(altNum);
  // set catch-up increases
  catchupDoc.setData(altNum);
}

/// <summary>Reads stored average wage increases from a static array.
/// </summary>
///
/// <param name="altNum">Desired alternative (1-4).</param>
void AssumptionsNonFile::averageWageMenu( int altNum )
{
  // set average wage increases
  awincProj.setData(altNum);
  awincProj.setTitle(altNum);
}

/// <summary>Sets the current year, including for the members.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> if year is out
/// of range (only in debug mode).</exception>
///
/// <param name="newIstart">New current year.</param>
void AssumptionsNonFile::setIstart( int newIstart )
{
#ifndef NDEBUG
  BaseYear::yearCheck(newIstart);
#endif
  Assumptions::setIstart(newIstart);
  catchupDoc.catchup.setCstart(newIstart);
  biProj.setFirstYear(newIstart);
  awincProj.setFirstYear(newIstart - 1);
}
