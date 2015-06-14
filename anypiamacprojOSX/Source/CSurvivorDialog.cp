// $Id: CSurvivorDialog.cp,v 1.5 2007/11/07 13:06:59 djolson Exp $
//  CSurvivorDialog.cp

#include <utility>
#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CTextView.h"
#include "CSurvivorDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "comma.h"
#include "Resource.h"
#include "WorkerDataArray.h"
#include "boost/date_time/gregorian/parsers.hpp"
#include "DateFormatter.h"

using namespace std;
using namespace std::rel_ops;

//  Create a dialog from a stream

CSurvivorDialog *CSurvivorDialog::CreateSurvivorDialogStream(
   LStream *inStream )
{
   return new CSurvivorDialog( inStream );
}

//  Create a dialog from a stream

CSurvivorDialog::CSurvivorDialog( LStream *inStream ) :
   LDialogBox( inStream )
{ }

//  Destroy a dialog

CSurvivorDialog::~CSurvivorDialog()
{ }

//  Set menu status

void CSurvivorDialog::FindCommandStatus(
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

void CSurvivorDialog::SetupDialog()
{
   StopListening();
   WorkerDataArray * widowDataArray = theDoc->widowDataArray;
   PiaDataArray * widowArray = theDoc->widowArray;
   mFamsize = widowArray->getFamSize();
   mFammem = 0;
   mIoasdi = theDoc->workerData->getJoasdi();
   for (int i = 0; i < mFamsize; i++) {
      WorkerData * widowDatat = widowDataArray->workerData[i];
      Secondary * secondaryt = theDoc->secondaryArray->secondary[i];
      mJbirth[i] = widowDatat->getBirthDate();
      mJonset[i] = widowDatat->disabPeriod[0].getOnsetDate();
      mEnt[i] = secondaryt->entDate;
      mSurv[i] = secondaryt->bic.getMajorBic();
      mYoung[i] = secondaryt->bic.getMinorBic();
   }
   setupdata();
   StartListening();
}

//  Listen to messages

void CSurvivorDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_SurD_OKButton:
      {
         if (check())
            break;
         // Values are good, so go back to top level
         LDialogBox::ListenToMessage( inMessage, ioParam );
      }
      break;

      case cmd_SurD_ChildSurvRadio:
         onChildSurv();
      break;

      case cmd_SurD_MotherRadio:
         onMother();
      break;

      case cmd_SurD_DisabWidRadio:
         onDisabWid();
      break;

      case cmd_SurD_DivDisabWidRadio:
         onDivDisabWid();
      break;

      case cmd_SurD_AgedWidRadio:
         onAgedWid();
      break;

      case cmd_SurD_DivAgedWidRadio:
         onDivAgedWid();
      break;

      case cmd_SurD_ChildLifeRadio:
         onChildLife();
      break;

      case cmd_SurD_YoungSpouseRadio:
         onYoungspouse();
      break;

      case cmd_SurD_AgedSpouseRadio:
         onAgedspouse();
      break;

      case cmd_SurD_DivAgedSpouseRadio:
         onDivAgedspouse();
      break;

      case cmd_SurD_PrevBenieButton:
         onPrevbenie();
      break;

      case cmd_SurD_NextBenieButton:
         onNextbenie();
      break;

      default:
      {
         LDialogBox::ListenToMessage( inMessage, ioParam );
         break;
      }
   }
}

// Set values and enable/disable boxes for child survivor choice

void CSurvivorDialog::onChildSurv()
{
   LEditField *theTextBox;  // handle to text box

   StopListening();
   mSurv[mFammem] = 'C';
   SetValueForPaneID(kSurD_MotherRadio, Button_Off);
   SetValueForPaneID(kSurD_DisabWidRadio, Button_Off);
   SetValueForPaneID(kSurD_DivDisabWidRadio, Button_Off);
   SetValueForPaneID(kSurD_AgedWidRadio, Button_Off);
   SetValueForPaneID(kSurD_DivAgedWidRadio, Button_Off);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kSurD_WidOnsetTextBox));
   theTextBox->Disable();
   StartListening();
}

// Set values and enable/disable boxes for mother/father choice

void CSurvivorDialog::onMother()
{
   LEditField *theTextBox;  // handle to text box

   StopListening();
   mSurv[mFammem] = 'E';
   SetValueForPaneID(kSurD_ChildSurvRadio, Button_Off);
   SetValueForPaneID(kSurD_DisabWidRadio, Button_Off);
   SetValueForPaneID(kSurD_DivDisabWidRadio, Button_Off);
   SetValueForPaneID(kSurD_AgedWidRadio, Button_Off);
   SetValueForPaneID(kSurD_DivAgedWidRadio, Button_Off);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kSurD_WidOnsetTextBox));
   theTextBox->Disable();
   StartListening();
}

// Set values and enable/disable boxes for disabled widow(er) choice

void CSurvivorDialog::onDisabWid()
{
   LEditField *theTextBox;  // handle to text box

   StopListening();
   mSurv[mFammem] = 'W';
   mYoung[mFammem] = ' ';
   SetValueForPaneID(kSurD_ChildSurvRadio, Button_Off);
   SetValueForPaneID(kSurD_MotherRadio, Button_Off);
   SetValueForPaneID(kSurD_DivDisabWidRadio, Button_Off);
   SetValueForPaneID(kSurD_AgedWidRadio, Button_Off);
   SetValueForPaneID(kSurD_DivAgedWidRadio, Button_Off);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kSurD_WidOnsetTextBox));
   theTextBox->Enable();
   StartListening();
}

// Set values and enable/disable boxes for divorced disabled widow(er) choice

void CSurvivorDialog::onDivDisabWid()
{
   LEditField *theTextBox;  // handle to text box

   StopListening();
   mSurv[mFammem] = 'W';
   mYoung[mFammem] = '6';
   SetValueForPaneID(kSurD_ChildSurvRadio, Button_Off);
   SetValueForPaneID(kSurD_MotherRadio, Button_Off);
   SetValueForPaneID(kSurD_DisabWidRadio, Button_Off);
   SetValueForPaneID(kSurD_AgedWidRadio, Button_Off);
   SetValueForPaneID(kSurD_DivAgedWidRadio, Button_Off);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kSurD_WidOnsetTextBox));
   theTextBox->Enable();
   StartListening();
}

// Set values and enable/disable boxes for aged widow(er) choice

void CSurvivorDialog::onAgedWid()
{
   LEditField *theTextBox;  // handle to text box

   StopListening();
   mSurv[mFammem] = 'D';
   mYoung[mFammem] = ' ';
   SetValueForPaneID(kSurD_ChildSurvRadio, Button_Off);
   SetValueForPaneID(kSurD_MotherRadio, Button_Off);
   SetValueForPaneID(kSurD_DisabWidRadio, Button_Off);
   SetValueForPaneID(kSurD_DivDisabWidRadio, Button_Off);
   SetValueForPaneID(kSurD_DivAgedWidRadio, Button_Off);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kSurD_WidOnsetTextBox));
   theTextBox->Disable();
   StartListening();
}


// Set values and enable/disable boxes for divorced aged widow(er) choice

void CSurvivorDialog::onDivAgedWid()
{
   LEditField *theTextBox;  // handle to text box

   StopListening();
   mSurv[mFammem] = 'D';
   mYoung[mFammem] = '6';
   SetValueForPaneID(kSurD_ChildSurvRadio, Button_Off);
   SetValueForPaneID(kSurD_MotherRadio, Button_Off);
   SetValueForPaneID(kSurD_DisabWidRadio, Button_Off);
   SetValueForPaneID(kSurD_DivDisabWidRadio, Button_Off);
   SetValueForPaneID(kSurD_AgedWidRadio, Button_Off);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kSurD_WidOnsetTextBox));
   theTextBox->Disable();
   StartListening();
}

// Set values and enable/disable boxes for child life choice

void CSurvivorDialog::onChildLife()
{
   StopListening();
   mSurv[mFammem] = 'C';
   SetValueForPaneID(kSurD_YoungSpouseRadio, Button_Off);
   SetValueForPaneID(kSurD_AgedSpouseRadio, Button_Off);
   SetValueForPaneID(kSurD_DivAgedSpouseRadio, Button_Off);
   StartListening();
}

// Set values and enable/disable boxes for young spouse choice

void CSurvivorDialog::onYoungspouse()
{
   StopListening();
   mSurv[mFammem] = 'B';
   mYoung[mFammem] = '2';
   SetValueForPaneID(kSurD_ChildLifeRadio, Button_Off);
   SetValueForPaneID(kSurD_AgedSpouseRadio, Button_Off);
   SetValueForPaneID(kSurD_DivAgedSpouseRadio, Button_Off);
   StartListening();
}

// Set values and enable/disable boxes for aged spouse choice

void CSurvivorDialog::onAgedspouse()
{
   StopListening();
   mSurv[mFammem] = 'B';
   mYoung[mFammem] = ' ';
   SetValueForPaneID(kSurD_ChildLifeRadio, Button_Off);
   SetValueForPaneID(kSurD_YoungSpouseRadio, Button_Off);
   SetValueForPaneID(kSurD_DivAgedSpouseRadio, Button_Off);
   StartListening();
}

// Set values and enable/disable boxes for divorced aged spouse choice

void CSurvivorDialog::onDivAgedspouse()
{
   StopListening();
   mSurv[mFammem] = 'B';
   mYoung[mFammem] = '6';
   SetValueForPaneID(kSurD_ChildLifeRadio, Button_Off);
   SetValueForPaneID(kSurD_YoungSpouseRadio, Button_Off);
   SetValueForPaneID(kSurD_AgedSpouseRadio, Button_Off);
   StartListening();
}

//  Connect to controls

void CSurvivorDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_SurvivorDialog );
}

//  Save results from dialog

void CSurvivorDialog::SaveResults()
{
   try {
      CTextView *theTextView = theDoc->getTextView();
      PiaData *piadata = theDoc->piadata;
      WorkerDataArray *widowDataArray = theDoc->widowDataArray;
      PiaDataArray *widowArray = theDoc->widowArray;
      for (int i = 0; i < widowArray->getFamSize(); i++) {
         WorkerData *widowData = widowDataArray->workerData[i];
         Secondary *secondaryData = theDoc->secondaryArray->secondary[i];
         if (secondaryData->bic.getMajorBic() != mSurv[i]) {
            secondaryData->bic.setMajorBic(mSurv[i]);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (secondaryData->bic.getMinorBic() != mYoung[i]) {
            secondaryData->bic.setMinorBic(mYoung[i]);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (widowData->getBirthDate() != mJbirth[i]) {
            widowData->setBirthDate(mJbirth[i]);
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (secondaryData->entDate != mEnt[i]) {
            secondaryData->entDate = mEnt[i];
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (secondaryData->bic.getMajorBic() == 'W') {
            if (widowData->disabPeriod[0].getOnsetDate() != mJonset[i]) {
               widowData->setOnsetDate(0, mJonset[i]);
               theDoc->valid = false;
               theTextView->SetDirty(true);
            }
         }
      }
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CSurvivorDialog::SaveResults");
   }
}

// Get string in a text box

string CSurvivorDialog::getTextBoxString( long textBoxID )
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

// Checks data for inconsistencies

bool CSurvivorDialog::check()
{
   try {
      try {
         mJbirth[mFammem] = boost::gregorian::from_us_string(
            getTextBoxString(kSurD_BirthTextBox).c_str());
         WorkerData::birth2Check(mJbirth[mFammem]);
      } catch (PiaException& e) {
         throw PiaException(PIA_IDS_JBIRTH5);
      }
      try {
         mEnt[mFammem] = DateMoyr(
            getTextBoxString(kSurD_EntdateTextBox).c_str());
         mEnt[mFammem].check();
      } catch (PiaException& e) {
         throw PiaException(PIA_IDS_ENT4);
      }
      if (mSurv[mFammem] == 'W') {
         try {
            mJonset[mFammem] = boost::gregorian::from_us_string(
               getTextBoxString(kSurD_WidOnsetTextBox).c_str());
         } catch (PiaException& e) {
            throw PiaException(PIA_IDS_JONSET7);
         }
         DisabPeriod::onsetDateCheck(mJonset[mFammem]);
         PiaCal::widowCheck(mJonset[mFammem], mJbirth[mFammem],
            mEnt[mFammem]);
      }
      return false;
   } catch (PiaException& e) {
      messageDialog(e.getNumber(), this);
      return true;
   }
}

// Handles click on previous benie button

void CSurvivorDialog::onPrevbenie()
{
   if (check())
      return;
   if (mFammem > 0)
      mFammem--;
   setupdata();
}

// Handles click on next benie button

void CSurvivorDialog::onNextbenie()
{
   if (check())
      return;
   if (mFammem < mFamsize - 1)
      mFammem++;
   setupdata();
}

// Sets up data for one family member, for display in dialog box

void CSurvivorDialog::setupdata()
{
   LEditField *theTextBox;
   LStdRadioButton *theRadioButton;

   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kSurD_NumFamMemTextBox));
   LStr255 temp1(poutNoComma(mFammem + 1).c_str());
   theTextBox->SetDescriptor(temp1);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kSurD_BirthTextBox));
   LStr255 temp2(DateFormatter::toString(mJbirth[mFammem],"s").c_str());
   theTextBox->SetDescriptor(temp2);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kSurD_WidOnsetTextBox));
   LStr255 temp3(DateFormatter::toString(mJonset[mFammem],"s").c_str());
   theTextBox->SetDescriptor(temp3);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kSurD_EntdateTextBox));
   LStr255 temp4(DateFormatter::toString(mEnt[mFammem],"s").c_str());
   theTextBox->SetDescriptor(temp4);
   if (mIoasdi == WorkerData::SURVIVOR) {
      // disable all the life benie radio buttons
      theRadioButton =
         dynamic_cast<LStdRadioButton *>(FindPaneByID(kSurD_ChildLifeRadio));
      theRadioButton->Disable();
      theRadioButton =
         dynamic_cast<LStdRadioButton *>(FindPaneByID(kSurD_YoungSpouseRadio));
      theRadioButton->Disable();
      theRadioButton =
         dynamic_cast<LStdRadioButton *>(FindPaneByID(kSurD_AgedSpouseRadio));
      theRadioButton->Disable();
      theRadioButton =
         dynamic_cast<LStdRadioButton *>(FindPaneByID(kSurD_DivAgedSpouseRadio));
      theRadioButton->Disable();
      switch (mSurv[mFammem]) {
   
         case 'C':
         {
            SetValueForPaneID(kSurD_ChildSurvRadio, Button_On);
            onChildSurv();
         }
         break;
   
         case 'E':
         {
            SetValueForPaneID(kSurD_MotherRadio, Button_On);
            onMother();
         }
         break;

         case 'W':
         {
            if (mYoung[mFammem] == '6') {
               SetValueForPaneID(kSurD_DivDisabWidRadio, Button_On);
               onDivDisabWid();
            } else {
               SetValueForPaneID(kSurD_DisabWidRadio, Button_On);
               onDisabWid();
            }
         }
         break;
   
         case 'D':
         {
            if (mYoung[mFammem] == '6') {
               SetValueForPaneID(kSurD_DivAgedWidRadio, Button_On);
               onDivAgedWid();
            } else {
               SetValueForPaneID(kSurD_AgedWidRadio, Button_On);
               onAgedWid();
            }
         }
         break;
         
         default:
         {
            SetValueForPaneID(kSurD_ChildSurvRadio, Button_On);
            onChildSurv();
         }
         break;
      }
   }
   else {
      theTextBox =
         dynamic_cast<LEditField *>(FindPaneByID(kSurD_WidOnsetTextBox));
      theTextBox->Disable();
      // disable all the survivor radio buttons
      theRadioButton =
         dynamic_cast<LStdRadioButton *>(FindPaneByID(kSurD_ChildSurvRadio));
      theRadioButton->Disable();
      theRadioButton =
         dynamic_cast<LStdRadioButton *>(FindPaneByID(kSurD_MotherRadio));
      theRadioButton->Disable();
      theRadioButton =
         dynamic_cast<LStdRadioButton *>(FindPaneByID(kSurD_DisabWidRadio));
      theRadioButton->Disable();
      theRadioButton =
         dynamic_cast<LStdRadioButton *>(FindPaneByID(kSurD_DivDisabWidRadio));
      theRadioButton->Disable();
      theRadioButton =
         dynamic_cast<LStdRadioButton *>(FindPaneByID(kSurD_AgedWidRadio));
      theRadioButton->Disable();
      theRadioButton =
         dynamic_cast<LStdRadioButton *>(FindPaneByID(kSurD_DivAgedWidRadio));
      theRadioButton->Disable();
      switch (mSurv[mFammem]) {
   
         case 'C':
         {
            SetValueForPaneID(kSurD_ChildLifeRadio, Button_On);
            onChildLife();
         }
         break;
   
         case 'B':
         {
            if (mYoung[mFammem] == '2') {
               SetValueForPaneID(kSurD_YoungSpouseRadio, Button_On);
               onYoungspouse();
            } else {
               if (mYoung[mFammem] == '6') {
                  SetValueForPaneID(kSurD_DivAgedSpouseRadio, Button_On);
                  onDivAgedspouse();
               } else {
                  SetValueForPaneID(kSurD_AgedSpouseRadio, Button_On);
                  onAgedspouse();
               }
            }
         }
         break;
         default:
         {
            SetValueForPaneID(kSurD_ChildLifeRadio, Button_On);
            onChildLife();
         }
         break;
      }
   }
   LStdButton *thePrevBenieButton =
      dynamic_cast<LStdButton *>(FindPaneByID(kSurD_PrevBenieButton));
   LStdButton *theNextBenieButton =
      dynamic_cast<LStdButton *>(FindPaneByID(kSurD_NextBenieButton));
   if (mFammem <= 0) {
      thePrevBenieButton->Disable();
   } else {
      thePrevBenieButton->Enable();
   }
   if (mFammem >= mFamsize - 1) {
      theNextBenieButton->Disable();
   } else {
	  theNextBenieButton->Enable();
   }
}
