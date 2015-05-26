// Declarations for the <see cref="PiaWriteAny"/> class to write out worker
// data for Anypia program.
//
// $Id: piawriteAny.h 1.4 2011/08/08 08:45:15EDT 044579 Development  $

#pragma once

#include "piawrite.h"
class EarnProject;
class Pebs;

/// <summary>Writes out the data required to calculate a Social Security
/// benefit for the Anypia program.</summary>
///
/// <remarks>This version adds data required for Social Security
/// Statements and factors to project earnings.</remarks>
class PiaWriteAny : public PiaWrite
{
private:
  /// <summary>Projected earnings information.</summary>
  const EarnProject& earnProject;
  /// <summary>Social Security Statement information.</summary>
  const Pebs& pebs;
public:
  PiaWriteAny( const WorkerData& newWorkerData,
    const WorkerDataArray& newWidowDataArray,
    const PiaDataArray& newWidowArray,
    const UserAssumptions& newUserAssumptions,
    const SecondaryArray& newSecondaryArray,
    const EarnProject& newEarnProject, const Pebs& newPebs );
  void writeAssump( std::ostream& out ) const;
  void writeBack ( std::ostream& out ) const;
  void writeEarnOasdi( std::ostream& out ) const;
  void writeEarnType( std::ostream& out ) const;
  void writeFwrd ( std::ostream& out ) const;
  void writePebes( std::ostream& out ) const;
private:
  PiaWriteAny operator=( const PiaWriteAny& newPiaWriteAny );
};
