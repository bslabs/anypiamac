// Declaration of the <see cref="AwbiDataFile"/> class to manage historical
// Social Security parameters stored in a file.
//
// $Id: AwbiDataFile.h 1.10 2011/07/28 10:16:26EDT 044579 Development  $

#pragma once

#include <string>
#include <iosfwd>
#include "awbidat.h"

/// <summary>Manages the historical Social Security parameters stored in a
/// file.</summary>
///
/// <remarks>See <see cref="AwbiDataNonFile"/> for a version of this class
/// that stores the parameters internally.</remarks>
///
/// <seealso cref="AwbiDataNonFile"/>
class AwbiDataFile : public AwbiData
{
private:
  /// <summary>File information.</summary>
  std::string pathname;
public:
  AwbiDataFile( int newLastYear, int newMaxyear,
    const std::string& newDirectory );
  void read();
  void read( std::istream& in );
  void setAwbiFile();
  void setData();
  void write();
  void write( std::ostream& out );
};
