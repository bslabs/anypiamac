// Functions for the <see cref="BaseYearNonFile"/> class to manage the
// computation year stored internally.
//
// $Id: baseyrnf.cpp 1.20 2011/07/28 11:50:13EDT 044579 Development  $

#include "BaseYearNonFile.h"

// Define BaseYearNonFile::YEAR, gcc needs it defined in a .cp file to stop undefined symbol errors
const int BaseYearNonFile::YEAR;

/// <summary>Initializes base year.</summary>
BaseYearNonFile::BaseYearNonFile() : BaseYear()
{
  read();
}

/// <summary>Reads stored base year.</summary>
void BaseYearNonFile::read()
{
  theYear = YEAR;
}

/// <summary>Saves base year (dummy function).</summary>
void BaseYearNonFile::write()
{ }
