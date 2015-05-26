// Functions for the <see cref="Bic"/> class to manage bic (Beneficiary
// Identification code) code.
//
// $Id: Bic.cpp 1.18 2011/07/29 11:25:02EDT 044579 Development  $

#include "bic.h"
#include "PiaException.h"
#include "Resource.h"

using namespace std;

// <summary>Type of survivor titles.</summary>
const string Bic::survivorType[6] = {
  "child survivor (minor, student, or disabled)",
  "mother/father (parent of child)",
  "disabled widow(er) (aged 50-59)",
  "divorced disabled widow(er) (aged 50-59)",
  "aged widow(er) (aged 60 or over)",
  "divorced aged widow(er) (aged 60 or over)"
};

// <summary>Type of life auxiliary titles.</summary>
const string Bic::lifeAuxType[4] = {
  "child life (minor, student, or disabled)",
  "young wife/husband (parent of child)",
  "aged wife/husband (aged 62 or over)",
  "divorced aged wife/husband (aged 62 or over)"
};

/// <summary>Initializes codes to blanks.</summary>
Bic::Bic() : majorBic(' '), minorBic(' ')
{ }

/// <summary>Initializes codes from the first two characters of a string.
/// </summary>
///
/// <param name="newCode">New bic codes as a string.</param>
Bic::Bic( const std::string& newCode ) :
majorBic(newCode[0]), minorBic(newCode[1])
{ }

/// <summary>Initializes codes from two characters.</summary>
///
/// <param name="newMajorBic">New major bic code.</param>
/// <param name="newMinorBic">New minor bic code.</param>
Bic::Bic( char newMajorBic, char newMinorBic ) :
majorBic(newMajorBic), minorBic(newMinorBic)
{ }

/// <summary>Sets the bic codes.</summary>
///
/// <param name="newCode">New bic codes as a string.</param>
void Bic::set( const std::string& newCode )
{
  majorBic = newCode[0];
  minorBic = newCode[1];
}

/// <summary>Returns the bic code as a string.</summary>
///
/// <returns>The bic code as a string.</returns>
std::string Bic::toString() const
{
  string str;
  str += majorBic;
  str += minorBic;
  return str;
}

/// <summary>Checks for valid survivor bic.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_JSURV"/> if bic is not a valid survivor bic.
/// </exception>
///
/// <param name="newMajorBic">The major bic to check.</param>
void Bic::survivorCheck( char newMajorBic )
{
  if (newMajorBic != 'C' && newMajorBic != 'D' && newMajorBic != 'E' &&
    newMajorBic != 'W') {
    throw PiaException(PIA_IDS_JSURV);
  }
}

/// <summary>Returns description of type of survivor.</summary>
///
/// <returns>Description of type of survivor (empty string if bic is not a
/// survivor).</returns>
std::string Bic::getSurvivorType() const
{
  switch (getMajorBic()) {
  case 'C':
    return survivorType[0];
  case 'D':
    if (getMinorBic() == ' ') {
      return survivorType[4];
    }
    else if (getMinorBic() == '6') {
      return survivorType[5];
    }
    else {
      return string();
    }
  case 'E':
    return survivorType[1];
  case 'W':
    if (getMinorBic() == ' ') {
      return survivorType[2];
    }
    else if (getMinorBic() == '6') {
      return survivorType[3];
    }
    else {
      return string();
    }
  default:
    return string();
  }
}

/// <summary>Returns description of type of life auxiliary.</summary>
///
/// <returns>Description of type of life auxiliary (empty string if bic is not
/// a life auxiliary).</returns>
std::string Bic::getLifeAuxType() const
{
  switch (getMajorBic()) {
  case 'C':
    return lifeAuxType[0];
  case 'B':
    if (getMinorBic() == ' ') {
      return lifeAuxType[2];
    }
    else if (getMinorBic() == '2') {
      return lifeAuxType[1];
    }
    else if (getMinorBic() == '6') {
      return lifeAuxType[3];
    }
    else {
      return string();
    }
  default:
    return string();
  }
}
