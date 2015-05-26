// Declarations for the <see cref="PiaReadAny"/> class to read worker
// data for Anypia.

// $Id: piareadAny.h 1.5 2011/08/08 08:45:14EDT 044579 Development  $

#pragma once

#include "piaread.h"
class EarnProject;
class Pebs;

/// <summary>Reads the data required to calculate a Social Security
/// benefit for the Anypia program.</summary>
///
/// <remarks>This version adds data required for Social Security
/// Statements and factors to project earnings.</remarks>
class PiaReadAny : public PiaRead
{
private:
  /// <summary>Projected earnings information.</summary>
  EarnProject& earnProject;
  /// <summary>Social Security Statement information.</summary>
  Pebs& pebs;
public:
  PiaReadAny( WorkerData& newWorkerData, WorkerDataArray& newWidowDataArray,
    PiaDataArray& newWidowArray, UserAssumptions& newUserAssumptions,
    SecondaryArray& newSecondaryArray, EarnProject& newEarnProject,
    Pebs& newPebs );
  void parseBack( const std::string& line );
  void parseEarnOasdi( int lineNumber, const std::string& line );
  void parseEarnType( const std::string& line );
  void parseFwrd( const std::string& line );
  void parsePebes( const std::string& line );
  void setEarnProjectYears( int year1, int year2 );
  void setPebsData();
private:
  PiaReadAny operator=( const PiaReadAny& newPiaReadAny );
};
