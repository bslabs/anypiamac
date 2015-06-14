// $Id: CMilservDialog.h,v 1.4 2010/08/16 18:18:07 djolson Exp $
//  CMilservDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"
#include "msdates.h"

class CMilservDialog : public LDialogBox
{
   public:
      enum { class_ID = 'MlsD' };
      
      static CMilservDialog * CreateMilservDialogStream(
         LStream * inStream );
      CMilservDialog( LStream * inStream );
      ~CMilservDialog();
      virtual void FindCommandStatus( CommandT inCommand,
         Boolean &outEnabled, Boolean &outUsesMark,
         UInt16 &outMark, Str255 outName );
      virtual void ListenToMessage( MessageT inMessage, void * ioParam );
      void SaveResults();
      void setdata( int index, std::string startDate, std::string endDate );
      virtual void SetupDialog();
   private:
      virtual void FinishCreateSelf();
      std::string getTextBoxString( long textBoxID );
   public:
      CTextDocument * theDoc;
      MilServDates msdates[15];
      int mMSCount;
};
