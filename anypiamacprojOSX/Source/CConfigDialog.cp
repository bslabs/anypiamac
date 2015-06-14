// $Id: CConfigDialog.cp,v 1.3 2007/11/07 13:06:57 djolson Exp $
//  CConfigDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CConfigDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "comma.h"

using namespace std;

//  Create a dialog from a stream

CConfigDialog *CConfigDialog::CreateConfigDialogStream(
   LStream *inStream )
{
   return new CConfigDialog( inStream );
}

//  Create a dialog from a stream

CConfigDialog::CConfigDialog( LStream *inStream ) :
   LDialogBox( inStream )
{ }

//  Destroy a dialog

CConfigDialog::~CConfigDialog()
{ }

//  Set menu status

void CConfigDialog::FindCommandStatus(
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

void CConfigDialog::SetupDialog()
{
   LEditField *theTextBox;

   StopListening();
   Config *config = theDoc->config;
   longOutput = config->getLongOutput();
   //if (longOutput) {
   //   SetValueForPaneID(kCnfD_ShortRadio,Button_Off);
   //   SetValueForPaneID(kCnfD_LongRadio,Button_On);
   //}
   //else {
   //   SetValueForPaneID(kCnfD_ShortRadio,Button_On);
   //   SetValueForPaneID(kCnfD_LongRadio,Button_Off);
   //}
   mTopmargin = config->getTopMargin();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kCnfD_TopmarTextBox));
   LStr255 temp1(poutNoComma(mTopmargin, 3, 2).c_str());
   theTextBox->SetDescriptor(temp1);
   mLeftmargin = config->getLeftMargin();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kCnfD_LeftmarTextBox));
   LStr255 temp2(poutNoComma(mLeftmargin, 3, 2).c_str());
   theTextBox->SetDescriptor(temp2);
   mPrinterPoints = config->getPrinterPoints();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kCnfD_FontsizeTextBox));
   LStr255 temp3(poutNoComma(mPrinterPoints).c_str());
   theTextBox->SetDescriptor(temp3);
   StartListening();
}

//  Listen to messages

void CConfigDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_CnfD_OKButton:
      {
         try {
            StopListening();
            mTopmargin = atof(getTextBoxString(kCnfD_TopmarTextBox).c_str());
            mLeftmargin = atof(getTextBoxString(kCnfD_LeftmarTextBox).c_str());
            mPrinterPoints = atof(getTextBoxString(kCnfD_FontsizeTextBox).c_str());
            StartListening();
            // Values are good, so go back to top level
            LDialogBox::ListenToMessage( inMessage, ioParam );
         } catch (PiaException e) {
            StartListening();
            messageDialog(e.getNumber(), this);
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CConfigDialog::ListenToMessage for cmd_CnfD_OKButton");
         }
      }
      break;

      //case cmd_CnfD_ShortRadio:
      //{
      //   if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
      //      longOutput = false;
      //      StopListening();
      //      SetValueForPaneID(kCnfD_LongRadio, Button_Off);
      //      StartListening();
      //   }
      //}
      //break;

      //case cmd_CnfD_LongRadio:
      //{
      //   if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
      //      longOutput = true;
      //      StopListening();
      //      SetValueForPaneID(kCnfD_ShortRadio, Button_Off);
      //      StartListening();
      //   }
      //}
      //break;

      default:
      {
         LDialogBox::ListenToMessage( inMessage, ioParam );
         break;
      }
   }
}

//  Connect to controls

void CConfigDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_ConfigDialog );
}

//  Save results from dialog

void CConfigDialog::SaveResults()
{
   try {
      Config *config = theDoc->config;
      if (config->getLongOutput() != longOutput) {
         config->setLongOutput(longOutput);
      }
      if (config->getTopMargin() != mTopmargin) {
         config->setTopMargin(mTopmargin);
      }
      if (config->getLeftMargin() != mLeftmargin) {
         config->setLeftMargin(mLeftmargin);
      }
      if (config->getPrinterPoints() != mPrinterPoints) {
         config->setPrinterPoints(mPrinterPoints);
      }
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CConfigDialog::SaveResults");
   }
}

// Get string in a text box

string CConfigDialog::getTextBoxString( long textBoxID )
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
