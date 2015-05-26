// Declarations for the <see cref="PageCounters"/> class to manage page
// counters.

// $Id: page.h 1.18 2011/08/19 15:57:43EDT 044579 Development  $

#pragma once

#include <vector>
#include "document.h"

/// <summary>Manages a vector of page counters.</summary>
///
/// <remarks>Manages a vector of page counters for use in runstreams with
/// multiple programs. This were formerly used in the ssgiat, ssgbp, and
/// jrunfunds runstreams, but since they were converted to html output, this
/// class is no longer used.</remarks>
class PageCounters : public Document
{
public:
  /// <summary>Error code.</summary>
  static const unsigned int PIA_IDS_PAGE1 = 1;
  /// <summary>Error code.</summary>
  static const unsigned int PIA_IDS_PAGE2 = 2;
  /// <summary>Error code.</summary>
  static const unsigned int PIA_IDS_PAGE3 = 3;
private:
  /// <summary>Page counters.</summary>
  std::vector< int > theData;
public:
  explicit PageCounters( int maxCounters );
  /// <summary>Destructor.</summary>
  ///
  /// <remarks>Does nothing.</remarks>
  ~PageCounters() { }
  void deleteContents();
  /// <summary>Returns page counter for one slot.</summary>
  ///
  /// <returns>Page counter for one slot.</returns>
  ///
  /// <param name="slot">Page counter slot to get.</param>
  int get( int slot ) const { return(theData[slot]); }
  void read();
  /// <summary>Sets page counter for one slot.</summary>
  ///
  /// <param name="slot">Page counter slot to use.</param>
  /// <param name="newPageNum">New page number.</param>
  void set( int slot, int newPageNum ) { theData[slot] = newPageNum; }
  void update( int slot );
  void write();
private:
  PageCounters& operator=( const PageCounters& );
};
