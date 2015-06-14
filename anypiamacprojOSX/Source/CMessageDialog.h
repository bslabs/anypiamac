// $Id: CMessageDialog.h,v 1.3 2007/11/07 13:06:58 djolson Exp $
//  CMessageDialog.h

#pragma once

#include <LDialogBox.h>

class CMessageDialog : public LDialogBox
{
   public:
      enum { class_ID = 'MsgD' };
      
      static CMessageDialog * CreateMessageDialogStream(
         LStream * inStream );
      CMessageDialog( LStream * inStream );
      ~CMessageDialog();
      virtual void FindCommandStatus( CommandT inCommand,
         Boolean &outEnabled, Boolean &outUsesMark,
         UInt16 &outMark, Str255 outName );
      virtual void ListenToMessage( MessageT inMessage, void * ioParam );
      void SaveResults();
      virtual void SetupDialog();
   private:
      virtual void FinishCreateSelf();
   public:
      unsigned int msgNum;  // message number
      const char *mString;  // message
};

// Global function to create a message dialog
void messageDialog( unsigned int number, LCommander* inSuperCommander );
void messageDialog( const char *message, LCommander* inSuperCommander );
