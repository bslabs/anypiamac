// Functions for the <see cref="PebsEarnings"/> class to manage Social
// Security Statement earnings page.

// $Id: PebsEarnings.cpp 1.21.1.2 2012/03/08 10:56:05EST 277133 Development  $

#include <iomanip>
#include <cmath>  // for floor
#include "PebsEarnings.h"
#include "UserAssumptions.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PebsOut.h"
#include "WageBase.h"
#include "piaparms.h"
#include "piadata.h"
#include "TaxData.h"

using namespace std;

/// <summary>Initializes PebsEarnings.</summary>
///
/// <param name="newWorkerData">Worker basic data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newTaxData">Taxes paid.</param>
PebsEarnings::PebsEarnings( const WorkerData& newWorkerData,
const PiaData& newPiaData, const PiaParams& newPiaParams,
const TaxData& newTaxData ) : PebsPageOut(),
workerData(newWorkerData), piaData(newPiaData), piaParams(newPiaParams),
taxData(newTaxData)
{ }

/// <summary>Destructor.</summary>
PebsEarnings::~PebsEarnings()
{ }

/// <summary>Prepares page of earnings for printing.</summary>
void PebsEarnings::prepareStrings()
{
  //try {
  //  outputString.clear();
  //  prepareHeader(workerData);
  //  outputString.push_back("      Your Social Security Earnings");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "                   SOCIAL SECURITY                 MEDICARE");
  //  outputString.push_back(
  //    "          ------------------------------  ------------------------------");
  //  outputString.push_back(
  //    "            Maximum      Your  Estimated    Maximum      Your  Estimated");
  //  outputString.push_back(
  //    "            Taxable  Reported  Taxes You    Taxable  Reported  Taxes You");
  //  outputString.push_back(
  //    "YEARS      Earnings  Earnings       Paid   Earnings  Earnings       Paid");
  //  outputString.push_back(hyphens);
  //  strm.str("");
  //  if (piaData.getEarnTotal50(PiaData::EARN_NO_TOTALIZATION) > 0.005) {
  //    strm.precision(0);
  //    strm << "1937-50      $" << setw(5) << piaParams.getBaseOasdi(YEAR50)
  //      << "   $" << setw(6)
  //      << floor(piaData.getEarnTotal50(PiaData::EARN_NO_TOTALIZATION))
  //      << "     $" << setw(5)
  //      << taxData.getTaxesTotal(TaxRates::OASDITF, TaxData::YEARS1937_50);
  //    outputString.push_back(strm.str());
  //  }
  //  int startyr = max(YEAR51,piaData.getIbeginAll());
  //  for (int yr = startyr; yr < UserAssumptions::getIstart() + 1; yr++) {
  //    strm.str("");
  //    strm.precision(0);
  //    strm << PunctFacet::getYear(yr) << setw(15)
  //      << piaParams.getBaseOasdi(yr);
  //    if (yr < UserAssumptions::getIstart() - 1) {
  //      strm << setw(10) << piaData.earnOasdiLimited[yr]
  //        << setw(11) << taxData.taxesOasdi[yr];
  //      // print HI earnings and taxes
  //      if (yr > 1965) {
  //        if (yr == 1966) {
  //          strm << "    $" << setw(6) << piaParams.getBaseHi(yr) << "   $"
  //            << setw(6) << piaData.earnHiLimited[yr] << "     $"
  //            << setw(5) << taxData.taxesHi[yr];
  //        }
  //        else {
  //          if (piaParams.getBaseHi(yr) > MAXEARN - 1) {
  //            strm << "   No limit";
  //          }
  //          else {
  //            strm << setw(11) << piaParams.getBaseHi(yr);
  //          }
  //          strm << setw(10) << piaData.earnHiLimited[yr]
  //            << setw(11) << taxData.taxesHi[yr];
  //        }
  //      }
  //    }
  //    else {
  //      if (yr == UserAssumptions::getIstart() - 1) {
  //        strm << "            Not Yet Recorded";
  //      }
  //    }
  //    outputString.push_back(strm.str());
  //  }
  //  outputString.push_back(hyphens);
  //} catch (PiaException&) {
  //  throw PiaException("Error in PebsEarnings::prepareStrings");
  //}
}
