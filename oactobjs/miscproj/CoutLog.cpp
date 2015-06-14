// $Id: CoutLog.cpp,v 1.2 2005/10/26 20:24:10 djolson Exp $
//
// Functions in class to print log messages to cout.

#include <iostream>
#include "CoutLog.h"

using namespace std;

// Description: Constructor.
CoutLog::CoutLog() : OutputLog()
{ }

// Description: Destructor.
CoutLog::~CoutLog()
{ }

// Description: Writes one line to cout.
//
// Remarks: Writes one line to cout. Note that this overrides
//   a virtual function in the OutputLog class.
//
// Arguments:
//   szString: String to write.
//   t: Current time.
void CoutLog::DoLogString( const char *szString, time_t t )
{
   cout << szString << endl;
}
