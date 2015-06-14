// $Id: CHistAmtUpdateDialog.h,v 1.4 2008/10/31 12:51:40 djolson Exp $
//  CHistAmtUpdateDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CHistAmtUpdateDialog : public LDialogBox
{
   public:
      enum { class_ID = 'HauD' };
      
      static CHistAmtUpdateDialog * CreateHistAmtUpdateDialogStream(
         LStream * inStream );
      CHistAmtUpdateDialog( LStream * inStream );
      ~CHistAmtUpdateDialog();
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
      int mYear;
      double mWb;
      double mWbold;
      double mFq;
      double mBi;
};
