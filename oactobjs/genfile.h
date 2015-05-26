// Declarations for the <see cref="Genfile"/> class to manage a general file.
//
// $Id: genfile.h 1.16 2011/08/11 16:23:33EDT 044579 Development  $

#pragma once

#include <iosfwd>
#include <string>

/// <summary>Manages a generic file.</summary>
///
/// <remarks>This class is meant to be a parent of other classes of files.
/// </remarks>
class Genfile
{
protected:
  /// <summary>Pathname of this file.</summary>
  std::string pathname;
private:
  /// <summary>Warning or status message.</summary>
  std::string mesg;
public:
  Genfile();
  explicit Genfile( const std::string& newDirectory );
  virtual ~Genfile();
  /// <summary>Returns current status or warning message.</summary>
  ///
  /// <returns>Current status or warning message.</returns>
  const std::string& getmesg() const { return(mesg); }
  void openapp( std::ofstream& out );
  void openin( std::ifstream& in );
  void openout( std::ofstream& out );
  /// <summary>Sets current status or warning message.</summary>
  ///
  /// <param name="newMesg">New message.</param>
  void setmesg ( const std::string& newMesg ) { mesg = newMesg; }
};
