// Declarations for the <see cref="FieldOfficeInfoNonFile"/> class to manage
// field office information stored in memory.
//
// $Id: FieldOfficeInfoNonFile.h 1.6 2011/07/29 15:26:16EDT 044579 Development  $

#pragma once

#include "foinfo.h"

/// <summary>Manages field office information not stored in a file
/// (but its data may be saved in an .ini file).</summary>
///
/// <remarks>See <see cref="FieldOfficeInfoFile"/> for a version of this
/// class that stores the field office information internally.</remarks>
///
/// <seealso cref="FieldOfficeInfoFile"/>
class FieldOfficeInfoNonFile : public FieldOfficeInfo
{
public:
  FieldOfficeInfoNonFile();
  ~FieldOfficeInfoNonFile();
  void setData();
  void read();
  void write();
};
