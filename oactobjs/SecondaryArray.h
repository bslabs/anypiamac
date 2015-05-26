// Declarations for the <see cref="SecondaryArray"/> class to manage a vector
// of secondary benefit data.
//
// $Id: SecondaryArray.h 1.12 2011/08/09 15:55:30EDT 044579 Development  $

#pragma once

#include <vector>
#include "Secondary.h"

/// <summary>Manages an array of <see cref="Secondary"/>'s.</summary>
///
/// <remarks>This represents all the auxiliary benefits in a family. This is
/// used in various functions in <see cref="PiaCal"/>.</remarks>
///
/// <seealso cref="Secondary"/>
/// <seealso cref="PiaCal"/>
class SecondaryArray
{
public:
  /// <summary>Array of secondary data.</summary>
  std::vector< Secondary * > secondary;
public:
  SecondaryArray();
  ~SecondaryArray();
  void deleteContents();
  double getTotalFullBenefit() const;
  double getTotalFullBenefitMax() const;
};
