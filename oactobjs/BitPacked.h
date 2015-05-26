// $Id: BitPacked.h,v 1.2 2005/10/04 21:45:41 djolson Exp $
//
// Definition of class to manage 32 annual bits (one word) as bools.

#ifndef BITPACKED_H
#define BITPACKED_H

#include <climits>

// Description: Number of bits in an unsigned int.
const int U_NUMBITS = sizeof(unsigned int) * CHAR_BIT;

// {group:Miscellaneous Classes}
// Description: Holds 32 annual bits (as bools) in one word (4 bytes).
//
// Remarks: This class is used in the Linux version of the library, where a
//   vector of BitPacked structures is used to hold the array of annual bits.
//   This class is unused in the NT version of the library.
class BitPacked
{
   protected:
      // Array of bits (bools) as unsigned int.
      unsigned bits;
   public:
      BitPacked();
      bool any() const;
      void deleteContents();
      bool get( int index ) const;
      void set( int index, int value );
};

#endif   // BITPACKED_H
