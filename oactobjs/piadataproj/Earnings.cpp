// $Id: Earnings.cpp,v 1.2 2005/10/04 21:45:50 djolson Exp $
//
// Functions using earnings to be ordered.

#include <cmath>  // for floor
#include "Earnings.h"

// Description: Constructor.
//
// Remarks: The default constructor initializes the index and earnings to zero.
Earnings::Earnings() : index(0u), earn(0.)
{ }

// Arguments:
//   newEarn: New earnings. If omitted, earnings are set to zero.
Earnings::Earnings( double newEarn ) : index(0u), earn(newEarn)
{ }

// Arguments:
//   newIndex: New index. If omitted, index is set to zero.
//   newEarn: New earnings. If omitted, earnings are set to zero.
Earnings::Earnings( unsigned newIndex, double newEarn ) : index(newIndex),
earn(newEarn)
{ }
