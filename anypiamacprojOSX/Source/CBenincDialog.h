// $Id: CBenincDialog.h,v 1.4 2008/10/31 12:51:40 djolson Exp $
//  CBenincDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CBenincDialog : public LDialogBox
{
   public:
      enum { class_ID = 'BenD' };
      
      static CBenincDialog * CreateBenincDialogStream(
         LStream * inStream );
      CBenincDialog( LStream * inStream );
      ~CBenincDialog();
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
      double mBiyear1;
      double mBiyear2;
      double mBiyear3;
      double mBiyear4;
      double mBiyear5;
      double mBiyear6;
      double mBiyear7;
      double mBiyear8;
      double mBiyear9;
      double mBiyear10;
      double mBiult;
      DoubleAnnual mBiproj;  // projected benefit increases
};
