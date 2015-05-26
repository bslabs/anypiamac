// Declarations for the <see cref="OldPia"/> class - parent of all methods
// using pia tables in the calculation of a Social Security benefit.

// $Id: OldPia.h 1.13 2011/08/01 11:38:37EDT 044579 Development  $

#pragma once

#include "PiaMethod.h"

/// <summary>Parent of all methods using pia tables in the calculation of a
/// Social Security benefit.</summary>
///
/// <remarks>Child classes are <see cref="OldStart"/>, <see cref="PiaTable"/>,
/// and <see cref="SpecMin"/>.</remarks>
///
/// <seealso cref="OldStart"/>
/// <seealso cref="PiaTable"/>
/// <seealso cref="SpecMin"/>
class OldPia : public PiaMethod
{
protected:
  /// <summary>Maximum ame in pia interval.</summary>
  int iamemax;
public:
  OldPia( const WorkerDataGeneral& newWorkerData, const PiaData& newPiaData,
    const PiaParams& newPiaParams, int newMaxyear,
    const std::string& newTitle, pia_type newMethod );
  virtual ~OldPia();
  table_type cpiBase( const DateMoyr& benefitDate, bool freeze,
    double amesub, bool saveInfo );
  double mfbOldCal( bool belowMin );
  table_type oldPiaCal();
  table_type pl1952( int amesub );
  table_type pl1954( int amesub );
  table_type pl1958( int amesub );
  table_type pl1965( int amesub );
  table_type pl1967( int amesub );
  table_type pl1969( int amesub );
  table_type pl1971( int amesub );
  table_type pl1972( int amesub );
  void pl1973( int amesub );
  void pl1973ext( int amesub );
private:
  OldPia& operator=( OldPia& newOldPia );
};
