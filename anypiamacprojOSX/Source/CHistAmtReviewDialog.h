// $Id: CHistAmtReviewDialog.h,v 1.3 2007/11/07 13:06:58 djolson Exp $
//  CHistAmtReviewDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CHistAmtReviewDialog : public LDialogBox
{
   public:
      enum { class_ID = 'HarD' };
      
      static CHistAmtReviewDialog * CreateHistAmtReviewDialogStream(
         LStream * inStream );
      CHistAmtReviewDialog( LStream * inStream );
      ~CHistAmtReviewDialog();
      virtual void FindCommandStatus( CommandT inCommand,
         Boolean &outEnabled, Boolean &outUsesMark,
         UInt16 &outMark, Str255 outName );
      virtual void ListenToMessage( MessageT inMessage, void * ioParam );
      void SaveResults();
      virtual void SetupDialog();
   private:
      virtual void FinishCreateSelf();
   public:
      CTextDocument * theDoc;
      int mStartyear;
      int mLastyear;
      int mYear;
      DoubleAnnual mWb;
      DoubleAnnual mWbold;
      DoubleAnnual mFq;
      DoubleAnnual mBi;
      void setupdata();
};
