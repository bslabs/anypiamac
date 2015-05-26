// Declarations for the <see cref="WorkerData"/> class to manage worker data.

// $Id: wrkrdata.h 1.69 2011/08/11 13:55:32EDT 044579 Development  $

#pragma once

#include "WorkerDataGeneral.h"
#include "ssn.h"
#include "MilServDatesVec.h"
#include "railroad.h"
#include "wbgenrl.h"
#include "avgwg.h"

/// <summary>Manages the basic data required to calculate a Social Security
/// benefit.</summary>
///
/// <remarks>It includes extra fields for more detailed calculations and
/// fuller output, compared to the parent class
/// <see cref="WorkerDataGeneral"/>. In particular, it allows for periods of
/// military service (which could generate wage credits) via
/// <see cref="MilServDatesVec"/>, railroad earnings via
/// <see cref="RailRoadData"/>, and a noncovered pension (which could trigger
/// the Windfall Elimination Provision).</remarks>
///
/// <seealso cref="MilServDatesVec"/>
/// <seealso cref="RailRoadData"/>
class WorkerData : public WorkerDataGeneral
{
  /// <summary>Friend class to allow serialization.</summary>
  friend class boost::serialization::access;
  // <summary>Archives the data.</summary>
  //
  // <remarks>When the class Archive corresponds to an output archive, the
  // & operator is defined similar to <<. Likewise, when the class Archive is
  // a type of input archive, the & operator is defined similar to >>.
  // </remarks>
  //
  // <param name="ar">The archive to use.</param>
  // <param name="version">The version of the archive to use.</param>
  template< class Archive >
  void serialize( Archive& ar, const unsigned int version )
  { ar & boost::serialization::base_object<WorkerDataGeneral>(*this);
    ar & ssn & milServDatesVec & railRoadData & earnOasdi & earnHi & taxType;
    ar & nhname & nhaddr & pubpen & pubpenDate & pubpenReservist;
    ar & oabEntDate & oabCessDate; }
public:
  /// <summary>Dimension limits.</summary>
  enum DimLimits {
    /// <summary>Number of address lines.</summary>
    NUM_ADDRESS_LINES = 3
  };
  /// <summary>Social security number of wage earner.</summary>
  Ssn ssn;
  /// <summary>Military service dates.</summary>
  MilServDatesVec milServDatesVec;
  /// <summary>Railroad quarters of coverage and earnings.</summary>
  RailRoadData railRoadData;
protected:
  /// <summary>Worker earnings, 1937 to last year considered, after any
  /// projection of earnings.</summary>
  AverageWage earnOasdi;
  /// <summary>Worker additional Medicare earnings, 1937 to last year
  /// considered.</summary>
  AverageWage earnHi;
  /// <summary>Type of taxes paid.</summary>
  ///
  /// <summary>See <see cref="TaxRates::taxrateType"/> for the meaning of each
  /// bit.</summary>
  BitAnnual taxType;
private:
  /// <summary>Name of wage earner.</summary>
  std::string nhname;
  /// <summary>Address of wage earner.</summary>
  std::string nhaddr[NUM_ADDRESS_LINES];
  /// <summary>Monthly noncovered pension.</summary>
  float pubpen;
  /// <summary>Month and year of noncovered pension entitlement.</summary>
  DateMoyr pubpenDate;
  /// <summary>Monthly noncovered pension, excluding military
  /// reservist pension, for benefits after December 1994.</summary>
  ///
  /// <remarks>Used only if pubpen is positive and getReservist is true.
  /// </remarks>
  float pubpenReservist;
  /// <summary>Month and year of OAB entitlement prior to most recent
  /// DIB entitlement.</summary>
  DateMoyr oabEntDate;
  /// <summary>Month and year of OAB cessation prior to most recent DIB.
  /// </summary>
  DateMoyr oabCessDate;
public:
  WorkerData();
  ~WorkerData();
  /// <summary>Returns true if any self-employed taxes.</summary>
  ///
  /// <returns>True if any self-employed taxes.</returns>
  bool anySelfEmp() const { return taxType.any(); }
  void deleteContents();
  /// <summary>Clears date of OAB cessation prior to most recent DIB.
  /// </summary>
  void deleteOabCessDate() { pubpenDate = DateMoyr(0,0); }
  /// <summary>Clears date of OAB entitlement prior to most recent DIB
  /// entitlement.</summary>
  void deleteOabEntDate() { pubpenDate = DateMoyr(0,0); }
  /// <summary>Clears date of noncovered pension entitlement.</summary>
  void deletePubpenDate() { pubpenDate = DateMoyr(0,0); }
  /// <summary>Returns Medicare earnings for specified year.</summary>
  ///
  /// <returns>Medicare earnings for specified year.</returns>
  ///
  /// <param name="year">Year of earnings.</param>
  double getEarnHi( int year ) const { return earnHi[year]; }
  /// <summary>Returns Medicare earnings for specified year if
  ///  there are no OASDI earnings; otherwise return zero.</summary>
  ///
  /// <returns>Medicare earnings for specified year if 
  ///  there are no OASDI earnings; otherwise return zero.</returns>
  ///
  /// <param name="year">Year of earnings.</param>
  double getEarnHiOnly( int year ) const
  { return (earnOasdi[year] > 4.5) ? 0.0 : earnHi[year]; }
  /// <summary>Returns military service earnings for specified year.</summary>
  ///
  /// <returns>Military service earnings for specified year.</returns>
  ///
  /// <param name="year">Year for which earnings are desired.</param>
  double getEarnMS( int year ) const
  { return milServDatesVec.getEarn(year); }
  /// <summary>Returns total military service earnings, 1937-50.</summary>
  ///
  /// <returns>Total military service earnings, 1937-50.</returns>
  double getEarnMS3750() const { return milServDatesVec.getEarn3750(); }
  /// <summary>Returns OASDI plus Medicare earnings for specified year.
  /// </summary>
  ///
  /// <returns>OASDI plus Medicare earnings for specified year.</returns>
  ///
  /// <param name="year">Year of earnings.</param>
  double getEarnOasdhi( int year ) const
  { return (earnOasdi[year] + earnHi[year]); }
  /// <summary>Returns OASDI-covered earnings for the specified year.
  /// </summary>
  ///
  /// <returns>OASDI-covered earnings for the specified year.</returns>
  ///
  /// <param name="year">Year for which earnings are desired.</param>
  double getEarnOasdi( int year ) const { return earnOasdi[year]; }
  /// <summary>Returns railroad earnings for specified year.</summary>
  ///
  /// <returns>Railroad earnings for specified year.</returns>
  ///
  /// <param name="year">Year for which earnings are desired.</param>
  double getEarnRR( int year ) const
  { return railRoadData.earningsArray[year]; }
  /// <summary>Returns total railroad earnings, 1937-50.</summary>
  ///
  /// <returns>Total railroad earnings, 1937-50.</returns>
  double getEarnRR3750() const { return railRoadData.getEarn3750(); }
  /// <summary>Returns first year of OASDI earnings (0 if none).</summary>
  ///
  /// <returns>First year of OASDI earnings (0 if none).</returns>
  int getFirstYearEarn() const { return earnOasdi.startYear(); }
  /// <summary>Returns first year of HI earnings (0 if none).</summary>
  ///
  /// <returns>First year of HI earnings (0 if none).</returns>
  int getFirstYearEarnHi() const { return earnHi.startYear(); }
  /// <summary>Returns first year of military service (0 if none).</summary>
  ///
  /// <returns>First year of military service (0 if none).</returns>
  int getFirstYearMS() const { return milServDatesVec.getFirstYear(); }
  /// <summary>Returns first year of railroad (0 if none).</summary>
  ///
  /// <returns>First year of railroad (0 if none).</returns>
  int getFirstYearRR() const { return railRoadData.getFirstYear(); }
  /// <summary>Returns Social Security number as a number.</summary>
  ///
  /// <returns>Social Security number as a number.</returns>
  unsigned long getIdNumber() const { return ssn.toNumber(); }
  /// <summary>Returns Social Security number as a string.</summary>
  ///
  /// <returns>Social Security number as a string.</returns>
  std::string getIdString() const { return ssn.toString(); }
  /// <summary>Returns military service indicator.</summary>
  ///
  /// <returns>True if any military service, false otherwise.</returns>
  bool getIndms() const { return milServDatesVec.getMSCount() > 0; }
  /// <summary>Returns last year of OASDI earnings (0 if none).</summary>
  ///
  /// <returns>Last year of OASDI earnings (0 if none).</returns>
  int getLastYearEarn() const { return earnOasdi.endYear(); }
  /// <summary>Returns last year of HI earnings (0 if none).</summary>
  ///
  /// <returns>Last year of HI earnings (0 if none).</returns>
  int getLastYearEarnHi() const { return earnHi.endYear(); }
  /// <summary>Returns last year of military service (0 if none).</summary>
  ///
  /// <returns>Last year of military service (0 if none).</returns>
  int getLastYearMS() const { return milServDatesVec.getLastYear(); }
  /// <summary>Returns last year of military service (0 if none).</summary>
  ///
  /// <returns>Last year of military service (0 if none).</returns>
  int getLastYearRR() const { return railRoadData.getLastYear(); }
  /// <summary>Returns one line of number holder's address.</summary>
  ///
  /// <returns>One line of number holder's address.</returns>
  ///
  /// <param name="line">Desired line number (0-2).</param>
  std::string getNhaddr( int line ) const { return(nhaddr[line]); }
  /// <summary>Returns number holder's name.</summary>
  ///
  /// <returns>Number holder's name.</returns>
  std::string getNhname() const { return(nhname); }
  /// <summary>Returns date of cessation of oab entitlement prior to
  /// most recent DIB entitlement.</summary>
  ///
  /// <returns>The date of cessation of oab entitlement prior to
  /// most recent DIB entitlement.</returns>
  DateMoyr getOabCessDate() const { return(oabCessDate); }
  /// <summary>Returns date of entitlement to oab prior to most
  /// recent DIB entitlement.</summary>
  ///
  /// <returns>The date of entitlement to oab prior to most recent DIB
  /// entitlement.</returns>
  DateMoyr getOabEntDate() const { return(oabEntDate); }
  /// <summary>Returns monthly noncovered pension.</summary>
  ///
  /// <returns>Monthly noncovered pension.</returns>
  float getPubpen() const { return(pubpen); }
  /// <summary>Returns monthly noncovered pension.</summary>
  ///
  /// <returns>Monthly noncovered pension.</returns>
  DateMoyr getPubpenDate() const { return(pubpenDate); }
  /// <summary>Returns monthly noncovered pension after December 1994,
  /// after removing reservist pension.</summary>
  ///
  /// <returns>Monthly noncovered pension after December 1994,
  /// after removing reservist pension.</returns>
  float getPubpenReservist() const { return(pubpenReservist); }
  /// <summary>Returns military service quarters of coverage for specified
  /// year.</summary>
  ///
  /// <returns>Military service quarters of coverage for specified
  /// year.</returns>
  ///
  /// <param name="year">Year for which quarters are desired.</param>
  unsigned getQcovMS( int year ) const
  { return milServDatesVec.getQcov(year); }
  /// <summary>Returns total military service quarters of coverage, 1937-50.
  /// </summary>
  ///
  /// <returns>Total military service quarters of coverage, 1937-50.</returns>
  unsigned getQcovMS3750() const { return milServDatesVec.getQc3750(); }
  /// <summary>Returns railroad quarters of coverage for specified year.
  /// </summary>
  ///
  /// <returns>Railroad quarters of coverage for specified year.</returns>
  ///
  /// <param name="year">Year for which quarters are desired.</param>
  unsigned getQcovRR( int year ) const
  { return railRoadData.qcArray.get(year); }
  /// <summary>Returns total railroad quarters of coverage, 1937-50.</summary>
  ///
  /// <returns>Total railroad quarters of coverage, 1937-50.</returns>
  unsigned getQcovRR3750() const { return railRoadData.getQc3750(); }
  /// <summary>Returns total railroad quarters of coverage, 1951-77.</summary>
  ///
  /// <returns>Total railroad quarters of coverage, 1951-77.</returns>
  unsigned getQcovRR5177() const
  { return railRoadData.qcArray.accumulate(1951, 1977, 0); }
  /// <summary>Returns type of taxes for specified year.</summary>
  ///
  /// <returns>Type of taxes for specified year.</returns>
  ///
  /// <param name="year">Year of taxes.</param>
  bool getTaxType( int year ) const { return taxType.getBit(year); }
  /// <summary>Returns array of type of taxes.</summary>
  ///
  /// <returns>Array of type of taxes.</returns>
  const BitAnnual& getTaxTypeArray() const { return taxType; }
  void pubpenDateCheck2() const;
  void setEarnHi( int year, double newEarnHi );
  void setEarnOasdi( int year, double newEarn );
  /// <summary>Sets one line of address.</summary>
  ///
  /// <param name="line">Line of address to set (0-2).</param>
  /// <param name="newNhaddr">New address to use.</param>
  void setNhaddr( int line, const std::string& newNhaddr )
  { nhaddr[line] = newNhaddr; }
  /// <summary>Sets number holder's name.</summary>
  ///
  /// <param name="newNhname">New name to use.</param>
  void setNhname( const std::string& newNhname ) { nhname = newNhname; }
  void setOabCessDate( const DateMoyr& datetemp );
  void setOabEntDate( const DateMoyr& datetemp );
  void setPubpen( float newPubpen );
  void setPubpenDate( const DateMoyr& newPubpenDate );
  void setPubpenReservist( float newPubpen );
  void setTaxType( int year, int newTaxType );
  void zeroEarn( int ibegint, int iendt );
  void zeroEarnHi( int ibegint, int iendt );
  void zeroEarnOasdi( int ibegint, int iendt );
};
