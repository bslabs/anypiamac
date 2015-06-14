// $Id: CDisabilityDialog.cp,v 1.7 2008/10/31 12:51:40 djolson Exp $
//  CDisabilityDialog.cp

#include <utility>
#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CTextView.h"
#include "CDisabilityDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "DisabPeriod.h"
#include "Resource.h"
#include "comma.h"
#include "DateFormatter.h"
#include "boost/date_time/gregorian/parsers.hpp"

using namespace std;
using namespace std::rel_ops;

//  Create a dialog from a stream

CDisabilityDialog *CDisabilityDialog::CreateDisabilityDialogStream(
   LStream *inStream )
{
   return new CDisabilityDialog( inStream );
}

//  Create a dialog from a stream

CDisabilityDialog::CDisabilityDialog( LStream *inStream ) :
   LDialogBox( inStream ), mPia(0.0f), mMfb(0.0f), mPia1(0.0f), mMfb1(0.0f)
{ }

//  Destroy a dialog

CDisabilityDialog::~CDisabilityDialog()
{ }

//  Set menu status

void CDisabilityDialog::FindCommandStatus(
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

void CDisabilityDialog::SetupDialog()
{
   LEditField *theTextBox;  // pointer to dialog text box
   LEditField *theOnset1TextBox;
   LEditField *theWaitper1TextBox;
   LEditField *thePriorent1TextBox;
   LEditField *theDicess1TextBox;

   StopListening();
   const WorkerData& workerData = *(theDoc->workerData);
   mValdi = workerData.getValdi();
   mIoasdi = workerData.getJoasdi();
   mBirthDate = workerData.getBirthDate();
   mDeathDate = workerData.getDeathDate();
   mEntDate = workerData.getEntDate();
   mBenDate = workerData.getBenefitDate();
   const DisabPeriod disabPeriod0 = workerData.disabPeriod[0];
   const DisabPeriod disabPeriod1 = workerData.disabPeriod[1];
   mOnset = disabPeriod0.getOnsetDate();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kDisD_OnsetTextBox));
   LStr255 temp1(DateFormatter::toString(mOnset,"s").c_str());
   theTextBox->SetDescriptor(temp1);
   mWaitper = disabPeriod0.getWaitperDate();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kDisD_WaitperTextBox));
   LStr255 temp2(DateFormatter::toString(mWaitper,"s").c_str());
   theTextBox->SetDescriptor(temp2);
   theTextBox->Enable();
   mPriorent = disabPeriod0.getEntDate();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kDisD_PriorentTextBox));
   LStr255 temp3(DateFormatter::toString(mPriorent,"s").c_str());
   theTextBox->SetDescriptor(temp3);
   theTextBox->Enable();
   mDicess = disabPeriod0.getCessationDate();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kDisD_DicessTextBox));
   LStr255 temp4(DateFormatter::toString(mDicess,"s").c_str());
   theTextBox->SetDescriptor(temp4);
   if (WorkerData::needDiCess(mValdi, mIoasdi)) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   mPia = disabPeriod0.getCessationPia();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kDisD_DipiaTextBox));
   LStr255 temp9(poutNoComma(mPia, 4, 2).c_str());
   theTextBox->SetDescriptor(temp9);
   if (WorkerData::needDiCess(mValdi, mIoasdi)) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   mMfb = disabPeriod0.getCessationMfb();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kDisD_DimfbTextBox));
   LStr255 temp10(poutNoComma(mMfb, 4, 2).c_str());
   theTextBox->SetDescriptor(temp10);
   if (WorkerData::needDiCess(mValdi, mIoasdi)) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   theOnset1TextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kDisD_Onset1TextBox));
   theWaitper1TextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kDisD_Waitper1TextBox));
   thePriorent1TextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kDisD_Priorent1TextBox));
   theDicess1TextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kDisD_Dicess1TextBox));
   if (mValdi > 1) {
      mOnset1 = disabPeriod1.getOnsetDate();
      LStr255 temp5(DateFormatter::toString(mOnset1,"s").c_str());
      theOnset1TextBox->SetDescriptor(temp5);
      theOnset1TextBox->Enable();
      mWaitper1 = disabPeriod1.getWaitperDate();
      LStr255 temp6(DateFormatter::toString(mWaitper1,"s").c_str());
      theWaitper1TextBox->SetDescriptor(temp6);
      theWaitper1TextBox->Enable();
      mPriorent1 = disabPeriod1.getEntDate();
      LStr255 temp7(DateFormatter::toString(mPriorent1,"s").c_str());
      thePriorent1TextBox->SetDescriptor(temp7);
      thePriorent1TextBox->Enable();
      mDicess1 = disabPeriod1.getCessationDate();
      LStr255 temp8(DateFormatter::toString(mDicess1,"s").c_str());
      theDicess1TextBox->SetDescriptor(temp8);
      theDicess1TextBox->Enable();
   }
   else {
      theOnset1TextBox->Disable();
      theWaitper1TextBox->Disable();
      thePriorent1TextBox->Disable();
      theDicess1TextBox->Disable();
   }
   mPia1 = disabPeriod1.getCessationPia();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kDisD_Dipia1TextBox));
   LStr255 temp11(poutNoComma(mPia1, 4, 2).c_str());
   theTextBox->SetDescriptor(temp11);
   if (mValdi > 1) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   mMfb1 = disabPeriod1.getCessationMfb();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kDisD_Dimfb1TextBox));
   LStr255 temp12(poutNoComma(mMfb1, 4, 2).c_str());
   theTextBox->SetDescriptor(temp12);
   if (mValdi > 1) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   StartListening();
}

//  Listen to messages

void CDisabilityDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_DisD_OKButton:
      {
         try {
            StopListening();
            PiaData *piadata = theDoc->piadata;
            try {
               mOnset = boost::gregorian::from_us_string(getTextBoxString(
                   kDisD_OnsetTextBox).c_str());
            } catch (PiaException& e) {
               throw PiaException(PIA_IDS_ONSET9);
            }
            DisabPeriod::onsetDateCheck(mOnset);
            WorkerData::disBirthEntCheck(mOnset, mBirthDate, mIoasdi, mEntDate, mBenDate);
            try {
               mWaitper = DateMoyr(getTextBoxString(kDisD_WaitperTextBox).c_str());
            } catch (PiaException& e) {
               throw PiaException(PIA_IDS_WAITPD7);
            }
            DisabPeriod::waitperDateCheck(mWaitper);
            try {
               mPriorent = DateMoyr(getTextBoxString(kDisD_PriorentTextBox).c_str());
            } catch (PiaException& e) {
               throw PiaException(PIA_IDS_PRRENT6);
            }
            DisabPeriod::entDateCheck(mPriorent);
            WorkerData::priorentCheck(mPriorent, mOnset, mEntDate, mIoasdi);
            if (mIoasdi != WorkerData::DISABILITY) {
               try {
                  mDicess = DateMoyr(getTextBoxString(kDisD_DicessTextBox).c_str());
               } catch (PiaException& e) {
                  throw PiaException(PIA_IDS_DICESS8);
               }
               DisabPeriod::cessationDateCheck(mDicess);
               WorkerData::cessationCheck2(mDicess, mOnset, mDeathDate,
                  mEntDate, mIoasdi);
               mPia = atof(getTextBoxString(kDisD_DipiaTextBox).c_str());
               mMfb = atof(getTextBoxString(kDisD_DimfbTextBox).c_str());
            }
            if (mValdi > 1) {
               try {
                  mOnset1 = boost::gregorian::from_us_string(getTextBoxString(kDisD_Onset1TextBox).c_str());
               } catch (PiaException& e) {
                  throw PiaException(PIA_IDS_ONSET10);
               }
               DisabPeriod::onsetDateCheck(mOnset1);
               WorkerData::dis1Check(mOnset1, mOnset, mValdi, mBirthDate);
               try {
                  mWaitper1 = DateMoyr(getTextBoxString(kDisD_Waitper1TextBox).c_str());
               } catch (PiaException& e) {
                  throw PiaException(PIA_IDS_WAITPD7);
               }
               DisabPeriod::waitperDateCheck(mWaitper1);
               try {
                  mPriorent1 = DateMoyr(getTextBoxString(kDisD_Priorent1TextBox).c_str());
               } catch (PiaException& e) {
                   throw PiaException(PIA_IDS_PRRENT6);
               }
               DisabPeriod::entDateCheck(mPriorent1);
               try {
                  mDicess1 = DateMoyr(getTextBoxString(kDisD_Dicess1TextBox).c_str());
               } catch (PiaException& e) {
                  throw PiaException(PIA_IDS_DICESS8);
               }
               DisabPeriod::cessationDateCheck(mDicess1);
               WorkerData::cessation1Check(mDicess1, mOnset1, mOnset);
               mPia1 = atof(getTextBoxString(kDisD_Dipia1TextBox).c_str());
               mMfb1 = atof(getTextBoxString(kDisD_Dimfb1TextBox).c_str());
            }
            StartListening();
            // Values are good, so go back to top level
            LDialogBox::ListenToMessage( inMessage, ioParam );
         } catch (PiaException& e) {
            StartListening();
            messageDialog(e.getNumber(), this);
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDisabilityDialog::ListenToMessage for cmd_DisD_OKButton");
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

void CDisabilityDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_DisabilityDialog );
}

//  Save results from dialog

void CDisabilityDialog::SaveResults()
{
   try {
      CTextView *theTextView = theDoc->getTextView();
      WorkerData& workerData = *(theDoc->workerData);
      const DisabPeriod disabPeriod0 = workerData.disabPeriod[0];
      const DisabPeriod disabPeriod1 = workerData.disabPeriod[1];
      if (mOnset != disabPeriod0.getOnsetDate()) {
         workerData.setOnsetDate(0, mOnset);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (mWaitper != disabPeriod0.getWaitperDate()) {
         workerData.setWaitperDate(0, mWaitper);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (mPriorent != disabPeriod0.getEntDate()) {
         workerData.setPriorentDateCheck(0, mPriorent);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (workerData.needDiCess()) {
         if (mDicess != disabPeriod0.getCessationDate()) {
            workerData.setCessationDate(0, mDicess);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (mPia != disabPeriod0.getCessationPia()) {
            workerData.setCessationPia(0, mPia);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (mMfb != disabPeriod0.getCessationMfb()) {
            workerData.setCessationMfb(0, mMfb);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
      if (mValdi > 1) {
         if (mOnset1 != disabPeriod1.getOnsetDate()) {
            workerData.setOnsetDate(1, mOnset1);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (mWaitper1 != disabPeriod1.getWaitperDate()) {
            workerData.setWaitperDate(1, mWaitper1);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (mPriorent1 != disabPeriod1.getEntDate()) {
            workerData.setPriorentDateCheck(1, mPriorent1);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (mDicess1 != disabPeriod1.getCessationDate()) {
            workerData.setCessationDate(1, mDicess1);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (mPia1 != disabPeriod1.getCessationPia()) {
            workerData.setCessationPia(1, mPia1);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (mMfb1 != disabPeriod1.getCessationMfb()) {
            workerData.setCessationMfb(1, mMfb1);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
      if (workerData.getJoasdi() == WorkerData::PEBS_CALC)
         theDoc->piacal->pebsOabCal();
      else
         theDoc->piaOut->summary1Page.prepareStrings();
   } catch (PiaException& e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CDisabilityDialog::SaveResults");
   }
}

// Get string in a text box

string CDisabilityDialog::getTextBoxString( long textBoxID )
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
