// Functions to handle PIA formula AIME portions.
//
// $Id: PortionAime.cpp 1.2 2017/11/14 12:57:31EST 277133 Development  $


#include <cmath>  // for floor
#include <algorithm>
#include "PortionAime.h"

using namespace std;

/// <summary>Constructor.</summary>
///
/// <param name="newNumPart">Number of AIME portions in formula.</param>
PortionAime::PortionAime( int newNumPart)
{
	if (newNumPart > MAXPART) {
    throw PiaException("Number of AIME portions is too large in PortionAime::PortionAime.");
  }
  numPart = newNumPart;
  fill(begin(), begin() + MAXPART, 0.0);
}

/// <summary>Destroys bend point array.</summary>
PortionAime::~PortionAime()
{ }

/// <summary>Zeroes out all values.</summary>
void PortionAime::deleteContents()
{
   fill(begin(), begin() + MAXPART, 0.0);
}

/// <summary>Sets number of AIME portions.</summary>
///
/// <param name="newNumPart">Number of AIME portions in formula.</param>
void PortionAime::setNumPart( int newNumPart )
{
   if (newNumPart > MAXPART) {
     throw PiaException("Number of AIME portions is too large in PortionAime::setNumPart.");
   }
   numPart = newNumPart;
   fill(begin(), begin() + MAXPART, 0.0);
}