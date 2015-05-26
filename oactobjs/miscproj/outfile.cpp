// Functions for the <see cref="Outfile"/>, <see cref="Out132"/>, and
// <see cref="Out80"/> classes - parents of classes to manage output file with
// page counters.

// $Id: outfile.cpp 1.46 2011/08/12 08:47:03EDT 044579 Development  $

using namespace std;

#pragma warning( disable: 4244 4127 4100 )
#include "outfile.h"
#include "Path.h"
#include "Trace.h"
#include "resource.h"
#include "PiaException.h"
#include "boost/date_time/gregorian/gregorian.hpp"

/// <summary>Initializes output file core name and extension to specified
/// values; parent directory is set to the current working directory.
/// </summary>
///
/// <param name="newCorename">New core file name.</param>
/// <param name="newExtension">New extension on the file name.</param>
Outfile::Outfile( const std::string& newCorename,
 const std::string& newExtension ) :
Document(newCorename, ""), pageNum(0), dateModyyr(), outstrm()
{
  pathname = Path::changeExtension(newCorename, newExtension);
}

/// <summary>Initializes output file core name, extension, and parent
/// directory to the specified values.</summary>
///
/// <param name="newDirectory">Name of parent directory.</param>
/// <param name="newCorename">New core file name.</param>
/// <param name="newExtension">New extension on the file name.</param>
Outfile::Outfile( const std::string& newDirectory,
const std::string& newCorename, const std::string& newExtension ) :
Document(newCorename, newDirectory), pageNum(0), dateModyyr()
{
  pathname = Path::changeExtension(pathname, newExtension);
}

/// <summary>Destroys this instance.</summary>
Outfile::~Outfile( )
{ }

/// <summary>Closes output file.</summary>
void Outfile::closeout()
{
  outstrm.close();
}

/// <summary>Opens output file.</summary>
void Outfile::openout()
{
  outstrm.open(pathname.c_str(), ios::out);
  if (!outstrm.is_open())
    throw PiaException(PIA_IDS_GENFILE1);
  outstrm.setf(ios::fixed, ios::floatfield);
}

/// <summary>Initializes output file core name and extension.</summary>
///
/// <param name="newCorename">New core file name.</param>
/// <param name="newExtension">New extension on the file name.</param>
Out132::Out132( const std::string& newCorename,
 const std::string& newExtension ) : Outfile(newCorename, newExtension)
{ }

/// <summary>Writes date, time, 2-line title, and page to 132-column print
/// file.</summary>
///
/// <remarks>Prints the two-line title, along with the date, time, and page
/// counter. Then prints two blank lines.</remarks>
void Out132::pgtitle()
{
  pageNum++;
  formfeed();
  ostringstream ss;
  ss << "  Date      Time           ";
  const unsigned length0 = static_cast<unsigned>(gettitle(0).length());
  const unsigned spacecount0 = (length0 < 95u) ? (95u - length0) / 2u : 0u;
  ss << setw(spacecount0) << ' ' << gettitle(0)
    << setw(spacecount0) << ' ' << "Page " << pageNum;
  printLine(ss.str());
  ss.str("");
  ss << boost::gregorian::day_clock::local_day();
  const unsigned length1 = static_cast<unsigned>(gettitle(1).length());
  const unsigned spacecount1 = (length1 < 95u) ? (95u - length1) / 2u : 0u;
  ss << setw(spacecount1) << ' ' << gettitle(1);
  printLine(ss.str());
  advanceLine();
  advanceLine();
  return;
}

/// <summary>Prepares the two-line title strings for output.</summary>
///
/// <remarks>Puts the specified titles in the next two strings in the
/// specified string table, along with the date, time, and page counter. Then
/// adds two blank lines to the string table.</remarks>
///
/// <param name="stringTable">String table to put results into.</param>
/// <param name="count">Output count of number of lines prepared.</param>
/// <param name="pageNum">Page counter.</param>
/// <param name="newPageTitle">Title array (length 2).</param>
void Out132::prepareStrings( std::string* stringTable, int* count,
 int* pageNum, const std::string* newPageTitle )
{
  pageNum++;
  ostringstream ss;
  ss << "  Date      Time           ";
  const unsigned length0 = static_cast<unsigned>(newPageTitle[0].length());
  const unsigned spacecount0 = (length0 < 95u) ? (95u - length0) / 2u : 0u;
  ss << setw(spacecount0) << ' ' << newPageTitle[0]
    << setw(spacecount0) << ' ' << "Page " << *pageNum;
  stringTable[(*count)++] = ss.str();
  ss.str("");
  ss << boost::gregorian::day_clock::local_day();
  const unsigned length1 = static_cast<unsigned>(newPageTitle[1].length());
  const unsigned spacecount1 = (length1 < 95u) ? (95u - length1) / 2u : 0u;
  ss << setw(spacecount1) << ' ' << newPageTitle[1];
  stringTable[(*count)++] = ss.str();
  stringTable[(*count)++] = "";
  stringTable[(*count)++] = "";
}

/// <summary>Initializes output file core name and extension.</summary>
///
/// <param name="newCorename">New core file name.</param>
/// <param name="newExtension">New extension on the file name.</param>
Out80::Out80( const std::string& newCorename,
 const std::string& newExtension ) : Outfile(newCorename, newExtension)
{ }

/// <summary>Writes date, time, 1-line title, and page to 80-column print
///   file.</summary>
void Out80::pgtitle()
{
  pageNum++;
  formfeed();
  stringstream ss;
  ss << boost::gregorian::day_clock::local_day()
    << setw(43) << ' ' << "Page " << pageNum;
  printLine(ss.str());
  for (int i = 0; i < PAGE_TITLE_LINES; i++) {
    if (gettitle(i).length() > 0) {
      string temp(" ");
      const unsigned length = static_cast<unsigned>(gettitle(i).length());
      const unsigned spacecount = (length < 74u) ? (74u - length) / 2u : 0u;
      temp.append(spacecount, ' ');;
      temp += gettitle(i);
      printLine(temp);
    }
  }
  advanceLine();
  advanceLine();
  return;
}

/// <summary>Prepares the two-line title strings for output.</summary>
///
/// <remarks>Puts the specified titles in the next two strings in the
/// specified string table, along with the date, time, and page counter. Then
/// adds two blank lines to the string table.</remarks>
///
/// <param name="stringTable">String table to put results into.</param>
/// <param name="count">Output count of number of lines prepared.</param>
/// <param name="pageNum">Page counter.</param>
/// <param name="newPageTitle">Title array (length 2).</param>
void Out80::prepareStrings( std::string* stringTable, int* count, int* pageNum,
const std::string* newPageTitle )
{
  (*pageNum)++;
  stringstream ss;
  ss << boost::gregorian::day_clock::local_day();
  ss << setw(43) << ' ' << "Page " << *pageNum;
  stringTable[(*count)++] = ss.str();
  for (int i = 0; i < PAGE_TITLE_LINES; i++, (*count)++) {
    if (newPageTitle[i].length() > 0) {
      stringTable[*count] = " ";
      const unsigned length =
        static_cast<unsigned>(newPageTitle[i].length());
      const unsigned spacecount = (length < 74u) ? (74u - length) / 2u : 0u;
      stringTable[*count].append(spacecount, ' ');
      stringTable[*count] += newPageTitle[i];
    }
  }
  stringTable[(*count)++] = "";
  stringTable[(*count)++] = "";
  return;
}
