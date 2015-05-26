// $Id: BitAnnualBase.h,v 1.3 2007/05/14 18:15:37 djolson Exp $
//
// Declarations needed for class to manage an array of annual bits for Windows.

#pragma once

#include <bitset>
// Description: Size to use for bit array in BitAnnual (from 1937 to 2100).
const int MAX_BITSET_SIZE = 164;
// Description: Bit array used in BitAnnual (a std::bitset).
typedef std::bitset< MAX_BITSET_SIZE > BitAnnualData;
