// $Id: CAnnearnDialog.cp,v 1.4 2010/08/16 18:18:05 djolson Exp $
//  CAnnearnDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>
#include <LGroupBox.h>

#include "CTextView.h"
#include "CAnnearnDialog.h"
#include "CSpecYearDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "cminmax.h"
#include "comma.h"
#include "FormatString.h"
#include "Resource.h"
#include "piaparmsAny.h"
#include "WageBase.h"
#include "wrkrdata.h"

using namespace std;

//  Create a dialog from a stream

CAnnearnDialog *CAnnearnDialog::CreateAnnearnDialogStream(
   LStream *inStream )
{
   return new CAnnearnDialog( inStream );
}

//  Create a dialog from a stream

CAnnearnDialog::CAnnearnDialog( LStream *inStream ) :
   LDialogBox( inStream ),
   mDAoasdi(YEAR37, WorkerData::getMaxyear()),
   mDAhi(YEAR37, WorkerData::getMaxyear()),
   mDAbase(YEAR37, WorkerData::getMaxyear()),
   mDAfq(YEAR37, WorkerData::getMaxyear()),
   mDAwaghigh(YEAR37, WorkerData::getMaxyear()),
   mDAwagmin(YEAR37, WorkerData::getMaxyear()),
   mIAearntype(YEAR37, WorkerData::getMaxyear()),
   mIAtaxtype(YEAR37, WorkerData::getMaxyear()),
   mIAqc(YEAR37, WorkerData::getMaxyear()),
   mDArrearn(YEAR37, WorkerData::getMaxyear()),
   mIArrqc(YEAR37, WorkerData::getMaxyear()),
   mBAchildcare(YEAR37, WorkerData::getMaxyear())
{ }

//  Destroy a dialog

CAnnearnDialog::~CAnnearnDialog()
{ }

//  Set menu status

void CAnnearnDialog::FindCommandStatus(
   CommandT  inCommand,
   Boolean   &outEnabled,
   Boolean&  /* outUsesMark */,
   UInt16&   /* outMark */,
   Str255    /* outName */ )
{
   outEnabled = false;
   if ( inCommand == cmd_About ) {
      outEnabled = true;
   }
}

//  Set up dialog

void CAnnearnDialog::SetupDialog()
{
   StopListening();
   const WorkerData& workerData = *(theDoc->workerData);
   PiaParamsAny * piaparms = theDoc->piaparms;
   EarnProject *earnProject = theDoc->earnProject;
   mStartyear = earnProject->getFirstYear();
   mLastyear = earnProject->getLastYear();
   mStartyearRR = workerData.railRoadData.getFirstYear();
   if (workerData.railRoadData.getEarn3750() > 0.0 || theDoc->hasrr3750)
      mStartyearRR = 1950;
   mLastyearRR = workerData.railRoadData.getLastYear();
   if (mStartyearRR == 1950 && mLastyearRR < 1950)
      mLastyearRR = 1950;
   mQcbyyear = workerData.hasQcsByYear();
   mMqge = workerData.getMqge();
   mbPebes = (workerData.getJoasdi() == WorkerData::PEBS_CALC);
   mIndrr = workerData.getIndrr();
   mIndearn = workerData.getIndearn();
   if (mIndearn) {
      for (int i = mStartyear; i <= mLastyear; i++) {
         mDAoasdi[i] = earnProject->earnpebs[i];
         mDAhi[i] = workerData.getEarnHi(i);
         mIAearntype[i] = earnProject->getEarntype()[i];
         mIAtaxtype[i] = workerData.getTaxType(i);
         mIAqc[i] = workerData.qc.get(i);
         mDAbase[i] = piaparms->getBaseOasdi(i);
         mDAfq[i] = piaparms->getFq(i);
         mDAwaghigh[i] = piaparms->getWageHigh(i);
         mDAwagmin[i] = piaparms->getWageMin(i);
         mBAchildcare.setBit(i, workerData.childCareYears.getBit(i));
      }
   }
   if (mIndrr) {
      if (mStartyearRR == 1950) {
         mDArrearn[1950] = workerData.railRoadData.getEarn3750();
         mIArrqc[1950] = workerData.railRoadData.getQc3750();
      }
      for (int i = _MAX(1951, mStartyearRR); i <= mLastyearRR; i++) {
         mDArrearn[i] = workerData.railRoadData.earningsArray[i];
         mIArrqc[i] = workerData.railRoadData.qcArray.get(i);
      }
   }
   if (mIndearn) {
      mStartyear1 = mStartyear;
      mLastyear1 = mLastyear;
      if (mIndrr && mStartyear1 > mStartyearRR)
         mStartyear1 = mStartyearRR;
      if (mIndrr && mLastyear1 < mLastyearRR)
         mLastyear1 = mLastyearRR;
   }
   else {
      mStartyear1 = mStartyearRR;
      mLastyear1 = mLastyearRR;
   }
   mEarnyear = mStartyear1;
   setupdata();
   StartListening();
}

// Fill in strings in dialog box, and enable/disable selections

void CAnnearnDialog::setupdata()
{
   LEditField *theTextBox;  // pointer to dialog text box
   LStdRadioButton *theEnteredRadio;  // pointer to Entered radio button
   LStdRadioButton *theMaximumRadio;  // pointer to Maximum radio button
   LStdRadioButton *theHighRadio;  // pointer to High radio button
   LStdRadioButton *theAverageRadio;  // pointer to Average radio button
   LStdRadioButton *theLowRadio;  // pointer to Low radio button
   LStdButton *thePrevyearButton;  // handle to previous year button
   LStdButton *theNextyearButton;  // handle to next year button

   mRRearn = mDArrearn[mEarnyear];
   bool indrr = (mIndrr && mEarnyear >= mStartyearRR &&
      mEarnyear <= mLastyearRR);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kAneD_RRearnTextBox));
   LStr255 temp1(poutNoComma(mRRearn, 4, 2).c_str());
   theTextBox->SetDescriptor(temp1);
   if (indrr)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   mRRqc = mIArrqc[mEarnyear];
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kAneD_RRQcTextBox));
   LStr255 temp2(poutNoComma(mRRqc).c_str());
   theTextBox->SetDescriptor(temp2);
   if (indrr && mEarnyear < 1978)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   mChildcare = mBAchildcare.getBit(mEarnyear);
   SetValueForPaneID(kAneD_ChildcareCheckBox,
      mChildcare ? Button_On : Button_Off);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kAneD_EarnyearTextBox));
   LStr255 temp3(poutNoComma(mEarnyear).c_str());
   theTextBox->SetDescriptor(temp3);
   theTextBox->Disable();
   mEarnoasdi = mDAoasdi[mEarnyear];
   mEarnhi = mDAhi[mEarnyear];
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kAneD_EarnhiTextBox));
   LStr255 temp5(poutNoComma(mEarnhi, 4, 2).c_str());
   theTextBox->SetDescriptor(temp5);
   if (mMqge)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   mQcann = mIAqc[mEarnyear];
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kAneD_QcannTextBox));
   LStr255 temp6(poutNoComma(mQcann).c_str());
   theTextBox->SetDescriptor(temp6);
   if (mQcbyyear && mEarnyear < 1978)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   mEarntype = mIAearntype[mEarnyear];
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kAneD_EarnoasdiTextBox));
   LStr255 temp4(poutNoComma(mEarnoasdi, 4, 2).c_str());
   theTextBox->SetDescriptor(temp4);
   theEnteredRadio =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kAneD_EnteredRadio));
   theMaximumRadio =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kAneD_MaximumRadio));
   theHighRadio =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kAneD_HighRadio));
   theAverageRadio =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kAneD_AverageRadio));
   theLowRadio =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kAneD_LowRadio));
   if (mIndearn && mStartyear <= mEarnyear && mEarnyear <= mLastyear) {
      theEnteredRadio->Enable();
      if (mbPebes) {
         SetValueForPaneID(kAneD_EnteredRadio, Button_On);
         theTextBox->Enable();
         SetValueForPaneID(kAneD_MaximumRadio, Button_Off);
         theMaximumRadio->Disable();
         SetValueForPaneID(kAneD_HighRadio, Button_Off);
         theHighRadio->Disable();
         SetValueForPaneID(kAneD_AverageRadio, Button_Off);
         theAverageRadio->Disable();
         SetValueForPaneID(kAneD_LowRadio, Button_Off);
         theLowRadio->Disable();
      }
      else {
         if (mEarntype == EarnProject::ENTERED)
            theTextBox->Enable();
         else
            theTextBox->Disable();
         theMaximumRadio->Enable();
         theHighRadio->Enable();
         theAverageRadio->Enable();
         theLowRadio->Enable();
         SetValueForPaneID(kAneD_EnteredRadio,
            (mEarntype == EarnProject::ENTERED) ? Button_On : Button_Off);
         SetValueForPaneID(kAneD_MaximumRadio,
            (mEarntype == EarnProject::MAXIMUM) ? Button_On : Button_Off);
         SetValueForPaneID(kAneD_HighRadio,
            (mEarntype == EarnProject::HIGH) ? Button_On : Button_Off);
         SetValueForPaneID(kAneD_AverageRadio,
            (mEarntype == EarnProject::AVERAGE) ? Button_On : Button_Off);
         SetValueForPaneID(kAneD_LowRadio,
            (mEarntype == EarnProject::LOW) ? Button_On : Button_Off);
      }
   }
   // if no regular earnings in this year, disable windows
   else {
      theTextBox->Disable();
      theEnteredRadio->Disable();
      theMaximumRadio->Disable();
      theHighRadio->Disable();
      theAverageRadio->Disable();
      theLowRadio->Disable();
   }
   mTaxtype = mIAtaxtype[mEarnyear];
   SetValueForPaneID(kAneD_EmployeeRadio,
      (mTaxtype == 0) ? Button_On : Button_Off);
   SetValueForPaneID(kAneD_SelfempRadio,
      (mTaxtype == 1) ? Button_On : Button_Off);
   string highstring;
   AfxFormatString1(highstring, PIA_IDS_MAXIMUMTEXT,
      poutNoComma(mDAbase[mEarnyear], 3, 2).c_str());
   LStr255 temp7(highstring.c_str());
   theMaximumRadio->SetDescriptor(temp7);
   AfxFormatString1(highstring, PIA_IDS_HIGHTEXT,
      poutNoComma(mDAwaghigh[mEarnyear], 3, 2).c_str());
   LStr255 temp8(highstring.c_str());
   theHighRadio->SetDescriptor(temp8);
   AfxFormatString1(highstring, PIA_IDS_AVERAGETEXT,
      poutNoComma(mDAfq[mEarnyear], 3, 2).c_str());
   LStr255 temp9(highstring.c_str());
   theAverageRadio->SetDescriptor(temp9);
   AfxFormatString1(highstring, PIA_IDS_LOWTEXT,
      poutNoComma(mDAwagmin[mEarnyear], 3, 2).c_str());
   LStr255 temp10(highstring.c_str());
   theLowRadio->SetDescriptor(temp10);
   thePrevyearButton =
      dynamic_cast<LStdButton *>(FindPaneByID(kAneD_PrevyearButton));
   theNextyearButton =
      dynamic_cast<LStdButton *>(FindPaneByID(kAneD_NextyearButton));
   if (mEarnyear <= mStartyear1)
      thePrevyearButton->Disable();
   else
      thePrevyearButton->Enable();
   if (mEarnyear >= mLastyear1)
      theNextyearButton->Disable();
   else
      theNextyearButton->Enable();
}

//  Listen to messages

void CAnnearnDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_AneD_OKButton:
      {
         if (check())
            break;
         // Values are good, so go back to top level
         LDialogBox::ListenToMessage( inMessage, ioParam );
      }
      break;

      case cmd_AneD_PrevyearButton:
      {
         if (check())
            break;
         if (mEarnyear > mStartyear1)
            mEarnyear--;
         StopListening();
         setupdata();
         StartListening();
      }
      break;

      case cmd_AneD_SpecyearButton:
      {
         if (check())
            break;
         CSpecYearDialog * theDialog;
         theDialog = dynamic_cast<CSpecYearDialog *>
            (LWindow::CreateWindow( rPPob_SpecYearDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theEarningsDialog = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_AneD_NextyearButton:
      {
         if (check())
            break;
         if (mEarnyear < mLastyear1)
            mEarnyear++;
         StopListening();
         setupdata();
         StartListening();
      }
      break;

      case cmd_AneD_EnteredRadio:
      {
         LEditField *theTextBox;  // pointer to earnings text box
         mEarntype = EarnProject::ENTERED;
         StopListening();
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kAneD_EarnoasdiTextBox));
         theTextBox->Enable();
         SetValueForPaneID(kAneD_MaximumRadio, Button_Off);
         SetValueForPaneID(kAneD_HighRadio, Button_Off);
         SetValueForPaneID(kAneD_AverageRadio, Button_Off);
         SetValueForPaneID(kAneD_LowRadio, Button_Off);
         StartListening();
      }
      break;
   
      case cmd_AneD_MaximumRadio:
      {
         LEditField *theTextBox;  // pointer to earnings text box
         mEarntype = EarnProject::MAXIMUM;
         StopListening();
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kAneD_EarnoasdiTextBox));
         theTextBox->Disable();
         SetValueForPaneID(kAneD_EnteredRadio, Button_Off);
         SetValueForPaneID(kAneD_HighRadio, Button_Off);
         SetValueForPaneID(kAneD_AverageRadio, Button_Off);
         SetValueForPaneID(kAneD_LowRadio, Button_Off);
         StartListening();
      }
      break;
   
      case cmd_AneD_HighRadio:
      {
         LEditField *theTextBox;  // pointer to earnings text box
         mEarntype = EarnProject::HIGH;
         StopListening();
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kAneD_EarnoasdiTextBox));
         theTextBox->Disable();
         SetValueForPaneID(kAneD_EnteredRadio, Button_Off);
         SetValueForPaneID(kAneD_MaximumRadio, Button_Off);
         SetValueForPaneID(kAneD_AverageRadio, Button_Off);
         SetValueForPaneID(kAneD_LowRadio, Button_Off);
         StartListening();
      }
      break;
   
      case cmd_AneD_AverageRadio:
      {
         LEditField *theTextBox;  // pointer to earnings text box
         mEarntype = EarnProject::AVERAGE;
         StopListening();
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kAneD_EarnoasdiTextBox));
         theTextBox->Disable();
         SetValueForPaneID(kAneD_EnteredRadio, Button_Off);
         SetValueForPaneID(kAneD_MaximumRadio, Button_Off);
         SetValueForPaneID(kAneD_HighRadio, Button_Off);
         SetValueForPaneID(kAneD_LowRadio, Button_Off);
         StartListening();
      }
      break;
   
      case cmd_AneD_LowRadio:
      {
         LEditField *theTextBox;  // pointer to earnings text box
         mEarntype = EarnProject::LOW;
         StopListening();
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kAneD_EarnoasdiTextBox));
         theTextBox->Disable();
         SetValueForPaneID(kAneD_EnteredRadio, Button_Off);
         SetValueForPaneID(kAneD_MaximumRadio, Button_Off);
         SetValueForPaneID(kAneD_HighRadio, Button_Off);
         SetValueForPaneID(kAneD_AverageRadio, Button_Off);
         StartListening();
      }
      break;
   
      case cmd_AneD_EmployeeRadio:
      {
         mTaxtype = 0;
         StopListening();
         SetValueForPaneID(kAneD_SelfempRadio, Button_Off);
         StartListening();
      }
      break;
   
      case cmd_AneD_SelfempRadio:
      {
         mTaxtype = 1;
         StopListening();
         SetValueForPaneID(kAneD_EmployeeRadio, Button_Off);
         StartListening();
      }
      break;
      
      case cmd_AneD_ChildcareCheckBox:
      {
         mChildcare =
            (GetValueForPaneID(kAneD_ChildcareCheckBox) == Button_On ) ?
            true : false;
      }
      break;

      default:
      {
         LDialogBox::ListenToMessage( inMessage, ioParam );
         break;
      }
   }
}

//  Connect to controls

void CAnnearnDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_AnnearnDialog );
}

//  Save results from dialog

void CAnnearnDialog::SaveResults()
{
   try {
      CTextView *theTextView = theDoc->getTextView();
      WorkerData& workerData = *(theDoc->workerData);
      EarnProject *earnProject = theDoc->earnProject;
      if (mIndearn) {
         for (int i = mStartyear; i <= mLastyear; i++) {
            if (workerData.childCareYears.getBit(i) != mBAchildcare.getBit(i)) {
               workerData.childCareYears.setBit(i, mBAchildcare.getBit(i));
               theDoc->valid = false;
               theTextView->SetDirty(true);
            }
            if (earnProject->earnpebs[i] != mDAoasdi[i]) {
               earnProject->earnpebs[i] = mDAoasdi[i];
               theDoc->valid = false;
               theTextView->SetDirty(true);
            }
            if (mMqge && workerData.getEarnHi(i) != mDAhi[i]) {
               workerData.setEarnHi(i, mDAhi[i]);
               theDoc->valid = false;
               theTextView->SetDirty(true);
            }
            if (earnProject->getEarntype()[i] != mIAearntype[i]) {
               earnProject->setEarntype(i, mIAearntype[i]);
               theDoc->valid = false;
               theTextView->SetDirty(true);
            }
            if (workerData.getTaxType(i) != mIAtaxtype[i]) {
               workerData.setTaxType(i, mIAtaxtype[i]);
               theDoc->valid = false;
               theTextView->SetDirty(true);
            }
            if (mQcbyyear && i < 1978 &&
               workerData.qc.get(i) != mIAqc[i]) {
               workerData.qc.set(i, mIAqc[i]);
               theDoc->valid = false;
               theTextView->SetDirty(true);
            }
         }
      }
      if (mIndrr) {
         if (mStartyearRR == 1950) {
            workerData.railRoadData.setEarn3750(mDArrearn[1950]);
            workerData.railRoadData.setQc3750(mIArrqc[1950]);
         }
         for (int i = _MAX(1951, mStartyearRR);
            i <= mLastyearRR; i++) {
            if (workerData.railRoadData.earningsArray[i] != mDArrearn[i]) {
               workerData.railRoadData.earningsArray[i] = mDArrearn[i];
               theDoc->valid = false;
               theTextView->SetDirty(true);
            }
            if (i < 1978 &&
               workerData.railRoadData.qcArray.get(i) != mIArrqc[i]) {
               workerData.railRoadData.qcArray.set(i, mIArrqc[i]);
               theDoc->valid = false;
               theTextView->SetDirty(true);
            }
         }
      }
      if (mQcbyyear) {
         int qctdt = workerData.qc.accumulate(1937, workerData.lastQcyr(), 0u);
         if (workerData.getQctottd() != qctdt)
            workerData.setQctd(qctdt);
         int qc51tdt = workerData.qc.accumulate(1951, workerData.lastQcyr(), 0u);
         if (workerData.getQctot51td() != qc51tdt)
            workerData.setQc51td(qc51tdt);
      }
      if (workerData.getJoasdi() == WorkerData::PEBS_CALC) {
         theDoc->piacal->pebsOabCal();
         theDoc->pebsOut->pebsWorkerInfo.prepareStrings();
      }
      else
         theDoc->piaOut->summary1Page.prepareStrings();
   } catch (PiaException& e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CAnnearnDialog::SaveResults");
   }
}

// Check data for inconsistencies
// Returns true if there is a problem, false otherwise

bool CAnnearnDialog::check()
{
   try {
      mRRearn = atof(getTextBoxString(kAneD_RRearnTextBox).c_str());
      mRRqc = atoi(getTextBoxString(kAneD_RRQcTextBox).c_str());
      mEarnoasdi = atof(getTextBoxString(kAneD_EarnoasdiTextBox).c_str());
      mEarnhi = atof(getTextBoxString(kAneD_EarnhiTextBox).c_str());
      mQcann = atoi(getTextBoxString(kAneD_QcannTextBox).c_str());
      // save railroad amounts
      if (mIndrr && mStartyearRR <= mEarnyear && mEarnyear <= mLastyearRR) {
         EarnProject::earnpebsCheck(mRRearn);
         mDArrearn[mEarnyear] = mRRearn;
         Qc::check(mRRqc);
         mIArrqc[mEarnyear] = mRRqc;
      }
      // save regular earnings
      if (mIndearn && mStartyear <= mEarnyear && mEarnyear <= mLastyear) {
         EarnProject::earnpebsCheck(mEarnoasdi);
         mDAoasdi[mEarnyear] = mEarnoasdi;
         EarnProject::earnpebsCheck(mEarnhi);
         mDAhi[mEarnyear] = mEarnhi;
         Qc::check(mQcann);
         mIAqc[mEarnyear] = mQcann;
         EarnProject::earntypeCheck(mEarntype);
         mIAearntype[mEarnyear] = mEarntype;
         WorkerData::taxTypeCheck(mTaxtype);
         mIAtaxtype[mEarnyear] = mTaxtype;
         mBAchildcare.setBit(mEarnyear, mChildcare);
      }
      return false;
   } catch (PiaException& e) {
      messageDialog(e.getNumber(), this);
      return true;
   }
}

// Get string in a text box

string CAnnearnDialog::getTextBoxString( long textBoxID )
{
   LEditField *theTextBox;  // handle to text box
   Str255 thePString;  // Pascal string to get text box result
   char theCString[256];  // holds converted Pascal string

   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(textBoxID));
   theTextBox->GetDescriptor(thePString);
   strncpy(theCString, (const char *)(thePString + 1),
      (unsigned long)thePString[0]);
   theCString[thePString[0]] = '\0';
   return string(theCString);
}
