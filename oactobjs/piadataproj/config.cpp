// Functions for the <see cref="Config"/> class to manage configuration
// information used by the PIA calculation program.
//
// $Id: config.cpp 1.14 2011/07/29 13:49:27EDT 044579 Development  $

#include "config.h"

/// <summary>Initializes configuration.</summary>
Config::Config() : longOutput(true), screenPoints(12), printerPoints(10),
topMargin(0.5f), leftMargin(0.5f)
{ }

/// <summary>Destroys configuration.</summary>
Config::~Config()
{ }
