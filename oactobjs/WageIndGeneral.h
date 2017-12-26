// Declarations for the <see cref="WageIndGeneral"/> class - parent of classes
// to manage wage-indexed pia calculations.

// $Id: WageIndGeneral.h 1.16 2017/10/12 12:48:34EDT 277133 Development  $

#pragma once

#include "PiaMethod.h"

/// <summary>Parent of all classes that manage the functions required for
/// calculation of a wage indexed Social Security benefit.</summary>
///
/// <remarks>Child classes are <see cref="WageInd"/> and
/// <see cref="ChildCareCalc"/>.</remarks>
///
/// <seealso cref="WageInd"/>
/// <seealso cref="ChildCareCalc"/>
class WageIndGeneral : public PiaMethod
{
public:
  /// <summary>Bend points for MFB formula.</summary>
  BendMfb bendMfb;
  /// <summary>Bend points for PIA formula.</summary>
  BendPia bendPia;
  /// <summary>Wage-indexed MFB formula percentages.</summary>
  PercMfb percMfb;
  /// <summary>Wage-indexed PIA formula percentages.</summary>
  PercPia percPia;
  /// <summary>Windfall PIA formula percentages.</summary>
  PercPia percWind;
  /// <summary>Portion of AIME in each interval of PIA formula.</summary>
  PortionAime portionAime;
  /// <summary>Portion of AIME PIA in each interval of MFB formula.</summary>
  double portionPiaElig[4];
  /// <summary>MFB at entitlement before real-wage-gain adjustment.</summary>
  BenefitAmount mfbRealWageGain;
  /// <summary>PIA at entitlement before real-wage-gain adjustment.</summary>
  BenefitAmount piaRealWageGain;
protected:
  /// <summary>Windfall indicator.</summary>
  WindfallElimType windfallInd;
  /// <summary>Number of years of coverage for windfall elimination.</summary>
  int yearsTotal;
  /// <summary>Average wage in indexing year.</summary>
  double indexYearAvgWage;
public:
  WageIndGeneral( const WorkerDataGeneral& newWorkerData,
    const PiaData& newPiaData, const PiaParams& newPiaParams,
    int newMaxyear, const std::string& newTitle, pia_type newMethod );
  virtual ~WageIndGeneral();
  double aimepiaCal( const PortionAime& portionAime, const PercPia& percPiaTemp,
    int year );
  void bendPointCal( int eligYear, BendPia& bendPiaTemp ) const;
  double deconvertAme( const BendPia& bendPiaTemp,
    const PercPia& percPiaTemp ) const;
  /// <summary>Returns average wage in indexing year.</summary>
  ///
  /// <returns>Average wage in indexing year.</returns>
  double getIndexYearAvgWage() const { return indexYearAvgWage; }
  /// <summary>Returns windfall indicator in wage-indexed method.</summary>
  ///
  /// <returns>Windfall indicator in wage-indexed method.</returns>
  PiaMethod::WindfallElimType getWindfall() const { return(windfallInd); }
  int getYearsTotal() const;
  void indexEarnings( int year1, int year2, int year3,
    const DoubleAnnual& earnings, DoubleAnnual& earnMultiplied,
    DoubleAnnual& earnIndexed, const DoubleAnnual& avgWage );
  void initialize();
  void realWageGainAdj( int eligYear );
  /// <summary>Sets windfall indicator in wage-indexed method.</summary>
  ///
  /// <param name="newWindfall">New windfall indicator.</param>
  void setWindfall( WindfallElimType newWindfall )
  { windfallInd = newWindfall; }
  void windfallCal();
  static void setPortionAime( double amesub, PortionAime& aimePart,
    const BendPia& bendPiaTemp );
private:
  WageIndGeneral& operator=( WageIndGeneral& newWageIndGeneral );
};
