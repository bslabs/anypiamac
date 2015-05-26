// Declarations for the <see cref="LawChangeArray"/> class to manage an array
// of <see cref="LawChange"/> parameters.
//
// $Id: LawChangeArray.h 1.12 2011/07/29 16:27:33EDT 044579 Development  $

#pragma once

#include <vector>
#include "document.h"
#include "LawChange.h"
class LawChangeBPFRACWAGE;
class LawChangeNEWFORMULA;
class LawChangeDECLINEPERC;
class LawChangeBPSPECRATE;
class LawChangeBPMINCONST;
class LawChangeAGE65COMP;
class LawChangeDROPOUTCHG;
class LawChangeALLEARN;
class LawChangeMARRLENGTH;
class LawChangeNEWSPECMIN;
class LawChangeTAXRATECHG;
class LawChangeNRACHANGE;
class LawChangeWIFEFACTOR;
class LawChangeWIDFACTOR;
class LawChangeCHILDCARECREDIT;
class LawChangeCHILDCAREDROPOUT;
class LawChangeTAXBENCHG;
class LawChangeWAGEBASECHG;
class LawChangeCOLACHANGE;
class LawChangePSAACCT;

/// <summary>Manages an array of <see cref="LawChange"/>'s.</summary>
///
/// <remarks>Manages an array of <see cref="LawChange"/>'s.</remarks>
class LawChangeArray : public Document
{
public:
  /// <summary>Pointers to the law changes.</summary>
  std::vector<LawChange *> lawChange;
  /// <summary>Pointer to <see cref="LawChangeBPFRACWAGE"/>.</summary>
  LawChangeBPFRACWAGE *lawChangeBPFRACWAGE;
  /// <summary>Pointer to <see cref="LawChangeNEWFORMULA"/>.</summary>
  LawChangeNEWFORMULA *lawChangeNEWFORMULA;
  /// <summary>Pointer to <see cref="LawChangeDECLINEPERC"/>.</summary>
  LawChangeDECLINEPERC *lawChangeDECLINEPERC;
  /// <summary>Pointer to <see cref="LawChangeBPSPECRATE"/>.</summary>
  LawChangeBPSPECRATE *lawChangeBPSPECRATE;
  /// <summary>Pointer to <see cref="LawChangeBPMINCONST"/>.</summary>
  LawChangeBPMINCONST *lawChangeBPMINCONST;
  /// <summary>Pointer to <see cref="LawChangeAGE65COMP"/>.</summary>
  LawChangeAGE65COMP *lawChangeAGE65COMP;
  /// <summary>Pointer to <see cref="LawChangeDROPOUTCHG"/>.</summary>
  LawChangeDROPOUTCHG *lawChangeDROPOUTCHG;
  /// <summary>Pointer to <see cref="LawChangeALLEARN"/>.</summary>
  LawChangeALLEARN *lawChangeALLEARN;
  /// <summary>Pointer to <see cref="LawChangeMARRLENGTH"/>.</summary>
  LawChangeMARRLENGTH *lawChangeMARRLENGTH;
  /// <summary>Pointer to <see cref="LawChangeNEWSPECMIN"/>.</summary>
  LawChangeNEWSPECMIN *lawChangeNEWSPECMIN;
  /// <summary>Pointer to <see cref="LawChangeTAXRATECHG"/>.</summary>
  LawChangeTAXRATECHG *lawChangeTAXRATECHG;
  /// <summary>Pointer to <see cref="LawChangeNRACHANGE"/>.</summary>
  LawChangeNRACHANGE *lawChangeNRACHANGE;
  /// <summary>Pointer to <see cref="LawChangeWIFEFACTOR"/>.</summary>
  LawChangeWIFEFACTOR *lawChangeWIFEFACTOR;
  /// <summary>Pointer to <see cref="LawChangeWIDFACTOR"/>.</summary>
  LawChangeWIDFACTOR *lawChangeWIDFACTOR;
  /// <summary>Pointer to <see cref="LawChangeCHILDCARECREDIT"/>.</summary>
  LawChangeCHILDCARECREDIT *lawChangeCHILDCARECREDIT;
  /// <summary>Pointer to <see cref="LawChangeCHILDCAREDROPOUT"/>.</summary>
  LawChangeCHILDCAREDROPOUT *lawChangeCHILDCAREDROPOUT;
  /// <summary>Pointer to <see cref="LawChangeTAXBENCHG"/>.</summary>
  LawChangeTAXBENCHG *lawChangeTAXBENCHG;
  /// <summary>Pointer to <see cref="LawChangeWAGEBASECHG"/>.</summary>
  LawChangeWAGEBASECHG *lawChangeWAGEBASECHG;
  /// <summary>Pointer to <see cref="LawChangeCOLACHANGE"/>.</summary>
  LawChangeCOLACHANGE *lawChangeCOLACHANGE;
  /// <summary>Pointer to <see cref="LawChangePSAACCT"/>.</summary>
  LawChangePSAACCT *lawChangePSAACCT;
private:
  /// <summary>Total of indicators.</summary>
  int indTotal;
public:
  LawChangeArray( int startt, int newMaxyear,
    const std::string& newDirectory );
  ~LawChangeArray();
  /// <summary>Returns total of law-change indicators (is greater than 0 if
  /// any law changes).</summary>
  ///
  /// <returns>Total of law-change indicators (is greater than 0 if any law
  /// changes).</returns>
  int getIndTotal() const { return(indTotal); }
  void indTotalCal();
};
