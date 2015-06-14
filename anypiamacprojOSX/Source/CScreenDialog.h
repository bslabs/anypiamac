// $Id: CScreenDialog.h,v 1.3 2007/11/07 13:06:59 djolson Exp $
//  CScreenDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CScreenDialog : public LDialogBox
{
   public:
      enum { class_ID = 'ScnD' };
      
      static CScreenDialog * CreateScreenDialogStream(
         LStream * inStream );
      CScreenDialog( LStream * inStream );
      ~CScreenDialog();
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
      int mScreenPoints;
};
