// Declarations for the <see cref="SgaGeneral"/> class - parent of classes to
// handle historical and projected SGA (Substantial Gainful Activity) amounts,
// stored in arrays.
//
// $Id: SgaGeneral.h 1.11 2011/08/09 15:55:29EDT 044579 Development  $

#pragma once

#include "dbleann.h"

/// <summary>Parent of classes to manage the SGA (Substantial Gainful Activity)
/// amounts (historical and projected).</summary>
///
/// <remarks>Any users of this class should make sure to call the
/// <see cref="project"/> function (for monthly projections) because the
/// constructor only initializes the historical data.
///
/// The only thing here that might ever need to be updated is
/// <see cref="FIRST_PROJ_YEAR"/>. This does <b>not</b> need to be updated for
/// every TR. For example, in the 2005 TR, the <see cref="FIRST_PROJ_YEAR"/>
/// could still be 2001, even thought 2001 is really an historical year. This
/// is because the only information the projection needs is the updated Average
/// Wage Series (which does get updated every year). Once the average wages are
/// updated, the projections will work automatically.
///
/// So the only reason to change the <see cref="FIRST_PROJ_YEAR"/> would be if
/// there was some change in the law that would not let the projections kick
/// off from 2000 amounts. In such a case, make sure to:
///
/// 1) Set <see cref="FIRST_PROJ_YEAR"/> correctly (in this file).
///
/// 2) Go to the function <see cref="SgaMinM::project"/> in the file
/// <see cref="SgaDerived.cpp"/>, and update the hard-coded number that
/// over-writes the data value in year <see cref="FIRST_PROJ_YEAR"/> - 1. This
/// number should match the max value for the corresponding year.
///
/// 3) In the file <see cref="SgaDerived.cpp"/>, update all the calls to
/// construct an <see cref="SgaGeneral"/> with the proper base years passed in
/// (if they have changed).
///
/// 4) In the file <see cref="SgaDerived.cpp"/>, update all the constructors
/// which initialize data. They should make sure to initialize the data for
/// all years up to (but not including) <see cref="FIRST_PROJ_YEAR"/> - 1.
///
/// Child classes are <see cref="SgaBlindA"/>, <see cref="SgaBlindM"/>,
/// <see cref="SgaMinA"/>, <see cref="SgaMinM"/>, <see cref="SgaMaxA"/>, and
/// <see cref="SgaMaxM"/>.</remarks>
///
/// <seealso cref="SgaBlindA"/>
/// <seealso cref="SgaBlindM"/>
/// <seealso cref="SgaMinA"/>
/// <seealso cref="SgaMinM"/>
/// <seealso cref="SgaMaxA"/>
/// <seealso cref="SgaMaxM"/>
class SgaGeneral : public DoubleAnnual
{
private:
  /// <summary>Year of base SGA amount used for projections.</summary>
  int baseSgaYear;
  /// <summary>Year of base Average Wage used for projections.</summary>
  int baseAverageWageYear;
protected:
  /// <summary>First year of projection (2001).</summary>
  static const int FIRST_PROJ_YEAR = 2001;
protected:
  static double project1( double baseAmount, double numerator,
    double denominator, double previous );
public:
  SgaGeneral( int newLastYear, int newBaseSgaYear );
  virtual ~SgaGeneral();
  void convertMonthToAnnual( const SgaGeneral& sgaM );
  virtual void project( const DoubleAnnual& averageWages );
};
