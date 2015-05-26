// Declarations for the <see cref="WageBaseLC"/> and
// <see cref="WageBaseOldLawLC"/> classes to manage wage base projections with
// law changes.

// $Id: wbgenrlLC.h 1.15 2011/08/11 13:55:33EDT 044579 Development  $

#pragma once

#include "WageBase.h"
class LawChangeArray;

/// <summary>Manages the OASDI wage base projection with law changes.
/// </summary>
///
/// <remarks>Manages the OASDI wage base projection with law changes.
/// </remarks>
class WageBaseLC : public WageBase
{
private:
  /// <summary>Law-change parameters.</summary>
  const LawChangeArray& lawChange;
public:
  WageBaseLC( const LawChangeArray& newLawChange, int newLastYear );
  WageBaseLC( const LawChangeArray& newLawChange, int newBaseYear,
    int newLastYear );
  void project( const DoubleAnnual& averageWages, const DoubleAnnual& cpi,
    int firstYeart, int lastYeart );
private:
  WageBaseLC& operator=( const WageBaseLC& );
};

/// <summary>Manages the old-law OASDI wage base projection with law changes.
/// </summary>
///
/// <remarks>Manages the old-law OASDI wage base projection with law changes.
/// </remarks>
class WageBaseOldLawLC : public WageBaseOldLaw
{
private:
  /// <summary>Law-change parameters.</summary>
  const LawChangeArray& lawChange;
public:
  WageBaseOldLawLC( const LawChangeArray& newLawChange, int newLastYear );
  WageBaseOldLawLC( const LawChangeArray& newLawChange, int newBaseYear,
    int newLastYear );
  void project( const DoubleAnnual& averageWages, const DoubleAnnual& cpi,
    int firstYeart, int lastYeart );
private:
  WageBaseOldLawLC& operator=( const WageBaseOldLawLC& );
};
