// $Id: CScreenDialog.cp,v 1.3 2007/11/07 13:06:59 djolson Exp $
//  CScreenDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CScreenDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "comma.h"

using namespace std;

//  Create a dialog from a stream

CScreenDialog *CScreenDialog::CreateScreenDialogStream(
   LStream *inStream )
{
   return new CScreenDialog( inStream );
}

//  Create a dialog from a stream

CScreenDialog::CScreenDialog( LStream *inStream ) :
   LDialogBox( inStream )
{ }

//  Destroy a dialog

CScreenDialog::~CScreenDialog()
{ }

//  Set menu status

void CScreenDialog::FindCommandStatus(
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

void CScreenDialog::SetupDialog()
{
   LEditField *theTextBox;

   StopListening();
   Config *config = theDoc->config;
   mScreenPoints = config->getScreenPoints();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kScnD_FontsizeTextBox));
   LStr255 temp1(poutNoComma(mScreenPoints).c_str());
   theTextBox->SetDescriptor(temp1);
   StartListening();
}

//  Listen to messages

void CScreenDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_ScnD_OKButton:
      {
         try {
            StopListening();
            mScreenPoints = atoi(getTextBoxString(kScnD_FontsizeTextBox).c_str());
            StartListening();
            // Values are good, so go back to top level
            LDialogBox::ListenToMessage( inMessage, ioParam );
         } catch (PiaException e) {
            StartListening();
            messageDialog(e.getNumber(), this);
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CScreenDialog::ListenToMessage for cmd_ScnD_OKButton");
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

void CScreenDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_ScreenDialog );
}

//  Save results from dialog

void CScreenDialog::SaveResults()
{
   try {
      Config *config = theDoc->config;
      if (config->getScreenPoints() != mScreenPoints) {
         config->setScreenPoints(mScreenPoints);
      }
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CScreenDialog::SaveResults");
   }
}

// Get string in a text box

string CScreenDialog::getTextBoxString( long textBoxID )
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
