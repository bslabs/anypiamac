// Declarations for the <see cref="PiaRead"/> class to read a file of
// worker data from disk.

// $Id: piaread.h 1.36 2011/08/08 08:45:14EDT 044579 Development  $

#pragma once

#include <string>
class UserAssumptions;
class SecondaryArray;
class PiaDataArray;
class WorkerDataArray;
class WorkerData;

/// <summary>Reads the basic data required to calculate a Social Security
/// benefit.</summary>
///
/// <remarks>This version is used in programs such as microsimulations, which
/// do not use Social Security Statements or factors to project earnings. For
/// programs that do use those things, such as Anypia, use the subclass
/// <see cref="PiaReadAny"/>.</remarks>
///
/// <seealso cref="PiaReadAny"/>
class PiaRead
{
protected:
  /// <summary>Worker data.</summary>
  WorkerData& workerData;
  /// <summary>Widow and other family member basic data.</summary>
  WorkerDataArray& widowDataArray;
  /// <summary>Widow and other family member calculation data.</summary>
  PiaDataArray& widowArray;
  /// <summary>User-specified assumptions.</summary>
  UserAssumptions& userAssumptions;
  /// <summary>Array of <see cref="Secondary"/> benefit information.</summary>
  SecondaryArray& secondaryArray;
private:
  /// <summary>Temporary first year of assumptions.</summary>
  int istart2;
  /// <summary>Number of columns at beginning of each line reserved for
  /// line number.</summary>
  ///
  /// <remarks>The Anypia program uses 2 columns; other programs that
  /// require more than 99 lines use 3 columns.</remarks>
  static int width;
  /// <summary>Number of columns for earnings and bases.</summary>
  static int earnWidth;
public:
  /// <summary>Input line.</summary>
  std::string inputLine;
  /// <summary>True if a line has been gotten from the file but not used.
  /// </summary>
  bool unusedLine;
  /// <summary>Type of input line.</summary>
  int lineType;
public:
  PiaRead( WorkerData& newWorkerData, WorkerDataArray& newWidowDataArray,
    PiaDataArray& newWidowArray, UserAssumptions& newUserAssumptions,
    SecondaryArray& newSecondaryArray );
  /// <summary>Returns width of earnings or base field.</summary>
  ///
  /// <returns>Width of earnings or base field.</returns>
  int getEarnWidth() const { return(earnWidth); }
  /// <summary>Returns width of field for line number in file.</summary>
  ///
  /// <returns>Width of field for line number in file.</returns>
  int getWidth() const { return(width); }
  void parseAssump( const std::string& line );
  void parseAw( int lineNumber, const std::string& line );
  virtual void parseBack( const std::string& );
  void parseBases( int lineNumber, const std::string& line );
  void parseBases77( int lineNumber, const std::string& line );
  void parseBendate( const std::string& line );
  void parseBi( int lineNumber, const std::string& line );
  void parseBlind( const std::string& line );
  void parseCatchup( int lineNumber, const std::string& line );
  void parseChildCareYears( const std::string& line );
  void parseDeath( const std::string& line );
  void parseDeemed( const std::string& line );
  void parseDisab1( const std::string& line );
  void parseDisab2( const std::string& line );
  virtual void parseEarnOasdi( int lineNumber, const std::string& line );
  void parseEarnHi( int lineNumber, const std::string& line );
  virtual void parseEarnType( const std::string& );
  void parseFamilyMember( int lineNumber, const std::string& line );
  virtual void parseFwrd( const std::string& );
  void parseMsdates( const std::string& line );
  void parseNhaddr( int lineNumber, const std::string& line );
  void parseNhname( const std::string& line );
  void parseOabent( const std::string& line );
  virtual void parsePebes( const std::string& );
  void parsePubpen( const std::string& line );
  void parsePubpenReservist( const std::string& line );
  void parseQc1 ( const std::string& line );
  void parseQc2 ( const std::string& line );
  void parseSsn ( const std::string& line );
  void parseTaxType( const std::string& line );
  void parseTitleaw( const std::string& line );
  void parseTitlebi( const std::string& line );
  void parseTob ( const std::string& line );
  void parseTotalize( const std::string& line );
  void parseYears( const std::string& line );
  unsigned int read( std::istream& in );
  virtual void setEarnProjectYears( int, int );
  /// <summary>Sets width of earnings or base field.</summary>
  ///
  /// <param name="newWidth">New width of earnings or base field.</param>
  void setEarnWidth( int newWidth ) { earnWidth = newWidth; }
  virtual void setPebsData();
  /// <summary>Sets width of field for line number in file,</summary>
  ///
  /// <param name="newWidth">New width of field for line number.</param>
  void setWidth( int newWidth ) { width = newWidth; }
private:
  PiaRead operator=( const PiaRead& newPiaRead );
};
