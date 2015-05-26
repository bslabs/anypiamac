// Functions for the <see cref="OutFileHTML"/> class to manage print output
// to an html file.

// $Id: OutfileHTML.cpp 1.19 2011/08/12 08:47:01EDT 044579 Development  $

#include <sstream>
#include "OutFileHTML.h"
#include "Resource.h"
#include "PiaException.h"
#include "NumPunct.h"

using namespace std;

const char OutFileHTML::tagRightChar = '>';

/// <summary>Constructor.</summary>
///
/// <remarks>Initializes the css page to use, the number of title lines, and
/// the punctuation facet.</remarks>
///
/// <param name="newCssPage">The css page to use.</param>
/// <param name="numTitleLines">Number of title lines at top of page.</param>
OutFileHTML::OutFileHTML( const std::string& newCssPage, int numTitleLines ) :
cssPage(newCssPage), PAGE_TITLE_LINES(numTitleLines), punct(new PunctFacet),
out(), loc(out.getloc(), punct), firstPage(true)
{
  pageTitle = new string[PAGE_TITLE_LINES];
  out.imbue(loc);
}

/// <summary>Destructor.</summary>
OutFileHTML::~OutFileHTML()
{
  delete[] pageTitle;
}

/// <summary>Opens output file.</summary>
///
/// <param name="pathname">The full pathname of the file.</param>
void OutFileHTML::openFile( const std::string& pathname )
{
  out.open(pathname.c_str(), ios::out);
  if (!out.is_open())
    throw PiaException(PIA_IDS_GENFILE1);
  out.setf(ios::fixed, ios::floatfield);
}

/// <summary>Writes title to html file.</summary>
void OutFileHTML::pgtitle()
{
  writeBeginTag("h1");
  if (firstPage) {
    writeAttribute("class", "FirstHeader");
    firstPage = false;
  }
  else {
    writeAttribute("class", "LaterHeader");
  }
  write(tagRightChar);
  for (int i = 0; i < PAGE_TITLE_LINES; i++) {
    write(gettitle(i));
    writeFullBeginTag("br");
  }
  writeEndTag("h1");
  writeLine();
}

/// <summary>Writes gray row starting information.</summary>
void OutFileHTML::writeBeginGrayRow()
{
  writeBeginTag("tr");
  writeAttribute("align", "right");
  writeAttribute("bgcolor", "#DFDFD5");
  writeAttribute("valign", "bottom");
  write(tagRightChar);
  writeLine();
}

/// <summary>Writes the html for a string as the contents of one cell in a
/// table, right-justified.</summary>
///
/// <remarks>There are multiple versions of this function. Each version uses
/// the inserter to put the argument into the output stream, surrounded by
/// the appropriate tags.</remarks>
///
/// <param name="contents">The contents of the cell in the table.</param>
void OutFileHTML::writeCellBodyRight( const std::string& contents )
{
  writeLine("<td valign=\"bottom\"><p class=\"CellBodyRight\">" +
    contents + "</p></td>");
}

/// <summary>Writes the html for a double as the contents of one cell in a
/// table, right-justified.</summary>
///
/// <remarks>There are multiple versions of this function. Each version uses
/// the inserter to put the argument into the output stream, surrounded by
/// the appropriate tags.</remarks>
///
/// <param name="theDouble">The double to be placed in the cell in the table.
/// </param>
/// <param name="precision">Precision to be used when writing the double.
/// </param>
void OutFileHTML::writeCellBodyRight( double theDouble, int precision )
{
  out.precision(precision);
  out << "<td valign=\"bottom\"><p class=\"CellBodyRight\">" << theDouble
    << "</p></td>";
  writeLine();
}

/// <summary>Writes the html for an integer as the contents of one cell in a
/// table, right-justified.</summary>
///
/// <remarks>There are multiple versions of this function. Each version uses
/// the inserter to put the argument into the output stream, surrounded by
/// the appropriate tags.</remarks>
///
/// <param name="theInt">The integer to be placed in the cell in the table.
/// </param>
void OutFileHTML::writeCellBodyRight( int theInt )
{
  writeBeginTag("td");
  writeAttribute("valign","bottom");
  out << tagRightChar;
  writeBeginTag("p");
  writeAttribute("class", "CellBodyRight");
  out << tagRightChar << theInt;
  writeEndTag("p");
  writeEndTag("td");
  writeLine();
}

/// <summary>Writes html header information.</summary>
///
/// <param name="webTitle">The title in the web page title bar.</param>
void OutFileHTML::doHeader( const std::string& webTitle )
{
  writeFullBeginTag("html");
  writeLine();
  writeLine();
  writeFullBeginTag("head");
  writeLine();
  writeBeginTag("link");
  writeAttribute("rel", "stylesheet");
  writeAttribute("href", cssPage);
  writeAttribute("charset", "ISO-8859-1");
  writeAttribute("type", "text/css");
  writeEndTag("link");
  writeLine();
  writeFullBeginTag("title");
  write(webTitle);
  writeEndTag("title");
  writeLine();
  writeEndTag("head");
  writeLine();
  writeLine();
  writeFullBeginTag("body");
  writeLine();
}

/// <summary>Writes the html for the start of a table, with a caption.
/// </summary>
///
/// <param name="width">Width of table (e.g. "95%").</param>
/// <param name="captionClass">Class of paragraph for table caption.</param>
/// <param name="theTitle">Title of table.</param>
void OutFileHTML::writeBeginTable( const std::string& width,
const std::string& captionClass, const std::string& theTitle )
{
  writeBeginTag("table");
  writeAttribute("cellspacing", "0");
  writeAttribute("width", width);
  write(tagRightChar);
  writeLine();
  writeFullBeginTag("caption");
  writeBeginTag("p");
  writeAttribute("class", captionClass);
  write(tagRightChar);
  write(theTitle);
  writeEndTag("p");
  writeEndTag("caption");
  writeLine();
}

/// <summary>Writes the html for the start of a table, without a caption.
/// </summary>
///
/// <param name="width">Width of table (e.g. "95%").</param>
void OutFileHTML::writeBeginTable( const std::string& width )
{
  writeBeginTag("table");
  writeAttribute("cellspacing", "0");
  writeAttribute("width", width);
  write(tagRightChar);
  writeLine();
}

/// <summary>Writes the html for a string as the contents of one heading in a
/// table.</summary>
///
/// <remarks>The cell does not span columns or rows.
///
/// There are multiple versions of this function. Each version uses
/// the inserter to put the argument into the output stream, surrounded by
/// the appropriate tags.</remarks>
///
/// <param name="heading">The heading in the table.</param>
void OutFileHTML::writeCellHeading( const std::string& heading )
{
  writeFullBeginTag("th");
  writeBeginTag("p");
  writeAttribute("class", "CellHeading");
  out << tagRightChar << heading;
  writeEndTag("p");
  writeEndTag("th");
  writeLine();
}

/// <summary>Writes the html for an integer as the contents of one heading in
/// a table.</summary>
///
/// <remarks>The cell does not span columns or rows.
///
/// There are multiple versions of this function. Each version uses
/// the inserter to put the argument into the output stream, surrounded by
/// the appropriate tags.</remarks>
///
/// <param name="theInt">The integer to use as the heading in the table.
/// </param>
void OutFileHTML::writeCellHeading( int theInt )
{
  writeFullBeginTag("th");
  writeBeginTag("p");
  writeAttribute("class", "CellHeading");
  out << tagRightChar << theInt;
  writeEndTag("p");
  writeEndTag("th");
  writeLine();
}

/// <summary>Writes the html for the contents of one heading in a table,
/// spanning columns.</summary>
///
/// <param name="heading">The heading in the table.</param>
/// <param name="spanStr">The number of columns to span, as a string.</param>
void OutFileHTML::writeCellHeadingColSpan( const std::string& heading,
const std::string& spanStr )
{
  writeBeginTag("th");
  writeAttribute("colspan", spanStr);
  write(tagRightChar);
  writeBeginTag("p");
  writeAttribute("class", "CellHeading");
  write(tagRightChar);
  write(heading);
  writeEndTag("p");
  writeEndTag("th");
  writeLine();
}

/// <summary>Writes the html for the contents of one heading in a table,
/// spanning columns.</summary>
///
/// <param name="heading">The heading in the table.</param>
/// <param name="spanInt">The number of columns to span, as an integer.
/// </param>
void OutFileHTML::writeCellHeadingColSpan( const std::string& heading,
int spanInt )
{
  writeBeginTag("th");
  writeAttribute("colspan", spanInt);
  write(tagRightChar);
  writeBeginTag("p");
  writeAttribute("class", "CellHeading");
  write(tagRightChar);
  write(heading);
  writeEndTag("p");
  writeEndTag("th");
  writeLine();
}

/// <summary>Writes the html for the contents of one heading in a table,
/// spanning rows.</summary>
///
/// <param name="heading">The heading in the table.</param>
/// <param name="span">The number of rows to span, as a string.</param>
void OutFileHTML::writeCellHeadingRowSpan( const std::string& heading,
const std::string& span )
{
  writeBeginTag("th");
  writeAttribute("rowspan", span);
  write(tagRightChar);
  writeBeginTag("p");
  writeAttribute("class", "CellHeading");
  write(tagRightChar);
  write(heading);
  writeEndTag("p");
  writeEndTag("th");
  writeLine();
}

/// <summary>Writes the html for a string as the contents of one cell in a
/// table, left-justified.</summary>
///
/// <remarks>There are multiple versions of this function. Each version uses
/// the inserter to put the argument into the output stream, surrounded by
/// the appropriate tags.</remarks>
///
/// <param name="contents">The contents of the cell in the table.</param>
void OutFileHTML::writeCellBody( const std::string& contents )
{
  writeBeginTag("td");
  writeAttribute("valign","top");
  out << tagRightChar;
  writeBeginTag("p");
  writeAttribute("class", "CellBody");
  out << tagRightChar << contents;
  writeEndTag("p");
  writeEndTag("td");
  writeLine();
}

/// <summary>Writes the html for a string as the contents of one cell in a
/// table, left-justified, spanning columns.</summary>
///
/// <remarks>There are multiple versions of this function. Each version uses
/// the inserter to put the argument into the output stream, surrounded by
/// the appropriate tags.</remarks>
///
/// <param name="contents">The contents of the cells in the table.</param>
/// <param name="spanInt">The number of columns to span, as an integer.
/// </param>
void OutFileHTML::writeCellBodyColSpan( const std::string& contents,
  int spanInt )
{
  writeBeginTag("td");
  writeAttribute("valign","top");
  writeAttribute("colspan", spanInt);
  out << tagRightChar;
  writeBeginTag("p");
  writeAttribute("class", "CellBody");
  out << tagRightChar << contents;
  writeEndTag("p");
  writeEndTag("td");
  writeLine();
}

/// <summary>Writes the html for an integer as the contents of one cell in a
/// table, left-justified.</summary>
///
/// <remarks>There are multiple versions of this function. Each version uses
/// the inserter to put the argument into the output stream, surrounded by
/// the appropriate tags.</remarks>
///
/// <param name="theInt">The integer to be placed in the cell in the table.
/// </param>
void OutFileHTML::writeCellBody( int theInt )
{
  writeBeginTag("td");
  writeAttribute("valign","top");
  out << tagRightChar;
  writeBeginTag("p");
  writeAttribute("class", "CellBody");
  out << tagRightChar << theInt;
  writeEndTag("p");
  writeEndTag("td");
  writeLine();
}

/// <summary>Prints year with left justification in html output file.
/// </summary>
///
/// <remarks>We have a special function to handle a year, because we do not
/// want a comma inserted in it.</remarks>
///
/// <param name="year">The year to print.</param>
void OutFileHTML::writeYear( int year )
{
  writeFullBeginTag("td");
  writeBeginTag("p");
  writeAttribute("class", "CellBody");
  out << tagRightChar << PunctFacet::getYear(year);
  writeEndTag("p");
  writeEndTag("td");
  writeLine();
}
