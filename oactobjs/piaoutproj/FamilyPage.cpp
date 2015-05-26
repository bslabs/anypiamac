// Functions for the <see cref="FamilyPage"/> class to manage family
// members' benefits output.

// $Id: FamilyPage.cpp 1.20.1.1 2012/03/08 07:17:37EST 277133 Development  $

#include <iomanip>
#include "FamilyPage.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PiadataArray.h"
#include "WorkerDataArray.h"
#include "SecondaryArray.h"
#include "DateFormatter.h"
#include "PiaCalAny.h"

using namespace std;

/// <summary>Initializes FamilyPage.</summary>
///
/// <param name="newPiaCal">Benefit calculation parameters and results.
/// </param>
FamilyPage::FamilyPage( const PiaCalAny& newPiaCal ) : PiaPageOut(),
workerData(newPiaCal.workerData), piaData(newPiaCal.piaData),
widowDataArray(newPiaCal.widowDataArray), widowArray(newPiaCal.widowArray),
piaCal(newPiaCal), secondaryArray(newPiaCal.secondaryArray)
{ }

/// <summary>Destructor.</summary>
FamilyPage::~FamilyPage()
{ }

/// <summary>Prepares strings for family members page.</summary>
void FamilyPage::prepareStrings()
{
  try {
    string s;  // temporary string
    // do not prepare this page if no family
    if (widowArray.getFamSize() == 0)
      return;
    pageTitle[0] = "Family members' benefits";
    outputString.clear();
    prepareHeader();
    for (int i = 0; i < widowArray.getFamSize(); i++) {
      Secondary* secondaryt = secondaryArray.secondary[i];
      WorkerData* widowData = widowDataArray.workerData[i];
      PiaData* widowPiaData = widowArray.piaData[i];
      // stop when reaching a blank bic
      if (secondaryt->bic.getMajorBic() == ' ')
        break;
      outputString.push_back("");
      strm.str("");
      strm << "Family member " << (i+1) << ":";
      outputString.push_back(strm.str());
      // write type of benie
      AfxFormatString1(s, PIA_IDS_JSURVOUT,
        (workerData.getJoasdi() == WorkerData::SURVIVOR) ?
        secondaryt->bic.getSurvivorType() :
        secondaryt->bic.getLifeAuxType());
      outputString.push_back(s);
      // write date of birth
      AfxFormatString1(s, PIA_IDS_DOBOUT,
        DateFormatter::toString(widowData->getBirthDate()));
      outputString.push_back(s);
      // write date of onset for disabled widow
      if (secondaryt->bic.getMajorBic() == 'W') {
        AfxFormatString1(s, PIA_IDS_WIDDIBOUT,
          DateFormatter::toString(widowData->disabPeriod[0].getOnsetDate()));
        outputString.push_back(s);
      }
      // write date of entitlement
      AfxFormatString1(s, PIA_IDS_SURVENTOUT,
        DateFormatter::toString(secondaryt->entDate));
      outputString.push_back(s);
      // write full retirement age for aged spouse or widow(er)
      if (secondaryt->bic.isReducible() &&
        secondaryt->bic.getMajorBic() != 'W') {
        if (secondaryt->bic.getMajorBic() == 'D') {
          AfxFormatString1(s, PIA_IDS_NRAWIDOUT,
            widowPiaData->fullRetAge.toString());
        }
        else {
          AfxFormatString1(s, PIA_IDS_NRAWIFEOUT,
            widowPiaData->fullRetAge.toString());
        }
        outputString.push_back(s);
      }
      // write benefit factor
      strm.str("");
      strm.precision(3);
      strm << setw(8) << secondaryt->getBenefitFactor();
      AfxFormatString1(s, PIA_IDS_BENFOUT, strm.str());
      outputString.push_back(s);
      // write full benefit
      strm.precision(2);
      strm.str("");
      strm << setw(8) << secondaryt->getFullBenefit();
      AfxFormatString1(s, PIA_IDS_FULLBEN, strm.str());
      outputString.push_back(s);
      // write benefit after mfb
      strm.str("");
      strm << setw(8) << secondaryt->getBenefit();
      AfxFormatString1(s, PIA_IDS_MFBBEN, strm.str());
      outputString.push_back(s);
      // write age reduction info
      if (secondaryt->bic.isReducible()) {
        strm.str("");
        strm << setw(8) << secondaryt->getMonthsArdri();
        AfxFormatString1(s, PIA_IDS_ARFMOUT, strm.str());
        outputString.push_back(s);
        strm.str("");
        strm << setw(8) << setprecision(5) << secondaryt->getArf();
        AfxFormatString1(s, PIA_IDS_ARFFOUT, strm.str());
        outputString.push_back(s);
      }
      // write benefit reduced for age
      strm.str("");
      strm.precision(2);
      strm << setw(8) << secondaryt->getReducedBenefit();
      AfxFormatString1(s, PIA_IDS_BEN1OUT, strm.str());
      outputString.push_back(s);
      strm.str("");
      strm << setw(8) << secondaryt->getRoundedBenefit();
      AfxFormatString1(s, PIA_IDS_BEN2OUT, strm.str());
      outputString.push_back(s);
    }
    // The following is a debugging quantity which is not needed at the
    // moment
    //strm << "Total Family Benefit: "
    //  << secondaryArray.getTotalFullBenefit();
    //outputString.push_back(strm.str());
  } catch (PiaException&) {
    throw PiaException("Error in FamilyPage::prepareStrings");
  }
}
