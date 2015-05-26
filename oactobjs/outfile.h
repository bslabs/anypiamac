// Declarations for the <see cref="Outfile"/>, <see cref="Out132"/>, and
// <see cref="Out80"/> classes - parents of classes to manage output file with
// page counters.

// $Id: outfile.h 1.39 2011/08/12 08:47:02EDT 044579 Development  $

#pragma once

#include <fstream>
#include "document.h"
#include "boost/date_time/gregorian/greg_date.hpp"

/// <summary>Abstract parent of classes to manage an output file in runstreams
/// with multiple programs.</summary>
///
/// <remarks>It also manages a 2-line title and a date and time string for
/// output at the top of each page, along with the page number. It is an
/// abstract class; one of its subclasses (<see cref="OutFile80"/>,
/// <see cref="OutDC80"/>, or <see cref="OutFile132"/>) should be instantiated.
///
/// This was formerly used in the ssgiat programs, but they are all now in C#.
/// This also was formerly used in some of the ssgbp and jrunfunds programs,
/// but they have all been changed to use <see cref="OutFileHTML"/>.</remarks>
///
/// <seealso cref="OutFile80"/>
/// <seealso cref="OutDC80"/>
/// <seealso cref="OutFile132"/>
/// <seealso cref="OutFileHTML"/>
class Outfile : public Document
{
protected:
  /// <summary>Output stream.</summary>
  std::ofstream outstrm;
  /// <summary>Page number.</summary>
  int pageNum;
  /// <summary>The current date as month, day, and year.</summary>
  boost::gregorian::date dateModyyr;
  /// <summary>Number of title lines.</summary>
  static const int PAGE_TITLE_LINES = 2;
private:
  /// <summary>Two-line title at the top of each output page.</summary>
  std::string pageTitle[PAGE_TITLE_LINES];
public:
  Outfile( const std::string& newCorename, const std::string& newExtension );
  Outfile( const std::string& newDirectory, const std::string& newCorename,
    const std::string& newExtension );
  virtual ~Outfile();
  /// <summary>Move to next line.</summary>
  virtual void advanceLine() = 0;
  void closeout();
  /// <summary>Writes a footer to a print file.</summary>
  virtual void footer() = 0;
  /// <summary>Move to next page.</summary>
  virtual void formfeed() = 0;
  /// <summary>Returns page number.</summary>
  ///
  /// <returns>Page number.</returns>
  int getPageNum() const { return pageNum; }
  /// <summary>Returns one page title line.</summary>
  ///
  /// <returns>Specified page title line.</returns>
  ///
  /// <param name="number">Page title line number (0 or 1).</param>
  const std::string& gettitle ( int number ) const
  { return pageTitle[number]; }
  /// <summary>Writes a heading line to a print file.</summary>
  ///
  /// <param name="webTitle">The heading line to print.</param>
  virtual void header( const std::string& webTitle ) = 0;
  void openout();
  /// <summary>Writes date, time, title, and page to print file.</summary>
  virtual void pgtitle() = 0;
  /// <summary>Print a line of output with a carriage return.</summary>
  virtual void printLine( const std::string& ) = 0;
  /// <summary>Print a line of output with a carriage return.</summary>
  virtual void printLine( const char* ) = 0;
  /// <summary>Print a line of output with no carriage return.</summary>
  virtual void printLinena( const std::string& ) = 0;
  /// <summary>Print a line of output with no carriage return.</summary>
  virtual void printLinena( const char* ) = 0;
  /// <summary>Sets page number.</summary>
  ///
  /// <param name="newPageNum">New page number.</param>
  void setPageNum( int newPageNum ) { pageNum = newPageNum; }
  /// <summary>Sets one page title line.</summary>
  ///
  /// <param name="number">Page title line number (0 or 1).</param>
  /// <param name="newPageTitle">New page title line.</param>
  void settitle( int number, const std::string& newPageTitle )
  { pageTitle[number].assign(newPageTitle); }
private:
  Outfile( const Outfile& );
  Outfile& operator=( const Outfile& );
};

/// <summary>An <see cref="Outfile"/> tailored for 132-column output.
/// </summary>
///
/// <remarks>This is an abstract class; one of its subclasses
/// (<see cref="OutFile132"/>) should be instantiated.</remarks>
///
/// <seealso cref="OutFile132"/>
class Out132 : public Outfile
{
public:
  Out132( const std::string& newCorename, const std::string& newExtension );
  /// <summary>Does nothing.</summary>
  void footer() { }
  /// <summary>Does nothing.</summary>
  void header( const std::string& ) { }
  void pgtitle();
  static void prepareStrings( std::string* stringTable, int* count,
    int* pageNum, const std::string* newPageTitle );
};

/// <summary>An <see cref="Outfile"/> tailored for 80-column output.</summary>
///
/// <remarks>This is an abstract class; one of its subclasses
/// (<see cref="OutFile80"/> or <see cref="OutDC80"/>) should be instantiated.
/// </remarks>
///
/// <seealso cref="OutFile80"/>
/// <seealso cref="OutDC80"/>
class Out80 : public Outfile
{
public:
  Out80( const std::string& newCorename, const std::string& newExtension );
  /// <summary>Does nothing.</summary>
  void footer() { }
  /// <summary>Does nothing.</summary>
  void header( const std::string& ) { }
  void pgtitle();
  static void prepareStrings( std::string* stringTable, int* count,
    int* pageNum, const std::string* newPageTitle );
};
