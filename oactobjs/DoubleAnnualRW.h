// Declarations for the <see cref="DoubleAnnualRead"/> and
// <see cref="DoubleAnnualWrite"/> classes to read or write an array of
// annual doubles.
//
// $Id: DoubleAnnualRW.h 1.3 2011/08/11 15:21:16EDT 044579 Development  $

#pragma once

#include <iosfwd>
class DoubleAnnual;

/// <summary>Reads a <see cref="DoubleAnnual"/>.</summary>
///
/// <remarks>If necessary, we could create FloatAnnualRead and IntAnnualRead
/// for similar classes that get <see cref="FloatAnnual"/>'s and
/// <see cref="IntAnnual"/>'s.</remarks>
///
/// <seealso cref="FloatAnnual"/>
/// <seealso cref="IntAnnual"/>
class DoubleAnnualRead
{
private:
  /// <summary>Vector of doubles to get.</summary>
  DoubleAnnual& theData;
private:
  DoubleAnnualRead operator=( const DoubleAnnualRead& );
public:
  DoubleAnnualRead( DoubleAnnual& newData );
  void read( std::istream& in, int firstYeart, int lastYeart );
};

/// <summary>Writes a <see cref="DoubleAnnual"/>.</summary>
///
/// <remarks>If necessary, we could create FloatAnnualWrite and IntAnnualWrite
/// for similar classes that output <see cref="FloatAnnual"/>'s and
/// <see cref="IntAnnual"/>'s.</remarks>
///
/// <seealso cref="FloatAnnual"/>
/// <seealso cref="IntAnnual"/>
class DoubleAnnualWrite
{
private:
  /// <summary>Vector of doubles to write.</summary>
  const DoubleAnnual& theData;
  /// <summary>Width of field when writing to an output file.</summary>
  const int width;
  /// <summary>Number of decimal places when writing to an output file.
  /// </summary>
  const int precision;
  /// <summary>Width of output line when writing to an output file.</summary>
  const int lineWidth;
private:
  DoubleAnnualWrite operator=( const DoubleAnnualWrite& );
public:
  DoubleAnnualWrite( const DoubleAnnual& newData, int newWidth,
    int newPrecision, int newLineWidth );
  /// <summary>Returns output line width.</summary>
  ///
  /// <returns>Output line width.</returns>
  int getLineWidth() const { return lineWidth; }
  /// <summary>Returns precision of output field.</summary>
  ///
  /// <returns>Precision of output field.</returns>
  int getPrecision() const { return precision; }
  /// <summary>Returns width of output field.</summary>
  ///
  /// <returns>Width of output field.</returns>
  int getWidth() const { return width; }
  void write( std::ostream& out, int firstYeart, int lastYeart ) const;
};
