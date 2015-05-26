// Declarations for the <see cref="PiaCalLC"/> class to manage law-change pia
// calculation functions.
//
// $Id: PiaCalLC.h 1.46 2013/10/29 13:12:18EDT 277133 Development  $

#pragma once

#include "piacal.h"
class LawChangeArray;
class LawChangeCHILDCARECREDIT;

/// <summary>Manages all of the law-change functions required for
/// calculation of a Social Security benefit.</summary>
///
/// <remarks>Manages all of the law-change functions required for
/// calculation of a Social Security benefit.</remarks>
class PiaCalLC : public PiaCal
{
private:
  /// <summary>Law change parameters.</summary>
  const LawChangeArray &lawChange;
public:
  PiaCalLC ( WorkerDataGeneral& newWorkerData, PiaData& newPiaData,
    const PiaParams& newPiaParams, const LawChangeArray& newLawChange );
  ~PiaCalLC();
  void calculate2( const DateMoyr& entDate );
  void earnProjection() const;
  void nCal( CompPeriod& compPeriod, const DateMoyr& entDate );
  int nelapsed2Cal( const DateMoyr& entDate );
  int nelapsed2NonFreezeCal( const DateMoyr& entDate );
  void piaCal( const DateMoyr& entDate );
  void reindWidCal( const WorkerDataGeneral& widowData,
    const PiaData& widowPiaData, Secondary& secondary );
  static double childCareMaxCreditCal( const WorkerDataGeneral& workerData,
    const DoubleAnnual& earnOasdi, const AverageWage& awi,
    const LawChangeCHILDCARECREDIT& lawChange, int year );
  static void childCareCreditCal( const WorkerDataGeneral& workerData,
    PiaData& piaData, const PiaParams& piaParams,
    const LawChangeArray& lawChange );
  static void childCareEarningsCal( const WorkerDataGeneral& workerData,
    PiaData& piaData, const LawChangeArray& lawChange );
  static void childCareQcCal( const WorkerDataGeneral& workerData,
    PiaData& piaData, const PiaParams& piaParams,
    const LawChangeArray& lawChange );
  static void earnProjectionStatic( const WorkerDataGeneral& workerData,
    PiaData& piaData, const PiaParams& piaParams,
    const LawChangeArray& lawChange );
private:
  PiaCalLC operator=( PiaCalLC newPiaCalLC );
};
