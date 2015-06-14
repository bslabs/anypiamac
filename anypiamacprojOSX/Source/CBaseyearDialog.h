// $Id: CBaseyearDialog.h,v 1.3 2007/11/07 13:06:56 djolson Exp $
//  CBaseyearDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CBaseyearDialog : public LDialogBox
{
   public:
      enum { class_ID = 'CmpD' };
      
      static CBaseyearDialog * CreateBaseyearDialogStream(
         LStream * inStream );
      CBaseyearDialog( LStream * inStream );
      ~CBaseyearDialog();
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
      int mBaseyear;
      int mLastyear;
};
