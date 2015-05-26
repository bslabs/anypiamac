// Declarations for the <see cref="PiaCal"/> class - parent of all classes
// that manage the calculation of a Social Security benefit.
//
// $Id: piacal.h 1.81 2013/10/29 13:12:17EDT 277133 Development  $

#pragma once

#include <vector>
#include "PiaMethod.h"
#include "SecondaryArray.h"
#include "inscode.h"
class Secondary;
class PiaDataArray;
class WorkerDataArray;
class PiaParams;
class DateMoyr;
class WageInd;
class ReindWid;
class OldStart;
class SpecMin;
class PiaTable;
class FrozMin;
class ChildCareCalc;
class TransGuar;
class DibGuar;
class WageIndNonFreeze;

/// <summary>Parent of all classes that manage the functions required for
/// calculation of a Social Security benefit.</summary>
///
/// <remarks>This class is abstract; child classes are <see cref="PiaCalAny"/>,
/// <see cref="PiaCalLC"/>, and <see cref="PiaCalPL"/>.</remarks>
///
/// <seealso cref="PiaCalAny"/>
/// <seealso cref="PiaCalLC"/>
/// <seealso cref="PiaCalPL"/>
class PiaCal
{
public:
  /// <summary>Worker's personal data.</summary>
  WorkerDataGeneral& workerData;
  /// <summary>Worker's pia calculation data.</summary>
  PiaData& piaData;
  /// <summary>Pia calculation parameters.</summary>
  const PiaParams& piaParams;
  /// <summary>Pia calculation methods that apply to all benies in the family.
  /// </summary>
  std::vector<PiaMethod *> piaMethod;
  /// <summary>Pia calculation methods that apply only to widow(er)s.
  /// </summary>
  std::vector<ReindWid *> widowMethod;
  /// <summary>Pointer to wage-indexed method.</summary>
  WageInd* wageInd;
  /// <summary>Pointer to old-start method.</summary>
  OldStart* oldStart;
  /// <summary>Pointer to special minimum method.</summary>
  SpecMin* specMin;
  /// <summary>Pointer to pia table method.</summary>
  PiaTable* piaTable;
  /// <summary>Pointer to frozen minimum method.</summary>
  FrozMin* frozMin;
  /// <summary>Pointer to child care calculation.</summary>
  ChildCareCalc* childCareCalc;
  /// <summary>Pointer to transitional guarantee.</summary>
  TransGuar* transGuar;
  /// <summary>Pointer to disability guarantee.</summary>
  DibGuar* dibGuar;
  /// <summary>Pointer to wage-indexed non-freeze method.</summary>
  WageIndNonFreeze* wageIndNonFreeze;
  /// <summary>Pointer to method with high pia.</summary>
  PiaMethod* highPiaMethod;
private:
  /// <summary>Type of benefit, set even if Statement case.</summary>
  ///
  /// <remarks>It has the following possible values:
  /// <table>
  ///   1   old-age.
  ///   2   survivor.
  ///   3   disability.
  /// </table></remarks>
  WorkerDataGeneral::ben_type ioasdi;
  /// <summary>True if old Pebs assumptions (with 1-percent real wage gain).
  /// </summary>
  bool pebsAssumptions;
public:
  PiaCal( WorkerDataGeneral& newWorkerData, PiaData& newPiaData,
    const PiaParams& newPiaParams );
  virtual ~PiaCal();
  void ageEntCheck() const;
  /// <summary>Applies maximum family benefit (already calculated) to
  /// family's benefits in a life case.</summary>
  ///
  /// <remarks>Calls the static version with 6 arguments, passing saved values
  /// for the other arguments. The worker's pia is subtracted from the mfb
  /// before applying the mfb to the auxiliaries' total benefits.</remarks>
  ///
  /// <param name="secondaryArray">Benefits for all family members.</param>
  /// <param name="famSize">Number of family members.</param>
  /// <param name="year">Year of rounding.</param>
  void applyLifeMfb( SecondaryArray& secondaryArray, int famSize,
    int year ) const
  { applyLifeMfb(secondaryArray, famSize, year, piaData.highMfb.get(),
    piaData.highPia.get(), secondaryArray.getTotalFullBenefitMax()); }
  /// <summary>Applies maximum family benefit (already calculated) to
  /// family's benefits in a survivor case.</summary>
  ///
  /// <remarks>Calls the static version with 5 arguments, passing saved values
  /// for the other arguments.</remarks>
  ///
  /// <param name="secondaryArray">Benefits for all family members.</param>
  /// <param name="famSize">Number of family members.</param>
  /// <param name="year">Year of rounding.</param>
  void applySurvivorMfb( SecondaryArray& secondaryArray, int famSize,
    int year ) const
  { applySurvivorMfb(secondaryArray, famSize, year, piaData.highMfb.get(),
    secondaryArray.getTotalFullBenefitMax()); }
  void ardriAuxCal( WorkerDataGeneral *widowData, PiaData *widowPiaData,
    Secondary *secondaryData, const DateMoyr& entDate ) const;
  void ardriCal() const;
  virtual void calculate2( const DateMoyr& entDate );
  virtual void dataCheck( const DateMoyr& entDate );
  void dataCheckAux( WorkerDataArray& widowDataArray,
    PiaDataArray& widowArray, SecondaryArray& secondaryArray );
  virtual void earnProjection() const;
  DateMoyr fullInsDateCal() const;
  /// <summary>Returns type of benefit.</summary>
  ///
  /// <returns>Type of benefit.</returns>
  WorkerDataGeneral::ben_type getIoasdi() const { return(ioasdi); }
  InsCode::InsCodeType insCal( const boost::gregorian::date& dateModyyr,
    int isWasPrimary );
  InsCode::InsCodeType insNonFreezeCal( const boost::gregorian::date& dateModyyr,
    int isWasPrimary );
  /// <summary>Returns Statement assumptions indicator.</summary>
  ///
  /// <returns>True if old Statement assumptions (with 1-percent real wage
  /// gain).</returns>
  bool isPebsAssumptions() const { return pebsAssumptions; }
  void monthsArAgedSpouseCal( PiaData *widowData,
    Secondary *secondaryData ) const;
  void monthsArCal() const;
  void monthsArDICal() const;
  void monthsArDisWidCal( PiaData *widowData, Secondary *secondaryData,
    const DateMoyr& entDate ) const;
  void monthsArWidCal( PiaData *widowData, Secondary *secondaryData ) const;
  void monthsDriCal() const;
  void monthsYngLifeCal( Secondary *secondaryData ) const;
  void monthsYngSurvCal( Secondary *secondaryData ) const;
  virtual void nCal( CompPeriod& compPeriod, const DateMoyr& entDate );
  int nDropCal( const DateMoyr& entDate );
  virtual void nNonFreezeCal( CompPeriod& compPeriod, const DateMoyr& entDate );
  int nDropNonFreezeCal( const DateMoyr& entDate );
  virtual int nelapsed2Cal( const DateMoyr& entDate );
  void nelapsedCal( CompPeriod& compPeriod, const DateMoyr& entDate );
  void nelapsedCal( CompPeriod& compPeriod, int elapsed2 );
  virtual int nelapsed2NonFreezeCal( const DateMoyr& entDate );
  void nelapsedNonFreezeCal( CompPeriod& compPeriod, const DateMoyr& entDate );
  void nelapsedNonFreezeCal( CompPeriod& compPeriod, int elapsed2 );
  /// <summary>Calculates PIA and MFB, not including re-indexed widow(er).
  /// </summary>
  virtual void piaCal( const DateMoyr& ) = 0;
  void piaCal1();
  void piaCal2();
  void piaCal3( PiaDataArray& widowArray, SecondaryArray& secondaryArray );
  void qcCal();
  void reindWidCalAll( const WorkerDataArray& widowDataArray,
    PiaDataArray& widowArray, SecondaryArray& secondaryArray );
  /// <summary>Calculates PIA for one re-indexed widow(er).</summary>
  virtual void reindWidCal( const WorkerDataGeneral&, const PiaData&,
    Secondary& ) = 0;
  double relEarnPositionCal();
  void releaseMemory();
  void resetAmend90();
  void setAmend90( const DateMoyr& entDate );
  void setArfApp();
  void setEligYearWidow( WorkerDataArray& widowDataArray,
    PiaDataArray& widowArray, SecondaryArray& secondaryArray ) const;
  void setHighMfb();
  void setHighPia();
  /// <summary>Sets the type of benefit.</summary>
  ///
  /// <param name="ioasdit">New type of benefit.</param>
  void setIoasdi( WorkerDataGeneral::ben_type ioasdit ) { ioasdi = ioasdit; }
  /// <summary>Sets Statement assumptions indicator.</summary>
  ///
  /// <param name="newPebsAssumptions">True if old Statement assumptions
  /// (with 1-percent real wage gain).</param>
  void setPebsAssumptions( bool newPebsAssumptions )
  { pebsAssumptions = newPebsAssumptions; }
  void setPifc();
  void setSupportPia();
  /// <summary>Applies maximum family benefit to family's benefits in a life
  /// case.</summary>
  ///
  /// <remarks>The worker's pia is subtracted from the mfb before applying the
  /// mfb to the auxiliaries' total benefits.</remarks>
  ///
  /// <param name="secondaryArray">Benefits for all family members.</param>
  /// <param name="famSize">Number of family members.</param>
  /// <param name="year">Year of rounding.</param>
  /// <param name="mfb">Maximum family benefit.</param>
  /// <param name="pia">Primary insurance amount.</param>
  /// <param name="totalBenefit">Total family benefit before applying
  /// maximum.</param>
  static void applyLifeMfb( SecondaryArray& secondaryArray, int famSize,
    int year, double mfb, double pia, double totalBenefit )
  { applyMfb(secondaryArray, famSize, (mfb - pia) / totalBenefit, year); }
  static void applyMfb( SecondaryArray& secondaryArray, int famSize,
    double ratio, int year );
  /// <summary>Applies maximum family benefit to family's benefits in a
  /// survivor case.</summary>
  ///
  /// <param name="secondaryArray">Benefits for all family members.</param>
  /// <param name="famSize">Number of family members.</param>
  /// <param name="year">Year of rounding.</param>
  /// <param name="mfb">Maximum family benefit.</param>
  /// <param name="totalBenefit">Total family benefit before applying
  /// maximum.</param>
  static void applySurvivorMfb( SecondaryArray& secondaryArray,
    int famSize, int year, double mfb, double totalBenefit )
  { applyMfb(secondaryArray, famSize, mfb / totalBenefit, year); }
  static InsCode::InsCodeType deadIns( int iswasPrimary );
  static bool deemedInsCal( PiaData& piaData, const QtrYear& qtrYear );
  static int didropCal( const WorkerDataGeneral& workerData, int elapsed1,
    int elapsed2, FreezeYears& frzyrst );
  static int didropNonFreezeCal( const WorkerDataGeneral& workerData, 
    int elapsed1, int elapsed2 );
  static DisInsCode::DisInsCodeType disInsCal(
    const WorkerDataGeneral& workerData, PiaData& piaData,
    const DateMoyr& dateMoyr, int iswasPrimary );
  static DisInsCode::DisInsCodeType disInsNonFreezeCal(
    const WorkerDataGeneral& workerData, PiaData& piaData,
    const DateMoyr& dateMoyr, int iswasPrimary );
  static void eligYearCal( const WorkerDataGeneral& workerData,
    PiaData& piaData, WorkerDataGeneral::ben_type ioasdi );
  static DateMoyr eligYearCal1( const WorkerDataGeneral& workerData,
    PiaData& piaData, WorkerDataGeneral::ben_type ioasdi, int jind7 );
  static int eligYearCal2( const WorkerDataGeneral& workerData,
    PiaData& piaData, WorkerDataGeneral::ben_type ioasdi, int eligYear );
  static int eligYearCal3( const WorkerDataGeneral& workerData,
    PiaData& piaData, WorkerDataGeneral::ben_type ioasdi, int eligYear );
  static int eligYearWidowCal( char majorBic, int byear,
    boost::gregorian::date onsetDate );
  static InsCode::InsCodeType fins1Cal( const WorkerDataGeneral& workerData,
    PiaData& piaData, const QtrYear& qtrYear, int iswasPrimary );
  static InsCode::InsCodeType finsNonFreeze1Cal( const WorkerDataGeneral& workerData,
    PiaData& piaData, const QtrYear& qtrYear, int iswasPrimary );
  static char fins2Cal( const WorkerDataGeneral& workerData,
    const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi );
  static char finsNonFreeze2Cal( const WorkerDataGeneral& workerData,
    const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi );
  static bool getPubpenEnable( const WorkerDataGeneral& workerData );
  static InsCode::InsCodeType insCal( const WorkerDataGeneral& workerData,
    PiaData& piaData, WorkerDataGeneral::ben_type ioasdi,
    const QtrYear& qtrYear, int isWasPrimary );
  static InsCode::InsCodeType insNonFreezeCal( const WorkerDataGeneral& workerData,
    PiaData& piaData, WorkerDataGeneral::ben_type ioasdi,
    const QtrYear& qtrYear, int isWasPrimary );
  static int qc3750simpCal( double earn3750 );
  static int qcCurrentCal( const PiaData& piaData, const QtrYear& qtrYear );
  static int qcCurrentNonFreezeCal( const PiaData& piaData, const QtrYear& qtrYear );
  static int qcReqCal ( const WorkerDataGeneral& workerData,
    PiaData& piaData, WorkerDataGeneral::ben_type ioasdi, int year );
  static int qcReqNonFreezeCal ( const WorkerDataGeneral& workerData,
    PiaData& piaData, WorkerDataGeneral::ben_type ioasdi, int year );
  static int qcReqPermCal( const WorkerDataGeneral& workerData,
    PiaData& piaData, WorkerDataGeneral::ben_type ioasdi );
  static int qcReqPermNonFreezeCal( const WorkerDataGeneral& workerData,
    PiaData& piaData, WorkerDataGeneral::ben_type ioasdi );
  /// <summary>Checks disabled widow's date of disability.</summary>
  ///
  /// <remarks>Calls the static version with 3 arguments.</remarks>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_JONSET5"/> if disability is before birth; of type
  /// <see cref="PIA_IDS_JONSET6"/> if disability is after entitlement.
  /// </exception>
  ///
  /// <param name="widowData">Family member's information.</param>
  /// <param name="secondaryData">Family member's benefit.</param>
  static void widowCheck( WorkerDataGeneral& widowData,
    Secondary& secondaryData )
  { widowCheck(widowData.disabPeriod[0].getOnsetDate(),
    widowData.getBirthDate(), secondaryData.entDate); }
  /// <summary>Checks disabled widow's date of disability.</summary>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_JONSET5"/> if disability is before birth; of type
  /// <see cref="PIA_IDS_JONSET6"/> if disability is after entitlement.
  /// </exception>
  ///
  /// <param name="jonsett">The date of disability onset.</param>
  /// <param name="jbirtht">The date of birth.</param>
  /// <param name="ientt">The date of entitlement.</param>
  static void widowCheck( const boost::gregorian::date& jonsett,
    const boost::gregorian::date& jbirtht, const DateMoyr& ientt )
  { if (jonsett < jbirtht) throw PiaException(PIA_IDS_JONSET5);
    if (ientt < jonsett) throw PiaException(PIA_IDS_JONSET6); }
private:
  PiaCal& operator=( PiaCal& newPiaCal );
};
