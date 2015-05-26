// Declarations for the <see cref="PiaParamsLC"/> class to handle changed
// law PIA parameters.

// $Id: piaparmsLC.h 1.51 2011/08/05 16:30:20EDT 044579 Development  $

#pragma once

#include "piaparms.h"
class LawChangeArray;
#include "wbgenrlLC.h"

/// <summary>Manages the changed law Social Security parameters required to
/// calculate Social Security benefits.</summary>
///
/// <remarks>Manages the changed law Social Security parameters required to
/// calculate Social Security benefits.</remarks>
class PiaParamsLC : public PiaParams
{
private:
  /// <summary>Wage base under present law, 1937 to maxyear.</summary>
  WageBaseLC baseOasdi;
  /// <summary>Old-law wage bases (ignoring 1979-81 ad hoc increases), 1937
  /// to maxyear.</summary>
  WageBaseOldLawLC base77;
  /// <summary>Alternate benefit increases, 1937 to maxyear.</summary>
  DoubleAnnual altCpiinc;
  /// <summary>Dollar amount caps on the COLA for each year.</summary>
  DoubleAnnual colaCaps;
  /// <summary>Indexing series for pia formula bend points when a mixture
  /// of average wages and other amounts.</summary>
  AverageWage fqbppia;
  /// <summary>Indexing series for earnings record when a mixture
  /// of average wages and other amounts.</summary>
  AverageWage fqindex;
  /// <summary>Law change data.</summary>
  const LawChangeArray& lawChange;
  /// <summary>Dollar amount caps on the COLA for the MFB for
  /// each year.</summary>
  DoubleAnnual mfbColaCaps;
  /// <summary>Array of extra special minimum pias.</summary>
  std::vector< double > specMinPiaExtra;
  /// <summary>Array of extra special minimum mfbs.</summary>
  std::vector< double > specMinMfbExtra;
public:
  PiaParamsLC( int newIstart, int newMaxyear, AwbiData& awbiData,
    const LawChangeArray& newLawChange );
  PiaParamsLC( int newMaxyear, const LawChangeArray& newLawChange );
  virtual ~PiaParamsLC();
  double applyCola( double pia, int year ) const;
  double applyCola( double pia, int year, int eligYear ) const;
  double applyColaMfb( double mfb, int year, double pia ) const;
  double applyColaMfb( double mfb, int year, int eligYear,
    double pia ) const;
  Age earlyAgeOabCal( Sex::sex_type sex,
    const boost::gregorian::date& kbirth ) const;
  double factorAgedSpouseCal( int eligYear, int benYear ) const;
  double factorAgedWidCal( int monthsArdri, const Age& age,
    int eligYear, const DateMoyr& benefitDate ) const;
  double factorArAgedSpouseCal( int monthsArdri ) const;
  double factorArCal( int monthsArdri ) const;
  double factorDisWidCal( int eligYear, const DateMoyr& benefitDate ) const;
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
  double getChildcareDropoutAmount( int eligYear, int benYear ) const;
  double getColaCap( int year ) const;
  double getCpiinc( int year ) const;
  const DoubleAnnual& getCpiincArray() const;
  const AverageWage& getFqBppia() const;
  const AverageWage& getFqIndex() const;
  int getMarrLengthForDivBen( const DateMoyr& dateMoyr ) const;
  int getMaxChildcareAge() const;
  int getMaxChildcareDropoutYears( int eligYear, int benYear ) const;
  double getMfbColaCap( int year ) const;
  double getSpecMinPia( const DateMoyr& dateMoyr, int yoc ) const;
  double getSpecMinMfb( const DateMoyr& dateMoyr, int yoc ) const;
  void initdata();
  Age  maxDibAge( int year ) const;
  bool needColaCap() const;
  bool needColaCap( int ) const;
  void projectCpiinc();
  void projectFq();
  void projectPerc();
  void projectSpecMin( int baseYear, int lastYear );
  void resizeSpecMin();
  double ribLimCal( double widowBen, double oabPia, double oabBen,
    const DateMoyr& benefitDate ) const;
  void setData( AwbiData& awbiData );
  int specMinMaxYears() const;
  int specMinMaxYears( int eligYear, int benYear ) const;
  double specMinAmountCal( const DateMoyr& date ) const;
  void updateBases();
  void updateBases( const DoubleAnnual& userbase,
    const DoubleAnnual& userbase77, BaseChangeType::change_type ibasch,
    int firstYeart, int lastYeart );
protected:
  void setAltCpiinc();
  void setColaCap( double cap, int year );
  void setFqBppia();
  void setFqIndex();
  void setMfbColaCap( double cap, int year );
private:
  PiaParamsLC operator=( PiaParamsLC& newPiaParamsLC );
};
