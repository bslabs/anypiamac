// Functions for the <see cref="PiaReadAny"/> class to read worker
// data for Anypia.

// $Id: piareadAny.cpp 1.11 2011/08/08 08:45:11EDT 044579 Development  $

#include <stdexcept>
#include <fstream>
#include <iomanip>
#include "piareadAny.h"
#include "WorkerDataArray.h"
#include "PiadataArray.h"
#include "UserAssumptions.h"
#include "SecondaryArray.h"
#include "EarnProject.h"
#include "pebs.h"
#include "awinc.h"
#include "wbgenrl.h"
#include "oactcnst.h"
#include "PiaException.h"
#include "Resource.h"
#include "AssumptionType.h"

using namespace std;

/// <summary>Initializes references to data.</summary>
///
/// <param name="newWorkerData">Worker's calculation data.</param>
/// <param name="newWidowDataArray">Widow and other family members' basic
/// data.</param>
/// <param name="newWidowArray">Widow and other family members' calculation
///  data.</param>
/// <param name="newUserAssumptions">User-specified assumptions.</param>
/// <param name="newSecondaryArray">Array of <see cref="Secondary"/> benefit
/// information.</param>
/// <param name="newEarnProject">Projected earnings.</param>
/// <param name="newPebs">Statement information.</param>
PiaReadAny::PiaReadAny( WorkerData& newWorkerData,
WorkerDataArray& newWidowDataArray, PiaDataArray& newWidowArray,
UserAssumptions& newUserAssumptions, SecondaryArray& newSecondaryArray,
EarnProject& newEarnProject, Pebs& newPebs ) :
PiaRead(newWorkerData, newWidowDataArray, newWidowArray, newUserAssumptions,
newSecondaryArray), earnProject(newEarnProject), pebs(newPebs)
{ }

/// <summary>Sets data for Social Security Statement.</summary>
///
/// <remarks>Calls <see cref="EarnProject::setPebsData"/>, then
/// <see cref="PiaRead::setPebsData"/>.</remarks>
void PiaReadAny::setPebsData()
{
  earnProject.setPebsData(UserAssumptions::getIstart(),
    workerData.getIbegin());
  workerData.setPebsData();
}

/// <summary>Parses Social Security Statement information.</summary>
///
/// <remarks>Parses the month of the statement and the planned age of
///   retirement.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_READERR"/> if unable to parse the line successfully.
/// </exception>
///
/// <param name="line">Line to parse.</param>
void PiaReadAny::parsePebes( const std::string& line )
{
  try {
    pebs.setMonthnow(atoi(line.substr(0, 2).c_str()));
    pebs.setAgePlan(atoi(line.substr(2, 2).c_str()));
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Sets the years in the <see cref="EarnProject"/> class.</summary>
///
/// <remarks>Sets the first and last years, then checks them by calling
/// <see cref="EarnProject::yearsCheck"/>.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_END1"/> if first year of earnings in forward projection
/// period is before last year of earnings in backward projection period; of
/// type <see cref="PIA_IDS_END3"/> if first year of earnings in forward
/// projection period is after last year of earnings in forward projection
/// period; of type <see cref="PIA_IDS_END4"/> if the total number of years of
/// earnings is greater than the maximum allowed (<see cref="MAXYRS"/>); of
/// type <see cref="PIA_IDS_END1"/> if last year of earnings is before 1937;
/// of type <see cref="PIA_IDS_END2"/> if last year of earnings is after
/// maximum year allowed.</exception>
///
/// <param name="year1">The first year of earnings.</param>
/// <param name="year2">The last year of earnings.</param>
void PiaReadAny::setEarnProjectYears( int year1, int year2 )
{
  earnProject.setFirstYear(year1);
  earnProject.setLastYear(year2);
  earnProject.yearsCheck(workerData.getIbegin(), workerData.getIend());
}

/// <summary>Parses backward projection amounts.</summary>
///
/// <remarks>Parses the projection type, the percentage factor, and the
///   beginning year.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_READERR"/> if unable to parse the line successfully.
/// </exception>
///
/// <param name="line">Line to parse.</param>
void PiaReadAny::parseBack( const std::string& line )
{
  try {
    earnProject.setProjback(atoi(line.substr(0, 1).c_str()));
    earnProject.setPercback(
      static_cast<double>(atof(line.substr(1, 6).c_str())));
    const int ibegint = atoi(line.substr(7, 4).c_str());
    EarnProject::ibegin2Check(ibegint);
    earnProject.setFirstYear(ibegint);
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses forward projection amounts.</summary>
///
/// <remarks>Parses the projection type, the percentage factor, and the
/// ending year.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_READERR"/> if unable to parse the line successfully.
/// </exception>
///
/// <param name="line">Line to parse.</param>
void PiaReadAny::parseFwrd( const std::string& line )
{
  try {
    earnProject.setProjfwrd(atoi(line.substr(0, 1).c_str()));
    earnProject.setPercfwrd(
      static_cast<double>(atof(line.substr(1, 6).c_str())));
    const int iendt = atoi(line.substr(7, 4).c_str());
    EarnProject::iendCheck(iendt);
    earnProject.setLastYear(iendt);
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses type of earnings vector.</summary>
///
/// <remarks>Parses the first and last years, and the earnings type for each
/// year in that span.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_READERR"/> if unable to parse the line successfully.
/// </exception>
///
/// <param name="line">Line to parse.</param>
void PiaReadAny::parseEarnType( const std::string& line )
{
  try {
    const int firstYear = earnProject.getFirstYear();
    const int lastYear = earnProject.getLastYear();
    for (int yr = firstYear; yr <= lastYear; yr++) {
      earnProject.setEarntype(yr,
        atoi(line.substr(yr - firstYear, 1).c_str()));
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses OASDI-covered earnings for up to 10 years.</summary>
///
/// <remarks>Includes projected earnings.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_READERR"/> if unable to parse the line successfully.
/// </exception>
///
/// <param name="lineNumber">Number of earnings line (0-7).</param>
/// <param name="line">Line to parse.</param>
void PiaReadAny::parseEarnOasdi( int lineNumber, const std::string& line )
{
  const int firstYear = earnProject.getFirstYear() + 10 * lineNumber;
  const int lastYear = min(firstYear + 9, earnProject.getLastYear());
  try {
    for (int yr = firstYear; yr <= lastYear; yr++) {
      earnProject.setEarnpebs(yr, atof(line.substr(getEarnWidth() *
        (yr - firstYear), getEarnWidth()).c_str()));
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
  if ((int)line.length() < getEarnWidth() * (lastYear - firstYear + 1)) {
    throw PiaException(PIA_IDS_READERR);
  }
}
