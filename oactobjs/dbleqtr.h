// Declarations for the <see cref="DoubleQuarter"/> class to manage an
// array of quarterly doubles.

// $Id: dbleqtr.h 1.10 2011/08/11 15:21:18EDT 044579 Development  $

#pragma once

#include <vector>
class DoubleMonth;
class QtrYear;

/// <summary>Manages an array of quarterly doubles.</summary>
///
/// <remarks>Manages an array of quarterly doubles.</remarks>
class DoubleQuarter
{
protected:
  /// <summary>Double data.</summary>
  std::vector< double > theData;
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
  DoubleQuarter( int newBaseYear, int newLastYear );
  /// <summary>Destructor.</summary>
  ///
  /// <remarks>Does nothing.</remarks>
  ~DoubleQuarter() { }
  double anninc( const QtrYear& qtrYear ) const;
  double anninc3( const QtrYear& qtrYear ) const;
  void assign( const DoubleMonth& doubleMonth,
    const QtrYear& firstQtrYear, const QtrYear& lastQtrYear );
  void assign( const DoubleQuarter& doubleQuarter, int firstYeart,
    int lastYeart );
  void assign( double newData, int firstYeart, int lastYeart );
  double average( int year ) const;
  double getAfter( const QtrYear& qtrYear ) const;
  double getAt( const QtrYear& qtrYear ) const;
  /// <summary>Returns base year.</summary>
  ///
  /// <returns>Base year.</returns>
  int getBaseYear() const { return(baseYear); }
  double getBefore( const QtrYear& qtrYear ) const;
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
  void multiply( const DoubleQuarter& doubleQuarter, int firstYeart,
    int lastYeart );
  void read( std::istream& in, int firstYeart, int lastYeart );
  void setAt( const QtrYear& qtrYear, double newData );
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
