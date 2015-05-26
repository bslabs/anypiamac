// Functions for the <see cref="PiaException"/> class to manage an
// exception in the piadata library.

// $Id: PiaException.cpp 1.16 2011/08/19 15:57:45EDT 044579 Development  $

#include <sstream>
#if !defined(SHORTFILENAMES)
#include "PiaException.h"
#else
#include "piaexcph"
#endif

using namespace std;

/// <summary>Sets the number to the specified value and initializes the
/// message to "PiaException: number".</summary>
///
/// <param name="newNumber">Number of this exception.</param>
PiaException::PiaException( int newNumber ) throw() :
exception(), exceptionNum(newNumber), theMessage("PiaException: ")
{
  ostringstream strm;
  strm << exceptionNum;
  theMessage += strm.str();
}

/// <summary>Sets the number to 0 and the message to the specified string.
/// </summary>
///
/// <param name="newMessage">Message of this exception.</param>
PiaException::PiaException( const std::string& newMessage ) throw() :
exception(), exceptionNum(0), theMessage(newMessage)
{ }

/// <summary>Copies a <see cref="PiaException"/>.</summary>
///
/// <param name="piaException">The <see cref="PiaException"/> to copy from.
/// </param>
PiaException::PiaException( const PiaException& piaException ) throw() :
exception(), exceptionNum(piaException.exceptionNum),
theMessage(piaException.theMessage)
{ }
