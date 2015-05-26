// Declarations for the <see cref="BendPoints"/> class to handle PIA and MFB
// formula bend points.
//
// $Id: BendPoints.h 1.3 2011/07/28 15:19:54EDT 044579 Development  $

#pragma once

#include "boost/array.hpp"
class AverageWage;

/// <summary>Manages the array of bend points used in the MFB formula.
/// </summary>
///
/// <remarks>An instance of this class is used in most of the
/// <see cref="PiaMethod"/> classes. There are 4 slots for bend points.
/// </remarks>
typedef boost::array< double, 4 > BendMfb;

/// <summary>Manages the array of bend points used in the PIA formula.
/// </summary>
///
/// <remarks>An instance of this class is used in <see cref="WageIndGeneral"/>.
/// There is an array with 3 slots for bend points.</remarks>
typedef boost::array< double, 3 > BendPia;

/// <summary>Manages the array of percentages used in the MFB formula.</summary>
///
/// <remarks>An instance of this class is a member of most
/// <see cref="PiaMethod"/> classes.</remarks>
typedef boost::array< double, 4 > PercMfb;

/// <summary>Manages the array of percentages used in the PIA formula.</summary>
///
/// <remarks>An instance of this class is a member of the
/// <see cref="WageIndGeneral"/> class.</remarks>
typedef boost::array< double, 3 > PercPia;

/// <summary>Static functions to handle bend points used in the PIA or MFB
/// formula.</summary>
///
/// <remarks>These functions are used throughout the piadata library.</remarks>
class BendPoints
{
public:
  /// <summary>Four MFB bend points from 1979.</summary>
  static const double MFBBP1979[4];
  /// <summary>Three PIA bend points from 1979.</summary>
  static const double PIABP1979[3];
  /// <summary>Four MFB percentages (for all years).</summary>
  static const double MFBPERC[4];
  /// <summary>Number of years of coverage that negates the windfall
  /// elimination provision.</summary>
  static const int WINDFALL_YEARS = 30;
  /// <summary>The three percentages for the regular PIA formula.</summary>
  static const double PIAPERC[3];
  /// <summary>The first PIA formula percentage under the windfall elimination
  /// provision.</summary>
  static const double WINDFALL_PERC;
public:
  static void projectMfb( BendMfb& bendMfb, int eligYear,
    const AverageWage& fq );
  static void projectPia( BendPia& bendPia, int eligYear,
    const AverageWage& fq );
  static void resetMfbPerc( PercMfb& percMfb );
  static void resetPiaPerc( PercPia& percPia );
  static void setWindfallPerc( PercPia& percPia, int eligYear, int benefitDate,
    int specMinTot );
};
