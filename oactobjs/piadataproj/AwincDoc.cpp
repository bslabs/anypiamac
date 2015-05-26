// Functions for the <see cref="AwincDoc"/> class to manage average wage
// increases.

// $Id: AwincDoc.cpp 1.18 2012/03/12 06:37:37EDT 277133 Development  $

#include <sstream>
#include "AwincDoc.h"
#include "avgwg.h"
#include "Resource.h"
#include "oactcnst.h"
#include "PiaException.h"
#include "baseyear.h"

using namespace std;

const char* AwincDoc::otherTitle = "Other";
const char* AwincDoc::avgwage1 =
  "Old PEBES calculation (1-percent real wage increases)";

/// <summary>Initializes projected average wage increase info.</summary>
///
/// <param name="newIstart">Starting year of average wage increases.</param>
/// <param name="newMaxyear">Maximum projected year.</param>
AwincDoc::AwincDoc( int newIstart, int newMaxyear ) :
firstYear(newIstart), awincTitle(), awinc(newMaxyear)
{
  // assign titles for non-TR assumptions
  avgwage[1] = avgwage1;
  avgwage[2] = otherTitle;
}

/// <summary>Destructor.</summary>
AwincDoc::~AwincDoc()
{ }

/// <summary>Sets average wage increase for specified year.</summary>
///
/// <param name="year">Year of average wage increase to set.</param>
/// <param name="newFqinc">New average wage increase.</param>
void AwincDoc::setFqinc( int year, double newFqinc )
{
  Awinc::fqincCheck(newFqinc);
  awinc[year] = newFqinc;
}

/// <summary>Sets first year of data.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> if year is out
/// of range (only in debug mode).</exception>
///
/// <param name="newFirstYear">New first year of data.</param>
void AwincDoc::setFirstYear( int newFirstYear )
{
#ifndef NDEBUG
  BaseYear::yearCheck(newFirstYear);
#endif
  firstYear = newFirstYear;
  ostringstream strm;
  strm << "No increase beyond " << (newFirstYear - 1)
    << " average wage";
  avgwage[0] = strm.str();
}
