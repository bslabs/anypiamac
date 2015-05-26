// Functions for the <see cref="PiaWrite"/> class to write out worker data.

// $Id: piawrite.cpp 1.77 2011/08/08 08:45:15EDT 044579 Development  $

#include <fstream>
#include <iomanip>
#include "piawrite.h"
#include "WorkerDataArray.h"
#include "PiadataArray.h"
#include "UserAssumptions.h"
#include "SecondaryArray.h"
#include "awinc.h"
#include "PiaException.h"
#include "AssumptionType.h"
#include "oactcnst.h"
#include "DateFormatter.h"

using namespace std;

// <summary>Number of columns at beginning of each line reserved for
//   line number.</summary>
int PiaWrite::width = 2;
// <summary>Number of columns for earnings and bases.</summary>
int PiaWrite::earnWidth = 11;
// <summary>Number of decimal places for earnings and bases.</summary>
int PiaWrite::numDecimals = 2;

/// <summary>Initializes references to data.</summary>
///
/// <param name="newWorkerData">Worker's calculation data.</param>
/// <param name="newWidowDataArray">Widow and other family members' basic data.
/// </param>
/// <param name="newWidowArray">Widow and other family members' calculation
///  data.</param>
/// <param name="newUserAssumptions">User-specified assumptions.</param>
/// <param name="newSecondaryArray">Array of <see cref="Secondary"/> benefit
/// information.</param>
PiaWrite::PiaWrite( const WorkerData& newWorkerData,
const WorkerDataArray& newWidowDataArray, const PiaDataArray& newWidowArray,
const UserAssumptions& newUserAssumptions,
const SecondaryArray& newSecondaryArray ) :
workerData(newWorkerData), widowDataArray(newWidowDataArray),
widowArray(newWidowArray), userAssumptions(newUserAssumptions),
secondaryArray(newSecondaryArray)
{ }

/// <summary>Saves a case to disk.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::write( std::ostream& out )
{
  writeSsn(out);
  writeDeath(out);
  writeTob(out);
  writeBenDate(out);
  writePebes(out);
  writeYears(out);
  writeBack(out);
  writeFwrd(out);
  writeDisab1(out);
  writeDisab2(out);
  writeMsdates(out);
  writePubpen(out);
  writeTotalize(out);
  writeBlind(out);
  writeDeemed(out);
  writeNhname(out);
  writeNhaddr(out);
  writeEarnType(out);
  writeTaxType(out);
  writeEarnOasdi(out);
  writeEarnHi(out);
  writePubpenReservist(out);
  writeOabEnt(out);
  writeAssump(out);
  writeBi(out);
  writeAw(out);
  writeBases(out);
  writeFamilyMembers(out);
  writeRailRoad(out);
  writeQc1(out);
  writeQc2(out);
  writeChildCareYears(out);
}

/// <summary>Writes Social Security number, sex, and date of birth.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeSsn( std::ostream& out ) const
{
  out << setfill('0') << setw(width) << 1 << workerData.ssn.toString("n")
    << setfill(' ') << static_cast<int>(workerData.getSex())
    << DateFormatter::toString(workerData.getBirthDate(), "n") << endl;
}

/// <summary>Writes date of death.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeDeath( std::ostream& out ) const
{
  if (!workerData.getDeathDate().is_not_a_date()) {
    out << setfill('0') << setw(width) << 2 << setfill(' ')
      << DateFormatter::toString(workerData.getDeathDate(), "n") << endl;
  }
}

/// <summary>Writes type of benefit and date of entitlement.</summary>
///
/// <remarks>Only written if there is a benefit.</remarks>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeTob( std::ostream& out ) const
{
  if (static_cast<int>(workerData.getJoasdi()) > 0) {
    out << setfill('0') << setw(width) << 3 << setfill(' ')
      << static_cast<int>(workerData.getJoasdi())
      << DateFormatter::toString(workerData.getEntDate(), "n") << endl;
  }
}

/// <summary>Writes date of benefit.</summary>
///
/// <remarks>Only written if this is a recalculation, or a survivor benefit.
/// </remarks>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeBenDate( std::ostream& out ) const
{
  if (workerData.getRecalc() ||
    (workerData.getJoasdi() == WorkerData::SURVIVOR)) {
    out << setfill('0') << setw(width) << 4 << setfill(' ')
      << DateFormatter::toString(workerData.getBenefitDate(), "n") << endl;
   }
}

/// <summary>Writes pebes information.</summary>
///
/// <remarks>This version does nothing.</remarks>
void PiaWrite::writePebes( std::ostream& ) const
{ }

/// <summary>Writes years of earnings.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeYears( std::ostream& out ) const
{
  // do not try to save if no earnings have been set
  if (workerData.getIndearn()) {
    try {
      workerData.ibeginCheck();
    } catch (PiaException&) {
      return;
    }
  }
  else {
    return;
  }
  out << setfill('0') << setw(width) << 6 << setfill(' ')
    << setw(4) << workerData.getIbegin()
    << setw(4) << workerData.getIend() << endl;
}

/// <summary>Writes backward projection amounts.</summary>
///
/// <remarks>This version does nothing.</remarks>
void PiaWrite::writeBack( std::ostream& ) const
{ }

/// <summary>Writes forward projection amounts.</summary>
///
/// <remarks>This version does nothing.</remarks>
void PiaWrite::writeFwrd( std::ostream& ) const
{ }

/// <summary>Writes disability information for most recent period.</summary>
///
/// <remarks>Does not print anything unless there is a period of disability.
/// Does not print the date of cessation for a current period of disability
/// (the type of benefit is
/// <see cref="WorkerDataGeneral::ben_type::DISABILITY"/>).</remarks>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeDisab1( std::ostream& out ) const
{
  if (workerData.getValdi() > 0) {
    string format((workerData.getJoasdi() == WorkerData::DISABILITY) ?
      "c" : "");
    out << setfill('0') << setw(width) << 9 << setfill(' ')
      << workerData.disabPeriod[0].toString(format) << endl;
  }
}

/// <summary>Writes disability information for second most recent period.
/// </summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeDisab2( std::ostream& out ) const
{
  if (workerData.getValdi() > 1) {
    out << setfill('0') << setw(width) << 10 << setfill(' ')
      << workerData.disabPeriod[1].toString() << endl;
  }
}

/// <summary>Writes information for all family members.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeFamilyMembers( std::ostream& out ) const
{
  for (int i = 0; i < widowArray.getFamSize(); i++) {
    writeFamilyMember(out, i);
  }
}

/// <summary>Writes information for one family member.</summary>
///
/// <param name="out">Output stream.</param>
/// <param name="i">Number of family member (0 to 14).</param>
void PiaWrite::writeFamilyMember( std::ostream& out, int i ) const
{
  Secondary * secondaryptr = secondaryArray.secondary[i];
  WorkerData * widowDataptr = widowDataArray.workerData[i];
  out << setfill('0') << setw(width) << 69 + i << setfill(' ')
    << secondaryptr->bic.toString();
  // write date of birth
  out << DateFormatter::toString(widowDataptr->getBirthDate(), "n");
  // write date of entitlement
  out << DateFormatter::toString(secondaryptr->entDate, "n");
  if (secondaryptr->bic.getMajorBic() == 'W') {
    out << DateFormatter::toString(
      widowDataptr->disabPeriod[0].getOnsetDate(), "n");
  }
  out << endl;
}

/// <summary>Writes noncovered pension.</summary>
///
/// <remarks>Writes the non-covered pension amount if it is greater
/// than zero. Also writes the pension start date if it exists.</remarks>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writePubpen( std::ostream& out ) const
{
  if (workerData.getPubpen() > 0.0f) {
    out << setfill('0') << setw(width) << 12 << setfill(' ')
      << setw(10) << setprecision(2) << workerData.getPubpen();
    if (workerData.getPubpenDate().getYear() > 0) {
      out << DateFormatter::toString(workerData.getPubpenDate(), "n");
    }
    out << endl;
  }
}

/// <summary>Writes noncovered pension after military reservist pension
///   removal.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writePubpenReservist( std::ostream& out ) const
{
  if (workerData.getReservist()) {
    out << setfill('0') << setw(width) << 38 << setfill(' ')
      << setprecision(2) << setw(10) << workerData.getPubpenReservist()
      << endl;
  }
}

/// <summary>Writes totalization indicator.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeTotalize( std::ostream& out ) const
{
  if (workerData.getTotalize()) {
    out << setfill('0') << setw(width) << 13 << setfill(' ')
      << workerData.getTotalize() << endl;
  }
}

/// <summary>Writes blind indicator.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeBlind( std::ostream& out ) const
{
  if (workerData.getBlindind()) {
    out << setfill('0') << setw(width) << 14 << setfill(' ')
      << workerData.getBlindind() << endl;
  }
}

/// <summary>Writes deemed insured indicator.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeDeemed( std::ostream& out ) const
{
  if (workerData.getDeemedind()) {
    out << setfill('0') << setw(width) << 15 << setfill(' ')
      << workerData.getDeemedind() << endl;
  }
}

/// <summary>Writes name of worker.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeNhname( std::ostream& out ) const
{
  if (workerData.getNhname().length() > 0) {
    out << setfill('0') << setw(width) << 16 << setfill(' ')
      << workerData.getNhname() << endl;
  }
}

/// <summary>Writes address of worker.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeNhaddr( std::ostream& out ) const
{
  for (int i1 = 0; i1 < 3; i1++) {
    if (workerData.getNhaddr(i1).length() > 0) {
      out << setfill('0') << setw(width) << (17 + i1) << setfill(' ')
        << workerData.getNhaddr(i1) << endl;
    }
  }
}

/// <summary>Writes type of earnings for all years.</summary>
///
/// <remarks>This version does nothing.</remarks>
void PiaWrite::writeEarnType( std::ostream& ) const
{ }

/// <summary>Writes type of taxes for all years.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeTaxType( std::ostream& out ) const
{
  // do not try to save if no earnings have been set
  if (workerData.getIndearn()) {
    try {
      workerData.ibeginCheck();
    } catch (PiaException&) {
      return;
    }
  }
  else {
    return;
  }
  if (workerData.anySelfEmp()) {
    out << setfill('0') << setw(width) << 21 << setfill(' ');
    for (int yr = workerData.getIbegin(); yr <= workerData.getIend(); yr++) {
      out << static_cast<int>(workerData.getTaxType(yr));
    }
    out << endl;
  }
}

/// <summary>Writes OASDI-covered earnings for all years, using unprojected
/// earnings array.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_WRITEEARN"/> if first year of
/// earnings is out of range (only in debug mode).</exception>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeEarnOasdi( std::ostream& out ) const
{
  // do not try to save if no earnings have been set
  if (workerData.getIndearn()) {
    try {
      workerData.ibeginCheck();
    } catch (PiaException&) {
      return;
    }
  }
  else {
    return;
  }
#if !defined(NDEBUG)
  if (workerData.getIbegin() < YEAR37)
    throw PiaException(PIA_IDS_WRITEEARN);
#endif
  out.precision(numDecimals);
  const int numrows =
    ((workerData.getIend() - workerData.getIbegin()) / 10) + 1;
  for (int row = 0; row < numrows; row++) {
    out << setfill('0') << setw(width) << (22 + row) << setfill(' ');
    const int firstyear = workerData.getIbegin() + 10 * row;
    const int lastyear = min(workerData.getIend(), firstyear + 9);
    for (int yr = firstyear; yr <= lastyear; yr++) {
      out << setw(earnWidth) << workerData.getEarnOasdi(yr);
    }
    out << endl;
  }
}

/// <summary>Writes summary quarters of coverage.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeQc1( std::ostream& out ) const
{
  if (workerData.getQctottd() > 0) {
    out << setfill('0') << setw(width) << 95 << setfill(' ')
      << setw(3) << workerData.getQctottd()
      << setw(3) << workerData.getQctot51td() << endl;
  }
}

/// <summary>Writes annual quarters of coverage.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeQc2( std::ostream& out ) const
{
  // do not try to save if no earnings have been set
  if (workerData.getIndearn()) {
    try {
      workerData.ibeginCheck();
    } catch (PiaException&) {
      return;
    }
  }
  else {
    return;
  }
  const int firstyear = workerData.getIbegin();
  const int lastyear = workerData.lastQcyr();
  if (workerData.hasQcsByYear() && lastyear >= firstyear) {
    out << setfill('0') << setw(width) << 96 << setfill(' ')
      << workerData.qc.toString(firstyear, lastyear) << endl;
  }
}

/// <summary>Writes Medicare earnings.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeEarnHi( std::ostream& out ) const
{
  // Don't write anything if there are no earnings.
  if (workerData.getIndearn()) {
    try {
      workerData.ibeginCheck();
    } catch (PiaException&) {
      return;
    }
  }
  else {
    return;
  }
  if (workerData.getMqge()) {
    const int firstYear1 = workerData.firstEarnHiYear();
    out.precision(numDecimals);
    const int numrows = ((workerData.getIend() - firstYear1) / 10) + 1;
    for (int row = 0; row < numrows; row++) {
      out << setfill('0') << setw(width) << (30 + row) << setfill(' ');
      const int firstyear2 = firstYear1 + 10 * row;
      const int lastyear = min(workerData.getIend(), firstyear2 + 9);
      for (int yr = firstyear2; yr <= lastyear; yr++) {
        out << setw(earnWidth) << workerData.getEarnHi(yr);
      }
      out << endl;
    }
  }
}

/// <summary>Writes assumption indicators.</summary>
///
/// <remarks>Assumption indicators are written only if this is a case
/// that needs assumptions (a benefit in the future).</remarks>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeAssump( std::ostream& out ) const
{
  if (workerData.need_aw(UserAssumptions::getIstart(), false) ||
    workerData.need_bi(UserAssumptions::getIstart()) ||
    userAssumptions.getIbasch() == BaseChangeType::ADHOC) {
    out << setfill('0') << setw(width) << 40 << setfill(' ')
      << setw(4) << userAssumptions.getIstart()
      << static_cast<int>(userAssumptions.getIaltbi())
      << static_cast<int>(userAssumptions.getIaltaw())
      << userAssumptions.getIbasch() << endl;
  }
}

/// <summary>Writes benefit increase assumptions.</summary>
///
/// <remarks>Benefit increase assumptions are written if they are not one
/// of the stored sets of assumptions.</remarks>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeBi( std::ostream& out ) const
{
  if (userAssumptions.getIaltbi() == AssumptionType::OTHER_ASSUM) {
    out.precision(1);
    const int i3 = static_cast<int>(workerData.getBenefitDate().getYear());
    const int numrows = ((i3 - UserAssumptions::getIstart()) / 20) + 1;
    for (int row = 0; row < numrows; row++) {
      out << setfill('0') << setw(width) << (41 + row) << setfill(' ');
      const int firstyear = UserAssumptions::getIstart() + 20 * row;
      for (int yr = firstyear; yr <= min(i3, firstyear + 19); yr++) {
        out << setw(4) << userAssumptions.biproj[yr];
      }
      out << endl;
    }
    if (userAssumptions.getAnscch() == 'Y') {
      for (int i1 = 0; i1 < Catchup::NUM_CATCHUP_ELIG_YEARS; i1++) {
        out << setfill('0') << setw(width) << (45 + i1) << setfill(' ')
          << userAssumptions.catchup.getCatchupIncreases(i1).toString();
      }
    }
    out << setfill('0') << setw(width) << 55
      << userAssumptions.titleBi << endl;
  }
}

/// <summary>Writes average wage increase assumptions.</summary>
///
/// <remarks>Average wage increase assumptions are written if they
/// are not one of the stored sets of assumptions.</remarks>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeAw( std::ostream& out ) const
{
  if (userAssumptions.getIaltaw() == AssumptionType::OTHER_ASSUM) {
    const int i3 = static_cast<int>(workerData.getBenefitDate().getYear());
    out.precision(2);
    const int numrows = ((i3 - UserAssumptions::getIstart() + 1) / 20) + 1;
    for (int i2 = 0; i2 < numrows; i2++) {
      out << setfill('0') << setw(width) << (56 + i2) << setfill(' ');
      const int firstyear = UserAssumptions::getIstart() - 1 + 20 * i2;
      for (int yr = firstyear; yr <= min(i3, firstyear + 19); yr++) {
        out << setw(6) << userAssumptions.awincproj[yr];
      }
      out << endl;
    }
    out << setfill('0') << setw(width) << 60
      << userAssumptions.titleAw << endl;
  }
}

/// <summary>Writes wage base assumptions.</summary>
///
/// <remarks>Projected wage bases are written if they are ad hoc.</remarks>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeBases( std::ostream& out ) const
{
  if (userAssumptions.getIbasch() == BaseChangeType::ADHOC) {
    out.precision(numDecimals);
    const int i3 = userAssumptions.baseOasdi.getLastYear();
    const int numrows = ((i3 - UserAssumptions::getIstart() - 1) / 20) + 1;
    for (int row = 0; row < numrows; row++) {
      out << setfill('0') << setw(width) << (61 + row) << setfill(' ');
      const int firstyear = UserAssumptions::getIstart() + 1 + 20 * row;
      for (int yr = firstyear; yr <= min(i3, firstyear + 19); yr++) {
        out << setw(earnWidth) << userAssumptions.baseOasdi[yr];
      }
      out << endl;
    }
    for (int row = 0; row < numrows; row++) {
      out << setfill('0') << setw(width) << (65 + row) << setfill(' ');
      const int firstyear = UserAssumptions::getIstart() + 1 + 20 * row;
      for (int yr = firstyear; yr <= min(i3, firstyear + 19); yr++) {
        out << setw(earnWidth) << userAssumptions.base77[yr];
      }
      out << endl;
    }
  }
}

/// <summary>Writes military service dates.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeMsdates( std::ostream& out ) const
{
  if (workerData.milServDatesVec.getMSCount() > 0) {
    out << setfill('0') << setw(width) << 11 << setfill(' ');
    for (unsigned i = 0; i < workerData.milServDatesVec.getMSCount(); i++) {
      out << workerData.milServDatesVec.msdates[i].getDatesString();
    }
    out << endl;
  }
}

/// <summary>Writes oab entitlement prior to most recent dib.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeOabEnt( std::ostream& out ) const
{
  if (workerData.getOabEntDate().getYear() > 0) {
    out << setfill('0') << setw(width) << 39 << setfill(' ')
      << DateFormatter::toString(workerData.getOabEntDate(), "n")
      << DateFormatter::toString(workerData.getOabCessDate(), "n") << endl;
  }
}

/// <summary>Writes annual child care years.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeChildCareYears( std::ostream& out ) const
{
  bool needwrite = false;
  if (workerData.getIndearn()) {
    try {
      workerData.ibeginCheck();
    } catch (PiaException&) {
      return;
    }
  }
  else {
    return;
  }
  for (int yr = workerData.getIbegin(); yr <= workerData.getIend(); yr++) {
    if (workerData.childCareYears.getBit(yr)) {
      needwrite = true;
    }
  }
  if (needwrite) {
    out << setfill('0') << setw(width) << 97 << setfill(' ');
    for (int yr = workerData.getIbegin(); yr <= workerData.getIend(); yr++) {
      out << static_cast<int>(workerData.childCareYears.getBit(yr));
    }
    out << endl;
  }
}

/// <summary>Writes railroad earnings and qcs.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWrite::writeRailRoad( std::ostream& out ) const
{
  // write pre-1951 qcs and earnings, if any
  if (workerData.railRoadData.getEarn3750() > 0.) {
    out << setfill('0') << setw(width) << 84 << setfill(' ')
      << workerData.railRoadData.get3750String(earnWidth, numDecimals) << endl;
  }
  // write post-1950 qcs and earnings, if any
  if (workerData.railRoadData.getLastYear() > 1950) {
    out << setfill('0') << setw(width) << 85 << setfill(' ')
      << workerData.railRoadData.getYearsString() << endl;
    if (workerData.railRoadData.getFirstYear() < 1978) {
      out << setfill('0') << setw(width) << 86 << setfill(' ')
        << workerData.railRoadData.getQcString() << endl;
    }
    // write earnings by decade
    for (int i = 0; i < workerData.railRoadData.getDecades(); i++) {
      out << setfill('0') << setw(width) << (87 + i) << setfill(' ')
        << workerData.railRoadData.getDecadeEarningsString(i, earnWidth,
        numDecimals) << endl;
    }
  }
}
