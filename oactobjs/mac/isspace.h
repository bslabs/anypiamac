// $Id: isspace.h,v 1.2 2005/10/04 21:45:46 djolson Exp $
//
// Functions to classify a character for Windows.

#pragma once
#ifndef __ISSPACE_H
#define __ISSPACE_H

#include <locale>
// Use the default locale.
static std::locale loc;

// Description: Returns true if argument is a space.
//
// Returns: True if argument is a space.
//
// Remarks: The Windows version uses the C++ isspace function.
//
// Arguments:
//   ch: The character to test.
inline bool ISSPACE( char ch )
{ return std::isspace(ch, loc); }

#endif  // __ISSPACE_H
