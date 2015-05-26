// Declarations for the <see cref="TextWriterTraceListener"/> class that
// listens for <see cref="Trace"/> output and writes to an ostream.

// $Id: TextWriterTraceListener.h 1.7 2011/08/19 15:57:46EDT 044579 Development  $

#pragma once

#include <iosfwd>
#if !defined(SHORTFILENAMES)
#include "TraceListener.h"
#else
#include "trlish"
#endif

/// <summary>An output-stream-based class that listens for <see cref="Trace"/>
/// output.</summary>
///
/// <remarks>This is a concrete implementation of the
/// <see cref="TraceListener"/> class, with a private ostream member. It can be
/// used with cout, cerr, or a user-created output stream, which is specified
/// in the constructor.</remarks>
///
/// <seealso cref="Trace"/>
class TextWriterTraceListener : public TraceListener
{
private:
  /// <summary>Logfile to use.</summary>
  std::ostream *strm;
public:
  TextWriterTraceListener( std::ostream *newStream );
  void close();
  void flush();
  void write( const std::string& str );
  void write( const char *str );
  void writeLine( const std::string& str );
  void writeLine( const char *str );
  ~TextWriterTraceListener();
};
