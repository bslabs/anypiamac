// Declarations for the <see cref="ScaledEarnFactors"/> class of scaled
// earnings factors.

// $Id: ScaledEarnFactors.h 1.12 2013/07/17 07:11:22EDT 277133 Development  $

#pragma once

#include <vector>
#include <iosfwd>
class EarnProject;
class AverageWage;

/// <summary>Manages scaled earner amounts.</summary>
///
/// <remarks>The factors are gotten from an open file via the
/// <see cref="read"/> function. They begin at a first age and go through a
/// last age, as specified in the constructor. There are three levels of
/// scaled earnings: high, average, and low.</remarks>
class ScaledEarnFactors
{
public: 
  /// <summary>The enumerated scaled earnings type indicator.</summary>
  enum scaleType {
    SCALEVERYLOW, // Very low scaled earnings.  (Career average at about 25% of the AWI.)
    SCALELOW, // Low scaled earnings.  (Career average at about 40% of the AWI.)
    SCALEAVERAGE, // Average scaled earnings.  (Career average at about 100% of the AWI.)
    SCALEHIGH // High scaled earnings.  (Career average at about 160% of the AWI.)
  };
private:
  /// <summary>Vector of high earner factors.</summary>
  std::vector< double > highFactors;
  /// <summary>Vector of average earner factors.</summary>
  std::vector< double > averageFactors;
  /// <summary>Vector of low earner factors.</summary>
  std::vector< double > lowFactors;
  /// <summary>Vector of low earner factors.</summary>
  std::vector< double > veryLowFactors;
  /// <summary>Width of field when writing to an output file.</summary>
  int fieldWidth;
  /// <summary>Number of decimal places when writing to an output file.
  /// </summary>
  int numDecimals;
  /// <summary>Base age of data.</summary>
  const int firstAge;
  /// <summary>Last age of data.</summary>
  const int lastAge;
public:
  ScaledEarnFactors( int newFirstAge, int newLastAge );
  ~ScaledEarnFactors();
  double getAverageEarnFactor( int theAge ) const;
  /// <summary>Returns number of decimal places in output.</summary>
  ///
  /// <returns>Number of decimal places in output.</returns>
  int getDecimals() const { return(numDecimals); }
  /// <summary>Returns first age.</summary>
  ///
  /// <returns>First age.</returns>
  int getFirstAge() const { return(firstAge); }
  double getHighEarnFactor( int theAge ) const;
  /// <summary>Returns last age.</summary>
  ///
  /// <returns>Last age.</returns>
  int getLastAge() const { return(lastAge); }
  double getLowEarnFactor( int theAge ) const;
  double getVeryLowEarnFactor( int theAge ) const;
  double getScaledEarnFactor( int theAge, scaleType earnType ) const;
  /// <summary>Returns width of output.</summary>
  ///
  /// <returns>Width of output.</returns>
  int getWidth() const { return(fieldWidth); }
  void read( std::istream& in, int ageStart, int ageEnd, bool desc );
  void setAverageEarnings( EarnProject& earnProject,
    const AverageWage& averageWage, int birthYear, int ageStart, int ageEnd );
  /// <summary>Sets number of decimals in output field.</summary>
  ///
  /// <param name="newDecimals">New number of decimals in output.</param>
  void setDecimals( int newDecimals ) { numDecimals = newDecimals; }
  void setHighEarnings( EarnProject& earnProject,
    const AverageWage& averageWage, int birthYear, int ageStart, int ageEnd );
  void setLowEarnings( EarnProject& earnProject,
    const AverageWage& averageWage, int birthYear, int ageStart, int ageEnd );
  void setVeryLowEarnings( EarnProject& earnProject,
    const AverageWage& averageWage, int birthYear, int ageStart, int ageEnd );
  void setScaledEarnings( EarnProject& earnProject,
    const AverageWage& averageWage, int birthYear, int ageStart, int ageEnd, 
    scaleType earnType);
  /// <summary>Sets width of output field.</summary>
  ///
  /// <param name="newWidth">New width of output field.</param>
  void setWidth( int newWidth ) { fieldWidth = newWidth; }
  void write( std::ostream& out, int ageStart, int ageEnd );
private:
  ScaledEarnFactors& operator=( const ScaledEarnFactors& );
};
