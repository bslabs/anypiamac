// Functions for the <see cref="PebsOasdiEst"/> class to manage the Social
// Security Statement oasdi estimates info page.

// $Id: PebsOasdiEst.cpp 1.12.1.2 2012/03/08 10:56:07EST 277133 Development  $

#include <iomanip>
#include <utility>  // for rel_ops
#include "PebsOasdiEst.h"
#include "UserAssumptions.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PebsOut.h"
#include "pebs.h"

using namespace std;
#if !defined(__SGI_STL_INTERNAL_RELOPS)
using namespace std::rel_ops;
#endif

/// <summary>Initializes PebsOasdiEst.</summary>
///
/// <param name="newWorkerData">Worker data.</param>
/// <param name="newPebs">Social Security Statement information.</param>
PebsOasdiEst::PebsOasdiEst( const WorkerData& newWorkerData,
const Pebs& newPebs ) : PebsPageOut(),
workerData(newWorkerData), pebs(newPebs)
{ }

/// <summary>Destructor.</summary>
PebsOasdiEst::~PebsOasdiEst()
{ }

/// <summary>Prepares OASI estimates for printing.</summary>
void PebsOasdiEst::prepareStrings()
{
  //try {
  //  outputString.clear();
  //  prepareHeader(workerData);
  //  outputString.push_back("Your Estimated Social Security Benefits");
  //  outputString.push_back(equals);
  //  outputString.push_back(
  //    "Your work under Social Security helps you and your family to qualify");
  //  outputString.push_back(
  //    "for benefit payments.  The kinds of benefits you might get are");
  //  outputString.push_back(
  //    "described below.  For each benefit, you need a certain number of");
  //  outputString.push_back(
  //    "work credits (see page 4).  Once you have enough credits, your");
  //  outputString.push_back(
  //    "benefit amount depends on your average earnings over your working");
  //  outputString.push_back(
  //    "lifetime.  We used the earnings in the chart on page 2 to figure");
  //  strm.str("");
  //  strm << "your credits and estimate your benefits.  We included any "
  //    << PunctFacet::getYear(UserAssumptions::getIstart() - 1) << " and ";
  //  outputString.push_back(strm.str());
  //  strm.str("");
  //  strm << PunctFacet::getYear(UserAssumptions::getIstart())
  //    << " earnings and any estimated future earnings you told us about.";
  //  outputString.push_back(strm.str());
  //  outputString.push_back("");
  //  outputString.push_back("Retirement Benefits");
  //  outputString.push_back("");
  //  strm.str("");
  //  strm << "To get Social Security retirement benefits, you need "
  //    << pebs.getQcReq(Pebs::PEBS_OAB_DELAYED) << " credits of";
  //  outputString.push_back(strm.str());
  //  outputString.push_back(
  //    "work.  That is also how many you need for Medicare at age 65.  Your");
  //  strm.str("");
  //  strm << "record shows that you";
  //  if (pebs.getQcReq(Pebs::PEBS_OAB_DELAYED) > pebs.getQcTotal()) {
  //    strm << " do not";
  //  }
  //  strm << " have enough credits.";
  //  outputString.push_back(strm.str());
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "On page 5, we explain about different ages when you can retire.  If");
  //  outputString.push_back(
  //    "you worked at your present rate up to each retirement age, your");
  //  outputString.push_back("monthly amount would be about:");
  //  // early retirement benefit
  //  if (pebs.getPebsOab() == 3) {
  //    strm.str("");
  //    strm << "  At age " << pebs.agePlan1.toString() << " (reduced benefit) ";
  //    const string::size_type slength = strm.str().size();
  //    strm << string(65 - slength, '.') << " $" << setw(5)
  //      << pebs.getBenefitPebs(Pebs::PEBS_OAB_EARLY);
  //    outputString.push_back(strm.str());
  //  }
  //  // normal retirement (or between normal retirement age and 70)
  //  if (pebs.getPebsOab() > 1) {
  //    strm.str("");
  //    if (pebs.oab1 > pebs.fullRetAge) {
  //      strm << "  At age " << pebs.oab1.toString() << " ";
  //    }
  //    else {
  //      strm << "  At full-retirement age (" << pebs.fullRetAge.toString()
  //        << ") ";
  //    }
  //    const string::size_type slength = strm.str().size();
  //    strm << string(65 - slength, '.') << " $" << setw(5)
  //      << pebs.getBenefitPebs(Pebs::PEBS_OAB_FULL);
  //    outputString.push_back(strm.str());
  //  }
  //  // delayed retirement (age 70 or later)
  //  strm.str("");
  //  if (pebs.oab2 > pebs.age70) {
  //    strm << "  At age " << pebs.oab2.toString() << " ";
  //  }
  //  else {
  //    strm << "  At age 70 ";
  //  }
  //  const string::size_type slength = strm.str().size();
  //  strm << string(65 - slength, '.') << " $" << setw(5)
  //    << pebs.getBenefitPebs(Pebs::PEBS_OAB_DELAYED);
  //  outputString.push_back(strm.str());
  //  if (pebs.getPebsDib() > 0) {
  //    outputString.push_back("");
  //    outputString.push_back("Disability Benefits");
  //    outputString.push_back("");
  //    strm.str("");
  //    strm << "If you become disabled right now, you need "
  //      << pebs.getQcReq(Pebs::PEBS_DISAB) << " credits to qualify for";
  //    outputString.push_back(strm.str());
  //    strm.str("");
  //    strm << "benefits.  You had to earn " << pebs.getQcDisReq()
  //      << " of these credits in the last " << pebs.getQcDisYears()
  //      << " years.";
  //    outputString.push_back(strm.str());
  //    strm.str("");
  //    strm << "Your record shows that you";
  //    if (pebs.getQcReq(Pebs::PEBS_DISAB) > pebs.getQcTotal() ||
  //      pebs.getQcDisReq() > pebs.getQcDisTotal()) {
  //      strm << " do not";
  //    }
  //    strm << " have enough credits in the " << pebs.getQcDisYears()
  //      << "-year";
  //    outputString.push_back(strm.str());
  //    outputString.push_back("period.");
  //    outputString.push_back("");
  //    if (pebs.getQcReq(Pebs::PEBS_DISAB) > pebs.getQcTotal() ||
  //      pebs.getQcDisReq() > pebs.getQcDisTotal()) {
  //      outputString.push_back(
  //        "Because you do not meet both parts of the disability credits");
  //      outputString.push_back(
  //        "requirement right now, we are not giving you a benefit estimate.");
  //      outputString.push_back(
  //        "If you work in the future and earn enough credits to qualify, we");
  //      outputString.push_back("will give you a benefit estimate then.");
  //    }
  //    else {
  //      outputString.push_back("If you become disabled right now:");
  //      strm.str("");
  //      strm << "  Your monthly benefit would be about " << string(27, '.')
  //        << " $" << setw(5) << pebs.getPiaPebs(Pebs::PEBS_DISAB);
  //      outputString.push_back(strm.str());
  //      outputString.push_back(
  //        "  You and your family members who qualify could get up to");
  //      strm.str("");
  //      strm << "  a monthly total of about " << string(38, '.') << " $"
  //        << setw(5) << pebs.getMfbPebs(Pebs::PEBS_DISAB);
  //      outputString.push_back(strm.str());
  //      outputString.push_back(
  //        "These estimates may be reduced if you get workers' compensation");
  //      outputString.push_back("or public disability benefits.");
  //    }
  //  }
  //  outputString.push_back("");
  //  outputString.push_back("Family Benefits");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "If you get retirement or disability benefits, your spouse and young");
  //  outputString.push_back("children may also qualify for benefits.");
  //  outputString.push_back("");
  //  outputString.push_back("Survivor Benefits");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "If you die, certain members of your family may qualify for survivor");
  //  outputString.push_back(
  //    "benefits on your record.  See page 5 for an explanation of who may");
  //  outputString.push_back("qualify.");
  //  outputString.push_back("");
  //  strm.str("");
  //  strm << "If you die right now, you need "
  //    << pebs.getQcReq(Pebs::PEBS_SURV)
  //    << " credits for your survivors to get";
  //  outputString.push_back(strm.str());
  //  strm.str("");
  //  strm << "benefits.  Your record shows you";
  //  if (pebs.getQcReq(Pebs::PEBS_SURV) > pebs.getQcTotal()) {
  //    strm << " do not";
  //  }
  //  strm << " have enough.  If they met";
  //  outputString.push_back(strm.str());
  //  outputString.push_back(
  //    "all other requirements, monthly benefit amounts would be about:");
  //  strm.str("");
  //  strm << "  For your child " << string(48, '.') << " $" << setw(5)
  //    << pebs.getBenefitPebs(Pebs::PEBS_SURV);
  //  outputString.push_back(strm.str());
  //  strm.str("");
  //  strm << "  For your spouse who is caring for your child "
  //    << string(18, '.') << " $" << setw(5)
  //    << pebs.getBenefitPebs(Pebs::PEBS_SURV);
  //  outputString.push_back(strm.str());
  //  strm.str("");
  //  strm << "  When your spouse reaches full retirement age "
  //    << string(18, '.') << " $" << setw(5)
  //    << pebs.getPiaPebs(Pebs::PEBS_SURV);
  //  outputString.push_back(strm.str());
  //  outputString.push_back(
  //    "  For all your family members, if others also qualify");
  //  strm.str("");
  //  strm << "  (more children, for example) " << string(34, '.') << " $"
  //    << setw(5) << pebs.getMfbPebs(Pebs::PEBS_SURV);
  //  outputString.push_back(strm.str());
  //  outputString.push_back(
  //    "We also may be able to pay your spouse or children a one-time death");
  //  outputString.push_back("benefit of $255.");
  //} catch (PiaException&) {
  //  throw PiaException("Error in PebsOasdiEst::prepareStrings");
  //}
}
