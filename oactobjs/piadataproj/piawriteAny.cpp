// Functions for the <see cref="PiaWriteAny"/> class to write out worker
// data for Anypia program.
//
// $Id: piawriteAny.cpp 1.7 2011/08/08 08:45:11EDT 044579 Development  $

#include <fstream>
#include <iomanip>
#include "piawriteAny.h"
#include "WorkerDataArray.h"
#include "PiadataArray.h"
#include "UserAssumptions.h"
#include "SecondaryArray.h"
#include "EarnProject.h"
#include "pebs.h"
#include "awinc.h"
#include "PiaException.h"
#include "AssumptionType.h"

using namespace std;

/// <summary>Initializes references to data.</summary>
///
/// <param name="newWorkerData">Worker's calculation data.</param>
/// <param name="newWidowDataArray">Widow(er) and other family members' basic
/// data.</param>
/// <param name="newWidowArray">Widow(er) and other family members'
/// calculation data.</param>
/// <param name="newUserAssumptions">User-specified assumptions.</param>
/// <param name="newSecondaryArray">Array of <see cref="Secondary"/> benefit
/// information.</param>
/// <param name="newEarnProject">Projected earnings.</param>
/// <param name="newPebs">Statement information.</param>
PiaWriteAny::PiaWriteAny( const WorkerData& newWorkerData,
const WorkerDataArray& newWidowDataArray, const PiaDataArray& newWidowArray,
const UserAssumptions& newUserAssumptions,
const SecondaryArray& newSecondaryArray, const EarnProject& newEarnProject,
const Pebs& newPebs ) :
PiaWrite(newWorkerData, newWidowDataArray, newWidowArray, newUserAssumptions,
newSecondaryArray),
earnProject(newEarnProject), pebs(newPebs)
{ }

/// <summary>Writes pebes information.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWriteAny::writePebes( std::ostream& out ) const
{
  if (workerData.getJoasdi() == WorkerData::PEBS_CALC) {
    out << setfill('0') << setw(getWidth()) << 5 << setfill(' ');
    out << setw(2) << pebs.getMonthnow()
      << setw(2) << pebs.getAgePlan() << endl;
  }
}

/// <summary>Writes backward projection amounts.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWriteAny::writeBack( std::ostream& out ) const
{
  if (earnProject.getProjback() > 0) {
    out.precision(2);
    out << setfill('0') << setw(getWidth()) << 7 << setfill(' ');
    out << earnProject.getProjback()
      << setw(6) << earnProject.getPercback()
      << setw(4) << earnProject.getFirstYear() << endl;
  }
}

/// <summary>Writes forward projection amounts.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWriteAny::writeFwrd( std::ostream& out ) const
{
  if (earnProject.getProjfwrd() > 0) {
    out.precision(2);
    out << setfill('0') << setw(getWidth()) << 8 << setfill(' ');
    out << earnProject.getProjfwrd()
      << setw(6) << earnProject.getPercfwrd()
      << setw(4) << earnProject.getLastYear() << endl;
  }
}

/// <summary>Writes type of earnings for all years.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_EARNTYPE1"/> if year of projection
/// is out of range (only in debug mode).</exception>
///
/// <param name="out">Output stream.</param>
void PiaWriteAny::writeEarnType( std::ostream& out ) const
{
  bool needwrite = false;
  // do not try to save if no earnings have been set
  if (workerData.getIndearn()) {
    try {
      workerData.ibeginCheck();
    } catch (PiaException&) {
      return;
    }
  }
  else
    return;
#if !defined(NDEBUG)
  if (earnProject.getFirstYear() < YEAR37)
    throw PiaException(PIA_IDS_EARNTYPE1);
#endif
  const int firstyear = earnProject.getFirstYear();
  const int lastyear = earnProject.getLastYear();
  for (int yr = firstyear; yr <= lastyear; yr++) {
    if (earnProject.getEarntype()[yr] != EarnProject::ENTERED)
      needwrite = true;
  }
  if (needwrite) {
    out << setfill('0') << setw(getWidth()) << 20 << setfill(' ');
    for (int yr = firstyear; yr <= lastyear; yr++) {
      out << earnProject.getEarntype()[yr];
    }
    out << endl;
  }
}

/// <summary>Writes earnings for all years, using projected earnings array.
/// </summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_WRITEEARN"/> if first year of
/// earnings is out of range (only in debug mode).</exception>
///
/// <param name="out">Output stream.</param>
void PiaWriteAny::writeEarnOasdi( std::ostream& out ) const
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
  if (earnProject.getFirstYear() < YEAR37)
    throw PiaException(PIA_IDS_WRITEEARN);
#endif
  if (earnProject.anyEntered()) {
    out.precision(getNumDecimals());
    const int numrows =
      ((earnProject.getLastYear() - earnProject.getFirstYear()) / 10) + 1;
    for (int row = 0; row < numrows; row++) {
      out << setfill('0') << setw(getWidth()) << (22 + row) << setfill(' ');
      const int firstyear = earnProject.getFirstYear() + 10 * row;
      const int lastyear = min(earnProject.getLastYear(), firstyear + 9);
      for (int yr = firstyear; yr <= lastyear; yr++) {
        out << setw(getEarnWidth()) << earnProject.earnpebs[yr];
      }
      out << endl;
    }
  }
}

/// <summary>Writes assumption indicators.</summary>
///
/// <param name="out">Output stream.</param>
void PiaWriteAny::writeAssump( std::ostream& out ) const
{
  const bool needAw = earnProject.need_aw(UserAssumptions::getIstart(),
    workerData.getIend());
  if (workerData.need_aw(UserAssumptions::getIstart(), needAw) ||
    workerData.need_bi(UserAssumptions::getIstart()) ||
    userAssumptions.getIbasch() == BaseChangeType::ADHOC) {
    out << setfill('0') << setw(getWidth()) << 40 << setfill(' ');
    out << setw(4) << userAssumptions.getIstart()
      << static_cast<int>(userAssumptions.getIaltbi())
      << static_cast<int>(userAssumptions.getIaltaw())
      << userAssumptions.getIbasch() << endl;
  }
}
