// Declarations for the <see cref="CatchupFile"/> class to manage file of
// catch-up benefit increases.

// $Id: CatchupFile.h 1.12 2011/07/29 13:49:24EDT 044579 Development  $

#pragma once

#include "cachupf.h"

/// <summary>Manages a file of catch-up benefit increases stored on disk.
/// </summary>
///
/// <remarks>See <see cref="CatchupNonFile"/> for a version of this class
/// that stores the catch-up benefit increases internally.</remarks>
///
/// <seealso cref="CatchupNonFile"/>
class CatchupFile : public CatchupDoc
{
protected:
  /// <summary>File information.</summary>
  std::string pathname;
protected:
  void setCuFile( int altNum );
public:
  CatchupFile( int newCstart, const std::string& newDirectory );
  ~CatchupFile();
  void read( int altNum );
  void setData( int altNum );
  void setData( int altNum, const Catchup& );
  void write( int altNum );
};
