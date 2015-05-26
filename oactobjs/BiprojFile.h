// Declarations for the <see cref="BiprojFile"/> class to manage projected
// benefit increases stored on disk.

// $Id: BiprojFile.h 1.15 2011/07/29 11:25:03EDT 044579 Development  $

#pragma once

#include "biproj.h"

/// <summary>Manages the file of projected benefit increases stored on disk.
/// </summary>
///
/// <remarks>See <see cref="BiprojNonFile"/> for a version of this class
/// that stores the benefit increases internally.</remarks>
///
/// <seealso cref="BiprojNonFile"/>
class BiprojFile : public Biproj
{
private:
  /// <summary>File information.</summary>
  std::string pathname;
protected:
  void setBiFile( int altNum );
public:
  BiprojFile( int newIstart, int newMaxyear,
    const std::string& newDirectory );
  void read( int altNum );
  void setData( int );
  void setData( int, const DoubleAnnual& );
  void setTitle( int altNum );
  void setTitle( int altNum, const std::string& );
  void write( int altNum );
};
