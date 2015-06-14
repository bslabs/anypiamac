// $Id: CSummaryQCDialog.cp,v 1.5 2010/08/16 18:18:07 djolson Exp $
//  CSummaryQCDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CTextView.h"
#include "CSummaryQCDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "comma.h"
#include "wrkrdata.h"

using namespace std;

//  Create a dialog from a stream

CSummaryQCDialog *CSummaryQCDialog::CreateSummaryQCDialogStream(
   LStream *inStream )
{
   return new CSummaryQCDialog( inStream );
}

//  Create a dialog from a stream

CSummaryQCDialog::CSummaryQCDialog( LStream *inStream ) :
   LDialogBox( inStream )
{ }

//  Destroy a dialog

CSummaryQCDialog::~CSummaryQCDialog()
{ }

//  Set menu status

void CSummaryQCDialog::FindCommandStatus(
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

void CSummaryQCDialog::SetupDialog()
{
   LEditField *theTextBox;

   StopListening();
   const WorkerData& workerData = *(theDoc->workerData);
   mQcann = workerData.hasQcsByYear();
   SetValueForPaneID(kSQCD_LumpRadio,
      (mQcann) ? Button_Off : Button_On);
   SetValueForPaneID(kSQCD_AnnualRadio,
      (mQcann) ? Button_On : Button_Off);
   mQctd = workerData.getQctottd();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kSQCD_QctdTextBox));
   LStr255 temp1(poutNoComma(mQctd).c_str());
   theTextBox->SetDescriptor(temp1);
   if (!mQcann) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   mQc51td = workerData.getQctot51td();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kSQCD_Qc51tdTextBox));
   LStr255 temp2(poutNoComma(mQc51td).c_str());
   theTextBox->SetDescriptor(temp2);
   mbQc51td = (workerData.getIbegin() < 1951 &&
      workerData.getIend() > 1950);
   if (!mQcann && mbQc51td) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   StartListening();
}

//  Listen to messages

void CSummaryQCDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_SQCD_OKButton:
      {
         try {
            StopListening();
            PiaData *piadata = theDoc->piadata;
            if (!mQcann) {
               mQctd = atoi(getTextBoxString(kSQCD_QctdTextBox).c_str());
               WorkerData::qctdCheck(mQctd);
               if (mbQc51td) {
                  mQc51td = atoi(getTextBoxString(kSQCD_Qc51tdTextBox).c_str());
                  WorkerData::qc51tdCheck(mQc51td);
               } else {
                  WorkerData::qc51tdCheck(mQctd);
                  mQc51td = mQctd;
               }
               WorkerData::qcCheck(mQctd, mQc51td);
            }
            StartListening();
            // Values are good, so go back to top level
            LDialogBox::ListenToMessage( inMessage, ioParam );
         } catch (PiaException& e) {
            StartListening();
            messageDialog(e.getNumber(), this);
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CSummaryQCDialog::ListenToMessage for cmd_SQCD_OKButton");
         }
      }
      break;
      
   	  case cmd_SQCD_LumpRadio:
   	  {  
         if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
         mQcann = false;
         StopListening();
         LEditField *theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kSQCD_Qc51tdTextBox));
         if (mbQc51td) {
            theTextBox->Enable();
         } else {
            theTextBox->Disable();
         }
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kSQCD_QctdTextBox));
         theTextBox->Enable();
         SetValueForPaneID(kSQCD_AnnualRadio, Button_Off);
         StartListening();
         }
   	  }
   	  break;
   	  
   	  case cmd_SQCD_AnnualRadio:
   	  {  
         if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
         mQcann = true;
         StopListening();
         LEditField *theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kSQCD_Qc51tdTextBox));
         theTextBox->Disable();
         theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kSQCD_QctdTextBox));
         theTextBox->Disable();
         SetValueForPaneID(kSQCD_LumpRadio, Button_Off);
         StartListening();
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

void CSummaryQCDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_SummaryQCDialog );
}

//  Save results from dialog

void CSummaryQCDialog::SaveResults()
{
   try {
      CTextView *theTextView = theDoc->getTextView();
      WorkerData& workerData = *(theDoc->workerData);
      if (workerData.hasQcsByYear() != mQcann) {
         workerData.setQcsByYear(mQcann);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (!mQcann) {
         if (workerData.getQctottd() != mQctd) {
            workerData.setQctd(mQctd);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (mbQc51td && workerData.getQctot51td() != mQc51td) {
            workerData.setQc51td(mQc51td);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
   } catch (PiaException& e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CSummaryQCDialog::SaveResults");
   }
}

// Get string in a text box

string CSummaryQCDialog::getTextBoxString( long textBoxID )
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
