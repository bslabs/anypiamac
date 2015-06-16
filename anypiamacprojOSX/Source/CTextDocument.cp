// $Id: CTextDocument.cp,v 1.8 2008/10/31 12:51:40 djolson Exp $
// ==============================================================================
// CTextDocument.cp  ©1996-1998 Metrowerks Inc. All rights reserved.
// ==============================================================================
// A subclass of LSingleDoc for basic management of the document.
//
// Note that once the file is created, both data and resource fork are opened
// with exclusive read/write access. Furthermore, the forks are not closed
// until they need to be (e.g. document destruction). This not only adds
// some protection from deleting the file in the Finder, others modifying
// the file, etc., but it is also necessary for Navigation Services' use
// of the Translation Manager (see the dtor). Please keep this need/assumption
// in mind as you work with this class.

#include "CTextDocument.h"
#include "CDocumentApp.h"

#include <iostream>
#include <fstream>

#include <LFile.h>
#include <LPlaceHolder.h>
#include <LPrintout.h>
#include <LString.h>
#include <LWindow.h>
#include <PP_Messages.h>
#include <UMemoryMgr.h>
#include <UResourceMgr.h>
#include <UWindows.h>

#include "AppConstants.h"
#include "CTextView.h"
#include "CMessageDialog.h"
#include "TaxRatesPL.h"
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
#include "CAbout.h"
#include "PiaException.h"
#include "Resource.h"
#include "FormatString.h"
#include "OutMac.h"
#include "UserAssumptions.h"
#include "piaparmsAny.h"
#include "WorkerDataArray.h"
#include "LawChangeArray.h"

using namespace std;

// ---------------------------------------------------------------------------
//	CTextDocument  [public]
// ---------------------------------------------------------------------------
// Constructor

CTextDocument::CTextDocument(
   LCommander*  inSuper,
   FSSpec*      inFileSpec)

   : LSingleDoc(inSuper), valid(false), hasrr3750(false)
{
   // Create window for our document.
   mWindow = LWindow::CreateWindow(PPob_TextWindow, this );
   ThrowIfNil_(mWindow);
	
   // Specify that the text view should be the
   // target when the window is activated.
   mTextView = dynamic_cast<CTextView*>(mWindow->FindPaneByID(kTextView));
   Assert_(mTextView != nil);
   mWindow->SetLatentSub(mTextView);
   // Set linkage to text view
   mTextView->SetTextDoc(this);
	// Set pia variables
	NewPIA();

   mFileDesignator = nil;
	
   if (inFileSpec == nil) {
      NameNewDoc();  // Set the name of new window.
   } else {
      OpenFile(*inFileSpec);  // Display contents of file in window.
   }
	
   // Make the window visible.
   mWindow->Show();
}


// -----------------------------------------------------------------------------
//	~CTextDocument  [public, virtual]
// -----------------------------------------------------------------------------
//	Destructor

CTextDocument::~CTextDocument()
{
   try {
      TakeOffDuty();
		
      if ((mFile != nil) && (mFileDesignator != nil)) {
         // next two lines not needed because we access files through the
         // business code
         //mFile->CloseDataFork();
         //mFile->CloseResourceFork();
         mFileDesignator->CompleteSave();
      }
   }
		
   catch (...) { }
	
   delete mFileDesignator;

   // delete the linkage from the CDocumentApp to the CTextDocument that is
   // about to be deleted
   CDocumentApp *docApp =
      dynamic_cast<CDocumentApp *> (LApplication::GetTopCommander());
   if (docApp->fOnlyDoc) {
      Assert_(docApp->fOnlyDoc == this);
      docApp->fOnlyDoc = NULL;
   }
   // delete the pia data structures
   DeletePIA();
}


// ---------------------------------------------------------------------------
//	NameNewDoc  [protected, virtual]
// ---------------------------------------------------------------------------
//	Name a new, untitled document window
//
//	Untitled windows start with "untitled", then "untitled 2",
//	"untitled 3", etc. Old numbers are reused, so there won't be
//	gaps in the numbering. This follows Apple's HIG.
//
//	This routine uses a STR# resource to store the "untitled" string,
//	which can be localized to different languages. The first string
//	is "untitled" and the second is "untitled " (trailing space),
//	which is used when appending a number to the name.

void CTextDocument::NameNewDoc()
{
   // Start with the default name ("untitled")
   LStr255 name(STRx_DefaultDocumentTitle, DefaultDocumentTitle_Untitled);

   // Check if an existing window has this name. If so,
   // append a count ("untitled 2"). Keep incrementing the
   // count until we find a unique name.

   long num = 1;
   while (UWindows::FindNamedWindow(name) != nil) {
			
      // "untitled " (note trailing space)
      name.Assign(STRx_DefaultDocumentTitle, DefaultDocumentTitle_UntitledX);

      name += (++num);  // Increment and append count to name
   }		

   mWindow->SetDescriptor(name);		// Finally, set window title
}


// -----------------------------------------------------------------------------
//	OpenFile  [protected, virtual]
// -----------------------------------------------------------------------------
//	Open the specified file

void CTextDocument::OpenFile(
   FSSpec&  inFileSpec)
{
   if (mFile)
      delete mFile;
   mFile = nil;
	
   // Read the file into the pia data structures
   ifstream in;  // input stream for reading case
   
   // get the full pathname of the chosen filespec
   {
      // first, convert the FSSpec to an FSRef
      FSRef fsRef;
      OSErr err = FSpMakeFSRef(&inFileSpec, &fsRef);

      if (err != noErr)
      {
         messageDialog("OpenFile(): could not convert to FSRef", this);
         return;
      }

      // now convert the FSRef to a CFURL
      CFURLRef url = CFURLCreateFromFSRef(kCFAllocatorDefault, &fsRef);
      if (url == NULL)
      {
         messageDialog("OpenFile(): creating CFURL failed", this);
         return;
      }

      // and get a file system path (either HFS+ or POSIX) for the CFURL
      // standard C++ I/O is used by genfile.
      // for a Mach-O binary (using the BSD libc), those functions use POSIX paths ('/' path separator)
      // for PEF binaries (using the CodeWarrior MSL libc), HFS+ paths are needed (':' path separator)
#if __MACH__
      CFURLPathStyle pathStyle = kCFURLPOSIXPathStyle;
#else
      CFURLPathStyle pathStyle = kCFURLHFSPathStyle;
#endif

      CFStringRef path = CFURLCopyFileSystemPath(url, pathStyle);
      CFRelease(url);
      if (path == NULL)
      {
         messageDialog("OpenFile(): creating CFString failed", this);
         return;
      }

      // copy the CFString to a buffer
      char c[1024];
      CFStringGetCString(path, c, sizeof(c), kCFStringEncodingUTF8);
      c[sizeof(c)-1] = '\0';

      CFRelease(path);

      genfile.setCorename(c);
   }

   try {
      genfile.openin(in);
   } catch (PiaException e) {
      string cstring;  // temporary string
      AfxFormatString1(cstring, PIA_IDS_OPENERR,
         genfile.getPathname().c_str());
      messageDialog(cstring.c_str(), this);
      return;
   }
   DeleteContents();
   mTextView->SetDirty(true);
   //BeginWaitCursor();
   mFile = new LFile(inFileSpec);
   mWindow->SetDescriptor(inFileSpec.name);
   mIsSpecified = true;
   try {
      piaread->read(in);
      in.close();
      //EndWaitCursor();
      if (userAssumptions->getIaltaw() == AssumptionType::ALTERN_IIA ||
          userAssumptions->getIaltaw() == AssumptionType::PEBS_ASSUM) {
         userAssumptions->setIaltaw(AssumptionType::FLAT);
      }
      if (userAssumptions->getIaltbi() == AssumptionType::ALTERN_IIA ||
          userAssumptions->getIaltbi() == AssumptionType::PEBS_ASSUM) {
         userAssumptions->setIaltbi(AssumptionType::FLAT);
      }
      mTextView->SetDirty(false);
      DoCalculate();
      return;
   } catch (PiaException e) {
      messageDialog(e.getNumber(), this);
      return;
   }
}


// -----------------------------------------------------------------------------
//	IsModified  [public, virtual]
// -----------------------------------------------------------------------------
//	Return whether the Document is has changed since the last save

Boolean
CTextDocument::IsModified()
{
   // Document has changed if the text view is dirty.
   mIsModified = mTextView->IsDirty();
   return mIsModified;
}


// -----------------------------------------------------------------------------
//	DoAESave  [public, virtual]
// -----------------------------------------------------------------------------
//	Save Document in the specified file with the specified file type
//
//	If file type is fileType_Default, use the normal file type for
//	this document

void
CTextDocument::DoAESave(
   FSSpec&  inFileSpec,
   OSType   inFileType )
{
   // Delete the existing file object.
   // Note: this does nothing to the actual file on disk.
   if (mFile)
      delete mFile;

   // Make a new file object.
   mFile = new LFile(inFileSpec);

   // Get the proper file type.
   OSType theFileType = GetFileType();
   if (inFileType != fileType_Default) {
      theFileType = inFileType;
   }

   // Make new file on disk.
   mFile->CreateNewFile(kApplicationCreator, theFileType);

   // Write out the data.
   DoSave();

   // Change window title to reflect the new name.
   mWindow->SetDescriptor(inFileSpec.name);

   // Document now has a specified file.
   mIsSpecified = true;
}


// -----------------------------------------------------------------------------
// DoSave  [public, virtual]
// -----------------------------------------------------------------------------
// Save the entire Document to its associated File, which must already exist

void CTextDocument::DoSave()
{
   ofstream out;  // output stream for saving case

   FSSpec outFileSpec;

   mFile->GetSpecifier(outFileSpec);

   // get the full pathname of the chosen filespec
   {
      // first, convert the FSSpec to an FSRef
      FSRef fsRef;
      OSErr err = FSpMakeFSRef(&outFileSpec, &fsRef);

      if (err != noErr)
      {
         messageDialog("DoSave(): could not convert to FSRef", this);
         return;
      }

      // now convert the FSRef to a CFURL
      CFURLRef url = CFURLCreateFromFSRef(kCFAllocatorDefault, &fsRef);
      if (url == NULL)
      {
         messageDialog("DoSave(): creating CFURL failed", this);
         return;
      }

      // and get a file system path (either HFS+ or POSIX) for the CFURL
      // standard C++ I/O is used by genfile.
      // for a Mach-O binary (using the BSD libc), those functions use POSIX paths ('/' path separator)
      // for PEF binaries (using the CodeWarrior MSL libc), HFS+ paths are needed (':' path separator)
#if __MACH__
      CFURLPathStyle pathStyle = kCFURLPOSIXPathStyle;
#else
      CFURLPathStyle pathStyle = kCFURLHFSPathStyle;
#endif

      CFStringRef path = CFURLCopyFileSystemPath(url, pathStyle);
      CFRelease(url);
      if (path == NULL)
      {
         messageDialog("DoSave(): creating CFString failed", this);
         return;
      }

      // copy the CFString to a buffer
      char c[1024];
      CFStringGetCString(path, c, sizeof(c), kCFStringEncodingUTF8);
      c[sizeof(c)-1] = '\0';

      CFRelease(path);

      genfile.setCorename(c);
   }

#if 0
   // Get the full pathname of the chosen filespec
   unsigned char *p = (unsigned char *) GetFullPathName(outFileSpec);
   char c[1024];
   memcpy(c, p+1, p[0]);
   c[p[0]] = 0;
   genfile.setCorename(c);
#endif

   try {
      const DateMoyr entDate =
         (workerData->getJoasdi() == WorkerDataGeneral::SURVIVOR) ?
         secondaryArray->secondary[0]->entDate :
         workerData->getEntDate();
      // Check primary data and set primary factors
      piacal->dataCheck(entDate);
      // Check auxiliary data and set auxiliary factors
      piacal->dataCheckAux(*widowDataArray, *widowArray, *secondaryArray);
   } catch (PiaException& e) {
      messageDialog(e.getNumber(), this);
      return;
   }

   try {
      genfile.openout(out);
      //BeginWaitCursor();
      piawrite->write(out);
      out.close();
      //EndWaitCursor();
      // Saving makes doc clean.
      mTextView->SetDirty(false);
      return;
   } catch (PiaException& e) {
      string cstring;  // temporary string
      AfxFormatString1(cstring, PIA_IDS_OUTFILE,
         genfile.getPathname().c_str());
      messageDialog(cstring.c_str(), this);
      return;
   }
}


// -----------------------------------------------------------------------------
// DoRevert  [public, virtual]
// -----------------------------------------------------------------------------
// Revert the Document to the last saved version on disk

void CTextDocument::DoRevert()
{
   // Assumes document is already open

   // Read the entire file contents and close the file.
   StHandleBlock theTextH(mFile->ReadDataFork());

   StResource theStylH;

   // Put the contents in the text view and clear the dirty flag.
   mTextView->SetTextHandle(theTextH,
      reinterpret_cast<StScrpHandle>(theStylH.Get()));
   mTextView->SetDirty(false);

   // Refresh the text view.
   mTextView->Refresh();
}


// -----------------------------------------------------------------------------
// DoPrint  [public, virtual]
// -----------------------------------------------------------------------------

void CTextDocument::DoPrint()
{
   // Create the printout.
   StDeleter<LPrintout>	thePrintout(LPrintout::CreatePrintout(PPob_TextPrintout));
   ThrowIfNil_(thePrintout.Get());

   // Set the print record.
   //if (mPrintRecordH) {
      thePrintout->SetPrintSpec(mPrintSpec);
   //}

   // Get the text placeholder.
   LPlaceHolder* thePlaceholder = dynamic_cast<LPlaceHolder*>
      (thePrintout->FindPaneByID(kTextPlaceholder));
   ThrowIfNil_(thePlaceholder);

   mTextView->SetTextTraitsID(135);

   // Install the text view in the placeholder.
   thePlaceholder->InstallOccupant(mTextView, atNone);

   // Set the frame size.
   SetPrintFrameSize();

   // Print.
   thePrintout->DoPrintJob();
   
   mTextView->SetTextTraitsID(134);

   //mTextView->SetSize(12);   
   //mTextView->SetDirty(false);
   //Repopulate();
   // Delete the printout (handled automatically by the
   // StDeleter object). The text view is returned
   // to the window when the placeholder is destroyed.
}

// -----------------------------------------------------------------------------
// DoCalculate  [public, virtual]
// -----------------------------------------------------------------------------

void CTextDocument::DoCalculate()
{
   // find the date of entitlement to use to calculate the pia
   const DateMoyr entDate =
   (workerData->getJoasdi() == WorkerDataGeneral::SURVIVOR) ?
   secondaryArray->secondary[0]->entDate :
      workerData->getEntDate();
   // check primary data and set primary factors
   piacal->dataCheck(entDate);
   // check auxiliary data and set auxiliary factors
   piacal->dataCheckAux(*widowDataArray, *widowArray, *secondaryArray);
   // set assumptions
   piacal->calculate1(*assumptions);
   // compute regular pias and primary benefit
   piacal->calculate2(entDate);
   // compute any re-indexed widow(er) pias
   piacal->reindWidCalAll(*widowDataArray, *widowArray, *secondaryArray);
   // compute secondary benefits
   piacal->piaCal3(*widowArray, *secondaryArray);
   taxrates->taxCalAllYears(piadata->earnOasdiLimited,
      workerData->getTaxTypeArray(), taxes->taxesOasi, TaxRates::OASITF);
   taxrates->taxCalAllYears(piadata->earnOasdiLimited,
      workerData->getTaxTypeArray(), taxes->taxesDi, TaxRates::DITF);
   taxrates->taxCalAllYears(piadata->earnHiLimited, workerData->getTaxTypeArray(),
      taxes->taxesHi, TaxRates::HITF);
   taxes->oasdhiTaxCal();
   taxes->totalTaxCal();
   if (workerData->getJoasdi() == WorkerDataGeneral::PEBS_CALC) {
      pebsOut->setPageNum();
      pebsOut->prepareStrings();
   } else {
      piaOut->setPageNum(*piacal);
      piaOut->prepareStrings();
   }
   valid = true;
   // Update the screen
   Repopulate();
}

// ---------------------------------------------------------------------------
// AskSaveAs  [public]
// ---------------------------------------------------------------------------
//	Ask the user to save a Document and give it a name
//
//	Returns false if the user cancels the operation

Boolean CTextDocument::AskSaveAs(
   FSSpec&  outFSSpec,
   Boolean  inRecordIt)
{
   Boolean saveOK = false;

   PP_StandardDialogs::LFileDesignator* designator =
      new PP_StandardDialogs::LFileDesignator;

   designator->SetFileType( GetFileType() );

   Str255 defaultName;
   if (designator->AskDesignateFile(GetDescriptor(defaultName))) {

      designator->GetFileSpec(outFSSpec);

      if (designator->IsReplacing() && UsesFileSpec(outFSSpec)) {
         // User chose to replace the file with
         //   one of the same name.

         if (inRecordIt) {  // This is the same as a save
            SendSelfAE(kAECoreSuite, kAESave, ExecuteAE_No);
         }

         // Same document is still open, so we don't need to complete
         // the save. However, user may have selected different
         // translation options, so we need to delete the original
         // designator and store the new one.

         delete mFileDesignator;
         mFileDesignator = designator;

         DoSave();  // Save data to disk
         saveOK = true;

      }
      else {

         if (inRecordIt) {  // Recording for AppleScript
            SendAESaveAs(outFSSpec, GetFileType(), ExecuteAE_No);
         }

         if (designator->IsReplacing()) {
            ThrowIfOSErr_(::FSpDelete(&outFSSpec));
         }

         // Save data to new file. This also
         //    closes the original file
         DoAESave(outFSSpec, fileType_Default);

         if (mFileDesignator != nil) {
            // Complete save of original designator
            mFileDesignator->CompleteSave();
            delete mFileDesignator;
         }

         // Store new designator so we can
         //   complete save when we close the document
         mFileDesignator = designator;

         saveOK = true;

      }
   }
   return saveOK;
}


// -----------------------------------------------------------------------------
// SetPrintFrameSize  [protected, virtual]
// -----------------------------------------------------------------------------

void CTextDocument::SetPrintFrameSize()
{
   // Get the frame size.
   SDimension16	theFrameSize;
   mTextView->GetFrameSize(theFrameSize);

   // Get the text edit record handle.
   TEHandle theTextEditH = mTextView->GetMacTEH();

   // Calculate the number of lines per page.
   SInt16 theLinesPerPage;
   theLinesPerPage = theFrameSize.height / (**theTextEditH).lineHeight;

   // Resize the frame to an integral number of lines.
   mTextView->ResizeFrameTo( theFrameSize.width,
      (**theTextEditH).lineHeight * theLinesPerPage, Refresh_No);
}


// ---------------------------------------------------------------------------
// GetFileType  [public, virtual]
// ---------------------------------------------------------------------------
// Return the type (four character code) of the file used for saving
// the Document. Subclasses should override if they support saving files.

OSType CTextDocument::GetFileType() const
{
   return ResType_Text;
}


// ---------------------------------------------------------------------------
// FindCommandStatus  [public, virtual]
// ---------------------------------------------------------------------------
// Determine the status of a Command for the purposes of menu updating.

void CTextDocument::FindCommandStatus(
   CommandT  inCommand,
   Boolean&  outEnabled,
   Boolean&  outUsesMark,
   UInt16&   outMark,
   Str255    outName)
{
   switch (inCommand) {

      case cmd_WorkerDialog:
         outEnabled = true;
      break;

      case cmd_PebesDialog:
      {
         //try {
            // do not allow for non-PEBES calculation
         //   if (workerData->getJoasdi() != WorkerData::PEBS_CALC) {
         //      outEnabled = false;
         //      break;
         //  }
         //   // do not allow if date of birth has not been set yet
         //   if (!workerData->getBirthDate().is_not_a_date()) {
         //      workerData->birth1Check(userAssumptions->getIstart());
         //   } else {
         //      outEnabled = false;
         //      break;
         //   }
         //} catch (exception& e) {
         //   outEnabled = false;
         //   break;
         //}
         //outEnabled = true;
      }
      break;

      case cmd_NonPebesDialog:
      {
         try {
            // do not allow for PEBES calculation
            if (workerData->getJoasdi() == WorkerData::PEBS_CALC) {
               outEnabled = false;
               break;
            }
            // do not allow if date of birth has not been set yet
            if (!workerData->getBirthDate().is_not_a_date()) {
               workerData->birth2Check();
            } else {
               outEnabled = false;
               break;
            }
         } catch (exception& e) {
            outEnabled = false;
            break;
         }
         outEnabled = true;
      }
      break;

      case cmd_DisabilityDialog:
         try {
            // do not allow if date of birth has not been set yet
            if (!workerData->getBirthDate().is_not_a_date()) {
               workerData->birth2Check();
            } else {
               outEnabled = false;
               break;
            }
            // do not allow if date of benefit has not been set yet
            if (!workerData->getBenefitDate().isNotADate()) {
               WorkerData::benefitDate1Check(workerData->getBenefitDate());
            } else {
               outEnabled = false;
               break;
            }
         } catch (exception& e) {
            outEnabled = false;
            break;
         }
         outEnabled = (workerData->getValdi() > 0u);
      break;

      case cmd_SurvivorDialog:
         outEnabled = (widowArray->getFamSize() > 0);
      break;

      case cmd_MilservDialog:
         outEnabled = (workerData->milServDatesVec.getMSCount() > 0);
      break;

      case cmd_SummaryQCDialog:
         if (workerData->getIbegin() < 1978 &&
            workerData->getIbegin() > 1936)
            outEnabled = true;
         else
            outEnabled = false;
      break;

      case cmd_AssumptionsDialog:
         if (workerData->getJoasdi() == WorkerData::PEBS_CALC) {
            outEnabled = true;
         }
         else {
            if (workerData->need_bi(UserAssumptions::getIstart()) ||
               workerData->need_aw(UserAssumptions::getIstart(),
               earnProject->need_aw(userAssumptions->getIstart(),
               workerData->getIend())))
               outEnabled = true;
            else {
               outEnabled = false;
               userAssumptions->setIaltbi(AssumptionType::FLAT);
               userAssumptions->setIaltaw(AssumptionType::FLAT);
            }
         }
      break;

      case cmd_BenincDialog:
         if (workerData->need_bi(UserAssumptions::getIstart()) &&
            userAssumptions->getIaltbi() == AssumptionType::OTHER_ASSUM)
            outEnabled = true;
         else
            outEnabled = false;
      break;

      case cmd_CatchupDialog:
         if (userAssumptions->getAnscch() == 'Y')
            outEnabled = true;
         else
            outEnabled = false;
      break;

      case cmd_AvgwageDialog:
         if (workerData->need_aw(UserAssumptions::getIstart(),
            earnProject->need_aw(userAssumptions->getIstart(),
            workerData->getIend())) &&
            userAssumptions->getIaltaw() == AssumptionType::OTHER_ASSUM)
            outEnabled = true;
         else
            outEnabled = false;
      break;

      case cmd_WagebaseDialog:
         if (userAssumptions->getIbasch() == 2)
            outEnabled = true;
         else
            outEnabled = false;
      break;

      case cmd_AnnearnDialog:
         outEnabled = workerData->getEarnEnable();
      break;

      case cmd_CompyearDialog:
         outEnabled = true;
      break;

      case cmd_FieldOfficeDialog:
         //outEnabled = true;
      break;

      case cmd_HistAmtRevDialog:
         outEnabled = true;
      break;

      case cmd_HistAmtModDialog:
      {
         if (awbidat->getLastYear() > baseyearStored)
            outEnabled = true;
         else
            outEnabled = false;
      }
      break;

      case cmd_HistAmtUpdateDialog:
         outEnabled = true;
      break;

      case cmd_BenincSet1Dialog:
         outEnabled = true;
      break;

      case cmd_BenincSet2Dialog:
         outEnabled = true;
      break;

      case cmd_BenincSet3Dialog:
         outEnabled = true;
      break;

      case cmd_BenincSet4Dialog:
         outEnabled = true;
      break;

      case cmd_CatchupSet1Dialog:
         outEnabled = assumptions->catchupDoc.allownonzero[0];
      break;

      case cmd_CatchupSet2Dialog:
         outEnabled = assumptions->catchupDoc.allownonzero[1];
      break;

      case cmd_CatchupSet3Dialog:
         outEnabled = assumptions->catchupDoc.allownonzero[2];
      break;

      case cmd_CatchupSet4Dialog:
         outEnabled = assumptions->catchupDoc.allownonzero[3];
      break;

      case cmd_AvgwageSet1Dialog:
         outEnabled = true;
      break;

      case cmd_AvgwageSet2Dialog:
         outEnabled = true;
      break;

      case cmd_AvgwageSet3Dialog:
         outEnabled = true;
      break;

      case cmd_AvgwageSet4Dialog:
         outEnabled = true;
      break;

      case cmd_ConfigDialog:
         outEnabled = true;
      break;

      case cmd_ScreenDialog:
         outEnabled = true;
      break;

      case cmd_AboD:
         outEnabled = true;
      break;

      case cmd_Calculate:
         outEnabled = true;
      break;

      default: {
         LSingleDoc::FindCommandStatus(inCommand, outEnabled,
            outUsesMark, outMark, outName);
         break;
      }
   }
}


// ---------------------------------------------------------------------------
// ObeyCommand  [public, virtual]
// ---------------------------------------------------------------------------
// Respond to Commands. Returns true if the Command was handled, false if not.

Boolean
CTextDocument::ObeyCommand(
   CommandT  inCommand,
   void*     ioParam)
{
   Boolean cmdHandled = true;	// Assume we'll handle the command

   switch (inCommand) {
   
      case cmd_WorkerDialog:
      {
         CWorkerInfoDialog * theDialog;
         theDialog = dynamic_cast<CWorkerInfoDialog *>
            (LWindow::CreateWindow( rPPob_WorkerDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_WkrD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CWorkerInfoDialog *theDialog;
            theDialog = (CWorkerInfoDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
		   } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_WkrD_OKButton");
		   }
      }
      break;
      
      case cmd_WkrD_CancelButton:
      {
         SDialogResponse *theResponse;
         CWorkerInfoDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CWorkerInfoDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
		   } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_WkrD_CancelButton");
		   }
      }
      break;
      
      case cmd_PebesDialog:
      {
         //CPebesDialog * theDialog;
         //theDialog = dynamic_cast<CPebesDialog *>
         //   (LWindow::CreateWindow( rPPob_PebesDialog, this ));
         //Assert_(theDialog != nil);
         //theDialog->theDoc = this;
         //theDialog->SetupDialog();
         //theDialog->Show();
      }
      break;

      case cmd_PbsD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CPebesDialog *theDialog;
            theDialog = (CPebesDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
		   } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_PbsD_OKButton");
		   }
      }
      break;
      
      case cmd_PbsD_CancelButton:
      {
         SDialogResponse *theResponse;
         CPebesDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CPebesDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
		   } catch (...) {
			   SignalStringLiteral_(
			      "Exception caught in CDocumentApp::ObeyCommand for cmd_PbsD_CancelButton");
		   }
      }
      break;

      case cmd_NonPebesDialog:
      {
         CNonPebesDialog * theDialog;
         theDialog = dynamic_cast<CNonPebesDialog *>
            (LWindow::CreateWindow( rPPob_NonPebesDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_NpbD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CNonPebesDialog *theDialog;
            theDialog = (CNonPebesDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_NpbD_OKButton");
		   }
      }
      break;
      
      case cmd_NpbD_CancelButton:
      {
         SDialogResponse *theResponse;
         CNonPebesDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CNonPebesDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_NpbD_CancelButton");
		   }
      }
      break;

      case cmd_DisabilityDialog:
      {
         CDisabilityDialog * theDialog;
         theDialog = dynamic_cast<CDisabilityDialog *>
            (LWindow::CreateWindow( rPPob_DisabilityDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_DisD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CDisabilityDialog *theDialog;
            theDialog = (CDisabilityDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_DisD_OKButton");
		   }
      }
      break;
      
      case cmd_DisD_CancelButton:
      {
         SDialogResponse *theResponse;
         CDisabilityDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CDisabilityDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_DisD_CancelButton");
		   }
      }
      break;

      case cmd_SurvivorDialog:
      {
         CSurvivorDialog * theDialog;
         theDialog = dynamic_cast<CSurvivorDialog *>
            (LWindow::CreateWindow( rPPob_SurvivorDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_SurD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CSurvivorDialog *theDialog;
            theDialog = (CSurvivorDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_SurD_OKButton");
		   }
      }
      break;
      
      case cmd_SurD_CancelButton:
      {
         SDialogResponse *theResponse;
         CSurvivorDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CSurvivorDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_SurD_CancelButton");
		   }
      }
      break;

      case cmd_MilservDialog:
      {
         CMilservDialog * theDialog;
         theDialog = dynamic_cast<CMilservDialog *>
            (LWindow::CreateWindow( rPPob_MilservDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_MlsD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CMilservDialog *theDialog;
            theDialog = (CMilservDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_MlsD_OKButton");
		   }
      }
      break;
      
      case cmd_MlsD_CancelButton:
      {
         SDialogResponse *theResponse;
         CMilservDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CMilservDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_MlsD_CancelButton");
		   }
      }
      break;

      case cmd_SummaryQCDialog:
      {
         CSummaryQCDialog * theDialog;
         theDialog = dynamic_cast<CSummaryQCDialog *>
            (LWindow::CreateWindow( rPPob_SummaryQCDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_SQCD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CSummaryQCDialog *theDialog;
            theDialog = (CSummaryQCDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_SQCD_OKButton");
		   }
      }
      break;
      
      case cmd_SQCD_CancelButton:
      {
         SDialogResponse *theResponse;
         CSummaryQCDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CSummaryQCDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_SQCD_CancelButton");
		   }
      }
      break;

      case cmd_AssumptionsDialog:
      {
         CAssumptionsDialog * theDialog;
         theDialog = dynamic_cast<CAssumptionsDialog *>
            (LWindow::CreateWindow( rPPob_AssumptionsDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_AssD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CAssumptionsDialog *theDialog;
            theDialog = (CAssumptionsDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_AssD_OKButton");
		   }
      }
      break;
      
      case cmd_AssD_CancelButton:
      {
         SDialogResponse *theResponse;
         CAssumptionsDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CAssumptionsDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_AssD_CancelButton");
		   }
      }
      break;

      case cmd_BenincDialog:
      {
         CBenincDialog * theDialog;
         theDialog = dynamic_cast<CBenincDialog *>
            (LWindow::CreateWindow( rPPob_BenincDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_BenD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CBenincDialog *theDialog;
            theDialog = (CBenincDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_BenD_OKButton");
		   }
      }
      break;
      
      case cmd_BenD_CancelButton:
      {
         SDialogResponse *theResponse;
         CBenincDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CBenincDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_BenD_CancelButton");
		   }
      }
      break;

      case cmd_CatchupDialog:
      {
         CCatchupDialog * theDialog;
         theDialog = dynamic_cast<CCatchupDialog *>
            (LWindow::CreateWindow( rPPob_CatchupDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_CatD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CCatchupDialog *theDialog;
            theDialog = (CCatchupDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_CatD_OKButton");
		   }
      }
      break;
      
      case cmd_CatD_CancelButton:
      {
         SDialogResponse *theResponse;
         CCatchupDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CCatchupDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_CatD_CancelButton");
		   }
      }
      break;

      case cmd_AvgwageDialog:
      {
         CAvgwageDialog * theDialog;
         theDialog = dynamic_cast<CAvgwageDialog *>
            (LWindow::CreateWindow( rPPob_AvgwageDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_AvwD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CAvgwageDialog *theDialog;
            theDialog = (CAvgwageDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_AvwD_OKButton");
		   }
      }
      break;
      
      case cmd_AvwD_CancelButton:
      {
         SDialogResponse *theResponse;
         CAvgwageDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CAvgwageDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_AvwD_CancelButton");
		   }
      }
      break;

      case cmd_WagebaseDialog:
      {
         CWagebaseDialog * theDialog;
         theDialog = dynamic_cast<CWagebaseDialog *>
            (LWindow::CreateWindow( rPPob_WagebaseDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_WgbD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CWagebaseDialog *theDialog;
            theDialog = (CWagebaseDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_WgbD_OKButton");
		   }
      }
      break;
      
      case cmd_WgbD_CancelButton:
      {
         SDialogResponse *theResponse;
         CWagebaseDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CWagebaseDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_WgbD_CancelButton");
		   }
      }
      break;

      case cmd_AnnearnDialog:
      {
         CAnnearnDialog * theDialog;
         theDialog = dynamic_cast<CAnnearnDialog *>
            (LWindow::CreateWindow( rPPob_AnnearnDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_AneD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CAnnearnDialog *theDialog;
            theDialog = (CAnnearnDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_AneD_OKButton");
		   }
      }
      break;
      
      case cmd_AneD_CancelButton:
      {
         SDialogResponse *theResponse;
         CAnnearnDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CAnnearnDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_AneD_CancelButton");
		   }
      }
      break;

      case cmd_SpcD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CSpecYearDialog *theDialog;
            theDialog = (CSpecYearDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_SpcD_OKButton");
		   }
      }
      break;
      
      case cmd_SpcD_CancelButton:
      {
         SDialogResponse *theResponse;
         CSpecYearDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CSpecYearDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_SpcD_CancelButton");
		   }
      }
      break;

      case cmd_CompyearDialog:
      {
         CBaseyearDialog * theDialog;
         theDialog = dynamic_cast<CBaseyearDialog *>
            (LWindow::CreateWindow( rPPob_CompyearDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_CmpD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CBaseyearDialog *theDialog;
            theDialog = (CBaseyearDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_CmpD_OKButton");
		   }
      }
      break;
      
      case cmd_CmpD_CancelButton:
      {
         SDialogResponse *theResponse;
         CBaseyearDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CBaseyearDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_CmpD_CancelButton");
		   }
      }
      break;

      case cmd_FieldOfficeDialog:
      {
         CFoinfoDialog * theDialog;
         theDialog = dynamic_cast<CFoinfoDialog *>
            (LWindow::CreateWindow( rPPob_FieldOfficeDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_FoiD_OKButton:
      {
         //try {
         //   SDialogResponse *theResponse =
         //      static_cast<SDialogResponse *> (ioParam);
         //   Assert_( theResponse != nil );
         //   CFoinfoDialog *theDialog;
         //   theDialog = (CFoinfoDialog *) theResponse->dialogBox;
         //   Assert_( theDialog != nil );
         //   theDialog->SaveResults();
         //   delete theDialog;
         //   // Update the screen
         //   Repopulate();
         //} catch (...) {
         //   SignalStringLiteral_(
         //      "Exception caught in CDocumentApp::ObeyCommand for cmd_FoiD_OKButton");
		 //}
      }
      break;
      
      case cmd_FoiD_CancelButton:
      {
         //SDialogResponse *theResponse;
         //CFoinfoDialog *theDialog;
         //try {
         //   theResponse = static_cast<SDialogResponse *> (ioParam);
         //   Assert_( theResponse != nil );
         //   theDialog = (CFoinfoDialog *) theResponse->dialogBox;
         //   Assert_( theDialog != nil );
         //   delete theDialog;
         //} catch (...) {
         //   SignalStringLiteral_(
         //      "Exception caught in CDocumentApp::ObeyCommand for cmd_FoiD_CancelButton");
		 //}
      }
      break;

      case cmd_ConfigDialog:
      {
         CConfigDialog * theDialog;
         theDialog = dynamic_cast<CConfigDialog *>
            (LWindow::CreateWindow( rPPob_ConfigDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_CnfD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CConfigDialog *theDialog;
            theDialog = (CConfigDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_CnfD_OKButton");
		   }
      }
      break;
      
      case cmd_CnfD_CancelButton:
      {
         SDialogResponse *theResponse;
         CConfigDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CConfigDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_CnfD_CancelButton");
		   }
      }
      break;

      case cmd_ScreenDialog:
      {
         CScreenDialog * theDialog;
         theDialog = dynamic_cast<CScreenDialog *>
            (LWindow::CreateWindow( rPPob_ScreenDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_ScnD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CScreenDialog *theDialog;
            theDialog = (CScreenDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_ScnD_OKButton");
		   }
      }
      break;
      
      case cmd_ScnD_CancelButton:
      {
         SDialogResponse *theResponse;
         CScreenDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CScreenDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_ScnD_CancelButton");
		   }
      }
      break;

      case cmd_HistAmtModDialog:
      {
         CHistAmtModifyDialog * theDialog;
         theDialog = dynamic_cast<CHistAmtModifyDialog *>
            (LWindow::CreateWindow( rPPob_HistAmtModDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_HamD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CHistAmtModifyDialog *theDialog;
            theDialog = (CHistAmtModifyDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_HamD_OKButton");
		   }
      }
      break;
      
      case cmd_HamD_CancelButton:
      {
         SDialogResponse *theResponse;
         CHistAmtModifyDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CHistAmtModifyDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_HamD_CancelButton");
		   }
      }
      break;

      case cmd_HistAmtRevDialog:
      {
         CHistAmtReviewDialog * theDialog;
         theDialog = dynamic_cast<CHistAmtReviewDialog *>
            (LWindow::CreateWindow( rPPob_HistAmtRevDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_HarD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CHistAmtReviewDialog *theDialog;
            theDialog = (CHistAmtReviewDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_HarD_OKButton");
		   }
      }
      break;
      
      case cmd_HarD_CancelButton:
      {
         SDialogResponse *theResponse;
         CHistAmtReviewDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CHistAmtReviewDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_HarD_CancelButton");
		   }
      }
      break;

      case cmd_HistAmtUpdateDialog:
      {
         CHistAmtUpdateDialog * theDialog;
         theDialog = dynamic_cast<CHistAmtUpdateDialog *>
            (LWindow::CreateWindow( rPPob_HistAmtUpdateDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_HauD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CHistAmtUpdateDialog *theDialog;
            theDialog = (CHistAmtUpdateDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_HauD_OKButton");
		   }
      }
      break;
      
      case cmd_HauD_CancelButton:
      {
         SDialogResponse *theResponse;
         CHistAmtUpdateDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CHistAmtUpdateDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_HauD_CancelButton");
		   }
      }
      break;

      case cmd_BenincSet1Dialog:
      {
         CBisetDialog * theDialog;
         theDialog = dynamic_cast<CBisetDialog *>
            (LWindow::CreateWindow( rPPob_BenincSetDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->mAlt = 1;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_BenincSet2Dialog:
      {
         CBisetDialog * theDialog;
         theDialog = dynamic_cast<CBisetDialog *>
            (LWindow::CreateWindow( rPPob_BenincSetDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->mAlt = 2;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_BenincSet3Dialog:
      {
         CBisetDialog * theDialog;
         theDialog = dynamic_cast<CBisetDialog *>
            (LWindow::CreateWindow( rPPob_BenincSetDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->mAlt = 3;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_BenincSet4Dialog:
      {
         CBisetDialog * theDialog;
         theDialog = dynamic_cast<CBisetDialog *>
            (LWindow::CreateWindow( rPPob_BenincSetDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->mAlt = 4;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_StbD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CBisetDialog *theDialog;
            theDialog = (CBisetDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_StbD_OKButton");
		   }
      }
      break;
      
      case cmd_StbD_CancelButton:
      {
         SDialogResponse *theResponse;
         CBisetDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CBisetDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_StbD_CancelButton");
		   }
      }
      break;

      case cmd_CatchupSet1Dialog:
      {
         CCusetDialog * theDialog;
         theDialog = dynamic_cast<CCusetDialog *>
            (LWindow::CreateWindow( rPPob_CatchupSetDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->mAlt = 1;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_CatchupSet2Dialog:
      {
         CCusetDialog * theDialog;
         theDialog = dynamic_cast<CCusetDialog *>
            (LWindow::CreateWindow( rPPob_CatchupSetDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->mAlt = 2;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_CatchupSet3Dialog:
      {
         CCusetDialog * theDialog;
         theDialog = dynamic_cast<CCusetDialog *>
            (LWindow::CreateWindow( rPPob_CatchupSetDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->mAlt = 3;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_CatchupSet4Dialog:
      {
         CCusetDialog * theDialog;
         theDialog = dynamic_cast<CCusetDialog *>
            (LWindow::CreateWindow( rPPob_CatchupSetDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->mAlt = 4;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_StcD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CCusetDialog *theDialog;
            theDialog = (CCusetDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_StcD_OKButton");
		   }
      }
      break;
      
      case cmd_StcD_CancelButton:
      {
         SDialogResponse *theResponse;
         CCusetDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CCusetDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_StcD_CancelButton");
		   }
      }
      break;

      case cmd_AvgwageSet1Dialog:
      {
         CAwsetDialog * theDialog;
         theDialog = dynamic_cast<CAwsetDialog *>
            (LWindow::CreateWindow( rPPob_AvgwageSetDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->mAlt = 1;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_AvgwageSet2Dialog:
      {
         CAwsetDialog * theDialog;
         theDialog = dynamic_cast<CAwsetDialog *>
            (LWindow::CreateWindow( rPPob_AvgwageSetDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->mAlt = 2;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_AvgwageSet3Dialog:
      {
         CAwsetDialog * theDialog;
         theDialog = dynamic_cast<CAwsetDialog *>
            (LWindow::CreateWindow( rPPob_AvgwageSetDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->mAlt = 3;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_AvgwageSet4Dialog:
      {
         CAwsetDialog * theDialog;
         theDialog = dynamic_cast<CAwsetDialog *>
            (LWindow::CreateWindow( rPPob_AvgwageSetDialog, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->mAlt = 4;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_StaD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CAwsetDialog *theDialog;
            theDialog = (CAwsetDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_StaD_OKButton");
		   }
      }
      break;
      
      case cmd_StaD_CancelButton:
      {
         SDialogResponse *theResponse;
         CAwsetDialog *theDialog;
         try {
            theResponse = static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            theDialog = (CAwsetDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_StaD_CancelButton");
		   }
      }
      break;

      case cmd_MsgD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CMessageDialog *theDialog;
            theDialog = (CMessageDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_MsgD_OKButton");
		   }
      }
      break;

      case cmd_AboD:
      {
         CAbout * theDialog;
         theDialog = dynamic_cast<CAbout *>
            (LWindow::CreateWindow( rPPob_AboD, this ));
         Assert_(theDialog != nil);
         theDialog->theDoc = this;
         theDialog->SetupDialog();
         theDialog->Show();
      }
      break;

      case cmd_AboD_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CMessageDialog *theDialog;
            theDialog = (CMessageDialog *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            delete theDialog;
            // Update the screen
            Repopulate();
         } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_About_OKButton");
		   }
      }
      break;

      case cmd_Calculate:
      {
         try {
            DoCalculate();
         } catch (PiaException e) {
            messageDialog(e.getNumber(), this);
         }
      }
      break;

      case cmd_Intr_OKButton:
      {
         try {
            SDialogResponse *theResponse =
               static_cast<SDialogResponse *> (ioParam);
            Assert_( theResponse != nil );
            CIntroduction *theDialog;
            theDialog = (CIntroduction *) theResponse->dialogBox;
            Assert_( theDialog != nil );
            theDialog->SaveResults();
            delete theDialog;
            // Update the screen
            Repopulate();
		   } catch (...) {
            SignalStringLiteral_(
               "Exception caught in CDocumentApp::ObeyCommand for cmd_Intr_OKButton");
		   }
      }
      break;

      default: {
         cmdHandled = LSingleDoc::ObeyCommand(inCommand, ioParam);
         break;
      }
   }
	
   return cmdHandled;
}

// -----------------------------------------------------------------------------
// Repopulate  [public]
// -----------------------------------------------------------------------------

void CTextDocument::Repopulate(
   int pageNum )
{
   string separator("___________________________________________________________");
   OutMac80 outfile((pageNum <= 0));
   outfile.setTextEditView(mTextView);
   outfile.deleteContents();

   anypiaOut->fillPageSlots(valid, config->getLongOutput());

   if (pageNum > 0) {
      anypiaOut->getPage(pageNum-1)->print(outfile);
   } else {
      lineIndex[0] = 0;
      for (int i = 1; i <= anypiaOut->getPageCount(); i++) {
         anypiaOut->getPage(i-1)->print(outfile);
         lineIndex[i] = outfile.fLines;
         
         if (workerData->getJoasdi() != WorkerData::PEBS_CALC) {
            outfile.printLine(separator);
            outfile.advanceLine();
         } else {
            outfile.advanceLine();
         }
      }
   }
   if (pageNum <= 0) {
      mWindow->ResizeWindowBy( +1, +1);
      mWindow->ResizeWindowBy( -1, -1);
   }
}

// Initializes all the piadata library classes

void CTextDocument::NewPIA()
{
   WorkerData::setQcLumpYear(1977);
   config = new ConfigNonFile();
   baseyear = new BaseYearNonFile();
   baseyearStored = baseyear->getYear();
   WorkerDataGeneral::setMaxyear(YEAR2090);
   workerData = new WorkerData();
   widowDataArray = new WorkerDataArray();
   piadata = new PiaData();
   widowArray = new PiaDataArray();
   secondaryArray = new SecondaryArray();
   userAssumptions = new UserAssumptions(WorkerData::getMaxyear());
   earnProject = new EarnProject(YEAR2090);
   pebs = new Pebs;
   piaread = new PiaReadAny(*workerData, *widowDataArray, *widowArray,
      *userAssumptions, *secondaryArray, *earnProject, *pebs);
   piawrite = new PiaWriteAny(*workerData, *widowDataArray, *widowArray,
      *userAssumptions, *secondaryArray, *earnProject, *pebs);
   awbidat = new AwbiDataNonFile(baseyear->getYear(),
      workerData->getMaxyear());
   userAssumptions->setIstart(baseyear->getYear());
   assumptions = new AssumptionsNonFile(baseyear->getYear(),
      workerData->getMaxyear());
   lawChange = new LawChangeArray(baseyear->getYear(),
      workerData->getMaxyear(), "");
   piaparms = new PiaParamsAny(baseyear->getYear(), workerData->getMaxyear(),
      *awbidat, *lawChange);
   piaparms->setHistFqinc();
   userAssumptions->setIaltbi(AssumptionType::FLAT);
   userAssumptions->setIaltaw(AssumptionType::FLAT);
   userAssumptions->setIbasch(1);
   taxes = new TaxData(workerData->getMaxyear());
   taxrates = new TaxRatesPL(workerData->getMaxyear());
   piacal = new PiaCalAny(*workerData, *piadata, *widowDataArray, *widowArray,
      *piaparms, *userAssumptions, *secondaryArray, *lawChange, *pebs,
      *earnProject);
   piacal->calculate1(*assumptions);
   foinfo = new FieldOfficeInfoNonFile();
   foinfo->setData();
   piaOut = new PiaOut(*piacal, *taxes);
   pebsOut = new PebsOut(*workerData, *piadata, *foinfo, *pebs, *piaparms,
      *taxes, *earnProject);
   anypiaOut = new AnypiaOut();
   anypiaOut->setWorkerData(workerData);
   anypiaOut->setPiaData(piadata);
   anypiaOut->setPiaCal(piacal);
   anypiaOut->setPiaOut(piaOut);
   anypiaOut->setPebsOut(pebsOut);
   if (workerData->getJoasdi() == WorkerData::PEBS_CALC) {
      piacal->pebsOabCal();
      pebsOut->setPageNum();
      pebsOut->pebsWorkerInfo.prepareStrings();
   }
   else {
      piaOut->setPageNum(*piacal);
      piaOut->summary1Page.prepareStrings();
   }
}

// @cmember Release memory.

void CTextDocument::DeletePIA()
{
   delete anypiaOut;
   delete pebsOut;
   delete piaOut;
   delete foinfo;
   delete piacal;
   delete pebs;
   delete taxrates;
   delete taxes;
   delete piaparms;
   delete lawChange;
   delete assumptions;
   delete awbidat;
   delete piawrite;
   delete piaread;
   delete userAssumptions;
   delete secondaryArray;
   delete widowArray;
   delete piadata;
   delete workerData;
   delete baseyear;
   delete config;
   delete earnProject;
}

void CTextDocument::DeleteContents()
{
   workerData->deleteContents();
   piadata->deleteContents();
   earnProject->deleteContents();
   widowArray->deleteContents();
   widowDataArray->deleteContents();
   secondaryArray->deleteContents();
   userAssumptions->setIaltaw(AssumptionType::FLAT);
   userAssumptions->setIaltbi(AssumptionType::FLAT);
   if (workerData->getJoasdi() == WorkerData::PEBS_CALC) {
      piacal->pebsOabCal();
      pebsOut->pebsWorkerInfo.prepareStrings();
   }
   else {
      piaOut->summary1Page.prepareStrings();
   }
   valid = FALSE;
}
