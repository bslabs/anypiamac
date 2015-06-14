// $Id: CCusetDialog.h,v 1.4 2008/10/31 12:51:40 djolson Exp $
//  CCusetDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CCusetDialog : public LDialogBox
{
   public:
      enum { class_ID = 'StcD' };
      
      static CCusetDialog * CreateCusetDialogStream(
         LStream * inStream );
      CCusetDialog( LStream * inStream );
      ~CCusetDialog();
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
      int mAlt;  // alternative number (1-4)
      bool check();
      void setupdata();
};
