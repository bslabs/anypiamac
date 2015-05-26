// $Id: CPebesDialog.cp,v 1.6 2010/08/16 18:18:07 djolson Exp $
//  CPebesDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CTextView.h"
#include "CPebesDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "comma.h"
#include "FormatString.h"
#include "Resource.h"
#include "UserAssumptions.h"
#include "DateFormatter.h"
#include "wrkrdata.h"

using namespace std;

//  Create a dialog from a stream

CPebesDialog *CPebesDialog::CreatePebesDialogStream(
   LStream *inStream )
{
   return new CPebesDialog( inStream );
}

//  Create a dialog from a stream

CPebesDialog::CPebesDialog( LStream *inStream ) :
   LDialogBox( inStream )
{ }

//  Destroy a dialog

CPebesDialog::~CPebesDialog()
{ }

//  Set menu status

void CPebesDialog::FindCommandStatus(
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

void CPebesDialog::SetupDialog()
{
   LEditField *theTextBox;  // pointer to dialog text box
   LCaption *theCaption;  // pointer to month caption

   StopListening();
   const WorkerData& workerData = *(theDoc->workerData);
   Pebs *pebs = theDoc->pebs;
   mBirthyear = workerData.getBirthDate().year();
   mBirthdate = workerData.getBirthDate();
   mDeathdate = workerData.getDeathDate();
   mAgeplan = pebs->getAgePlan();
   mBeginpebes = workerData.getIbegin();
   mNonpen = workerData.getPubpen();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kPbsD_NonpenTextBox));
   LStr255 temp10(poutNoComma(mNonpen, 4, 2).c_str());
   theTextBox->SetDescriptor(temp10);
   mNonpendate = workerData.getPubpenDate();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kPbsD_NonpenDateBox));
   LStr255 temp11(DateFormatter::toString(workerData.getPubpenDate(),"s").c_str());
   theTextBox->SetDescriptor(temp11);
   if (pebs->getMonthnow() == 0)
      pebs->setMonthnow();
   mMonth = pebs->getMonthnow();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kPbsD_AgePlanTextBox));
   LStr255 temp1(poutNoComma(mAgeplan).c_str());
   theTextBox->SetDescriptor(temp1);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kPbsD_BeginTextBox));
   LStr255 temp2(poutNoComma(mBeginpebes).c_str());
   theTextBox->SetDescriptor(temp2);
   SetValueForPaneID(kPbsD_JanuaryRadio,
      (mMonth == 1) ? Button_On : Button_Off);
   SetValueForPaneID(kPbsD_FebruaryRadio,
      (mMonth == 2) ? Button_On : Button_Off);
   SetValueForPaneID(kPbsD_MarchRadio,
      (mMonth == 3) ? Button_On : Button_Off);
   SetValueForPaneID(kPbsD_AprilRadio,
      (mMonth == 4) ? Button_On : Button_Off);
   SetValueForPaneID(kPbsD_MayRadio,
      (mMonth == 5) ? Button_On : Button_Off);
   SetValueForPaneID(kPbsD_JuneRadio,
      (mMonth == 6) ? Button_On : Button_Off);
   SetValueForPaneID(kPbsD_JulyRadio,
      (mMonth == 7) ? Button_On : Button_Off);
   SetValueForPaneID(kPbsD_AugustRadio,
      (mMonth == 8) ? Button_On : Button_Off);
   SetValueForPaneID(kPbsD_SeptemberRadio,
      (mMonth == 9) ? Button_On : Button_Off);
   SetValueForPaneID(kPbsD_OctoberRadio,
      (mMonth == 10) ? Button_On : Button_Off);
   SetValueForPaneID(kPbsD_NovemberRadio,
      (mMonth == 11) ? Button_On : Button_Off);
   SetValueForPaneID(kPbsD_DecemberRadio,
      (mMonth == 12) ? Button_On : Button_Off);
   string monthstring;
   mIstart = UserAssumptions::getIstart();
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kPbsD_MonthCaption));
   AfxFormatString1(monthstring, PIA_IDS_MONTHTEXT,
      poutNoComma(mIstart).c_str());
   LStr255 temp3(monthstring.c_str());
   theCaption->SetDescriptor(temp3);
   StartListening();
}

//  Listen to messages

void CPebesDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_PbsD_CancelButton:
      {
         LDialogBox::ListenToMessage( inMessage, ioParam );
      }
      break;
   
      case cmd_PbsD_OKButton:
      {
         try {
            StopListening();
            PiaData *piadata = theDoc->piadata;
            mAgeplan = atoi(getTextBoxString(kPbsD_AgePlanTextBox).c_str());
            Pebs::agePlanCheck(mAgeplan);
            mBeginpebes = atoi(getTextBoxString(kPbsD_BeginTextBox).c_str());
            WorkerData::ibegin1Check(mBeginpebes, UserAssumptions::getIstart());
            WorkerData::ibeginCheck(mBeginpebes, mBirthyear);
            mNonpen = atof(getTextBoxString(kPbsD_NonpenTextBox).c_str());
            if (mNonpen > 0) {
               try {
                  mNonpendate = DateMoyr(getTextBoxString(kPbsD_NonpenDateBox));
                  mNonpendate.check();
               } catch (PiaException& e) {
                  throw PiaException(PIA_IDS_NONPENDATE3);
               }
               WorkerData::pubpenDateCheck(mNonpendate, mBirthdate,
                  mDeathdate);
            } else {
               mNonpendate = DateMoyr(0,0);
            }
            if (GetValueForPaneID(kPbsD_JanuaryRadio))
               mMonth = 1;
            if (GetValueForPaneID(kPbsD_FebruaryRadio))
               mMonth = 2;
            if (GetValueForPaneID(kPbsD_MarchRadio))
               mMonth = 3;
            if (GetValueForPaneID(kPbsD_AprilRadio))
               mMonth = 4;
            if (GetValueForPaneID(kPbsD_MayRadio))
               mMonth = 5;
            if (GetValueForPaneID(kPbsD_JuneRadio))
               mMonth = 6;
            if (GetValueForPaneID(kPbsD_JulyRadio))
               mMonth = 7;
            if (GetValueForPaneID(kPbsD_AugustRadio))
               mMonth = 8;
            if (GetValueForPaneID(kPbsD_SeptemberRadio))
               mMonth = 9;
            if (GetValueForPaneID(kPbsD_OctoberRadio))
               mMonth = 10;
            if (GetValueForPaneID(kPbsD_NovemberRadio))
               mMonth = 11;
            if (GetValueForPaneID(kPbsD_DecemberRadio))
               mMonth = 12;
            StartListening();
            // Values are good, so go back to top level
            LDialogBox::ListenToMessage( inMessage, ioParam );
         } catch (PiaException e) {
            StartListening();
            messageDialog(e.getNumber(), this);
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CPebesDialog::ListenToMessage for cmd_PbsD_OKButton");
         }
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

void CPebesDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_PebesDialog );
}

//  Save results from dialog

void CPebesDialog::SaveResults()
{
   try {
      CTextView *theTextView = theDoc->getTextView();
      WorkerData& workerData = *(theDoc->workerData);
      Pebs *pebs = theDoc->pebs;
      EarnProject *earnProject = theDoc->earnProject;
      if (mAgeplan != pebs->getAgePlan()) {
         pebs->setAgePlan(mAgeplan);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (mBeginpebes != workerData.getIbegin()) {
         workerData.setIbegin(mBeginpebes);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (mNonpen != workerData.getPubpen()) {
         workerData.setPubpen((float)mNonpen);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (mNonpendate != workerData.getPubpenDate()) {
         if (mNonpen == 0.0) {
         	workerData.deletePubpenDate();
         } else {
            workerData.setPubpenDate(mNonpendate);
         }
         workerData.setPubpen((float)mNonpen);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (mMonth != pebs->getMonthnow()) {
         pebs->setMonthnow(mMonth);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      earnProject->setPebsData(UserAssumptions::getIstart(),
         workerData.getIbegin());
      workerData.setPebsData();
      theDoc->piacal->pebsOabCal();
      theDoc->pebsOut->pebsWorkerInfo.prepareStrings();
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CPebesDialog::SaveResults");
   }
}

// Get string in a text box

string CPebesDialog::getTextBoxString( long textBoxID )
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
