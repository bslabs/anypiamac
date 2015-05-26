// Declarations for the <see cref="ConfigFile"/> class to manage configuration
// information used by the PIA calculation program stored on disk.
//
// $Id: ConfigFile.h 1.11 2011/07/29 13:49:26EDT 044579 Development  $

#pragma once

#include <string>
#include "config.h"

/// <summary>Manages the configuration information used by the PIA calculation
/// program, stored in a file.</summary>
///
/// <remarks>See <see cref="ConfigNonFile"/> for a version of this class
/// that stores the configuration information internally.</remarks>
///
/// <seealso cref="ConfigNonFile"/>
class ConfigFile : public Config
{
private:
  /// <summary>File information.</summary>
  std::string pathname;
public:
  ConfigFile( const std::string& newDirectory );
  ~ConfigFile();
  void read();
  void write();
};
