// $Id: CDisabilityDialog.h,v 1.6 2010/08/16 18:18:06 djolson Exp $
//  CDisabilityDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"
#include "wrkrdata.h"

class CDisabilityDialog : public LDialogBox
{
   public:
      enum { class_ID = 'DisD' };
      
      static CDisabilityDialog * CreateDisabilityDialogStream(
         LStream * inStream );
      CDisabilityDialog( LStream * inStream );
      ~CDisabilityDialog();
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
      boost::gregorian::date mOnset;  // date of onset
      DateMoyr mWaitper;  // first month of waiting period
      DateMoyr mPriorent;  // date of prior entitlement
      DateMoyr mDicess;  // date of cessation
      float mPia; // cessation pia
      float mMfb; // cessation mfb
      boost::gregorian::date mOnset1;  // date of onset, prior period
      DateMoyr mWaitper1;  // first month of waiting period, prior period
      DateMoyr mPriorent1;  // date of prior entitlement, prior period
      DateMoyr mDicess1;  // date of cessation, prior period
      float mPia1; // cessation pia
      float mMfb1; // cessation mfb
      int mValdi;  // number of periods of disability
      WorkerData::ben_type mIoasdi;  // 1 for old-age, 2 for survivor, 3 for disability
      boost::gregorian::date mBirthDate;  // date of birth
      // date of entitlement
      DateMoyr mEntDate;
      // date of benefit
      DateMoyr mBenDate;
      // date of entitlement
      boost::gregorian::date mDeathDate;
};
