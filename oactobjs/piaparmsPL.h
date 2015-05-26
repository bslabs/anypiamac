// Declarations for the <see cref="PiaParamsPL"/> class of pia calculation
// parameters using present law only.

// $Id: piaparmsPL.h 1.31 2011/08/05 16:30:20EDT 044579 Development  $

#pragma once

#include "piaparms.h"

/// <summary>Manages the present law Social Security parameters required to
/// calculate Social Security benefits.</summary>
///
/// <remarks>Manages the present law Social Security parameters required to
/// calculate Social Security benefits.</remarks>
class PiaParamsPL : public PiaParams
{
private:
  /// <summary>Wage base under present law, 1937 to maxyear.</summary>
  WageBase baseOasdi;
  /// <summary>Old-law wage bases (ignoring 1979-81 ad hoc increases),
  /// 1937 to maxyear.</summary>
  WageBaseOldLaw base77;
public:
  PiaParamsPL( int newIstart, int newMaxyear, AwbiData& awbiData );
  explicit PiaParamsPL( int newMaxyear );
  ~PiaParamsPL();
  Age earlyAgeOabCal( Sex::sex_type sex,
    const boost::gregorian::date& kbirth ) const;
  double factorAgedSpouseCal( int, int ) const;
  double factorAgedWidCal( int monthsArdri, const Age& age, int,
    const DateMoyr& benefitDate ) const;
  double factorArAgedSpouseCal( int monthsArdri ) const;
  double factorArCal( int monthsArdri ) const;
  double factorDisWidCal( int, const DateMoyr& benefitDate ) const;
  Age fullRetAgeCal( int eligYear ) const;
  Age fullRetAgeCalDI( int eligYear, int currentYear ) const;
  /// <summary>Returns the array of OASDI wage bases.</summary>
  ///
  /// <returns>The array of OASDI wage bases.</returns>
  const WageBase& getBaseOasdiArray() const { return baseOasdi; }
  /// <summary>Returns one OASDI wage base.</summary>
  ///
  /// <returns>One OASDI wage base.</returns>
  ///
  /// <param name="year">Year for which base is desired.</param>
  double getBaseOasdi( int year ) const { return baseOasdi[year]; }
  /// <summary>Returns the array of old-law wage bases.</summary>
  ///
  /// <returns>The array of old-law wage bases.</returns>
  const WageBaseOldLaw& getBase77Array() const { return base77; }
  /// <summary>Returns one old-law wage base.</summary>
  ///
  /// <returns>One old-law wage base.</returns>
  ///
  /// <param name="year">Year for which base is desired.</param>
  double getBase77( int year ) const { return base77[year]; }
  double getChildcareDropoutAmount( int, int ) const;
  double getColaCap( int ) const;
  const AverageWage& getFqBppia() const;
  const AverageWage& getFqIndex() const;
  int getMarrLengthForDivBen( const DateMoyr& dateMoyr ) const;
  int getMaxChildcareAge() const;
  int getMaxChildcareDropoutYears( int, int ) const;
  double getMfbColaCap( int ) const;
  Age maxDibAge( int year ) const;
  bool needColaCap() const;
  bool needColaCap( int ) const;
  void projectCpiinc();
  double ribLimCal( double widowBen, double oabPia, double oabBen,
    const DateMoyr& benefitDate ) const;
  void setColaCap( double, int );
  void setData( AwbiData& awbiData );
  void setMfbColaCap( double, int );
  double specMinAmountCal( const DateMoyr& date ) const;
  int specMinMaxYears() const;
  int specMinMaxYears( int, int ) const;
  void updateBases();
  void updateBases( const DoubleAnnual& userbase,
    const DoubleAnnual& userbase77, BaseChangeType::change_type ibasch,
    int firstYeart, int lastYeart );
};
