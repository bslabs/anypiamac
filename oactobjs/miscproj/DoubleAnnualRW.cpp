// Functions for the <see cref="DoubleAnnualRead"/> and
// <see cref="DoubleAnnualWrite"/> classes to read or write an array of
// annual doubles.
//
// $Id: DoubleAnnualRW.cpp 1.5 2017/08/29 09:03:51EDT 277133 Development  $

#include <iomanip>
#include <algorithm>
#include "DoubleAnnualRW.h"
#include "PiaException.h"
#include "Resource.h"
#include "dbleann.h"

using namespace std;

/// <summary>Constructor.</summary>
///
/// <param name="newData">Vector of doubles to get.</param>
DoubleAnnualRead::DoubleAnnualRead( DoubleAnnual& newData ) :
theData(newData)
{ }

/// <summary>Reads array from an open stream.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_DBLEANN2"/> if unable to read specified fields.
/// </exception>
///
/// <param name="in">Input stream.</param>
/// <param name="firstYeart">First year to read.</param>
/// <param name="lastYeart">Last year to read.</param>
void DoubleAnnualRead::read( std::istream& in, int firstYeart, int lastYeart )
{
  double temp;  // temporary value
  for (int yr = firstYeart; yr <= lastYeart; yr++) {
    if (!(in >> temp))
      throw PiaException(PIA_IDS_DBLEANN2);
    theData[yr] = temp;
  }
}

/// <summary>Constructor.</summary>
///
/// <param name="newData">Vector of doubles to output.</param>
/// <param name="newWidth">Width of field when writing to an output file.
/// </param>
/// <param name="newPrecision">Number of decimal places when writing to an
/// output file.</param>
/// <param name="newLineWidth">Width of output line when writing to an output
/// file.</param>
DoubleAnnualWrite::DoubleAnnualWrite( const DoubleAnnual& newData,
int newWidth, int newPrecision, int newLineWidth ) :
theData(newData), width(newWidth), precision(newPrecision),
lineWidth(newLineWidth)
{ }

/// <summary>Writes array to an open stream.</summary>
///
/// <param name="out">Output stream.</param>
/// <param name="firstYeart">First year to write.</param>
/// <param name="lastYeart">Last year to write.</param>
void DoubleAnnualWrite::write( std::ostream& out, int firstYeart,
int lastYeart ) const
{
  const int year1 = max(theData.getBaseYear(), firstYeart);
  const int year2 = min(theData.getLastYear(), lastYeart);
  const int numperline = lineWidth / width;  // number of numbers per line
  out.precision(precision);
  for (int yr = year1; yr <= year2; yr++) {
    out << setw(width) << theData[yr];
    if ((yr - firstYeart + 1) % numperline == 0)
      out << endl;
  }
  if ((year2 - year1 + 1) % numperline != 0)
    out << endl;
}
