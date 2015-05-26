// Functions for the <see cref="Ssn"/> class to manage a Social Security
// number.

// $Id: ssn.cpp 1.23 2011/08/19 15:57:45EDT 044579 Development  $

#include <stdexcept>
#include "ssn.h"
#include "PiaException.h"
#include "Resource.h"

using namespace std;

/// <summary>Creates ssn, initializing all fields with zeroes.</summary>
Ssn::Ssn () : ssnFull("000000000"), ssnWithHyphens("000-00-0000")
{
  ssnPart[0] = "000";
  ssnPart[1] = "00";
  ssnPart[2] = "0000";
}

/// <summary>Creates ssn by using the three specified parts.</summary>
///
/// <param name="newSsnPart1">First part of ssn.</param>
/// <param name="newSsnPart2">Second part of ssn.</param>
/// <param name="newSsnPart3">Third part of ssn.</param>
Ssn::Ssn( const std::string& newSsnPart1, const std::string& newSsnPart2,
const std::string& newSsnPart3 )
{
  setSsnFull(newSsnPart1, newSsnPart2, newSsnPart3);
}

/// <summary>Creates ssn by using one string without hyphens.</summary>
///
/// <param name="newSsn">String to use for full ssn.</param>
Ssn::Ssn( const std::string& newSsn )
{
  string temp;
  if (newSsn.length() < 9) {
    temp.append(9 - newSsn.length(), '0');
    temp += newSsn;
  }
  else {
    temp = newSsn;
  }
  ssnFull = temp;
  try {
    ssnPart[0] = temp.substr(0,3);
    ssnPart[1] = temp.substr(3, 2);
    ssnPart[2] = temp.substr(5, 4);
  // if substr argument is out of range, catch the out_of_range exception
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_SSN1);
  }
}

/// <summary>Copies from another <see cref="Ssn"/>.</summary>
///
/// <param name="ssn">The <see cref="Ssn"/> to copy from.</param>
Ssn::Ssn( const Ssn& ssn ) : ssnFull(ssn.ssnFull),
ssnWithHyphens(ssn.ssnWithHyphens)
{
  for (int i = 0; i < 3; i++)
    ssnPart[i] = ssn.ssnPart[i];
}

/// <summary>Copies from another <see cref="Ssn"/>.</summary>
///
/// <returns>A reference to this instance.</returns>
///
/// <param name="ssn">The <see cref="Ssn"/> to copy from.</param>
Ssn& Ssn::operator=( const Ssn& ssn )
{
  for (int i = 0; i < 3; i++)
    ssnPart[i] = ssn.ssnPart[i];
  ssnFull = ssn.ssnFull;
  ssnWithHyphens = ssn.ssnWithHyphens;
  return *this;
}

/// <summary>Sets first part of ssn.</summary>
///
/// <param name="newPart1">New first part of ssn.</param>
void Ssn::setPart1( const std::string& newPart1 )
{
  string temp;
  if (newPart1.length() < 3) {
    temp.append(3 - newPart1.length(), '0');
    temp += newPart1;
  }
  else {
    temp = newPart1;
  }
  ssnPart[0] = temp.substr(0,3);
  update();
}

/// <summary>Sets second part of ssn.</summary>
///
/// <param name="newPart2">New second part of ssn.</param>
void Ssn::setPart2( const std::string& newPart2 )
{
  string temp;
  if (newPart2.length() < 2) {
    temp.append(2 - newPart2.length(), '0');
    temp += newPart2;
  }
  else {
    temp = newPart2;
  }
  ssnPart[1] = temp.substr(0,2);
  update();
}

/// <summary>Sets third part of ssn.</summary>
///
/// <param name="newPart3">New third part of ssn.</param>
void Ssn::setPart3( const std::string& newPart3 )
{
  string temp;
  if (newPart3.length() < 4) {
    temp.append(4 - newPart3.length(), '0');
    temp += newPart3;
  }
  else {
    temp = newPart3;
  }
  ssnPart[2] = temp.substr(0,4);
  update();
}

/// <summary>Updates full ssn from three parts.</summary>
void Ssn::update()
{
  ssnFull = ssnPart[0].substr(0,3);
  ssnFull += ssnPart[1].substr(0,2);
  ssnFull += ssnPart[2].substr(0,4);
  ssnWithHyphens = ssnPart[0].substr(0,3);
  ssnWithHyphens += "-";
  ssnWithHyphens += ssnPart[1].substr(0,2);
  ssnWithHyphens += "-";
  ssnWithHyphens += ssnPart[2].substr(0,4);
}

/// <summary>Returns formatted ssn with hyphens separating
/// the three parts.</summary>
///
/// <returns>Formatted ssn.</returns>
std::string Ssn::toString() const
{
  return(ssnWithHyphens);
}

/// <summary>Returns formatted ssn.</summary>
///
/// <remarks>If the format string begins with "n", do not use hyphens;
/// otherwise use hyphens separating the three parts.</remarks>
///
/// <returns>Formatted ssn.</returns>
///
/// <param name="format">Format to use.</param>
std::string Ssn::toString( const std::string& format ) const
{
  if (format.length() < 1)
    return ssnWithHyphens;
  switch (format[0]) {
  case 'n':
    return ssnFull;
  default:
    return ssnWithHyphens;
  }
}

/// <summary>Sets the full ssn by using the three specified parts.</summary>
///
/// <remarks>Three strings are used, the three parts are set,
/// then the full ssn is set from the concatenated parts.</remarks>
///
/// <param name="newSsnPart1">First part of ssn.</param>
/// <param name="newSsnPart2">Second part of ssn.</param>
/// <param name="newSsnPart3">Third part of ssn.</param>
void Ssn::setSsnFull( const std::string& newSsnPart1,
const std::string& newSsnPart2, const std::string& newSsnPart3 )
{
  string temp;
  if (newSsnPart1.length() < 3) {
    temp.append(3 - newSsnPart1.length(), '0');
    temp += newSsnPart1;
  }
  else {
    temp = newSsnPart1;
  }
  ssnPart[0] = temp.substr(0,3);
  if (newSsnPart2.length() < 2) {
    temp = "";
    temp.append(2 - newSsnPart2.length(), '0');
    temp += newSsnPart2;
  }
  else {
    temp = newSsnPart2;
  }
  ssnPart[1] = temp.substr(0,2);
  if (newSsnPart3.length() < 4) {
    temp = "";
    temp.append(4 - newSsnPart3.length(), '0');
    temp += newSsnPart3;
  }
  else {
    temp = newSsnPart3;
  }
  ssnPart[2] = temp.substr(0,4);
  update();
}

/// <summary>Sets the full ssn by using one string without hyphens.</summary>
///
/// <remarks>One string is used, the full ssn is set from that, then the
/// three parts are parsed.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_SSN1"/> if there any string-handling problems.
/// </exception>
///
/// <param name="newSsnFull">String to use for full ssn (without hyphens).
/// </param>
void Ssn::setSsnFull( const std::string& newSsnFull )
{
  string temp;
  if (newSsnFull.length() < 9) {
    temp.append(9 - newSsnFull.length(), '0');
    temp += newSsnFull;
  }
  else {
    temp = newSsnFull;
  }
  try {
    ssnPart[0] = temp.substr(0,3);
    ssnPart[1] = temp.substr(3, 2);
    ssnPart[2] = temp.substr(5, 4);
  // if substr argument is out of range, catch the out_of_range exception
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_SSN1);
  }
  update();
}

/// <summary>Compares to another <see cref="Ssn"/>.</summary>
///
/// <returns>True if this <see cref="Ssn"/> is less than the argument
/// <see cref="Ssn"/>, false otherwise.</returns>
///
/// <param name="ssntemp"><see cref="Ssn"/> to compare to.</param>
bool Ssn::operator<( const Ssn& ssntemp ) const
{
  return (ssnFull < ssntemp.ssnFull);
}

/// <summary>Compares to another <see cref="Ssn"/>.</summary>
///
/// <returns>True if this <see cref="Ssn"/> is equal to the argument
/// <see cref="Ssn"/>, false otherwise.</returns>
///
/// <param name="ssntemp"><see cref="Ssn"/> to compare to.</param>
bool Ssn::operator==( const Ssn& ssntemp ) const
{
  return (ssnFull == ssntemp.ssnFull);
}

/// <summary>Zeroes the three parts.</summary>
void Ssn::deleteContents()
{
  ssnPart[0] = "000";
  ssnPart[1] = "00";
  ssnPart[2] = "0000";
  update();
}

/// <summary>Returns ssn converted to a number.</summary>
///
/// <returns>The ssn converted to a number. It could be as large as 999999999.
/// </returns>
long Ssn::toNumber() const
{
  return atoi(ssnPart[0].c_str()) * 1000000 +
    atoi(ssnPart[1].c_str()) * 10000 + atoi(ssnPart[2].c_str());
}
