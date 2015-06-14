// $Id: CSpecYearDialog.cp,v 1.3 2007/11/07 13:06:59 djolson Exp $
//  CSpecYearDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CSpecYearDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"

using namespace std;

//  Create a dialog from a stream

CSpecYearDialog *CSpecYearDialog::CreateSpecYearDialogStream(
   LStream *inStream )
{
   return new CSpecYearDialog( inStream );
}

//  Create a dialog from a stream

CSpecYearDialog::CSpecYearDialog( LStream *inStream ) :
   LDialogBox( inStream )
{ }

//  Destroy a dialog

CSpecYearDialog::~CSpecYearDialog()
{ }

//  Set menu status

void CSpecYearDialog::FindCommandStatus(
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

void CSpecYearDialog::SetupDialog()
{
   StopListening();
   mYear = theEarningsDialog->mEarnyear;
   mStartyear = theEarningsDialog->mStartyear1;
   mLastyear = theEarningsDialog->mLastyear1;
   StartListening();
}

//  Listen to messages

void CSpecYearDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_SpcD_OKButton:
      {
         StopListening();
         mYear = atoi(getTextBoxString(kSpcD_YearTextBox).c_str());
         StartListening();
         if (mYear < mStartyear || mYear > mLastyear) {
            messageDialog("Year is out of range", this);
            break;
         }
         // Values are good, so go back to top level
         LDialogBox::ListenToMessage( inMessage, ioParam );
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

void CSpecYearDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_SpecYearDialog );
}

//  Save results from dialog

void CSpecYearDialog::SaveResults()
{
   try {
      theEarningsDialog->StopListening();
      theEarningsDialog->mEarnyear = mYear;
      theEarningsDialog->setupdata();
      theEarningsDialog->StartListening();
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CSpecYearDialog::SaveResults");
   }
}

// Get string in a text box

string CSpecYearDialog::getTextBoxString( long textBoxID )
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
