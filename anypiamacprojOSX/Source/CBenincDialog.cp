// $Id: CBenincDialog.cp,v 1.5 2010/08/16 18:18:06 djolson Exp $
//  CBenincDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CTextView.h"
#include "CBenincDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "comma.h"
#include "FormatString.h"
#include "Resource.h"
#include "UserAssumptions.h"
#include "wrkrdata.h"

using namespace std;

//  Create a dialog from a stream

CBenincDialog *CBenincDialog::CreateBenincDialogStream(
   LStream *inStream )
{
   return new CBenincDialog( inStream );
}

//  Create a dialog from a stream

CBenincDialog::CBenincDialog( LStream *inStream ) :
   LDialogBox( inStream ), mBiproj(YEAR51, WorkerData::getMaxyear())
{ }

//  Destroy a dialog

CBenincDialog::~CBenincDialog()
{ }

//  Set menu status

void CBenincDialog::FindCommandStatus(
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

void CBenincDialog::SetupDialog()
{
   LEditField * theTextBox;
   LCaption * theCaption;
   const char *month = "Dec ";  // month of benefit increase
   string temp;

   StopListening();
   UserAssumptions *userAssumptions = theDoc->userAssumptions;
   for (int i = 0; i <= 10; i++) {
      int year = UserAssumptions::getIstart() + i;
      mBiproj[year] = userAssumptions->biproj[year];
   }
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kBenD_Biyear1TextBox));
   mBiyear1 = mBiproj[UserAssumptions::getIstart()];
   LStr255 temp1(poutNoComma(mBiyear1, 3, 1).c_str());
   theTextBox->SetDescriptor(temp1);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kBenD_Biyear2TextBox));
   mBiyear2 = mBiproj[UserAssumptions::getIstart() + 1];
   LStr255 temp2(poutNoComma(mBiyear2, 3, 1).c_str());
   theTextBox->SetDescriptor(temp2);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kBenD_Biyear3TextBox));
   mBiyear3 = mBiproj[UserAssumptions::getIstart() + 2];
   LStr255 temp3(poutNoComma(mBiyear3, 3, 1).c_str());
   theTextBox->SetDescriptor(temp3);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kBenD_Biyear4TextBox));
   mBiyear4 = mBiproj[UserAssumptions::getIstart() + 3];
   LStr255 temp4(poutNoComma(mBiyear4, 3, 1).c_str());
   theTextBox->SetDescriptor(temp4);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kBenD_Biyear5TextBox));
   mBiyear5 = mBiproj[UserAssumptions::getIstart() + 4];
   LStr255 temp5(poutNoComma(mBiyear5, 3, 1).c_str());
   theTextBox->SetDescriptor(temp5);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kBenD_Biyear6TextBox));
   mBiyear6 = mBiproj[UserAssumptions::getIstart() + 5];
   LStr255 temp6(poutNoComma(mBiyear6, 3, 1).c_str());
   theTextBox->SetDescriptor(temp6);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kBenD_Biyear7TextBox));
   mBiyear7 = mBiproj[UserAssumptions::getIstart() + 6];
   LStr255 temp7(poutNoComma(mBiyear7, 3, 1).c_str());
   theTextBox->SetDescriptor(temp7);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kBenD_Biyear8TextBox));
   mBiyear8 = mBiproj[UserAssumptions::getIstart() + 7];
   LStr255 temp8(poutNoComma(mBiyear8, 3, 1).c_str());
   theTextBox->SetDescriptor(temp8);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kBenD_Biyear9TextBox));
   mBiyear9 = mBiproj[UserAssumptions::getIstart() + 8];
   LStr255 temp9(poutNoComma(mBiyear9, 3, 1).c_str());
   theTextBox->SetDescriptor(temp9);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kBenD_Biyear10TextBox));
   mBiyear10 = mBiproj[UserAssumptions::getIstart() + 9];
   LStr255 temp10(poutNoComma(mBiyear10, 3, 1).c_str());
   theTextBox->SetDescriptor(temp10);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kBenD_BiultTextBox));
   mBiult = mBiproj[UserAssumptions::getIstart() + 10];
   LStr255 temp11(poutNoComma(mBiult, 3, 1).c_str());
   theTextBox->SetDescriptor(temp11);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kBenD_Biyear1Caption));
   temp = month;
   temp += poutNoComma(UserAssumptions::getIstart());
   LStr255 temp12(temp.c_str());
   theCaption->SetDescriptor(temp12);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kBenD_Biyear2Caption));
   temp = month;
   temp += poutNoComma(UserAssumptions::getIstart() + 1);
   LStr255 temp13(temp.c_str());
   theCaption->SetDescriptor(temp13);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kBenD_Biyear3Caption));
   temp = month;
   temp += poutNoComma(UserAssumptions::getIstart() + 2);
   LStr255 temp14(temp.c_str());
   theCaption->SetDescriptor(temp14);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kBenD_Biyear4Caption));
   temp = month;
   temp += poutNoComma(UserAssumptions::getIstart() + 3);
   LStr255 temp15(temp.c_str());
   theCaption->SetDescriptor(temp15);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kBenD_Biyear5Caption));
   temp = month;
   temp += poutNoComma(UserAssumptions::getIstart() + 4);
   LStr255 temp16(temp.c_str());
   theCaption->SetDescriptor(temp16);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kBenD_Biyear6Caption));
   temp = month;
   temp += poutNoComma(UserAssumptions::getIstart() + 5);
   LStr255 temp17(temp.c_str());
   theCaption->SetDescriptor(temp17);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kBenD_Biyear7Caption));
   temp = month;
   temp += poutNoComma(UserAssumptions::getIstart() + 6);
   LStr255 temp18(temp.c_str());
   theCaption->SetDescriptor(temp18);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kBenD_Biyear8Caption));
   temp = month;
   temp += poutNoComma(UserAssumptions::getIstart() + 7);
   LStr255 temp19(temp.c_str());
   theCaption->SetDescriptor(temp19);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kBenD_Biyear9Caption));
   temp = month;
   temp += poutNoComma(UserAssumptions::getIstart() + 8);
   LStr255 temp20(temp.c_str());
   theCaption->SetDescriptor(temp20);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kBenD_Biyear10Caption));
   temp = month;
   temp += poutNoComma(UserAssumptions::getIstart() + 9);
   LStr255 temp21(temp.c_str());
   theCaption->SetDescriptor(temp21);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kBenD_BiultCaption));
   AfxFormatString1(temp, PIA_IDS_ANDLATER,
      poutNoComma(UserAssumptions::getIstart() + 10));
   LStr255 temp22(temp.c_str());
   theCaption->SetDescriptor(temp22);
   StartListening();
}

//  Listen to messages

void CBenincDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_BenD_OKButton:
      {
         try {
            StopListening();
            UserAssumptions *userAssumptions = theDoc->userAssumptions;
            mBiyear1 = atof(getTextBoxString(kBenD_Biyear1TextBox).c_str());
            Biproj::cpiincCheck(mBiyear1);
            mBiproj[UserAssumptions::getIstart()] = mBiyear1;
            mBiyear2 = atof(getTextBoxString(kBenD_Biyear2TextBox).c_str());
            Biproj::cpiincCheck(mBiyear2);
            mBiproj[UserAssumptions::getIstart() + 1] = mBiyear2;
            mBiyear3 = atof(getTextBoxString(kBenD_Biyear3TextBox).c_str());
            Biproj::cpiincCheck(mBiyear3);
            mBiproj[UserAssumptions::getIstart() + 2] = mBiyear3;
            mBiyear4 = atof(getTextBoxString(kBenD_Biyear4TextBox).c_str());
            Biproj::cpiincCheck(mBiyear4);
            mBiproj[UserAssumptions::getIstart() + 3] = mBiyear4;
            mBiyear5 = atof(getTextBoxString(kBenD_Biyear5TextBox).c_str());
            Biproj::cpiincCheck(mBiyear5);
            mBiproj[UserAssumptions::getIstart() + 4] = mBiyear5;
            mBiyear6 = atof(getTextBoxString(kBenD_Biyear6TextBox).c_str());
            Biproj::cpiincCheck(mBiyear6);
            mBiproj[UserAssumptions::getIstart() + 5] = mBiyear6;
            mBiyear7 = atof(getTextBoxString(kBenD_Biyear7TextBox).c_str());
            Biproj::cpiincCheck(mBiyear7);
            mBiproj[UserAssumptions::getIstart() + 6] = mBiyear7;
            mBiyear8 = atof(getTextBoxString(kBenD_Biyear8TextBox).c_str());
            Biproj::cpiincCheck(mBiyear8);
            mBiproj[UserAssumptions::getIstart() + 7] = mBiyear8;
            mBiyear9 = atof(getTextBoxString(kBenD_Biyear9TextBox).c_str());
            Biproj::cpiincCheck(mBiyear9);
            mBiproj[UserAssumptions::getIstart() + 8] = mBiyear9;
            mBiyear10 = atof(getTextBoxString(kBenD_Biyear10TextBox).c_str());
            Biproj::cpiincCheck(mBiyear10);
            mBiproj[UserAssumptions::getIstart() + 9] = mBiyear10;
            mBiult = atof(getTextBoxString(kBenD_BiultTextBox).c_str());
            Biproj::cpiincCheck(mBiult);
            mBiproj[UserAssumptions::getIstart() + 10] = mBiult;
            StartListening();
            // Values are good, so go back to top level
            LDialogBox::ListenToMessage( inMessage, ioParam );
         } catch (PiaException e) {
            StartListening();
            messageDialog(e.getNumber(), this);
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CBenincDialog::ListenToMessage for cmd_BenD_OKButton");
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

void CBenincDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_BenincDialog );
}

//  Save results from dialog

void CBenincDialog::SaveResults()
{
   int year;

   try {
      UserAssumptions *userAssumptions = theDoc->userAssumptions;
      CTextView *theTextView = theDoc->getTextView();
      for (int i = 0; i < 10; i++) {
         year = UserAssumptions::getIstart() + i;
         if (mBiproj[year] != userAssumptions->biproj[year]) {
            userAssumptions->biproj[year] = mBiproj[year];
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
      year = UserAssumptions::getIstart() + 10;
      double dtemp = mBiproj[year];
      if (dtemp != userAssumptions->biproj[year]) {
         for (int i = year; i <= userAssumptions->biproj.getLastYear(); i++) {
            userAssumptions->biproj[i] = dtemp;
         }
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CBenincDialog::SaveResults");
   }
}

// Get string in a text box

string CBenincDialog::getTextBoxString( long textBoxID )
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
