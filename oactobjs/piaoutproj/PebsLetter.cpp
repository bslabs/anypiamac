// Functions for the <see cref="PebsLetter"/> class to manage the Social
// Security Statement letter page.

// $Id: PebsLetter.cpp 1.10.1.3 2012/03/12 12:49:10EDT 277133 Development  $

using namespace std;

#include "PebsLetter.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "DateFormatter.h"
#include "wrkrdata.h"
#include "foinfo.h"

/// <summary>Initializes this instance.</summary>
///
/// <param name="newWorkerData">Worker data.</param>
/// <param name="newFieldOfficeInfo">Field office information.</param>
PebsLetter::PebsLetter( const WorkerData& newWorkerData,
const FieldOfficeInfo& newFieldOfficeInfo ) :
PebsPageOut(), workerData(newWorkerData), fieldOfficeInfo(newFieldOfficeInfo)
{ }

/// <summary>Destructor.</summary>
PebsLetter::~PebsLetter()
{ }

/// <summary>Prepares letter for printing.</summary>
void PebsLetter::prepareStrings()
{
  try{
    outputString.clear();
    outputString.push_back("");
    outputString.push_back("");
    outputString.push_back("");
    outputString.push_back(
      "The detailed calculator no longer provides estimates like the one in your save ");
    outputString.push_back(
      "file.  However, you can still produce a retirement benefit estimate using the ");
    outputString.push_back(
      "information in your save file.  If you wish to do this, please change the type ");
    outputString.push_back(
      "of benefit on the worker information form to \"old-age\", then set the ");
    outputString.push_back(
      "entitlement date and forward earnings projection year on the supplemental ");
    outputString.push_back(
      "worker information form.  The entitlement date should be the date you will ");
    outputString.push_back(
      "start retirement benefits and the earnings projection year should be the last ");
    outputString.push_back(
      "year you expect to work.  This will give you an estimate of your retirement ");
    outputString.push_back(
      "benefit. ");
  } catch (PiaException&) {
    throw PiaException("Error in PebsLetter::prepareStrings");
  }
  //try {
  //  outputString.clear();
  //  outputString.push_back("");
  //  outputString.push_back("");
  //  outputString.push_back("");
  //  outputString.push_back("");
  //  for (int i1 = 0; i1 < 4; i1++) {
  //    if (fieldOfficeInfo.getAddress(i1).length() > 0) {
  //      strm.str("");
  //      strm << "                                 "
  //        << fieldOfficeInfo.getAddress(i1);
  //      outputString.push_back(strm.str());
  //    }
  //  }
  //  strm.str("");
  //  strm << "                                 "
  //    << DateFormatter::toString(boost::gregorian::day_clock::local_day());
  //  outputString.push_back(strm.str());
  //  outputString.push_back("");
  //  outputString.push_back(workerData.getNhname());
  //  for (int i1 = 0; i1 < 3; i1++) {
  //    if (workerData.getNhaddr(i1).length() > 0) {
  //      outputString.push_back(workerData.getNhaddr(i1));
  //    }
  //  }
  //  outputString.push_back("");
  //  strm.str("");
  //  strm << "Dear " << workerData.getNhname() << ":";
  //  outputString.push_back(strm.str());
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "  We are pleased to send you the Social Security Statement you ");
  //  outputString.push_back(
  //    "requested.  We hope it will help you to better understand the Social");
  //  outputString.push_back(
  //    "Security program and what it means to you and your family.  The");
  //  outputString.push_back(
  //    "statement shows the estimated amount of benefits, under current law,");
  //  outputString.push_back(
  //    "that you and your family may be eligible for now and in the future.  It");
  //  outputString.push_back(
  //    "also lists the earnings that your employers (or you, if you're self-");
  //  outputString.push_back(
  //    "employed) have reported to Social Security.  We hope you will use this");
  //  outputString.push_back(
  //    "information along with other information you may have about your savings,");
  //  outputString.push_back(
  //    "pensions and investments to help you plan for a secure financial future.");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "  Remember, Social Security can be an important part of your financial");
  //  outputString.push_back(
  //    "future whether you're young or old, male or female, single or married.");
  //  outputString.push_back(
  //    "It can help you when you need it most by paying you benefits when you");
  //  outputString.push_back(
  //    "retire or if you become severely disabled.  Social Security also can");
  //  outputString.push_back("help support your family when you die.");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "  Some people are concerned that Social Security won't be there in the");
  //  outputString.push_back(
  //    "future.  The program has changed in the past to meet the demands of the");
  //  outputString.push_back(
  //    "times, and it must do so again.  Today we are working to resolve long-");
  //  outputString.push_back(
  //    "range financing issues and to ensure that future generations of");
  //  outputString.push_back(
  //    "Americans can count on the same financial protections from Social");
  //  outputString.push_back("Security as past generations did.");
  //  outputString.push_back("");
  //  outputString.push_back("");
  //  outputString.push_back("");
  //  outputString.push_back("");
  //  strm.str("");
  //  strm << "                                 "
  //    << fieldOfficeInfo.getFieldOfficeName();
  //  outputString.push_back(strm.str());
  //  outputString.push_back("");
  //  strm.str("");
  //  strm << "                                 "
  //    << fieldOfficeInfo.getFieldOfficeTitle();
  //  outputString.push_back(strm.str());
  //  outputString.push_back("");
  //  outputString.push_back("");
  //} catch (PiaException&) {
  //  throw PiaException("Error in PebsLetter::prepareStrings");
  //}
}
