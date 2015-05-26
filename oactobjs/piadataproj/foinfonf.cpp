// Functions for the <see cref="FieldOfficeInfoNonFile"/> class to manage
// field office information stored in memory.
//
// $Id: foinfonf.cpp 1.8 2011/07/29 15:26:15EDT 044579 Development  $

#include "FieldOfficeInfoNonFile.h"

/// <summary>Initializes field office info.</summary>
FieldOfficeInfoNonFile::FieldOfficeInfoNonFile() : FieldOfficeInfo()
{ }

/// <summary>Destroys field office info.</summary>
FieldOfficeInfoNonFile::~FieldOfficeInfoNonFile()
{ }

/// <summary>Initializes name and address.</summary>
void FieldOfficeInfoNonFile::setData()
{
  setFieldOfficeName("Field Office Manager's Name");
  setFieldOfficeTitle("Field Office Manager's Title");
  setAddress(1, "Address Line 1");
  setAddress(2, "Address Line 2");
  setAddress(3, "Address Line 3");
}

/// <summary>Reads field office information (dummy function).</summary>
void FieldOfficeInfoNonFile::read()
{ }

/// <summary>Saves field office information (dummy function).</summary>
void FieldOfficeInfoNonFile::write()
{ }
