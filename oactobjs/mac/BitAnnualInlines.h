// $Id: BitAnnualInlines.h,v 1.3 2007/05/14 18:15:37 djolson Exp $
//
// Inline functions for class to manage an array of annual bits for Windows.

#pragma once

// Description: Returns one constant value of data.
//
// Returns: One constant value of data.
//
// Throws: PiaException of type PIA_IDS_BITANN1 if year is out of range (only
//   in debug mode).
//
// Arguments:
//   year: Year for which data is desired.
inline bool BitAnnual::getBit( int year ) const
{
#if !defined(NDEBUG)
   if (year < baseYear || year > lastYear) {
      throw PiaException(PIA_IDS_BITANN1);
   }
#endif
   return theData.test(year - baseYear);
}

// Description: Sets one value of data.
//
// Throws: PiaException of type PIA_IDS_BITANN1 if year is out of range (only
//   in debug mode).
//
// Arguments:
//   year: Year for which data is to be set.
//   bit: Value to set (0 for false, any other value for true).
inline void BitAnnual::setBit( int year, int bit )
{
#if !defined(NDEBUG)
   if (year < baseYear || year > lastYear) {
      throw PiaException(PIA_IDS_BITANN1);
   }
#endif
   theData.set(year - baseYear, (bit > 0));
}

// Description: Returns true if any bit is set, false otherwise.
//
// Returns: True if any bit is set, false otherwise.
inline bool BitAnnual::any() const
{ return theData.any(); }

// Description: Zeroes out all data values.
inline void BitAnnual::deleteContents()
{ theData.reset(); }
