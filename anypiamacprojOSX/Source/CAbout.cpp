// $Id: CAbout.cpp,v 1.3 2007/11/07 13:06:56 djolson Exp $
//  CAbout.cpp

#include <LStdControl.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <LEditField.h>
#include <LString.h>

#include "CAbout.h"
#include "AppConstants.h"
#include "PiaException.h"
#include "comma.h"

using namespace std;

//  Create a dialog from a stream

CAbout *CAbout::CreateAboutStream(
   LStream *inStream )
{
   return new CAbout( inStream );
}

//  Create a dialog from a stream

CAbout::CAbout( LStream *inStream ) :
   LDialogBox( inStream )
{ }

//  Destroy a dialog

CAbout::~CAbout()
{ }

//  Set menu status

void CAbout::FindCommandStatus(
   CommandT  inCommand,
   Boolean   &outEnabled,
   Boolean&  /* outUsesMark */,
   UInt16&   /* outMark */,
   Str255    /* outName */ )
{
   outEnabled = false;
   if ( inCommand == cmd_AboD ) {
      outEnabled = true;
   }
}

//  Set up dialog

void CAbout::SetupDialog()
{
   StopListening();
   StartListening();
}

//  Listen to messages

void CAbout::ListenToMessage(
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
               "Exception caught in CAbout::ListenToMessage for cmd_About_OKButton");
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

void CAbout::FinishCreateSelf()
{
   LDialogBox::FinishCreateSelf();
   UReanimator::LinkListenerToControls( this, this,
      rRidL_AboD );
}

//  Save results from dialog

void CAbout::SaveResults()
{
   try {
      // nothing to save yet
   } catch (...) {
      SignalStringLiteral_(
         "Exception caught in CAbout::SaveResults");
   }
}
