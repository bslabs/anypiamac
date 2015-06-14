// $Id: CConfigDialog.h,v 1.3 2007/11/07 13:06:57 djolson Exp $
//  CConfigDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CConfigDialog : public LDialogBox
{
   public:
      enum { class_ID = 'CnfD' };
      
      static CConfigDialog * CreateConfigDialogStream(
         LStream * inStream );
      CConfigDialog( LStream * inStream );
      ~CConfigDialog();
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
      float mLeftmargin;
      float mTopmargin;
      int mPrinterPoints;
      bool longOutput;
};
