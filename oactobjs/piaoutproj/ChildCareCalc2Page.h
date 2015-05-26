// Declarations for the <see cref="ChildCareCalc2Page"/> class to manage wage
// indexed results with child-care dropout years.

// $Id: ChildCareCalc2Page.h 1.10.1.1 2012/03/08 07:17:35EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;
class UserAssumptions;

/// <summary>Manages wage indexed results with child-care dropout years
/// from the calculation of a Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="ChildCareCalc1Page"/>.
/// </remarks>
///
/// <seealso cref="ChildCareCalc1Page"/>
/// <seealso cref="PiaOut"/>
class ChildCareCalc2Page : public PiaPageOut
{
protected:
  /// <summary>Worker basic data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pia calculation summary data.</summary>
  const PiaData& piaData;
  /// <summary>Pia calculation parameters.</summary>
  const PiaParams& piaParams;
  /// <summary>Pia calculation detailed data.</summary>
  const PiaCalAny& piaCal;
  /// <summary>User-specified assumptions.</summary>
  const UserAssumptions& userAssumptions;
public:
  ChildCareCalc2Page( const PiaCalAny& newPiaCal );
  ~ChildCareCalc2Page();
  void prepareStrings();
private:
  // {secret}
  ChildCareCalc2Page& operator=( ChildCareCalc2Page& newChildCareCalc2Page );
};
