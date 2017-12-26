// Declarations for the <see cref="PiaPageOut"/> class - parent of classes to
// write out results of a non-Statement benefit calculation.

// $Id: PiaPageOut.h 1.20.1.4 2017/10/12 13:07:41EDT 277133 Development  $

#pragma once

#include "PageOut.h"
#include "PiaMethod.h"
class WageIndGeneral;

/// <summary>Parent of classes that manage a page of output from the
/// calculation of a non-Statement Social Security benefit.</summary>
///
/// <remarks>Child classes are <see cref="ChildCareCalc1Page"/>,
/// <see cref="ChildCareCalc2Page"/>, <see cref="DibGuarPage"/>,
/// <see cref="EarningsPage"/>, <see cref="FamilyPage"/>,
/// <see cref="FrozMinPage"/>, <see cref="InsuredPage"/>,
/// <see cref="OldStart1Page"/>, <see cref="OldStart2Page"/>,
/// <see cref="OnePage"/>, <see cref="PiaTable1Page"/>,
/// <see cref="PiaTable2Page"/>, <see cref="ReindWid1Page"/>,
/// <see cref="ReindWid2Page"/>, <see cref="SpecMin1Page"/>,
/// <see cref="SpecMin2Page"/>, <see cref="Summary1Page"/>,
/// <see cref="Summary3Page"/>, <see cref="TaxesPage"/>,
/// <see cref="TransGuar1Page"/>, <see cref="TransGuar2Page"/>,
/// <see cref="WageInd1Page"/>, and <see cref="WageInd2Page"/>.
///
/// For a similar class that is the parent of classes that manage a page of
/// Statement output, see <see cref="PebsPageOut"/>.</remarks>
///
/// <seealso cref="ChildCareCalc1Page"/>
/// <seealso cref="ChildCareCalc2Page"/>
/// <seealso cref="DibGuarPage"/>
/// <seealso cref="EarningsPage"/>
/// <seealso cref="FamilyPage"/>
/// <seealso cref="FrozMinPage"/>
/// <seealso cref="InsuredPage"/>
/// <seealso cref="OldStart1Page"/>
/// <seealso cref="OldStart2Page"/>
/// <seealso cref="OnePage"/>
/// <seealso cref="PiaTable1Page"/>
/// <seealso cref="PiaTable2Page"/>
/// <seealso cref="ReindWid1Page"/>
/// <seealso cref="ReindWid2Page"/>
/// <seealso cref="SpecMin1Page"/>
/// <seealso cref="SpecMin2Page"/>
/// <seealso cref="Summary1Page"/>
/// <seealso cref="Summary3Page"/>
/// <seealso cref="TaxesPage"/>
/// <seealso cref="TransGuar1Page"/>
/// <seealso cref="TransGuar2Page"/>
/// <seealso cref="WageInd1Page"/>
/// <seealso cref="WageInd2Page"/>
/// <seealso cref="PebsPageOut"/>
class PiaPageOut : public PageOut
{
private:
  /// <summary>Page number.</summary>
  int pageNum;
  /// <summary>Years of Social Security Acts.</summary>
  static const char *tableLabel[10];
  /// <summary>Warning for totalization benefit (less than 6 qcs, or insured).
  /// </summary>
  static const std::string warnTotalization;
protected:
  /// <summary>Title at top of page.</summary>
  std::string pageTitle[2];
protected:
  static PiaMethod::cpi_year_type endYearCal( int type );
  static PiaMethod::cpi_year_type firstYearCal( int type );
public:
  PiaPageOut();
  virtual ~PiaPageOut();
  void baseYearOut( int year );
  void biOut( const PiaMethod& piaMethod, const PiaParams& piaParams,
    int eligYear, int type, int piamfb );
  void biOut( const PiaMethod& piaMethod, const PiaParams& piaParams,
    int eligYear, int firstYear, int lastYear, int piamfb );
  void compYear( const WorkerDataGeneral& workerData, const PiaData& piaData,
    const CompPeriod& compPeriod );
  void disInsOut( const PiaData& piaData );
  void disMfbOut( double capt, double amepia, double mfbt );
  void mfbBpOut( const BendMfb& bendMfb );
  void mfbOut( const PiaMethod& piaMethod, const double portionPiaElig[],
    const PercMfb& percMfb, double aime );
  void mfbOut( const PiaMethod& piaMethod, const double portionPiaElig[],
    const PercMfb& percMfb );
  void newTableOut( const PiaMethod&, const PiaParams& piaParams,
    int eligYear, int firstYear, int lastYear, int piamfb );
  void nonIns( char finsCode, const WorkerDataGeneral& workerData, 
    const PiaData& piaData );
  void nonpenOut( double pubpen, const DateMoyr& pubpenDate );
  void osTableOut( const PiaMethod& piaMethod );
  void piaBpOut ( const BendPia& bendPia );
  void piaEligOut ( const PercPia& percPia, const PortionAime& portionAime,
    double piaTemp );
  void piaEntOut( double piaentt );
  void piamfb1( const PiaMethod& piaMethod );
  void piamfb2( const PiaMethod& piaMethod );
  void piamfb3( const PiaMethod& piaMethod );
  void prepareHeader();
  void printIndexedEarningsTable( const WageIndGeneral& wageInd,
    const PiaData& piaData, int eligYear, const AverageWage& earnings,
    int firstYear );
  void printNameSsn( const WorkerDataGeneral& workerData );
  void printRealWageGain( int eligYear, int istart,
    const PiaMethod& piaMethod, double rwgpia, double rwgmfb );
  void pubPenOut( const WageIndGeneral& wageInd, double pubpen,
    const DateMoyr& pubpenDate );
  void rawPiaOut( double rawpia );
  /// <summary>Sets page number.</summary>
  ///
  /// <param name="newPageNum">New page number.</param>
  void setPageNum( int newPageNum ) { pageNum = newPageNum; }
  void tableHead1( double avgbsyr );
  void tableHead2();
  void totPiaOut( const PiaMethod& piaMethod, int qcTotal, int n );
};
