// $Id: NulloutLog.h,v 1.2 2005/10/04 21:45:43 djolson Exp $
//
// Declaration of class to ignore log messages.

#ifndef NULLOUTLOG_H
#define NULLOUTLOG_H

#include "OutputLog.h"

// {group:Miscellaneous Classes}
// Description: Handles output messages by ignoring them.
//
// Remarks: Handles output messages by ignoring them.
class NulloutLog : public OutputLog
{
   public:
      NulloutLog();
      ~NulloutLog();
      void DoLogString( const char *szString, time_t t );
};

#endif  // NULLOUTLOG_H
