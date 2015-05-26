// Declarations for the <see cref="ConfigNonFile"/> class to manage
// configuration information used by the PIA calculation program stored
// internally.
//
// $Id: ConfigNonFile.h 1.6 2011/07/29 13:49:26EDT 044579 Development  $

#pragma once

#include "config.h"

/// <summary>Manages the configuration information used by
/// the PIA calculation program, stored with the code.</summary>
///
/// <remarks>See <see cref="ConfigFile"/> for a version of this class
/// that stores the configuration information on disk.</remarks>
///
/// <seealso cref="ConfigFile"/>
class ConfigNonFile : public Config
{
public:
  ConfigNonFile();
  ~ConfigNonFile();
  void read();
  void write();
};
