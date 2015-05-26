// Declarations for the <see cref="LawChangeRead"/> class to read law
// changes from a file.

// $Id: LawChangeRead.h 1.12 2011/08/01 10:06:09EDT 044579 Development  $

#pragma once

#include <iosfwd>
class LawChangeArray;

/// <summary>Reads law changes stored in a file.</summary>
///
/// <remarks>Reads a <see cref="LawChangeArray"/> stored in a file.</remarks>
class LawChangeRead
{
private:
  /// <summary>Law change parameters.</summary>
  LawChangeArray& lawChange;
public:
  LawChangeRead( LawChangeArray& newLawChange );
  void read();
  void read( std::istream& infile );
private:
  LawChangeRead operator=( LawChangeRead newLawChangeRead );
};
