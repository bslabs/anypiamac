// Declarations for the <see cref="SgaBlindA"/>, <see cref="SgaBlindM"/>,
// <see cref="SgaMinA"/>, <see cref="SgaMinM"/>, <see cref="SgaMaxA"/>, and
// <see cref="SgaMaxM"/> classes (derived classes of the
// <see cref="SgaGeneral"/> class).

// $Id: SgaDerived.h 1.7 2011/08/09 15:55:29EDT 044579 Development  $

#pragma once

#include "SgaGeneral.h"

/// <summary>Manages the annual SGA amounts (historical and projected) for
/// blind beneficiaries.</summary>
///
/// <remarks>There is no need to update the data in this class with each new
/// TR. See notes in <see cref="SgaGeneral"/> as to when an update may be
/// neccessary (and how to do it).
///
/// The monthly SGA amounts for blind beneficiaries are in
/// <see cref="SgaBlindM"/>.</remarks>
///
/// <seealso cref="SgaBlindM"/>
class SgaBlindA : public SgaGeneral
{
public:
  SgaBlindA( int newLastYear );
  void project( const DoubleAnnual& );
};

/// <summary>Manages the monthly SGA amounts (historical and projected) for
/// blind beneficiaries (as of December).</summary>
///
/// <remarks>There is no need to update the data in this class with each new
/// TR. See notes in <see cref="SgaGeneral"/> as to when an update may be
/// neccessary (and how to do it).
///
/// The annual SGA amounts for blind beneficiaries are in
/// <see cref="SgaBlindA"/>.</remarks>
///
/// <seealso cref="SgaBlindA"/>
class SgaBlindM : public SgaGeneral
{
public:
  SgaBlindM( int newLastYear );
};

/// <summary>Manages the annual SGA minimum amounts (historical and projected)
/// for non-blind beneficiaries.</summary>
///
/// <remarks>There is no need to update the data in this class with each new
/// TR. See notes in <see cref="SgaGeneral"/> as to when an update may be
/// neccessary (and how to do it).
///
/// The monthly SGA minimum amounts are in <see cref="SgaMinM"/>.</remarks>
///
/// <seealso cref="SgaMinM"/>
class SgaMinA : public SgaGeneral
{
public:
  SgaMinA( int newLastYear );
  void project( const DoubleAnnual& );
};

/// <summary>Manages the monthly SGA minimum amounts (historical and projected)
/// for non-blind beneficiaries (as of December).</summary>
///
/// <remarks>There is no need to update the data in this class with each new
/// TR. See notes in <see cref="SgaGeneral"/> as to when an update may be
/// neccessary (and how to do it).
///
/// The annual SGA minimum amounts are in <see cref="SgaMinA"/>.</remarks>
///
/// <seealso cref="SgaMinA"/>
class SgaMinM : public SgaGeneral
{
public:
  SgaMinM( int newLastYear );
  void project( const DoubleAnnual& averageWages );
};

/// <summary>Manages the annual SGA maximum amounts(historical and projected)
/// for non-blind beneficiaries.</summary>
///
/// <remarks>There is no need to update the data in this class with each new
/// TR. See notes in <see cref="SgaGeneral"/> as to when an update may be
/// neccessary (and how to do it).
///
/// The monthly SGA maximum amounts are in <see cref="SgaMaxM"/>.</remarks>
///
/// <seealso cref="SgaMaxM"/>
class SgaMaxA : public SgaGeneral
{
public:
  SgaMaxA( int newLastYear );
  void project( const DoubleAnnual& );
};

/// <summary>Manages the monthly SGA maximum amounts (historical and projected)
/// for non-blind beneficiaries (as of December).</summary>
///
/// <remarks>There is no need to update the data in this class with each new
/// TR. See notes in <see cref="SgaGeneral"/> as to when an update may be
/// neccessary (and how to do it).
///
/// The annual SGA maximum amounts are in <see cref="SgaMaxA"/>.</remarks>
///
/// <seealso cref="SgaMaxA"/>
class SgaMaxM : public SgaGeneral
{
public:
  SgaMaxM( int newLastYear );
};
