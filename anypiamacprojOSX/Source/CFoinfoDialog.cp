// $Id: CFoinfoDialog.cp,v 1.3 2007/11/07 13:06:57 djolson Exp $
//  CFoinfoDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CFoinfoDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"

using namespace std;

//  Create a dialog from a stream

CFoinfoDialog *CFoinfoDialog::CreateFoinfoDialogStream(
   LStream *inStream )
{
   return new CFoinfoDialog( inStream );
}

//  Create a dialog from a stream

CFoinfoDialog::CFoinfoDialog( LStream *inStream ) :
   LDialogBox( inStream )
{ }

//  Destroy a dialog

CFoinfoDialog::~CFoinfoDialog()
{ }

//  Set menu status

void CFoinfoDialog::FindCommandStatus(
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

void CFoinfoDialog::SetupDialog()
{
   LEditField *theTextBox;

   StopListening();
   FieldOfficeInfo *foinfo = theDoc->foinfo;
   mFoname = foinfo->getFieldOfficeName();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kFoiD_FonameTextBox));
   LStr255 temp1(mFoname.c_str());
   theTextBox->SetDescriptor(temp1);
   mFotitle = foinfo->getFieldOfficeTitle();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kFoiD_FotitleTextBox));
   LStr255 temp2(mFotitle.c_str());
   theTextBox->SetDescriptor(temp2);
   mFoaddr1 = foinfo->getAddress(0);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kFoiD_Foaddr1TextBox));
   LStr255 temp3(mFoaddr1.c_str());
   theTextBox->SetDescriptor(temp3);
   mFoaddr2 = foinfo->getAddress(1);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kFoiD_Foaddr2TextBox));
   LStr255 temp4(mFoaddr2.c_str());
   theTextBox->SetDescriptor(temp4);
   mFoaddr3 = foinfo->getAddress(2);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kFoiD_Foaddr3TextBox));
   LStr255 temp5(mFoaddr3.c_str());
   theTextBox->SetDescriptor(temp5);
   mFoaddr4 = foinfo->getAddress(3);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kFoiD_Foaddr4TextBox));
   LStr255 temp6(mFoaddr4.c_str());
   theTextBox->SetDescriptor(temp6);
   StartListening();
}

//  Listen to messages

void CFoinfoDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_FoiD_OKButton:
      {
         try {
            StopListening();
            mFoname = getTextBoxString(kFoiD_FonameTextBox);
            mFotitle = getTextBoxString(kFoiD_FotitleTextBox);
            mFoaddr1 = getTextBoxString(kFoiD_Foaddr1TextBox);
            mFoaddr2 = getTextBoxString(kFoiD_Foaddr2TextBox);
            mFoaddr3 = getTextBoxString(kFoiD_Foaddr3TextBox);
            mFoaddr4 = getTextBoxString(kFoiD_Foaddr4TextBox);
            StartListening();
            // Values are good, so go back to top level
            LDialogBox::ListenToMessage( inMessage, ioParam );
         } catch (PiaException e) {
            StartListening();
            messageDialog(e.getNumber(), this);
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CFoinfoDialog::ListenToMessage for cmd_FoiD_OKButton");
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

void CFoinfoDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_FieldOfficeDialog );
}

//  Save results from dialog

void CFoinfoDialog::SaveResults()
{
   try {
      FieldOfficeInfo *foinfo = theDoc->foinfo;
      if (foinfo->getFieldOfficeName().compare(mFoname) != 0) {
         foinfo->setFieldOfficeName(mFoname);
      }
      if (foinfo->getFieldOfficeTitle().compare(mFotitle) != 0) {
         foinfo->setFieldOfficeTitle(mFotitle);
      }
      if (foinfo->getAddress(0).compare(mFoaddr1) != 0) {
         foinfo->setAddress(0, mFoaddr1);
      }
      if (foinfo->getAddress(1).compare(mFoaddr2) != 0) {
         foinfo->setAddress(1, mFoaddr2);
      }
      if (foinfo->getAddress(2).compare(mFoaddr3) != 0) {
         foinfo->setAddress(2, mFoaddr3);
      }
      if (foinfo->getAddress(3).compare(mFoaddr4) != 0) {
         foinfo->setAddress(3, mFoaddr4);
      }
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CFoinfoDialog::SaveResults");
   }
}

// Get string in a text box

string CFoinfoDialog::getTextBoxString( long textBoxID )
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
