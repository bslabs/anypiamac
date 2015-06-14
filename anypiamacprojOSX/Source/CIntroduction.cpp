// $Id: CIntroduction.cpp,v 1.3 2007/11/07 13:06:58 djolson Exp $
//  CIntroduction.cpp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CIntroduction.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "comma.h"

using namespace std;

//  Create a dialog from a stream

CIntroduction *CIntroduction::CreateIntroductionStream(
   LStream *inStream )
{
   return new CIntroduction( inStream );
}

//  Create a dialog from a stream

CIntroduction::CIntroduction( LStream *inStream ) :
   LDialogBox( inStream )
{ }

//  Destroy a dialog

CIntroduction::~CIntroduction()
{ }

//  Set menu status

void CIntroduction::FindCommandStatus(
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

void CIntroduction::SetupDialog()
{
   StopListening();
   SetValueForPaneID(kIntr_NoIntroCheckBox, Button_Off);
   StartListening();
}

//  Listen to messages

void CIntroduction::ListenToMessage(
   MessageT  inMessage,
   void      *ioParam )
{
   switch ( inMessage ) {
   
      case cmd_Intr_OKButton:
      {
         try {
            StopListening();
            // nothing to enter
            StartListening();
            // Values are good, so go back to top level
            LDialogBox::ListenToMessage( inMessage, ioParam );
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CIntroduction::ListenToMessage for cmd_Intr_OKButton");
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

void CIntroduction::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_Introduction );
}

//  Save results from dialog

void CIntroduction::SaveResults()
{
   try {
      // nothing to save yet
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CIntroduction::SaveResults");
   }
}
