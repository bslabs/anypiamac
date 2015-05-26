// $Id: OutputLog.h,v 1.2 2005/10/04 21:45:43 djolson Exp $
//
// Declaration of generic class to print output log messages.

#ifndef OUTPUTLOG_H
#define OUTPUTLOG_H

#include <string>
#include <ctime>
#if defined(__POWERPC__) || defined(__MC68K__)
// Description: Redefine time_t for machines that put it in std.
//
// Remarks: The time_t type is in std on the Mac.
#define time_t std::time_t
#endif

// Description: Type of log message (same as unsigned long).
typedef unsigned long wxLogLevel;

// Description: Different standard log levels (you may also define your own).
enum LOG_LEVEL
{
    wxLOG_FatalError, // program can't continue, abort immediately
    wxLOG_Error,      // a serious error, user must be informed about it
    wxLOG_Warning,    // user is normally informed about it but may be ignored
    wxLOG_Message,    // normal message (i.e. normal output of a non GUI app)
    wxLOG_Info,       // informational message (a.k.a. 'Verbose')
    wxLOG_Status,     // informational: might go to the status line of GUI app
    wxLOG_Debug,      // never shown to the user, disabled in release mode
    wxLOG_Trace,      // trace messages are also only enabled in debug mode
    wxLOG_Progress,   // used for progress indicator (not yet)
    wxLOG_User = 100  // user defined levels start here
};

// {group:Miscellaneous Classes}
// Description: A generic output log file.
//
// Remarks: This class is abstract; a derived class must define a DoLogString
//   function.  Class CoutLog is a derived class that simply writes to cout.
class OutputLog
{
   private:
      // Currently active log.
      static OutputLog *ms_pLogger;
   public:
      OutputLog();
      virtual ~OutputLog();
      virtual void DoLog( wxLogLevel level, const char *szString, time_t t );
      virtual void DoLogString( const char *szString, time_t t );
      static OutputLog *GetActiveTarget();
      static void OnLog( wxLogLevel level, const char *szString, time_t t );
      static OutputLog *SetActiveTarget( OutputLog *pLogger );
};

// Description: Get current log target.
//
// Returns: Current log target.
inline OutputLog *OutputLog::GetActiveTarget()
{ return ms_pLogger; }

// Description: Change log target.
//
// Returns: Previous log target.
//
// Arguments:
//   pLogger: New log target.
inline OutputLog *OutputLog::SetActiveTarget( OutputLog *pLogger )
{ OutputLog *pOldLogger = ms_pLogger; ms_pLogger = pLogger; return pOldLogger; }

// Description: Writes one line to output logfile.
//
// Arguments:
//   level: Type of message.
//   szString: Message to write.
//   t: Current time.
inline void OutputLog::OnLog( wxLogLevel level, const char *szString, time_t t )
{ OutputLog *pLogger = GetActiveTarget();
  if (pLogger)
     pLogger->DoLog(level, szString, t);
}

void wxLogMessage( const char *szFormat, const char *message );
void logMessage( const char *message );

#endif  // OUTPUTLOG_H
