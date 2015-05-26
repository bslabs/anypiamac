// Functions for the <see cref="Document"/> class to manage a generic
// document.

// $Id: document.cpp 1.16 2011/08/11 15:21:17EDT 044579 Development  $

#include "document.h"
#include "Path.h"

using namespace std;

/// <summary>Initializes the parent directory, core file name, and title to "".
/// </summary>
Document::Document() : Genfile(), corename(), docTitle()
{
  pathname = corename;
}

/// <summary>Initializes the parent directory and core file name to the
/// specified arguments, and the title to "".</summary>
///
/// <param name="newCorename">New core file name, before modifiers or
///  extensions.</param>
/// <param name="newDirectory">New directory holding this document.</param>
Document::Document( const std::string& newCorename,
 const std::string& newDirectory ) :
Genfile(newDirectory), corename(newCorename), docTitle()
{
  pathname = Path::combine(newDirectory, corename);
}

/// <summary>Destroys document.</summary>
Document::~Document()
{ }

/// <summary>Returns message when reading from file.</summary>
///
/// <returns>Message when reading from file.</returns>
std::string Document::readmesg()
{
  return string("Reading ") + docTitle + " from " + getPathname();
}

/// <summary>Returns message when writing to file.</summary>
///
/// <returns>Message when writing to file.</returns>
std::string Document::writemesg()
{
  return string("Writing ") + docTitle + " to " + getPathname();
}

/// <summary>Returns message when error reading from file.</summary>
///
/// <returns>Message when error reading from file.</returns>
std::string Document::errormesg()
{
  return string("Error reading ") + docTitle;
}

/// <summary>Sets the path and modifier on the file name.</summary>
///
/// <param name="newPath">The new path.</param>
/// <param name="newModifier">The new file modifier.</param>
void Document::setPathAndModifier( const std::string& newPath,
 const std::string& newModifier )
{
  pathname = Path::combine(newPath, corename) + newModifier;
}

/// <summary>Sets the path and modifier on the file name.</summary>
///
/// <param name="newPath">The new path.</param>
void Document::setPath( const std::string& newPath )
{
  pathname = Path::combine(newPath, pathname);
}

/// <summary>Sets the extension on the file name.</summary>
///
/// <param name="newExtension">The new file extension.</param>
void Document::setExtension( const std::string& newExtension )
{
  pathname = Path::changeExtension(pathname, newExtension);
}
