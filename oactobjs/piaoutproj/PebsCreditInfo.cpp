// Functions for the <see cref="PebsCreditInfo"/> class to manage the Social
// Security Statement credit info page.

// $Id: PebsCreditInfo.cpp 1.13.1.2 2012/03/08 10:56:05EST 277133 Development  $

#include "PebsCreditInfo.h"
#include "UserAssumptions.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PebsOut.h"
#include "piaparms.h"

using namespace std;

/// <summary>Initializes this instance.</summary>
///
/// <param name="newWorkerData">Worker data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
PebsCreditInfo::PebsCreditInfo( const WorkerData& newWorkerData,
const PiaParams& newPiaParams ) :
PebsPageOut(), workerData(newWorkerData), piaParams(newPiaParams)
{ }

/// <summary>Destructor.</summary>
PebsCreditInfo::~PebsCreditInfo()
{ }

/// <summary>Prepares general credit and tax information for printing.
/// </summary>
void PebsCreditInfo::prepareStrings()
{
  //try {
  //  outputString.clear();
  //  prepareHeader(workerData);
  //  outputString.push_back("Earning Social Security and Medicare Credits");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "As you work and pay Social Security taxes, you earn Social Security");
  //  outputString.push_back("credits:");
  //  outputString.push_back(
  //    "  Before 1978, when your employer reported your earnings every 3");
  //  outputString.push_back(
  //    "  months, they were called \"quarters of coverage.\"  Back then, you");
  //  outputString.push_back(
  //    "  earned a quarter or credit if you earned at least $50 in a 3-month");
  //  outputString.push_back("  quarter.");
  //  outputString.push_back(
  //    "  Starting with 1978, your employer reports your earnings just once");
  //  outputString.push_back(
  //    "  a year and credits are based on how much you earn during the year.");
  //  outputString.push_back(
  //    "  The amount it takes to earn a credit changes each year.");
  //  strm.str("");
  //  strm.precision(0);
  //  const int year = UserAssumptions::getIstart();
  //  strm << "  In " << PunctFacet::getYear(year)
  //    << ", you get one credit for each $" << piaParams.qcamt[year]
  //    << " of your covered annual";
  //  outputString.push_back(strm.str());
  //  outputString.push_back(
  //    "  earnings, up to a maximum of 4 credits for the year, no matter");
  //  outputString.push_back("  when you work during the year.");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "On page 3, we tell you how many credits you need for each kind of");
  //  outputString.push_back(
  //    "benefit and whether you have enough.  Most people need 40 credits (10");
  //  outputString.push_back(
  //    "years of work) to qualify for benefits.  Younger people need fewer");
  //  outputString.push_back(
  //    "credits for disability or for their family members to get survivors");
  //  outputString.push_back("benefits if they should die.");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "The credits you already earned remain on your record, and you add to");
  //  outputString.push_back(
  //    "them as you continue to work and pay Social Security taxes.  Under");
  //  outputString.push_back(
  //    "certain conditions, we may also use credits you earned under a");
  //  outputString.push_back(
  //    "foreign social security system to help you qualify for benefits.");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "When you earn credits for Social Security benefits, they also count");
  //  outputString.push_back(
  //    "for Medicare.  However, if you have government earnings on which you");
  //  outputString.push_back(
  //    "pay Medicare taxes but not Social Security taxes, those are");
  //  outputString.push_back(
  //    "considered \"Medicare-qualified government earnings.\"  Those earnings");
  //  outputString.push_back(
  //    "give you credits for Medicare but do not count for Social Security");
  //  outputString.push_back("benefits.");
  //  outputString.push_back("");
  //  outputString.push_back(equals);
  //  outputString.push_back("Estimating Your Benefits");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "It is the earnings on your record, not the amount of taxes you paid");
  //  outputString.push_back(
  //    "or the number of credits you have, that we use to figure how much");
  //  outputString.push_back(
  //    "you will get each month.  The Social Security law has a special");
  //  outputString.push_back(
  //    "formula for figuring benefits.  The formula uses your average");
  //  outputString.push_back(
  //    "earnings over your entire working life.  For most retirement benefit");
  //  outputString.push_back(
  //    "estimates, we will be averaging your 35 best years of earnings.  If");
  //  outputString.push_back(
  //    "you become disabled or die before retirement, we may use fewer years");
  //  outputString.push_back("to figure those benefits.");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "For the first retirement amount shown, we assumed you would work");
  //  outputString.push_back(
  //    "up to the expected retirement age you gave us.  For later retirement");
  //  outputString.push_back(
  //    "ages, we assumed you would keep on working up to those ages and earn");
  //  outputString.push_back(
  //    "the \"Estimated Future Average Yearly Earnings\" shown on page 1.");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "We now show benefit estimates in current dollars.  If you requested");
  //  outputString.push_back(
  //    "a statement like this before September 1993, we had increased your");
  //  outputString.push_back(
  //    "retirement estimate amount on that statement by 1 percent for each");
  //  outputString.push_back(
  //    "remaining year up to age 62.  This reflected expected economic");
  //  outputString.push_back(
  //    "growth.  We stopped doing this to make your estimate more consistent");
  //  outputString.push_back(
  //    "with estimates prepared in other pension planning programs.");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "If your pension is based on work not covered by Social Security, the");
  //  outputString.push_back(
  //    "amount of your Social Security benefits may be lower than shown on");
  //  outputString.push_back(
  //    "this statement.  Your spouse's benefits on your record may also be");
  //  outputString.push_back(
  //    "affected by his or her pension.  If this applies to you, ask for the");
  //  outputString.push_back(
  //    "free fact sheets \"A Pension From Work Not Covered By Social");
  //  outputString.push_back(
  //    "Security\" and \"Government Pension Offset.\"");
  //} catch (PiaException&) {
  //  throw PiaException("Error in PebsCreditInfo::prepareStrings");
  //}
}
