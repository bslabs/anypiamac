// $Id: CTextView.h,v 1.3 2007/11/07 13:07:00 djolson Exp $
// ==============================================================================
// CTextView.h  ©1996-1999 Metrowerks Inc. All rights reserved.
// ==============================================================================

#ifndef _H_CTextView
#define _H_CTextView
#pragma once

#include <LTextEditView.h>
#include "CTextDocument.h"

class CTextView : public LTextEditView {

public:
   enum { class_ID = FOUR_CHAR_CODE('TxtV') };

                   CTextView(
                      LStream* inStream);
   virtual         ~CTextView();

   virtual void    UserChangedText();

   virtual void    SavePlace(
                      LStream* outPlace);
   virtual void    RestorePlace(
                      LStream* inPlace);

   Boolean         IsDirty() const
                   {
                      return mIsDirty;
                   }
   void            SetDirty(
                      Boolean inDirty)
                   {
                      mIsDirty = inDirty;
                   }
   // @cmember Count the number of pages in the anticipated printout.
   virtual void    CountPanels(
                      UInt32 &outHorizPanels,
                      UInt32 &outVertPanels);
   // @cmember Adjusts the mRevealedRect so that only the desired text
   //   gets printed (as opposed to the stuff past it).
   virtual void    SuperPrintPanel(
                      const PanelSpec &inSuperPanel,
                      RgnHandle inSuperPrintRgnH);
   // @cmember Scroll the text in the panel so that the first line on the
   //   current page appears at the top.
   virtual Boolean ScrollToPanel(
                      const PanelSpec &inPanel);
   // @cmember Sets the pointer to the document.
   void            SetTextDoc(
                      CTextDocument *doc)
                   {
                      mTextDoc = doc;
                   }
								
protected:
   bool            mIsDirty;
   // @cmember Pointer from the view to its document.
   CTextDocument  *mTextDoc;

private:
                   CTextView();
                   CTextView(const CTextView& inOriginal);
   CTextView& operator=(const CTextView& inRhs);
};

#endif // _H_CTextView