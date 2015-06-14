// $Id: CommaBase.h,v 1.2 2005/10/04 21:45:45 djolson Exp $
//
// The Comma functions can use the stringstream functions or the strstream
// functions.

#pragma once
#ifndef __COMMABASE_H
#define __COMMABASE_H    // Prevent multiple #includes

// Use stringstreams for systems that have them.
#include <sstream>
// {secret}
// Description: Use ostringstream for formatting output.
#define OSTREAM std::ostringstream
// {secret}
// Description: No special character for end of ostringstream.
#define ENDS
// {secret}
// Description: No special function to freeze the string.
// Arguments:
//   strm: Stream to freeze.
#define FREEZE(strm)

#endif  // __COMMABASE_H
