// $Id: CIntroduction.h,v 1.3 2007/11/07 13:06:58 djolson Exp $
//  CIntroduction.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CIntroduction : public LDialogBox
{
   public:
      enum { class_ID = 'IntD' };
      
      static CIntroduction * CreateIntroductionStream(
         LStream * inStream );
      CIntroduction( LStream * inStream );
      ~CIntroduction();
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
      bool mNointro;
};
