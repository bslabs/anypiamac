// $Id: CWorkerInfoDialog.h,v 1.5 2010/08/16 18:18:07 djolson Exp $
//  CWorkerInfoDialog.h

#pragma once

#include <LDialogBox.h>
#include <LGroupBox.h>
#include "CTextDocument.h"
#include "wrkrdata.h"

class CWorkerInfoDialog : public LDialogBox
{
   public:
      enum { class_ID = 'WkrD' };
      
      static CWorkerInfoDialog * CreateWorkerInfoDialogStream(
         LStream * inStream );
      CWorkerInfoDialog( LStream * inStream );
      ~CWorkerInfoDialog();
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
      int mSex;  // sex of worker (1 for male, 2 for female)
      WorkerData::ben_type mJoasdi;  // type of benefit (1 for old-age,
         // 2 for survivor, 3 for disability, 4 for PEBES)
      Boolean mTotalize;  // true if totalization benefit
      Boolean mIndearn;  // true if any regular OASDI earnings
      Boolean mRailroad;  // true if any railroad earnings
      Boolean mIndMS;  // true if any military service
      Boolean mMqge;  // true if any Medicare-only earnings
      Boolean mBlind;  // true if worker is statutorily blind
      Boolean mDeemed;  // true if deemed insured provision applies
      boost::gregorian::date mDob;  // date of birth
      boost::gregorian::date mDeath;  // date of death
      int mRRYear1;  // first railroad year
      int mRRYear2;  // last railroad year
      int mMSCount;  // number of periods of military service
      int mFammem;  // number of family members
};
