// $Id: NulloutLog.cpp,v 1.2 2005/10/26 20:24:10 djolson Exp $
//
// Functions in class to ignore log messages.

#include "NulloutLog.h"

using namespace std;

// Description: Constructor.
NulloutLog::NulloutLog() : OutputLog()
{ }

// Description: Destructor.
NulloutLog::~NulloutLog()
{ }

// Description: Ignores one line.
//
// Remarks: Ignores one line. Note that this overrides
//   a virtual function in the OutputLog class.
//
// Arguments:
//   szString: String to write.
//   t: Current time.
void NulloutLog::DoLogString( const char *szString, time_t t )
{ }
