// Functions for the <see cref="Sga"/> class to manage SGA amounts.

// $Id: Sga.cpp 1.6 2011/08/09 15:55:30EDT 044579 Development  $

#include "Sga.h"

using namespace std;

/// <summary>Constructor.</summary>
///
/// <param name="newLastYear">Last year allowed.</param>
Sga::Sga( int newLastYear ) :
sgaBlindA(newLastYear), sgaBlindM(newLastYear),
sgaMinA(newLastYear), sgaMinM(newLastYear),
sgaMaxA(newLastYear), sgaMaxM(newLastYear)
{ }

/// <summary>Projects all SGA amounts.</summary>
///
/// <param name="averageWages">Average wages to use for projection.</param>
void Sga::project( const DoubleAnnual& averageWages )
{
  sgaBlindM.project(averageWages);
  sgaBlindA.convertMonthToAnnual(sgaBlindM);
  sgaMinM.project(averageWages);
  sgaMinA.convertMonthToAnnual(sgaMinM);
  sgaMaxM.project(averageWages);
  sgaMaxA.convertMonthToAnnual(sgaMaxM);
}
