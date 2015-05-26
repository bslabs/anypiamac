// Functions for the <see cref="TaxesPage"/> class to manage the taxes
// output page.

// $Id: TaxesPage.cpp 1.16.1.1 2012/03/08 07:17:56EST 277133 Development  $

#include <iomanip>
#include "TaxesPage.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "TaxData.h"

using namespace std;

/// <summary>Initializes TaxesPage.</summary>
///
/// <param name="newWorkerData">Worker basic data.</param>
/// <param name="newPiaData">Benefit calculation data.</param>
/// <param name="newTaxData">Taxes paid.</param>
TaxesPage::TaxesPage( const WorkerDataGeneral& newWorkerData,
const PiaData& newPiaData, const TaxData& newTaxData ) :
PiaPageOut(), workerData(newWorkerData), piaData(newPiaData),
taxData(newTaxData)
{ }

/// <summary>Destructor.</summary>
TaxesPage::~TaxesPage()
{ }

/// <summary>Prepares taxes strings for printing.</summary>
void TaxesPage::prepareStrings()
{
  try {
    outputString.clear();
    pageTitle[0] = "Taxes paid";
    prepareHeader();
    outputString.push_back("year      Earnings      OASI        DI        "
      "OASDI       HI       OASDHI");
    outputString.push_back("");
    if (workerData.getIndearn()) {
      for (int yr = workerData.getIbegin(); yr <= workerData.getIend();
        yr++) {
        strm.str("");
        strm.precision(2);
        strm << PunctFacet::getYear(yr)
          << setw(14) << piaData.earnOasdiLimited[yr]
          << setw(11) << taxData.taxesOasi[yr]
          << setw(11) << taxData.taxesDi[yr]
          << setw(11) << taxData.taxesOasdi[yr]
          << setw(11) << taxData.taxesHi[yr]
          << setw(11) << taxData.taxesOasdhi[yr];
        outputString.push_back(strm.str());
      }
      outputString.push_back("");
      strm.str("");
      strm << "Total             ";
      for (int tf = 0; tf < TaxRates::NUMTRUSTFUNDTYPES; tf++) {
        strm << setw(11) << taxData.getTaxesTotal((TaxRates::trustfundType)tf,
          TaxData::YEARS1937PLUS);
      }
      outputString.push_back(strm.str());
    }
    else {
      outputString.push_back("None");
    }
    outputString.push_back("");
    printNameSsn(workerData);
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_TXSTRING);
  }
}
