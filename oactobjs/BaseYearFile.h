// Declarations for the <see cref="BaseYearFile"/> class to manage the
// computation year on disk.
//
// $Id: BaseYearFile.h 1.10 2011/07/28 11:50:13EDT 044579 Development  $

#pragma once

#include <string>
#include "baseyear.h"

/// <summary>Manages the computation year for a pia program where it is
/// gotten from disk.</summary>
///
/// <remarks>See <see cref="BaseYearNonFile"/> for a class where the
/// computation year is stored internally.</remarks>
///
/// <seealso cref="BaseYearNonFile"/>
class BaseYearFile : public BaseYear
{
private:
  /// <summary>File information.</summary>
  std::string pathname;
public:
  BaseYearFile( const std::string& newDirectory );
  void read();
  void write();
};
