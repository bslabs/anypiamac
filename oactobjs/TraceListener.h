// Declarations for the <see cref="TraceListener"/> class - parent of classes
// that listen for <see cref="Trace"/> output.

// $Id: TraceListener.h 1.11 2011/08/19 15:57:42EDT 044579 Development  $

#pragma once

#include <string>

/// <summary>The parent of classes that listen for <see cref="Trace"/> output.
/// </summary>
///
/// <remarks>Child classes are DefaultTraceListener (not yet implemented) and
/// <see cref="TextWriterTraceListener"/>.</remarks>
///
/// <seealso cref="TextWriterTraceListener"/>
/// <seealso cref="Trace"/>
class TraceListener
{
protected:
  TraceListener();
public:
  /// <summary>Closes this instance.</summary>
  ///
  /// <remarks>The default version does nothing.</remarks>
  virtual void close() { }
  /// <summary>Flushes the output of this instance.</summary>
  ///
  /// <remarks>The default version does nothing.</remarks>
  virtual void flush() { }
  /// <summary>Write a string of output, without a newline.</summary>
  ///
  /// <param name="str">The string to write.</param>
  virtual void write( const std::string& str ) = 0;
  /// <summary>Write the characters of output, without a newline.</summary>
  ///
  /// <param name="str">The characters to write.</param>
  virtual void write( const char *str ) = 0;
  /// <summary>Write a string of output, with a newline.</summary>
  ///
  /// <param name="str">The string to output.</param>
  virtual void writeLine( const std::string& str ) = 0;
  /// <summary>Write the characters of output, with a newline.</summary>
  ///
  /// <param name="str">The characters to output.</param>
  virtual void writeLine( const char *str ) = 0;
  virtual ~TraceListener();
};
