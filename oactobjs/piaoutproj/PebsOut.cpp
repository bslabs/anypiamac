// Functions for the <see cref="PebsOut"/> class to print out the
// Social Security Statement.

// $Id: PebsOut.cpp 1.12.1.3 2012/03/12 12:49:10EDT 277133 Development  $

#include "PebsOut.h"

using namespace std;

/// <summary>Initializes PebsOut instance.</summary>
///
/// <param name="newWorkerData">Worker data.</param>
/// <param name="newPiaData">Pia calculation data.</param>
/// <param name="newFieldOfficeInfo">Field office information.</param>
/// <param name="newPebs">Social Security Statement data.</param>
/// <param name="newPiaParams">Pia calculation parameters.</param>
/// <param name="newTaxData">Taxes paid.</param>
/// <param name="newEarnProject">Projected earnings information.</param>
PebsOut::PebsOut( const WorkerData& newWorkerData, const PiaData& newPiaData,
const FieldOfficeInfo& newFieldOfficeInfo, const Pebs& newPebs,
const PiaParams& newPiaParams, const TaxData& newTaxData,
const EarnProject& newEarnProject ) :
pebsLetter(newWorkerData, newFieldOfficeInfo),
pebsWorkerInfo(newWorkerData, newPebs, newPiaParams, newEarnProject),
pebsCreditInfo(newWorkerData, newPiaParams),
pebsEarnings(newWorkerData, newPiaData, newPiaParams, newTaxData),
pebsOasdiEst(newWorkerData, newPebs),
pebsEarningsInfo(newPiaParams),
pebsOasdiInfo(newWorkerData),
pebsWorkerEarnings(pebsWorkerInfo, pebsEarningsInfo)
{ }

/// <summary>Destructor.</summary>
PebsOut::~PebsOut()
{ }

/// <summary>Prepares strings for printing.</summary>
void PebsOut::prepareStrings()
{
  pebsLetter.prepareStrings();
  //pebsWorkerInfo.prepareStrings();
  //pebsCreditInfo.prepareStrings();
  //pebsEarnings.prepareStrings();
  //pebsOasdiEst.prepareStrings();
  //pebsEarningsInfo.prepareStrings();
  //pebsOasdiInfo.prepareStrings();
  //pebsWorkerEarnings.prepareStrings();
}

/// <summary>Fills out page numbers.</summary>
void PebsOut::setPageNum()
{
  int pageCount = 0;
  pebsWorkerInfo.setPageNum(++pageCount);
  pebsEarnings.setPageNum(++pageCount);
  pebsOasdiEst.setPageNum(++pageCount);
  pebsCreditInfo.setPageNum(++pageCount);
  pebsOasdiInfo.setPageNum(++pageCount);
}
