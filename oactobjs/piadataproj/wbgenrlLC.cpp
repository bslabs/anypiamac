// Functions for the <see cref="WageBaseLC"/> and
// <see cref="WageBaseOldLawLC"/> classes to manage wage base projections with
// law changes.

// $Id: wbgenrlLC.cpp 1.22 2011/08/11 13:55:34EDT 044579 Development  $

#include <cmath>  // for floor
#include "wbgenrlLC.h"
#include "LawChangeArray.h"
#include "LawChangeWAGEBASECHG.h"

using namespace std;

/// <summary>Initializes type of projection to present law, with the data
/// array starting in 1937.</summary>
///
/// <param name="newLawChange">Law change parameters.</param>
/// <param name="newLastYear">Last year of projection.</param>
WageBaseLC::WageBaseLC( const LawChangeArray& newLawChange,
int newLastYear ) :
WageBase(newLastYear), lawChange(newLawChange)
{ }

/// <summary>Initializes type of projection to present law, with the data
/// array starting in the specified year.</summary>
///
/// <param name="newLawChange">Law change parameters.</param>
/// <param name="newLastYear">Last year of projection.</param>
/// <param name="newBaseYear">First year of data.</param>
WageBaseLC::WageBaseLC( const LawChangeArray& newLawChange, int newBaseYear,
int newLastYear ) :
WageBase(newBaseYear, newLastYear), lawChange(newLawChange)
{ }

/// <summary>Initializes type of projection to old law, with the data array
/// starting in 1937.</summary>
///
/// <param name="newLawChange">Law change parameters.</param>
/// <param name="newLastYear">Last year of projection.</param>
WageBaseOldLawLC::WageBaseOldLawLC( const LawChangeArray& newLawChange,
int newLastYear ) :
WageBaseOldLaw(newLastYear), lawChange(newLawChange)
{ }

/// <summary>Initializes type of projection to old law, with the data array
/// starting in the specified year.</summary>
///
/// <param name="newLawChange">Law change parameters.</param>
/// <param name="newLastYear">Last year of projection.</param>
/// <param name="newBaseYear">First year of data.</param>
WageBaseOldLawLC::WageBaseOldLawLC( const LawChangeArray& newLawChange,
int newBaseYear, int newLastYear ) :
WageBaseOldLaw(newBaseYear, newLastYear), lawChange(newLawChange)
{ }

/// <summary>Projects oasdi wage bases with law changes.</summary>
///
/// <param name="averageWages">Average wages for wage base projection.</param>
/// <param name="cpi">Benefit increases.</param>
/// <param name="firstYeart">First year of projection.</param>
/// <param name="lastYeart">Last year of projection.</param>
void WageBaseLC::project( const DoubleAnnual& averageWages,
const DoubleAnnual& cpi, int firstYeart, int lastYeart )
{
  LawChangeWAGEBASECHG *lawChangeWAGEBASECHG =
    lawChange.lawChangeWAGEBASECHG;
  if (lawChangeWAGEBASECHG->getInd() == 1 ||
    lawChangeWAGEBASECHG->getInd() == 3) {
    // project oasdi bases before using ad hoc bases
    WageBaseGeneral::project(averageWages, cpi, firstYeart,
      lawChangeWAGEBASECHG->getStartYear() - 1);
    // use ad hoc bases
    for (int yr = lawChangeWAGEBASECHG->getStartYear();
      yr <= lawChangeWAGEBASECHG->getEndYear(); yr++) {
      theData[yr - getBaseYear()] = lawChangeWAGEBASECHG->getBaseOasdi(yr);
    }
    // project oasdi bases after using ad hoc bases
    projectLC(averageWages, cpi, lawChangeWAGEBASECHG->getEndYear());
  }
  else {
    // project oasdi bases using present law
    WageBaseGeneral::project(averageWages, cpi, firstYeart, lastYeart);
  }
}

/// <summary>Projects 1977 law wage bases with law changes.</summary>
///
/// <param name="averageWages">Average wages for wage base projection.</param>
/// <param name="cpi">Benefit increases.</param>
/// <param name="firstYeart">First year of projection.</param>
/// <param name="lastYeart">Last year of projection.</param>
void WageBaseOldLawLC::project( const DoubleAnnual& averageWages,
const DoubleAnnual& cpi, int firstYeart, int lastYeart )
{
  LawChangeWAGEBASECHG *lawChangeWAGEBASECHG =
    lawChange.lawChangeWAGEBASECHG;
  if (lawChangeWAGEBASECHG->getInd() == 2 ||
    lawChangeWAGEBASECHG->getInd() == 3) {
    // project oasdi bases before using ad hoc bases
    WageBaseGeneral::project(averageWages, cpi, firstYeart,
      lawChangeWAGEBASECHG->getStartYear() - 1);
    // use ad hoc bases
    for (int yr = lawChangeWAGEBASECHG->getStartYear();
      yr <= lawChangeWAGEBASECHG->getEndYear(); yr++) {
      theData[yr - getBaseYear()] = lawChangeWAGEBASECHG->getBase77(yr);
    }
    // project oasdi bases after using ad hoc bases
    projectLC(averageWages, cpi, lawChangeWAGEBASECHG->getEndYear());
  }
  else {
    // project oasdi bases using present law
    WageBaseGeneral::project(averageWages, cpi, firstYeart, lastYeart);
  }
}
