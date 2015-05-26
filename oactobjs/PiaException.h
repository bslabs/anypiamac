// Declarations for the <see cref="PiaException"/> class to manage an
// exception in the piadata library.

// $Id: PiaException.h 1.16 2011/08/19 15:57:43EDT 044579 Development  $

#pragma once

#include <exception>
#include <string>

/// <summary>Manages an exception in the piadata library.</summary>
///
/// <remarks>The exception could be created with a number, which could be
/// used to look up an associated string in a string table. Alternatively,
/// the exception could be created by a string, in which case the
/// associated number is set to zero.</remarks>
class PiaException : public std::exception
{
private:
  /// <summary>Number of exception.</summary>
  int exceptionNum;
  /// <summary>Message for this exception.</summary>
  std::string theMessage;
public:
  PiaException( int newNumber ) throw();
  PiaException( const std::string& newMessage ) throw();
  PiaException( const PiaException& piaException ) throw();
  /// <summary>Destructor.</summary>
  ///
  /// <remarks>Does nothing.</remarks>
  ~PiaException() throw() { }
  /// <summary>Returns number of this exception.</summary>
  ///
  /// <returns>Number of this exception.</returns>
  int getNumber() const throw() { return exceptionNum; }
  /// <summary>Returns string with message about this error.</summary>
  ///
  /// <remarks>Note that this overrides a virtual function in the exception
  /// class.</remarks>
  ///
  /// <returns>String with message about this error.</returns>
  const char *what() const throw() { return theMessage.c_str(); }
private:
  PiaException& operator=( const PiaException& );
};
