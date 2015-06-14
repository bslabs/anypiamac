// $Id: CHistAmtModifyDialog.h,v 1.4 2008/10/31 12:51:40 djolson Exp $
//  CHistAmtModifyDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CHistAmtModifyDialog : public LDialogBox
{
   public:
      enum { class_ID = 'HamD' };
      
      static CHistAmtModifyDialog * CreateHistAmtModifyDialogStream(
         LStream * inStream );
      CHistAmtModifyDialog( LStream * inStream );
      ~CHistAmtModifyDialog();
      virtual void FindCommandStatus( CommandT inCommand,
         Boolean &outEnabled, Boolean &outUsesMark,
         UInt16 &outMark, Str255 outName );
      virtual void ListenToMessage( MessageT inMessage, void * ioParam );
      void SaveResults();
      virtual void SetupDialog();
   private:
      virtual void FinishCreateSelf();
      std::string getTextBoxString( long textBoxID );
   public:
      CTextDocument * theDoc;
      int mStartyear;
      int mLastyear;
      int mYear;
      DoubleAnnual mDAwb;
      DoubleAnnual mDAwbold;
      DoubleAnnual mDAfq;
      DoubleAnnual mFAbi;
      double mWb;
      double mWbold;
      double mFq;
      double mBi;
      void setupdata();
      bool check();
};
