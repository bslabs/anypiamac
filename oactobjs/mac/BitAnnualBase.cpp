// $Id: BitAnnualBase.cpp,v 1.3 2007/05/14 18:15:36 djolson Exp $
//
// Functions to handle BitAnnual arrays for Windows.

#include "oactcnst.h"
#include "BitAnnual.h"

using namespace std;

// GENERAL NOTE:
// When adding functions to this class, or changing the implementation
// of existing functions, be careful not to use square brackets ( [] )
// to access 'data' unless you're really sure of what you're doing.
// This is because there are different implementations of this class
// depending on whether you're using NT or Linux. Instead, use the
// setBit() and getBit() functions.

// Description: Constructor.
//
// Remarks: The version with two integer arguments initializes the first and
// last years and sets all data values to false. The version with a BitAnnual
// argument copies all values from that argument to this instance.
//
// Arguments:
//   newBaseYear: New base year.
//   newLastYear: New last year.
//
// Remarks: On the linux system, data is a vector of BitPacked's -- so the
// constructor needs to know how many to create. On other systems (which
// conform to the standard and have a BitSet class built in), data is a
// BitSet and doesn't need any parameters because the default constructor
// initializes all of the bits to zero.
BitAnnual::BitAnnual( int newBaseYear, int newLastYear ) :
theData(), baseYear(newBaseYear), lastYear(newLastYear)
{ }

// Arguments:
//   bitAnnual: The BitAnnual to copy from.
BitAnnual::BitAnnual( const BitAnnual& bitAnnual ) :
theData(bitAnnual.theData), baseYear(bitAnnual.baseYear),
lastYear(bitAnnual.lastYear)
{ }
