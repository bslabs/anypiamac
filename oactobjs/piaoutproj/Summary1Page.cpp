// Functions for the <see cref="Summary1Page"/> class to manage the top part
// of a summary of results page.

// $Id: Summary1Page.cpp 1.20.1.1 2012/03/08 07:17:53EST 277133 Development  $

#include "Summary1Page.h"
#include "Resource.h"
#include "FormatString.h"
#include "PiaException.h"
#include "DateFormatter.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes Summary1Page.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
Summary1Page::Summary1Page( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
Summary1Page::~Summary1Page()
{ }

/// <summary>Prepares top part of summary page.</summary>
void Summary1Page::prepareStrings()
{
  string s;  // temporary string
  try {
    outputString.clear();
    pageTitle[0] = "Summary of results";
    pageTitle[1] = "";
    prepareHeader();
    printNameSsn(workerData);
    AfxFormatString1(s, PIA_IDS_SEXOUT, Sex(workerData.getSex()).toString());
    outputString.push_back(s);
    AfxFormatString1(s, PIA_IDS_DOBOUT,
      DateFormatter::toString(workerData.getBirthDate()));
    outputString.push_back(s);
    switch (workerData.getJoasdi())
    {
    case WorkerDataGeneral::OLD_AGE:
      AfxFormatString2(s, PIA_IDS_OABENTOUT,
        DateFormatter::toString(workerData.getEntDate()),
        piaData.ageEnt.toString());
      outputString.push_back(s);
      if (workerData.getRecalc()) {
        AfxFormatString2(s, PIA_IDS_OABBENOUT,
          DateFormatter::toString(workerData.getBenefitDate()),
          piaData.ageBen.toString());
        outputString.push_back(s);
      }
      if (workerData.getValdi() > 0) {
        printDisInfo();
      }
      AfxFormatString1(s, PIA_IDS_NRAOUT, piaData.fullRetAge.toString());
      outputString.push_back(s);
      AfxFormatString1(s, PIA_IDS_NRA1OUT, piaData.earlyRetAge.toString());
      outputString.push_back(s);
      if (piaData.getFullRetDate() < piaData.getFullInsDate()) {
        s = "Date first insured: ";
        s += DateFormatter::toString(piaData.getFullInsDate());
        outputString.push_back(s);
      }
      break;
    case WorkerDataGeneral::SURVIVOR:
      AfxFormatString1(s, PIA_IDS_DEATHOUT,
        DateFormatter::toString(workerData.getDeathDate()));
      outputString.push_back(s);
      AfxFormatString1(s, PIA_IDS_SURVBENOUT,
        DateFormatter::toString(workerData.getBenefitDate()));
      outputString.push_back(s);
      if (workerData.getValdi() > 0) {
        printDisInfo();
      }
    break;
    case WorkerDataGeneral::DISABILITY:
      AfxFormatString1(s, PIA_IDS_ONSETOUT,
        DateFormatter::toString(workerData.disabPeriod[0].getOnsetDate()));
      outputString.push_back(s);
      if (workerData.getValdi() > 1) {
        AfxFormatString1(s, PIA_IDS_PRIOROUT,
          DateFormatter::toString(workerData.disabPeriod[1].getOnsetDate()));
        outputString.push_back(s);
        AfxFormatString1(s, PIA_IDS_PRCESSOUT, DateFormatter::toString(
          workerData.disabPeriod[1].getCessationDate()));
        outputString.push_back(s);
      }
      AfxFormatString2(s, PIA_IDS_DIBENTOUT, DateFormatter::toString(
        workerData.getEntDate()), piaData.ageEnt.toString());
      outputString.push_back(s);
      if (workerData.getRecalc()) {
        AfxFormatString2(s, PIA_IDS_OABBENOUT, DateFormatter::toString(
          workerData.getBenefitDate()), piaData.ageBen.toString());
        outputString.push_back(s);
      }
      break;
    default:
      return;
    }  // end switch
    if (workerData.getPubpen() > 0.0f)
      nonpenOut(piaData.getPubpenApp(workerData),workerData.getPubpenDate());
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_TWOPAGE3);
  }
}

/// <summary>Prints disability information.</summary>
///
/// <remarks>Called from <see cref="prepareStrings"/> only if there is a
/// disability.</remarks>
void Summary1Page::printDisInfo()
{
  string s;  // temporary string
  const DisabPeriod& disabPeriod0 = workerData.disabPeriod[0];
  AfxFormatString1(s, PIA_IDS_PONSETOUT,
    DateFormatter::toString(disabPeriod0.getOnsetDate()));
  outputString.push_back(s);
  AfxFormatString1(s, PIA_IDS_PRENTOUT,
    DateFormatter::toString(disabPeriod0.getEntDate()));
  outputString.push_back(s);
  AfxFormatString1(s, PIA_IDS_PRCESSOUT,
    DateFormatter::toString(disabPeriod0.getCessationDate()));
  outputString.push_back(s);
  if (workerData.getValdi() > 1) {
    const DisabPeriod& disabPeriod1 = workerData.disabPeriod[1];
    AfxFormatString1(s, PIA_IDS_ONSET2OUT,
      DateFormatter::toString(disabPeriod1.getOnsetDate()));
    outputString.push_back(s);
    AfxFormatString1(s, PIA_IDS_CESS2OUT,
      DateFormatter::toString(disabPeriod1.getCessationDate()));
    outputString.push_back(s);
  }
}
