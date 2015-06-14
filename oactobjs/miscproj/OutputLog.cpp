// $Id: OutputLog.cpp,v 1.2 2005/10/26 20:24:10 djolson Exp $
//
// Functions in generic class to print output log messages.

#include "OutputLog.h"
#include "PiaException.h"

// Pointer to active OutputLog.
OutputLog *OutputLog::ms_pLogger;

// Description: Constructor.
OutputLog::OutputLog()
{ }

// Description: Destructor.
OutputLog::~OutputLog()
{ }

// Description: Writes one line to output logfile.
//
// Arguments:
//   level: Type of message.
//   szString: String to write.
//   t: Current time.
void OutputLog::DoLog( wxLogLevel level, const char *szString, time_t t )
{
   switch (level) {
   case wxLOG_Message:
   default:
      DoLogString(szString, t);
      break;
   }
}

// Description: Writes one line to output logfile.
//
// Remarks: The version in DoLogString simply throws an exception. A child
//   class must override this function.
//
// Arguments:
//   szString: String to write.
//   t: Current time.
void OutputLog::DoLogString( const char *szString, time_t t )
{
   throw PiaException("DoLogString must be overridden if it's called.");
}

// Description: Global function to write out message to default logfile.
//
// Arguments:
//   szFormat: Format to use (should always be "%s").
//   message: Message to write.
void wxLogMessage( const char *szFormat, const char *message )
{
   OutputLog::OnLog(wxLOG_Message, message, 0L);
}

// Description: Global function to write out message to default logfile.
//
// Arguments:
//   message: Message to write.
void logMessage( const char *message )
{
   OutputLog::OnLog(wxLOG_Message, message, 0L);
}
