// $Id: CBaseyearDialog.cp,v 1.4 2010/08/16 18:18:06 djolson Exp $
//  CBaseyearDialog.cp

#include <cstdlib>
#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CBaseyearDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "comma.h"
#include "Resource.h"
#include "piaparmsAny.h"
#include "UserAssumptions.h"
#include "wrkrdata.h"

using namespace std;

//  Create a dialog from a stream

CBaseyearDialog *CBaseyearDialog::CreateBaseyearDialogStream(
   LStream *inStream )
{
   return new CBaseyearDialog( inStream );
}

//  Create a dialog from a stream

CBaseyearDialog::CBaseyearDialog( LStream *inStream ) :
   LDialogBox( inStream )
{ }

//  Destroy a dialog

CBaseyearDialog::~CBaseyearDialog()
{ }

//  Set menu status

void CBaseyearDialog::FindCommandStatus(
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

void CBaseyearDialog::SetupDialog()
{
   LEditField *theTextBox;

   StopListening();
   AwbiData *awbidat = theDoc->awbidat;
   mLastyear = awbidat->getLastYear();
   BaseYear *baseyear = theDoc->baseyear;
   mBaseyear = baseyear->getYear();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kCmpD_BaseyearTextBox));
   LStr255 temp(poutNoComma(mBaseyear).c_str());
   theTextBox->SetDescriptor(temp);
   StartListening();
}

//  Listen to messages

void CBaseyearDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_CmpD_OKButton:
      {
         try {
            StopListening();
            PiaData *piadata = theDoc->piadata;
            mBaseyear = atoi(getTextBoxString(kCmpD_BaseyearTextBox).c_str());
            BaseYear::yearCheck(mBaseyear);
            if (mBaseyear > mLastyear)
               throw PiaException(PIA_IDS_BASEYR1);
            StartListening();
            // Values are good, so go back to top level
            LDialogBox::ListenToMessage( inMessage, ioParam );
         } catch (PiaException e) {
            StartListening();
            messageDialog(e.getNumber(), this);
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CBaseyearDialog::ListenToMessage for cmd_CmpD_OKButton");
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

void CBaseyearDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_CompyearDialog );
}

//  Save results from dialog

void CBaseyearDialog::SaveResults()
{
   try {
      BaseYear *baseyear = theDoc->baseyear;
      PiaParams *piaparms = theDoc->piaparms;
      AwbiData *awbidat = theDoc->awbidat;
      WorkerData *workerData = theDoc->workerData;
      UserAssumptions *userAssumptions = theDoc->userAssumptions;
      AssumptionsNonFile& assumptions = *(theDoc->assumptions);
      if (baseyear->getYear() != mBaseyear) {
         piaparms->setIstart(mBaseyear);
         if (mBaseyear > baseyear->getYear())
            piaparms->setData(*awbidat);
         piaparms->setHistFqinc();
         userAssumptions->zeroJalt();
         baseyear->setYear(mBaseyear);
         userAssumptions->setIstart(mBaseyear);
         assumptions.setIstart(mBaseyear);
         theDoc->piacal->calculate1(assumptions);
         if (workerData->getJoasdi() == WorkerData::PEBS_CALC)
            workerData->setPebsEarn(UserAssumptions::getIstart());
         theDoc->valid = false;
      }
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CBaseyearDialog::SaveResults");
   }
}

// Get string in a text box

string CBaseyearDialog::getTextBoxString( long textBoxID )
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
