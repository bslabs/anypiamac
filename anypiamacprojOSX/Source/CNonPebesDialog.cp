// $Id: CNonPebesDialog.cp,v 1.6 2010/08/16 18:18:07 djolson Exp $
//  CNonPebesDialog.cp

#include <utility>
#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CTextView.h"
#include "CNonPebesDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "comma.h"
#include "Resource.h"
#include "DateFormatter.h"
#include "wrkrdata.h"

using namespace std;
using namespace std::rel_ops;

//  Create a dialog from a stream

CNonPebesDialog *CNonPebesDialog::CreateNonPebesDialogStream(
   LStream *inStream )
{
   return new CNonPebesDialog( inStream );
}

//  Create a dialog from a stream

CNonPebesDialog::CNonPebesDialog( LStream *inStream ) :
   LDialogBox( inStream )
{ }

//  Destroy a dialog

CNonPebesDialog::~CNonPebesDialog()
{ }

//  Set menu status

void CNonPebesDialog::FindCommandStatus(
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

void CNonPebesDialog::SetupDialog()
{
   LEditField *theTextBox;  // pointer to dialog text box
   LStdRadioButton *theRadioButton;  // pointer to dialog radio button
   LStdCheckBox *theCheckBox;  // pointer to dialog check box

   StopListening();
   const WorkerData& workerData = *(theDoc->workerData);
   EarnProject *earnProject = theDoc->earnProject;
   mBirthyear = workerData.getBirthDate().year();
   mBirthdate = workerData.getBirthDate();
   mDeathdate = workerData.getDeathDate();
   mJoasdi = workerData.getJoasdi();
   mRecalc = workerData.getRecalc();
   SetValueForPaneID(kNpbD_LaterDateCheckBox,
      mRecalc ? Button_On : Button_Off);
   theCheckBox =
      dynamic_cast<LStdCheckBox *>(FindPaneByID(kNpbD_LaterDateCheckBox));
   if (mJoasdi != WorkerData::SURVIVOR) {
      theCheckBox->Enable();
   } else {
      theCheckBox->Disable();
   }
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kNpbD_EntdateTextBox));
   LStr255 temp2(DateFormatter::toString(workerData.getEntDate(),"s").c_str());
   if (mJoasdi != WorkerData::SURVIVOR) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   theTextBox->SetDescriptor(temp2);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kNpbD_BendateTextBox));
   LStr255 temp3(DateFormatter::toString(workerData.getBenefitDate(),"s").c_str());
   theTextBox->SetDescriptor(temp3);
   if (mRecalc || mJoasdi == WorkerData::SURVIVOR) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   miBackType = earnProject->getProjback();
   miFwrdType = earnProject->getProjfwrd();
   mIndearn = workerData.getIndearn();
   mbNonpen = PiaCal::getPubpenEnable(workerData);
   mbBack = ((miBackType > 0) && mIndearn);
   mbFwrd = ((miFwrdType > 0) && mIndearn);
   if (mbBack) {
      mBegin = earnProject->getFirstYear();
      mBackYear = workerData.getIbegin();
      mBackPerc = earnProject->getPercback();
   } else {
      mBegin = workerData.getIbegin();
      mBackYear = 1937;
      mBackPerc = 0.0f;
   }
   if (mbFwrd) {
      mEnd = earnProject->getLastYear();
      mFwrdYear = workerData.getIend();
      mFwrdPerc = earnProject->getPercfwrd();
   } else {
      mEnd = workerData.getIend();
      mFwrdYear = 2100;
      mFwrdPerc = 0.0f;
   }
   mNonpen = workerData.getPubpen();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kNpbD_NonpenTextBox));
   LStr255 temp10(poutNoComma(mNonpen, 4, 2).c_str());
   theTextBox->SetDescriptor(temp10);
   if (mbNonpen) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   mNonpendate = workerData.getPubpenDate();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kNpbD_NonpenDateBox));
   LStr255 temp11(DateFormatter::toString(workerData.getPubpenDate(),"s").c_str());
   theTextBox->SetDescriptor(temp11);
   if (mbNonpen) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   mValdi = workerData.getValdi();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kNpbD_ValdiTextBox));
   LStr255 temp5(poutNoComma(mValdi).c_str());
   theTextBox->SetDescriptor(temp5);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kNpbD_BeginTextBox));
   LStr255 temp1(poutNoComma(mBegin).c_str());
   theTextBox->SetDescriptor(temp1);
   if (mIndearn) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kNpbD_EndTextBox));
   LStr255 temp4(poutNoComma(mEnd).c_str());
   theTextBox->SetDescriptor(temp4);
   if (mIndearn) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kNpbD_BackYearTextBox));
   LStr255 temp6(poutNoComma(mBackYear).c_str());
   theTextBox->SetDescriptor(temp6);
   if (mbBack) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kNpbD_BackPercTextBox));
   LStr255 temp7(poutNoComma(mBackPerc, 3, 2).c_str());
   theTextBox->SetDescriptor(temp7);
   if (mbBack) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   SetValueForPaneID(kNpbD_BackType0Radio,
      (miBackType == 0) ? Button_On : Button_Off);
   theRadioButton =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kNpbD_BackType0Radio));
   if (mIndearn) {
      theRadioButton->Enable();
   } else {
      theRadioButton->Disable();
   }
   SetValueForPaneID(kNpbD_BackType1Radio,
      (miBackType == 1) ? Button_On : Button_Off);
   theRadioButton =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kNpbD_BackType1Radio));
   if (mIndearn) {
      theRadioButton->Enable();
   } else {
      theRadioButton->Disable();
   }
   SetValueForPaneID(kNpbD_BackType2Radio,
      (miBackType == 2) ? Button_On : Button_Off);
   theRadioButton =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kNpbD_BackType2Radio));
   if (mIndearn) {
      theRadioButton->Enable();
   } else {
      theRadioButton->Disable();
   }
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kNpbD_FwrdYearTextBox));
   LStr255 temp8(poutNoComma(mFwrdYear).c_str());
   theTextBox->SetDescriptor(temp8);
   if (mbFwrd) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kNpbD_FwrdPercTextBox));
   LStr255 temp9(poutNoComma(mFwrdPerc, 3, 2).c_str());
   theTextBox->SetDescriptor(temp9);
   if (mbFwrd) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   SetValueForPaneID(kNpbD_FwrdType0Radio,
      (miFwrdType == 0) ? Button_On : Button_Off);
   theRadioButton =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kNpbD_FwrdType0Radio));
   if (mIndearn) {
      theRadioButton->Enable();
   } else {
      theRadioButton->Disable();
   }
   SetValueForPaneID(kNpbD_FwrdType1Radio,
      (miFwrdType == 1) ? Button_On : Button_Off);
   theRadioButton =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kNpbD_FwrdType1Radio));
   if (mIndearn) {
      theRadioButton->Enable();
   } else {
      theRadioButton->Disable();
   }
   SetValueForPaneID(kNpbD_FwrdType2Radio,
      (miFwrdType == 2) ? Button_On : Button_Off);
   theRadioButton =
      dynamic_cast<LStdRadioButton *>(FindPaneByID(kNpbD_FwrdType2Radio));
   if (mIndearn) {
      theRadioButton->Enable();
   } else {
      theRadioButton->Disable();
   }
   StartListening();
}

//  Listen to messages

void CNonPebesDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_NpbD_CancelButton:
      {
         LDialogBox::ListenToMessage( inMessage, ioParam );
      }
      break;

      case cmd_NpbD_OKButton:
      {
         try {
            StopListening();
            PiaData *piadata = theDoc->piadata;
            if (mJoasdi != WorkerData::SURVIVOR) {
               try {
                  mEntdate = DateMoyr(getTextBoxString(kNpbD_EntdateTextBox));
               } catch (PiaException& e) {
                  throw PiaException(PIA_IDS_ENT4);
               }
               WorkerData::entCheck(mEntdate);
            }
            if (mRecalc || mJoasdi == WorkerData::SURVIVOR) {
               try {
                  mBendate = DateMoyr(getTextBoxString(kNpbD_BendateTextBox));
               } catch (PiaException& e) {
                  throw PiaException(PIA_IDS_BENDATE5);
               }
               WorkerData::benefitDate1Check(mBendate);
               if (mJoasdi != WorkerData::SURVIVOR) {
                  WorkerData::benefitDate2Check(mEntdate, mBendate);
               }
            }
            if (mIndearn) {
               mBegin = atoi(getTextBoxString(kNpbD_BeginTextBox).c_str());
               WorkerData::ibegin2Check(mBegin);
               mEnd = atoi(getTextBoxString(kNpbD_EndTextBox).c_str());
               WorkerData::iendCheck(mEnd);
               if (mbBack) {
                  mBackYear =
                     atoi(getTextBoxString(kNpbD_BackYearTextBox).c_str());
                  WorkerData::ibegin2Check(mBackYear);
                  mBackPerc =
                     atof(getTextBoxString(kNpbD_BackPercTextBox).c_str());
                  EarnProject::percbackCheck(mBackPerc);
               } else {
                  mBackYear = mBegin;
               }
               if (mbFwrd) {
                  mFwrdYear =
                     atoi(getTextBoxString(kNpbD_FwrdYearTextBox).c_str());
                  WorkerData::iendCheck(mFwrdYear);
                  mFwrdPerc =
                     atof(getTextBoxString(kNpbD_FwrdPercTextBox).c_str());
                  EarnProject::percfwrdCheck(mFwrdPerc);
               } else {
                  mFwrdYear = mEnd;
               }
               // Check for consistency among years
               EarnProject::ibeginCheck(mBackYear, mBegin);
               WorkerData::ibeginCheck(mBackYear, mBirthyear);
               EarnProject::yearsCheck(mBackYear, mBegin, mFwrdYear, mEnd);
               if (mbBack) {
                  if (miBackType == 0)
                     throw PiaException(PIA_IDS_PROJBACK2);
               }
               if (mbFwrd) {
                  if (miFwrdType == 0)
                     throw PiaException(PIA_IDS_PROJFWRD2);
               }
            }
            mNonpen = atof(getTextBoxString(kNpbD_NonpenTextBox).c_str());
            if (mNonpen > 0) {
               try {
                  mNonpendate = DateMoyr(getTextBoxString(kNpbD_NonpenDateBox));
                  mNonpendate.check();
               } catch (PiaException& e) {
                  throw PiaException(PIA_IDS_NONPENDATE3);
               }
               WorkerData::pubpenDateCheck(mNonpendate, mBirthdate,
                  mDeathdate);
            } else {
               mNonpendate = DateMoyr(0,0);
            }
            mValdi = atoi(getTextBoxString(kNpbD_ValdiTextBox).c_str());
            if (mJoasdi == WorkerData::DISABILITY) {
               WorkerData::valdi2Check(mValdi);
            } else {
               WorkerData::valdi1Check(mValdi);
            }
            LDialogBox::ListenToMessage( inMessage, ioParam );
            StartListening();
         } catch (PiaException e) {
            StartListening();
            messageDialog(e.getNumber(), this);
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CNonPebesDialog::ListenToMessage for cmd_NpbD_OKButton");
         }
      }
      break;

      case cmd_NpbD_LaterDateCheckBox:
      {
         mRecalc = (GetValueForPaneID(kNpbD_LaterDateCheckBox) == Button_On) ?
            true : false;
         LEditField *theTextBox =
            dynamic_cast<LEditField *>(FindPaneByID(kNpbD_BendateTextBox));
         if (mRecalc) {
            theTextBox->Enable();
         } else {
            theTextBox->Disable();
         }
      }
      break;

      case cmd_NpbD_BackType0Radio:
      {
         if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
            miBackType = 0;
            mbBack = false;
            OnBacktype();
         }
      }
      break;

      case cmd_NpbD_BackType1Radio:
      {
         if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
            miBackType = 1;
            mbBack = true;
            OnBacktype();
         }
      }
      break;

      case cmd_NpbD_BackType2Radio:
      {
         if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
            miBackType = 2;
            mbBack = true;
            OnBacktype();
         }
      }
      break;

      case cmd_NpbD_FwrdType0Radio:
      {
         if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
            miFwrdType = 0;
            mbFwrd = false;
            OnFwrdtype();
         }
      }
      break;

      case cmd_NpbD_FwrdType1Radio:
      {
         if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
            miFwrdType = 1;
            mbFwrd = true;
            OnFwrdtype();
         }
      }
      break;

      case cmd_NpbD_FwrdType2Radio:
      {
         if ( *(static_cast<SInt32 *>(ioParam)) == Button_On ) {
            miFwrdType = 2;
            mbFwrd = true;
            OnFwrdtype();
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

void CNonPebesDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_NonPebesDialog );
}

//  Save results from dialog

void CNonPebesDialog::SaveResults()
{
   try {
      CTextView *theTextView = theDoc->getTextView();
      WorkerData& workerData = *(theDoc->workerData);
      EarnProject *earnProject = theDoc->earnProject;
      if (workerData.getJoasdi() != WorkerData::SURVIVOR) {
         if (workerData.getEntDate() != mEntdate) {
            workerData.setEntDate(mEntdate);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
      if (workerData.getRecalc() != mRecalc) {
         workerData.setRecalc(mRecalc);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (mRecalc || workerData.getJoasdi() == WorkerData::SURVIVOR) {
         if (workerData.getBenefitDate() != mBendate) {
            workerData.setBenefitDate(mBendate);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      } else {
         workerData.setBenefitDate();
      }
      // Always check to set first year of earnings in earnProject
      if (workerData.getIndearn() && 
         earnProject->getFirstYear() != mBegin) {
         earnProject->setFirstYear(mBegin);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (mbBack) {
         if (workerData.getIbegin() != mBackYear) {
            workerData.setIbegin(mBackYear);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (earnProject->getPercback() != mBackPerc) {
            earnProject->setPercback(mBackPerc);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      } else {
         if (mIndearn && workerData.getIbegin() != mBegin) {
            workerData.setIbegin(mBegin);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
      // Always check to set last year of earnings in earnProject
      if (workerData.getIndearn() && 
         earnProject->getLastYear() != mEnd) {
         earnProject->setLastYear(mEnd);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (mbFwrd) {
         if (workerData.getIend() != mFwrdYear) {
            workerData.setIend(mFwrdYear);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (earnProject->getPercfwrd() != mFwrdPerc) {
            earnProject->setPercfwrd(mFwrdPerc);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      } else {
         if (mIndearn && workerData.getIend() != mEnd) {
            workerData.setIend(mEnd);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
      if (mbBack) {
         if (miBackType != earnProject->getProjback()) {
            earnProject->setProjback(miBackType);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
      if (mbFwrd) {
         if (miFwrdType != earnProject->getProjfwrd()) {
            earnProject->setProjfwrd(miFwrdType);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
      if (mNonpen != workerData.getPubpen()) {
         workerData.setPubpen((float)mNonpen);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (mNonpendate != workerData.getPubpenDate()) {
         if (mNonpen == 0.0) {
         	workerData.deletePubpenDate();
         } else {
            workerData.setPubpenDate(mNonpendate);
         }
         workerData.setPubpen((float)mNonpen);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      if (mValdi != workerData.getValdi()) {
         workerData.setValdi(mValdi);
         theDoc->valid = false;
         theTextView->SetDirty(true);
      }
      theDoc->piaOut->summary1Page.prepareStrings();
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CNonPebesDialog::SaveResults");
   }
}

// Get string in a text box

string CNonPebesDialog::getTextBoxString( long textBoxID )
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

// Description: Handles enabling/disabling due to button click on any
// backward projection.
void CNonPebesDialog::OnBacktype()
{
   LEditField *theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kNpbD_BackYearTextBox));
   if (mbBack) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kNpbD_BackPercTextBox));
   if (mbBack) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
}

// Description: Handles enabling/disabling due to button click on any
// forward projection.
void CNonPebesDialog::OnFwrdtype()
{
   LEditField *theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kNpbD_FwrdYearTextBox));
   if (mbFwrd) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kNpbD_FwrdPercTextBox));
   if (mbFwrd) {
      theTextBox->Enable();
   } else {
      theTextBox->Disable();
   }
}
