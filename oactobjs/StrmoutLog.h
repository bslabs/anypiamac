// $Id: StrmoutLog.h,v 1.2 2005/10/04 21:45:43 djolson Exp $
//
// Declaration of class to print output log messages to a stream.

#ifndef STRMOUTLOG_H
#define STRMOUTLOG_H

#include <iosfwd>
#include "OutputLog.h"

// {group:Miscellaneous Classes}
// Description: Manages an output log file using a stream.
//
// Remarks: Manages an output log file using a stream.
class StrmoutLog : public OutputLog
{
  protected:
    // Logfile to use.
    std::ostream *m_ostr;
  public:
    StrmoutLog( std::ostream *ostr );
    ~StrmoutLog();
    void DoLogString( const char *szString, time_t t );
};

// Description: Writes one line to output logfile.
//
// Remarks: Note that this overrides a virtual function in the OutputLog class.
//
// Arguments:
//   szString: String to write.
//   t: Current time.
inline void StrmoutLog::DoLogString( const char *szString, time_t t )
{ (*m_ostr) << szString << std::endl; }

#endif  // STRMOUTLOG_H
