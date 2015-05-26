// Functions for the <see cref="PebsEarningsInfo"/> class to manage the
// Social Security Statement earnings info page.

// $Id: PebsEarningsInfo.cpp 1.17.1.2 2012/03/08 10:56:06EST 277133 Development  $

#include "PebsEarningsInfo.h"
#include "UserAssumptions.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PebsOut.h"
#include "WageBase.h"
#include "piaparms.h"

using namespace std;

/// <summary>Initializes this instance.</summary>
///
/// <param name="newPiaParams">Pia calculation parameters.</param>
PebsEarningsInfo::PebsEarningsInfo( const PiaParams& newPiaParams ) :
PebsPageOut(), piaParams(newPiaParams)
{ }

/// <summary>Destructor.</summary>
PebsEarningsInfo::~PebsEarningsInfo()
{ }

/// <summary>Prepares earnings info for printing.</summary>
void PebsEarningsInfo::prepareStrings()
{
  //try {
  //  outputString.clear();
  //  outputString.push_back(equals);
  //  outputString.push_back("Your Earnings Record");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "We keep a record of the amount of earnings reported each year under");
  //  outputString.push_back(
  //    "your name and Social Security number.  When you apply for benefits,");
  //  outputString.push_back(
  //    "we check your record to see if you worked enough over the years to");
  //  outputString.push_back(
  //    "qualify.  Then we base the amount of your payments on your average");
  //  outputString.push_back("earnings over your working lifetime.");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "Almost all kinds of employment and self-employment earnings are");
  //  outputString.push_back("covered for Social Security and Medicare:");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "  Most wages have been covered by Social Security taxes since 1937");
  //  outputString.push_back(
  //    "  and most kinds of self-employment since 1951.");
  //  outputString.push_back(
  //    "  Medicare taxes on both kinds of earnings started in 1966.");
  //  outputString.push_back(
  //    "  Some Federal, State and local government workers do not pay Social");
  //  outputString.push_back(
  //    "  Security taxes, but most of them do pay Medicare taxes on their");
  //  outputString.push_back(
  //    "  \"Medicare-qualified government earnings.\"");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "If you work for wages, your employer reports the amount of your");
  //  outputString.push_back(
  //    "earnings to Social Security after the end of each year.  If you are");
  //  outputString.push_back(
  //    "self-employed, you report your net earnings on your yearly income");
  //  outputString.push_back(
  //    "tax return.  The chart on page 2 shows the amounts of earnings");
  //  outputString.push_back(
  //    "reported to us.  If you had more than one employer during the year,");
  //  outputString.push_back(
  //    "your earnings from all of them have been combined.");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "There are limits each year on how much earnings are taxable for");
  //  outputString.push_back(
  //    "Social Security and for Medicare.  If you earn more than the maximum");
  //  outputString.push_back(
  //    "amount, the extra earnings will not be shown.  The chart on page 2");
  //  outputString.push_back(
  //    "shows the maximum amount that was taxable for each year so far.");
  //  outputString.push_back("");
  //  outputString.push_back(equals);
  //  outputString.push_back("Your Social Security Taxes");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "The Internal Revenue Service collects your Social Security and");
  //  outputString.push_back(
  //    "Medicare taxes.  We do not keep that record.  To estimate the Social");
  //  outputString.push_back(
  //    "Security and Medicare taxes you paid, we multiplied your reported");
  //  outputString.push_back(
  //    "earnings by the tax rate for each year.  The amounts are shown in");
  //  outputString.push_back(
  //    "separate columns on the chart.  If you had both wages and self-");
  //  outputString.push_back(
  //    "employment earnings in the same year, we estimated the taxes as if");
  //  outputString.push_back(
  //    "the total amount was wages.  If you had both Social Security");
  //  outputString.push_back(
  //    "earnings and government earnings that qualified for Medicare in the");
  //  outputString.push_back(
  //    "same year, we estimated the combined Medicare taxes you paid.");
  //  outputString.push_back("");
  //  if (UserAssumptions::getIstart() == 2011) {
  //    strm.str("");
  //    strm << "For " << PunctFacet::getYear(UserAssumptions::getIstart())
  //      << " you pay 4.2 percent and your employer pays 6.2 percent";
  //    outputString.push_back(strm.str());
  //    strm.str("");
  //    strm.precision(0);
  //    strm << "on the first $"
  //      << piaParams.getBaseOasdi(UserAssumptions::getIstart())
  //      << " of covered wages.  You each also pay Medicare ";
  //    outputString.push_back(strm.str());
  //    outputString.push_back(
  //      "taxes of 1.45 percent on all your covered wages.  If you are");
  //    outputString.push_back(
  //      "self-employed, your Social Security tax is 10.4 percent and your");
  //    outputString.push_back(
  //      "Medicare tax is 2.9 percent on the same amounts of earnings.");
  //    outputString.push_back("");
  //  } else {
  //    strm.str("");
  //    strm << "For " << PunctFacet::getYear(UserAssumptions::getIstart())
  //      << " you and your employer each pay Social Security taxes of";
  //    outputString.push_back(strm.str());
  //    strm.str("");
  //    strm.precision(0);
  //    strm << "6.2 percent on the first $"
  //      << piaParams.getBaseOasdi(UserAssumptions::getIstart())
  //      << " of covered wages.  You each also ";
  //    outputString.push_back(strm.str());
  //    outputString.push_back(
  //      "pay Medicare taxes of 1.45 percent on all your covered wages.  If");
  //    outputString.push_back(
  //      "you are self-employed, your Social Security tax is 12.4 percent and");
  //    outputString.push_back(
  //      "your Medicare tax is 2.9 percent on the same amounts of earnings.");
  //    outputString.push_back("");
  //  }
  //} catch (PiaException&) {
  //  throw PiaException("Error in PebsEarningsInfo::prepareStrings");
  //}
}
