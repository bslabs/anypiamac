// $Id: CHistAmtModifyDialog.cp,v 1.4 2010/08/16 18:18:07 djolson Exp $
//  CHistAmtModifyDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CHistAmtModifyDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "comma.h"
#include "FormatString.h"
#include "Resource.h"
#include "wbgenrl.h"
#include "wrkrdata.h"

using namespace std;

//  Create a dialog from a stream

CHistAmtModifyDialog *CHistAmtModifyDialog::CreateHistAmtModifyDialogStream(
   LStream *inStream )
{
   return new CHistAmtModifyDialog( inStream );
}

//  Create a dialog from a stream

CHistAmtModifyDialog::CHistAmtModifyDialog( LStream *inStream ) :
   LDialogBox( inStream ), mDAwb(YEAR37, WorkerData::getMaxyear()),
   mDAwbold(YEAR37, WorkerData::getMaxyear()),
   mDAfq(YEAR37, WorkerData::getMaxyear()),
   mFAbi(YEAR51, WorkerData::getMaxyear())
{ }

//  Destroy a dialog

CHistAmtModifyDialog::~CHistAmtModifyDialog()
{ }

//  Set menu status

void CHistAmtModifyDialog::FindCommandStatus(
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

void CHistAmtModifyDialog::SetupDialog()
{
   StopListening();
   AwbiData *awbidat = theDoc->awbidat;
   mStartyear = theDoc->baseyearStored + 1;
   mLastyear = awbidat->getLastYear();
   for (unsigned i = mStartyear; i <= mLastyear; i++) {
      mDAwb[i] = awbidat->baseOasdi[i];
      mDAwbold[i] = awbidat->base77[i];
      mDAfq[i-2] = awbidat->fq[i-2];
      mFAbi[i-1] = awbidat->cpiinc[i-1];
   }
   mYear = mStartyear;
   setupdata();
   StartListening();
}

//  Listen to messages

void CHistAmtModifyDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_HamD_OKButton:
      {
         if (check())
            break;
         // Values are good, so go back to top level
         LDialogBox::ListenToMessage( inMessage, ioParam );
      }
      break;

      case cmd_HamD_NextyearButton:
      {
         if (check())
            return;
         if (mYear < mLastyear)
            mYear++;
         StopListening();
         setupdata();
         StartListening();
      }
      break;

      case cmd_HamD_PrevyearButton:
      {
         if (check())
            return;
         if (mYear > mStartyear)
            mYear--;
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

void CHistAmtModifyDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_HistAmtModDialog );
}

//  Save results from dialog

void CHistAmtModifyDialog::SaveResults()
{
   try {
      AwbiData *awbidat = theDoc->awbidat;
      for (unsigned i = mStartyear; i <= mLastyear; i++) {
         awbidat->setLastYear(i);
         awbidat->baseOasdi[i] = mDAwb[i];
         awbidat->base77[i] = mDAwbold[i];
         awbidat->fq[i-2] = mDAfq[i-2];
         awbidat->cpiinc[i-1] = mFAbi[i-1];
      }
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CHistAmtModifyDialog::SaveResults");
   }
}

// Get string in a text box

string CHistAmtModifyDialog::getTextBoxString( long textBoxID )
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

void CHistAmtModifyDialog::setupdata()
{
   LEditField *theTextBox;
   LCaption *theCaption;
   LStdButton *thePrevyearButton;
   LStdButton *theNextyearButton;
   string histamtstring;

   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kHamD_MainCaption));
   AfxFormatString1(histamtstring, PIA_IDS_HISTAMTUPDATE,
      poutNoComma(mYear));
   LStr255 temp5(histamtstring.c_str());
   theCaption->SetDescriptor(temp5);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kHamD_PLwbTextBox));
   LStr255 temp1(poutNoComma(mDAwb[mYear], 3, 0).c_str());
   theTextBox->SetDescriptor(temp1);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kHamD_OLwbTextBox));
   LStr255 temp2(poutNoComma(mDAwbold[mYear], 3, 0).c_str());
   theTextBox->SetDescriptor(temp2);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kHamD_AvgwageTextBox));
   LStr255 temp3(poutNoComma(mDAfq[mYear-2], 3, 2).c_str());
   theTextBox->SetDescriptor(temp3);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kHamD_BenincTextBox));
   LStr255 temp4(poutNoComma(mFAbi[mYear-1], 2, 1).c_str());
   theTextBox->SetDescriptor(temp4);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kHamD_PLwbCaption));
   AfxFormatString1(histamtstring, PIA_IDS_UPDATEWB,
      poutNoComma(mYear));
   LStr255 temp6(histamtstring.c_str());
   theCaption->SetDescriptor(temp6);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kHamD_OLwbCaption));
   AfxFormatString1(histamtstring, PIA_IDS_UPDATEWBOLD,
      poutNoComma(mYear));
   LStr255 temp7(histamtstring.c_str());
   theCaption->SetDescriptor(temp7);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kHamD_BenincCaption));
   if (mYear-1 > 1982)
      AfxFormatString1(histamtstring, PIA_IDS_UPDATEBI,
         poutNoComma(mYear-1));
   else
      AfxFormatString1(histamtstring, PIA_IDS_REVIEWBI,
         poutNoComma(mYear-1));
   LStr255 temp8(histamtstring.c_str());
   theCaption->SetDescriptor(temp8);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kHamD_AvgwageCaption));
   AfxFormatString1(histamtstring, PIA_IDS_UPDATEAW,
      poutNoComma(mYear-2));
   LStr255 temp9(histamtstring.c_str());
   theCaption->SetDescriptor(temp9);
   thePrevyearButton =
      dynamic_cast<LStdButton *>(FindPaneByID(kHamD_PrevyearButton));
   theNextyearButton =
      dynamic_cast<LStdButton *>(FindPaneByID(kHamD_NextyearButton));
   if (mYear <= mStartyear)
      thePrevyearButton->Disable();
   else
      thePrevyearButton->Enable();
   if (mYear >= mLastyear)
      theNextyearButton->Disable();
   else
      theNextyearButton->Enable();
}

bool CHistAmtModifyDialog::check()
{
   try {
      mWb = atof(getTextBoxString(kHamD_PLwbTextBox).c_str());
      WageBaseGeneral::check(mWb);
      mDAwb[mYear] = mWb;
      mWbold = atof(getTextBoxString(kHamD_OLwbTextBox).c_str());
      WageBaseGeneral::check(mWbold);
      mDAwbold[mYear] = mWbold;
      mFq = atof(getTextBoxString(kHamD_AvgwageTextBox).c_str());
      WageBaseGeneral::check(mFq);
      mDAfq[mYear] = mFq;
      mBi = atof(getTextBoxString(kHamD_BenincTextBox).c_str());
      Biproj::cpiincCheck(mBi);
      mFAbi[mYear] = mBi;
      return false;
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
      return true;
   }
}
