// Declarations for the <see cref="Qcamt"/> class to manage array of amounts
// required for quarter of coverage.
//
// $Id: qcamt.h 1.11 2011/08/09 14:58:43EDT 044579 Development  $

#pragma once

#include "dbleann.h"

/// <summary>Manages an array of quarter of coverage amounts.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaParams"/>.
/// </remarks>
class Qcamt : public DoubleAnnual
{
public:
  /// <summary>The year when automatic increases started for the quarter
  /// of coverage amount.</summary>
  static const int AUTO_YEAR = 1978;
  /// <summary>The quarter of coverage amount in 1978.</summary>
  static const double qcamt1978;
  /// <summary>The quarter of coverage amount prior to 1978.</summary>
  static const double qcamt1977;
public:
  Qcamt( int newLastYear );
  Qcamt( int newBaseYear, int newLastYear );
  ~Qcamt();
  void project( const DoubleAnnual& averageWages, int firstYeart );
};
