// Declarations for the <see cref="Trace"/> class to trace execution of code.

// $Id: Trace.h 1.7 2011/08/19 15:57:46EDT 044579 Development  $

#pragma once

#include <vector>
#if !defined(SHORTFILENAMES)
#include "TraceListener.h"
#else
#include "trlish"
#endif

/// <summary>Controls the logfiles used to trace execution of code.</summary>
///
/// <remarks>This class maintains a vector of <see cref="TraceListener"/>
/// objects. When a <see cref="Trace"/> function is called, the corresponding
/// function is called for each <see cref="TraceListener"/> object. Note that
/// all public functions are static, and there is no public constructor.
/// Simply use the static functions directly, without an instance of
/// <see cref="Trace"/>, i.e. <see cref="Trace::writeLine"/>("abc").</remarks>
class Trace
{
private:
  /// <summary>The vector of <see cref="TraceListener"/> objects.</summary>
  static std::vector< TraceListener* > listeners;
private:
  Trace();
  Trace( const Trace& );
  ~Trace();
  Trace& operator=( const Trace& ) { return *this; }
public:
  static void close();
  static void flush();
  static void write( const std::string& str );
  static void write( const char *str );
  static void writeLine( const std::string& str );
  static void writeLine( const char *str );
  /// <summary>Returns the vector of <see cref="TraceListener"/> objects.
  /// </summary>
  ///
  /// <returns>The vector of <see cref="TraceListener"/> objects.</returns>
  static std::vector< TraceListener* >& getListeners() { return listeners; }
};
