// Functions needed for <see cref="AverageWage"/> class to manage array of average wages.

// $Id: avgwg.cpp 1.27 2010/11/22 16:15:23EST 044579 Development  $

#include "avgwg.h"
#include "awinc.h"
#include "Resource.h"
#include "PiaException.h"

/// <summary>Initializes the first and last years.</summary>
///
/// <param name="newBaseYear">Year that array starts in.</param>
/// <param name="newLastYear">Maximum year of projection.</param>
AverageWage::AverageWage( int newBaseYear, int newLastYear ) :
DoubleAnnual(newBaseYear, newLastYear)
{ }

/// <summary>Destroys average wage array.</summary>
AverageWage::~AverageWage()
{ }

/// <summary>Sets this instance equal to another instance.</summary>
///
/// <returns>A reference to this <see cref="AverageWage"/>.</returns>
///
/// <param name="newData">The instance to copy from.</param>
AverageWage& AverageWage::operator=( const AverageWage& newData )
{
  const DoubleAnnual& da1 = newData;
  this->assign(da1, da1.getBaseYear(), da1.getLastYear());
  return (*this);
}

/// <summary>Projects average wage amounts to the last year of the vector of
/// average wage amounts.</summary>
///
/// <param name="averageWageInc">Average wage percentage increases
/// (e.g. 5.0 for 5 percent increase).</param>
/// <param name="firstYeart">First year to project.</param>
void AverageWage::project( const Awinc& averageWageInc, int firstYeart )
{
  for (int yr = firstYeart; yr <= getLastYear(); yr++) {
    projectForward(yr, averageWageInc[yr] / 100.0 + 1.0);
  }
}

/// <summary>Projects average wage amounts to a specified last year.</summary>
///
/// <param name="firstYeart">First year to project.</param>
/// <param name="factor">Multiplicative factor used to increase each year
/// (e.g. 1.05).</param>
/// <param name="lastYeart">Last year to project to.</param>
void AverageWage::project( double factor, int firstYeart, int lastYeart )
{
  for (int yr = firstYeart; yr <= lastYeart; yr++) {
    projectForward(yr, factor);
  }
}

/// <summary>Projects the earnings amount forward for the specified year.
/// </summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_AVGWG"/> if year is out of range (only in debug mode).
/// </exception>
///
/// <remarks>Calls <see cref="projectForwardStatic"/> with an amount and a
/// factor.</remarks>
///
/// <param name="year">Year for which average wage is desired.</param>
/// <param name="factor">Multiplicative factor used to increase
///  previous year.</param>
void AverageWage::projectForward( int year, double factor )
{
#ifndef NDEBUG
  if (year <= getBaseYear() || year > getLastYear())
    throw PiaException(PIA_IDS_AVGWG);
#endif
  const int index = year - getBaseYear();
  theData[index] = projectForwardStatic(theData[index - 1], factor);
}

/// <summary>Projects the earnings amount backward for the specified year.
/// </summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_AVGWG"/> if year is out of range (only in debug mode).
/// </exception>
///
/// <param name="year">Year for which average wage is desired.</param>
/// <param name="factor">Multiplicative factor used to decrease from
///  following year.</param>
void AverageWage::projectBackward( int year, double factor )
{
#ifndef NDEBUG
  if (year < getBaseYear() || year >= getLastYear())
    throw PiaException(PIA_IDS_AVGWG);
#endif
  const int index = year - getBaseYear();
  theData[index] = projectBackwardStatic(theData[index + 1], factor);
}

/// <summary>Returns the first year that data > 4.5, or returns 0.</summary>
///
/// <returns>The first year that data > 4.5, or returns 0.</returns>
int AverageWage::startYear() const
{
  for (int yr = getBaseYear(); yr <= getLastYear(); yr++) {
    if (theData[yr - getBaseYear()] > 4.5)
      return(yr);
  }
  return(0);
}

/// <summary>Returns the last year that data > 4.5, or returns 0.</summary>
///
/// <returns>The last year that data > 4.5, or returns 0.</returns>
int AverageWage::endYear() const 
{
  for (int yr = getLastYear(); yr >= getBaseYear(); yr--) {
    if (theData[yr - getBaseYear()] > 4.5)
      return(yr);
  }
  return(0);
}
