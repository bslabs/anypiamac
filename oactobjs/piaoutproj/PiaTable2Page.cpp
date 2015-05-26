// Functions for the <see cref="PiaTable2Page"/> class to manage a pia table
// calculation results page.

// $Id: PiaTable2Page.cpp 1.14.1.3 2012/09/06 10:39:40EDT 277133 Development  $

#include <iomanip>
#include "PiaTable2Page.h"
#include "PiaTable.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes PiaTable2Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
PiaTable2Page::PiaTable2Page( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaParams(newPiaCal.piaParams), piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
PiaTable2Page::~PiaTable2Page()
{ }

/// <summary>Prepares pia table strings for printing.</summary>
void PiaTable2Page::prepareStrings()
{
  try {
    const PiaTable* piaTable = piaCal.piaTable;
    if (piaTable != (const PiaTable*)0 && piaTable->getApplicable() > 0) {
      pageTitle[0] = piaTable->getTitle();
      outputString.clear();
      prepareHeader();
      compYear(workerData, piaData, piaData.compPeriodNew);
      const int entYear = piaTable->getCpiYear(PiaMethod::YEAR_ENT);
      const int benYear = piaTable->getCpiYear(PiaMethod::YEAR_BEN);
      const int firstYear = piaTable->getCpiYear(PiaMethod::FIRST_YEAR);
      const int tableYear = piaTable->getCpiYear(PiaMethod::YEAR_TABLE);
      strm.precision(2);
      strm.str("");
      strm << "AME = " << piaTable->getftearn() << "/("
        << piaData.compPeriodNew.getN() << "*12) = " << setprecision(0)
        << piaTable->getAme();
      outputString.push_back(strm.str());
      outputString.push_back("");
      osTableOut(*piaTable);
      if (entYear >= firstYear) {
        piamfb3(*piaTable);
        if (tableYear >= firstYear) {
          if (entYear <= tableYear) {
            newTableOut(*piaTable, piaParams, piaData.getEligYear(), 
              firstYear, entYear, 3);
          }
          else {
            newTableOut(*piaTable, piaParams, piaData.getEligYear(),
              firstYear, tableYear, 3);
            biOut(*piaTable, piaParams, piaData.getEligYear(), tableYear + 1,
              entYear, 3);
            if (benYear > entYear) {
              piamfb1(*piaTable);
              biOut(*piaTable, piaParams, piaData.getEligYear(), 2, 3);
            }
          }
        }
        else {
          biOut(*piaTable, piaParams, piaData.getEligYear(), 0, 3);
          if (benYear > entYear) {
            piamfb1(*piaTable);
            biOut(*piaTable, piaParams, piaData.getEligYear(), 2, 3);
          }
        }
      }
      else {
        if (benYear >= firstYear) {
          piamfb3(*piaTable);
          if (tableYear >= firstYear) {
            newTableOut(*piaTable, piaParams, piaData.getEligYear(),
              firstYear, tableYear, 3);
            if (benYear > tableYear) {
              biOut(*piaTable, piaParams, piaData.getEligYear(),
                tableYear + 1, benYear, 3);
            }
          }
          else {
            biOut(*piaTable, piaParams, piaData.getEligYear(), 3, 3);
          }
        }
      }
      if (workerData.getTotalize()) {
        totPiaOut(*piaTable, piaData.getQcTotalRel(),
          piaData.compPeriodNew.getN());
      }
      piamfb2(*piaTable);
      // print noninsured message
      nonIns(piaData.getFinsCode2(),workerData,piaData);
      if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY) {
        disInsOut(piaData);
      }
      outputString.push_back("");
      printNameSsn(workerData);
    }
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_PTSTRING);
  }
}
