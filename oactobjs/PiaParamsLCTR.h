// Declarations for the <see cref="PiaParamsLCTR"/> class to manage the changed
// law Social Security parameters, already projected based on a Trustees Report
// assumption.
//
// $Id: PiaParamsLCTR.h 1.5 2011/08/05 16:30:19EDT 044579 Development  $

#pragma once

#include "piaparmsLC.h"
#include "AssumptionType.h"

/// <summary>Manages the changed law Social Security parameters, already
/// projected based on a Trustees Report assumption.</summary>
///
/// <remarks>Often the user knows that only one version of
/// <see cref="PiaParams"/> will be used. This subclass of
/// <see cref="PiaParams"/> has a specified Trustees Report assumption, so no
/// additional work is necessary to begin using it.</remarks>
class PiaParamsLCTR : public PiaParamsLC
{
public:
  PiaParamsLCTR( int newMaxyear, AssumptionType::assum_type altNum,
    const LawChangeArray& newLawChange );
  virtual ~PiaParamsLCTR();
};
