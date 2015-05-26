// Functions for the <see cref="TextWriterTraceListener"/> class that
// listens for <see cref="Trace"/> output and writes to an ostream.

// $Id: TextWriterTraceListener.cpp 1.10 2011/08/19 15:57:43EDT 044579 Development  $

#include <ostream>
#if !defined(SHORTFILENAMES)
#include "TextWriterTraceListener.h"
#else
#include "twtrlish"
#endif

using namespace std;

/// <summary>Creates this instance.</summary>
///
/// <remarks>Associates the argument with the internal output stream.
/// </remarks>
///
/// <param name="newStream">The output stream to use as the logfile.</param>
TextWriterTraceListener::TextWriterTraceListener( std::ostream *newStream ) :
TraceListener(), strm(newStream)
{ }

/// <summary>Destroys this instance.</summary>
TextWriterTraceListener::~TextWriterTraceListener()
{ }

/// <summary>Closes things.</summary>
void TextWriterTraceListener::close()
{ }

/// <summary>Flushes the output.</summary>
void TextWriterTraceListener::flush()
{
  strm->flush();
}

/// <summary>Write a string of output, without a newline.</summary>
///
/// <param name="str">The string to output.</param>
void TextWriterTraceListener::write( const std::string& str )
{
  *strm << str;
}

/// <summary>Write the characters of output, without a newline.</summary>
///
/// <param name="str">The characters to output.</param>
void TextWriterTraceListener::write( const char *str )
{
  *strm << str;
}

/// <summary>Write a string of output, with a newline.</summary>
///
/// <param name="str">The string to output.</param>
void TextWriterTraceListener::writeLine( const std::string& str )
{
  *strm << str << endl;
}

/// <summary>Write the characters of output, with a newline.</summary>
///
/// <param name="str">The characters to output.</param>
void TextWriterTraceListener::writeLine( const char *str )
{
  *strm << str << endl;
}
