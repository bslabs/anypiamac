// Functions for the <see cref="FieldOfficeInfo"/> class to manage field
// office information.
//
// $Id: foinfo.cpp 1.12 2011/07/29 15:26:17EDT 044579 Development  $

#include "foinfo.h"

/// <summary>Initializes field office info.</summary>
FieldOfficeInfo::FieldOfficeInfo() : foname(), fotitle()
{
  foaddr[0] = "Social Security Administration";
}

/// <summary>Destroys field office information.</summary>
FieldOfficeInfo::~FieldOfficeInfo()
{ }
