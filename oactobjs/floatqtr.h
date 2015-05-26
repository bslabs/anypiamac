// Declarations for the <see cref="FloatQuarter"/> class to manage an
// array of quarterly floats.

// $Id: floatqtr.h 1.17 2011/08/11 16:23:33EDT 044579 Development  $

#pragma once

#include <vector>
class FloatMonth;
class QtrYear;

/// <summary>Manages an array of quarterly floats.</summary>
///
/// <remarks>Manages an array of quarterly floats.</remarks>
class FloatQuarter
{
protected:
  /// <summary>Float data.</summary>
  std::vector< float > theData;
private:
  /// <summary>Width of output field.</summary>
  int width;
  /// <summary>Base year of data.</summary>
  int baseYear;
  /// <summary>Last year of data.</summary>
  int lastYear;
  /// <summary>Number of decimal places on output.</summary>
  int precision;
  /// <summary>Width of output line.</summary>
  int lineWidth;
public:
  FloatQuarter( int newBaseYear, int newLastYear );
  float anninc( const QtrYear& qtrYear ) const;
  float anninc3( const QtrYear& qtrYear ) const;
  void assign( const FloatMonth& floatMonth, const QtrYear& firstQtrYear,
    const QtrYear& lastQtrYear );
  void assign( const FloatQuarter& floatQuarter, int firstYeart,
    int lastYeart );
  void assign( float newData, int firstYeart, int lastYeart );
  float average( int year ) const;
  float getAfter( const QtrYear& qtrYear ) const;
  float getAt( const QtrYear& qtrYear ) const;
  /// <summary>Returns base year.</summary>
  ///
  /// <returns>Base year.</returns>
  int getBaseYear() const { return(baseYear); }
  float getBefore( const QtrYear& qtrYear ) const;
  /// <summary>Returns last year.</summary>
  ///
  /// <returns>Last year.</returns>
  int getLastYear() const { return(lastYear); }
  /// <summary>Returns output line width.</summary>
  ///
  /// <returns>Output line width.</returns>
  int getLineWidth() const { return lineWidth; }
  /// <summary>Returns field width on output.</summary>
  ///
  /// <returns>Field width on output.</returns>
  int getWidth() const { return width; }
  void multiply( const FloatQuarter& floatQuarter, int firstYeart,
    int lastYeart );
  void read( std::istream& in, int firstYeart, int lastYeart );
  void setAt( const QtrYear& qtrYear, float newData );
  /// <summary>Initializes output line width.</summary>
  ///
  /// <param name="newLineWidth">New output line width.</param>
  void setLineWidth( int newLineWidth ) { lineWidth = newLineWidth; }
  /// <summary>Initializes precision on output.</summary>
  ///
  /// <param name="newPrecision">New value of precision on output.</param>
  void setPrecision( int newPrecision ) { precision = newPrecision; }
  /// <summary>Initializes field width.</summary>
  ///
  /// <param name="newWidth">New value of field width.</param>
  void setWidth( int newWidth ) { width = newWidth; }
  void write( std::ostream& out, int firstYeart, int lastYeart );
};
