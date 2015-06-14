// $Id: CWagebaseDialog.h,v 1.3 2007/11/07 13:07:00 djolson Exp $
//  CWagebaseDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CWagebaseDialog : public LDialogBox
{
   public:
      enum { class_ID = 'WgbD' };
      
      static CWagebaseDialog * CreateWagebaseDialogStream(
         LStream * inStream );
      CWagebaseDialog( LStream * inStream );
      ~CWagebaseDialog();
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
      double mOlwb;
      double mPlwb;
      int mStartyear;
      int mLastyear;
      int mYear;
      DoubleAnnual mDAplwb;
      DoubleAnnual mDAolwb;
      void setupdata();
      bool check();
};
