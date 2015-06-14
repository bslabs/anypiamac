// $Id: Floor.h,v 1.2 2005/10/04 21:45:45 djolson Exp $
//
// Declarations needed for floor function for Windows.

#pragma once
#ifndef __FLOOR_H
#define __FLOOR_H

#include <cmath>
using namespace std;

// Description: Calls the floor function.
//
// Returns: The greatest integer in the argument.
//
// Arguments:
//   amount: Amount to be floored.
inline double FLOOR( double amount )
{ return floor(amount); }

#endif  // __FLOOR_H
