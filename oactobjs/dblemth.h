// Declarations for the <see cref="DoubleMonth"/> class to manage an array
// of monthly doubles.

// $Id: dblemth.h 1.10 2011/08/11 15:21:18EDT 044579 Development  $

#pragma once

#include <vector>
class DateMoyr;
class QtrYear;

/// <summary>Manages an array of monthly doubles.</summary>
///
/// <remarks>Manages an array of monthly doubles.</remarks>
class DoubleMonth
{
protected:
  /// <summary>Double array.</summary>
  std::vector< double > theData;
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
  DoubleMonth( int newBaseYear, int newLastYear );
  /// <summary>Destructor.</summary>
  //
  /// <remarks>Does nothing.</remarks>
  ~DoubleMonth() { }
  void assign( const DoubleMonth& newData, int firstYeart, int lastYeart );
  void assign( double newDouble, int firstYeart, int lastYeart );
  double annAvgCal( int year ) const;
  double getAt( const DateMoyr& dateMoyr ) const;
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
  double qtrAvgCal( const QtrYear& qtrYear ) const;
  void read( std::istream& in, int firstYeart, int lastYeart );
  void setAt( const DateMoyr& dateMoyr, double newData );
  /// <summary>Initializes line width for output.</summary>
  ///
  /// <param name="newLineWidth">New line width for output.</param>
  void setLineWidth( int newLineWidth ) { lineWidth = newLineWidth; }
  /// <summary>Initializes precision for output.</summary>
  ///
  /// <param name="newPrecision">New value of precision for output.</param>
  void setPrecision( int newPrecision ) { precision = newPrecision; }
  /// <summary>Initializes field width for output.</summary>
  ///
  /// <param name="newWidth">New value of field width for output.</param>
  void setWidth( int newWidth ) { width = newWidth; }
  void write( std::ostream& out, int firstYeart, int lastYeart );
};
