// $Id: CAssumptionsDialog.cp,v 1.4 2010/08/16 18:18:05 djolson Exp $
//  CAssumptionsDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CTextView.h"
#include "CAssumptionsDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "UserAssumptions.h"
#include "piaparmsAny.h"
#include "wrkrdata.h"

using namespace std;

//  Create a dialog from a stream

CAssumptionsDialog *CAssumptionsDialog::CreateAssumptionsDialogStream(
   LStream *inStream )
{
   return new CAssumptionsDialog( inStream );
}

//  Create a dialog from a stream

CAssumptionsDialog::CAssumptionsDialog( LStream *inStream ) :
   LDialogBox( inStream )
{ }

//  Destroy a dialog

CAssumptionsDialog::~CAssumptionsDialog()
{ }

//  Set menu status

void CAssumptionsDialog::FindCommandStatus(
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

void CAssumptionsDialog::SetupDialog()
{
   LStdRadioButton *theWagebaseAutoRadio;
   LStdRadioButton *theWagebaseAdhocRadio;
   LStdRadioButton *theRadioButton;
   LEditField *theTextBox;
   LStdCheckBox *theCheckBox;

   const WorkerData& workerData = *(theDoc->workerData);
   UserAssumptions *userAssumptions = theDoc->userAssumptions;
   PiaParams *piaParams = theDoc->piaparms;
   StopListening();
   mIaltbi = userAssumptions->getIaltbi();
   mIaltaw = userAssumptions->getIaltaw();
   for (int i = 0; i < 6; i++) {
      mBeninc[i] = theDoc->assumptions->getBenefitInc(i+1);
      mAvgwage[i] = theDoc->assumptions->getAverageWage(i+1);
   }
   mBitext = userAssumptions->titleBi;
   mAwtext = userAssumptions->titleAw;
   mBases = userAssumptions->getIbasch();
   mbCachup = (userAssumptions->getAnscch() == 'Y');
   mbBases = workerData.need_bases(userAssumptions->getIstart());
   SetValueForPaneID(kAssD_WagebaseAutoRadio,
      (mBases == 1) ? Button_On : Button_Off);
   SetValueForPaneID(kAssD_WagebaseAdhocRadio,
      (mBases == 2) ? Button_On : Button_Off);
   theWagebaseAutoRadio =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kAssD_WagebaseAutoRadio));
   theWagebaseAdhocRadio =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kAssD_WagebaseAdhocRadio));
   if (mbBases) {
      theWagebaseAutoRadio->Enable();
      theWagebaseAdhocRadio->Enable();
   }
   else {
      theWagebaseAutoRadio->Disable();
      theWagebaseAdhocRadio->Disable();
   }
   theRadioButton =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kAssD_Beninc1Radio));
   LStr255 temp1(mBeninc[0].c_str());
   theRadioButton->SetDescriptor(temp1);
   SetValueForPaneID(kAssD_Beninc1Radio,
      (mIaltbi == AssumptionType::ALTERN_I) ? Button_On : Button_Off);
   theRadioButton =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kAssD_Beninc2Radio));
   LStr255 temp2(mBeninc[1].c_str());
   theRadioButton->SetDescriptor(temp2);
   SetValueForPaneID(kAssD_Beninc2Radio,
      (mIaltbi == AssumptionType::ALTERN_IIB) ? Button_On : Button_Off);
   theRadioButton =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kAssD_Beninc3Radio));
   LStr255 temp3(mBeninc[2].c_str());
   theRadioButton->SetDescriptor(temp3);
   SetValueForPaneID(kAssD_Beninc3Radio,
      (mIaltbi == AssumptionType::ALTERN_III) ? Button_On : Button_Off);
   //theRadioButton =
   //   dynamic_cast<LStdRadioButton *>(FindPaneByID(kAssD_Beninc4Radio));
   //LStr255 temp4(mBeninc[3].c_str());
   //theRadioButton->SetDescriptor(temp4);
   //SetValueForPaneID(kAssD_Beninc4Radio,
   //   (mIaltbi == AssumptionType::ALTERN_IIA) ? Button_On : Button_Off);
   theRadioButton =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kAssD_Beninc5Radio));
   LStr255 temp5(mBeninc[4].c_str());
   theRadioButton->SetDescriptor(temp5);
   SetValueForPaneID(kAssD_Beninc5Radio,
      (mIaltbi == AssumptionType::FLAT) ? Button_On : Button_Off);
   //theRadioButton =
   //   dynamic_cast<LStdRadioButton *>(FindPaneByID(kAssD_Beninc6Radio));
   //LStr255 temp6(mBeninc[5].c_str());
   //theRadioButton->SetDescriptor(temp6);
   //SetValueForPaneID(kAssD_Beninc6Radio,
   //   (mIaltbi == AssumptionType::PEBS_ASSUM) ? Button_On : Button_Off);
   SetValueForPaneID(kAssD_Beninc7Radio,
      (mIaltbi == AssumptionType::OTHER_ASSUM) ? Button_On : Button_Off);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kAssD_BenincTextBox));
   LStr255 temp7(mBitext.c_str());
   theTextBox->SetDescriptor(temp7);
   theCheckBox =
      dynamic_cast<LStdCheckBox *>(FindPaneByID(kAssD_CatchupCheckBox));
   if (mIaltbi == AssumptionType::OTHER_ASSUM) {
      theTextBox->Enable();
      theCheckBox->Enable();
   }
   else {
      theTextBox->Disable();
      theCheckBox->Disable();
   }
   theRadioButton =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kAssD_Avgwage1Radio));
   LStr255 temp8(mAvgwage[0].c_str());
   theRadioButton->SetDescriptor(temp8);
   SetValueForPaneID(kAssD_Avgwage1Radio,
      (mIaltaw == AssumptionType::ALTERN_I) ? Button_On : Button_Off);
   theRadioButton =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kAssD_Avgwage2Radio));
   LStr255 temp9(mAvgwage[1].c_str());
   theRadioButton->SetDescriptor(temp9);
   SetValueForPaneID(kAssD_Avgwage2Radio,
      (mIaltaw == AssumptionType::ALTERN_IIB) ? Button_On : Button_Off);
   theRadioButton =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kAssD_Avgwage3Radio));
   LStr255 temp10(mAvgwage[2].c_str());
   theRadioButton->SetDescriptor(temp10);
   SetValueForPaneID(kAssD_Avgwage3Radio,
      (mIaltaw == AssumptionType::ALTERN_III) ? Button_On : Button_Off);
   //theRadioButton =
   //   dynamic_cast<LStdRadioButton *>(FindPaneByID(kAssD_Avgwage4Radio));
   //LStr255 temp11(mAvgwage[3].c_str());
   //theRadioButton->SetDescriptor(temp11);
   //SetValueForPaneID(kAssD_Avgwage4Radio,
   //   (mIaltaw == AssumptionType::ALTERN_IIA) ? Button_On : Button_Off);
   theRadioButton =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kAssD_Avgwage5Radio));
   LStr255 temp12(mAvgwage[4].c_str());
   theRadioButton->SetDescriptor(temp12);
   SetValueForPaneID(kAssD_Avgwage5Radio,
      (mIaltaw == AssumptionType::FLAT) ? Button_On : Button_Off);
   //theRadioButton =
   //   dynamic_cast<LStdRadioButton *>(FindPaneByID(kAssD_Avgwage6Radio));
   //LStr255 temp13(mAvgwage[5].c_str());
   //theRadioButton->SetDescriptor(temp13);
   //SetValueForPaneID(kAssD_Avgwage6Radio,
   //   (mIaltaw == AssumptionType::PEBS_ASSUM) ? Button_On : Button_Off);
   SetValueForPaneID(kAssD_Avgwage7Radio,
      (mIaltaw == AssumptionType::OTHER_ASSUM) ? Button_On : Button_Off);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kAssD_AvgwageTextBox));
   LStr255 temp14(mAwtext.c_str());
   theTextBox->SetDescriptor(temp14);
   if (mIaltaw == AssumptionType::OTHER_ASSUM)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   mbCachup = (userAssumptions->getAnscch() == 'Y');
   SetValueForPaneID(kAssD_CatchupCheckBox,
      mbCachup ? Button_On : Button_Off);
   StartListening();
}

//  Listen to messages

void CAssumptionsDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_AssD_OKButton:
      {
         try {
            StopListening();
            PiaData *piadata = theDoc->piadata;
            mBitext = getTextBoxString(kAssD_BenincTextBox);
            mAwtext = getTextBoxString(kAssD_AvgwageTextBox);
            StartListening();
            // Values are good, so go back to top level
            LDialogBox::ListenToMessage( inMessage, ioParam );
         } catch (PiaException e) {
            StartListening();
            messageDialog(e.getNumber(), this);
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CAssumptionsDialog::ListenToMessage for cmd_AssD_OKButton");
         }
      }
      break;

      case cmd_AssD_Beninc1Radio:
      {
         mIaltbi = AssumptionType::ALTERN_I;
         mbCachup = false;
         StopListening();
         LEditField *theTextBox;
         LStdCheckBox *theCheckBox;
         SetValueForPaneID(kAssD_Beninc2Radio, Button_Off);
         SetValueForPaneID(kAssD_Beninc3Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc4Radio, Button_Off);
         SetValueForPaneID(kAssD_Beninc5Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc6Radio, Button_Off);
         SetValueForPaneID(kAssD_Beninc7Radio, Button_Off);
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kAssD_BenincTextBox));
         theCheckBox =
            dynamic_cast<LStdCheckBox *>(FindPaneByID(kAssD_CatchupCheckBox));
         theTextBox->Disable();
         theCheckBox->Disable();
         SetValueForPaneID(kAssD_CatchupCheckBox, Button_Off);
         StartListening();
      }
      break;

      case cmd_AssD_Beninc2Radio:
      {
         mIaltbi = AssumptionType::ALTERN_IIB;
         mbCachup = false;
         StopListening();
         LEditField *theTextBox;
         LStdCheckBox *theCheckBox;
         SetValueForPaneID(kAssD_Beninc1Radio, Button_Off);
         SetValueForPaneID(kAssD_Beninc3Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc4Radio, Button_Off);
         SetValueForPaneID(kAssD_Beninc5Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc6Radio, Button_Off);
         SetValueForPaneID(kAssD_Beninc7Radio, Button_Off);
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kAssD_BenincTextBox));
         theCheckBox =
            dynamic_cast<LStdCheckBox *>(FindPaneByID(kAssD_CatchupCheckBox));
         theTextBox->Disable();
         theCheckBox->Disable();
         SetValueForPaneID(kAssD_CatchupCheckBox, Button_Off);
         StartListening();
      }
      break;

      case cmd_AssD_Beninc3Radio:
      {
         mIaltbi = AssumptionType::ALTERN_III;
         mbCachup = false;
         StopListening();
         LEditField *theTextBox;
         LStdCheckBox *theCheckBox;
         SetValueForPaneID(kAssD_Beninc1Radio, Button_Off);
         SetValueForPaneID(kAssD_Beninc2Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc4Radio, Button_Off);
         SetValueForPaneID(kAssD_Beninc5Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc6Radio, Button_Off);
         SetValueForPaneID(kAssD_Beninc7Radio, Button_Off);
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kAssD_BenincTextBox));
         theCheckBox =
            dynamic_cast<LStdCheckBox *>(FindPaneByID(kAssD_CatchupCheckBox));
         theTextBox->Disable();
         theCheckBox->Disable();
         SetValueForPaneID(kAssD_CatchupCheckBox, Button_Off);
         StartListening();
      }
      break;

      case cmd_AssD_Beninc4Radio:
      {
         //mIaltbi = AssumptionType::ALTERN_IIA;
         //mbCachup = false;
         //StopListening();
         //LEditField *theTextBox;
         //LStdCheckBox *theCheckBox;
         //SetValueForPaneID(kAssD_Beninc1Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc2Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc3Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc5Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc6Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc7Radio, Button_Off);
         //theTextBox =
         //   dynamic_cast<LEditField *>(FindPaneByID(kAssD_BenincTextBox));
         //theCheckBox =
         //   dynamic_cast<LStdCheckBox *>(FindPaneByID(kAssD_CatchupCheckBox));
         //theTextBox->Disable();
         //theCheckBox->Disable();
         //SetValueForPaneID(kAssD_CatchupCheckBox, Button_Off);
         //StartListening();
      }
      break;

      case cmd_AssD_Beninc5Radio:
      {
         mIaltbi = AssumptionType::FLAT;
         mbCachup = false;
         StopListening();
         LEditField *theTextBox;
         LStdCheckBox *theCheckBox;
         SetValueForPaneID(kAssD_Beninc1Radio, Button_Off);
         SetValueForPaneID(kAssD_Beninc2Radio, Button_Off);
         SetValueForPaneID(kAssD_Beninc3Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc4Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc6Radio, Button_Off);
         SetValueForPaneID(kAssD_Beninc7Radio, Button_Off);
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kAssD_BenincTextBox));
         theCheckBox =
            dynamic_cast<LStdCheckBox *>(FindPaneByID(kAssD_CatchupCheckBox));
         theTextBox->Disable();
         theCheckBox->Disable();
         SetValueForPaneID(kAssD_CatchupCheckBox, Button_Off);
         StartListening();
      }
      break;

      case cmd_AssD_Beninc6Radio:
      {
         //mIaltbi = AssumptionType::PEBS_ASSUM;
         //mbCachup = false;
         //StopListening();
         //LEditField *theTextBox;
         //LStdCheckBox *theCheckBox;
         //SetValueForPaneID(kAssD_Beninc1Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc2Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc3Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc4Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc5Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc7Radio, Button_Off);
         //theTextBox =
         //   dynamic_cast<LEditField *>(FindPaneByID(kAssD_BenincTextBox));
         //theCheckBox =
         //   dynamic_cast<LStdCheckBox *>(FindPaneByID(kAssD_CatchupCheckBox));
         //theTextBox->Disable();
         //theCheckBox->Disable();
         //SetValueForPaneID(kAssD_CatchupCheckBox, Button_Off);
         //StartListening();
      }
      break;

      case cmd_AssD_Beninc7Radio:
      {
         mIaltbi = AssumptionType::OTHER_ASSUM;
         StopListening();
         LEditField *theTextBox;
         LStdCheckBox *theCheckBox;
         SetValueForPaneID(kAssD_Beninc1Radio, Button_Off);
         SetValueForPaneID(kAssD_Beninc2Radio, Button_Off);
         SetValueForPaneID(kAssD_Beninc3Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc4Radio, Button_Off);
         SetValueForPaneID(kAssD_Beninc5Radio, Button_Off);
         //SetValueForPaneID(kAssD_Beninc6Radio, Button_Off);
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kAssD_BenincTextBox));
         theCheckBox =
            dynamic_cast<LStdCheckBox *>(FindPaneByID(kAssD_CatchupCheckBox));
         theTextBox->Enable();
         theCheckBox->Enable();
         StartListening();
      }
      break;

      case cmd_AssD_Avgwage1Radio:
      {
         mIaltaw = AssumptionType::ALTERN_I;
         StopListening();
         LEditField *theTextBox;
         SetValueForPaneID(kAssD_Avgwage2Radio, Button_Off);
         SetValueForPaneID(kAssD_Avgwage3Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage4Radio, Button_Off);
         SetValueForPaneID(kAssD_Avgwage5Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage6Radio, Button_Off);
         SetValueForPaneID(kAssD_Avgwage7Radio, Button_Off);
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kAssD_AvgwageTextBox));
         theTextBox->Disable();
         StartListening();
      }
      break;

      case cmd_AssD_Avgwage2Radio:
      {
         mIaltaw = AssumptionType::ALTERN_IIB;
         StopListening();
         LEditField *theTextBox;
         SetValueForPaneID(kAssD_Avgwage1Radio, Button_Off);
         SetValueForPaneID(kAssD_Avgwage3Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage4Radio, Button_Off);
         SetValueForPaneID(kAssD_Avgwage5Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage6Radio, Button_Off);
         SetValueForPaneID(kAssD_Avgwage7Radio, Button_Off);
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kAssD_AvgwageTextBox));
         theTextBox->Disable();
         StartListening();
      }
      break;

      case cmd_AssD_Avgwage3Radio:
      {
         mIaltaw = AssumptionType::ALTERN_III;
         StopListening();
         LEditField *theTextBox;
         SetValueForPaneID(kAssD_Avgwage1Radio, Button_Off);
         SetValueForPaneID(kAssD_Avgwage2Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage4Radio, Button_Off);
         SetValueForPaneID(kAssD_Avgwage5Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage6Radio, Button_Off);
         SetValueForPaneID(kAssD_Avgwage7Radio, Button_Off);
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kAssD_AvgwageTextBox));
         theTextBox->Disable();
         StartListening();
      }
      break;

      case cmd_AssD_Avgwage4Radio:
      {
         //mIaltaw = AssumptionType::ALTERN_IIA;
         //StopListening();
         //LEditField *theTextBox;
         //SetValueForPaneID(kAssD_Avgwage1Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage2Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage3Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage5Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage6Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage7Radio, Button_Off);
         //theTextBox =
         //   dynamic_cast<LEditField *>(FindPaneByID(kAssD_AvgwageTextBox));
         //theTextBox->Disable();
         //StartListening();
      }
      break;

      case cmd_AssD_Avgwage5Radio:
      {
         mIaltaw = AssumptionType::FLAT;
         StopListening();
         LEditField *theTextBox;
         SetValueForPaneID(kAssD_Avgwage1Radio, Button_Off);
         SetValueForPaneID(kAssD_Avgwage2Radio, Button_Off);
         SetValueForPaneID(kAssD_Avgwage3Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage4Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage6Radio, Button_Off);
         SetValueForPaneID(kAssD_Avgwage7Radio, Button_Off);
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kAssD_AvgwageTextBox));
         theTextBox->Disable();
         StartListening();
      }
      break;

      case cmd_AssD_Avgwage6Radio:
      {
         //mIaltaw = AssumptionType::PEBS_ASSUM;
         //StopListening();
         //LEditField *theTextBox;
         //SetValueForPaneID(kAssD_Avgwage1Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage2Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage3Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage4Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage5Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage7Radio, Button_Off);
         //theTextBox =
         //   dynamic_cast<LEditField *>(FindPaneByID(kAssD_AvgwageTextBox));
         //theTextBox->Disable();
         //StartListening();
      }
      break;

      case cmd_AssD_Avgwage7Radio:
      {
         mIaltaw = AssumptionType::OTHER_ASSUM;
         StopListening();
         LEditField *theTextBox;
         SetValueForPaneID(kAssD_Avgwage1Radio, Button_Off);
         SetValueForPaneID(kAssD_Avgwage2Radio, Button_Off);
         SetValueForPaneID(kAssD_Avgwage3Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage4Radio, Button_Off);
         SetValueForPaneID(kAssD_Avgwage5Radio, Button_Off);
         //SetValueForPaneID(kAssD_Avgwage6Radio, Button_Off);
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kAssD_AvgwageTextBox));
         theTextBox->Enable();
         StartListening();
      }
      break;

      case cmd_AssD_WagebaseAutoRadio:
      {
         mBases = 1;
         StopListening();
         SetValueForPaneID(kAssD_WagebaseAdhocRadio, Button_Off);
         StartListening();
      }
      break;

      case cmd_AssD_WagebaseAdhocRadio:
      {
         mBases = 2;
         StopListening();
         SetValueForPaneID(kAssD_WagebaseAutoRadio, Button_Off);
         StartListening();
      }
      break;

      case cmd_AssD_CatchupCheckBox:
      {
         mbCachup = (GetValueForPaneID(kAssD_CatchupCheckBox) == Button_On);
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

void CAssumptionsDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_AssumptionsDialog );
}

//  Save results from dialog

void CAssumptionsDialog::SaveResults()
{
   try {
      CTextView *theTextView = theDoc->getTextView();
      UserAssumptions *userAssumptions = theDoc->userAssumptions;
      if (userAssumptions->getIaltbi() != mIaltbi) {
         userAssumptions->setIaltbi(mIaltbi);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (mIaltbi == AssumptionType::OTHER_ASSUM &&
         userAssumptions->titleBi.compare(mBitext) != 0) {
         userAssumptions->titleBi = mBitext;
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (userAssumptions->getIaltaw() != mIaltaw) {
         userAssumptions->setIaltaw(mIaltaw);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (mIaltaw == AssumptionType::OTHER_ASSUM &&
         userAssumptions->titleAw.compare(mAwtext) != 0) {
         userAssumptions->titleAw = mAwtext;
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (userAssumptions->getIbasch() != mBases) {
         userAssumptions->setIbasch(mBases);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (mbCachup && (userAssumptions->getAnscch() == 'N')) {
         userAssumptions->setAnscch('Y');
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (!mbCachup && (userAssumptions->getAnscch() == 'Y')) {
         userAssumptions->setAnscch('N');
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      theDoc->piacal->calculate1(*(theDoc->assumptions));
   } catch (PiaException& e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CAssumptionsDialog::SaveResults");
   }
}

// Get string in a text box

string CAssumptionsDialog::getTextBoxString( long textBoxID )
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
