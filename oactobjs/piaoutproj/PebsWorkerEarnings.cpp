// Functions for the <see cref="PebsWorkerEarnings"/> class to manage the
// Social Security Statement worker info and earnings page.

// $Id: PebsWorkerEarnings.cpp 1.8.1.2 2012/03/08 10:56:08EST 277133 Development  $

#include "PebsWorkerEarnings.h"
#include "PebsWorkerInfo.h"
#include "PebsEarningsInfo.h"
#include "PiaException.h"

using namespace std;

/// <summary>Initializes PebsWorkerEarnings.</summary>
///
/// <param name="newPebsWorkerInfo">Worker info.</param>
/// <param name="newPebsEarningsInfo">Worker's earnings.</param>
PebsWorkerEarnings::PebsWorkerEarnings(
const PebsWorkerInfo& newPebsWorkerInfo,
const PebsEarningsInfo& newPebsEarningsInfo ) : PageOut(),
pebsWorkerInfo(newPebsWorkerInfo), pebsEarningsInfo(newPebsEarningsInfo)
{ }

/// <summary>Destructor.</summary>
PebsWorkerEarnings::~PebsWorkerEarnings()
{ }

/// <summary>Prepares top part of summary page.</summary>
void PebsWorkerEarnings::prepareStrings()
{
  //try {
  //  outputString.clear();
  //  for (unsigned int i = 0u; i < pebsWorkerInfo.getCount(); i++) {
  //    outputString.push_back(pebsWorkerInfo.getString(i));
  //  }
  //  for (unsigned int i = 0u; i < pebsEarningsInfo.getCount(); i++) {
  //    outputString.push_back(pebsEarningsInfo.getString(i));
  //  }
  //} catch (PiaException&) {
  //  throw PiaException("Error in PebsWorkerEarnings::prepareStrings");
  //}
}
