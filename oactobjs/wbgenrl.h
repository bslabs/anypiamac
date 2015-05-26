// Declarations for the <see cref="WageBaseGeneral"/> class - parent of classes
// that manage wage base projections.

// $Id: wbgenrl.h 1.20 2011/08/11 13:55:33EDT 044579 Development  $

#pragma once

#include "dbleann.h"

/// <summary>Ancestor of the classes that manage the wage base projections.
/// </summary>
///
/// <remarks>Child classes are <see cref="WageBase"/>,
/// <see cref="WageBaseHI"/>, and <see cref="WageBaseOldLaw"/>.</remarks>
///
/// <seealso cref="WageBase"/>
/// <seealso cref="WageBaseHI"/>
/// <seealso cref="WageBaseOldLaw"/>
class WageBaseGeneral : public DoubleAnnual
{
protected:
  /// <summary>Type of wage base.</summary>
  ///
  /// <remarks>It has the following possible values:
  /// <table>
  ///  0   present law OASDI.
  ///  1   prior law (ignore 1990-92 ad hoc increases).
  ///  2   old law (ignore 1979-81 ad hoc increases).
  ///  3   HI.
  /// </table></remarks>
  int wageBaseInd;
  /// <summary>Increase in average wage due to deferred comp in 1990.
  /// </summary>
  static const double defcomp;
public:
  WageBaseGeneral( int newWageBaseInd, int newLastYear );
  WageBaseGeneral( int newWageBaseInd, int newBaseYear, int newLastYear );
  virtual ~WageBaseGeneral();
  static void check( double baset );
  void earnLimit( DoubleAnnual& earnings1, const DoubleAnnual& earnings2 )
    const;
  virtual void project( const DoubleAnnual& averageWages,
    const DoubleAnnual& cpi, int firstYeart, int lastYeart );
  void projectLC( const DoubleAnnual& averageWages, const DoubleAnnual& cpi,
    int lastYearLC );
};
