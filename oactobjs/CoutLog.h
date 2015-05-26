// $Id: CoutLog.h,v 1.2 2005/10/04 21:45:42 djolson Exp $
//
// Declaration of class to print log messages to cout.

#ifndef COUTLOG_H
#define COUTLOG_H

#include "OutputLog.h"

// {group:Miscellaneous Classes}
// Description: Handles output messages going to cout.
//
// Remarks: Handles output messages going to cout.
class CoutLog : public OutputLog
{
   public:
      CoutLog();
      virtual ~CoutLog();
      void DoLogString( const char *szString, time_t t );
};

#endif  // COUTLOG_H
