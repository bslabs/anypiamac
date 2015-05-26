// Declarations for the <see cref="Document"/> class to manage a generic
// document.

// $Id: document.h 1.19 2011/08/11 15:21:18EDT 044579 Development  $

#pragma once

#include "genfile.h"

/// <summary>Manages a generic document.</summary>
///
/// <remarks>This class is meant to be a parent of a user's document.</remarks>
class Document : public Genfile
{
private:
  /// <summary>The core file name, before modifiers or extensions.</summary>
  std::string corename;
  /// <summary>Title of the document.</summary>
  std::string docTitle;
public:
  Document();
  Document( const std::string& newCorename, const std::string& newDirectory );
  virtual ~Document();
  std::string errormesg();
  /// <summary>Returns the core file name, before modifiers or extensions.
  /// </summary>
  ///
  /// <returns>The core file name, before modifiers or extensions.</returns>
  std::string getCorename() const { return corename; }
  /// <summary>Returns the modifier on the file name.</summary>
  ///
  /// <returns>The modifier on the file name.</returns>
  std::string getModifier() const
    { return pathname.substr(corename.length()); }
  /// <summary>Returns full path name.</summary>
  ///
  /// <returns>Full path name.</returns>
  std::string getPathname() const { return(pathname); }
  /// <summary>Returns title of this document.</summary>
  ///
  /// <returns>Title of this document.</returns>
  const std::string& getTitle() const { return(docTitle); }
  std::string readmesg();
  /// <summary>Sets the core part of the file name, before modifiers or
  /// extensions.</summary>
  ///
  /// <remarks>Also sets the path name to the core name.</remarks>
  ///
  /// <param name="newCorename">The new core name.</param>
  void setCorename( const std::string& newCorename )
    { pathname = corename = newCorename; }
  void setExtension( const std::string& newExtension );
  /// <summary>Sets the modifier on the file name.</summary>
  ///
  /// <param name="newModifier">The new file modifier.</param>
  void setModifier( const std::string& newModifier )
    { pathname = corename + newModifier; }
  void setPath( const std::string& newPath );
  void setPathAndModifier( const std::string& newPath,
    const std::string& newModifier );
  /// <summary>Sets full path name.</summary>
  ///
  /// <param name="newPathname">New full path name.</param>
  void setPathname( const std::string& newPathname )
  { pathname = newPathname; }
  /// <summary>Sets title of this document.</summary>
  ///
  /// <param name="newTitle">New title of this document.</param>
  void setTitle( const std::string& newTitle ) { docTitle = newTitle; }
  std::string writemesg();
};
