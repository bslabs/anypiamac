// Declarations for the <see cref="OldStart"/> class to manage old-start pia
// calculations.
//
// $Id: OldStart.h 1.22 2011/08/01 11:38:34EDT 044579 Development  $

#pragma once

#include "OldPia.h"

/// <summary>Manages the functions required for calculation of an old start
/// Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaCalPL"/>.
/// </remarks>
class OldStart : public OldPia
{
public:
  /// <summary>Old-start PIA calculation methods.</summary>
  enum OldStartType {
    OS1939 = 0,  /// 1939 Amendments.
    OS1950,      /// 1950 old-start.
    OS1958,      /// 1950 old-start with 1958 conversion table.
    OS1965,      /// 1965 old-start.
    OS1967,      /// 1967 old-start.
    OS1977_78,   /// 1977 old-start with Jan 1978 old-law PIA table.
    OS1977_79,   /// 1977 old-start with Dec 1978 frozen PIA table.
    OS1990,      /// 1990 old-start.
    NUM_OLD_START_TYPES  // Number of old-start PIA methods (8).
  };
  /// <summary>Factor for amount of increment per year (0.01).</summary>
  static const double INC_PER_YEAR;
public:
  /// <summary>Bend points for MFB formula.</summary>
  BendMfb bendMfb;
  /// <summary>Wage-indexed MFB formula percentages.</summary>
  PercMfb percMfb;
  /// <summary>Portion of AME in each interval of PIB formula.</summary>
  double portionAme[2];
  /// <summary>Portion of AIME PIA in each interval of MFB formula.</summary>
  double portionPiaElig[4];
protected:
  /// <summary>The date of entitlement.</summary>
  const DateMoyr entDate;
private:
  /// <summary>Old-start AME.</summary>
  int ameOs;
  /// <summary>Divisor for old-start pre-1951 earnings.</summary>
  int divisorOs;
  /// <summary>Number of old-start increment years.</summary>
  int incyrs;
  /// <summary>Old-start PIA calculation method.</summary>
  OldStartType methodOs;
  /// <summary>Old-start PIB before incrementing.</summary>
  double pib;
  /// <summary>Old-start PIB after incrementing.</summary>
  double pibInc;
public:
  OldStart( const WorkerDataGeneral& newWorkerData, const PiaData& newPiaData,
    const PiaParams& newPiaParams, int newMaxyear,
    const DateMoyr& newEntDate );
  virtual ~OldStart();
  void calculate();
  /// <summary>Returns old-start AME.</summary>
  ///
  /// <returns>Old-start AME.</returns>
  int getAmeOs() const { return(ameOs); }
  /// <summary>Returns divisor for old-start pre-1951 earnings.</summary>
  ///
  /// <returns>Divisor for old-start pre-1951 earnings.</returns>
  int getDivisorOs() const { return(divisorOs); }
  /// <summary>Returns number of old-start increment years.</summary>
  ///
  /// <returns>Number of old-start increment years.</returns>
  int getIncyrs() const { return(incyrs); }
  /// <summary>Returns old-start calculation method number.</summary>
  ///
  /// <returns>Old-start calculation method number.</returns>
  OldStartType getMethodOs() const { return(methodOs); }
  /// <summary>Returns old-start PIB before incrementing.</summary>
  ///
  /// <returns>Old-start PIB before incrementing.</returns>
  double getPib() const { return(pib); }
  /// <summary>Returns old-start PIB after incrementing.</summary>
  ///
  /// <returns>Old-start PIB after incrementing.</returns>
  double getPibInc() const { return(pibInc); }
  void imputeEarnings();
  int incrementCal() const;
  int incrementOldCal() const;
  void initialize();
  bool isApplicable();
  /// <summary>Returns true if the old-start method is 1977 or 1990.</summary>
  ///
  /// <returns>True if the old-start method is 1977 or 1990.</returns>
  bool isOs1977_90() const
  { return (methodOs == OS1977_78 || methodOs == OS1977_79 ||
    methodOs == OS1990); }
  OldStartType methodOsCal();
  void oldStart39Cal();
  void oldStart50Cal();
  /// <summary>Sets old-start AME.</summary>
  ///
  /// <param name="newAmeOs">New old-start AME.</param>
  void setAmeOs ( int newAmeOs ) { ameOs = newAmeOs; }
  /// <summary>Sets divisor for old-start pre-1951 earnings.</summary>
  ///
  /// <param name="newDivisorOs">New divisor for old-start
  ///  pre-1951 earnings.</param>
  void setDivisorOs( int newDivisorOs ) { divisorOs = newDivisorOs; }
  /// <summary>Sets number of old-start increment years.</summary>
  ///
  /// <param name="newIncyrs">New number of old-start increment years.</param>
  void setIncyrs( int newIncyrs ) { incyrs = newIncyrs; }
  /// <summary>Sets old-start calculation method number.</summary>
  ///
  /// <param name="newMethodOs">New old-start calculation method number.
  /// </param>
  void setMethodOs( OldStartType newMethodOs ) { methodOs = newMethodOs; }
  /// <summary>Sets old-start PIB before incrementing.</summary>
  ///
  /// <param name="newPib">New PIB before incrementing.</param>
  void setPib( double newPib ) { pib = newPib; }
  /// <summary>Sets old-start PIB with increment years.</summary>
  void setPibInc()
  { pibInc = pib * (1.0 + INC_PER_YEAR * static_cast<double>(incyrs)); }
  static bool isApplicable( const PiaData& piaData );
private:
   OldStart operator=( OldStart newOldStart );
};
