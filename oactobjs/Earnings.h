// $Id: Earnings.h,v 1.2 2005/10/04 21:45:42 djolson Exp $
//
// Declarations needed for class to manage earnings to be ordered.

#ifndef __EARNINGS_H
#define __EARNINGS_H

// {group:Calculation Classes}
// Description: Manages annual earnings paired with the index of the
//   year the earnings were earned.
//
// Remarks: Instances of this class are ordered in
//   PiaMethod when calculating an ame or aime.
class Earnings
{
   public:
      // Index of this year of earnings.
      unsigned index;
      // The earnings.
      double earn;
   public:
      Earnings();
      Earnings( double newEarn );
      Earnings( unsigned newIndex, double newEarn );
      bool operator<( const Earnings& earn1 ) const;
};

// Description: Compares earnings with earnings from another structure.
//
// Returns: True if the earnings in this instance are less than the earnings
//   in the argument, false otherwise.
//
// Arguments:
//   earn1: Earnings structure to compare to.
inline bool Earnings::operator<( const Earnings& earn1 ) const
{ return(earn < earn1.earn); }

// Description: Returns the sum of the earnings part of two Earnings
//   structures.
//
// Returns: The sum of the earnings part of two Earnings structures.
//
// Arguments:
//   earn1: First Earnings structure.
//   earn2: Second Earnings structure.
inline double operator+( const Earnings& earn1, const Earnings& earn2 )
{ return(earn1.earn + earn2.earn); }

#endif  // __EARNINGS_H
