// $Id: CHistAmtReviewDialog.cp,v 1.4 2010/08/16 18:18:07 djolson Exp $
//  CHistAmtReviewDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CHistAmtReviewDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "comma.h"
#include "FormatString.h"
#include "Resource.h"
#include "piaparmsAny.h"
#include "WageBase.h"
#include "wrkrdata.h"

using namespace std;

//  Create a dialog from a stream

CHistAmtReviewDialog *CHistAmtReviewDialog::CreateHistAmtReviewDialogStream(
   LStream *inStream )
{
   return new CHistAmtReviewDialog( inStream );
}

//  Create a dialog from a stream

CHistAmtReviewDialog::CHistAmtReviewDialog( LStream *inStream ) :
   LDialogBox( inStream ), mWb(YEAR37, WorkerData::getMaxyear()),
   mWbold(YEAR37, WorkerData::getMaxyear()),
   mFq(YEAR37, WorkerData::getMaxyear()),
   mBi(YEAR51, WorkerData::getMaxyear())
{ }

//  Destroy a dialog

CHistAmtReviewDialog::~CHistAmtReviewDialog()
{ }

//  Set menu status

void CHistAmtReviewDialog::FindCommandStatus(
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

void CHistAmtReviewDialog::SetupDialog()
{
   StopListening();
   const PiaParamsAny& piaparms = *(theDoc->piaparms);
   mStartyear = 1979;
   mLastyear = theDoc->awbidat->getLastYear();
   for (int i = mStartyear; i <= mLastyear; i++) {
      mWb[i] = piaparms.getBaseOasdi(i);
      mWbold[i] = piaparms.getBase77(i);
      mFq[i-2] = piaparms.getFq(i-2);
      mBi[i-1] = piaparms.getCpiinc(i-1);
   }
   mYear = mStartyear;
   setupdata();
   StartListening();
}

//  Listen to messages

void CHistAmtReviewDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_HarD_OKButton:
      {
         // Values are good, so go back to top level
         LDialogBox::ListenToMessage( inMessage, ioParam );
      }
      break;

      case cmd_HarD_NextyearButton:
      {
         if (mYear < mLastyear)
            mYear++;
         StopListening();
         setupdata();
         StartListening();
      }
      break;

      case cmd_HarD_PrevyearButton:
      {
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

void CHistAmtReviewDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_HistAmtRevDialog );
}

//  Save results from dialog

void CHistAmtReviewDialog::SaveResults()
{
   // Nothing to save
}

void CHistAmtReviewDialog::setupdata()
{
   LEditField *theTextBox;
   LCaption *theCaption;
   LStdButton *thePrevyearButton;
   LStdButton *theNextyearButton;
   string histamtstring;

   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kHarD_MainCaption));
   AfxFormatString1(histamtstring, PIA_IDS_HISTAMTUPDATE,
      poutNoComma(mYear));
   LStr255 temp5(histamtstring.c_str());
   theCaption->SetDescriptor(temp5);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kHarD_PLwbTextBox));
   LStr255 temp1(poutNoComma(mWb[mYear], 3, 0).c_str());
   theTextBox->SetDescriptor(temp1);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kHarD_OLwbTextBox));
   LStr255 temp2(poutNoComma(mWb[mYear], 3, 0).c_str());
   theTextBox->SetDescriptor(temp2);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kHarD_AvgwageTextBox));
   LStr255 temp3(poutNoComma(mFq[mYear-2], 3, 2).c_str());
   theTextBox->SetDescriptor(temp3);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kHarD_BenincTextBox));
   LStr255 temp4(poutNoComma(mBi[mYear-1], 2, 1).c_str());
   theTextBox->SetDescriptor(temp4);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kHarD_PLwbCaption));
   AfxFormatString1(histamtstring, PIA_IDS_UPDATEWB,
      poutNoComma(mYear));
   LStr255 temp6(histamtstring.c_str());
   theCaption->SetDescriptor(temp6);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kHarD_OLwbCaption));
   AfxFormatString1(histamtstring, PIA_IDS_UPDATEWBOLD,
      poutNoComma(mYear));
   LStr255 temp7(histamtstring.c_str());
   theCaption->SetDescriptor(temp7);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kHarD_BenincCaption));
   if (mYear-1 > 1982)
      AfxFormatString1(histamtstring, PIA_IDS_UPDATEBI,
         poutNoComma(mYear-1));
   else
      AfxFormatString1(histamtstring, PIA_IDS_REVIEWBI,
         poutNoComma(mYear-1));
   LStr255 temp8(histamtstring.c_str());
   theCaption->SetDescriptor(temp8);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kHarD_AvgwageCaption));
   AfxFormatString1(histamtstring, PIA_IDS_UPDATEAW,
      poutNoComma(mYear-2));
   LStr255 temp9(histamtstring.c_str());
   theCaption->SetDescriptor(temp9);
   thePrevyearButton =
      dynamic_cast<LStdButton *>(FindPaneByID(kHarD_PrevyearButton));
   theNextyearButton =
      dynamic_cast<LStdButton *>(FindPaneByID(kHarD_NextyearButton));
   if (mYear <= mStartyear)
      thePrevyearButton->Disable();
   else
      thePrevyearButton->Enable();
   if (mYear >= mLastyear)
      theNextyearButton->Disable();
   else
      theNextyearButton->Enable();
}
