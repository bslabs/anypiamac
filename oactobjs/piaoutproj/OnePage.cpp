// Functions for the <see cref="OnePage"/> class to manage one-page summary
// of results.

// $Id: OnePage.cpp 1.25.1.3 2017/10/12 13:07:40EDT 277133 Development  $

#include "OnePage.h"
#include "PiaTable.h"
#include "TransGuar.h"
#include "WageInd.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "DateFormatter.h"
#include "PiaCalAny.h"
#include "wrkrdata.h"
#include "PiadataArray.h"

using namespace std;

/// <summary>Initializes OnePage.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
OnePage::OnePage( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
piaCal(newPiaCal)
{ }

/// <summary>Destructor.</summary>
OnePage::~OnePage()
{ }

/// <summary>Prepares strings for 1-page summary.</summary>
void OnePage::prepareStrings()
{
  int firstyr;  // first year to print out
  int lastyr;  // last year to print out
  try {
    outputString.clear();
    if (workerData.getJoasdi() == WorkerDataGeneral::PEBS_CALC)
      return;
    outputString.push_back("");
    outputString.push_back("");
    switch (workerData.getJoasdi())
    {
    case WorkerDataGeneral::OLD_AGE:
      outputString.push_back(
        "                   R E T I R E M E N T   E S T I M A T E");
      break;
    case WorkerDataGeneral::SURVIVOR:
      outputString.push_back(
        "                     S U R V I V O R   E S T I M A T E");
      break;
    case WorkerDataGeneral::DISABILITY:
      outputString.push_back(
        "                   D I S A B I L I T Y   E S T I M A T E");
      break;
    default:
      break;
    }
    outputString.push_back("");
    outputString.push_back("");
    strm.str("");
    strm << "                                               "
      << DateFormatter::toString(boost::gregorian::day_clock::local_day());
    outputString.push_back(strm.str());
    printNameSsn(workerData);
    string s = "Worker date of birth: ";
    s +=  DateFormatter::toString(workerData.getBirthDate());
    outputString.push_back(s);
    if (workerData.getJoasdi() == WorkerDataGeneral::SURVIVOR) {
      s = "Worker date of death: ";
      s +=  DateFormatter::toString(workerData.getDeathDate());
      outputString.push_back(s);
    }
    if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY) {
      s = "Disability onset date: ";
      s += DateFormatter::toString(workerData.disabPeriod[0].getOnsetDate());
      outputString.push_back(s);
    }
    outputString.push_back("");
    outputString.push_back("");
    outputString.push_back(
      "                      R E G U L A R   E A R N I N G S");
    outputString.push_back("");
    outputString.push_back("");
    const double earnTotal50 =
      piaData.getEarnTotal50(PiaData::EARN_NO_TOTALIZATION);
    strm.precision(2);
    if (earnTotal50 > 0.0) {
      strm.str("");
      strm << "50 " << setw(10) << earnTotal50;
      outputString.push_back(strm.str());
      firstyr = 1951;
    }
    else {
      firstyr = (workerData.getIbegin() > 1950) ?
        5 * ((workerData.getIbegin() - 1) / 5) + 1 : 1951;
    }
    lastyr = 5 * ((workerData.getIend() - 1) / 5) + 5;
    printEarn(firstyr, lastyr, 0);
    if ((piaCal.piaTable != (PiaTable*)0 && piaCal.piaTable->getApplicable())
      || (piaCal.transGuar != (TransGuar*)0 &&
      piaCal.transGuar->getApplicable())) {
      outputString.push_back("");
      strm.str("");
      strm << "Dividend:  ";
      if (piaCal.piaTable != (PiaTable*)0 &&
        piaCal.piaTable->getApplicable() > 0) {
        strm << setw(10) << piaCal.piaTable->getftearn();
      }
      else {
        strm << setw(10) << piaCal.transGuar->getftearn();
      }
      strm << "                                  Divisor months: "
        << (12 * piaData.compPeriodNew.getN());
      outputString.push_back(strm.str());
    }
    outputString.push_back("");
    if (piaCal.wageInd != (WageInd*)0) {
      outputString.push_back("");
      outputString.push_back(
        "                      I N D E X E D   E A R N I N G S");
      outputString.push_back("");
      outputString.push_back("");
      printEarn(firstyr, lastyr, 1);
      outputString.push_back("");
      strm.str("");
      strm << "Dividend:  " << setw(13) << piaCal.wageInd->getftearn()
        << "                                  Divisor months: "
        << (12 * piaData.compPeriodNew.getN());
      outputString.push_back(strm.str());
    }
    outputString.push_back("");
    strm.str("");
    strm << " Eff.                          "
      << ((piaData.getArf() >= 1.0) ? "Increment" : "Reduction")
      << "              Family";
    outputString.push_back(strm.str());
    outputString.push_back(
      " date       AIME      PIA       factor       MBA      maximum");
    outputString.push_back(
      "-------    ------    ------    ---------    ------    -------");
    const char *asterisk = (piaData.getIappn() == (int)PiaMethod::WAGE_IND ||
      piaData.getIappn() == -1) ? " " : "*";
    strm.str("");
    strm << DateFormatter::toString(workerData.getBenefitDate(), "ss");
    if (piaCal.wageInd != (WageInd*)0) {
      strm.precision(0);
      strm << setw(10) << (piaCal.wageInd->getAme() + 0.1);
    }
    else {
      strm << "          ";
    }
    strm.precision(2);
    strm << setw(10) << piaData.highPia.get() << asterisk;
    strm << setw(12) << setprecision(5) << piaData.getArf();
    if (workerData.getJoasdi() != WorkerData::SURVIVOR) {
    strm << setprecision(2) << setw(10) << piaData.unroundedBenefit.get()
      << asterisk << setw(10) << piaData.highMfb.get() << asterisk;
    }
    outputString.push_back(strm.str());
    outputString.push_back("");
    // print footnotes
    if (asterisk[0] == '*') {
      strm.str("");
      strm << "*Based on " << piaCal.highPiaMethod->getTitle();
      outputString.push_back(strm.str());
    }
    for (vector< PiaMethod* >::const_iterator iter = piaCal.piaMethod.begin();
      iter != piaCal.piaMethod.end(); iter++) {
      if ((*iter)->getMethod() != PiaMethod::WAGE_IND &&
        (*iter)->getApplicable() > 0) {
        strm.str("");
        strm << "   " << (*iter)->getTitle() << " PIA: "
          << setprecision(2) << (*iter)->piaEnt.get();
        outputString.push_back(strm.str());
        if ((*iter)->getMethod() == PiaMethod::SPEC_MIN) {
          strm.str("");
          strm << "Years of coverage for special minimum: " 
               << (*iter)->getYearsTotal();
          outputString.push_back(strm.str());
        }
     }
    }
    // print windfall message
    if (piaCal.wageInd != (WageInd*)0) {
      if (piaCal.wageInd->getWindfall() != PiaMethod::NOWINDFALLELIM) {
        if (piaCal.wageInd->getWindfall() == PiaMethod::HAS30YEARS) {
          outputString.push_back("WEP considered, but not applied");
        }
        else {
          outputString.push_back("WEP applied");
        }
      }
    }
    // print uninsured message
    nonIns(piaData.getFinsCode2(),workerData,piaData);
    // print disability insured message
    if (workerData.getJoasdi() == WorkerDataGeneral::DISABILITY) {
      disInsOut(piaData);
    }
    if (piaCal.widowArray.getFamSize() > 0) {
      outputString.push_back("");
      outputString.push_back("");
      strm.str("");
      strm << "                      PIA       MFB   raw MBA       MBA    PIFC BIC";
      outputString.push_back(strm.str());
      strm.str("");
      strm << "                   ------    ------   -------    ------    ---- ---";
      outputString.push_back(strm.str());
    }
    for (int i = 0; i < piaCal.widowArray.getFamSize(); i++) {
      outputString.push_back("");
      strm.str("");
      strm << "Family Member ";
      strm << setprecision(0) << (i + 1);
      strm << setprecision(2)  << setfill(' ');
      strm << setw(10) << piaData.highPia.get();
      strm << setw(10) << piaData.highMfb.get();
      Secondary *secondary = piaCal.secondaryArray.secondary[i];
      strm << setw(10) << secondary->getFullBenefit();
      strm << setw(10) << secondary->getRoundedBenefit();
      strm << "       " << secondary->pifc.get();
      // bic from secondary record
      strm << "   " << secondary->bic.toString();
      outputString.push_back(strm.str());
    }
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_ONEPAGE);
  }
}

/// <summary>Prepares table of earnings.</summary>
///
/// <remarks>There are two tiers of the table, as follows:
/// <table>
///     0   actual earnings.
///     1   indexed earnings.
/// </table></remarks>
///
/// <param name="firstyr">First year of earnings.</param>
/// <param name="lastyr">Last year of earnings</param>
/// <param name="tier">Tier of table to prepare.</param>
void OnePage::printEarn( int firstyr, int lastyr, int tier )
{
  const int lines = (lastyr - firstyr + 1) / 5;
  strm.precision(2);
  for (int i1 = firstyr; i1 < firstyr + lines; i1++) {
    strm.str("");
    for (int i2 = 0; i2 < 5; i2++) {
      const int i3 = i1 + i2 * lines;
      int year = i3 % 100;
      if (year == 0) {
        strm << "  00";
      }
      else {
        strm << ((year < 10) ? "  0" : "  ") << year;
      }
      if (tier == 0) {
        strm << setw(11) << piaData.earnOasdiLimited[i3];
      }
      else {
        strm << setw(11) << piaCal.wageInd->earnIndexed[i3];
      }
    }
    outputString.push_back(strm.str());
  }
}
