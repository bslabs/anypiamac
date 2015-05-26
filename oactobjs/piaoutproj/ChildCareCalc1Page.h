// Declarations for the <see cref="ChildCareCalc1Page"/> class to manage wage
// indexed earnings with child-care dropout years.

// $Id: ChildCareCalc1Page.h 1.11.1.1 2012/03/08 07:17:35EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;
class PiaData;

/// <summary>Manages wage indexed earnings from the calculation of a Social
/// Security benefit with child-care dropout years.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="ChildCareCalc2Page"/>.
/// </remarks>
///
/// <seealso cref="ChildCareCalc2Page"/>
/// <seealso cref="PiaOut"/>
class ChildCareCalc1Page : public PiaPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pia calculation summary data.</summary>
  const PiaData& piaData;
  /// <summary>Pia calculation detailed data.</summary>
  const PiaCalAny& piaCal;
public:
  ChildCareCalc1Page( const PiaCalAny& newPiaCal );
  ~ChildCareCalc1Page();
  void prepareStrings();
private:
  // {secret}
  ChildCareCalc1Page& operator=( ChildCareCalc1Page& newChildCareCalc1Page );
};
