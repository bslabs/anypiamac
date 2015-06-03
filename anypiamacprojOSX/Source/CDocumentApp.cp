// $Id: CDocumentApp.cp,v 1.4 2007/11/07 13:06:57 djolson Exp $
// ===========================================================================
//	CDocumentApp.cp 		�1994-1999 Metrowerks Inc. All rights reserved.
// ===========================================================================
//	This file contains the starter code for a Document PowerPlant project

#include "CDocumentApp.h"

#include <LGrowZone.h>
#include <PP_Messages.h>
#include <PP_Resources.h>
#include <UDrawingState.h>
#include <UMemoryMgr.h>
#include <URegistrar.h>
#include <UEnvironment.h>
#include <UAttachments.h>
#include <LDocument.h>
#include <LUndoer.h>

#include <UStandardDialogs.h>
#if PP_StdDialogs_Option == PP_StdDialogs_Conditional
   #include <UConditionalDialogs.h>
#endif

#include <UControlRegistry.h>

#include <LWindow.h>
#include <LPrintout.h>
#include <LPlaceHolder.h>

#include "CTextView.h"
#include "CTextDocument.h"

#include <Appearance.h>
#include "AppConstants.h"
#include "CAbout.h"
#include "CWorkerInfoDialog.h"
#include "CPebesDialog.h"
#include "CNonPebesDialog.h"
#include "CDisabilityDialog.h"
#include "CSurvivorDialog.h"
#include "CMilservDialog.h"
#include "CSummaryQCDialog.h"
#include "CAssumptionsDialog.h"
#include "CBenincDialog.h"
#include "CCatchupDialog.h"
#include "CAvgwageDialog.h"
#include "CWagebaseDialog.h"
#include "CAnnearnDialog.h"
#include "CMessageDialog.h"
#include "CSpecYearDialog.h"
#include "CBaseyearDialog.h"
#include "CFoinfoDialog.h"
#include "CScreenDialog.h"
#include "CConfigDialog.h"
#include "CHistAmtModifyDialog.h"
#include "CHistAmtReviewDialog.h"
#include "CHistAmtUpdateDialog.h"
#include "CBisetDialog.h"
#include "CAwsetDialog.h"
#include "CCusetDialog.h"
#include "CIntroduction.h"
#include "PiaException.h"

using namespace std;

// ===========================================================================
//	� main
// ===========================================================================

int main()
{							
   // Set Debugging options
   SetDebugThrow_(debugAction_Alert);
   SetDebugSignal_(debugAction_Alert);

   // Initialize Memory Manager. Parameter is the number of
   // master pointer blocks to allocate
   InitializeHeap(3);
	
   // Initialize standard Toolbox managers
   UQDGlobals::InitializeToolbox(/*&qd*/);
	
   // Install a GrowZone to catch low-memory situations	
   new LGrowZone(20000);

   // Create the application object and run
   CDocumentApp theApp;
   theApp.Run();
	
   return 0;
}


// ---------------------------------------------------------------------------
//	� CDocumentApp									[public]
// ---------------------------------------------------------------------------
//	Application object constructor

CDocumentApp::CDocumentApp() : fOnlyDoc(NULL)
{
   // Register ourselves with the Appearance Manager
   if (UEnvironment::HasFeature(env_HasAppearance)) {
      ::RegisterAppearanceClient();
   }

   RegisterClasses();
	
   // Preload facilities for the Standard Dialogs
   PP_StandardDialogs::Load();
	
   // Require at least Navigation Services 1.1. See comments
   // above SetTryNavServices in UConditionalDialogs.cp for why
   // you might wish to do this.
#if PP_StdDialogs_Option == PP_StdDialogs_Conditional
   UConditionalDialogs::SetTryNavServices(0x01108000);
#endif
}


// ---------------------------------------------------------------------------
//	� ~CDocumentApp									[public, virtual]
// ---------------------------------------------------------------------------
//	Application object destructor

CDocumentApp::~CDocumentApp()
{
   // Clean up after Standard Dialogs
   PP_StandardDialogs::Unload();
}


// ---------------------------------------------------------------------------
//	� StartUp										[protected, virtual]
// ---------------------------------------------------------------------------
//	Perform an action in response to the Open Application AppleEvent.
//	Here, issue the New command to open a window.

void
CDocumentApp::StartUp()
{
   ObeyCommand(cmd_New, nil);
   // Show the introduction screen.  This is commented out because clicking
   // the OK button does not make the screen go away, and I don't know how
   // to fix it.
   //CIntroduction *theIntroduction;
   //theIntroduction = dynamic_cast<CIntroduction *>
   //   (LWindow::CreateWindow( rPPob_Introduction, this ));
   //Assert_(theIntroduction != nil);
   //theIntroduction->SetupDialog();
   //theIntroduction->Show();
}


// ---------------------------------------------------------------------------
//	� DoReopenApp									[protected, virtual]
// ---------------------------------------------------------------------------
//	Support the Finder's "re-open application" ('rapp') Apple Event. From
//	Apple TechNote 1102 (on Mac OS 8):
//
//	   The Finder now sends a 're-open application' Apple event ('rapp') to
//	   applications when the application is already running and it is opened
//	   from one of the Finder's windows (either by a double click or by
//	   selecting the application and choosing the Open command). Applications
//	   receiving a 'rapp' event (when they do not have any windows open) should
//	   open a new untitled document just as they would when processing an 'oapp'
//	   event.

void
CDocumentApp::DoReopenApp()
{
   // Given the suggested course of action by TN1102, the appropriate
   // action to take would be the equivalent of calling StartUp(). You
   // could call that here if you wish, but PowerPlant (nor this stationery)
   // will do that by default as StartUp() may contain code that one would
   // only wish to execute in response to an 'oapp' event, e.g. displaying
   // a splash screen. Additionally, 'rapp' should only open the new
   // untitled document if there are no other windows open.
   //
   // Given these circumstances, we'll create a new untitled document
   // (cmd_New) if no regular or modal windows are open (the TN didn't
   // address layers, but we'll assume this is what they mean).
	
   if ((UDesktop::FetchTopRegular() == nil)
      && (UDesktop::FetchTopModal() == nil)) {
      ObeyCommand(cmd_New, nil);
   }
}


// ------------------------------------------------------------------------
// OpenDocument  [protected, virtual]
// ------------------------------------------------------------------------
// This method is called when a file is chosen from the Open File dialog.

void
CDocumentApp::OpenDocument(
   FSSpec*  inMacFSSpec)
{
   LDocument* theDoc = LDocument::FindByFileSpec(*inMacFSSpec);
	
   // If the document is already open make it the current document,
   // else create a new document.
   if (theDoc != nil) {
      theDoc->MakeCurrent();
   } else {
      // close any open window, and open and repopulate a new window if the
      // close was successful
      if (fOnlyDoc)
         fOnlyDoc->AttemptClose(false);
      if (fOnlyDoc == NULL) {  // close succeeded?
         fOnlyDoc = new CTextDocument(this, inMacFSSpec);
         fOnlyDoc->Repopulate();
      }
   }
}


// ---------------------------------------------------------------------------
// MakeNewDocument  [protected, virtual]
// ---------------------------------------------------------------------------
// This method creates a new document and installs it into the application's
// Apple Event Object Model hierarchy.

LModelObject*
CDocumentApp::MakeNewDocument()
{
   // Make a new empty document.
   // return new CTextDocument(this, nil);
   // Close any open window, and open and repopulate a new window only if
   // the close was successful.
   if (fOnlyDoc)
      fOnlyDoc->AttemptClose(false);
   if (fOnlyDoc == NULL) { // did close succeed?
      fOnlyDoc = new CTextDocument(this, nil);
      fOnlyDoc->Repopulate();
   }
   return fOnlyDoc;
}


// ---------------------------------------------------------------------------
// ChooseDocument  [protected, virtual]
// ---------------------------------------------------------------------------
// This method uses the PowerPlant Standard Dialogs to let the user choose a
// document to open.

void
CDocumentApp::ChooseDocument()
{
   PP_StandardDialogs::LFileChooser	chooser;

   // Open any/all TEXT files
#ifdef PP_OpenPowerPlant_r21_or_higher
   NavDialogCreationOptions*	options = chooser.GetDialogOptions();
   if (options != nil) {
      options->optionFlags =	kNavDefaultNavDlogOptions
           //+ kNavSelectAllReadableItem
           //+ kNavNoTypePopup
         + kNavAllFilesInPopup;
   }
#else
   NavDialogOptions*	options = chooser.GetDialogOptions();
   if (options != nil) {
      options->dialogOptionFlags =	kNavDefaultNavDlogOptions
           //+ kNavSelectAllReadableItem
           //+ kNavNoTypePopup
         + kNavAllFilesInPopup;
   }
#endif

   if (chooser.AskOpenFile(LFileTypeList(ResType_Text))) {
      AEDescList docList;
      chooser.GetFileDescList(docList);
      SendAEOpenDocList(docList);
   }
}


// ---------------------------------------------------------------------------
// PrintDocument  [protected, virtual]
// ---------------------------------------------------------------------------
// This method is called in response to a Print request

void CDocumentApp::PrintDocument(
   FSSpec*  inMacFSSpec)
{
   // Close any open window, and open and repopulate a new window only if 
   // the close was successful
   if (fOnlyDoc)
      fOnlyDoc->AttemptClose(false);
   // Create a new document using the file spec
   if (fOnlyDoc == NULL) {  // did close succeed?
      fOnlyDoc = new CTextDocument(this, inMacFSSpec);
      fOnlyDoc->Repopulate();
   }

   // Tell the document to print.
   fOnlyDoc->DoPrint();
}

// ---------------------------------------------------------------------------
// RegisterClasses  [protected]
// ---------------------------------------------------------------------------
// To reduce clutter within the Application object's constructor, class
// registrations appear here in this seperate function for ease of use.

void CDocumentApp::RegisterClasses()
{
   // Register core PowerPlant classes.
   RegisterClass_(LWindow);
   RegisterClass_(LPrintout);
   RegisterClass_(LPlaceHolder);
   RegisterClass_(LKeyScrollAttachment);
   RegisterClass_(LColorEraseAttachment);
   RegisterClass_(LUndoer);
   RegisterClass_(LDialogBox);
   RegisterClass_(LCaption);
   RegisterClass_(LEditField);
   RegisterClass_(LStdButton);
   RegisterClass_(LStdCheckBox);
   RegisterClass_(LGroupBox);
   RegisterClass_(LStdRadioButton);
   RegisterClass_(LRadioGroup);
   RegisterClass_(LTabGroup);

   // Register the Appearance Manager/GA classes. You may want
   // to remove this use of UControlRegistry and instead perform
   // a "manual" registration of the classes. This cuts down on
   // extra code being linked in and streamlines your app and
   // project. However, use UControlRegistry as a reference/index
   // for your work, and ensure to check UControlRegistry against
   // your registrations each PowerPlant release in case
   // any mappings might have changed.
		
   UControlRegistry::RegisterClasses();

   // Register custom classes
   RegisterClass_(CTextView);
   RegisterClass_(CWorkerInfoDialog);
   RegisterClass_(CPebesDialog);
   RegisterClass_(CNonPebesDialog);
   RegisterClass_(CDisabilityDialog);
   RegisterClass_(CSurvivorDialog);
   RegisterClass_(CMilservDialog);
   RegisterClass_(CSummaryQCDialog);
   RegisterClass_(CAssumptionsDialog);
   RegisterClass_(CBenincDialog);
   RegisterClass_(CCatchupDialog);
   RegisterClass_(CAvgwageDialog);
   RegisterClass_(CWagebaseDialog);
   RegisterClass_(CAnnearnDialog);
   RegisterClass_(CMessageDialog);
   RegisterClass_(CSpecYearDialog);
   RegisterClass_(CBaseyearDialog);
   RegisterClass_(CFoinfoDialog);
   RegisterClass_(CConfigDialog);
   RegisterClass_(CScreenDialog);
   RegisterClass_(CHistAmtModifyDialog);
   RegisterClass_(CHistAmtReviewDialog);
   RegisterClass_(CHistAmtUpdateDialog);
   RegisterClass_(CBisetDialog);
   RegisterClass_(CAwsetDialog);
   RegisterClass_(CCusetDialog);
   RegisterClass_(CIntroduction);
   RegisterClass_(CAbout);
}
