// Declarations for the <see cref="Summary3Page"/> class to manage the bottom
// part of a summary of results page.

// $Id: Summary3Page.h 1.12.1.1 2012/03/08 07:17:55EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;
class UserAssumptions;
class EarnProject;

/// <summary>Manages the bottom part of the summary of results page from the
/// calculation of a Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="Summary1Page"/> and
/// <see cref="Summary2Page"/>.</remarks>
///
/// <seealso cref="Summary1Page"/>
/// <seealso cref="Summary2Page"/>
/// <seealso cref="PiaOut"/>
class Summary3Page : public PiaPageOut
{
private:
  /// <summary>Worker basic data.</summary>
  const WorkerDataGeneral& workerData;
  /// <summary>Pia calculation summary data.</summary>
  const PiaData& piaData;
  /// <summary>Pia calculation detailed data.</summary>
  const PiaCalAny& piaCal;
  /// <summary>User-specified assumptions.</summary>
  const UserAssumptions& userAssumptions;
  /// <summary>Projected earnings.</summary>
  const EarnProject& earnProject;
  /// <summary>Indicator for AWI projections in estimate.</summary>
  bool awiProj;
  /// <summary>Indicator for COLA projections in estimate.</summary>
  bool biProj;
public:
  Summary3Page( const PiaCalAny& newPiaCal );
  ~Summary3Page();
  void bottom3( bool isPebs );
  void nonInsPara();
  void disclaimer();
  void prepareStrings();
private:
  // {secret}
  Summary3Page& operator=( Summary3Page& newSummary3Page );
};
