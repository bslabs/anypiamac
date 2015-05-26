// Functions for the <see cref="Trace"/> class to trace execution of code.

// $Id: Trace.cpp 1.7 2011/08/19 15:57:45EDT 044579 Development  $

#if !defined(SHORTFILENAMES)
#include "Trace.h"
#else
#include "traceh"
#endif

std::vector< TraceListener* > Trace::listeners;

/// <summary>Close all the <see cref="TraceListener"/> objects.</summary>
void Trace::close()
{
  for (size_t i = 0; i < listeners.size(); i++)
    listeners[i]->close();
}

/// <summary>Flush all the <see cref="TraceListener"/> objects.</summary>
void Trace::flush()
{
  for (size_t i = 0; i < listeners.size(); i++)
    listeners[i]->flush();
}

/// <summary>Write a string of output, without a newline, to all the
/// <see cref="TraceListener"/> objects.</summary>
///
/// <param name="str">The string to write.</param>
void Trace::write( const std::string& str )
{
  for (size_t i = 0; i < listeners.size(); i++)
    listeners[i]->write(str);
}

/// <summary>Write the characters of output, without a newline, to all the
/// <see cref="TraceListener"/> objects.</summary>
///
/// <param name="str">The characters to write.</param>
void Trace::write( const char *str )
{
  for (size_t i = 0; i < listeners.size(); i++)
    listeners[i]->write(str);
}

/// <summary>Write a string of output, with a newline, to all the
/// <see cref="TraceListener"/> objects.</summary>
///
/// <param name="str">The string to output.</param>
void Trace::writeLine( const std::string& str )
{
  for (size_t i = 0; i < listeners.size(); i++)
    listeners[i]->writeLine(str);
}

/// <summary>Write the characters of output, with a newline, to all the
/// <see cref="TraceListener"/> objects.</summary>
///
/// <param name="str">The characters to output.</param>
void Trace::writeLine( const char *str )
{
  for (size_t i = 0; i < listeners.size(); i++)
    listeners[i]->writeLine(str);
}
