// Declarations for the <see cref="LawChangeWrite"/> class to write law
// changes to a file.

// $Id: LawChangeWrite.h 1.12 2011/08/01 11:38:35EDT 044579 Development  $

#pragma once

#include <iosfwd>
class LawChangeArray;

/// <summary>Writes a set of law changes to a file.</summary>
///
/// <remarks>Writes a <see cref="LawChangeArray"/> to a file.</remarks>
class LawChangeWrite
{
private:
  /// <summary>Law change parameters.</summary>
  LawChangeArray& lawChange;
public:
  LawChangeWrite( LawChangeArray& newLawChange );
  void write();
  void write( std::ostream& out );
private:
  LawChangeWrite operator=( LawChangeWrite newLawChangeWrite );
};
