// Declarations for the <see cref="FieldOfficeInfoFile"/> class to manage field
// office information stored on disk.
//
// $Id: FieldOfficeInfoFile.h 1.10 2011/07/29 15:26:15EDT 044579 Development  $

#pragma once

#include <string>
#include "foinfo.h"

/// <summary>Reads and writes field office information from and to a file.
/// </summary>
///
/// <remarks>See <see cref="FieldOfficeInfoNonFile"/> for a version of this
/// class that stores the field office information internally.</remarks>
///
/// <seealso cref="FieldOfficeInfoNonFile"/>
class FieldOfficeInfoFile : public FieldOfficeInfo
{
private:
  /// <summary>File information.</summary>
  std::string pathname;
public:
  FieldOfficeInfoFile( const std::string& newDirectory );
  ~FieldOfficeInfoFile();
  void setData();
  void read();
  void write();
};
