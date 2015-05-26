// Declarations for the <see cref="SpecMin"/> class to manage special minimum
// pia calculations.
//
// $Id: SpecMin.h 1.17 2011/08/09 15:59:14EDT 044579 Development  $

#pragma once

#include "OldPia.h"

/// <summary>Manages the functions required for calculation of a special
/// minimum Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalPL"/>.
/// </remarks>
///
/// <seealso cref="PiaCalPL"/>
class SpecMin : public OldPia
{
private:
  /// <summary>Number of years of coverage in special minimum
  /// in excess of 10, up to 30.</summary>
  int specMinYearsExcess;
  /// <summary>Special minimum dollar amount per year of coverage.</summary>
  double specMinAmount;
  /// <summary>Total number of years of coverage.</summary>
  int yearsTotal;
public:
  SpecMin( const WorkerDataGeneral& newWorkerData, const PiaData& newPiaData,
    const PiaParams& newPiaParams, int newMaxyear );
  virtual ~SpecMin();
  void calculate();
  /// <summary>Returns number of years of coverage in special minimum
  /// in excess of 10, up to 30.</summary>
  //
  /// <returns>Number of years of coverage in special minimum
  /// in excess of 10, up to 30.</returns>
  int getSpecMinYearsExcess() const { return(specMinYearsExcess); }
  /// <summary>Returns special minimum dollar amount per year of coverage.
  /// </summary>
  ///
  /// <returns>Special minimum dollar amount per year of coverage.</returns>
  double getSpecMinAmount() const { return(specMinAmount); }
  /// <summary>Returns number of years of coverage.</summary>
  ///
  /// <returns>Number of years of coverage.</returns>
  int getYearsTotal() const { return(yearsTotal); }
  void initialize();
  bool isApplicable();
  /// <summary>Sets special minimum dollar amount per year of coverage.
  /// </summary>
  ///
  /// <param name="newAmount">New dollar amount per year of coverage.</param>
  void setSpecMinAmount( double newAmount ) { specMinAmount = newAmount; }
  /// <summary>Sets number of years of coverage in special minimum in
  ///  excess of 10, up to 30.</summary>
  ///
  /// <param name="newYears">New number of years of coverage.</param>
  void setSpecMinYearsExcess( int newYears )
  { specMinYearsExcess = newYears; }
  virtual int specMinYearsExcessCal() const;
  static bool isApplicable( const WorkerDataGeneral& workerData );
private:
  SpecMin operator=( SpecMin newSpecMin );
};
