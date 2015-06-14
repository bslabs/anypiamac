// $Id: StrmoutLog.cpp,v 1.2 2005/10/27 15:17:59 djolson Exp $
//
// Functions to print output log messages to a stream.

#if defined(_WIN32)
#pragma warning(disable:4786)
#endif

#include <iostream>
#include <fstream>
#include "StrmoutLog.h"
#include "PiaException.h"

using namespace std;

// Description: Constructor.
//
// Arguments:
//   ostr: Stream to use for output.
StrmoutLog::StrmoutLog( std::ostream *ostr ) : OutputLog(), m_ostr(ostr)
{
   // reset stream pointer if none passed in
   if (ostr == NULL)
      m_ostr = &cerr;
}

// Description: Destructor.
StrmoutLog::~StrmoutLog()
{ }
