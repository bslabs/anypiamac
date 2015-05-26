// Declarations for the <see cref="PiaParamsTR"/> class of pia calculation
// parameters using present law only, with projected values set to Trustees
// Report assumptions.

// $Id: PiaParamsTR.h 1.7 2011/08/05 16:30:21EDT 044579 Development  $

#pragma once

#include "piaparmsPL.h"
#include "AssumptionType.h"

/// <summary>Manages the present law Social Security parameters required
/// to calculate Social Security benefits, with projected values set to
/// Trustees Report assumptions.</summary>
///
/// <remarks>Manages the present law Social Security parameters required
/// to calculate Social Security benefits, with projected values set to
/// Trustees Report assumptions.</remarks>
class PiaParamsTR : public PiaParamsPL
{
protected:
  /// <summary>Trustees Report alternative
  /// (<see cref="AssumptionType::assum_type::ALTERN_I"/>,
  /// <see cref="AssumptionType::assum_type::ALTERN_IIB"/>,
  /// <see cref="AssumptionType::assum_type::ALTERN_III"/>, or
  /// <see cref="AssumptionType::assum_type::ALTERN_IIA"/>).</summary>
  const AssumptionType::assum_type altNum;
public:
  PiaParamsTR( int newMaxyear, AssumptionType::assum_type newAltNum );
  ~PiaParamsTR();
  /// <summary>Returns the Trustees Report assumption.</summary>
  ///
  /// <returns>The Trustees Report assumption.</returns>
  AssumptionType::assum_type getAltNum() const { return altNum; }
};
