// Declarations for the <see cref="WageBase"/>, <see cref="WageBaseHI"/>,
// and <see cref="WageBaseOldLaw"/> classes to manage wage base projections.

// $Id: WageBase.h 1.8 2011/08/11 13:55:34EDT 044579 Development  $

#pragma once

#include "wbgenrl.h"

/// <summary>Manages the OASDI wage base projections.</summary>
///
/// <remarks>Manages the OASDI wage base projections.</remarks>
class WageBase : public WageBaseGeneral
{
public:
  WageBase( int newLastYear );
  WageBase( int newBaseYear, int newLastYear );
};

/// <summary>Manages the HI wage base projections.</summary>
///
/// <remarks>Manages the HI wage base projections.</remarks>
class WageBaseHI : public WageBaseGeneral
{
public:
  WageBaseHI( int newLastYear );
  WageBaseHI( int newBaseYear, int newLastYear );
};

/// <summary>Manages the old-law OASDI wage base projections.</summary>
///
/// <remarks>Manages the old-law OASDI wage base projections.</remarks>
class WageBaseOldLaw : public WageBaseGeneral
{
public:
  WageBaseOldLaw( int newLastYear );
  WageBaseOldLaw( int newBaseYear, int newLastYear );
};
