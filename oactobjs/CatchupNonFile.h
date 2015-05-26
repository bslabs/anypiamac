// Declarations for the <see cref="CatchupNonFile"/> class to manage a set of
// catch-up benefit increases stored in arrays.
//
// $Id: CatchupNonFile.h 1.10 2011/07/29 13:49:25EDT 044579 Development  $

#pragma once

#include "cachupf.h"

/// <summary>Manages a file of catch-up benefit increases stored in arrays.
/// </summary>
///
/// <remarks>See <see cref="CatchupFile"/> for a version of this class
/// that stores the catch-up benefit increases on disk.</remarks>
///
/// <seealso cref="CatchupFile"/>
class CatchupNonFile : public CatchupDoc
{
private:
  /// <summary>Stored catch-up increases from Trustees Report.</summary>
  Catchup cachuptr[4];
public:
  CatchupNonFile( int newCstart );
  ~CatchupNonFile();
  void read( int altNum );
  void setData ( int altNum );
  void setData ( int altNum, const Catchup& newCatchup );
  void write( int );
};
