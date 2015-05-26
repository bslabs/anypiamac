// Declarations for the <see cref="PebsEarningsInfo"/> class to manage the
// Social Security Statement earnings info page.

// $Id: PebsEarningsInfo.h 1.8.1.1 2012/03/08 07:17:43EST 277133 Development  $

#pragma once

#include "PebsPageOut.h"
class PiaParams;

/// <summary>Manages the Social Security Statement earnings info page.
/// </summary>
///
/// <remarks>An instance of this class is created in <see cref="PebsOut"/>.
/// </remarks>
///
/// <seealso cref="PebsOut"/>
class PebsEarningsInfo : public PebsPageOut
{
private:
  /// <summary>Pia calculation parameters.</summary>
  const PiaParams& piaParams;
public:
  PebsEarningsInfo( const PiaParams& newPiaParams );
  ~PebsEarningsInfo();
  void prepareStrings();
private:
  // {secret}
  PebsEarningsInfo& operator=( PebsEarningsInfo& newPebsEarningsInfo );
};
