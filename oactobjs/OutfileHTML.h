// Declarations for the <see cref="OutFileHTML"/> class to manage print output
// to an html file.

// $Id: OutfileHTML.h 1.15 2011/08/12 08:47:02EDT 044579 Development  $

#pragma once

#include <fstream>
#include "document.h"

/// <summary>Manages the print output functions, when printing to an HTML file.
/// </summary>
///
/// <remarks>Manages the print output functions, when printing to an HTML file.
/// </remarks>
class OutFileHTML
{
public:
  /// <summary>Represents the closing angle-bracket of an HTML tag.</summary>
  static const char tagRightChar;
  /// <summary>The CSS page.</summary>
  std::string cssPage;
  /// <summary>Number of title lines.</summary>
  int PAGE_TITLE_LINES;
private:
  /// <summary>Numeric punctuation facet with 3 digits per thousands separator.
  /// </summary>
  std::numpunct<char> *punct;
  /// <summary>Output file.</summary>
  std::ofstream out;
  const std::locale loc;
  /// <summary>Multi-line title at the top of each output page.</summary>
  std::string *pageTitle;
protected:
  /// <summary>Indicator for first page of output.</summary>
  bool firstPage;
public:
  OutFileHTML( const std::string& newCssPage, int numTitleLines );
  ~OutFileHTML();
  /// <summary>Close the output file.</summary>
  void closeFile() { out.close(); }
  void doHeader( const std::string& webTitle );
  /// <summary>Returns one page title line.</summary>
  ///
  /// <returns>Specified page title line.</returns>
  ///
  /// <param name="number">Page title line number (0 or 1).</param>
  const std::string& gettitle ( int number ) const
    { return pageTitle[number]; }
  //void RowStub( const std::string& rowstub );
  void openFile( const std::string& pathname );
  void pgtitle();
  /// <summary>Sets one page title line.</summary>
  ///
  /// <param name="number">Page title line number (0 or 1).</param>
  /// <param name="newPageTitle">New page title line.</param>
  void settitle( int number, const std::string& newPageTitle )
    { pageTitle[number].assign(newPageTitle); }
  /// <summary>Prints the string without a carriage return.</summary>
  ///
  /// <param name="str">String to print.</param>
  void write( const std::string& str ) { out << str; }
  /// <summary>Prints the C-style string without a carriage return.</summary>
  ///
  /// <param name="str">String to print.</param>
  void write( const char* str ) { out << str; }
  /// <summary>Prints the character without a carriage return.</summary>
  ///
  /// <param name="ch">Character to print.<param>
  void write( char ch ) { out << ch; }
  /// <summary>Writes an HTML attribute and its string value.</summary>
  ///
  /// <remarks>They are written as a space, the attribute name, an equals
  /// sign, and the attribute value in double quotes.</remarks>
  ///
  /// <param name="style">The attribute name.</param>
  /// <param name="s">The attribute value as a string.</param>
  void writeAttribute( const std::string& style, const std::string& s )
    { out << " " << style << "=\"" << s << "\""; }
  /// <summary>Writes an HTML attribute and its integer value.</summary>
  ///
  /// <remarks>They are written as a space, the attribute name, an equals
  /// sign, and the attribute value in double quotes.</remarks>
  ///
  /// <param name="style">The attribute name.</param>
  /// <param name="value">The attribute value as an integer.</param>
  void writeAttribute( const std::string& style, int value )
    { out << " " << style << "=\"" << value << "\""; }
  void writeBeginGrayRow();
  void writeBeginTable( const std::string& width );
  void writeBeginTable( const std::string& width,
    const std::string& captionClass, const std::string& theTitle );
  /// <summary>Writes the opening tag of the specified HTML element.</summary>
  ///
  /// <remarks>The closing angle-bracket is left off so that attributes may be
  /// added.</remarks>
  ///
  /// <param name="s">The specified HTML element.</param>
  void writeBeginTag( const std::string& s ) { out << "<" << s; }
  void writeCellBody( const std::string& contents );
  void writeCellBody( int theInt );
  void writeCellBodyRight( const std::string& contents );
  void writeCellBodyRight( double theDouble, int precision );
  void writeCellBodyRight( int theInt );
  void writeCellBodyColSpan( const std::string& contents, int spanInt );
  void writeCellHeading( const std::string& heading );
  void writeCellHeading( int theInt );
  void writeCellHeadingColSpan( const std::string& heading,
    const std::string& spanStr );
  void writeCellHeadingColSpan( const std::string& heading, int spanInt );
  void writeCellHeadingRowSpan( const std::string& heading,
    const std::string& span );
  /// <summary>Writes the closing tag of the specified HTML element.</summary>
  ///
  /// <param name="s">The specified HTML element.</param>
  void writeEndTag( const std::string& s ) { out << "</" << s << ">"; }
  /// <summary>Writes the opening tag of the specified HTML element.</summary>
  ///
  /// <remarks>The closing angle-bracket is added.</remarks>
  ///
  /// <param name="s">The specified HTML element.</param>
  void writeFullBeginTag( const std::string& s ) { out << "<" << s << ">"; }
  /// <summary>Prints one line with a carriage return.</summary>
  ///
  /// <param name="str">String to print.</param>
  void writeLine( const std::string& str ) { out << str << std::endl; }
  /// <summary>Prints one line with a carriage return.</summary>
  ///
  /// <param name="str">String to print.</param>
  void writeLine( const char* str ) { out << str << std::endl; }
  /// <summary>Prints a carriage return.</summary>
  void writeLine() { out << std::endl; }
  void writeYear( int year );
};
