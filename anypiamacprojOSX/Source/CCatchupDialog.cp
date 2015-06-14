// $Id: CCatchupDialog.cp,v 1.3 2007/11/07 13:06:57 djolson Exp $
//  CCatchupDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CTextView.h"
#include "CCatchupDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "comma.h"
#include "UserAssumptions.h"

using namespace std;

//  Create a dialog from a stream

CCatchupDialog *CCatchupDialog::CreateCatchupDialogStream(
   LStream *inStream )
{
   return new CCatchupDialog( inStream );
}

//  Create a dialog from a stream

CCatchupDialog::CCatchupDialog( LStream *inStream ) :
   LDialogBox( inStream )
{ }

//  Destroy a dialog

CCatchupDialog::~CCatchupDialog()
{ }

//  Set menu status

void CCatchupDialog::FindCommandStatus(
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

void CCatchupDialog::SetupDialog()
{
   LCaption * theCaption;
   const char *month = "Dec ";  // month of benefit increase
   string temp;

   StopListening();
   UserAssumptions *userAssumptions = theDoc->userAssumptions;
   mCachup = userAssumptions->catchup;
   mEligyear = mCachup.getCstart();
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kCatD_Year1Caption));
   temp = month;
   temp += poutNoComma(mCachup.getCstart() + 2);
   LStr255 temp1(temp.c_str());
   theCaption->SetDescriptor(temp1);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kCatD_Year2Caption));
   temp = month;
   temp += poutNoComma(mCachup.getCstart() + 3);
   LStr255 temp2(temp.c_str());
   theCaption->SetDescriptor(temp2);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kCatD_Year3Caption));
   temp = month;
   temp += poutNoComma(mCachup.getCstart() + 4);
   LStr255 temp3(temp.c_str());
   theCaption->SetDescriptor(temp3);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kCatD_Year4Caption));
   temp = month;
   temp += poutNoComma(mCachup.getCstart() + 5);
   LStr255 temp4(temp.c_str());
   theCaption->SetDescriptor(temp4);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kCatD_Year5Caption));
   temp = month;
   temp += poutNoComma(mCachup.getCstart() + 6);
   LStr255 temp5(temp.c_str());
   theCaption->SetDescriptor(temp5);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kCatD_Year6Caption));
   temp = month;
   temp += poutNoComma(mCachup.getCstart() + 7);
   LStr255 temp6(temp.c_str());
   theCaption->SetDescriptor(temp6);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kCatD_Year7Caption));
   temp = month;
   temp += poutNoComma(mCachup.getCstart() + 8);
   LStr255 temp7(temp.c_str());
   theCaption->SetDescriptor(temp7);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kCatD_Year8Caption));
   temp = month;
   temp += poutNoComma(mCachup.getCstart() + 9);
   LStr255 temp8(temp.c_str());
   theCaption->SetDescriptor(temp8);
   setupdata();
   StartListening();
}

//  Listen to messages

void CCatchupDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_CatD_OKButton:
      {
         if (check())
            break;
         // Values are good, so go back to top level
         LDialogBox::ListenToMessage( inMessage, ioParam );
      }
      break;

      case cmd_CatD_PrevyearButton:
      {
         if (check())
            break;
         if (mEligyear > mCachup.getCstart())
            mEligyear--;
         StopListening();
         setupdata();
         StartListening();
      }
      break;

      case cmd_CatD_NextyearButton:
      {
         if (check())
            break;
         if (mEligyear < mCachup.getCstart() + 9)
            mEligyear++;
         StopListening();
         setupdata();
         StartListening();
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

void CCatchupDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_CatchupDialog );
}

//  Save results from dialog

void CCatchupDialog::SaveResults()
{
   try {
      UserAssumptions *userAssumptions = theDoc->userAssumptions;
      CTextView *theTextView = theDoc->getTextView();
      for (int i = mCachup.getCstart(); i < mCachup.getCstart() + 10; i++) {
         for (int j = mCachup.getCstart() + 2;
            j < mCachup.getCstart() + 10; j++) {
            if (mCachup.get(i, j) != userAssumptions->catchup.get(i, j)) {
               userAssumptions->catchup.set(i, j, mCachup.get(i, j));
               theDoc->valid = false;
               theTextView->SetDirty(true);
            }
         }
      }
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CCatchupDialog::SaveResults");
   }
}

// Get string in a text box

string CCatchupDialog::getTextBoxString( long textBoxID )
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

void CCatchupDialog::setupdata()
{
   LEditField *theTextBox;  // handle to text boxes
   LStdButton *thePrevyearButton;  // handle to previous year button
   LStdButton *theNextyearButton;  // handle to next year button

   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kCatD_EligyearTextBox));
   LStr255 temp1(poutNoComma(mEligyear).c_str());
   theTextBox->SetDescriptor(temp1);
   mCuyear1 = mCachup.get(mEligyear, mCachup.getCstart() + 2);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kCatD_Year1TextBox));
   LStr255 temp2(poutNoComma(mCuyear1, 2, 1).c_str());
   theTextBox->SetDescriptor(temp2);
   mCuyear2 = mCachup.get(mEligyear, mCachup.getCstart() + 3);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kCatD_Year2TextBox));
   LStr255 temp3(poutNoComma(mCuyear2, 2, 1).c_str());
   theTextBox->SetDescriptor(temp3);
   mCuyear3 = mCachup.get(mEligyear, mCachup.getCstart() + 4);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kCatD_Year3TextBox));
   LStr255 temp4(poutNoComma(mCuyear3, 2, 1).c_str());
   theTextBox->SetDescriptor(temp4);
   mCuyear4 = mCachup.get(mEligyear, mCachup.getCstart() + 5);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kCatD_Year4TextBox));
   LStr255 temp5(poutNoComma(mCuyear4, 2, 1).c_str());
   theTextBox->SetDescriptor(temp5);
   mCuyear5 = mCachup.get(mEligyear, mCachup.getCstart() + 6);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kCatD_Year5TextBox));
   LStr255 temp6(poutNoComma(mCuyear5, 2, 1).c_str());
   theTextBox->SetDescriptor(temp6);
   mCuyear6 = mCachup.get(mEligyear, mCachup.getCstart() + 7);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kCatD_Year6TextBox));
   LStr255 temp7(poutNoComma(mCuyear6, 2, 1).c_str());
   theTextBox->SetDescriptor(temp7);
   mCuyear7 = mCachup.get(mEligyear, mCachup.getCstart() + 8);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kCatD_Year7TextBox));
   LStr255 temp8(poutNoComma(mCuyear7, 2, 1).c_str());
   theTextBox->SetDescriptor(temp8);
   mCuyear8 = mCachup.get(mEligyear, mCachup.getCstart() + 9);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kCatD_Year8TextBox));
   LStr255 temp9(poutNoComma(mCuyear8, 2, 1).c_str());
   theTextBox->SetDescriptor(temp9);
   thePrevyearButton =
      dynamic_cast<LStdButton *>(FindPaneByID(kCatD_PrevyearButton));
   theNextyearButton =
      dynamic_cast<LStdButton *>(FindPaneByID(kCatD_NextyearButton));
   if (mEligyear <= mCachup.getCstart()) {
      thePrevyearButton->Disable();
   } else {
      thePrevyearButton->Enable();
   }
   if (mEligyear >= mCachup.getCstart() + 9) {
      theNextyearButton->Disable();
   } else {
      theNextyearButton->Enable();
   }
}

bool CCatchupDialog::check()
{
   try {
      mCuyear1 = atof(getTextBoxString(kCatD_Year1TextBox).c_str());
      Biproj::cpiincCheck(mCuyear1);
      mCachup.set(mEligyear, mCachup.getCstart() + 2, mCuyear1);
      mCuyear2 = atof(getTextBoxString(kCatD_Year2TextBox).c_str());
      Biproj::cpiincCheck(mCuyear2);
      mCachup.set(mEligyear, mCachup.getCstart() + 3, mCuyear2);
      mCuyear3 = atof(getTextBoxString(kCatD_Year3TextBox).c_str());
      Biproj::cpiincCheck(mCuyear3);
      mCachup.set(mEligyear, mCachup.getCstart() + 4, mCuyear3);
      mCuyear4 = atof(getTextBoxString(kCatD_Year4TextBox).c_str());
      Biproj::cpiincCheck(mCuyear4);
      mCachup.set(mEligyear, mCachup.getCstart() + 5, mCuyear4);
      mCuyear5 = atof(getTextBoxString(kCatD_Year5TextBox).c_str());
      Biproj::cpiincCheck(mCuyear5);
      mCachup.set(mEligyear, mCachup.getCstart() + 6, mCuyear5);
      mCuyear6 = atof(getTextBoxString(kCatD_Year6TextBox).c_str());
      Biproj::cpiincCheck(mCuyear6);
      mCachup.set(mEligyear, mCachup.getCstart() + 7, mCuyear6);
      mCuyear7 = atof(getTextBoxString(kCatD_Year7TextBox).c_str());
      Biproj::cpiincCheck(mCuyear7);
      mCachup.set(mEligyear, mCachup.getCstart() + 8, mCuyear7);
      mCuyear8 = atof(getTextBoxString(kCatD_Year8TextBox).c_str());
      Biproj::cpiincCheck(mCuyear8);
      mCachup.set(mEligyear, mCachup.getCstart() + 9, mCuyear8);
      return false;
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
      return true;
   }
}
