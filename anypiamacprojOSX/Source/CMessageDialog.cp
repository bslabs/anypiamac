// $Id: CMessageDialog.cp,v 1.3 2007/11/07 13:06:58 djolson Exp $
//  CMessageDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CMessageDialog.h"
#include "AppConstants.h"
#include "PiaException.h"

//  Create a dialog from a stream

CMessageDialog *CMessageDialog::CreateMessageDialogStream(
   LStream *inStream )
{
   return new CMessageDialog( inStream );
}

//  Create a dialog from a stream

CMessageDialog::CMessageDialog( LStream *inStream ) :
   LDialogBox( inStream ), msgNum(0), mString("Message")
{ }

//  Destroy a dialog

CMessageDialog::~CMessageDialog()
{ }

//  Set menu status

void CMessageDialog::FindCommandStatus(
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

void CMessageDialog::SetupDialog()
{
   LCaption *theTextBox;  // pointer to dialog text box

   StopListening();
   theTextBox =
      dynamic_cast<LCaption *>(FindPaneByID(kMsgD_MessageTextBox));
   LStr255 theString;
   if (msgNum > 61203) {
      unsigned int rsc0, rsc1, rsc2;
      rsc0 = msgNum / 4;
      rsc1 = rsc0 - 14300;
      rsc2 = msgNum - 4 * rsc0 + 1;
      GetIndString(theString, rsc1, rsc2);
   }
   else {
      theString = mString;
   }
   theTextBox->SetDescriptor(theString);
   StartListening();
}

//  Listen to messages

void CMessageDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_MsgD_OKButton:
      {
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

void CMessageDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_MessageDialog );
}

//  Save results from dialog

void CMessageDialog::SaveResults()
{
}

//  Create and show a message dialog

void messageDialog( unsigned int number, LCommander* inSuperCommander )
{
   CMessageDialog *msgDialog;
   msgDialog = dynamic_cast<CMessageDialog *>
      (LWindow::CreateWindow( rPPob_MessageDialog, inSuperCommander ));
   Assert_(msgDialog != nil);
   msgDialog->msgNum = number;
   msgDialog->SetupDialog();
   msgDialog->Show();
}

//  Create and show a message dialog

void messageDialog( const char *message, LCommander* inSuperCommander )
{
   CMessageDialog *msgDialog;
   msgDialog = dynamic_cast<CMessageDialog *>
      (LWindow::CreateWindow( rPPob_MessageDialog, inSuperCommander ));
   Assert_(msgDialog != nil);
   msgDialog->msgNum = 0;
   msgDialog->mString = message;
   msgDialog->SetupDialog();
   msgDialog->Show();
}
