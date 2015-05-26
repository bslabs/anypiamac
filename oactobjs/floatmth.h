// Declarations for the <see cref="FloatMonth"/> class to manage an array
// of monthly floats.

// $Id: floatmth.h 1.19 2011/08/11 16:23:33EDT 044579 Development  $

#pragma once

#include <vector>
class DateMoyr;
class QtrYear;

/// <summary>Manages an array of monthly floats.</summary>
///
/// <remarks>Manages an array of monthly floats.</remarks>
class FloatMonth
{
protected:
  /// <summary>Float array.</summary>
  std::vector< float > theData;
private:
  /// <summary>Width of field for output.</summary>
  int width;
  /// <summary>Base year of data.</summary>
  int baseYear;
  /// <summary>Last year of data.</summary>
  int lastYear;
  /// <summary>Number of decimal places for output.</summary>
  int precision;
  /// <summary>Width of output line.</summary>
  int lineWidth;
public:
  FloatMonth( int newBaseYear, int newLastYear );
  /// <summary>Destructor.</summary>
  ///
  /// <remarks>Does nothing.</remarks>
  ~FloatMonth() { }
  void assign( const FloatMonth& newData, int firstYeart, int lastYeart );
  void assign( float newFloat, int firstYeart, int lastYeart );
  float annAvgCal( int year ) const;
  float getAt( const DateMoyr& dateMoyr ) const;
  /// <summary>Returns base year.</summary>
  ///
  /// <returns>Base year.</returns>
  int getBaseYear() const { return(baseYear); }
  /// <summary>Returns last year.</summary>
  ///
  /// <returns>Last year.</returns>
  int getLastYear() const { return(lastYear); }
  /// <summary>Returns line width for output.</summary>
  ///
  /// <returns>Line width for output.</returns>
  int getLineWidth() const { return lineWidth; }
  /// <summary>Returns precision for output.</summary>
  ///
  /// <returns>Precision for output.</returns>
  int getPrecision() const { return precision; }
  /// <summary>Returns field width for output.</summary>
  ///
  /// <returns>Field width for output.</returns>
  int getWidth() const { return width; }
  int nmonCal( int year ) const;
  float qtrAvgCal( const QtrYear& qtrYear ) const;
  void read( std::istream& in, int firstYeart, int lastYeart );
  void setAt( const DateMoyr& dateMoyr, float newData );
  /// <summary>Initializes line width for output.</summary>
  ///
  /// <param name="newLineWidth">New line width for output.</param>
  void setLineWidth( int newLineWidth ) { lineWidth = newLineWidth; }
  /// <summary>Initializes precision for output.</summary>
  ///
  /// <param name="newPrecision">New value of precision for output.</param>
  void setPrecision( int newPrecision ) { precision = newPrecision; }
  /// <summary>Initializes field width for output.</summary>
  //
  /// <param name="newWidth">New value of field width for output.</param>
  void setWidth( int newWidth ) { width = newWidth; }
  void write( std::ostream& out, int firstYeart, int lastYeart );
};
