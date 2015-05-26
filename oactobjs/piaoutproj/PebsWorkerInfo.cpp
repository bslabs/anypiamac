// Functions for the <see cref="PebsWorkerInfo"/> class to manage the Social
// Security Statement worker info page.

// $Id: PebsWorkerInfo.cpp 1.20.1.2 2012/03/08 10:56:08EST 277133 Development  $

#include <iomanip>
#include "PebsWorkerInfo.h"
#include "UserAssumptions.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PebsOut.h"
#include "WageBase.h"
#include "DateFormatter.h"
#include "wrkrdata.h"
#include "EarnProject.h"
#include "piaparms.h"
#include "pebs.h"

using namespace std;

/// <summary>Initializes PebsWorkerInfo.</summary>
///
/// <param name="newWorkerData">Basic worker data.</param>
/// <param name="newPebs">Social Security Statement data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newEarnProject">Projected earnings information.</param>
PebsWorkerInfo::PebsWorkerInfo( const WorkerData& newWorkerData,
const Pebs& newPebs, const PiaParams& newPiaParams,
const EarnProject& newEarnProject ) :
PebsPageOut(), workerData(newWorkerData), pebs(newPebs),
piaParams(newPiaParams), earnProject(newEarnProject)
{ }

/// <summary>Destructor.</summary>
PebsWorkerInfo::~PebsWorkerInfo()
{ }

/// <summary>Prepares worker information for printing.</summary>
void PebsWorkerInfo::prepareStrings()
{
  //try {
  //  outputString.clear();
  //  outputString.push_back("THE FACTS YOU GAVE US");
  //  outputString.push_back("");
  //  strm.str("");
  //  strm << "Your Name " << string(61 - workerData.getNhname().length(), '.')
  //    << " " << workerData.getNhname();
  //  outputString.push_back(strm.str());
  //  strm.str("");
  //  strm << "Your Social Security Number " << string(32, '.') << " "
  //    << workerData.ssn.toString();
  //  outputString.push_back(strm.str());
  //  string temp(DateFormatter::toString(workerData.getBirthDate()));
  //  strm.str("");
  //  strm << "Your Date of Birth " << string(52 - temp.length(), '.') << " "
  //    << temp;
  //  outputString.push_back(strm.str());
  //  // print two years of lag earnings
  //  strm.precision(0);
  //  for (int i1 = 1; i1 >= 0; i1--) {
  //    const int year = UserAssumptions::getIstart() - i1;
  //    strm.str("");
  //    strm << PunctFacet::getYear(year) << " Earnings ";
  //    const double earnt = earnProject.earnpebs[year];
  //    if (earnt < 0.01) {
  //      strm << string(53, '.') << " NONE";
  //    }
  //    else if (earnt >= piaParams.getBaseOasdi(year)) {
  //      strm << string(45, '.') << " OVER $" << setw(6)
  //        << piaParams.getBaseOasdi(year);
  //    }
  //    else {
  //      strm << string(50, '.') << " $" << setw(6) << earnt;
  //    }
  //    outputString.push_back(strm.str());
  //  }
  //  // print future earnings
  //  const int year = UserAssumptions::getIstart() + 1;
  //  strm.str("");
  //  strm << "Your Estimated Future Yearly Earnings ";
  //  const double earnt = earnProject.earnpebs[year];
  //  if (earnt < 0.01) {
  //    strm << string(29, '.') << " NONE";
  //  }
  //  else if (earnt >= piaParams.getBaseOasdi(year)) {
  //    strm << string(21, '.') << " OVER $" << setw(6)
  //      << piaParams.getBaseOasdi(year);
  //  }
  //  else {
  //    strm << string(26, '.') << " $" << setw(6) << earnt;
  //  }
  //  outputString.push_back(strm.str());
  //  strm.str("");
  //  strm << "The Age You Plan to Retire ";
  //  if (pebs.getAgePlan() > 0) {
  //    strm << string(42, '.') << " " << pebs.getAgePlan2();
  //  }
  //  else {
  //    strm << string(32, '.') << " NOT PROVIDED";
  //  }
  //  outputString.push_back(strm.str());
  //  strm.str("");
  //  strm << "Your Monthly Noncovered Pension ";
  //  if (workerData.getPubpen() < 0.01f) {
  //    strm << string(35, '.') << " NONE";
  //  }
  //  else {
  //    strm << string(32, '.') << " $" << setw(6) << workerData.getPubpen();
  //  }
  //  outputString.push_back(strm.str());
  //  if (workerData.getTotalize()) {
  //    outputString.push_back(
  //      "Benefits Estimated Using Totalization Provision");
  //  }
  //  strm.str("");
  //  DateMoyr datetemp(pebs.getMonthnow(), UserAssumptions::getIstart());
  //  strm << "Processing month "
  //    << string(54 - DateFormatter::toString(datetemp).length(), '.')
  //    << " " << DateFormatter::toString(datetemp);
  //  outputString.push_back(strm.str());
  //  outputString.push_back("");
  //} catch (PiaException&) {
  //  throw PiaException("Error in PebsWorkerInfo::prepareStrings");
  //}
}
