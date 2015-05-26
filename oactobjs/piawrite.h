// Declarations for the <see cref="PiaWrite"/> class to write out worker data.
//
// $Id: piawrite.h 1.30 2011/08/08 08:45:15EDT 044579 Development  $

#pragma once

#include <string>
class UserAssumptions;
class SecondaryArray;
class WorkerDataArray;
class PiaDataArray;
class WorkerData;

/// <summary>Writes out the basic data required to calculate a Social Security
/// benefit.</summary>
///
/// <remarks>This version is used in programs such as microsimulations, which
/// do not use Social Security Statements or factors to project earnings. For
/// programs that do use those things, such as Anypia, use the subclass
/// <see cref="PiaWriteAny"/>.</remarks>
///
/// <seealso cref="PiaWriteAny"/>
class PiaWrite
{
protected:
  /// <summary>Worker data.</summary>
  const WorkerData& workerData;
  /// <summary>Widow and other family member basic data.</summary>
  const WorkerDataArray& widowDataArray;
  /// <summary>Widow and other family member calculation data.</summary>
  const PiaDataArray& widowArray;
  /// <summary>User-specified assumptions.</summary>
  const UserAssumptions& userAssumptions;
  /// <summary>Array of <see cref="Secondary"/> benefit information.</summary>
  const SecondaryArray& secondaryArray;
private:
  /// <summary>Number of columns at beginning of each line reserved for line
  /// number.</summary>
  ///
  /// <remarks>The Anypia program uses 2 columns; other programs that
  /// require more than 99 lines use 3 columns.</remarks>
  static int width;
  /// <summary>Number of columns for earnings and bases.</summary>
  static int earnWidth;
  /// <summary>Number of decimal places for earnings and bases.</summary>
  static int numDecimals;
public:
  PiaWrite ( const WorkerData& newWorkerData,
    const WorkerDataArray& newWidowDataArray,
    const PiaDataArray& newWidowArray,
    const UserAssumptions& newUserAssumptions,
    const SecondaryArray& newSecondaryArray );
  /// <summary>Returns width of earnings or base field.</summary>
  ///
  /// <returns>Width of earnings or base field.</returns>
  int getEarnWidth() const { return(earnWidth); }
  /// <summary>Returns number of decimal places for earnings or base.</summary>
  ///
  /// <returns>Number of decimal places for earnings or base.</returns>
  int getNumDecimals() const { return(numDecimals); }
  /// <summary>Returns width of field for line number in file.</summary>
  ///
  /// <returns>Width of field for line number in file.</returns>
  int getWidth() const { return(width); }
  /// <summary>Sets width of earnings or base field.</summary>
  ///
  /// <param name="newWidth">New width of earnings or base field.</param>
  void setEarnWidth( int newWidth ) { earnWidth = newWidth; }
  /// <summary>Sets number of decimal places for earnings or base.</summary>
  ///
  /// <param name="newNumDecimals">New number of decimals.</param>
  void setNumDecimals( int newNumDecimals ) { numDecimals = newNumDecimals; }
  /// <summary>Sets width of field for line number in file.</summary>
  ///
  /// <param name="newWidth">New width of column for line number.</param>
  void setWidth( int newWidth ) { width = newWidth; }
  void write( std::ostream& out );
  virtual void writeAssump( std::ostream& out ) const;
  void writeAw( std::ostream& out ) const;
  virtual void writeBack( std::ostream& ) const;
  void writeBases( std::ostream& out ) const;
  void writeBenDate( std::ostream& out ) const;
  void writeBi( std::ostream& out ) const;
  void writeBlind( std::ostream& out ) const;
  void writeChildCareYears( std::ostream& out ) const;
  void writeDeath( std::ostream& out ) const;
  void writeDeemed( std::ostream& out ) const;
  void writeDisab1( std::ostream& out ) const;
  void writeDisab2( std::ostream& out ) const;
  virtual void writeEarnOasdi( std::ostream& out ) const;
  void writeEarnHi( std::ostream& out ) const;
  virtual void writeEarnType( std::ostream& ) const;
  void writeFamilyMember( std::ostream& out, int i ) const;
  void writeFamilyMembers( std::ostream& out ) const;
  virtual void writeFwrd( std::ostream& ) const;
  void writeMsdates( std::ostream& out ) const;
  void writeNhaddr( std::ostream& out ) const;
  void writeNhname( std::ostream& out ) const;
  void writeOabEnt( std::ostream& out ) const;
  virtual void writePebes( std::ostream& ) const;
  void writePubpen( std::ostream& out ) const;
  void writePubpenReservist( std::ostream& out ) const;
  void writeQc1( std::ostream& out ) const;
  void writeQc2( std::ostream& out ) const;
  void writeRailRoad( std::ostream& out ) const;
  void writeSsn( std::ostream& out ) const;
  void writeTaxType( std::ostream& out ) const;
  void writeTob( std::ostream& out ) const;
  void writeTotalize( std::ostream& out ) const;
  void writeYears( std::ostream& out ) const;
private:
   PiaWrite operator=( const PiaWrite& newPiaWrite );
};
