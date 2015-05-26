// Declarations for the <see cref="AnnualArrays"/> class of static functions
// using annual arrays.
//
// $Id: AnnualArrays.h 1.3 2011/08/11 15:21:15EDT 044579 Development  $

#pragma once

class DoubleAnnual;
class IntAnnual;

/// <summary>Functions using annual arrays.</summary>
///
/// <remarks>These are used in the inttab library.</remarks>
class AnnualArrays
{
private:
  AnnualArrays();
  ~AnnualArrays();
public:
  static void add( DoubleAnnual& data1, const DoubleAnnual& data2,
    int firstYear, int lastYear );
  static DoubleAnnual convert( const IntAnnual& intAnnual );
  static void divide( DoubleAnnual& data1, const DoubleAnnual& data2,
    int firstYear, int lastYear );
  static void multiply( DoubleAnnual& data1, const DoubleAnnual& data2,
    int firstYear, int lastYear );
  static void multiply( DoubleAnnual& data1, double factor, int firstYear,
    int lastYear );
  static void subtract( DoubleAnnual& data1, const DoubleAnnual& data2,
    int firstYear, int lastYear );
};
