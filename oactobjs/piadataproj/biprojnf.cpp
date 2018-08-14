// Functions for the <see cref="BiprojNonFile"/> class to manage projected
// benefit increases stored in arrays.
//
// $Id: biprojnf.cpp 1.78 2018/06/28 10:32:30EDT 277133 Development  $

#include <algorithm>
#include "BiprojNonFile.h"
#include "Resource.h"
#include "PiaException.h"
#include "AssumptionType.h"
#include "BaseYearNonFile.h"
#include "AwbiDataNonFile.h"
#include "oactcnst.h"

using namespace std;

// <summary>Titles for 2017 Trustees Report</summary>
const string BiprojNonFile::titletrOriginal[NUMASSUMSETS] = {
  "2018 Trustees Report Alternative I",
  "2018 Trustees Report Alternative II",
  "2018 Trustees Report Alternative III",
  "No increase beginning with 2018 benefit increase"
};

// <remarks>Benefit increases for 2018 Trustees Report, 2018 to 2032</remarks>
const double BiprojNonFile::biprojtrOriginal[NUMASSUMSETS][NUMPROJYEARS] = {
  {           3.0, 3.5, 3.2,
    3.2, 3.2, 3.2, 3.2, 3.2,
    3.2, 3.2, 3.2, 3.2, 3.2,
    3.2, 3.2 },
  {           2.4, 2.7, 2.6,
    2.6, 2.6, 2.6, 2.6, 2.6,
    2.6, 2.6, 2.6, 2.6, 2.6, 
    2.6, 2.6 },
  {           1.6, 1.7, 2.0,
    2.0, 2.0, 2.0, 2.0, 2.0,
    2.0, 2.0, 2.0, 2.0, 2.0, 
    2.0, 2.0 },
  {           0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0 }
};

// <remarks>Ultimate benefit increases for 2017 Trustees Report.</remarks>
const double BiprojNonFile::biult[NUMASSUMSETS] = {
  3.2, 2.6, 2.0, 0.0
};

/// <summary>Initializes projected benefit increase info for latest Trustees
/// Report.</summary>
///
/// <remarks>Creates the arrays of Trustees Report benefit increases.</remarks>
///
/// <param name="newIstart">Starting year of benefit increases.</param>
/// <param name="newMaxyear">Maximum projected year.</param>
BiprojNonFile::BiprojNonFile( int newIstart, int newMaxyear ) :
Biproj(newIstart, newMaxyear)
{
  // Set last year of projected benefit increases, inclusive of YEAR.
  const int LASTYEAR = BaseYearNonFile::YEAR + NUMPROJYEARS - 1;
  // ensure bounds are great enough to hold data
  const int i1 = min(YEAR79, newIstart);
  const int i2 = max(LASTYEAR, newMaxyear);
  for (int i = 0; i < NUMASSUMSETS; i++) {
    biprojtr[i] = new DoubleAnnual(i1, i2);
    DoubleAnnual& temp = *biprojtr[i];
    // Set historical data
    for (int year = YEAR79; year < BaseYearNonFile::YEAR; year++) {
      temp[year] = AwbiDataNonFile::getCpiincData(year);
    }
    // set title and projected values
    read(i+1);
  }
}

/// <summary>Destructor.</summary>
///
/// <remarks>Deletes the arrays of Trustees Report benefit increases.</remarks>
BiprojNonFile::~BiprojNonFile()
{
  for (int i = 0; i < NUMASSUMSETS; i++) {
    delete biprojtr[i];
  }
}

/// <summary>Restore values to original values.</summary>
///
/// <param name="altNum">Desired alternative number (1-4).</param>
void BiprojNonFile::read( int altNum )
{
  try {
    // set title
    titletr[altNum - 1] = titletrOriginal[altNum - 1];
    // Set last year of projected benefit increases, inclusive of YEAR.
    const int LASTYEAR = BaseYearNonFile::YEAR + NUMPROJYEARS - 1;
    // set values in short-range
    DoubleAnnual& temp = *biprojtr[altNum - 1];
    for (int year = BaseYearNonFile::YEAR; year <= LASTYEAR; year++) {
      temp[year] =
        biprojtrOriginal[altNum - 1][year - BaseYearNonFile::YEAR];
    }
    // set values in long-range
    for (int year = LASTYEAR + 1; year <= temp.getLastYear(); year++) {
         temp[year] = biult[altNum - 1];
    }
  } catch (PiaException& e) {
    throw PiaException(string("In BiprojNonFile::read: ") + e.what());
  }
}

/// <summary>Dummy function.</summary>
void BiprojNonFile::write( int )
{ }

/// <summary>Sets data for one alternative.</summary>
///
/// <remarks>This version has one argument uses the data stored
/// for the specified alternative.</remarks>
///
/// <param name="altNum">Desired alternative number (1-7).</param>
void BiprojNonFile::setData( int altNum )
{
  const int i1 = max(getFirstYear(), theData.getBaseYear());
  if (AssumptionType::isTR(altNum)) {
    theData.assign(*biprojtr[altNum - 1], i1, theData.getLastYear());
  }
  else {
    theData.assign(0.0, i1, theData.getLastYear());
  }
}

/// <summary>Sets data for one alternative.</summary>
///
/// <remarks>This version has two arguments stores the data for the 
/// specified alternative, then sets the current data to that data.</remarks>
///
/// <param name="altNum">Desired alternative number (1-7).</param>
/// <param name="newData">Data to use.</param>
void BiprojNonFile::setData( int altNum, const DoubleAnnual& newData )
{
  if (AssumptionType::isTR(altNum)) {
    biprojtr[altNum - 1]->assign(newData, getFirstYear(),
      biprojtr[altNum - 1]->getLastYear());
  }
  setData(altNum);
}

/// <summary>Sets the current title.</summary>
///
/// <remarks>This version has one argument uses the title stored
/// for the specified alternative.</remarks>
///
/// <param name="altNum">Desired alternative number (1-7).</param>
void BiprojNonFile::setTitle( int altNum )
{
  if (AssumptionType::isNotTR(altNum)) {
    biprojTitle = beninc[altNum - AssumptionType::FLAT];
  }
  else {
    biprojTitle = titletr[altNum - 1];
  }
}

/// <summary>Sets the current title.</summary>
///
/// <remarks>This version has two arguments stores the title for
/// the specified alternative, then sets the current title to
/// that title.</remarks>
///
/// <param name="altNum">Desired alternative number (1-7).</param>
/// <param name="newTitle">Title to use.</param>
void BiprojNonFile::setTitle( int altNum, const std::string& newTitle )
{
  if (AssumptionType::isNotTR(altNum)) {
    biprojTitle = beninc[altNum - AssumptionType::FLAT];
  }
  else {
    titletr[altNum - 1] = newTitle;
    biprojTitle = titletr[altNum - 1];
  }
}
