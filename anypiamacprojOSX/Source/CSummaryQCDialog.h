// $Id: CSummaryQCDialog.h,v 1.4 2007/11/07 13:06:59 djolson Exp $
//  CSummaryQCDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CSummaryQCDialog : public LDialogBox
{
   public:
      enum { class_ID = 'SQCD' };
      
      static CSummaryQCDialog * CreateSummaryQCDialogStream(
         LStream * inStream );
      CSummaryQCDialog( LStream * inStream );
      ~CSummaryQCDialog();
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
      int mQc51td;
      int mQctd;
      bool mbQc51td;
      bool mQcann;
};
