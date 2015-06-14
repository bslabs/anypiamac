// $Id: CTextView.cp,v 1.4 2010/08/16 18:18:07 djolson Exp $
// =============================================================================
// CTextView.cp  ©1996-1999 Metrowerks Inc. All rights reserved.
// =============================================================================

#include "CTextView.h"
#include "wrkrdata.h"

#include <LStream.h>


// -----------------------------------------------------------------------------
// CTextView  [public]
// -----------------------------------------------------------------------------
//	LStream constructor

CTextView::CTextView(
   LStream* inStream) : LTextEditView(inStream)
{
   mIsDirty = false;
}


// -----------------------------------------------------------------------------
// ~CTextView  [public, virtual]
// -----------------------------------------------------------------------------
//	Destructor

CTextView::~CTextView()
{
   // nothing
}

// @mfunc Called when a user action changes the text.  This is commented out
//   because currently only the program changes the text, not the user.

void
CTextView::UserChangedText()
{
   //if (IsDirty() == false) {

      // Set the update menus flag.
      //SetUpdateCommandStatus(true);

      // Set the dirty flag.
      //SetDirty(true);
   //}
}

// -----------------------------------------------------------------------------
// SavePlace  [public, virtual]
// -----------------------------------------------------------------------------

void
CTextView::SavePlace(
   LStream* outPlace )
{
   // Call inherited.
   LTextEditView::SavePlace(outPlace);

   // Save the image size.
   outPlace->WriteData(&mImageSize, sizeof(SDimension32));
}

// -----------------------------------------------------------------------------
// RestorePlace  [public, virtual]
// -----------------------------------------------------------------------------

void
CTextView::RestorePlace(
   LStream* inPlace )
{
   // Call inherited.
   LTextEditView::RestorePlace(inPlace);

   // Restore the image size.
   inPlace->ReadData(&mImageSize, sizeof(SDimension32));

   // Realign the text edit rects.
   AlignTextEditRects();
}

//	@mfunc This method just asks the document how many pages long it is
//   and returns that. Always 1 page wide by n pages long.

void CTextView::CountPanels(
   UInt32  &outHorizPanels,
   UInt32  &outVertPanels )
{
   // We're always one panel across. The number of pages is determined by the
   // CTextDocument.
   outHorizPanels = 1;
   outVertPanels = mTextDoc->anypiaOut->getPageCount();
}

//	@mfunc  This function scrolls the text image to the top of the
//	  currently requested page. MDM: I stole some code from the inherited
//   class and modified it.

Boolean CTextView::ScrollToPanel(
   const PanelSpec  &inPanel)
{
   Boolean	panelInImage = false;

   SDimension16	frameSize;
   GetFrameSize(frameSize);
	
   // Get the number of panels in the document
   UInt32	horizPanelCount;
   UInt32	vertPanelCount;
   SInt32	firstLine, vertPos = 0;
   CountPanels(horizPanelCount, vertPanelCount);

   // Just a check to make sure we're not printing outside the document	
   if ((inPanel.horizIndex <= horizPanelCount) &&
      (inPanel.vertIndex <= vertPanelCount)) {
		
      // First page: start at 0,0
      // Second etc page: scroll up to just above the first line on that page,
      // as determined in lineIndex.
      if (inPanel.pageNumber > 1) {
         // Skip over separator for non-Statement output
         if (mTextDoc->workerData->getJoasdi() == WorkerData::PEBS_CALC) {
            firstLine = mTextDoc->lineIndex[inPanel.pageNumber-1];
         }
         else {
            firstLine = mTextDoc->lineIndex[inPanel.pageNumber-1] + 2;
         }
         vertPos = (SInt32) ::TEGetHeight(firstLine, 0, mTextEditH);
      }
      ScrollImageTo(0, vertPos, false);
      panelInImage = true;
   }

   return panelInImage;
}

//	@mfunc This method figures out how long the text on the current page goes 
//   and sets mRevealedRect to only enclose just that much text. That way the
//   unwanted text (on this page) gets clipped off.

void  CTextView::SuperPrintPanel(
   const PanelSpec  &inSuperPanel,
   RgnHandle        inSuperPrintRgnH )
{
   Rect saveRect = mRevealedRect;

   //	Figure out the first and last lines on this page	
   SInt32 lastLine, firstLine;

   if (inSuperPanel.pageNumber == 1) {
      lastLine = mTextDoc->lineIndex[1]+1;
      firstLine = 1;
   }
   else {
      lastLine = mTextDoc->lineIndex[inSuperPanel.pageNumber] - 2;
      if (mTextDoc->workerData->getJoasdi() == WorkerData::PEBS_CALC) {
         firstLine = mTextDoc->lineIndex[inSuperPanel.pageNumber - 1] - 2;
      }
      else {
         firstLine = mTextDoc->lineIndex[inSuperPanel.pageNumber - 1];
      }
   }

   // Make mRevaledRect only show that range. Left-right range doesn't
   // matter because we have no limitation on width.
   mRevealedRect.top = 0;
   mRevealedRect.left = 0;
   mRevealedRect.bottom = ::TEGetHeight(lastLine, firstLine, mTextEditH);
   mRevealedRect.right = 32767;

   LView::SuperPrintPanel (inSuperPanel, inSuperPrintRgnH);

   // Restore it, for what it's worth	
   mRevealedRect = saveRect;
}
