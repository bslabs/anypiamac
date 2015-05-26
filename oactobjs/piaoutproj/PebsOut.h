// Declarations for the <see cref="PebsOut"/> class to print out the
// Social Security Statement.

// $Id: PebsOut.h 1.10.1.1 2012/03/08 07:17:45EST 277133 Development  $

#pragma once

#include "PebsLetter.h"
#include "PebsWorkerInfo.h"
#include "PebsCreditInfo.h"
#include "PebsEarnings.h"
#include "PebsOasdiEst.h"
#include "PebsEarningsInfo.h"
#include "PebsOasdiInfo.h"
#include "PebsWorkerEarnings.h"

/// <summary>Manages the printing of the results of a Social Security
/// Statement calculation.</summary>
///
/// <remarks>It contains an instance of each of the pages of output related
/// to a Social Security Statement. For a similar class that contains an
/// instance of each of the pages of output related to a non-Statement
/// calculation, see <see cref="PiaOut"/>.</remarks>
///
/// <seealso cref="PiaOut"/>
class PebsOut
{
public:
  /// <summary>Letter transmitting results.</summary>
  PebsLetter pebsLetter;
  /// <summary>Worker information.</summary>
  PebsWorkerInfo pebsWorkerInfo;
  /// <summary>Credit information.</summary>
  PebsCreditInfo pebsCreditInfo;
  /// <summary>Earnings information.</summary>
  PebsEarnings pebsEarnings;
  /// <summary>OASDI estimates.</summary>
  PebsOasdiEst pebsOasdiEst;
  /// <summary>Earnings information.</summary>
  PebsEarningsInfo pebsEarningsInfo;
  /// <summary>OASDI benefit information.</summary>
  PebsOasdiInfo pebsOasdiInfo;
  /// <summary>Worker information and earnings.</summary>
  PebsWorkerEarnings pebsWorkerEarnings;
public:
  PebsOut( const WorkerData& newWorkerData, const PiaData& newPiaData,
    const FieldOfficeInfo& newFieldOfficeInfo, const Pebs& newPebs,
    const PiaParams& newPiaParams, const TaxData& newTaxData,
    const EarnProject& newEarnProject );
  ~PebsOut();
  void prepareStrings();
  void setPageNum();
};
