// $Id: CPebesDialog.h,v 1.4 2007/11/07 13:06:59 djolson Exp $
//  CPebesDialog.h

#pragma once

#include <LDialogBox.h>
#include <LGroupBox.h>
#include "CTextDocument.h"

class CPebesDialog : public LDialogBox
{
   public:
      enum { class_ID = 'PbsD' };
      
      static CPebesDialog * CreatePebesDialogStream(
         LStream * inStream );
      CPebesDialog( LStream * inStream );
      ~CPebesDialog();
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
      int mAgeplan;  // planned age of retirement
      int mBeginpebes;  // first year of earnings
      int mMonth;  // month of calculation
      int mBirthyear;  // year of birth
      int mIstart;  // current year
      // @cmember Amount of monthly noncovered pension.
      double mNonpen;
      // Date of entitlement to noncovered pension.
      DateMoyr mNonpendate;
      boost::gregorian::date mBirthdate;
      boost::gregorian::date mDeathdate;
};
