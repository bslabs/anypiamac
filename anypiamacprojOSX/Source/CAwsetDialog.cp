// $Id: CAwsetDialog.cp,v 1.5 2010/08/16 18:18:06 djolson Exp $
//  CAwsetDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CAwsetDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "comma.h"
#include "FormatString.h"
#include "Resource.h"
#include "piaparmsAny.h"
#include "UserAssumptions.h"
#include "wrkrdata.h"

using namespace std;

//  Create a dialog from a stream

CAwsetDialog *CAwsetDialog::CreateAwsetDialogStream(
   LStream *inStream )
{
   return new CAwsetDialog( inStream );
}

//  Create a dialog from a stream

CAwsetDialog::CAwsetDialog( LStream *inStream ) :
   LDialogBox( inStream ), mAwproj(YEAR37, WorkerData::getMaxyear())
{ }

//  Destroy a dialog

CAwsetDialog::~CAwsetDialog()
{ }

//  Set menu status

void CAwsetDialog::FindCommandStatus(
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

void CAwsetDialog::SetupDialog()
{
   LEditField * theTextBox;
   LCaption * theCaption;
   string temp;

   StopListening();
   AssumptionsNonFile& assumptions = *(theDoc->assumptions);
   mAwprojTitle = assumptions.getAverageWage(mAlt);
   assumptions.awincProj.setData(mAlt);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kStaD_AvgwageSetTextBox));
   LStr255 temp25(mAwprojTitle.c_str());
   theTextBox->SetDescriptor(temp25);
   for (int i = -1; i <= 10; i++) {
      int year = UserAssumptions::getIstart() + i;
      mAwproj[year] = assumptions.awincProj.awinc[year];
   }
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kStaD_Awyear1TextBox));
   mAwyear1 = mAwproj[UserAssumptions::getIstart() - 1];
   LStr255 temp1(poutNoComma(mAwyear1, 3, 2).c_str());
   theTextBox->SetDescriptor(temp1);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kStaD_Awyear2TextBox));
   mAwyear2 = mAwproj[UserAssumptions::getIstart()];
   LStr255 temp2(poutNoComma(mAwyear2, 3, 2).c_str());
   theTextBox->SetDescriptor(temp2);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kStaD_Awyear3TextBox));
   mAwyear3 = mAwproj[UserAssumptions::getIstart() + 1];
   LStr255 temp3(poutNoComma(mAwyear3, 3, 2).c_str());
   theTextBox->SetDescriptor(temp3);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kStaD_Awyear4TextBox));
   mAwyear4 = mAwproj[UserAssumptions::getIstart() + 2];
   LStr255 temp4(poutNoComma(mAwyear4, 3, 2).c_str());
   theTextBox->SetDescriptor(temp4);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kStaD_Awyear5TextBox));
   mAwyear5 = mAwproj[UserAssumptions::getIstart() + 3];
   LStr255 temp5(poutNoComma(mAwyear5, 3, 2).c_str());
   theTextBox->SetDescriptor(temp5);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kStaD_Awyear6TextBox));
   mAwyear6 = mAwproj[UserAssumptions::getIstart() + 4];
   LStr255 temp6(poutNoComma(mAwyear6, 3, 2).c_str());
   theTextBox->SetDescriptor(temp6);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kStaD_Awyear7TextBox));
   mAwyear7 = mAwproj[UserAssumptions::getIstart() + 5];
   LStr255 temp7(poutNoComma(mAwyear7, 3, 2).c_str());
   theTextBox->SetDescriptor(temp7);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kStaD_Awyear8TextBox));
   mAwyear8 = mAwproj[UserAssumptions::getIstart() + 6];
   LStr255 temp8(poutNoComma(mAwyear8, 3, 2).c_str());
   theTextBox->SetDescriptor(temp8);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kStaD_Awyear9TextBox));
   mAwyear9 = mAwproj[UserAssumptions::getIstart() + 7];
   LStr255 temp9(poutNoComma(mAwyear9, 3, 2).c_str());
   theTextBox->SetDescriptor(temp9);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kStaD_Awyear10TextBox));
   mAwyear10 = mAwproj[UserAssumptions::getIstart() + 8];
   LStr255 temp10(poutNoComma(mAwyear10, 3, 2).c_str());
   theTextBox->SetDescriptor(temp10);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kStaD_Awyear11TextBox));
   mAwyear11 = mAwproj[UserAssumptions::getIstart() + 9];
   LStr255 temp11(poutNoComma(mAwyear11, 3, 2).c_str());
   theTextBox->SetDescriptor(temp11);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kStaD_AwultTextBox));
   mAwult = mAwproj[UserAssumptions::getIstart() + 10];
   LStr255 temp12(poutNoComma(mAwult, 3, 2).c_str());
   theTextBox->SetDescriptor(temp12);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kStaD_Awyear1Caption));
   temp = poutNoComma(UserAssumptions::getIstart() - 1);
   LStr255 temp13(temp.c_str());
   theCaption->SetDescriptor(temp13);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kStaD_Awyear2Caption));
   temp = poutNoComma(UserAssumptions::getIstart());
   LStr255 temp14(temp.c_str());
   theCaption->SetDescriptor(temp14);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kStaD_Awyear3Caption));
   temp = poutNoComma(UserAssumptions::getIstart() + 1);
   LStr255 temp15(temp.c_str());
   theCaption->SetDescriptor(temp15);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kStaD_Awyear4Caption));
   temp = poutNoComma(UserAssumptions::getIstart() + 2);
   LStr255 temp16(temp.c_str());
   theCaption->SetDescriptor(temp16);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kStaD_Awyear5Caption));
   temp = poutNoComma(UserAssumptions::getIstart() + 3);
   LStr255 temp17(temp.c_str());
   theCaption->SetDescriptor(temp17);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kStaD_Awyear6Caption));
   temp = poutNoComma(UserAssumptions::getIstart() + 4);
   LStr255 temp18(temp.c_str());
   theCaption->SetDescriptor(temp18);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kStaD_Awyear7Caption));
   temp = poutNoComma(UserAssumptions::getIstart() + 5);
   LStr255 temp19(temp.c_str());
   theCaption->SetDescriptor(temp19);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kStaD_Awyear8Caption));
   temp = poutNoComma(UserAssumptions::getIstart() + 6);
   LStr255 temp20(temp.c_str());
   theCaption->SetDescriptor(temp20);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kStaD_Awyear9Caption));
   temp = poutNoComma(UserAssumptions::getIstart() + 7);
   LStr255 temp21(temp.c_str());
   theCaption->SetDescriptor(temp21);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kStaD_Awyear10Caption));
   temp = poutNoComma(UserAssumptions::getIstart() + 8);
   LStr255 temp22(temp.c_str());
   theCaption->SetDescriptor(temp22);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kStaD_Awyear11Caption));
   temp = poutNoComma(UserAssumptions::getIstart() + 9);
   LStr255 temp23(temp.c_str());
   theCaption->SetDescriptor(temp23);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kStaD_AwultCaption));
   AfxFormatString1(temp, PIA_IDS_ANDLATER,
      poutNoComma(UserAssumptions::getIstart() + 10));
   LStr255 temp24(temp.c_str());
   theCaption->SetDescriptor(temp24);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kStaD_SetCaption));
   temp = "Set ";
   temp += poutNoComma(mAlt);
   LStr255 temp26(temp.c_str());
   theCaption->SetDescriptor(temp26);
   StartListening();
}

//  Listen to messages

void CAwsetDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_StaD_OKButton:
      {
         try {
            StopListening();
            UserAssumptions *userAssumptions = theDoc->userAssumptions;
            mAwyear1 = atof(getTextBoxString(kStaD_Awyear1TextBox).c_str());
            Awinc::fqincCheck(mAwyear1);
            mAwproj[UserAssumptions::getIstart() - 1] = mAwyear1;
            mAwyear2 = atof(getTextBoxString(kStaD_Awyear2TextBox).c_str());
            Awinc::fqincCheck(mAwyear2);
            mAwproj[UserAssumptions::getIstart()] = mAwyear2;
            mAwyear3 = atof(getTextBoxString(kStaD_Awyear3TextBox).c_str());
            Awinc::fqincCheck(mAwyear3);
            mAwproj[UserAssumptions::getIstart() + 1] = mAwyear3;
            mAwyear4 = atof(getTextBoxString(kStaD_Awyear4TextBox).c_str());
            Awinc::fqincCheck(mAwyear4);
            mAwproj[UserAssumptions::getIstart() + 2] = mAwyear4;
            mAwyear5 = atof(getTextBoxString(kStaD_Awyear5TextBox).c_str());
            Awinc::fqincCheck(mAwyear5);
            mAwproj[UserAssumptions::getIstart() + 3] = mAwyear5;
            mAwyear6 = atof(getTextBoxString(kStaD_Awyear6TextBox).c_str());
            Awinc::fqincCheck(mAwyear6);
            mAwproj[UserAssumptions::getIstart() + 4] = mAwyear6;
            mAwyear7 = atof(getTextBoxString(kStaD_Awyear7TextBox).c_str());
            Awinc::fqincCheck(mAwyear7);
            mAwproj[UserAssumptions::getIstart() + 5] = mAwyear7;
            mAwyear8 = atof(getTextBoxString(kStaD_Awyear8TextBox).c_str());
            Awinc::fqincCheck(mAwyear8);
            mAwproj[UserAssumptions::getIstart() + 6] = mAwyear8;
            mAwyear9 = atof(getTextBoxString(kStaD_Awyear9TextBox).c_str());
            Awinc::fqincCheck(mAwyear9);
            mAwproj[UserAssumptions::getIstart() + 7] = mAwyear9;
            mAwyear10 = atof(getTextBoxString(kStaD_Awyear10TextBox).c_str());
            Awinc::fqincCheck(mAwyear10);
            mAwproj[UserAssumptions::getIstart() + 8] = mAwyear10;
            mAwyear11 = atof(getTextBoxString(kStaD_Awyear11TextBox).c_str());
            Awinc::fqincCheck(mAwyear11);
            mAwproj[UserAssumptions::getIstart() + 9] = mAwyear11;
            mAwult = atof(getTextBoxString(kStaD_AwultTextBox).c_str());
            Awinc::fqincCheck(mAwult);
            mAwproj[UserAssumptions::getIstart() + 10] = mAwult;
            StartListening();
            // Values are good, so go back to top level
            LDialogBox::ListenToMessage( inMessage, ioParam );
         } catch (PiaException e) {
            StartListening();
            messageDialog(e.getNumber(), this);
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CAwsetDialog::ListenToMessage for cmd_StaD_OKButton");
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

void CAwsetDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_AvgwageSetDialog );
}

//  Save results from dialog

void CAwsetDialog::SaveResults()
{
   int year;
   bool needUpdate = false;

   try {
      AssumptionsNonFile& assumptions = *(theDoc->assumptions);
      for (int i = -1; i <= 10; i++) {
         year = UserAssumptions::getIstart() + i;
         if (mAwproj[year] != assumptions.awincProj.awinc[year])
            needUpdate = true;
      }
      if (mAwprojTitle.compare(assumptions.getAverageWage(mAlt)) != 0)
         needUpdate = true;
      if (needUpdate) {
         assumptions.awincProj.setData(mAlt, mAwproj);
         assumptions.awincProj.setTitle(mAlt, mAwprojTitle);
      }
   } catch (PiaException& e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CAwsetDialog::SaveResults");
   }
}

// Get string in a text box

string CAwsetDialog::getTextBoxString( long textBoxID )
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
