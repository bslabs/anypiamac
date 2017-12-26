// Functions for the <see cref="ScaledEarnFactors"/> class of scaled
// earnings factors.

// $Id: ScaledEarnFactors.cpp 1.16 2017/08/29 09:01:26EDT 277133 Development  $

#include <iomanip>
#include <fstream>
#include <algorithm>
#include "ScaledEarnFactors.h"
#include "EarnProject.h"
#include "avgwg.h"

using namespace std;

/// <summary>Initializes scaled earnings factors.</summary>
///
/// <remarks>Resizes the internal arrays to newLastAge - newFirstAge + 1.
/// </remarks>
///
/// <param name="newFirstAge">First age of data.</param>
/// <param name="newLastAge">Last age of data.</param>
ScaledEarnFactors::ScaledEarnFactors( int newFirstAge, int newLastAge ) :
highFactors(newLastAge - newFirstAge + 1),
averageFactors(newLastAge - newFirstAge + 1),
lowFactors(newLastAge - newFirstAge + 1),
veryLowFactors(newLastAge - newFirstAge + 1),
fieldWidth(10), numDecimals(6),
firstAge(newFirstAge), lastAge(newLastAge)
{
  highFactors.assign(lastAge - firstAge + 1, 0.0);
  averageFactors.assign(lastAge - firstAge + 1, 0.0);
  lowFactors.assign(lastAge - firstAge + 1, 0.0);
  veryLowFactors.assign(lastAge - firstAge + 1, 0.0);
}

/// <summary>Destructor.</summary>
ScaledEarnFactors::~ScaledEarnFactors()
{ }

/// <summary>Returns high earner factor for an age.</summary>
///
/// <returns>High earner factor for an age.</returns>
///
/// <param name="theAge">The age for which factor is desired.</param>
double ScaledEarnFactors::getHighEarnFactor( int theAge ) const
{
  return (theAge >= firstAge && theAge <= lastAge) ?
    highFactors[theAge - firstAge] : 0.0;
}

/// <summary>Returns average earner factor for an age.</summary>
///
/// <returns>Average earner factor for an age.</returns>
///
/// <param name="theAge">The age for which factor is desired.</param>
double ScaledEarnFactors::getAverageEarnFactor( int theAge ) const
{
  return (theAge >= firstAge && theAge <= lastAge) ?
    averageFactors[theAge - firstAge] : 0.0;
}

/// <summary>Returns low earner factor for an age.</summary>
///
/// <returns>Low earner factor for an age.</returns>
///
/// <param name="theAge">The age for which factor is desired.</param>
double ScaledEarnFactors::getLowEarnFactor( int theAge ) const
{
  return (theAge >= firstAge && theAge <= lastAge) ?
    lowFactors[theAge - firstAge] : 0.0;
}

/// <summary>Returns very low earner factor for an age.</summary>
///
/// <returns>Very low earner factor for an age.</returns>
///
/// <param name="theAge">The age for which factor is desired.</param>
double ScaledEarnFactors::getVeryLowEarnFactor( int theAge ) const
{
  return (theAge >= firstAge && theAge <= lastAge) ?
    veryLowFactors[theAge - firstAge] : 0.0;
}

/// <summary>Returns a scaled earner factor for an age.</summary>
///
/// <returns>Scaled earner factor for an age.</returns>
///
/// <param name="theAge">The age for which factor is desired.</param>
/// <param name="earnType">The type of scaled earnings which is desired.</param>
double ScaledEarnFactors::getScaledEarnFactor( int theAge, scaleType earnType ) const
{
  double factor = 0.0;
  if (theAge >= firstAge && theAge <= lastAge) {
    switch (earnType){
    case SCALEVERYLOW: 
      factor = veryLowFactors[theAge - firstAge];
      break;
    case SCALELOW: 
      factor = lowFactors[theAge - firstAge];
      break;
    case SCALEAVERAGE: 
      factor = averageFactors[theAge - firstAge];
      break;
    case SCALEHIGH: 
      factor = highFactors[theAge - firstAge];
      break;
    default:
      break;
    }
  }
  return factor;
}

/// <summary>Sets earnings in an <see cref="EarnProject"/> to high earnings
/// between two ages.</summary>
///
/// <remarks>For each year, sets earnings equal to highFactors times the
/// average wage.</remarks>
///
/// <param name="earnProject">Place to set earnings.</param>
/// <param name="averageWage">The average wage index.</param>
/// <param name="birthYear">Worker's year of birth.</param>
/// <param name="ageStart">First year of high earnings.</param>
/// <param name="ageEnd">Last year of high earnings.</param>
void ScaledEarnFactors::setHighEarnings( EarnProject& earnProject,
const AverageWage& averageWage, int birthYear, int ageStart, int ageEnd )
{
  const int startYear =
    max(earnProject.getFirstYear(), birthYear + ageStart);
  const int endYear = min(earnProject.getLastYear(), birthYear + ageEnd);
  for (int yr = startYear; yr <= endYear; yr++) {
    earnProject.setEarnpebs(yr,
      getHighEarnFactor(yr - birthYear) * averageWage[yr]);
   }
}

/// <summary>Sets earnings in an <see cref="EarnProject"/> to average earnings
/// between two ages.</summary>
///
/// <remarks>For each year, sets earnings equal to averageFactors
/// times the average wage.</remarks>
///
/// <param name="earnProject">Place to set earnings.</param>
/// <param name="averageWage">The average wage index.</param>
/// <param name="birthYear">Worker's year of birth.</param>
/// <param name="ageStart">First year of average earnings.</param>
/// <param name="ageEnd">Last year of average earnings.</param>
void ScaledEarnFactors::setAverageEarnings( EarnProject& earnProject,
const AverageWage& averageWage, int birthYear, int ageStart, int ageEnd )
{
  const int startYear =
    max(earnProject.getFirstYear(), birthYear + ageStart);
  const int endYear = min(earnProject.getLastYear(), birthYear + ageEnd);
  for (int yr = startYear; yr <= endYear; yr++) {
    earnProject.setEarnpebs(yr,
      getAverageEarnFactor(yr - birthYear) * averageWage[yr]);
  }
}

/// <summary>Sets earnings in an <see cref="EarnProject"/> to low earnings
/// between two ages.</summary>
///
/// <remarks>For each year, sets earnings equal to lowFactors times
/// the average wage.</remarks>
///
/// <param name="earnProject">Place to set earnings.</param>
/// <param name="averageWage">The average wage index.</param>
/// <param name="birthYear">Worker's year of birth.</param>
/// <param name="ageStart">First year of low earnings.</param>
/// <param name="ageEnd">Last year of low earnings.</param>
void ScaledEarnFactors::setLowEarnings( EarnProject& earnProject,
const AverageWage& averageWage, int birthYear, int ageStart, int ageEnd )
{
  const int startYear =
    max(earnProject.getFirstYear(), birthYear + ageStart);
  const int endYear = min(earnProject.getLastYear(), birthYear + ageEnd);
  for (int yr = startYear; yr <= endYear; yr++) {
    earnProject.setEarnpebs(yr,
      getLowEarnFactor(yr - birthYear) * averageWage[yr]);
  }
}

/// <summary>Sets earnings in an <see cref="EarnProject"/> to very low earnings
/// between two ages.</summary>
///
/// <remarks>For each year, sets earnings equal to veryLowFactors times
/// the average wage.</remarks>
///
/// <param name="earnProject">Place to set earnings.</param>
/// <param name="averageWage">The average wage index.</param>
/// <param name="birthYear">Worker's year of birth.</param>
/// <param name="ageStart">First year of low earnings.</param>
/// <param name="ageEnd">Last year of low earnings.</param>
void ScaledEarnFactors::setVeryLowEarnings( EarnProject& earnProject,
const AverageWage& averageWage, int birthYear, int ageStart, int ageEnd )
{
  const int startYear =
    max(earnProject.getFirstYear(), birthYear + ageStart);
  const int endYear = min(earnProject.getLastYear(), birthYear + ageEnd);
  for (int yr = startYear; yr <= endYear; yr++) {
    earnProject.setEarnpebs(yr,
      getVeryLowEarnFactor(yr - birthYear) * averageWage[yr]);
  }
}

/// <summary>Sets earnings in an <see cref="EarnProject"/> to a particular 
/// type of scaled earnings between two ages.</summary>
///
/// <remarks>For each year, sets earnings equal to a scaled factor times
/// the average wage.</remarks>
///
/// <param name="earnProject">Place to set earnings.</param>
/// <param name="averageWage">The average wage index.</param>
/// <param name="birthYear">Worker's year of birth.</param>
/// <param name="ageStart">First year of low earnings.</param>
/// <param name="ageEnd">Last year of low earnings.</param>
/// <param name="earnType">Type of scaled earnings.</param>
void ScaledEarnFactors::setScaledEarnings( EarnProject& earnProject,
const AverageWage& averageWage, int birthYear, int ageStart, int ageEnd, scaleType earnType )
{
  const int startYear =
    max(earnProject.getFirstYear(), birthYear + ageStart);
  const int endYear = min(earnProject.getLastYear(), birthYear + ageEnd);
  for (int yr = startYear; yr <= endYear; yr++) {
    earnProject.setEarnpebs(yr,
      getScaledEarnFactor(yr - birthYear, earnType) * averageWage[yr]);
  }
}

/// <summary>Reads scaled earnings factors from file.</summary>
///
/// <param name="in">Input file. (It must already be open.)</param>
/// <param name="ageStart">The age to begin reading from file.</param>
/// <param name="ageEnd">The age to end reading from file.</param>
/// <param name="desc">Boolean for skipping the first line in the file.</param>
void ScaledEarnFactors::read( std::istream& in, int ageStart, int ageEnd, bool desc )
{
  int temp;
  double tempFactors[4];
  string firstLine;
  // Skip first line if it is a label line.
  if (desc) {
    getline(in, firstLine);
  }
  for (int age = ageStart; age <= ageEnd; age++) {
    // Ignore age at beginning of line.
    in >> temp;
    in >> tempFactors[SCALEVERYLOW] >> tempFactors[SCALELOW] 
       >> tempFactors[SCALEAVERAGE] >> tempFactors[SCALEHIGH];
    if (age >= firstAge && age <= lastAge) {
      veryLowFactors[age - firstAge] = tempFactors[SCALEVERYLOW];
      lowFactors[age - firstAge] = tempFactors[SCALELOW];
      averageFactors[age - firstAge] = tempFactors[SCALEAVERAGE];
      highFactors[age - firstAge] = tempFactors[SCALEHIGH];
    }
  }
}

/// <summary>Writes scaled earnings factors to file.</summary>
///
/// <param name="out">Output file. (It must already be open.)</param>
/// <param name="ageStart">The age to begin writing to file.</param>
/// <param name="ageEnd">The age to end writing to file.</param>
void ScaledEarnFactors::write( std::ostream& out, int ageStart, int ageEnd )
{
  const int age1 = max(ageStart, firstAge);
  const int age2 = min(ageEnd, lastAge);
  out.precision(numDecimals);
  for (int age = age1; age <= age2; age++) {
    out << age << setw(fieldWidth) << veryLowFactors[age - firstAge]
      << setw(fieldWidth) << lowFactors[age - firstAge]
      << setw(fieldWidth) << averageFactors[age - firstAge]
      << setw(fieldWidth) << highFactors[age - firstAge] << endl;
  }
}
