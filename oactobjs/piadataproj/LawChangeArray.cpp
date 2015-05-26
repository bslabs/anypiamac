// Functions for the <see cref="LawChangeArray"/> class to manage an array
// of <see cref="LawChange"/> parameters.
//
// $Id: LawChangeArray.cpp 1.14 2011/07/29 16:27:31EDT 044579 Development  $

#include "LawChangeArray.h"
#include "Resource.h"
#include "PiaException.h"
#include "LawChangeBPFRACWAGE.h"
#include "LawChangeNEWFORMULA.h"
#include "LawChangeDECLINEPERC.h"
#include "LawChangeBPSPECRATE.h"
#include "LawChangeBPMINCONST.h"
#include "LawChangeAGE65COMP.h"
#include "LawChangeDROPOUTCHG.h"
#include "LawChangeALLEARN.h"
#include "LawChangeMARRLENGTH.h"
#include "LawChangeNEWSPECMIN.h"
#include "LawChangeTAXRATECHG.h"
#include "LawChangeNRACHANGE.h"
#include "LawChangeWIFEFACTOR.h"
#include "LawChangeWIDFACTOR.h"
#include "LawChangeCHILDCARECREDIT.h"
#include "LawChangeCHILDCAREDROPOUT.h"
#include "LawChangeTAXBENCHG.h"
#include "LawChangeWAGEBASECHG.h"
#include "LawChangePSAACCT.h"
#include "LawChangeCOLACHANGE.h"
#include "Path.h"

using namespace std;

/// <summary>Initializes law-change array.</summary>
///
/// <remarks>Creates all of the <see cref="LawChange"/>'s.</remarks>
///
/// <param name="startt">Earliest starting year for changes.</param>
/// <param name="newMaxyear">Maximum projected year.</param>
/// <param name="newDirectory">Directory with law-change indicators.</param>
LawChangeArray::LawChangeArray( int startt, int newMaxyear,
const std::string& newDirectory ) :
Document("lawchg.dat", newDirectory), lawChange(LawChange::MAXLCH)
{
  LawChange::setStartYearLC(startt);
  LawChange::setEndYearLC(newMaxyear);
  lawChange[LawChange::PRE1977LAW] = new LawChange("pre-1977 law benefits");
  lawChange[LawChange::BPFRACWAGE] = new LawChangeBPFRACWAGE();
  lawChangeBPFRACWAGE =
    dynamic_cast<LawChangeBPFRACWAGE *>(lawChange[LawChange::BPFRACWAGE]);
  lawChange[LawChange::BPCPI] =
    new LawChange("bend points increasing at CPI rate");
  lawChange[LawChange::EARNINDCPI] =
    new LawChange("earnings indexed to CPI rate");
  lawChange[LawChange::NEWFORMULA] = new LawChangeNEWFORMULA();
  lawChangeNEWFORMULA =
    dynamic_cast<LawChangeNEWFORMULA *>(lawChange[LawChange::NEWFORMULA]);
  lawChange[LawChange::BPSPECRATE] = new LawChangeBPSPECRATE();
  lawChangeBPSPECRATE =
    dynamic_cast<LawChangeBPSPECRATE *>(lawChange[LawChange::BPSPECRATE]);
  lawChange[LawChange::BPMINCONST] = new LawChangeBPMINCONST();
  lawChangeBPMINCONST =
    dynamic_cast<LawChangeBPMINCONST *>(lawChange[LawChange::BPMINCONST]);
  lawChange[LawChange::AGE65COMP] = new LawChangeAGE65COMP();
  lawChangeAGE65COMP =
    dynamic_cast<LawChangeAGE65COMP *>(lawChange[LawChange::AGE65COMP]);
  lawChange[LawChange::DECLINEPERC] = new LawChangeDECLINEPERC();
  lawChangeDECLINEPERC =
    dynamic_cast<LawChangeDECLINEPERC *>(lawChange[LawChange::DECLINEPERC]);
  lawChange[LawChange::NOCPIELIG] =
    new LawChange("no benefit increase in year of eligibility");
  lawChange[LawChange::NOREINDWID] =
    new LawChange("no re-indexed widow guarantee");
  lawChange[LawChange::WINDFALL] =
    new LawChange("no windfall elimination provision");
  lawChange[LawChange::NEWSPECMIN] = new LawChangeNEWSPECMIN();
  lawChangeNEWSPECMIN =
    dynamic_cast<LawChangeNEWSPECMIN *>(lawChange[LawChange::NEWSPECMIN]);
  lawChange[LawChange::NOOLDSTART] = new LawChange("no Old-Start");
  lawChange[LawChange::DIDROP5] = new LawChange("5-year DI dropout");
  lawChange[LawChange::TRANSGUAR1] =
    new LawChange("use earnings after age 62 for transitional guarantee");
  lawChange[LawChange::MARRLENGTH] = new LawChangeMARRLENGTH();
  lawChangeMARRLENGTH =
    dynamic_cast<LawChangeMARRLENGTH *>(lawChange[LawChange::MARRLENGTH]);
  lawChange[LawChange::TRANSGUAR3] =
    new LawChange("use average of type-16 and aime for transitional guar");
  lawChange[LawChange::ALLEARN] = new LawChangeALLEARN();
  lawChangeALLEARN =
    dynamic_cast<LawChangeALLEARN *>(lawChange[LawChange::ALLEARN]);
  lawChange[LawChange::DROPOUTCHG] = new LawChangeDROPOUTCHG();
  lawChangeDROPOUTCHG =
    dynamic_cast<LawChangeDROPOUTCHG *>(lawChange[LawChange::DROPOUTCHG]);
  lawChange[LawChange::RETTESTCHG] =
    new LawChange("retirement test ad hoc change");
  lawChange[LawChange::SANFORD1] =
    new LawChange("original Sanford proposal (3% to retirement)");
  lawChange[LawChange::NOPIATABLE] = new LawChange("no PIA table method");
  lawChange[LawChange::NOTRANSGUAR] =
    new LawChange("no transitional guarantee");
  lawChange[LawChange::TAXRATECHG] = new LawChangeTAXRATECHG();
  lawChangeTAXRATECHG =
    dynamic_cast<LawChangeTAXRATECHG *>(lawChange[LawChange::TAXRATECHG]);
  lawChange[LawChange::SANFORD2] = new LawChange(
    "second Sanford notch proposal (3.25% to age 62 and red yrs)");
  lawChange[LawChange::NRACHANGE] = new LawChangeNRACHANGE();
  lawChangeNRACHANGE =
    dynamic_cast<LawChangeNRACHANGE *>(lawChange[LawChange::NRACHANGE]);
  lawChange[LawChange::CHILDCARECREDIT] = new LawChangeCHILDCARECREDIT();
  lawChangeCHILDCARECREDIT =
    dynamic_cast<LawChangeCHILDCARECREDIT *>(
    lawChange[LawChange::CHILDCARECREDIT]);
  lawChange[LawChange::RETROWAGEIND] =
    new LawChange("retroactive wage indexing");
  lawChange[LawChange::TRANSGUAR4] =
    new LawChange("Corman-Myers COLA limitation (notch proposal)");
  lawChange[LawChange::WIFEFACTOR] = new LawChangeWIFEFACTOR();
  lawChangeWIFEFACTOR =
    dynamic_cast<LawChangeWIFEFACTOR *>(lawChange[LawChange::WIFEFACTOR]);
  lawChange[LawChange::WIDFACTOR] = new LawChangeWIDFACTOR();
  lawChangeWIDFACTOR =
    dynamic_cast<LawChangeWIDFACTOR *>(lawChange[LawChange::WIDFACTOR]);
  lawChange[LawChange::TAXBENCHG] = new LawChangeTAXBENCHG();
  lawChangeTAXBENCHG =
    dynamic_cast<LawChangeTAXBENCHG *>(lawChange[LawChange::TAXBENCHG]);
  lawChange[LawChange::PSAACCT] = new LawChangePSAACCT();
  lawChangePSAACCT =
    dynamic_cast<LawChangePSAACCT *>(lawChange[LawChange::PSAACCT]);
  lawChange[LawChange::WAGEBASECHG] = new LawChangeWAGEBASECHG();
  lawChangeWAGEBASECHG =
    dynamic_cast<LawChangeWAGEBASECHG *>(lawChange[LawChange::WAGEBASECHG]);
  lawChange[LawChange::STATELOCAL] =
    new LawChange("coverage of State and local workers");
  lawChange[LawChange::FEDERAL] =
    new LawChange("coverage of Federal workers");
  lawChange[LawChange::CHILDCAREDROPOUT] = new LawChangeCHILDCAREDROPOUT();
  lawChangeCHILDCAREDROPOUT =
    dynamic_cast<LawChangeCHILDCAREDROPOUT *>(
    lawChange[LawChange::CHILDCAREDROPOUT]);
  lawChange[LawChange::COLACHANGE] = new LawChangeCOLACHANGE();
  lawChangeCOLACHANGE =
    dynamic_cast<LawChangeCOLACHANGE *>(lawChange[LawChange::COLACHANGE]);
  lawChange[LawChange::NODIBGUAR] = new LawChange("No DIB guarantee");
}

/// <summary>Destructor.</summary>
///
/// <remarks>Deletes all of the <see cref="LawChange"/>'s.</remarks>
LawChangeArray::~LawChangeArray()
{
  for (int i = 0; i < LawChange::MAXLCH; i++) {
    delete lawChange[i];
  }
}

/// <summary>Calculates total changes in law.</summary>
void LawChangeArray::indTotalCal()
{
  indTotal = 0;
  for (int i1 = 0; i1 < LawChange::MAXLCH; i1++) {
    indTotal += lawChange[i1]->getInd();
  }
}
