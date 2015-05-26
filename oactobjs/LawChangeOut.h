// Declarations for the <see cref="LawChangeOut"/> class to manage the output
// file of law change descriptions.
//
// $Id: LawChangeOut.h 1.9 2011/08/01 10:06:09EDT 044579 Development  $

#pragma once

#include "PageOut.h"
class LawChangeArray;

/// <summary>Manages the output file of law change descriptions.</summary>
///
/// <remarks>The law changes are contained in a <see cref="LawChangeArray"/>.
/// </remarks>
class LawChangeOut : public PageOut
{
public:
  /// <summary>Law change data.</summary>
  const LawChangeArray& lawChange;
public:
  LawChangeOut( const LawChangeArray& newLawChange );
  ~LawChangeOut();
  void prepareStrings();
private:
  LawChangeOut operator=( LawChangeOut newLawChangeOut );
};
