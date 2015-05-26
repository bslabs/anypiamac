// Declarations for the <see cref="LawChangeMARRLENGTH"/> class to manage
// parameters for a proposal to change the length of marriage requirement for
// divorced spouse benefits.
//
// $Id: LawChangeMARRLENGTH.h 1.7 2011/08/01 10:06:10EDT 044579 Development  $

#pragma once

#include "LawChange.h"

/// <summary>Parameters for a proposal to change the length of marriage
/// requirement for divorced spouse benefits.</summary>
///
/// <remarks>There is one additional data field compared to the base class.
/// </remarks>
class LawChangeMARRLENGTH : public LawChange
{
private:
  /// <summary>Number of years married needed for divorced spouse benefit.
  /// </summary>
  int marrLengthForDivBen;
public:
  LawChangeMARRLENGTH();
  /// <summary>Returns the number of years of marriage needed for a divorced
  /// spouse benefit.</summary>
  ///
  /// <returns>The number of years of marriage needed for a divorced spouse
  /// benefit.</returns>
  int getMarrLengthForDivBen() const { return marrLengthForDivBen; }
  void prepareStrings( std::vector< std::string >& outputString );
  void read( std::istream& infile );
  /// <summary>Sets the number of years of marriage needed for a divorced
  /// spouse benefit.</summary>
  ///
  /// <param name="newMarrLength">New number of years of marriage.</param>
  void setMarrLengthForDivBen( int newMarrLength )
  { marrLengthForDivBen = newMarrLength; }
  void write( std::ostream& out ) const;
};
