// $Id: CCatchupDialog.h,v 1.4 2008/10/31 12:51:40 djolson Exp $
//  CCatchupDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CCatchupDialog : public LDialogBox
{
   public:
      enum { class_ID = 'CatD' };
      
      static CCatchupDialog * CreateCatchupDialogStream(
         LStream * inStream );
      CCatchupDialog( LStream * inStream );
      ~CCatchupDialog();
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
      double mCuyear1;
      double mCuyear2;
      double mCuyear3;
      double mCuyear4;
      double mCuyear5;
      double mCuyear6;
      double mCuyear7;
      double mCuyear8;
      Catchup mCachup;
      int mEligyear;
      bool check();
      void setupdata();
};
