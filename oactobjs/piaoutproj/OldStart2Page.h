// Declarations for the <see cref="OldStart2Page"/> class to manage old start
// calculation results.

// $Id: OldStart2Page.h 1.12.1.1 2012/03/08 07:17:40EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
#include "OldStart.h"
class PiaCalAny;

/// <summary>Manages old start results from the calculation of a Social
/// Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="OldStart1Page"/>.</remarks>
///
/// <seealso cref="OldStart1Page"/>
/// <seealso cref="PiaOut"/>
class OldStart2Page : public PiaPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pia calculation summary data.</summary>
  const PiaData& piaData;
  /// <summary>Pia calculation parameters.</summary>
  const PiaParams& piaParams;
  /// <summary>Pia calculation detailed data.</summary>
  const PiaCalAny& piaCal;
  /// <summary>Years of old-start Social Security Acts.</summary>
  static const char *iyros[OldStart::NUM_OLD_START_TYPES];
  /// <summary>Sub-titles of old-start Social Security Acts.</summary>
  static const char *ios77[OldStart::NUM_OLD_START_TYPES];
public:
  OldStart2Page( const PiaCalAny& newPiaCal );
  ~OldStart2Page();
  void prepareStrings();
  void pubPenOut( const OldStart& oldStart, double pubpen,
    const DateMoyr& pubpenDate );
private:
  // {secret}
  OldStart2Page& operator=( OldStart2Page& newOldStart2Page );
};
