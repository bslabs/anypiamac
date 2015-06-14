// $Id: CAwsetDialog.h,v 1.3 2007/11/07 13:06:56 djolson Exp $
//  CAwsetDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CAwsetDialog : public LDialogBox
{
   public:
      enum { class_ID = 'StaD' };
      
      static CAwsetDialog * CreateAwsetDialogStream(
         LStream * inStream );
      CAwsetDialog( LStream * inStream );
      ~CAwsetDialog();
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
      double mAwyear1;
      double mAwyear2;
      double mAwyear3;
      double mAwyear4;
      double mAwyear5;
      double mAwyear6;
      double mAwyear7;
      double mAwyear8;
      double mAwyear9;
      double mAwyear10;
      double mAwyear11;
      double mAwult;
      DoubleAnnual mAwproj;  // projected average wage increases
      std::string mAwprojTitle;
      int mAlt;  // alternative number (1-4)
};
