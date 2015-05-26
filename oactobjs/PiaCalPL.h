// Declarations for the <see cref="PiaCalPL"/> class to manage present law
// pia calculation functions.
//
// $Id: PiaCalPL.h 1.26 2011/08/01 16:22:23EDT 044579 Development  $

#pragma once

#include "piacal.h"

/// <summary>Manages all of the present law functions required
/// for calculation of a Social Security benefit.</summary>
///
/// <remarks>Manages all of the present law functions required
/// for calculation of a Social Security benefit.</remarks>
class PiaCalPL : public PiaCal
{
public:
  PiaCalPL( WorkerDataGeneral& newWorkerData, PiaData& newPiaData,
    const PiaParams& newPiaParams );
  ~PiaCalPL();
  void calculate2( const DateMoyr& entDate );
  void piaCal( const DateMoyr& entDate );
  void reindWidCal( const WorkerDataGeneral& widowData,
    const PiaData& widowPiaData, Secondary& secondary );
private:
  PiaCalPL operator=( PiaCalPL newPiaCalPL );
};
