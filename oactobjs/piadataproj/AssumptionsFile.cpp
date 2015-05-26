// Functions for the <see cref="AssumptionsFile"/> class that reads the
// average wage and benefit increase parameters from disk storage.

// $Id: AssumptionsFile.cpp 1.23 2011/07/28 08:55:09EDT 044579 Development  $

#include "AssumptionsFile.h"
#include "oactcnst.h"
#include "baseyear.h"

using namespace std;

/// <summary>Initializes arrays.</summary>
///
/// <param name="newIstart">Year after last known benefit increase.</param>
/// <param name="newMaxyear">Maximum projected year.</param>
/// <param name="newDirectory">Directory with conversion table file.</param>
AssumptionsFile::AssumptionsFile( int newIstart, int newMaxyear,
const std::string& newDirectory ) :
Assumptions(newIstart, newMaxyear), awincProj(1978, newMaxyear, newDirectory),
biProj(YEAR79, newMaxyear, newDirectory), catchupDoc(newIstart, newDirectory)
{
  setIstart(newIstart);
}

/// <summary>Destructor.</summary>
AssumptionsFile::~AssumptionsFile()
{ }

/// <summary>Reads stored benefit increases from disk.</summary>
///
/// <param name="altNum">Desired alternative (1-4).</param>
void AssumptionsFile::benefitIncMenu( int altNum )
{
  biProj.read(altNum);
  catchupDoc.read(altNum);
}

/// <summary>Reads stored average wage increases from disk.</summary>
///
/// <param name="altNum">Desired alternative (1-4).</param>
void AssumptionsFile::averageWageMenu( int altNum )
{
  awincProj.read(altNum);
}

/// <summary>Sets the current year, including for the members.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> if year is out
/// of range (only in debug mode).</exception>
///
/// <param name="newIstart">New current year.</param>
void AssumptionsFile::setIstart( int newIstart )
{
#ifndef NDEBUG
  BaseYear::yearCheck(newIstart);
#endif
  Assumptions::setIstart(newIstart);
  catchupDoc.catchup.setCstart(newIstart);
  biProj.setFirstYear(newIstart);
  awincProj.setFirstYear(newIstart - 1);
}
