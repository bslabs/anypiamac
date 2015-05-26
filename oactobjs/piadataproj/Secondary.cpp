// Functions for the <see cref="Secondary"/> class containing the
// information about a secondary benefit.
//
// $Id: Secondary.cpp 1.14 2011/08/09 15:55:28EDT 044579 Development  $

#include "Secondary.h"
#include "PiaException.h"
#include "Resource.h"

/// <summary>Initializes codes to blank or zero.</summary>
Secondary::Secondary() : bic(), pifc(), entDate(), fullBenefit(0.),
benefit(0.), reducedBenefit(0.), roundedBenefit(0.), benefitFactor(0.),
monthsArdri(0), arf(0.)
{ }

/// <summary>Sets codes to blank or zero.</summary>
void Secondary::deleteContents()
{
  bic.set("  ");
  pifc.set(' ');
  fullBenefit = benefit = reducedBenefit = roundedBenefit = 0.;
  benefitFactor = arf = 0.;
  monthsArdri = 0;
  entDate.deleteContents();
}

/// <summary>Checks data for validity.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_SECONDARY"/> if type of beneficiary is not set.
/// </exception>
void Secondary::check()
{
  if (bic.getMajorBic() == ' ')
    throw PiaException(PIA_IDS_SECONDARY);
}
