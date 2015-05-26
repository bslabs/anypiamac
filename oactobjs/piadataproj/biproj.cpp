// Functions for the <see cref="Biproj"/> class to manage projected benefit
// increases.

// $Id: biproj.cpp 1.26 2012/03/12 06:37:37EDT 277133 Development  $

#include <sstream>
#include "biproj.h"
#include "Resource.h"
#include "PiaException.h"
#include "baseyear.h"

using namespace std;

const char* Biproj::beninc1 =
  "Old PEBES calculation (no future benefit increases)";
const char* Biproj::otherTitle = "Other";

/// <summary>Initializes projected benefit increase info.</summary>
///
/// <remarks>Resizes the data array to cover the specified span of
///  years.</remarks>
///
/// <param name="newIstart">Starting year of benefit increases.</param>
/// <param name="newMaxyear">Maximum projected year.</param>
Biproj::Biproj( int newIstart, int newMaxyear ) :
firstYear(newIstart), biprojTitle(), theData(newIstart, newMaxyear)
{
  /// <summary>assign titles for non-TR assumptions</summary>
  beninc[1] = beninc1;
  beninc[2] = otherTitle;
}

/// <summary>Destructor.</summary>
Biproj::~Biproj()
{ }

/// <summary>Sets one benefit increase.</summary>
///
/// <param name="year">Year of benefit increase to set.</param>
/// <param name="newCpiinc">Value to use.</param>
void Biproj::setCpiinc( int year, double newCpiinc )
{
  cpiincCheck(newCpiinc);
  theData[year] = newCpiinc;
}

/// <summary>Checks a benefit increase.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_CPIINC1"/> if benefit increase is out of range.
/// </exception>
///
/// <param name="cpiinct">Value to check.</param>
void Biproj::cpiincCheck( double cpiinct )
{
  if (cpiinct > 99.99 || cpiinct < 0.0) {
    throw PiaException(PIA_IDS_CPIINC1);
  }
}

/// <summary>Sets first year of data.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> if year is out
/// of range (only in debug mode).</exception>
///
/// <param name="newFirstYear">New first year of data.</param>
void Biproj::setFirstYear( int newFirstYear )
{
#ifndef NDEBUG
  BaseYear::yearCheck(newFirstYear);
#endif
  firstYear = newFirstYear;
  ostringstream strm;
  strm << "No increase beginning with " << newFirstYear
    << " benefit increase";
  beninc[0] = strm.str();
}
