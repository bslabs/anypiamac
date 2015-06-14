// $Id: CWagebaseDialog.cp,v 1.4 2010/08/16 18:18:07 djolson Exp $
//  CWagebaseDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CTextView.h"
#include "CWagebaseDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "comma.h"
#include "FormatString.h"
#include "Resource.h"
#include "UserAssumptions.h"
#include "wbgenrl.h"
#include "wrkrdata.h"

using namespace std;

//  Create a dialog from a stream

CWagebaseDialog *CWagebaseDialog::CreateWagebaseDialogStream(
   LStream *inStream )
{
   return new CWagebaseDialog( inStream );
}

//  Create a dialog from a stream

CWagebaseDialog::CWagebaseDialog( LStream *inStream ) :
   LDialogBox( inStream ), mDAplwb(YEAR37, WorkerData::getMaxyear()),
   mDAolwb(YEAR37, WorkerData::getMaxyear())
{ }

//  Destroy a dialog

CWagebaseDialog::~CWagebaseDialog()
{ }

//  Set menu status

void CWagebaseDialog::FindCommandStatus(
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

void CWagebaseDialog::SetupDialog()
{
   StopListening();
   const WorkerData& workerData = *(theDoc->workerData);
   const UserAssumptions& userAssumptions = *(theDoc->userAssumptions);
   mStartyear = userAssumptions.getIstart() + 1;
   mLastyear = workerData.getEntDate().getYear();
   mYear = mStartyear;
   for (int year = mStartyear; year <= mLastyear; year++) {
      mDAplwb[year] = userAssumptions.baseOasdi[year];
      mDAolwb[year] = userAssumptions.base77[year];
   }
   setupdata();
   StartListening();
}

//  Listen to messages

void CWagebaseDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_WgbD_OKButton:
      {
         if (check())
            break;
         // Values are good, so go back to top level
         LDialogBox::ListenToMessage( inMessage, ioParam );
      }
      break;

      case cmd_WgbD_PrevyearButton:
      {
         if (check())
            break;
         if (mYear > mStartyear)
            mYear--;
         StopListening();
         setupdata();
         StartListening();
      }
      break;

      case cmd_WgbD_NextyearButton:
      {
         if (check())
            break;
         if (mYear < mLastyear)
            mYear++;
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

void CWagebaseDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_WagebaseDialog );
}

//  Save results from dialog

void CWagebaseDialog::SaveResults()
{
   int year;  // loop counter

   try {
      CTextView *theTextView = theDoc->getTextView();
      UserAssumptions *userAssumptions = theDoc->userAssumptions;
      for (year = mStartyear; year <= mLastyear; year++) {
         if (mDAplwb[year] != userAssumptions->baseOasdi[year]) {
            userAssumptions->baseOasdi[year] = mDAplwb[year];
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
      for (year = mStartyear; year <= mLastyear; year++) {
         if (mDAolwb[year] != userAssumptions->base77[year]) {
            userAssumptions->base77[year] = mDAolwb[year];
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CWagebaseDialog::SaveResults");
   }
}

// Get string in a text box

string CWagebaseDialog::getTextBoxString( long textBoxID )
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

void CWagebaseDialog::setupdata()
{
   LCaption *theCaption;  // handle to main caption
   string wagebasestring;  // main caption as a string
   LEditField *theTextBox;  // handle to text boxes
   LStdButton *thePrevyearButton;  // handle to previous year button
   LStdButton *theNextyearButton;  // handle to next year button

   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kWgbD_MainCaption));
   AfxFormatString1(wagebasestring, PIA_IDS_WAGEBASE,
      poutNoComma(mYear));
   LStr255 temp1(wagebasestring.c_str());
   theCaption->SetDescriptor(temp1);
   mPlwb = mDAplwb[mYear];
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kWgbD_PLTextBox));
   LStr255 temp2(poutNoComma(mPlwb, 3, 2).c_str());
   theTextBox->SetDescriptor(temp2);
   mOlwb = mDAolwb[mYear];
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kWgbD_OLTextBox));
   LStr255 temp3(poutNoComma(mOlwb, 3, 2).c_str());
   theTextBox->SetDescriptor(temp3);
   thePrevyearButton =
      dynamic_cast<LStdButton *>(FindPaneByID(kWgbD_PrevyearButton));
   theNextyearButton =
      dynamic_cast<LStdButton *>(FindPaneByID(kWgbD_NextyearButton));
   if (mYear <= mStartyear)
      thePrevyearButton->Disable();
   else
      thePrevyearButton->Enable();
   if (mYear >= mLastyear)
      theNextyearButton->Disable();
   else
      theNextyearButton->Enable();
}

bool CWagebaseDialog::check()
{
   try {
      mPlwb = atof(getTextBoxString(kWgbD_PLTextBox).c_str());
      WageBaseGeneral::check(mPlwb);
      mDAplwb[mYear] = mPlwb;
      mOlwb = atof(getTextBoxString(kWgbD_OLTextBox).c_str());
      WageBaseGeneral::check(mOlwb);
      mDAolwb[mYear] = mOlwb;
      return false;
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
      return true;
   }
}
