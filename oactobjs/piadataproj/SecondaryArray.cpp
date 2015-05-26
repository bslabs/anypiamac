// Functions for the <see cref="SecondaryArray"/> class to manage a vector
// of secondary benefit data.
//
// $Id: SecondaryArray.cpp 1.10 2011/08/09 16:29:15EDT 044579 Development  $

#include "SecondaryArray.h"
#include "PiadataArray.h"

/// <summary>Initializes arrays.</summary>
///
/// <remarks>Creates <see cref="PiaDataArray::DimLimits::MAXFAMSIZE"/> new
/// <see cref="Secondary"/>'s and pushes them onto the vector.</remarks>
SecondaryArray::SecondaryArray()
{
  // allocate memory for secondary benefit data
  for (int i = 0; i < PiaDataArray::MAXFAMSIZE; i++) {
    Secondary *secondaryptr = new Secondary();
    secondary.push_back(secondaryptr);
  }
}

/// <summary>Releases memory.</summary>
SecondaryArray::~SecondaryArray()
{
  // release memory for secondary benefit
  for (int i = 0; i < PiaDataArray::MAXFAMSIZE; i++) {
    delete secondary[i];
  }
}

/// <summary>Resets all values to zero.</summary>
///
/// <remarks>For each vector element, calls
/// <see cref="Secondary::deleteContents"/>.</remarks>
void SecondaryArray::deleteContents()
{
  // reset all benefit data
  for (int i = 0; i < PiaDataArray::MAXFAMSIZE; i++)
    secondary[i]->deleteContents();
}

/// <summary>Returns total family benefit before reduction for age or family
/// maximum.</summary>
///
/// <returns>Total family benefit before reduction for age or family maximum.
/// </returns>
double SecondaryArray::getTotalFullBenefit() const
{
  double temp = 0.;
  // add benefit from each family member
  for (int i = 0; i < PiaDataArray::MAXFAMSIZE; i++) {
    temp += secondary[i]->getFullBenefit();
  }
  return temp;
}

/// <summary>Returns total family benefit before reduction for age or family
/// maximum, excluding beneficiaries who are not affected by the maximum.
/// </summary>
///
/// <remarks>This is what is compared to the mfb to see if a reduction is
/// required.</remarks>
///
/// <returns>Total family benefit before reduction for age or family
/// maximum, excluding beneficiaries who are not affected by the maximum.
/// </returns>
double SecondaryArray::getTotalFullBenefitMax() const
{
  double temp = 0.0;
  // add benefit from each family member
  for (int i = 0; i < PiaDataArray::MAXFAMSIZE; i++) {
    if (secondary[i]->eligibleForMax()) {
      temp += secondary[i]->getFullBenefit();
    }
  }
  return temp;
}
