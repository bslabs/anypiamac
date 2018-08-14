// Functions for the <see cref="AwincNonFile"/> class to manage average
// wage increases stored in static arrays.

// $Id: awincnf.cpp 1.77 2018/06/28 10:32:28EDT 277133 Development  $

#include <algorithm>
#include "AwincNonFile.h"
#include "avgwg.h"
#include "Resource.h"
#include "AssumptionType.h"
#include "BaseYearNonFile.h"
#include "oactcnst.h"

using namespace std;

// <remarks>Historical average wage increases, 1978-2016.
// When updating in November, add a value to this array.</remarks>
const double AwincNonFile::awincHist[] = {
                         7.941048,  8.747891,  9.007392,
  10.066281,  5.505224,  4.871540,  5.878443,  4.260533,
   2.968106,  6.377448,  4.925132,  3.959390,  4.619158,
   3.726559,  5.152396,  0.860023,  2.683910,  4.008373,
   4.890539,  5.835092,  5.233866,  5.572834,  5.529993,
   2.385645,  1.002888,  2.444538,  4.648767,  3.659027,
   4.596306,  4.538179,  2.300406, -1.508069,  2.363503,
   3.133333,  3.122550,  1.278133,  3.549622,  3.479039,
   1.130011
};

// <remarks>Titles for 2018 Trustees Report.</remarks>
const std::string AwincNonFile::titletrOriginal[NUMASSUMSETS] = {
  "2018 Trustees Report Alternative I",
  "2018 Trustees Report Alternative II",
  "2018 Trustees Report Alternative III",
  "No increase beyond 2016 average wage"
};

// <remarks>Average wage increases for 2018 Trustees Report,
//  years 2017-2032.</remarks>
const double AwincNonFile::awincprojtrOriginal[NUMASSUMSETS][NUMPROJYEARS] = {
  {           2.856884, 4.752198, 6.255537, 6.296018,
    5.993629, 5.498352, 5.352487, 5.317564, 5.377918,
    5.406575, 5.221944, 5.154130, 5.132409, 5.125221,
    5.132895, 5.135903
	},
  {           2.834044, 3.746010, 4.205554, 4.543972,
    4.519957, 4.427827, 4.375743, 4.324569, 4.265333,
    4.206222, 3.978307, 3.903107, 3.885128, 3.879770,
    3.888378, 3.892621			
	},
  {           2.824073, 2.978696, 0.455678, 2.905817,
    3.287554, 3.496874, 3.458120, 3.272107, 3.262212,
    3.238793, 2.882875, 2.662887, 2.643352, 2.640940,
    2.651002, 2.657093			
	},
  {      0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0 }
};

// <summary>Ultimate average wage increases for 2018 Trustees Report.</summary>
const double AwincNonFile::awincult[NUMASSUMSETS] = {
   5.0, 3.8, 2.6, 0.0
};

/// <summary>Initializes projected average wage increase info with latest
/// Trustees Report assumptions.</summary>
///
/// <param name="newIstart">Starting year of average wage increases.</param>
/// <param name="newMaxyear">Maximum projected year.</param>
AwincNonFile::AwincNonFile( int newIstart, int newMaxyear ) :
AwincDoc(newIstart, newMaxyear)
{
  // Set last year of projected average wage increases, inclusive of
  // YEAR - 1.
  const int LASTYEAR = BaseYearNonFile::YEAR + NUMPROJYEARS - 2;
  // ensure bounds are great enough to hold data</summary>
  const int YEAR78 = YEAR79 - 1;
  const int yr1 = min(YEAR78, newIstart);
  const int yr2 = max(LASTYEAR, newMaxyear);
  for (int i = 0; i < NUMASSUMSETS; i++) {
    awincprojtr[i] = new DoubleAnnual(yr1, yr2);
    DoubleAnnual& temp = *awincprojtr[i];
    // Set historical data
    for (int year = YEAR78; year < BaseYearNonFile::YEAR - 1; year++) {
      temp[year] = awincHist[year - YEAR78];
    }
    // set title and projected values
    read(i+1);
  }
}

/// <summary>Destructor.</summary>
///
/// <remarks>Deletes the vectors of stored Trustees Report assumptions.
/// </remarks>
AwincNonFile::~AwincNonFile()
{
  for (int i = 0; i < NUMASSUMSETS; i++) {
    delete awincprojtr[i];
  }
}

/// <summary>Restore values to original values.</summary>
///
/// <exception cref="PiaException">Thrown if there is any error restoring
/// values to original values.</exception>
/// <param name="altNum">Desired alternative number (1-4).</param>
void AwincNonFile::read( int altNum )
{
  try {
    // set title
    titletr[altNum - 1] = titletrOriginal[altNum - 1];
    // Set last year of projected average wage increases, inclusive of YEAR
    const int LASTYEAR = BaseYearNonFile::YEAR + NUMPROJYEARS - 2;
    // set values in short-range
    DoubleAnnual& temp = *awincprojtr[altNum - 1];
    for (int year = BaseYearNonFile::YEAR - 1; year <= LASTYEAR; year++) {
      temp[year] =
        awincprojtrOriginal[altNum - 1][year - BaseYearNonFile::YEAR + 1];
    }
    // set values in long-range
    for (int year = LASTYEAR + 1; year <= temp.getLastYear(); year++) {
      temp[year] = awincult[altNum - 1];
    }
  } catch (PiaException& e) {
    throw PiaException(string("In AwincNonFile::read: ") + e.what());
  }
}

/// <summary>Dummy function.</summary>
void AwincNonFile::write( int )
{ }

/// <summary>Sets data for one alternative, using the stored data for the
/// specified alternative.</summary>
///
/// <param name="altNum">Desired alternative number (1-7).</param>
void AwincNonFile::setData( int altNum )
{
  const int i1 = max(getFirstYear(), awinc.getBaseYear());
  if (AssumptionType::isTR(altNum)) {
    awinc.assign(*awincprojtr[altNum - 1], i1, awinc.getLastYear());
  }
  else {
    awinc.assign(0.0, i1, awinc.getLastYear());
  }
}

/// <summary>Sets data for one alternative, storing the data for the specified
/// alternative, then setting the current data to that data.</summary>
///
/// <param name="altNum">Desired alternative number (1-7).</param>
/// <param name="newData">Data to use.</param>
void AwincNonFile::setData( int altNum, const DoubleAnnual& newData )
{
  if (AssumptionType::isTR(altNum)) {
    awincprojtr[altNum - 1]->assign(newData, getFirstYear(),
      awincprojtr[altNum - 1]->getLastYear());
  }
  setData(altNum);
}

/// <summary>Sets the current title, using the title stored for the specified
/// alternative.</summary>
///
/// <param name="altNum">Desired alternative number (1-7).</param>
void AwincNonFile::setTitle( int altNum )
{
  if (AssumptionType::isNotTR(altNum)) {
    awincTitle = avgwage[altNum - AssumptionType::FLAT];
  }
  else {
    awincTitle = titletr[altNum - 1];
  }
}

/// <summary>Sets the current title, storing the title for the specified
/// alternative, then setting the current title to that title.</summary>
///
/// <param name="altNum">Desired alternative number (1-7).</param>
/// <param name="newTitle">Title to use.</param>
void AwincNonFile::setTitle( int altNum, const std::string& newTitle )
{
  if (AssumptionType::isNotTR(altNum)) {
    awincTitle = avgwage[altNum - AssumptionType::FLAT];
  }
  else {
    titletr[altNum - 1] = newTitle;
    awincTitle = titletr[altNum - 1];
  }
}
