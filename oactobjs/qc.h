// Declarations for the <see cref="Qc"/> class to manage one annual quarters
// of coverage number.
//
// $Id: qc.h 1.17 2011/08/09 14:58:42EDT 044579 Development  $

#pragma once

/// <summary>Handles calculations with quarters of coverage for one year.
/// </summary>
///
/// <remarks>The number of quarters of coverage for one year must not exceed 4.
/// This class has two static functions to handle qc calculations.</remarks>
class Qc
{
public:
  /// <summary>The maximum number of qcs in one year (4).</summary>
  static const unsigned int MAXQCSINYEAR = 4u;
  static void check( unsigned newNumber );
  static unsigned qcCal( double earnings, double qcAmount );
private:
  Qc();
  Qc( const Qc& qc );
  ~Qc();
  Qc& operator=( const Qc& qc );
};
