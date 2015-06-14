// $Id: CMilservDialog.cp,v 1.5 2010/08/16 18:18:07 djolson Exp $
//  CMilservDialog.cp

#include <utility>
#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CTextView.h"
#include "CMilservDialog.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "CMessageDialog.h"
#include "Resource.h"
#include "DateFormatter.h"
#include "wrkrdata.h"

using namespace std;
using namespace std::rel_ops;

//  Create a dialog from a stream

CMilservDialog *CMilservDialog::CreateMilservDialogStream(
   LStream *inStream )
{
   return new CMilservDialog( inStream );
}

//  Create a dialog from a stream

CMilservDialog::CMilservDialog( LStream *inStream ) :
   LDialogBox( inStream )
{ }

//  Destroy a dialog

CMilservDialog::~CMilservDialog()
{ }

//  Set menu status

void CMilservDialog::FindCommandStatus(
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

void CMilservDialog::SetupDialog()
{
   LEditField *theTextBox;

   const WorkerData& workerData = *(theDoc->workerData);
   StopListening();
   mMSCount = workerData.milServDatesVec.getMSCount();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_Start1TextBox));
   LStr255 tempStart1(DateFormatter::toString(workerData.milServDatesVec.msdates[0].startDate,"s").c_str());
   theTextBox->SetDescriptor(tempStart1);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_End1TextBox));
   LStr255 tempEnd1(DateFormatter::toString(workerData.milServDatesVec.msdates[0].endDate,"s").c_str());
   theTextBox->SetDescriptor(tempEnd1);
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_Start2TextBox));
   LStr255 tempStart2(DateFormatter::toString(
      workerData.milServDatesVec.msdates[1].startDate,"s").c_str());
   theTextBox->SetDescriptor(tempStart2);
   if (mMSCount > 1)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_End2TextBox));
   LStr255 tempEnd2(DateFormatter::toString(
        workerData.milServDatesVec.msdates[1].endDate,"s").c_str());
   theTextBox->SetDescriptor(tempEnd2);
   if (mMSCount > 1)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_Start3TextBox));
   LStr255 tempStart3(DateFormatter::toString(
          workerData.milServDatesVec.msdates[2].startDate,"s").c_str());
   theTextBox->SetDescriptor(tempStart3);
   if (mMSCount > 2)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_End3TextBox));
   LStr255 tempEnd3(DateFormatter::toString(
           workerData.milServDatesVec.msdates[2].endDate,"s").c_str());
   theTextBox->SetDescriptor(tempEnd3);
   if (mMSCount > 2)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_Start4TextBox));
   LStr255 tempStart4(DateFormatter::toString(
      workerData.milServDatesVec.msdates[3].startDate,"s").c_str());
   theTextBox->SetDescriptor(tempStart4);
   if (mMSCount > 3)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_End4TextBox));
   LStr255 tempEnd4(DateFormatter::toString(
       workerData.milServDatesVec.msdates[3].endDate,"s").c_str());
   theTextBox->SetDescriptor(tempEnd4);
   if (mMSCount > 3)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_Start5TextBox));
   LStr255 tempStart5(DateFormatter::toString(
      workerData.milServDatesVec.msdates[4].startDate,"s").c_str());
   theTextBox->SetDescriptor(tempStart5);
   if (mMSCount > 4)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_End5TextBox));
   LStr255 tempEnd5(DateFormatter::toString(
      workerData.milServDatesVec.msdates[4].endDate,"s").c_str());
   theTextBox->SetDescriptor(tempEnd5);
   if (mMSCount > 4)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_Start6TextBox));
   LStr255 tempStart6(DateFormatter::toString(
      workerData.milServDatesVec.msdates[5].startDate,"s").c_str());
   theTextBox->SetDescriptor(tempStart6);
   if (mMSCount > 5)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_End6TextBox));
   LStr255 tempEnd6(DateFormatter::toString(
      workerData.milServDatesVec.msdates[5].endDate,"s").c_str());
   theTextBox->SetDescriptor(tempEnd6);
   if (mMSCount > 5)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_Start7TextBox));
   LStr255 tempStart7(DateFormatter::toString(
      workerData.milServDatesVec.msdates[6].startDate,"s").c_str());
   theTextBox->SetDescriptor(tempStart7);
   if (mMSCount > 6)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_End7TextBox));
   LStr255 tempEnd7(DateFormatter::toString(
      workerData.milServDatesVec.msdates[6].endDate,"s").c_str());
   theTextBox->SetDescriptor(tempEnd7);
   if (mMSCount > 6)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_Start8TextBox));
   LStr255 tempStart8(DateFormatter::toString(
      workerData.milServDatesVec.msdates[7].startDate,"s").c_str());
   theTextBox->SetDescriptor(tempStart8);
   if (mMSCount > 7)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_End8TextBox));
   LStr255 tempEnd8(DateFormatter::toString(
      workerData.milServDatesVec.msdates[7].endDate,"s").c_str());
   theTextBox->SetDescriptor(tempEnd8);
   if (mMSCount > 7)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_Start9TextBox));
   LStr255 tempStart9(DateFormatter::toString(
      workerData.milServDatesVec.msdates[8].startDate,"s").c_str());
   theTextBox->SetDescriptor(tempStart9);
   if (mMSCount > 8)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_End9TextBox));
   LStr255 tempEnd9(DateFormatter::toString(
      workerData.milServDatesVec.msdates[8].endDate,"s").c_str());
   theTextBox->SetDescriptor(tempEnd2);
   if (mMSCount > 8)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_Start10TextBox));
   LStr255 tempStart10(DateFormatter::toString(
      workerData.milServDatesVec.msdates[9].startDate,"s").c_str());
   theTextBox->SetDescriptor(tempStart10);
   if (mMSCount > 9)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_End10TextBox));
   LStr255 tempEnd10(DateFormatter::toString(
      workerData.milServDatesVec.msdates[9].endDate,"s").c_str());
   theTextBox->SetDescriptor(tempEnd10);
   if (mMSCount > 9)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_Start11TextBox));
   LStr255 tempStart11(DateFormatter::toString(
      workerData.milServDatesVec.msdates[10].startDate,"s").c_str());
   theTextBox->SetDescriptor(tempStart11);
   if (mMSCount > 10)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_End11TextBox));
   LStr255 tempEnd11(DateFormatter::toString(
      workerData.milServDatesVec.msdates[10].endDate,"s").c_str());
   theTextBox->SetDescriptor(tempEnd11);
   if (mMSCount > 10)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_Start12TextBox));
   LStr255 tempStart12(DateFormatter::toString(
      workerData.milServDatesVec.msdates[11].startDate,"s").c_str());
   theTextBox->SetDescriptor(tempStart12);
   if (mMSCount > 11)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_End12TextBox));
   LStr255 tempEnd12(DateFormatter::toString(
      workerData.milServDatesVec.msdates[11].endDate,"s").c_str());
   theTextBox->SetDescriptor(tempEnd12);
   if (mMSCount > 11)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_Start13TextBox));
   LStr255 tempStart13(DateFormatter::toString(
      workerData.milServDatesVec.msdates[12].startDate,"s").c_str());
   theTextBox->SetDescriptor(tempStart13);
   if (mMSCount > 12)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_End13TextBox));
   LStr255 tempEnd13(DateFormatter::toString(
      workerData.milServDatesVec.msdates[12].endDate,"s").c_str());
   theTextBox->SetDescriptor(tempEnd13);
   if (mMSCount > 12)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_Start14TextBox));
   LStr255 tempStart14(DateFormatter::toString(
      workerData.milServDatesVec.msdates[13].startDate,"s").c_str());
   theTextBox->SetDescriptor(tempStart14);
   if (mMSCount > 13)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_End14TextBox));
   LStr255 tempEnd14(DateFormatter::toString(
      workerData.milServDatesVec.msdates[13].endDate,"s").c_str());
   theTextBox->SetDescriptor(tempEnd14);
   if (mMSCount > 13)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_Start15TextBox));
   LStr255 tempStart15(DateFormatter::toString(
      workerData.milServDatesVec.msdates[14].startDate,"s").c_str());
   theTextBox->SetDescriptor(tempStart15);
   if (mMSCount > 14)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   theTextBox =
      dynamic_cast<LEditField *>(FindPaneByID(kMlsD_End15TextBox));
   LStr255 tempEnd15(DateFormatter::toString(
      workerData.milServDatesVec.msdates[14].endDate,"s").c_str());
   theTextBox->SetDescriptor(tempEnd15);
   if (mMSCount > 14)
      theTextBox->Enable();
   else
      theTextBox->Disable();
   StartListening();
}

//  Listen to messages

void CMilservDialog::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_MlsD_OKButton:
      {
         try {
            StopListening();
            PiaData *piadata = theDoc->piadata;
            setdata(0, getTextBoxString(kMlsD_Start1TextBox),
               getTextBoxString(kMlsD_End1TextBox));
            if (mMSCount > 1) {
               setdata(1, getTextBoxString(kMlsD_Start2TextBox),
                  getTextBoxString(kMlsD_End2TextBox));
            }
            if (mMSCount > 2) {
               setdata(2, getTextBoxString(kMlsD_Start3TextBox),
                  getTextBoxString(kMlsD_End3TextBox));
            }
            if (mMSCount > 3) {
               setdata(3, getTextBoxString(kMlsD_Start4TextBox),
                  getTextBoxString(kMlsD_End4TextBox));
            }
            if (mMSCount > 4) {
               setdata(4, getTextBoxString(kMlsD_Start5TextBox),
                  getTextBoxString(kMlsD_End5TextBox));
            }
            if (mMSCount > 5) {
               setdata(5, getTextBoxString(kMlsD_Start6TextBox),
                  getTextBoxString(kMlsD_End6TextBox));
            }
            if (mMSCount > 6) {
               setdata(6, getTextBoxString(kMlsD_Start7TextBox),
                  getTextBoxString(kMlsD_End7TextBox));
            }
            if (mMSCount > 7) {
               setdata(7, getTextBoxString(kMlsD_Start8TextBox),
                  getTextBoxString(kMlsD_End8TextBox));
            }
            if (mMSCount > 8) {
               setdata(8, getTextBoxString(kMlsD_Start9TextBox),
                  getTextBoxString(kMlsD_End9TextBox));
            }
            if (mMSCount > 9) {
               setdata(9, getTextBoxString(kMlsD_Start10TextBox),
                  getTextBoxString(kMlsD_End10TextBox));
            }
            if (mMSCount > 10) {
               setdata(10, getTextBoxString(kMlsD_Start11TextBox),
                  getTextBoxString(kMlsD_End11TextBox));
            }
            if (mMSCount > 11) {
               setdata(11, getTextBoxString(kMlsD_Start12TextBox),
                  getTextBoxString(kMlsD_End12TextBox));
            }
            if (mMSCount > 12) {
               setdata(12, getTextBoxString(kMlsD_Start13TextBox),
                  getTextBoxString(kMlsD_End13TextBox));
            }
            if (mMSCount > 13) {
               setdata(13, getTextBoxString(kMlsD_Start14TextBox),
                  getTextBoxString(kMlsD_End14TextBox));
            }
            if (mMSCount > 14) {
               setdata(14, getTextBoxString(kMlsD_Start15TextBox),
                  getTextBoxString(kMlsD_End15TextBox));
            }
            StartListening();
            // Values are good, so go back to top level
            LDialogBox::ListenToMessage( inMessage, ioParam );
         } catch (PiaException e) {
            StartListening();
            messageDialog(e.getNumber(), this);
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CMilservDialog::ListenToMessage for cmd_MlsD_OKButton");
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

void CMilservDialog::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_MilservDialog );
}

//  Save results from dialog

void CMilservDialog::SaveResults()
{
   try {
      CTextView *theTextView = theDoc->getTextView();
      WorkerData& workerData = *(theDoc->workerData);
      for (int i = 0; i < mMSCount; i++) {
         if (workerData.milServDatesVec.msdates[i].startDate !=
            msdates[i].startDate) {
            workerData.milServDatesVec.msdates[i].startDate =
               msdates[i].startDate;
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
         if (workerData.milServDatesVec.msdates[i].endDate !=
            msdates[i].endDate) {
            workerData.milServDatesVec.msdates[i].endDate =
               msdates[i].endDate;
            theDoc->valid = false;
            theTextView->SetDirty(true);
         }
      }
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CMilservDialog::SaveResults");
   }
}

// Get string in a text box

string CMilservDialog::getTextBoxString( long textBoxID )
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

// Set starting and ending dates from entered data

void CMilservDialog::setdata( int index, string startDate, string endDate )
{
   msdates[index].startDate = DateMoyr(startDate);
   MilServDates::startDateCheck(msdates[index].startDate);
   msdates[index].endDate = DateMoyr(endDate);
   MilServDates::endDateCheck(msdates[index].endDate);
   msdates[index].check();
}
