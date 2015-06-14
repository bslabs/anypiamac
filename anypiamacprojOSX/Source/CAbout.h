// $Id: CAbout.h,v 1.3 2007/11/07 13:06:56 djolson Exp $
//  CAbout.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CAbout : public LDialogBox
{
   public:
      enum { class_ID = 'AboD' };
      
      static CAbout * CreateAboutStream(
         LStream * inStream );
      CAbout( LStream * inStream );
      ~CAbout();
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
};
