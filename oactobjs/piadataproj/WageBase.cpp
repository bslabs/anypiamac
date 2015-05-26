// Functions for the <see cref="WageBase"/>, <see cref="WageBaseHI"/>,
// and <see cref="WageBaseOldLaw"/> classes to manage wage base projections.

// $Id: WageBase.cpp 1.11 2011/08/11 13:55:35EDT 044579 Development  $

#include "WageBase.h"

/// <summary>Initializes type of projection to present law, first year is
/// 1937.</summary>
///
/// <param name="newLastYear">Last year of projection.</param>
WageBase::WageBase( int newLastYear ) : WageBaseGeneral(0, newLastYear)
{ }

/// <summary>Initializes type of projection to present law, first year is
/// specified.</summary>
///
/// <param name="newLastYear">Last year of projection.</param>
/// <param name="newBaseYear">First year of data.</param>
WageBase::WageBase( int newBaseYear, int newLastYear ) :
WageBaseGeneral(0, newBaseYear, newLastYear)
{ }

/// <summary>Initializes type of projection to HI, first year is 1937.
/// </summary>
///
/// <param name="newLastYear">Last year of projection.</param>
WageBaseHI::WageBaseHI( int newLastYear ) : WageBaseGeneral(3, newLastYear)
{ }

/// <summary>Initializes type of projection to HI, first year is specified.
/// </summary>
///
/// <param name="newLastYear">Last year of projection.</param>
/// <param name="newBaseYear">First year of data.</param>
WageBaseHI::WageBaseHI( int newBaseYear, int newLastYear ) :
WageBaseGeneral(3, newBaseYear, newLastYear)
{ }

/// <summary>Initializes type of projection to old law, first year is 1937.
/// </summary>
///
/// <param name="newLastYear">Last year of projection.</param>
WageBaseOldLaw::WageBaseOldLaw( int newLastYear ) :
WageBaseGeneral(2, newLastYear)
{ }

/// <summary>Initializes type of projection to old law, first year is
/// specified.</summary>
///
/// <param name="newLastYear">Last year of projection.</param>
/// <param name="newBaseYear">First year of data.</param>
WageBaseOldLaw::WageBaseOldLaw( int newBaseYear, int newLastYear ) :
WageBaseGeneral(2, newBaseYear, newLastYear)
{ }
