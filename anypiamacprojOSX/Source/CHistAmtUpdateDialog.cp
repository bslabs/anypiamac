// $Id: CHistAmtUpdateDialog.cp,v 1.3 2007/11/07 13:06:58 djolson Exp $
//  CHistAmtUpdateDialog.cp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CHistAmtUpdateDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "comma.h"
#include "FormatString.h"
#include "Resource.h"
#include "wbgenrl.h"

using namespace std;

//  Create a dialog from a stream

CHistAmtUpdateDialog *CHistAmtUpdateDialog::CreateHistAmtUpdateDialogStream(
   LStream *inStream )
{
   return new CHistAmtUpdateDialog( inStream );
}

//  Create a dialog from a stream

CHistAmtUpdateDialog::CHistAmtUpdateDialog( LStream *inStream ) :
   LDialogBox( inStream )
{ }

//  Destroy a dialog

CHistAmtUpdateDialog::~CHistAmtUpdateDialog()
{ }

//  Set menu status

void CHistAmtUpdateDialog::FindCommandStatus(
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

void CHistAmtUpdateDialog::SetupDialog()
{
   LEditField *theTextBox;
   LCaption *theCaption;
   string histamtstring;

   StopListening();
   AwbiData *awbidat = theDoc->awbidat;
   mYear = awbidat->getLastYear() + 1;
   mWb = 0.;
   mWbold = 0.;
   mFq = 0.;
   mBi = 0.;
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kHauD_MainCaption));
   AfxFormatString1(histamtstring, PIA_IDS_HISTAMTUPDATE,
      poutNoComma(mYear));
   LStr255 temp5(histamtstring.c_str());
   theCaption->SetDescriptor(temp5);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kHauD_PLwbTextBox));
   LStr255 temp1(poutNoComma(mWb).c_str());
   theTextBox->SetDescriptor(temp1);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kHauD_OLwbTextBox));
   LStr255 temp2(poutNoComma(mWbold).c_str());
   theTextBox->SetDescriptor(temp2);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kHauD_AvgwageTextBox));
   LStr255 temp3(poutNoComma(mFq, 3, 2).c_str());
   theTextBox->SetDescriptor(temp3);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kHauD_BenincTextBox));
   LStr255 temp4(poutNoComma(mBi, 2, 1).c_str());
   theTextBox->SetDescriptor(temp4);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kHauD_PLwbCaption));
   AfxFormatString1(histamtstring, PIA_IDS_UPDATEWB,
      poutNoComma(mYear));
   LStr255 temp6(histamtstring.c_str());
   theCaption->SetDescriptor(temp6);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kHauD_OLwbCaption));
   AfxFormatString1(histamtstring, PIA_IDS_UPDATEWBOLD,
      poutNoComma(mYear));
   LStr255 temp7(histamtstring.c_str());
   theCaption->SetDescriptor(temp7);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kHauD_BenincCaption));
   if (mYear-1 > 1982)
      AfxFormatString1(histamtstring, PIA_IDS_UPDATEBI,
         poutNoComma(mYear-1));
   else
      AfxFormatString1(histamtstring, PIA_IDS_REVIEWBI,
         poutNoComma(mYear-1));
   LStr255 temp8(histamtstring.c_str());
   theCaption->SetDescriptor(temp8);
   theCaption =
      dynamic_cast<LCaption *>(FindPaneByID(kHauD_AvgwageCaption));
   AfxFormatString1(histamtstring, PIA_IDS_UPDATEAW,
      poutNoComma(mYear-2));
   LStr255 temp9(histamtstring.c_str());
   theCaption->SetDescriptor(temp9);
   StartListening();
}

//  Listen to messages

void CHistAmtUpdateDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_HauD_OKButton:
      {
         try {
            StopListening();
            mWb = atof(getTextBoxString(kHauD_PLwbTextBox).c_str());
            WageBaseGeneral::check(mWb);
            mWbold = atof(getTextBoxString(kHauD_OLwbTextBox).c_str());
            WageBaseGeneral::check(mWbold);
            mFq = atof(getTextBoxString(kHauD_AvgwageTextBox).c_str());
            WageBaseGeneral::check(mFq);
            mBi = atof(getTextBoxString(kHauD_BenincTextBox).c_str());
            Biproj::cpiincCheck(mBi);
            StartListening();
            // Values are good, so go back to top level
            LDialogBox::ListenToMessage( inMessage, ioParam );
         } catch (PiaException e) {
            StartListening();
            messageDialog(e.getNumber(), this);
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CHistAmtUpdateDialog::ListenToMessage for cmd_HauD_OKButton");
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

void CHistAmtUpdateDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_HistAmtUpdateDialog );
}

//  Save results from dialog

void CHistAmtUpdateDialog::SaveResults()
{
   try {
      AwbiData *awbidat = theDoc->awbidat;
      awbidat->setLastYear(mYear);
      awbidat->baseOasdi[mYear] = mWb;
      awbidat->base77[mYear] = mWbold;
      awbidat->fq[mYear-2] = mFq;
      awbidat->cpiinc[mYear-1] = mBi;
      theDoc->valid = false;
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CHistAmtUpdateDialog::SaveResults");
   }
}

// Get string in a text box

string CHistAmtUpdateDialog::getTextBoxString( long textBoxID )
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
