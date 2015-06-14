// $Id: CSpecYearDialog.h,v 1.3 2007/11/07 13:06:59 djolson Exp $
//  CSpecYearDialog.h

#pragma once

#include <LDialogBox.h>
#include "CAnnearnDialog.h"

class CSpecYearDialog : public LDialogBox
{
   public:
      enum { class_ID = 'SpcD' };
      
      static CSpecYearDialog * CreateSpecYearDialogStream(
         LStream * inStream );
      CSpecYearDialog( LStream * inStream );
      ~CSpecYearDialog();
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
      CAnnearnDialog * theEarningsDialog;
      int mYear;  // selected year
      int mStartyear;  // first possible year
      int mLastyear;  // last possible year
};
