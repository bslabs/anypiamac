// @doc CWORKERINFODIALOG
// $Id: CWorkerInfoDialog.cp,v 1.5 2008/04/28 14:08:54 djolson Exp $
// @module CWorkerInfoDialog.cp |
//
// Dialog to ask for basic worker information.

#include <utility>
#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CTextView.h"
#include "CWorkerInfoDialog.h"
#include "CMessageDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "Resource.h"
#include "comma.h"
#include "UserAssumptions.h"
#include "DateFormatter.h"
#include "PiaDataArray.h"
#include "boost/date_time/gregorian/parsers.hpp"

using namespace std;
using namespace std::rel_ops;

// @mfunc Create a dialog from a stream.

CWorkerInfoDialog *CWorkerInfoDialog::CreateWorkerInfoDialogStream(
   LStream *inStream )
{
   return new CWorkerInfoDialog( inStream );
}

// @mfunc Create a dialog from a stream.

CWorkerInfoDialog::CWorkerInfoDialog( LStream *inStream ) :
   LDialogBox( inStream )
{ }

// @mfunc Destroy a dialog.

CWorkerInfoDialog::~CWorkerInfoDialog()
{ }

// @mfunc Set menu status.

void CWorkerInfoDialog::FindCommandStatus(
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

// @mfunc Set up dialog.

void CWorkerInfoDialog::SetupDialog()
{
   LEditField *theTextBox;  // pointer to dialog text box

   StopListening();
   WorkerData *workerData = theDoc->workerData;
   PiaDataArray *widowArray = theDoc->widowArray;
   EarnProject *earnProject = theDoc->earnProject;
   mSex = workerData->getSex();
   mJoasdi = workerData->getJoasdi();
   mTotalize = workerData->getTotalize();
   mIndearn = workerData->getIndearn();
   mRailroad = workerData->getIndrr();
   mRRYear1 = workerData->railRoadData.getFirstYear();
   if (workerData->railRoadData.getEarn3750() > 0. || theDoc->hasrr3750)
      mRRYear1 = 1950;
   mRRYear2 = workerData->railRoadData.getLastYear();
   if (mRRYear2 < 1950)
      mRRYear2 = 1950;
   mIndMS = (workerData->milServDatesVec.getMSCount() > 0);
   mMSCount = workerData->milServDatesVec.getMSCount();
   mMqge = workerData->getMqge();
   mBlind = workerData->getBlindind();
   mDeemed = workerData->getDeemedind();
   mFammem = widowArray->getFamSize();
   SetValueForPaneID(kWkrD_FemaleRadio,
      (mSex == Sex::FEMALE) ? Button_On : Button_Off);
   SetValueForPaneID(kWkrD_MaleRadio,
      (mSex == Sex::MALE) ? Button_On : Button_Off);
   // Assume old-age if not set
   if (mJoasdi != WorkerData::OLD_AGE && mJoasdi != WorkerData::SURVIVOR &&
      mJoasdi != WorkerData::DISABILITY && mJoasdi != WorkerData::PEBS_CALC) {
      mJoasdi = WorkerData::OLD_AGE;
   }
   SetValueForPaneID(kWkrD_OldAgeRadio,
      (mJoasdi == WorkerData::OLD_AGE) ? Button_On : Button_Off);
   SetValueForPaneID(kWkrD_SurvivorRadio,
      (mJoasdi == WorkerData::SURVIVOR) ? Button_On : Button_Off);
   SetValueForPaneID(kWkrD_DisabilityRadio,
      (mJoasdi == WorkerData::DISABILITY) ? Button_On : Button_Off);
   SetValueForPaneID(kWkrD_PEBESRadio,
      (mJoasdi == WorkerData::PEBS_CALC) ? Button_On : Button_Off);
   SetValueForPaneID(kWkrD_TotalizeCheckBox,
      mTotalize ? Button_On : Button_Off);
   SetValueForPaneID(kWkrD_IndEarnCheckBox,
      mIndearn ? Button_Off : Button_On);
   SetValueForPaneID(kWkrD_RailroadCheckBox,
      mRailroad ? Button_On : Button_Off);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kWkrD_RRYear1TextBox));
   LStr255 temp11(poutNoComma(mRRYear1).c_str());
   theTextBox->SetDescriptor(temp11);
   if (mRailroad) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kWkrD_RRYear2TextBox));
   LStr255 temp12(poutNoComma(mRRYear2).c_str());
   theTextBox->SetDescriptor(temp12);
   if (mRailroad) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   SetValueForPaneID(kWkrD_IndMSCheckBox,
      mIndMS ? Button_On : Button_Off);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kWkrD_MSCountTextBox));
   LStr255 temp13(poutNoComma(mMSCount).c_str());
   theTextBox->SetDescriptor(temp13);
   if (mIndMS) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   SetValueForPaneID(kWkrD_MqgeCheckBox,
      mMqge ? Button_On : Button_Off);
   SetValueForPaneID(kWkrD_BlindCheckBox,
      mBlind ? Button_On : Button_Off);
   SetValueForPaneID(kWkrD_DeemedCheckBox,
      mDeemed ? Button_On : Button_Off);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kWkrD_NameTextBox));
   LStr255 temp1(workerData->getNhname().c_str());
   theTextBox->SetDescriptor(temp1);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kWkrD_SSN1TextBox));
   LStr255 temp2(workerData->ssn.getPart1().c_str());
   theTextBox->SetDescriptor(temp2);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kWkrD_SSN2TextBox));
   LStr255 temp3(workerData->ssn.getPart2().c_str());
   theTextBox->SetDescriptor(temp3);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kWkrD_SSN3TextBox));
   LStr255 temp4(workerData->ssn.getPart3().c_str());
   theTextBox->SetDescriptor(temp4);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kWkrD_BirthTextBox));
   LStr255 temp5(DateFormatter::toString(workerData->getBirthDate(),"s").c_str());
   theTextBox->SetDescriptor(temp5);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kWkrD_Address1TextBox));
   LStr255 temp6(workerData->getNhaddr(0).c_str());
   theTextBox->SetDescriptor(temp6);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kWkrD_Address2TextBox));
   LStr255 temp7(workerData->getNhaddr(1).c_str());
   theTextBox->SetDescriptor(temp7);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kWkrD_Address3TextBox));
   LStr255 temp8(workerData->getNhaddr(2).c_str());
   theTextBox->SetDescriptor(temp8);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kWkrD_FamMemTextBox));
   LStr255 temp9(poutNoComma(mFammem).c_str());
   theTextBox->SetDescriptor(temp9);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kWkrD_DeathTextBox));
   LStr255 temp10(DateFormatter::toString(workerData->getDeathDate(),"s").c_str());
   theTextBox->SetDescriptor(temp10);
   if (mJoasdi == WorkerData::SURVIVOR) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   StartListening();
}

// @mfunc Listen to messages.

void CWorkerInfoDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_WkrD_CancelButton:
      {
         LDialogBox::ListenToMessage( inMessage, ioParam );
      }
      break;
   
      case cmd_WkrD_OKButton:
      {
         try {
            StopListening();
            PiaData *piadata = theDoc->piadata;
            try {
               mDob = boost::gregorian::from_us_string(getTextBoxString(kWkrD_BirthTextBox));
            } catch (boost::gregorian::bad_year) {
               throw PiaException(PIA_IDS_BIRTH2);
            } catch (boost::gregorian::bad_month) {
               throw PiaException(PIA_IDS_BIRTH3);
            } catch (boost::gregorian::bad_day_of_month) {
               throw PiaException(PIA_IDS_BIRTH4);
            } catch (PiaException& e) {
               throw PiaException(PIA_IDS_BIRTH6);
            }
            if (mJoasdi == WorkerData::SURVIVOR) {
               try {
                  mDeath = boost::gregorian::from_us_string(
                     getTextBoxString(kWkrD_DeathTextBox).c_str());
               } catch (PiaException& e) {
                  throw PiaException(PIA_IDS_DEATH6);
               }
               WorkerData::deathCheck1(mDeath, mJoasdi);
               WorkerData::deathCheck2(mDeath);
            }
            if (mJoasdi == WorkerData::PEBS_CALC)
               WorkerData::birth1Check(mDob, UserAssumptions::getIstart());
            else
               WorkerData::birth2Check(mDob);
            if (mIndMS) {
               mMSCount = atoi(getTextBoxString(kWkrD_MSCountTextBox).c_str());
               MilServDatesVec::msCountCheck(mMSCount);
            }
            if (mRailroad) {
               mRRYear1 = atoi(getTextBoxString(kWkrD_RRYear1TextBox).c_str());
               mRRYear2 = atoi(getTextBoxString(kWkrD_RRYear2TextBox).c_str());
            }
            mFammem = atoi(getTextBoxString(kWkrD_FamMemTextBox).c_str());
            PiaDataArray::famSizeCheck(mFammem);
            LDialogBox::ListenToMessage( inMessage, ioParam );
            StartListening();
         } catch (PiaException& e) {
            StartListening();
            messageDialog(e.getNumber(), this);
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CWorkerDialog::ListenToMessage for cmd_WkrD_OKButton");
         }
      }
      break;

      case cmd_WkrD_FemaleRadio:
      {
         if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
            mSex = Sex::FEMALE;
         }
      }
      break;
      
      case cmd_WkrD_MaleRadio:
      {
         if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
            mSex = Sex::MALE;
         }
      }
      break;
      
      case cmd_WkrD_OldAgeRadio:
      {
         if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
            mJoasdi = WorkerData::OLD_AGE;
            StopListening();
            LEditField *theTextBox =
               dynamic_cast<LEditField *>(FindPaneByID(kWkrD_DeathTextBox));
            theTextBox->Disable();
            StartListening();
         }
      }
      break;
      
      case cmd_WkrD_SurvivorRadio:
      {
         if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
            mJoasdi = WorkerData::SURVIVOR;
            StopListening();
            LEditField *theTextBox =
               dynamic_cast<LEditField *>(FindPaneByID(kWkrD_DeathTextBox));
            theTextBox->Enable();
            StartListening();
         }
      }
      break;
      
      case cmd_WkrD_DisabilityRadio:
      {
         if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
            mJoasdi = WorkerData::DISABILITY;
            StopListening();
            LEditField *theTextBox =
               dynamic_cast<LEditField *>(FindPaneByID(kWkrD_DeathTextBox));
            theTextBox->Disable();
            StartListening();
         }
      }
      break;
      
      case cmd_WkrD_PEBESRadio:
      {
         if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
            mJoasdi = WorkerData::PEBS_CALC;
            StopListening();
            LEditField *theTextBox =
               dynamic_cast<LEditField *>(FindPaneByID(kWkrD_DeathTextBox));
            theTextBox->Disable();
            StartListening();
         }
      }
      break;
      
      case cmd_WkrD_TotalizeCheckBox:
      {
         mTotalize =
            ( GetValueForPaneID( kWkrD_TotalizeCheckBox ) == Button_On ) ?
            true : false;
      }
      break;

      case cmd_WkrD_IndEarnCheckBox:
      {
         mIndearn =
            ( GetValueForPaneID( kWkrD_IndEarnCheckBox ) == Button_On ) ?
            false : true;
      }
      break;
      
      case cmd_WkrD_RailroadCheckBox:
      {
         mRailroad =
            ( GetValueForPaneID( kWkrD_RailroadCheckBox ) == Button_On ) ?
            true : false;
         LEditField *theEditField =
            dynamic_cast<LEditField *>(FindPaneByID(kWkrD_RRYear1TextBox));
         if (mRailroad)
            theEditField->Enable();
         else
            theEditField->Disable();
         theEditField =
            dynamic_cast<LEditField *>(FindPaneByID(kWkrD_RRYear2TextBox));
         if (mRailroad)
            theEditField->Enable();
         else
            theEditField->Disable();
      }
      break;
      
      case cmd_WkrD_IndMSCheckBox:
      {
         mIndMS = ( GetValueForPaneID( kWkrD_IndMSCheckBox ) == Button_On ) ?
            true : false;
         LEditField *theEditField =
            dynamic_cast<LEditField *>(FindPaneByID(kWkrD_MSCountTextBox));
         if (mIndMS)
            theEditField->Enable();
         else
            theEditField->Disable();
      }
      break;
      
      case cmd_WkrD_MqgeCheckBox:
      {
         mMqge = ( GetValueForPaneID( kWkrD_MqgeCheckBox ) == Button_On ) ?
            true : false;
      }
      break;
      
      case cmd_WkrD_BlindCheckBox:
      {
         mBlind = ( GetValueForPaneID( kWkrD_BlindCheckBox ) == Button_On ) ?
            true : false;
      }
      break;
      
      case cmd_WkrD_DeemedCheckBox:
      {
         mDeemed = ( GetValueForPaneID( kWkrD_DeemedCheckBox ) == Button_On ) ?
            true : false;
      }
      break;

      default:
      {
         LDialogBox::ListenToMessage( inMessage, ioParam );
         break;
      }
   }
}

// @mfunc Connect to controls.

void CWorkerInfoDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_WorkerDialog );
}

// @mfunc Save results from dialog.

void CWorkerInfoDialog::SaveResults()
{
   CTextView *theTextView = theDoc->getTextView();
   try {
      StopListening();
      WorkerData *workerData = theDoc->workerData;
      PiaData *piadata = theDoc->piadata;
      PiaDataArray *widowArray = theDoc->widowArray;
      SecondaryArray *secondaryArray = theDoc->secondaryArray;
      EarnProject *earnProject = theDoc->earnProject;
      if (workerData->getBirthDate() != mDob) {
         workerData->setBirthDate(mDob);
         piadata->setBirthDateMinus1(workerData->getBirthDate());
         piadata->ageEnt.deleteContents();
         piadata->ageBen.deleteContents();
         piadata->fullRetAge.deleteContents();
         piadata->earlyRetAge.deleteContents();
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (workerData->getMqge() != mMqge) {
         workerData->setMqge(mMqge);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (workerData->getBlindind() != mBlind) {
         workerData->setBlindind(mBlind);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (workerData->getDeemedind() != mDeemed) {
         workerData->setDeemedind(mDeemed);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (workerData->getTotalize() != mTotalize) {
         workerData->setTotalize(mTotalize);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (workerData->getIndearn() != mIndearn) {
         workerData->setIndearn(mIndearn);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (workerData->getIndrr() != mRailroad) {
         workerData->setIndrr(mRailroad);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (workerData->getIndrr()) {
         if (mRRYear1 == 1950) {
            theDoc->hasrr3750 = true;
            if (mRRYear2 > 1950)
               mRRYear1 = 1951;
            else
               mRRYear1 = YEAR37;
         }
         if (workerData->railRoadData.getFirstYear() != mRRYear1) {
            workerData->railRoadData.setFirstYear(mRRYear1);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (workerData->railRoadData.getLastYear() != mRRYear2) {
            workerData->railRoadData.setLastYear(mRRYear2);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
      else {
         theDoc->hasrr3750 = false;
         workerData->railRoadData.setEarn3750(0.);
         workerData->railRoadData.setQc3750(0);
      }
      if (mIndMS) {
         if (workerData->milServDatesVec.getMSCount() != mMSCount) {
            workerData->milServDatesVec.setMSCount(mMSCount);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
      else {
         if (workerData->milServDatesVec.getMSCount() > 0) {
            workerData->milServDatesVec.setMSCount(0);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
      string str = getTextBoxString(kWkrD_Address1TextBox);
      if (workerData->getNhaddr(0) != str) {
         workerData->setNhaddr(0, str.c_str());
         // result is still valid, but file is dirty
         theTextView->SetDirty(true);
      }
      str = getTextBoxString(kWkrD_Address2TextBox);
      if (workerData->getNhaddr(1) != str) {
         workerData->setNhaddr(1, str.c_str());
         // result is still valid, but file is dirty
         theTextView->SetDirty(true);
      }
      str = getTextBoxString(kWkrD_Address3TextBox);
      if (workerData->getNhaddr(2) != str) {
         workerData->setNhaddr(2, str.c_str());
         // result is still valid, but file is dirty
         theTextView->SetDirty(true);
      }
      str = getTextBoxString(kWkrD_NameTextBox);
      if (workerData->getNhname() != str) {
         workerData->setNhname(str.c_str());
         // result is still valid, but file is dirty
         theTextView->SetDirty(true);
      }
      str = getTextBoxString(kWkrD_SSN1TextBox);
      if (workerData->ssn.getPart1() != str) {
         workerData->ssn.setPart1(str);
         // result is still valid, but file is dirty
         theTextView->SetDirty(true);
      }
      str = getTextBoxString(kWkrD_SSN2TextBox);
      if (workerData->ssn.getPart2() != str) {
         workerData->ssn.setPart2(str);
         // result is still valid, but file is dirty
         theTextView->SetDirty(true);
      }
      str = getTextBoxString(kWkrD_SSN3TextBox);
      if (workerData->ssn.getPart3() != str) {
         workerData->ssn.setPart3(str);
         // result is still valid, but file is dirty
         theTextView->SetDirty(true);
      }
      if (workerData->getSex() != mSex) {
         workerData->setSex((Sex::sex_type)mSex);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (workerData->getJoasdi() != mJoasdi) {
         workerData->setJoasdi(mJoasdi);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (workerData->getJoasdi() != WorkerData::SURVIVOR) {
         workerData->deleteDeathDate();
      }
      else {
         // must have at least 1 survivor
         if (mFammem < 1)
            mFammem = 1;
         if (workerData->getDeathDate()!= mDeath) {
            workerData->setDeathDate(mDeath);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
      if (workerData->getJoasdi() == WorkerData::PEBS_CALC) {
         // no family members for PEBS
         if (mFammem > 0) {
            mFammem = 0;
         }
         workerData->setRecalc(false);
         earnProject->setPebsData(UserAssumptions::getIstart(),
            workerData->getIbegin());
         workerData->setPebsData();
      }
      if (widowArray->getFamSize() != mFammem) {
         if (mFammem < widowArray->getFamSize()) {
            for (int i = mFammem; i < widowArray->getFamSize(); i++)
               secondaryArray->secondary[i]->deleteContents();
         }
         widowArray->setFamSize(mFammem);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (workerData->getJoasdi() == WorkerData::DISABILITY &&
         workerData->getValdi() == 0)
         workerData->setValdi(1);
      if (workerData->getJoasdi() == WorkerData::PEBS_CALC) {
         theDoc->piacal->pebsOabCal();
         theDoc->pebsOut->pebsWorkerInfo.prepareStrings();
      }
      else
         theDoc->piaOut->summary1Page.prepareStrings();
      StartListening();
   } catch (PiaException e) {
      StartListening();
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CWorkerDialog::SaveResults");
   }
}

// @mfunc Get string in a text box.

string CWorkerInfoDialog::getTextBoxString( long textBoxID )
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
